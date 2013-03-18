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

#ifndef _GENGIS_CUSTOM_COLOUR_BUTTON_
#define _GENGIS_CUSTOM_COLOUR_BUTTON_

#include "../core/Precompiled.hpp"


namespace GenGIS
{
	/**
	* @brief Used to track which colour picker is associated with the custom colour button.
	*/
	class CustomColourButton : public wxStaticBitmap
	{
	public:
		explicit CustomColourButton( wxWindow* parent, wxColourPickerCtrl* wxColourButton, wxColour wxcolour ) :
		wxStaticBitmap( parent, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER ),
			m_wxColourButton( wxColourButton )
		{
			SetBackgroundColour( wxcolour );
		}

		wxColourPickerCtrl* GetWXColourPickerCtrl() { return m_wxColourButton; }

		void SetSequenceText( std::wstring sequenceText ) { m_sequenceText = sequenceText; }

		std::wstring GetSequenceText() { return m_sequenceText; }

	private:
		wxColourPickerCtrl* m_wxColourButton;

		std::wstring m_sequenceText;
	};
}

#endif
