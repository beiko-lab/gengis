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

#include "../core/App.hpp"
#include "../core/LayoutObjectProperties.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/LayerTreeView.hpp"
#include "../core/LayerTreeModel.hpp"
#include "../core/StudyLayer.hpp"
#include "../core/UserElementsWrapper.hpp"
#include "../core/ViewportWrapper.hpp"
#include "../core/MapController.hpp"

#include "../gui/GenGisFrame.hpp"
#include "../gui/MainWindowWrapper.hpp"

#include "../utils/Font.hpp"
#include "../utils/ColourMapManager.hpp"
#include "../utils/ShapeMapManager.hpp"

using namespace GenGIS;

template<class Archive> 
void App::serialize(Archive & ar, const unsigned int version)
{
	if ( version != SAVE_VERSION )
	{
		if ( version < SAVE_VERSION )
		{
			wxMessageBox(
				wxT("The session file was created using an older version of GenGIS ")
				wxT("which is incompatible with the current version.")
				wxT("\n\nUnable to open the session file."),
				wxT("Unable to open session file"), wxOK | wxICON_INFORMATION
			);
			Log::Inst().Error("(Error) Session file was created by an older version of GenGIS.");
			exit(1);
		}
		else // version > COMPATIBILITY
		{
			wxMessageBox(
				wxT("The session file was created using a newer version of GenGIS ")
				wxT("which is incompatible with the current version.")
				wxT("\n\nUnable to open the session file"),
				wxT("Unable to open session file."), wxOK | wxICON_INFORMATION
			);
			Log::Inst().Error("(Error) Session file was created by a newer version of GenGIS.");
			exit(1);
		}
		
	}
	//ar & m_mainWindow;            // wxFrame*
	//ar & m_mainWindowWrapper;     // MainWindowWrapperPtr
	//ar & m_viewport;              // ViewportPtr
	//ar & m_viewportWrapper;       // ViewportWrapperPtr
	ar & m_layerTreeController;   // LayerTreeControllerPtr
	ar & m_userElementsWrapper;   // UserElementsWrapperPtr
	ar & m_mode;                  // MODE
	//ar & m_mouseWorldPos;         // Point3D
	//ar & m_mouseScreenPos;        // Point2D
	//ar & m_mouseDragging;         // bool
	ar & m_layoutObjProp;         // LayoutObjectPropertiesPtr
	//ar & m_selectedView;          // ViewPtr
	//ar & m_rightClickView;        // ViewPtr
	//ar & m_colourMapManager;      // ColourMapManagerPtr
	//ar & m_shapeMapManager;       // ShapeMapManagerPtr
	ar & m_font;                  // FontPtr
	//ar & m_dbConnectionString;    // std::wstring
	ar & m_resolutionFactor;      // uint
	//ar & m_bRenderingHiResImage;  // bool
	//ar & m_sessionFullPath;       // wxFileName
	//ar & m_bCurrentlySerializing; // bool
	//ar & m_bSaveIsUpToDate;       // bool
	//ar & m_bIsEmptySession;       // bool
	//ar & m_appVersion;            // wxString
}
template void App::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void App::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

/** Private singleton constructor (must instantiate this class through Inst()). */
App::App():
	m_layoutObjProp( new LayoutObjectProperties() ),
	m_mainWindowWrapper( new MainWindowWrapper() ),
	m_userElementsWrapper( new UserElementsWrapper() ),
	m_viewportWrapper( new ViewportWrapper() ),
	m_dbConnectionString( _T("http://ratite.cs.dal.ca/mg_web/fetch_metagenome_samples.cgi?type=metagenome") ),
	m_sessionFullPath( wxEmptyString ),
	m_bCurrentlySerializing( false ),
	m_bSaveIsUpToDate( true ),
	m_bIsEmptySession( true ),
	//m_appVersion( wxT( "2.0.2" ) )
	m_appVersion( wxT( "2.1.1" ) )
{
	m_colourMapManager.reset(new ColourMapManager());
	std::wstring colourMapPath = (GetExeDir() + wxT("colourMaps")).c_str();
	m_colourMapManager->LoadColourMaps(std::string(colourMapPath.begin(), colourMapPath.end()));

	m_shapeMapManager.reset(new GenGIS::ShapeMapManager());
}

void App::SetMainWindow(wxFrame* window)
{
	m_mainWindow = window;
}

StudyControllerPtr App::GetStudyController() const
{
	return m_layerTreeController->GetStudyLayer(0)->GetStudyController();
}

void App::InitFont(const std::wstring& font)
{
	wxString fontFile = App::Inst().GetExeDir() + wxT("fonts/") + font.c_str();
	m_font.reset(new Font(std::string(fontFile.begin(), fontFile.end()), 12));
}

Colour App::GetSelectionColour() const
{
	return Colour(255, 0, 0);
}

void App::SetSelectedView( ViewPtr view ) 
{ 
	// unselect current view
	if( m_selectedView )
	{
		m_selectedView->SetSelected( false );
	}

	// set new selected view
	m_selectedView = view; 
	if( m_selectedView )
	{
		m_selectedView->SetSelected( true );
	}
}

wxString App::GetUserDataDir() const
{
	wxStandardPaths standardPaths;
	if(!wxDir::Exists(standardPaths.GetUserLocalDataDir()))
	{
		boost::filesystem::create_directory(standardPaths.GetUserLocalDataDir().c_str());
	}

	return standardPaths.GetUserLocalDataDir();
}

wxString App::GetExeDir() const
{
	wxStandardPaths standardPaths;
	wxString exeDir;
	exeDir = standardPaths.GetExecutablePath();
#ifdef WIN32
	// Windows version: if you need to change these lines please contact Donovan first
	exeDir = exeDir.SubString(0, exeDir.find_last_of('\\'));
#else
	// Mac version
	exeDir = exeDir.SubString(0, exeDir.length()-13);
	exeDir = exeDir + wxT("Resources/");
#endif
	return exeDir;
}

void App::SaveUserPreferences()
{
	((GenGisFrame*)App::Inst().GetMainWindow())->SaveUserPreferences();
}

void App::LoadUserPreferences()
{
	((GenGisFrame*)App::Inst().GetMainWindow())->LoadUserPreferences();
}

void App::SetSessionFullPath( wxString sessionFilePath )
{
	m_sessionFullPath.SetPath( sessionFilePath );
}

void App::SetSaveStatus(bool status)
{
	m_bIsEmptySession = false;
	if ( ( m_bSaveIsUpToDate == true ) && ( status == false ) )
	{
		m_mainWindow->SetTitle( m_mainWindow->GetTitle().Prepend( wxT( '*' ) ) );
	}
	else if ( ( m_bSaveIsUpToDate == false) && ( status == true ) )
	{
		wxString windowTitle = m_mainWindow->GetTitle().AfterFirst( '*' );
		if ( !windowTitle.IsEmpty() )
		{
			m_mainWindow->SetTitle( windowTitle );
		}
	}
	m_bSaveIsUpToDate = status;
}
