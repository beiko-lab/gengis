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

#ifndef _GENGIS_COLOUR_PICKER_DLG_
#define _GENGIS_COLOUR_PICKER_DLG_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/**
	* @brief Singleton colour picker dialog box which supports retention of custom colours.
	*/
	class ColourPickerDlg
	{
	public:
		/** Get the one and only instance of this class. */
		static ColourPickerDlg& Inst()
		{
			static ColourPickerDlg singleton;

			return singleton;
		}

		/** Destructor */
		~ColourPickerDlg() {}

		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		* @param colourPickerBtn wxButton being used as a colour picker.
		*/
		void ShowModal(wxWindow* parent, wxButton* colourPickerBtn);

		/** Get colour. */
		wxColour GetColour() const { return m_colour; }

	protected:
		/** Private singleton constructor (must instantiate this class through Inst()). */
		ColourPickerDlg() {}

		/** Private copy constructor (must instantiate this class through Inst()). */
		ColourPickerDlg(const ColourPickerDlg&);

		/** Private assignment constructor (must instantiate this class through Inst()). */
		ColourPickerDlg& operator= (const ColourPickerDlg&);

	private:
		/** Selected colour. */
		wxColour m_colour;

		/** Custom colour data. */
		wxColourData m_colourData;
	};

}

#endif 
