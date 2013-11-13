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

#ifndef _GENGIS_LOCATION_SET_PROPERTIES_DLG_
#define _GENGIS_LOCATION_SET_PROPERTIES_DLG_

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"

#include "../gui/LocationSetPropertiesLayout.h"
#include "../gui/GenGisFrame.hpp"

#include "../utils/ColourMap.hpp"

#include "../widgets/CustomColourButton.hpp"

namespace GenGIS
{

	/**
	* @brief Handles all events and GUI processing for location set properties dialog.
	*/
	class LocationSetPropertiesDlg : public LocationSetPropertiesLayout
	{
	public:
		enum PAGE { GENERAL = 0, SYMBOLOGY = 1, CHART = 2, METADATA = 3 };

	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		* @param locationSetLayer Layer to set properties of.
		*/
		explicit LocationSetPropertiesDlg(wxWindow* parent, LocationSetLayerPtr locationSetLayer, int currentPageSelection = 0);

		/** Destructor */
		~LocationSetPropertiesDlg();

		/** Set the currently open page. */
		void SetPage(int pageIndex) { m_notebook->SetSelection(pageIndex); }

	private:
		/** Initialize controls on General page. */
		void InitGeneral();

		/** Initialize all controls on Location Set page. */
		void InitLocationSet();

		/** Initialize controls on Location Set->Colour page. */
		void InitLocationSetColour();

		void InitLocationGridColour();

		/** Initialize controls on Location Set->Size page. */
		void InitLocationSetShape();

		/** Initialize controls on Location Set->Size page. */
		void InitLocationSetSize();

		/** Initialize controls on Location Set->Label page. */
		void InitLocationSetLabel();

		/** Initialize controls on Chart page. */
		void InitChart();

		/** Initialize controls on Grid page. */
		void InitLocationGrid();

		/** Initialize alignment controls on Grid page. */
		void InitLocationGridAlignment();

		/** Initialize controls on MetaData page. */
		void InitMetadata();

		/** Change type of chart. */
		virtual void OnChartTypeChanged( wxCommandEvent& event );

		/** Apply all parameter values. */
		void Apply();

		/** Apply parameter values of controls on General page. */
		void ApplyGeneral();

		/** Apply parameter values of controls on Location Set->Colour page. */
		void ApplyLocationSetColour();

		/** Apply parameter values of controls on Location Set->Size page. */
		void ApplyLocationSetShape();

		/** Apply parameter values of controls on Location Set->Size page. */
		void ApplyLocationSetSize();

		/** Apply parameter values of controls on Location Set->Label page. */
		void ApplyLocationLabel();

		/** Apply parameter values of controls on Chart page. */
		void ApplyChart();

		/** Apply parameter values of controls on Grid page. */
		void ApplyGrid();

		/** User is requesting a new uniform colour for location sites. */
		void OnUniformColourPicker( wxCommandEvent& event );

		/** User is requesting a new label colour. */
		void OnLabelColourPicker( wxCommandEvent& event );

		/** User is requesting a new chart dropline colour. */
		void OnChartDropLineColourPicker( wxCommandEvent& event );

		/** Checkbox indicating if all location sites should have the same colour. */
		void OnUniformColour( wxCommandEvent& event );

		/** Field combobox value on Location Set->Colour page has changed. */
		void OnColourFieldChange( wxCommandEvent& event );

		/** Field combobox value on Location Grid->Colour page has changed. */
		void OnChoiceGridFieldToChartChange( wxCommandEvent& event );

		/** Colourmap combobox value on Location Set->Colour page has changed. */
		void OnColourMapChange( wxCommandEvent& event );

		/** Checkbox indicating if all location sites should have the same shape. */
		void OnUniformShape( wxCommandEvent& event );

		/** Field combobox value on Location Set->Shape page has changed. */
		void OnShapeFieldChange( wxCommandEvent& event );

		/** Colourmap combobox value on Location Set->Shape page has changed. */
		void OnShapeMapChange( wxCommandEvent& event );

		/** Field combobox value on chart page has changed. */
		void OnChartFieldChange( wxCommandEvent& event );

		/** Field combobox value on grid page has changed. */
		void OnGridFieldChange( wxCommandEvent& event);

		/** Colourmap combobox value on chart page has changed. */
		void OnChartColourMapChange( wxCommandEvent& event );

		/** Colourmap combobox value on grid page has changed. */
		void OnGridColourMapChange( wxCommandEvent& event);

		/** Enable or disable controls based on wether labels are to be shown. */
		void OnShowLabels( wxCommandEvent& event );

		/** Determine if quantitative charts are desired. */
		void OnQuantitative( wxCommandEvent& event );

