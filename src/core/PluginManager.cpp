//=======================================================================
// Authors: Donovan Parks & Timothy Mankowski
//
// Copyright 2009 Donovan Parks & Timothy Mankowski
//
// This file is part of GenGIS.
//
// GenGIS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GenGIS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GenGIS.  If not, see <http://www.gnu.org/licenses/>.
//=======================================================================

#include "../core/Precompiled.hpp"

// Don't use this code in Windows debug builds!
#ifndef _DEBUG

#include "../core/PluginManager.hpp"

#include "../core/App.hpp"

#include "../gui/GenGisFrame.hpp"
#include "../gui/SelectorWindowDlg.hpp"
#include "../gui/PluginManagerDlg.hpp"
#include "../gui/WaitingDlg.hpp"

#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"

using namespace GenGIS;

const int ID_PLUGIN_MANAGER = 20000;
const int ID_PLUGINS        = 20001;

wxFrame* PluginManager::m_mainWindow;
wxMenu*  PluginManager::m_mnuPlugins;

std::vector<std::string> PluginManager::m_fields;
std::vector< std::vector<std::string> > PluginManager::m_pluginMetadata;

std::map<uint, std::string> PluginManager::m_objectIdToPluginModuleName;
std::map<uint, bool> PluginManager::m_objectIdToRequiresR;
std::map<uint, std::string> PluginManager::m_objectIdToRDependencies;

PluginManager::~PluginManager()
{
  //m_mainWindow->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PluginManager::OnPluginsPluginManager ) );
}

