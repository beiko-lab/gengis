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
#include "../core/LocationLayer.hpp"
#include "../core/LayerTreeView.hpp"

#include "../utils/ColourMapDiscrete.hpp"
#include "../utils/ColourMapManager.hpp"

#include "../gui/ColourMapWidget.hpp"

using namespace GenGIS;

void ColourMapWidget::ConnectColourPickerEvents()
{
	for(unsigned int i = 0; i < m_colourPickers.size(); ++i)
	{
		ColourPickerTracker* tracker = new ColourPickerTracker(i);
		m_colourPickers.at(i)->Connect(wxEVT_COMMAND_COLOURPICKER_CHANGED, 
			wxColourPickerEventHandler(ColourMapWidget::OnColourChange), 
			tracker, this);
		m_trackers.push_back(tracker);
		#ifndef WIN32
		m_customColourButtons.at(i)->Connect( wxEVT_LEFT_DOWN,
			wxMouseEventHandler( ColourMapWidget::OnCustomColourButtonClicked ), NULL, this );
		#endif
	}
}

void ColourMapWidget::DisconnectColourPickerEvents()
{
	for(unsigned int i = 0; i < m_colourPickers.size(); ++i)
	{
		m_colourPickers.at(i)->Disconnect(wxEVT_COMMAND_COLOURPICKER_CHANGED, 
			wxColourPickerEventHandler(ColourMapWidget::OnColourChange), 
			m_trackers.at(i), this);
		#ifndef WIN32
		m_customColourButtons.at(i)->Disconnect( wxEVT_LEFT_DOWN,
			wxMouseEventHandler( ColourMapWidget::OnCustomColourButtonClicked ), NULL, this );
		#endif
	}
}

void ColourMapWidget::OnCustomColourButtonClicked( wxMouseEvent& event )
{
	// Retrieve the CustomColourButton object that was clicked
	CustomColourButton* customColourButton = ( CustomColourButton* )event.GetEventObject();

	// Create and open the colour picker dialog
	wxColourData *colourData = new wxColourData();
	colourData->SetColour( customColourButton->GetBackgroundColour() );
	wxColourDialog *colourPicker = new wxColourDialog( this, colourData );
	if ( colourPicker->ShowModal() == wxID_CANCEL ) return;

	// Set the CustomColourButton background colour (used for display under Mac/Unix releases)
	customColourButton->SetBackgroundColour( colourPicker->GetColourData().GetColour() );
	customColourButton->Refresh();
	
	// Set the wxColourPickerCtrl colour (hidden, but its value will be used to change the colour)
	wxColourPickerCtrl* wxColourPicker = customColourButton->GetWXColourPickerCtrl();
	wxColourPicker->SetColour( colourPicker->GetColourData().GetColour() );

	// Trigger the 'OnColourChange' event
	wxColourPickerEvent newEvent( wxColourPicker, wxColourPicker->GetId(), wxColourPicker->GetColour() );
	OnColourChange( newEvent );
}

void ColourMapWidget::OnColourChange( wxColourPickerEvent& event )
{
	//ColourPickerTracker* tracker = (ColourPickerTracker*)event.m_callbackUserData;
	//int index = tracker->GetIndex();
	int index = event.GetId();

	std::wstring name = m_fieldValues.at(index)->GetLabel().c_str();
	m_colourMap->SetColour(name, Colour(m_colourPickers.at(index)->GetColour()));
	m_colourMap->SetCustom(true);

	if(m_cboColourMap->FindString(wxString(m_colourMap->GetExtendedName().c_str())) == wxNOT_FOUND)
		m_cboColourMap->Append(wxString(m_colourMap->GetExtendedName().c_str()));

	m_cboColourMap->SetValue(wxString(m_colourMap->GetExtendedName().c_str()));
}