		/** Quantitative field changed. */
		void OnQuantitativeFieldChanged( wxCommandEvent& event );

		/** Handler for Colour Fill Properties radio control.
		    'No fill', 'Uniform colour', 'Colour map' */
		void OnRadioColourFill( wxCommandEvent& event );

		/** Show grid borders check box handler. */
		void OnShowGridBorders( wxCommandEvent& event );

		/** Handler for longitude / latitude divisions radio control. */
		void OnRadioLatitudeLongitude( wxCommandEvent& event );

		/** Handler for grid divisions spin control. */
		void OnNumberOfDivisions( wxCommandEvent& event );

		/** Handler for 'Align to' radio controls (origin, location, or coordinates). */
		void OnRadioAlignTo( wxCommandEvent& event );

		/** Handler for wx objects that only need to signal a property change for the grid. */
		void OnGridChanged( wxCommandEvent& event);

		/** Auto adjust to map elevation check box handler. */
		void OnAutoAdjustElevation( wxCommandEvent& event );

		/** OK button event handler. */
		void OnOK( wxCommandEvent& event ){ Apply(); Destroy(); }

		/** Apply button event handler. */
		void OnApply( wxCommandEvent& event ){ Apply(); }

		/** Cancel button event handler. */
		void OnCancel( wxCommandEvent& event ){ Destroy(); }

		/** Change percentage of other category event handler. */
		void OnSpinAssignTaxa( wxSpinEvent& event );

		/** Checkbox indicating if the size of a chart should be set proportional to number of sequences. */
		void OnProportionalChartSize( wxCommandEvent& event );

		/** Close dialog. */
		void OnClose( wxCloseEvent& event ) { Destroy(); }

		/** Event handler for selecting colours under Unix. */
		void OnCustomColourButtonClicked( wxMouseEvent& event );

		/** Show/Hide the custom colour button */
		void ShowButton( wxColourPickerCtrl* wxColourPicker, bool showStatus );

		/** Enable/Disable the custom colour button */
		void EnableButton( wxColourPickerCtrl* wxColourPicker, bool enableStatus );

		/** Replace the standard wxColourPickerCtrl with a Mac/Unix compatible version. */
		void ReplaceColourPicker( wxColourPickerCtrl* wxColourPicker, Colour colour );

		/** Get all field values for a given field in sorted order. */
		void GetSortedFieldValues(const std::wstring& field, std::vector<std::wstring>& fieldValues);

		/** Get all field values for a given field in sorted order for tiles. */
		void GetSortedGridFieldValues(const std::wstring& field, std::vector<std::wstring>& fieldValues, std::vector<TileModelPtr> m_tileModels);

		/** Sort field values either in lexigraphically or numerically. */
		void SortFieldValues(std::vector<std::wstring>& fieldValues);

		/** Reset tile coordinates and tiles. */
		void OnCoordinateReset(wxCommandEvent &event);

		/** Align tiles to mouse click. */
		void OnAlignCoordinateToMouse(wxCommandEvent &event);

		/** Set lat/long coordinates for Mouse position on Viewport. */
		void SetMouseCoordinates(wxMouseEvent& event); 

		/** Change the way tile field values are combined. */
		void OnTileFieldChoiceChange(wxCommandEvent& event);

		/** Change how tile divisions are made. */
		void OnRadioDivideBy(wxCommandEvent& event);

		/** Change how to divide tiles. */
		void OnRadioDivideType(wxCommandEvent& event);

		void OnHelp( wxCommandEvent& event )
		{
			wxLaunchDefaultBrowser( wxT( "http://kiwi.cs.dal.ca/GenGIS/The_GenGIS_2.1_Manual#Location_Set_Layer_Properties" ));
		}

	protected:
		/** Location set to display properties of. */
		LocationSetLayerPtr m_locationSetLayer;

		/** Helper functions for interacting with colour maps on the Location Set->Colour page. */
		ColourMapWidgetPtr m_colourMapWidget;

		/** Helper functions for interacting with colour maps on the Location Set->Grid page. */
		ColourMapWidgetPtr m_gridColourMapWidget;

		/** Helper functions for interacting with colour maps on the chart page. */
		ColourMapWidgetPtr m_chartColourMapWidget;

		/** Helper functions for interacting with shape maps on the Location Set->Shape page. */
		ShapeMapWidgetPtr m_shapeMapWidget;

		/** Location set controller for setting visual properties to entire set of locations. */
		LocationSetControllerPtr m_locationSetController;

		std::map<wxColourPickerCtrl*, CustomColourButton*> m_colourButton;
	};

}

#endif 
