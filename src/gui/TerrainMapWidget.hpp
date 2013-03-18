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

#ifndef _GENGIS_TERRAIN_MAP_WIDGET_
#define _GENGIS_TERRAIN_MAP_WIDGET_

#include "../core/Precompiled.hpp"

#include "../utils/ColourMapDiscrete.hpp"

#include "../widgets/CustomColourButton.hpp"

namespace GenGIS
{
	/**
	* @brief Used to track colour picker and text control events.
	*/
	class Tracker : public wxObject
	{
	public:
		explicit Tracker(int index): m_index(index) {}

		int GetIndex() const { return m_index; }

	public:
		int m_index;
	};

	/**
	* @brief Helper functions for interacting with terrain (interval) colour maps.
	*/
	class TerrainMapWidget : public wxDialog 
	{
	public:
		/** Constructor. */
		TerrainMapWidget(wxComboBox* cboColourMap, wxScrolledWindow* scrolledWindowColourMap, bool* bColourMapChanged) :
			m_cboColourMap(cboColourMap),
			m_scrolledWindowColourMap(scrolledWindowColourMap),
			m_bColourMapChanged(bColourMapChanged)
		{
			m_colourMapDiscrete.reset( new ColourMapDiscrete() );
		}

		/** Destructor */
		~TerrainMapWidget() {}

		/** Set colour map. */
		void SetColourMap(ColourMapDiscretePtr colourMap)
		{
			// Create a copy of the colour map
			m_colourMapDiscrete->CopyColourMap( colourMap );
		}

		/** Set interval data. */
		void SetIntervalData(const std::vector<float>& intervals) { m_intervals = intervals; }

		/** Get colour map. */
		ColourMapDiscretePtr GetColourMap() const { return m_colourMapDiscrete; }

		/** Get colour pickers. */
		std::vector<wxColourPickerCtrl*>& GetColourPicker() { return m_colourPickers; }

		/** Get entry values. */
		void GetEntries(std::vector<float>& entries);

		/** Populate the wxScrolledWindow with entries. */
		void SetEntries(wxScrolledWindow* scrolledWindow, uint numEntries, float maxElevation, float minElevation);

		/** Evenly distribute elevation amongst colour map entries. */
		void EvenlySpaceEntries(wxScrolledWindow* scrolledWindow, float maxElevation, float minElevation);

		/** Assign colours to entires based on colour map. */
		void SetColourMap();

		/** Populate combo box with all available colour maps. */
		void PopulateColourMapComboBox();

	protected:
		/** Specify event for all wxColourPickers. */
		void ConnectColourPickerEvents();

		/** Disconnect event for all wxColourPickers. */
		void DisconnectColourPickerEvents();

		/** Specify event for all wxTextCtrls. */
		void ConnectTextControlEvents();

		/** Disconnect event for all wxTextCtrls. */
		void DisconnectTextControlEvents();

		/** Event indicating the colour of a field value has been changed. */
		void OnColourChange( wxColourPickerEvent& event );

		/** Event handler for selecting colours under Mac/Unix. */
		void OnCustomColourButtonClicked( wxMouseEvent& event );

		/** Event indicating the field of a text control has changed. */
		void OnTextChange( wxCommandEvent& event );

	private:
		/** Colour map of interest. */
		ColourMapDiscretePtr m_colourMapDiscrete;

		/** Colour pickers used to display colour map. */
		std::vector<wxColourPickerCtrl*> m_colourPickers;

		/** Custom colour pickers used to display colour map on Mac/Unix builds. */
		std::vector<CustomColourButton*> m_customColourButtons;

		/** Interval value associated with a colour. */
		std::vector<wxTextCtrl*> m_entries;

		/** Tracker used to determine which colour picker triggered an event. */
		std::vector<Tracker*> m_colourPickerTrackers;

		/** Tracker used to determine which colour picker triggered an event. */
		std::vector<Tracker*> m_textControlTrackers;

		/** Combo box to populate with available colour maps. */
		wxComboBox* m_cboColourMap;

		/** Scrolled window that contains the colour pickers and labels. */
		wxScrolledWindow* m_scrolledWindowColourMap;

		/** Flag indicating if colour map has changed. */
		bool* m_bColourMapChanged;

		/** Interval data. */
		std::vector<float> m_intervals;
	};

}

#endif 