void PluginManager::LoadPlugins()
{
	// specify plugin metadata fields
	m_fields.push_back("Plugin");
	m_fields.push_back("Version");
	m_fields.push_back("Description");
	m_fields.push_back("Author(s)");
	m_fields.push_back("Publication date");
	m_fields.push_back("Minimum GenGIS\n version");
	m_fields.push_back("Requires R");
	m_fields.push_back("R Dependencies\n (R packages)");

	// get plugin menu
	wxMenuBar* menuBar = m_mainWindow->GetMenuBar();	
	m_mnuPlugins = menuBar->GetMenu(menuBar->FindMenu(wxT("Plugins")));

	// add plugins directory to python path
	#ifdef WIN32
		wxString gengisDir        = App::Inst().GetExeDir();
		gengisDir                 = gengisDir.BeforeLast('\\').BeforeLast('\\');
		wxString pluginPath       = gengisDir + wxT("\\plugins");
		std::string pluginPathStr = "sys.path.insert(0,r\"" + std::string(pluginPath.mb_str())+ "\")";
		PyRun_SimpleString(pluginPathStr.c_str());
	#else
		//wxString pluginPath = wxT("./GenGIS.app/Contents/Resources/Plugins");
		//PyRun_SimpleString("sys.path.insert(0,\"./GenGIS.app/Contents/Resources/Plugins\")");
		wxString pluginPath = wxT("./Plugins");
		PyRun_SimpleString("sys.path.insert(0,\"./Plugins\")");
	#endif

	// add library directory to Python path
	#ifdef WIN32
		wxString libraryPath       = gengisDir + wxT("\\library");
		std::string libraryPathStr = "sys.path.insert(0,r\"" + std::string(libraryPath.mb_str())+ "\")";
		PyRun_SimpleString(libraryPathStr.c_str());
	#else
		wxString libraryPath = wxT("./Library");
		PyRun_SimpleString("sys.path.insert(0,\"./Library\")");
	#endif

	// get all plugins
	wxArrayString files;
	wxDir::GetAllFiles(pluginPath, &files, wxT("__init__.py"));

	for(uint i = 0; i < files.Count(); ++i)
	{
		std::vector<std::string> pluginMetadata;
        
		// get plugin module
		//wxString pluginModuleName = files[i].Mid(0, files[i].Find(wxT('\\'), true));
		//pluginModuleName = pluginModuleName.Mid(pluginModuleName.Find(wxT('\\'), true)+1);
		wxFileName pluginFileName( files[i] );
		wxString pluginModuleName = pluginFileName.GetDirs().Last();
		std::string pluginModuleNameStr = std::string( pluginModuleName.mb_str() );

		PyObject* myModuleString = PyString_FromString(pluginModuleNameStr.c_str());
		PyObject* myModule = PyImport_Import(myModuleString);

		// get plugin name
		PyObject* myFunction = PyObject_GetAttrString(myModule,(char*)"name");
		PyObject* myResult = PyObject_CallObject(myFunction, 0);
		std::string pluginName = PyString_AsString(myResult);
		pluginMetadata.push_back(pluginName.c_str());

		// get version
		myFunction = PyObject_GetAttrString(myModule,(char*)"version");
		myResult = PyObject_CallObject(myFunction, 0);
		pluginMetadata.push_back(PyString_AsString(myResult));

		// get description
		myFunction = PyObject_GetAttrString(myModule,(char*)"description");
		myResult = PyObject_CallObject(myFunction, 0);
		std::string pluginDesc = PyString_AsString(myResult);
		pluginMetadata.push_back(pluginDesc);

		// get authors
		myFunction = PyObject_GetAttrString(myModule,(char*)"authors");
		myResult = PyObject_CallObject(myFunction, 0);
		pluginMetadata.push_back(PyString_AsString(myResult));

		// get publication date
		myFunction = PyObject_GetAttrString(myModule,(char*)"publicationDate");
		myResult = PyObject_CallObject(myFunction, 0);
		pluginMetadata.push_back(PyString_AsString(myResult));

		// get minimum required version of GenGIS
		myFunction = PyObject_GetAttrString(myModule,(char*)"minimumVersionOfGenGIS");
		myResult = PyObject_CallObject(myFunction, 0);
		pluginMetadata.push_back(PyString_AsString(myResult));

		// check if plugin required R
		myFunction = PyObject_GetAttrString(myModule,(char*)"requireR");
		myResult = PyObject_CallObject(myFunction, 0);
		bool bRequireR = PyInt_AsLong(myResult);
		if(bRequireR)
			pluginMetadata.push_back("True");
		else
			pluginMetadata.push_back("False");

		// get R dependencies
		if (bRequireR)
		{
			myFunction = PyObject_GetAttrString(myModule,(char*)"rDependencies");
			myResult = PyObject_CallObject(myFunction, 0);
			pluginMetadata.push_back(PyString_AsString(myResult));
			m_objectIdToRDependencies[ID_PLUGINS+i] = PyString_AsString(myResult);
		}
		else
		{
			pluginMetadata.push_back( "-" );
		}

		m_pluginMetadata.push_back(pluginMetadata);

		wxString menuName = wxString( pluginName.c_str(), wxConvUTF8 );
		if (bRequireR)
			menuName += wxT( " (R required)" );

		// add plugin to plugin menu
		wxMenuItem* pluginMenu = new wxMenuItem( m_mnuPlugins, ID_PLUGINS+i, 
		                                         menuName, 
		                                         wxString(pluginDesc.c_str(), wxConvUTF8),
		                                         wxITEM_NORMAL );
		m_mnuPlugins->Append(pluginMenu);
		m_mainWindow->Connect( pluginMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PluginManager::OnExecutePlugin ) );
	
		// add to plugin map
		m_objectIdToPluginModuleName[ID_PLUGINS+i] = pluginModuleNameStr;
		m_objectIdToRequiresR[ID_PLUGINS+i] = bRequireR;
	}
	
	// add plugin manager to plugin menu
	m_mnuPlugins->InsertSeparator(m_pluginMetadata.size());
	wxMenuItem* mnuPluginManager = new wxMenuItem( m_mnuPlugins, ID_PLUGIN_MANAGER, 
	                                               wxString(wxT("Plugin Manager")) + wxT('\t') + wxT("Ctrl+P"), 
	                                               wxT("Information and options for plugins"), 
	                                               wxITEM_NORMAL );
	m_mnuPlugins->Append(mnuPluginManager);
	m_mainWindow->Connect( mnuPluginManager->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PluginManager::OnPluginsPluginManager ) );

	// define function for executing plugins directly from the Python console
	std::string commands = "def RunPlugin(pluginModuleName):\n";
	commands += "  modStr = pluginModuleName + '.' + pluginModuleName\n";
	commands += "  theModule = __import__(modStr, fromlist='*')\n";
	commands += "  theClass = getattr(theModule, pluginModuleName)\n";
	commands += "  theObject = theClass(wx.GetTopLevelWindows()[0])\n";
	commands += "  theObject.Show()";
	PyRun_SimpleString(commands.c_str());

	#ifndef WIN32
	((GenGisFrame*)m_mainWindow)->SetRAutoConfigUtilStatus( false );
	#endif
}

