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

#include "../gui/TerrainMapWidget.hpp"

#include "../utils/ColourMapManager.hpp"
#include "../utils/ColourMap.hpp"
#include "../utils/ColourMapDiscrete.hpp"
#include "../utils/StringTools.hpp"

using namespace GenGIS;

void TerrainMapWidget::ConnectColourPickerEvents()
{
	for (unsigned int i = 0; i < m_colourPickers.size(); ++i)
	{
		Tracker* tracker = new Tracker(i);
		m_colourPickers.at(i)->Connect(wxEVT_COMMAND_COLOURPICKER_CHANGED, 
			wxColourPickerEventHandler(TerrainMapWidget::OnColourChange), 
			tracker, this);
		m_colourPickerTrackers.push_back(tracker);
		#ifndef WIN32
		m_customColourButtons.at(i)->Connect( wxEVT_LEFT_DOWN,
			wxMouseEventHandler( TerrainMapWidget::OnCustomColourButtonClicked ), NULL, this );
		#endif
	}
}

void TerrainMapWidget::DisconnectColourPickerEvents()
{
	for(unsigned int i = 0; i < m_colourPickers.size(); ++i)
	{
		m_colourPickers.at(i)->Disconnect(wxEVT_COMMAND_COLOURPICKER_CHANGED, 
			wxColourPickerEventHandler(TerrainMapWidget::OnColourChange), 
			m_colourPickerTrackers.at(i), this);
		#ifndef WIN32
		m_customColourButtons.at(i)->Disconnect( wxEVT_LEFT_DOWN,
			wxMouseEventHandler( TerrainMapWidget::OnCustomColourButtonClicked ), NULL, this );
		#endif
	}
}

void TerrainMapWidget::ConnectTextControlEvents()
{
	for(unsigned int i = 0; i < m_entries.size(); ++i)
	{
		Tracker* tracker = new Tracker(i);
		m_entries.at(i)->Connect(wxEVT_COMMAND_TEXT_UPDATED, 
			wxCommandEventHandler(TerrainMapWidget::OnTextChange), 
			tracker, this);
		m_textControlTrackers.push_back(tracker);
	}
}

void TerrainMapWidget::DisconnectTextControlEvents()
{
	for(unsigned int i = 0; i < m_entries.size(); ++i)
	{
		m_entries.at(i)->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, 
			wxCommandEventHandler(TerrainMapWidget::OnTextChange), 
			m_textControlTrackers.at(i), this);
	}
}

void TerrainMapWidget::OnCustomColourButtonClicked( wxMouseEvent& event )
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

void TerrainMapWidget::OnColourChange( wxColourPickerEvent& event )
{
//	Tracker* tracker = (Tracker*)event.m_callbackUserData;
//	int index = tracker->GetIndex();
	uint index = event.GetId();

	m_colourMapDiscrete->SetColour(StringTools::ToStringW(index), Colour(m_colourPickers.at(index)->GetColour()));
	m_colourMapDiscrete->SetCustom(true);

	if(m_cboColourMap->FindString(wxString(m_colourMapDiscrete->GetExtendedName().c_str())) == wxNOT_FOUND)
		m_cboColourMap->Append(wxString(m_colourMapDiscrete->GetExtendedName().c_str()));

	m_cboColourMap->SetValue(wxString(m_colourMapDiscrete->GetExtendedName().c_str()));

	(*m_bColourMapChanged) = true;
}

void TerrainMapWidget::OnTextChange( wxCommandEvent& event )
{
	Tracker* tracker = (Tracker*)event.m_callbackUserData;

	int index = tracker->GetIndex();
	m_intervals.at(index) = StringTools::ToDouble(m_entries.at(index)->GetValue().c_str());

	(*m_bColourMapChanged) = true;
}

void TerrainMapWidget::PopulateColourMapComboBox()
{
	ColourMapManagerPtr colourMapManager = App::Inst().GetColourMapManager();
	const std::map<std::wstring, ColourMapPtr>& colourMaps = colourMapManager->GetColourMaps();
	foreach(ColourMapManager::ColourMapPair colourMapPair, colourMaps)
	{
		m_cboColourMap->Append(wxString(colourMapPair.second->GetExtendedName().c_str()));
	}

	if(m_cboColourMap->FindString(wxString(m_colourMapDiscrete->GetExtendedName().c_str())) == wxNOT_FOUND)
		m_cboColourMap->Append(wxString(m_colourMapDiscrete->GetExtendedName().c_str()));

	// Set colour map combo box to currently active colour map
	m_cboColourMap->SetValue(wxString(m_colourMapDiscrete->GetExtendedName().c_str()));
}

void TerrainMapWidget::EvenlySpaceEntries(wxScrolledWindow* scrolledWindow, float maxElevation, float minElevation)
{
	float step = (maxElevation-minElevation) / (m_entries.size() - 1);

	m_intervals.clear();
	for(uint i = 0; i < m_entries.size(); ++i)
	{
		m_intervals.push_back(minElevation + i*step);
	}

	SetEntries(scrolledWindow, m_intervals.size(), maxElevation, minElevation);
}

