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

#ifndef _GENGIS_SHAPE_MAP_WIDGET_
#define _GENGIS_SHAPE_MAP_WIDGET_

#include "../core/Precompiled.hpp"


namespace GenGIS
{

	/**
	* @brief Used to track which shape combobox trigger an event.
	*/
	class ShapeTracker : public wxObject
	{
	public:
		explicit ShapeTracker(int index): m_index(index) {}

		int GetIndex() const { return m_index; }

	public:
		int m_index;
	};

	/**
	* @brief Helper functions for interacting with shape maps.
	*/
	class ShapeMapWidget : public wxDialog 
	{
	public:
		/** Constructor. */
		ShapeMapWidget(wxComboBox* cboShapeMap): m_cboShapeMap(cboShapeMap) {}

		/** Destructor */
		~ShapeMapWidget() {}

		/** Set colour map. */
		void SetShapeMap(ShapeMapPtr shapeMap) { m_shapeMap = shapeMap; }

		/** Get colour map. */
		ShapeMapPtr GetShapeMap() const { return m_shapeMap; }

		/** Get shape combobox. */
		std::vector<wxComboBox*>& GetShapeComboBox() { return m_shapeComboBox; }

		/** Get field values. */
		std::vector<wxStaticText*>& GetFieldValues() { return m_fieldValues; }

		/** Get shape combobox trackers. */
		std::vector<ShapeTracker*>& GetShapeTracker() { return m_trackers; }

		/** Specify event for all wxColourPickers. */
		void ConnectShapeComboBoxEvents();

		/** Disconnect event for all wxColourPickers. */
		void DisconnectShapeComboBoxEvents();

		/** Populate the wxScrolledWindow with all possible field values. */
		void SetFieldValues(wxScrolledWindow* scrolledWindow, const std::vector<std::wstring>& fieldValues);

		/** Assign colours to field values based on shape map. */
		void SetShapeMap();

		/** Populate combo box with all available shape maps. */
		void PopulateShapeMapComboBox();

		/** Populate a combo box with all shapes in a shape map. */
		void PopulateShapeComboBox(wxComboBox* cbo);

	protected:
		/** Event indicating the shape of a field value has been changed. */
		void OnShapeChange( wxCommandEvent& event );

	private:
		/** Shape map of interest. */
		ShapeMapPtr m_shapeMap;

		/** Shape comboboxes used to display shape map. */
		std::vector<wxComboBox*> m_shapeComboBox;

		/** Labels indicating name associated with a shape. */
		std::vector<wxStaticText*> m_fieldValues;

		/** Tracker used to determine which colour picker triggered an event. */
		std::vector<ShapeTracker*> m_trackers;

		/** Combo box to populate with available shape maps. */
		wxComboBox* m_cboShapeMap;
	};

}

#endif 
