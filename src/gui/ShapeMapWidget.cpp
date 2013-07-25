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

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"

#include "../gui/ShapeMapWidget.hpp"

#include "../utils/ShapeMap.hpp"
#include "../utils/ShapeMapManager.hpp"

using namespace GenGIS;

void ShapeMapWidget::ConnectShapeComboBoxEvents()
{
	for(unsigned int i = 0; i < m_shapeComboBox.size(); ++i)
	{
		ShapeTracker* tracker = new ShapeTracker(i);
		m_shapeComboBox.at(i)->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(ShapeMapWidget::OnShapeChange), 
			tracker, this);
		m_trackers.push_back(tracker);
	}
}

void ShapeMapWidget::DisconnectShapeComboBoxEvents()
{
	for(unsigned int i = 0; i < m_shapeComboBox.size(); ++i)
	{
		m_shapeComboBox.at(i)->Disconnect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(ShapeMapWidget::OnShapeChange), 
			m_trackers.at(i), this);
	}
}

void ShapeMapWidget::OnShapeChange( wxCommandEvent& event )
{
	ShapeTracker* tracker = (ShapeTracker*)event.m_callbackUserData;
	int index = tracker->GetIndex();

	std::wstring shapeName = (m_shapeComboBox.at(index)->GetValue()).c_str();
	VisualMarker::MARKER_SHAPE shape = m_shapeMap->ToShape(shapeName);
	std::wstring fieldName = m_fieldValues.at(index)->GetLabel().c_str();

	m_shapeMap->SetShape(fieldName, shape);
	m_shapeMap->SetCustom(true);

	// add in custom shape mape if it is not already there
	if(m_cboShapeMap->FindString(wxString((m_shapeMap->GetName()).c_str())) == wxNOT_FOUND)
		m_cboShapeMap->Append(wxString(m_shapeMap->GetName().c_str()));

	m_cboShapeMap->SetValue(wxString(m_shapeMap->GetName().c_str()));
}

void ShapeMapWidget::PopulateShapeComboBox(wxComboBox* cbo)
{
	std::vector<VisualMarker::MARKER_SHAPE> shapes = m_shapeMap->GetShapeMap();
	foreach(VisualMarker::MARKER_SHAPE shape, shapes)
	{
		std::wstring shapeStr = m_shapeMap->ToStringW(shape);
		cbo->Append(wxString(shapeStr.c_str()));
	}
}

void ShapeMapWidget::PopulateShapeMapComboBox()
{
	ShapeMapManagerPtr shapeMapManager = App::Inst().GetShapeMapManager();
	const std::map<std::wstring, ShapeMapPtr>& shapeMaps = shapeMapManager->GetShapeMaps();
	foreach(ShapeMapManager::ShapeMapPair shapeMapPair, shapeMaps)
	{
		m_cboShapeMap->Append(wxString(shapeMapPair.second->GetName().c_str()));
	}

	// add in custom shap mape if it is not already there
	if(m_cboShapeMap->FindString(wxString(m_shapeMap->GetName().c_str())) == wxNOT_FOUND)
		m_cboShapeMap->Append(wxString(m_shapeMap->GetName().c_str()));

	m_cboShapeMap->SetValue(wxString(m_shapeMap->GetName().c_str()));
}

void ShapeMapWidget::SetFieldValues(wxScrolledWindow* scrolledWindow, const std::vector<std::wstring>& fieldValues)
{
	// clear any previous items
	wxSizer* prevSizer = scrolledWindow->GetSizer();
	if(prevSizer != NULL)
	{
		prevSizer->Clear(true);
		m_shapeComboBox.clear();
		m_fieldValues.clear();
		m_shapeMap->ClearNames();
		DisconnectShapeComboBoxEvents();
	}

	// populate scroll window with colour pickers and labels for each possible field value
	std::vector<std::wstring>::const_iterator setIt;	
	wxBoxSizer* sizerScrollWindow = new wxBoxSizer( wxVERTICAL );
	scrolledWindow->Freeze();

	int i = 0;
	for(setIt = fieldValues.begin(); setIt != fieldValues.end(); ++setIt)
	{
		wxBoxSizer* boxSizer = new wxBoxSizer( wxHORIZONTAL );

		// get colour from colour map unless field values have already been associated with colours
		VisualMarker::MARKER_SHAPE shape;
		do
		{
			if(!m_shapeMap->GetShape(*setIt, shape))
			{
				shape = m_shapeMap->GetShape(i);
				i++;
			}
		} while ( shape == VisualMarker::CIRCLE_FAST );
		// Avoid using CIRCLE_FAST in this case due to its similarity to CIRCLE

		wxComboBox* cbo = new wxComboBox( scrolledWindow, wxID_ANY, wxString(m_shapeMap->ToStringW(shape).c_str()),
			wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT  );

		PopulateShapeComboBox(cbo);
		cbo->SetValue(wxString(m_shapeMap->ToStringW(shape).c_str()));

		boxSizer->Add( cbo, 0, wxALL, 2 );
		m_shapeComboBox.push_back(cbo);

		wxStaticText* label = new wxStaticText( scrolledWindow, wxID_ANY, wxString((*setIt).c_str()), wxDefaultPosition, wxDefaultSize, 0 );
		label->Wrap( -1 );
		boxSizer->Add( label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
		m_fieldValues.push_back(label);

		sizerScrollWindow->Add( boxSizer, 0, wxEXPAND, 5 );

		wxStaticLine* staticLine = new wxStaticLine( scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
		sizerScrollWindow->Add( staticLine, 0, wxEXPAND | wxALL, 0 );	

		m_shapeMap->SetShape((*setIt), shape);
	}

	ConnectShapeComboBoxEvents();

	scrolledWindow->Thaw();
	scrolledWindow->SetSizer( sizerScrollWindow );	
	scrolledWindow->FitInside();
	scrolledWindow->Layout();	
}

void ShapeMapWidget::SetShapeMap()
{
	ShapeMapManagerPtr shapeMapManager = App::Inst().GetShapeMapManager();
	ShapeMapPtr selectedShapeMap = shapeMapManager->GetShapeMap(m_cboShapeMap->GetValue().c_str());
	m_shapeMap->SetName(selectedShapeMap->GetName());

	for(unsigned int i = 0; i < m_shapeComboBox.size(); ++i)
	{
		VisualMarker::MARKER_SHAPE shape = selectedShapeMap->GetShape(i);

		m_shapeComboBox.at(i)->SetValue(wxString(selectedShapeMap->ToStringW(shape).c_str()));
		std::wstring name = m_fieldValues.at(i)->GetLabel().c_str();
		m_shapeMap->SetShape(name, shape);
	}
}