void PluginManager::OnPluginsPluginManager( wxCommandEvent& event )
{
	PluginManagerDlg* dlg = new PluginManagerDlg(m_mainWindow);
	dlg->PopulateTable(m_fields, m_pluginMetadata);
	dlg->Show();
}

void PluginManager::OnExecutePlugin( wxCommandEvent& event )
{
	std::string moduleName = m_objectIdToPluginModuleName[event.GetId()];
	bool bRequire = m_objectIdToRequiresR[event.GetId()];

	if(bRequire)
	{
		// perform a quick test to see if R is installed and configured
		if ( !ConfigureRInstallation() )
			return;

		// Check whether all dependent R packages for the plugin are installed
		if ( !CheckPluginRDependencies( event.GetId() ) )
			return;
	}

	std::string runPluginCommand = "RunPlugin('" + moduleName + "')";
	runPluginCommand = "shell.run(\"" + runPluginCommand + "\")";

	bool bError = PyRun_SimpleString(runPluginCommand.c_str());
	if(bError)
	{
		Log::Inst().Write("Error executing plugin.");
		PyErr_Print();
	}
}

bool PluginManager::CheckPluginRDependencies( int pluginId )
{
	// Retrieve list of R dependencies for this plugin
	std::string rDependencies = m_objectIdToRDependencies[ pluginId ];

	// Return if there are no R dependencies
	if ( rDependencies.empty() )
		return true;

	// Tokenize the dependency list and place the result into a vector
	std::vector<std::string> dependencyList;
	boost::split( dependencyList, rDependencies, boost::is_any_of( "," ) );

	// Iterate through the dependency list
	std::vector<std::string>::iterator it;
	for ( it = dependencyList.begin(); it < dependencyList.end(); it++ )
	{
		// Check whether the R package is installed
		PyRun_SimpleString( "import rpy2.robjects as robjects" );
		std::string pythonCommand = "robjects.r('library(" + *it + ")')";
		bool bError = PyRun_SimpleString( pythonCommand.c_str() );

		// Move to the next R package dependency if this package is already installed
		if ( !bError )
			continue;

		// Prompt the user on whether to retrieve and install the R package
		wxString rDependencies_wx( it->c_str(), wxConvUTF8 );
		int answer = wxMessageBox(
			wxT( "The \"") + rDependencies_wx + wxT( "\" R package is missing and is required by this plugin." )
			wxT( "\n\nWould you like GenGIS to retrieve it?"),
			wxT( "R dependency missing" ), wxYES_NO );

		if ( answer == wxNO )
			return false;

		// Retrieve and install R package
		pythonCommand = "robjects.r('a <- \"" + *it + "\"')";
		if ( PyRun_SimpleString( pythonCommand.c_str() ) != 0 )
		{
			// Unable to complete the R configuration
			wxMessageBox( wxT( "GenGIS could not complete the R configuration." ),
			wxT( "GenGIS" ), wxOK | wxICON_ERROR );
			return false;
		}

		#ifdef WIN32
		pythonCommand = "robjects.r('if (!is.element(a,installed.packages()[,1])) install.packages(a)')";
		#else
		pythonCommand = "robjects.r('if (!is.element(a,installed.packages()[,1])) install.packages(a, repos = \"http://mirror.its.dal.ca/cran/\")')";
		#endif

		if ( PyRun_SimpleString( pythonCommand.c_str() ) != 0 )
		{
			// Unable to complete the R configuration
			wxMessageBox( wxT( "GenGIS could not complete the R configuration." ),
			wxT( "GenGIS" ), wxOK | wxICON_ERROR );
			return false;
		}
	}
	return true;
}