void ColourMapWidget::PopulateColourMapComboBox()
{
	ColourMapManagerPtr colourMapManager = App::Inst().GetColourMapManager();
	const std::map<std::wstring, ColourMapPtr>& colourMaps = colourMapManager->GetColourMaps();
	foreach(ColourMapManager::ColourMapPair colourMapPair, colourMaps)
		m_cboColourMap->Append(wxString(colourMapPair.second->GetExtendedName().c_str()));

	if(m_cboColourMap->FindString(wxString(m_colourMap->GetExtendedName().c_str())) == wxNOT_FOUND)
		m_cboColourMap->Append(wxString(m_colourMap->GetExtendedName().c_str()));

	// Set colour map combo box to currently active colour map
	m_cboColourMap->SetValue(wxString(m_colourMap->GetExtendedName().c_str()));
}

void ColourMapWidget::SetFieldValues(wxScrolledWindow* scrolledWindow, const std::vector<std::wstring>& fieldValues)
{
	// clear any previous items
	wxSizer* prevSizer = scrolledWindow->GetSizer();
	if(prevSizer != NULL)
	{
		prevSizer->Clear(true);
		m_colourPickers.clear();
		m_customColourButtons.clear();
		m_fieldValues.clear();
//		m_colourMap->ClearNames();
		DisconnectColourPickerEvents();
	}

	// populate scroll window with colour pickers and labels for each possible field value
	std::vector<std::wstring>::const_iterator setIt;	
	wxBoxSizer* sizerScrollWindow = new wxBoxSizer( wxVERTICAL );
	scrolledWindow->Freeze();

	uint index = 0;
	for(setIt = fieldValues.begin(); setIt != fieldValues.end(); ++setIt)
	{
		wxBoxSizer* boxSizer = new wxBoxSizer( wxHORIZONTAL );

		// get colour from colour map unless field values have already been associated with colours
		Colour colour;
		if(!m_colourMap->GetColour(*setIt, colour))
		{
			if(m_colourMap->GetType() == ColourMap::DISCRETE || m_colourMap->GetType() == ColourMap::DISCRETIZED_CONTINUOUS)
				colour = m_colourMap->GetColour(index++);
			else if(m_colourMap->GetType() == ColourMap::CONTINUOUS)
				colour = m_colourMap->GetInterpolatedColour(index++, 0, fieldValues.size()-1);
		}

		wxColour wxcolour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt());

		uint currentIteratorPos = std::distance( fieldValues.begin(), setIt );
		wxColourPickerCtrl* colourPicker = new wxColourPickerCtrl(scrolledWindow, currentIteratorPos, wxcolour, 
			wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
		boxSizer->Add( colourPicker, 0, wxALL, 2 );
		m_colourPickers.push_back(colourPicker);

		#ifndef WIN32
		colourPicker->Hide();
		CustomColourButton* customColourPicker = new CustomColourButton( scrolledWindow, colourPicker, wxcolour );
		boxSizer->Add( customColourPicker, 0, wxALL, 2 );
		m_customColourButtons.push_back( customColourPicker );
		#endif

		wxStaticText* label = new wxStaticText( scrolledWindow, wxID_ANY, wxString((*setIt).c_str()), wxDefaultPosition, wxDefaultSize, 0 );
		label->Wrap( -1 );
		boxSizer->Add( label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
		m_fieldValues.push_back(label);

		m_colourMap->SetColour((*setIt), colour);

		sizerScrollWindow->Add( boxSizer, 0, wxEXPAND, 5 );

		wxStaticLine* staticLine = new wxStaticLine( scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
		sizerScrollWindow->Add( staticLine, 0, wxEXPAND | wxALL, 0 );	
	}

	ConnectColourPickerEvents();

	scrolledWindow->Thaw();
	scrolledWindow->SetSizer( sizerScrollWindow );	
	scrolledWindow->FitInside();
	scrolledWindow->Layout();	
}

void ColourMapWidget::ConnectCheckboxEvents()
{
	for(unsigned int i = 0; i < m_visibilityBoxes.size(); ++i)
	{
		CheckboxTracker* checkboxTracker = new CheckboxTracker(i);
		m_visibilityBoxes.at(i)->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, 
										 wxCommandEventHandler(ColourMapWidget::OnChecked), 
										 checkboxTracker, this);
		m_checkboxTrackers.push_back(checkboxTracker);

	}
}

void ColourMapWidget::DisconnectCheckboxEvents()
{
	for(unsigned int i = 0; i < m_visibilityBoxes.size(); ++i)
	{
		m_visibilityBoxes.at(i)->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, 
										 wxCommandEventHandler(ColourMapWidget::OnChecked), 
										 m_checkboxTrackers.at(i), this);
	}
}

