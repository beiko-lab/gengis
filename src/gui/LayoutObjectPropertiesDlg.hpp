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

#ifndef _GENGIS_LAYOUT_OBJECT_PROPERTIES_DLG_
#define _GENGIS_LAYOUT_OBJECT_PROPERTIES_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/LayoutObjectPropertiesLayout.h"

namespace GenGIS
{

	/**
	* @brief Configure layout object (e.g., line, ellipse) properties.
	*/
	class LayoutObjectPropertiesDlg : public LayoutObjectPropertiesLayout
	{
	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		*/
		explicit LayoutObjectPropertiesDlg(wxWindow* parent);

		/** Destructor */
		~LayoutObjectPropertiesDlg();

	private:
		/** Initialize control values. */
		void Init();

		/** Event handler for custom colour buttons (Mac/Unix releases). */
		void OnCustomColourButtonClicked( wxMouseEvent& event );

		/** Apply all parameter values. */
		void Apply();

		/** Change colour of layout object. */
		void OnLayoutObjectColourPicker( wxCommandEvent& event );

		/** Change colour of layout line. */
		void OnLayoutLineColourPicker( wxCommandEvent& event );

		/** Change colour of control points. */
		void OnControlPointColourPicker( wxCommandEvent& event );

		/** OK button event handler. */
		void OnOK( wxCommandEvent& event ){ Apply(); Destroy(); }

		/** Apply button event handler. */
		void OnApply( wxCommandEvent& event ){ Apply(); }

		/**Cancel button event handler. */
		void OnCancel( wxCommandEvent& event ){ Destroy(); }

		/** Close dialog. */
		void OnClose( wxCommandEvent& event ) { Destroy(); }
	};

}

#endif 
