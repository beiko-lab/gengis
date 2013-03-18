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
#ifndef _GENGIS_WAITING_DLG_
#define _GENGIS_WAITING_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/WaitingLayout.h"

namespace GenGIS
{

	/**
	* @brief A friendly dialog displayed during the save operation.
	*/
	class WaitingDlg : public WaitingLayout
	{
	public:
		/** Constructor. */
		explicit WaitingDlg( wxWindow* parent ) : WaitingLayout ( parent )
		{
			//
		}

		/** Constructor. */
		explicit WaitingDlg( wxWindow* parent, wxString messageText ) : WaitingLayout ( parent )
		{
			m_messageText->SetLabel( messageText );
		}

		/** Update dialog text. */
		void UpdateText( wxString messageText )
		{
			m_messageText->SetLabel( messageText );
		}

		/** Updates the save icon and dialog message. */
		void SaveOK()
		{
			wxBitmap bitmap;
			bitmap.LoadFile( GenGIS::App::Inst().GetExeDir() + wxT( "images/filesaveok.xpm" ), wxBITMAP_TYPE_XPM );
			m_bitmap->SetBitmap( bitmap );
			m_messageText->SetLabel( wxT( "Session saved successfully." ) );
		}

		/** Destructor. */
		virtual ~WaitingDlg()
		{
			//
		}

	private:

	};

}

#endif
