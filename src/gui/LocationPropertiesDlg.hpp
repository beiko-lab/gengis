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

#ifndef _GENGIS_LOCATION_PROPERTIES_DLG_
#define _GENGIS_LOCATION_PROPERTIES_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/LocationPropertiesLayout.h"

#include "../utils/ColourMap.hpp"

namespace GenGIS
{

	/**
	* @brief Handles all events and GUI processing for location properties dialog.
	*/
	class LocationPropertiesDlg : public LocationPropertiesLayout
	{
	public:
		enum PAGE { GENERAL = 0, SYMBOLOGY = 1, METADATA = 3 };

	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		* @param locationLayer Location layer to modify properties of.
		* @param layerTreeController Controller for all layer properties.
		* @param mapController Controller for map model.
		*/
		explicit LocationPropertiesDlg(wxWindow* parent, LocationLayerPtr locationLayer);

		/** Destructor */
		~LocationPropertiesDlg();

		/** Set the currently open page. */
		void SetPage(int pageIndex) { m_notebook->SetSelection(pageIndex); }

	private:
		/** Initialize controls on General page. */
		void InitGeneral();

		/** Initialize controls on Symbology page. */
		void InitSymbology();

		/** Initialize controls on MetaData page. */
		void InitMetadata();

		/** Apply all parameter values. */
		void Apply();

		/** Apply parameter values of controls on General page. */
		void ApplyGeneral();

		/** Apply parameter values of controls on Symbology page. */
		void ApplySymbology();

		/** OK button event handler. */
		void OnOK( wxCommandEvent& event ){ Apply(); Destroy(); }

		/** Apply button event handler. */
		void OnApply( wxCommandEvent& event ){ Apply(); }

		/**Cancel button event handler. */
		void OnCancel( wxCommandEvent& event ){ Destroy(); }

		/** Close dialog. */
		void OnClose( wxCloseEvent& event ) { Destroy(); }
		
		/** Event handler for selecting colours under Unix. */
		void OnCustomColourButtonClicked( wxMouseEvent& event );

		/** Replace the standard wxColourPickerCtrl with a Mac/Unix compatible version. */
		void ReplaceColourPicker( wxColourPickerCtrl* wxColourPicker, Colour& colour );

		void OnHelp( wxCommandEvent& event )
		{
			wxLaunchDefaultBrowser( wxT( "http://kiwi.cs.dal.ca/GenGIS/The_GenGIS_2.0_Manual#Location_Layer_Properties" ));
		}

	protected:
		LocationLayerPtr m_locationLayer;
	};

}

#endif 
