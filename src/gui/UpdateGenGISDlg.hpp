//=======================================================================
// Author: Timothy Mankowski
//
// Copyright 2011 Timothy Mankowski
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
#ifndef _GENGIS_UPDATE_GENGIS_DLG_
#define _GENGIS_UPDATE_GENGIS_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/UpdateGenGISLayout.h"

namespace GenGIS
{
	/**
	* @brief Shows information about the latest release of GenGIS.
	*/
	class UpdateGenGISDlg : public UpdateGenGISLayout
	{
	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		*/
		explicit UpdateGenGISDlg( wxWindow* parent ) : UpdateGenGISLayout ( parent )
		{
			SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));
			if (! m_htmlUpdateMessage->LoadPage( wxT( "http://kiwi.research.cs.dal.ca/~GenGIS/gengis_dev/description.txt" ) ) )
			{
				m_htmlUpdateMessage->SetPage( wxT( "Could not load information page." ) );
			}
		}

		/**
		* @brief Event handler for the Yes and No buttons.
		* @param event wxCommandEvent event.
		*/
		void OnClose( wxCommandEvent& event )
		{
			if ( m_checkBoxSendOSInformation->IsChecked() )
			{
				wxString appVersion = App::Inst().GetAppVersion();
				wxFileSystem fsCounter;
				#ifdef WIN32
				wxString osVersion = wxT( "Windows" );
				#else
				wxString osVersion = wxT( "MacOSX" );
				#endif
				wxString serverPath(
					wxT( "http://kiwi.cs.dal.ca/~GenGIS/cgi-bin/counter.cgi?OS=" ) +
					osVersion +  wxT( "&VER=" ) + appVersion
				);
				
				wxFSFile* counterScript = fsCounter.OpenFile( serverPath );
				if( counterScript == NULL )
				{
					wxMessageBox( wxT("GenGIS server is unavailable."),
						wxT("Connection error"), wxOK | wxICON_INFORMATION );
					return;
				}
				counterScript->DetachStream();
			}

			Destroy();
		}
		
		void OpenDownloadPage( wxCommandEvent& event )
		{
			wxLaunchDefaultBrowser( wxT( "http://kiwi.cs.dal.ca/GenGIS/Download" ));
			OnClose( event );
		}

		/** @brief Destructor. */
		virtual ~UpdateGenGISDlg() {}

	private:

	};

}

#endif
