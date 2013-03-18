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

#ifndef _GENGIS_LABEL_TOOL_DLG_
#define _GENGIS_LABEL_TOOL_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/LabelToolLayout.h"

namespace GenGIS
{

	/**
	* @brief Label tool dialog box.
	*/
	class LabelToolDlg : public LabelToolLayout
	{
	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		*/
		explicit LabelToolDlg( wxWindow* parent );

		/** Destructor */
		~LabelToolDlg();

		/** Get label text. */
		wxString GetText() const
		{
			return m_txtLabel->GetValue();
		}

		/** Set label text. */
		void SetText( const wxString& text )
		{
			m_txtLabel->SetValue( text );
		}

		/** Get label size. */
		int GetSize() const
		{
			return m_spinLabelSize->GetValue();
		}

		/** Set label size. */
		void SetSize(int size)
		{
			m_spinLabelSize->SetValue( size );
		}

		/** Get label colour. */
		wxColour GetColour() const
		{
			return m_colourLabel->GetColour();
		}

		/** Set label colour. */
		void SetColour( const wxColour& colour )
		{
			m_colourLabel->SetColour( colour );
		}

		/** Get label style. */
		wxString GetStyle() const
		{
			return m_cboLabelStyle->GetValue();
		}

		/** Set label style. */
		void SetStyle( const wxString& style )
		{
			m_cboLabelStyle->SetValue( style );
		}

		/** Sets the custom colour buttons (Mac/Unix releases). */
		void SetCustomColourButton();

		/** Keeps track of whether or not the changes should be saved. */
		bool SaveChanges() { return m_saveChanges; }

	private:

		/** Event handler for custom colour buttons (Mac/Unix releases). */
		void OnCustomColourButtonClicked( wxMouseEvent& event );

		/** OK button event handler. */
		void OnOK( wxCommandEvent& event );

		/**Cancel button event handler. */
		void OnCancel( wxCommandEvent& event )
		{
			EndModal( wxID_CANCEL );
		}

		/** Keeps track of whether or not the changes should be saved. */
		bool m_saveChanges;
	};

}

#endif 
