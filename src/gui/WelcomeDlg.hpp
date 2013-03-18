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
#ifndef _GENGIS_WELCOME_DLG_
#define _GENGIS_WELCOME_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/WelcomeLayout.h"

namespace GenGIS
{

	/**
	* @brief A friendly welcome window displayed at startup.
	*/
	class WelcomeDlg : public WelcomeLayout
	{
	public:
		/** Constructor. */
		explicit WelcomeDlg( wxWindow* parent, wxString path ) : WelcomeLayout ( parent ), m_parent( parent )
		{
			wxBitmap bitmap;
			bitmap.LoadFile( path + wxT( "images/Logo.png" ), wxBITMAP_TYPE_PNG );
			m_bitmap->SetBitmap( bitmap );

			SetIcon( wxIcon( path + wxT("images/CrazyEye.ico" ), wxBITMAP_TYPE_ICO ));

			m_listBoxRecentSessions->Set( ((GenGisFrame*)m_parent)->GetRecentSessionList() );
		}

		void OnRecentSession( wxCommandEvent& event )
		{
			int id = m_listBoxRecentSessions->GetSelection();
			id += 8001;
			event.SetId( id );
			this->Destroy();
			((GenGisFrame*)m_parent)->OnFileOpenSession( event );
		}

		void OnOpenRecentSession( wxCommandEvent& event )
		{
			int id = m_listBoxRecentSessions->GetSelection();
			if ( id >= 0 )
			{
				id += 8001;
				event.SetId( id );
				this->Destroy();
				((GenGisFrame*)m_parent)->OnFileOpenSession( event );
			}
		}

		void OnNewSession( wxCommandEvent& event )
		{
			this->Destroy();
		}

		void OnOpenSession( wxCommandEvent& event )
		{
			this->Destroy();
			((GenGisFrame*)m_parent)->OnFileOpenSession( event );
		}

		void OnGenGISManual( wxCommandEvent& event )
		{
			this->Destroy();
			((GenGisFrame*)m_parent)->OnHelpManual( event );
		}

		void OnExitGenGIS( wxCommandEvent& event )
		{
			m_parent->Destroy();
		}

		void OnCheckShowOnStartup( wxCommandEvent& event )
		{
			((GenGisFrame*)m_parent)->SetWelcomeDialogStatus( m_checkBoxShowOnStartup->GetValue() );
			((GenGisFrame*)m_parent)->SaveUserPreferences();
		}

		/** Destructor. */
		virtual ~WelcomeDlg()
		{
			//
		}

	private:
		wxWindow* m_parent;

	};

}

#endif
