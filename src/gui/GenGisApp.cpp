//=======================================================================
// Author: Donovan Parks
//
// Copyright 2009 Donovan Parks
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

#include "../gui/GenGisApp.hpp"
#include "../gui/GenGisFrame.hpp"
#include "../gui/WelcomeDlg.hpp"

#include "../core/App.hpp"

#include "../utils/Log.hpp"

// Don't use this code in Windows debug builds!
#ifndef _DEBUG
	#include "../python/PythonInterface.hpp"
	#include "../core/PluginManager.hpp"
#endif

// disable checked iterators as this is a large performance hit
// (see: http://msdn.microsoft.com/en-us/library/aa985896(VS.80).aspx and google search for SECURE_SCL)
#ifdef NDEBUG
#define _SECURE_SCL 0
#endif

IMPLEMENT_APP(GenGisApp)

bool GenGisApp::OnInit()
{
	try
	{
		// initialize image handlers
		wxImage::AddHandler(new wxPNGHandler);
		wxImage::AddHandler(new wxICOHandler);
		wxImage::AddHandler(new wxXPMHandler);
		//wxImage::AddHandler(new wxGIFHandler);

		GenGIS::Log::Inst().SilentWrite("Program Started.");

		// show splash screen since initialization can take a few seconds
//		wxBitmap bitmap;
//		wxSplashScreen* splash;
//		if(bitmap.LoadFile(GenGIS::App::Inst().GetExeDir() + wxT("images/GenGIS_splash.png"), wxBITMAP_TYPE_PNG))
//		{
//			splash = new wxSplashScreen(bitmap,wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT,
//			                            -1, NULL, -1, wxDefaultPosition, wxDefaultSize,
//			                            wxSIMPLE_BORDER|wxSTAY_ON_TOP);
//		}
//		wxYield();

		// initialize OpenGL fonts
		GenGIS::App::Inst().InitFont(_T("times.ttf"));

		GenGIS::App::Inst().SetResolutionFactor(1);
		GenGIS::App::Inst().SetSavingHiResImage(false);

		GenGIS::Log::Inst().SilentWrite("Fonts initialized.");

		// initial file system handlers
		wxFileSystem::AddHandler(new wxInternetFSHandler);

		GenGIS::Log::Inst().SilentWrite("Internet file system handlers added.");

		// load main window
		GenGIS::GenGisFrame* frame = new GenGIS::GenGisFrame();
		GenGIS::App::Inst().SetMainWindow(frame);

		GenGIS::Log::Inst().SilentWrite("Main window set.");

		// set default database connection string
		//GenGIS::App::Inst().SetDatabaseConnectionString(_T("http://ratite.cs.dal.ca/mg_web/fetch_metagenome_samples.cgi?type=metagenome"));

		// Do not show the Python console in debug builds
		#ifndef _DEBUG
		// Initialize python
		GenGIS::PythonInterface::Inst();

		GenGIS::Log::Inst().SilentWrite("Python console initialized.");

		// Load plugins
		GenGIS::PluginManager::SetMainWindow(frame);
		GenGIS::PluginManager::LoadPlugins();

		GenGIS::Log::Inst().SilentWrite("Python console initialized.");
		#endif

		GenGIS::Log::Inst().SilentWrite("Map controller initialized.");

		// Load main window
		SetTopWindow(frame);
		wxIcon icon(GenGIS::App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO);
		frame->SetIcon(icon);

		GenGIS::App::Inst().LoadUserPreferences();
		GenGIS::Log::Inst().SilentWrite("Preferences loaded.");

		frame->Show();
		GenGIS::Log::Inst().SilentWrite("Frame shown.");

		#ifdef WIN32
		// Handle incoming session files passed as parameters (Windows)
		if ( argc > 1 )
		{
			wxString sessionFilePath( argv[1], wxConvUTF8 );
			( ( GenGIS::GenGisFrame* )GenGIS::App::Inst().GetMainWindow() )->OpenSession( sessionFilePath );
		}
		#endif

		// Remove splash screen
		//splash->Destroy();
//		wxString temp = wxString::Format(wxT("%i"), this->argc);
//		wxMessageBox( temp, temp, wxICON_INFORMATION);
		
		// Show the welcome screen
		if ( ( frame->GetWelcomeDialogStatus() == true ) && ( argc == 1 ) )
		{
			GenGIS::WelcomeDlg* welcomeDlg = new GenGIS::WelcomeDlg( frame, GenGIS::App::Inst().GetExeDir() );
			welcomeDlg->Show();
		}
	}
	catch(...)
	{
		GenGIS::Log::Inst().Error("(Error) Unknown exception.");
	}

	return true;
}

int GenGisApp::OnExit()
{
	return 0;
}

void GenGisApp::MacOpenFile(const wxString &fileName)
{
	if ( GenGIS::App::Inst().IsNewSession() )
	{
		( ( GenGIS::GenGisFrame* )GenGIS::App::Inst().GetMainWindow() )->OpenSession( fileName );
	}
	else
	{
		std::string _fileName = std::string( fileName.mb_str() );
		std::string newSessionCmd = "open -n GenGIS.app " + _fileName;
		system( newSessionCmd.c_str() );
	}
}