void ColourMapWidget::OnChecked( wxCommandEvent& event ){

	CheckboxTracker* tracker = (CheckboxTracker*)event.m_callbackUserData;
	int index = tracker->GetIndex();

	bool isChecked = event.IsChecked();

	m_fieldValues.at(index)->Enable(isChecked);

	for (uint i = 0; i < m_locations.size(); i++) {
		std::map<std::wstring,std::wstring> data = m_locations.at(i)->GetLocationController()->GetData();
		std::map<std::wstring,std::wstring>::const_iterator currentField = data.find(m_colourField);

		if ( currentField->second == m_fieldValues.at(index)->GetLabel().c_str() ) {
			
			m_locations.at(i)->SetActive(isChecked);
			wxTreeItemId treeItem = m_locations.at(i)->GetWXTreeItemId();
			LayerTreeViewPtr treeView = App::Inst().GetLayerTreeController()->GetTreeView();
			treeView->SetChecked(treeItem, isChecked);

		}	
	}
}

void ColourMapWidget::ActivateLocations() {

	for (uint i = 0; i < m_locations.size(); i++) {

		m_locations.at(i)->SetActive(true);
		wxTreeItemId treeItem = m_locations.at(i)->GetWXTreeItemId();
		LayerTreeViewPtr treeView = App::Inst().GetLayerTreeController()->GetTreeView();
		treeView->SetChecked(treeItem, true);

	}

}