bool PluginManager::ConfigureRInstallation()
{
	#ifdef WIN32
	bool isRunningWindowsXP = false;

	// Determine whether the user is running Windows XP
	// Only XP users require additional Path configuration
	OSVERSIONINFO osvi;
	ZeroMemory( &osvi, sizeof( OSVERSIONINFO ) );
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &osvi );

	// Windows XP (5.1) Only 
	if ( ( osvi.dwMajorVersion == 5 ) && ( osvi.dwMinorVersion >= 1 ) )
		isRunningWindowsXP = true;

	bool bError = PyRun_SimpleString("from rpy2 import robjects");

	// Performing checks for Windows Vista and 7
	if ( !isRunningWindowsXP )
	{
		// If R is installed
		if ( !bError )
			return true;

		// If R is installed but needs DLLs transferred
		return TransferDllFilesR();
	}

	// Performing checks for Windows XP
	if ( bError )
	{
		if ( !TransferDllFilesR() )
			return false;
	}

	//wxMessageBox( wxT( "Checking the PATH variable." ), wxT( "GenGIS" ), wxOK );
	WaitingDlg pathChecker( NULL, wxT( "Checking PATH." ) );
	pathChecker.Show();
	pathChecker.Update();
	Sleep( 500 );

	// Get R Path version from RPY
	std::wstring   rPathString  = GetPathToR();
	const wchar_t* rPathStringW = rPathString.c_str();

	return SetRPath( rPathStringW );

	#else
	return true;
	#endif
}

std::wstring PluginManager::GetPathToR()
{
	#ifdef WIN32
	wchar_t folderPath[MAX_PATH + 1] = {0};

	// Locate the Windows installation root directory letter (usually this is 'C:')
	ExpandEnvironmentStrings( TEXT( "%HOMEDRIVE%" ), folderPath, ARRAYSIZE( folderPath ) );

	// Check if the system root directory was found, if not return NULL
	if ( GetLastError() != 0 )
		return NULL;

	// Append "\Program Files\R\R-"
	wcscat( folderPath, TEXT( "\\Program Files\\R\\R-" ) );

	// Get R version from RPY
	std::wstring rVerString = GetRVersion();

	if ( rVerString.empty() )
		return NULL;

	const wchar_t* rVerStringW = rVerString.c_str();

	wcscat( folderPath, rVerStringW );
	wcscat( folderPath, TEXT( "\\bin\\" ) );

	return std::wstring( folderPath );
	#else
	return NULL;
	#endif
}

