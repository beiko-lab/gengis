//=======================================================================
// Author: Timothy Mankowski
//
// Copyright 2012 Timothy Mankowski
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
#ifndef _GENGIS_SELECTOR_WINDOW_DLG_
#define _GENGIS_SELECTOR_WINDOW_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/SelectorWindowLayout.h"

namespace GenGIS
{

	/**
	* @brief A window containing a selection box with 'OK' and 'Cancel' buttons
	*/
	class SelectorWindowDlg : public SelectorWindowLayout
	{
	public:
		/** Constructor. */
		explicit SelectorWindowDlg( wxWindow* parent, wxArrayString selectionList ) :
			SelectorWindowLayout( parent ),
			m_parent( parent ), m_selectionList( selectionList ), m_currentSelection( wxEmptyString )
		{
			SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

			m_listBoxSelect->InsertItems( selectionList, 0 );
		}

		/** Destructor. */
		virtual ~SelectorWindowDlg()
		{
			//
		}

		void SetLabel( wxString label )
		{
			m_staticText->SetLabel( label );
		}

		void OnSelectionDoubleClick( wxCommandEvent& event )
		{
			OnSelectButton( event );
		}

		void OnSelectButton( wxCommandEvent& event )
		{
			int selection = m_listBoxSelect->GetSelection();
			if ( selection != wxNOT_FOUND )
			{
				m_currentSelection = m_selectionList[selection];
				this->Destroy();
			}
			else
			{
				wxMessageBox( wxT("Please make a selection"), wxT("GenGIS"), wxOK );
			}
		}

		void OnCancel( wxCommandEvent& event )
		{
			this->Destroy();
		}

		wxString GetSelection()
		{
			return m_currentSelection;
		}

	private:
		wxWindow* m_parent;

		wxArrayString m_selectionList;
		wxString m_currentSelection;
	};

}

#endif
