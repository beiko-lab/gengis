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

#ifndef _GENGIS_LIGHTING_PROPERTIES_DLG_
#define _GENGIS_LIGHTING_PROPERTIES_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/LightingPropertiesLayout.h"

namespace GenGIS
{

	/**
	* @brief Configure lighting properties.
	*/
	class LightingPropertiesDlg : public LightingPropertiesLayout
	{
	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		*/
		explicit LightingPropertiesDlg(wxWindow* parent);

		/** Destructor */
		~LightingPropertiesDlg();
	private:
		/** Initialize control values. */
		void Init();

		/** Apply all parameter values. */
		void Apply();

		/** OK button event handler. */
		void OnOK( wxCommandEvent& event ){ Apply(); Destroy(); }

		/** Apply button event handler. */
		void OnApply( wxCommandEvent& event ){ Apply(); }

		/**Cancel button event handler. */
		void OnCancel( wxCommandEvent& event ){ Destroy(); }

		/** Close dialog. */
		void OnClose( wxCommandEvent& event ) { Destroy(); }

		/** Restore default lighting values. */
		void OnDefaultValues( wxCommandEvent& event );
		
		/** One of the lighting sliders was changed. */
		void OnScrollChanged( wxScrollEvent& event );
	};

}

#endif 