bool PluginManager::TransferDllFilesR()
{
	#ifdef WIN32
	wchar_t folderPath[MAX_PATH + 1] = {0}, copySource[MAX_PATH + 1] = {0}, copyDestination[MAX_PATH + 1] = {0};

	// Locate the Windows installation root directory letter (usually this is 'C:')
	ExpandEnvironmentStrings( TEXT( "%HOMEDRIVE%" ), folderPath, ARRAYSIZE( folderPath ) );

	// Check if the system home directory was found, if not return false
	if ( GetLastError() != 0 )
		return false;

	// Append "\Program Files\R\"
	wcscat( folderPath, TEXT( "\\Program Files\\R\\" ) );

	// Check if the 'R' directory exists in the "Program Files" directory
	if ( _waccess( folderPath, 0 ) != 0 )
	{
		wxMessageBox(
			wxT( "GenGIS could not locate an R installation on your system.")
			wxT("\nPlease install R to continue." ),
			wxT( "GenGIS" ), wxOK );
		return false;
	}

	// Append an asterix (wildcard) to search all files/directories
	wcscat( folderPath, TEXT( "*" ) );

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile( folderPath, &FindFileData );

	if ( hFind == INVALID_HANDLE_VALUE )
	{
		wxMessageBox(
			wxT( "GenGIS could not locate an R installation on your system.")
			wxT("\nPlease install R to continue." ),
			wxT( "GenGIS" ), wxOK );
		return false;
	}

	// Locate R installation directories (e.g., "R-2.14.1")
	wxArrayString fileList;
	do {
		// Ignore the '.' and ".." directories, and any non-directories
		if ( ( wcscmp( FindFileData.cFileName, TEXT( "." ) )  != 0 ) &&
			 ( wcscmp( FindFileData.cFileName, TEXT( ".." ) ) != 0 ) &&
			 ( FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY ) )
		{
			fileList.Add( wxString( FindFileData.cFileName ) );
		}
	} while ( FindNextFile( hFind, &FindFileData ) );

	// Create a listing of R versions for the user to select
	SelectorWindowDlg *fileSelector = new SelectorWindowDlg( NULL, fileList );
	fileSelector->SetTitle( wxT( "Select R most recent version" ) );
	fileSelector->SetLabel( wxT( "Please select the most recent version of R from the list:" ) );
	fileSelector->ShowModal();

	wxString selection = fileSelector->GetSelection();
	if ( selection == wxEmptyString )
		return false;

	// Remove the asterix from the end of folderPath
	folderPath[wcslen( folderPath ) - 1] = '\0';

	// Set folderPath to point to the correct R version
	wcscat( folderPath, selection.wc_str() );

	// Set up source and destination paths
	wcscpy( copySource, folderPath );
	wcscat( copySource, TEXT( "\\bin\\i386\\" ) );

	wcscpy( copyDestination, folderPath );
	wcscat( copyDestination, TEXT( "\\bin\\" ) );

	// Check whether the source "i386" directory exists
	if ( _waccess( copySource, 0 ) != 0 )
	{
		// Unable to locate the "i386" directory
		wxMessageBox( wxT( "GenGIS could not complete the search operation." ),
			wxT( "GenGIS" ), wxOK | wxICON_ERROR );
		return false;
	}

	// Prompt the user whether to transfer the DLLs
	int answer = wxMessageBox(
		wxT("GenGIS needs to configure R in order to use plugins.")
		wxT("\n\nThis will not hurt your current R installation, however,")
		wxT("\nthe existing R installation will be modified.")
		wxT("\n\nAll *.dll files from:\n\t\"") + wxString( copySource ) +
		wxT("\"\nwill be copied to:\n\t\"") + wxString( copyDestination ) +
		wxT("\"\n\nFor more information please see:")
		wxT("\nhttp://kiwi.cs.dal.ca/GenGIS/The_GenGIS_2.1_Manual#R_and_GenGIS")
		wxT("\n\nWould you like GenGIS to configure R?"),
		wxT("R Configuration Required (Please Read)"), wxYES_NO );

	if ( answer == wxNO )
		return false;

	// Append wildcard to copy all DLLs
	wcscat( copySource, TEXT( "*.dll" ) );

	SHFILEOPSTRUCT FileOp;
	FileOp.hwnd   = NULL;
	FileOp.wFunc  = FO_COPY;
	FileOp.pFrom  = copySource;
	FileOp.pTo    = copyDestination;
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;

	// Transfer the DLLs
	int fileCopyOutcome = SHFileOperation( &FileOp );

	// Return true if the copy operation is successful
	if ( fileCopyOutcome == 0 )
		return true;

	// If all else fails, return false
	wxMessageBox( wxT( "Could not complete R configuration." ), wxT("GenGIS"), wxOK | wxICON_ERROR );
	return false;

	#else
	return true;
	#endif
}