/** Overloaded to handle adding visibility checkboxes to the colour window */
void ColourMapWidget::SetFieldValues(wxScrolledWindow* scrolledWindow, const std::vector<std::wstring>& fieldValues, 
									 const std::wstring& colourField, const std::vector<LocationLayerPtr>& locations)
{
	// clear any previous items
	wxSizer* prevSizer = scrolledWindow->GetSizer();
	if(prevSizer != NULL)
	{
		prevSizer->Clear(true);
		m_colourPickers.clear();
		m_customColourButtons.clear();
		m_fieldValues.clear();
		m_visibilityBoxes.clear();
//		m_colourMap->ClearNames();
		DisconnectColourPickerEvents();
		DisconnectCheckboxEvents();
	}

	// populate scroll window with checkboxes, colour pickers and labels for each possible field value
	std::vector<std::wstring>::const_iterator setIt;	
	wxBoxSizer* sizerScrollWindow = new wxBoxSizer( wxVERTICAL );
	scrolledWindow->Freeze();

	//Activates all locations if the colour field has changed
	if (m_colourField != colourField)
		ActivateLocations();

	uint index = 0;
	for(setIt = fieldValues.begin(); setIt != fieldValues.end(); ++setIt)
	{
		wxBoxSizer* boxSizer = new wxBoxSizer( wxHORIZONTAL );

		wxCheckBox* chkVisibility = new wxCheckBox( scrolledWindow, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );

		bool checkboxTrue = true;
		if (m_colourField == colourField) {

			//The checkbox is only set to false when all locations in the field value are hidden
			checkboxTrue = false;
			for (uint i = 0; i < locations.size(); i++) {

				std::map<std::wstring,std::wstring> data = locations.at(i)->GetLocationController()->GetData();
				std::map<std::wstring,std::wstring>::const_iterator currentField = data.find(colourField);

				if ( currentField->second == (*setIt).c_str() && locations.at(i)->IsActive() )
					checkboxTrue = true;

			}
				
		}
	
		chkVisibility->SetValue(checkboxTrue);


		boxSizer->Add( chkVisibility, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2 );
		m_visibilityBoxes.push_back(chkVisibility);


		// get colour from colour map unless field values have already been associated with colours
		Colour colour;
		if(!m_colourMap->GetColour(*setIt, colour))
		{
			if(m_colourMap->GetType() == ColourMap::DISCRETE || m_colourMap->GetType() == ColourMap::DISCRETIZED_CONTINUOUS)
				colour = m_colourMap->GetColour(index++);
			else if(m_colourMap->GetType() == ColourMap::CONTINUOUS)
				colour = m_colourMap->GetInterpolatedColour(index++, 0, fieldValues.size()-1);
		}

		wxColour wxcolour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt());

		uint currentIteratorPos = std::distance( fieldValues.begin(), setIt );
		wxColourPickerCtrl* colourPicker = new wxColourPickerCtrl(scrolledWindow, currentIteratorPos, wxcolour, 
			wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
		boxSizer->Add( colourPicker, 0, wxALL, 2 );
		m_colourPickers.push_back(colourPicker);

		#ifndef WIN32
		colourPicker->Hide();
		CustomColourButton* customColourPicker = new CustomColourButton( scrolledWindow, colourPicker, wxcolour );
		boxSizer->Add( customColourPicker, 0, wxALL, 2 );
		m_customColourButtons.push_back( customColourPicker );
		#endif

		wxStaticText* label = new wxStaticText( scrolledWindow, wxID_ANY, wxString((*setIt).c_str()), wxDefaultPosition, wxDefaultSize, 0 );
		label->Wrap( -1 );
		label->Enable(checkboxTrue);
		boxSizer->Add( label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
		m_fieldValues.push_back(label);

		m_colourMap->SetColour((*setIt), colour);

		sizerScrollWindow->Add( boxSizer, 0, wxEXPAND, 5 );

		wxStaticLine* staticLine = new wxStaticLine( scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
		sizerScrollWindow->Add( staticLine, 0, wxEXPAND | wxALL, 0 );	
	}

	m_colourField = colourField;
	m_locations = locations;

	ConnectColourPickerEvents();
	ConnectCheckboxEvents();

	scrolledWindow->Thaw();
	scrolledWindow->SetSizer( sizerScrollWindow );	
	scrolledWindow->FitInside();
	scrolledWindow->Layout();	
}

void ColourMapWidget::SetColourMap()
{
	ColourMapManagerPtr colourMapManager = App::Inst().GetColourMapManager();
	ColourMapPtr selectedColourMap = colourMapManager->GetColourMap(m_colourMap->ReduceExtendedName(m_cboColourMap->GetValue().c_str()));
	m_colourMap->SetName(selectedColourMap->GetName());
	m_colourMap->CopyColourMap(selectedColourMap);


	/* Check if colour is supposed to be user selected uniform:
		 If True, Make every location this colour */
	if(selectedColourMap->GetName().find(_T("Uniform (Custom)")) != std::wstring::npos) 
	{
		wxColourDialog dialog(this);
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColourData colData = dialog.GetColourData();
			wxColour wxCol = colData.GetColour();
			Colour col(wxCol.Red(),wxCol.Green(),wxCol.Blue());
			selectedColourMap->SetColour(col,0 );
		}
		else
			return
	}

	for(unsigned int i = 0; i < m_colourPickers.size(); ++i)
	{
		Colour colour;
		/* Check if colour is supposed to be uniform:
		 If True, Make every location this colour */
		if( (selectedColourMap->GetType() == ColourMap::DISCRETE) && 
			(selectedColourMap->GetName().find(_T("Uniform")) != std::wstring::npos) )
		{
			m_colourMap->SetType(ColourMap::DISCRETE);
			colour = selectedColourMap->GetColour(0);
		}
		else if(selectedColourMap->GetType() == ColourMap::DISCRETE)
		{
			m_colourMap->SetType(ColourMap::DISCRETE);
			colour = selectedColourMap->GetColour(i);
		}
		else if(selectedColourMap->GetType() == ColourMap::CONTINUOUS)
		{
			m_colourMap->SetType(ColourMap::DISCRETIZED_CONTINUOUS);
			colour = selectedColourMap->GetInterpolatedColour(i, 0, m_colourPickers.size()-1);
		}

		m_colourPickers.at(i)->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt()));
		#ifndef WIN32
		m_customColourButtons.at(i)->SetBackgroundColour( wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt()) );
		#endif
		std::wstring name = m_fieldValues.at(i)->GetLabel().c_str();
		m_colourMap->SetColour(name, colour);
	}
	m_scrolledWindowChart->Refresh();
	m_scrolledWindowColour->Refresh();
}
