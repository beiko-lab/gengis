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
#ifndef _GENGIS_ABOUT_DLG_
#define _GENGIS_ABOUT_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/AboutLayout.h"

namespace GenGIS
{
	/**
	* @brief The "About GenGIS" window.
	*/
	class AboutDlg : public AboutLayout
	{
	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		*/
		explicit AboutDlg( wxWindow* parent ) : AboutLayout ( parent )
		{
			SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

			wxString appVersion = App::Inst().GetAppVersion();
			m_staticTextGenGisVersion->SetLabel( wxT( "GenGIS Version " ) + appVersion );

			wxBitmap bitmap;
			bitmap.LoadFile( GenGIS::App::Inst().GetExeDir() + wxT( "images/Logo.png" ), wxBITMAP_TYPE_PNG );
			m_bitmap->SetBitmap( bitmap );

			wxString path = App::Inst().GetExeDir();
			m_htmlReleaseNotes->LoadPage( path + wxT( "release_notes.html" ) );
			m_htmlAuthors->LoadPage( path + wxT( "authors.html" ) );
			m_htmlLicense->LoadPage( path + wxT( "license.html" ) );
		}

		/**
		* @brief Event handler for the OK button.
		* @param event wxCommandEvent event.
		*/
		void OnClose( wxCommandEvent& event )
		{
			Destroy();
		}

		/** @brief Destructor. */
		virtual ~AboutDlg() {}

	private:

	};

}

#endif