bool PluginManager::SetRPath( const wchar_t* rPath )
{
	#ifdef WIN32

	if ( rPath == NULL )
		return false;

	// Add the R directory to the Windows "Path" environment variable
	HKEY hKey;

	wchar_t temp[10000] = {0};
	DWORD dwBufLen;
	dwBufLen = sizeof(temp);

	// Open a handler to the location in the Registry where the "Path" variable is found
	LPCWSTR registrySubKey       = TEXT("System\\CurrentControlSet\\Control\\Session Manager\\Environment");
	DWORD   registryAccessRights = KEY_READ | KEY_SET_VALUE;

	if ( RegOpenKeyEx( HKEY_LOCAL_MACHINE, registrySubKey, 0, registryAccessRights, &hKey) != ERROR_SUCCESS )
	{
		wxMessageBox( wxT( "Could not set path. (Error 1)" ), wxT( "GenGIS" ), wxOK | wxICON_ERROR );
		return false;
	}

	// Locate the registry key called "Path" abd grab its value
	LPCWSTR variableName = TEXT( "Path" );
	if ( RegQueryValueEx( hKey, variableName, NULL, NULL, (LPBYTE)&temp, &dwBufLen ) != ERROR_SUCCESS )
	{
		wxMessageBox( wxT( "Could not set path. (Error 2)" ), wxT( "GenGIS" ), wxOK | wxICON_ERROR );
		return false;
	}

	// Check whether the R directory is correctly set in the Path
	if ( wcswcs( temp, rPath ) != NULL )
		return true;

	// Prompt the user to add R to the Path
	int answer = wxMessageBox(
		wxT( "GenGIS requires R to be included in the Windows Path variable." )
		wxT( "\nAdd R to the Path?" ),
		wxT("GenGIS"), wxYES_NO );

	if ( answer == wxNO )
		return false;

	// Check whether the path ends in a semicolon; append if missing
	if ( temp[wcslen( temp ) - 1] != TEXT( ';' ) )
		wcscat( temp, TEXT( ";" ) );

	wcscat( temp, rPath );
	wcscat( temp, TEXT( ";" ) );

	// Add the new Path value to the Registry
	if ( RegSetValueEx( hKey, TEXT("Path"), NULL, REG_EXPAND_SZ,(LPBYTE)temp, wcslen(temp)*sizeof(wchar_t) + 2 ) != ERROR_SUCCESS)
	{
		wxMessageBox( wxT( "Could not set path. (Error 3)" ), wxT( "GenGIS" ), wxOK | wxICON_ERROR );
		return false;
	}

	RegCloseKey( hKey );

	// Propagate new environment variables to the system
	if ( SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_ABORTIFHUNG, 5000, NULL) == 0)
	{
		wxMessageBox(
			wxT( "R configuration was successful.")
			wxT( "\nPlease restart Windows for changes to take effect." ),
			wxT( "GenGIS" ), wxOK );
		return false;
	}

	wxMessageBox( wxT( "Please restart Windows for changes to take effect." ), wxT( "GenGIS" ), wxOK );
	#endif
	return true;
}

std::wstring PluginManager::GetRVersion()
{
	PyObject *module;
	PyObject *dictionary;
	PyObject *result;

	bool bError = PyRun_SimpleString(
		"import rpy2.robjects as robjects\n"
		//"from rpy2 import robjects\n"
		//"import rpy2.robjects\n"
		"a08bf52516fc03 = robjects.r('a<-paste(R.Version()$major, R.Version()$minor, sep=\".\")')\n"
		"result = a08bf52516fc03[0]\n" );

	if ( bError )
	{
		wxMessageBox( wxT( "(Error 2)" ), wxT( "GenGIS" ), wxOK | wxICON_ERROR );
		return NULL;
	}

	module     = PyImport_AddModule( "__main__" );
	dictionary = PyModule_GetDict( module );
	result     = PyMapping_GetItemString( dictionary, "result" );

	if ( result != NULL )
	{
		char *cResult = PyString_AsString( result );
		return std::wstring( cResult, cResult + strlen( cResult ) );
	}
	else
		return NULL;
}

#endif //_DEBUG