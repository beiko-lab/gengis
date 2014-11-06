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

#ifndef _GENGIS_COLOUR_MAP_WIDGET_
#define _GENGIS_COLOUR_MAP_WIDGET_

#include "../core/Precompiled.hpp"
#include "../core/LocationLayer.hpp"

#include "../utils/ColourMapDiscrete.hpp"

#include "../widgets/CustomColourButton.hpp"

namespace GenGIS
{

	/**
	* @brief Used to track which colour picker trigger an event.
	*/
	class ColourPickerTracker : public wxObject
	{
	public:
		explicit ColourPickerTracker(int index): m_index(index) {}

		int GetIndex() const { return m_index; }

	public:
		int m_index;
	};


	//Used to track the checkbox events
	class CheckboxTracker : public wxObject
	{
	public:
		explicit CheckboxTracker(int index): m_index(index) {}

		int GetIndex() const { return m_index; }

	public:
		int m_index;
	};

	/**
	* @brief Helper functions for interacting with colour maps.
	*/
	class ColourMapWidget : public wxDialog 
	{
	public:
		/** Constructor. */
		ColourMapWidget(wxComboBox* cboColourMap, wxScrolledWindow* scrolledWindowChart, wxScrolledWindow* scrolledWindowColour) :
			m_cboColourMap(cboColourMap), m_scrolledWindowChart(scrolledWindowChart), m_scrolledWindowColour(scrolledWindowColour)
			{
				m_colourMap.reset( new ColourMapDiscrete() );
			}

		/** Destructor */
		~ColourMapWidget() {}

		/** Assign colours to field values based on colour map. */
		void SetColourMap();

		/** Changes the colour provided and returns 1 on success ( a colour is chosen) and 0 on failure*/
		bool GetCustomUniformColour(Colour &col);

		/** Set colour map. */
		void SetColourMap(ColourMapDiscretePtr colourMap)
		{
			// Create a copy of the colour map
			m_colourMap->CopyColourMap( colourMap );
		}

		/** Get colour map. */
		ColourMapDiscretePtr GetColourMap() const { return m_colourMap; }

		/** Get colour pickers. */
		std::vector<wxColourPickerCtrl*>& GetColourPicker() { return m_colourPickers; }

		/** Get field values. */
		std::vector<wxStaticText*>& GetFieldValues() { return m_fieldValues; }

		/** Populate the wxScrolledWindow with all possible field values. */
		void SetFieldValues(wxScrolledWindow* scrolledWindow, const std::vector<std::wstring>& fieldValues);

		/** Overloaded to handle adding visibility checkboxes to the colour window */
		void SetFieldValues(wxScrolledWindow* scrolledWindow, const std::vector<std::wstring>& fieldValues, 
							const std::wstring& colourField, const std::vector<LocationLayerPtr>& locations);

		/** Populate combo box with all available colour maps. */
		void PopulateColourMapComboBox();

	protected:
		/** Specify event for all wxColourPickers. */
		void ConnectColourPickerEvents();

		/** Disconnect event for all wxColourPickers. */
		void DisconnectColourPickerEvents();

		/** Specify event for all wxCheckboxes */
		void ConnectCheckboxEvents();

		/** Disconnect event for all wxCheckboxes */
		void DisconnectCheckboxEvents();

		/** Checks all checkboxes and shows all locations */
		void ActivateLocations();

		/** Event handler for selecting colours under Mac/Unix. */
		void OnCustomColourButtonClicked( wxMouseEvent& event );

		/** Event indicating the colour of a field value has been changed. */
		void OnColourChange( wxColourPickerEvent& event );

		/** Event handler for the visibility checkboxes */
		void OnChecked( wxCommandEvent& event );

	private:
		/** Colour map of interest. */
		ColourMapDiscretePtr m_colourMap;

		/** Location Layers used for hiding/revealing locations */
		std::vector<LocationLayerPtr> m_locations;

		/** Colour field used to determine the current field being charted */
		std::wstring m_colourField;

		/** Colour pickers used to display colour map. */
		std::vector<wxColourPickerCtrl*> m_colourPickers;

		/** Custom colour pickers used to display colour map on Mac/Unix builds. */
		std::vector<CustomColourButton*> m_customColourButtons;

		/** Labels indicating name associated with a colour. */
		std::vector<wxStaticText*> m_fieldValues;

		/** Checkboxes used to determine whether a certain field of locations should be shown */
		std::vector<wxCheckBox*> m_visibilityBoxes;

		/** Tracker used to determine which colour picker triggered an event. */
		std::vector<ColourPickerTracker*> m_trackers;

		/** Tracker used to determine which checkbox triggered an event */
		std::vector<CheckboxTracker*> m_checkboxTrackers;

		/** Combo box to populate with available colour maps. */
		wxComboBox* m_cboColourMap;

		/** Scrolled window that contains the colour pickers and labels. */
		wxScrolledWindow* m_scrolledWindowChart;

		/** Scrolled window that contains the colour pickers and labels. */
		wxScrolledWindow* m_scrolledWindowColour;
	};

}

#endif 