void TerrainMapWidget::SetEntries(wxScrolledWindow* scrolledWindow, uint numEntries, float maxElevation, float minElevation)
{
	// clear any previous items
	wxSizer* prevSizer = scrolledWindow->GetSizer();
	if(prevSizer != NULL)
	{
		prevSizer->Clear(true);
		m_colourPickers.clear();
		m_customColourButtons.clear();
		m_entries.clear();
		//m_colourMapDiscrete->ClearNames();
		DisconnectColourPickerEvents();
		DisconnectTextControlEvents();
	}

	// populate scroll window with colour pickers and labels for each possible field value
	std::vector<std::wstring>::const_iterator setIt;	
	wxBoxSizer* sizerScrollWindow = new wxBoxSizer( wxVERTICAL );
	scrolledWindow->Freeze();

	for(uint index = 0; index < numEntries; ++index)
	{
		wxBoxSizer* boxSizer = new wxBoxSizer( wxHORIZONTAL );

		// get colour from colour map unless field values have already been associated with colours
		Colour colour;
		if(!m_colourMapDiscrete->GetColour(StringTools::ToStringW(index), colour))
		{
			if(m_colourMapDiscrete->GetType() == ColourMap::DISCRETE || m_colourMapDiscrete->GetType() == ColourMap::DISCRETIZED_CONTINUOUS)
				colour = m_colourMapDiscrete->GetColour(index);
			else if(m_colourMapDiscrete->GetType() == ColourMap::CONTINUOUS)
				colour = m_colourMapDiscrete->GetInterpolatedColour(index, 0, numEntries-1);
		}

		wxColour wxcolour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt());

		wxColourPickerCtrl* colourPicker = new wxColourPickerCtrl(scrolledWindow, index,
			wxcolour, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
		boxSizer->Add( colourPicker, 0, wxALL, 2 );
		m_colourPickers.push_back(colourPicker);

		#ifndef WIN32
		colourPicker->Hide();
		CustomColourButton* customColourPicker = new CustomColourButton( scrolledWindow, colourPicker, wxcolour );
		boxSizer->Add( customColourPicker, 0, wxALL, 2 );
		m_customColourButtons.push_back( customColourPicker );
		#endif

		std::wstring elevationStr;
		if(index == 0)
			elevationStr = StringTools::ToStringW(minElevation, 2);
		else if(index == numEntries-1)
			elevationStr = StringTools::ToStringW(maxElevation, 2);
		else if(index < m_intervals.size())
			elevationStr = StringTools::ToStringW(m_intervals.at(index), 2);
		else
			elevationStr = StringTools::ToStringW(maxElevation, 2);

		wxTextCtrl* textCtrl = new wxTextCtrl(scrolledWindow, wxID_ANY, elevationStr.c_str(), wxDefaultPosition, wxDefaultSize, 0 );
		if(index == 0 || index == numEntries-1)
		{
			// make sure user can't change the first or last interval value
			textCtrl->SetEditable(false);
			textCtrl->Enable(false);
		}

		boxSizer->Add( textCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
		m_entries.push_back(textCtrl);

		m_colourMapDiscrete->SetColour(StringTools::ToStringW(index), colour);

		sizerScrollWindow->Add( boxSizer, 0, wxEXPAND, 5 );

		wxStaticLine* staticLine = new wxStaticLine( scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
		sizerScrollWindow->Add( staticLine, 0, wxEXPAND | wxALL, 0 );	
	}

	m_intervals.clear();
	for(uint i = 0; i < numEntries; ++i)
	{
		m_intervals.push_back(StringTools::ToDouble(m_entries.at(i)->GetValue().c_str()));
	}

	ConnectColourPickerEvents();
	ConnectTextControlEvents();

	scrolledWindow->Thaw();
	scrolledWindow->SetSizer( sizerScrollWindow );	
	scrolledWindow->FitInside();
	scrolledWindow->Layout();	
}

void TerrainMapWidget::SetColourMap()
{
	if (m_colourMapDiscrete->ReduceExtendedName(m_cboColourMap->GetValue().c_str()) == L"Custom")
	{
		return;
	}

	// Grab the list of available colour maps
	ColourMapManagerPtr colourMapManager = App::Inst().GetColourMapManager();
	// Get the colour map that was selected in the combo box list
	ColourMapPtr selectedColourMap = colourMapManager->GetColourMap(m_colourMapDiscrete->ReduceExtendedName(m_cboColourMap->GetValue().c_str()));

	m_colourMapDiscrete->SetName(selectedColourMap->GetName());
	m_colourMapDiscrete->CopyColourMap(selectedColourMap);

	for(uint i = 0; i < m_colourPickers.size(); ++i)
	{
		Colour colour;
		if(selectedColourMap->GetType() == ColourMap::DISCRETE || selectedColourMap->GetType() == ColourMap::DISCRETIZED_CONTINUOUS)
			colour = selectedColourMap->GetColour(i);
		else if(selectedColourMap->GetType() == ColourMap::CONTINUOUS)
			colour = selectedColourMap->GetInterpolatedColour(i, 0, m_colourPickers.size()-1);

		m_colourPickers.at(i)->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt()));
		#ifndef WIN32
		m_customColourButtons.at(i)->SetBackgroundColour( wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt()) );
		#endif
		m_colourMapDiscrete->SetColour(StringTools::ToStringW(i), colour);
	}
	m_scrolledWindowColourMap->Refresh();
}

void TerrainMapWidget::GetEntries(std::vector<float>& entries) 
{ 
	for(uint i = 0; i < m_entries.size(); ++i)
	{
		entries.push_back(StringTools::ToDouble(m_entries.at(i)->GetValue().c_str()));
	}
}
