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

#include "../gui/SeqSelectionDlg.hpp"

#include "../core/App.hpp"
#include "../core/SequenceController.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/SequenceController.hpp"
#include "../core/LocationSetController.hpp"
#include "../core/Viewport.hpp"
#include "../core/ChartSetView.hpp"
#include "../core/LayerTreeController.hpp"

#include "../gui/GenGisFrame.hpp"

#include "../utils/StringTools.hpp"

using namespace GenGIS;

SeqSelectionDlg::SeqSelectionDlg(wxWindow* parent, LocationSetLayerPtr locationSetLayer)
	: SeqSelectionLayout(parent), m_locationSetLayer(locationSetLayer)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	if(locationSetLayer)
	{
		m_locationSetController = locationSetLayer->GetLocationSetController();
		m_locationSetController->SetLocationSetLayers(locationSetLayer->GetAllLocationLayers());

		Init();

		Fit();
	}
}

SeqSelectionDlg::~SeqSelectionDlg()
{

}

void SeqSelectionDlg::Init()
{
	if(m_locationSetLayer->GetLocationLayer(0)->GetNumSequenceLayers() <= 0)
		return;

	// populate combo box with all fields associated with a location
	SequenceControllerPtr seqController = m_locationSetLayer->GetLocationLayer(0)->GetSequenceLayer(0)->GetSequenceController();
	std::map<std::wstring,std::wstring> data = 	seqController->GetData();
	std::map<std::wstring,std::wstring>::iterator it;
	for(it = data.begin(); it != data.end(); ++it)
		m_cboFields->Append(wxString((*it).first.c_str()));

	if(!seqController->GetActiveField().empty())
		m_cboFields->SetValue(seqController->GetActiveField().c_str());
	else
		m_cboFields->SetValue(m_cboFields->GetString(0));

	// Set field values
	wxCommandEvent dummy;
	OnFieldChange(dummy);
}

void SeqSelectionDlg::OnFieldChange( wxCommandEvent& event ) 
{ 
	m_lstInactiveValues->Clear();
	m_lstActiveValues->Clear();

	std::vector<std::wstring> activeFieldValues;
	GetSortedFieldValues(m_cboFields->GetValue().c_str(), true, activeFieldValues);

	std::vector<std::wstring> inactiveFieldValues;
	GetSortedFieldValues(m_cboFields->GetValue().c_str(), false, inactiveFieldValues);

	SequenceControllerPtr seqController = m_locationSetLayer->GetLocationLayer(0)->GetSequenceLayer(0)->GetSequenceController();
	seqController->SetActiveField(m_cboFields->GetValue().c_str());

	// If a given field value is currently both inactive and active add it just to the active list box.
	// This situation can easily happen if the user is setting the active set based on multiple criteria through the Python console.
	for(uint i = 0; i < activeFieldValues.size(); ++i)
	{
		std::vector<std::wstring>::iterator it;
		for(it = inactiveFieldValues.begin(); it != inactiveFieldValues.end(); ++it)
		{			
			if(*it == activeFieldValues.at(i))
			{
				inactiveFieldValues.erase(it);
				break;
			}
		}
	}

	// populate inactive and active list boxes
	for(uint i = 0; i < activeFieldValues.size(); ++i)
		m_lstActiveValues->Append(activeFieldValues.at(i).c_str());

	for(uint i = 0; i < inactiveFieldValues.size(); ++i)
		m_lstInactiveValues->Append(inactiveFieldValues.at(i).c_str());
}

void SeqSelectionDlg::GetSortedFieldValues(const std::wstring& field, bool bActive, std::vector<std::wstring>& fieldValues)
{
	// get all unique field values within the given field
	std::set<std::wstring> uniqueFieldValues;
	for(uint i = 0; i < m_locationSetLayer->GetNumLocationLayers(); ++i)
	{
		LocationLayerPtr locLayer = m_locationSetLayer->GetLocationLayer(i);
		for(uint j = 0; j < locLayer->GetNumSequenceLayers(); ++j)
		{			
			SequenceControllerPtr seqController =	locLayer->GetSequenceLayer(j)->GetSequenceController();
			if(bActive == seqController->IsActive())
			{
				std::map<std::wstring,std::wstring> data = seqController->GetData();

				std::map<std::wstring,std::wstring>::const_iterator it = data.find(field);

				if(it->second != _T("Other") && it->second != _T("other"))
					uniqueFieldValues.insert(it->second);
			}
		}
	}

	fieldValues.clear();
	fieldValues = std::vector<std::wstring>(uniqueFieldValues.begin(), uniqueFieldValues.end());

	SortFieldValues(fieldValues);
}

void SeqSelectionDlg::SortFieldValues(std::vector<std::wstring>& fieldValues)
{
	// if all data is numerical, then sort in numerical as opposed to lexigraphical order
	bool bNumeric = true;
	foreach(std::wstring str, fieldValues)
	{
		if(!StringTools::IsDecimalNumber(str))
		{
			bNumeric = false;
			break;
		}
	}

	// sort field values eith numerically or lexigraphically
	if(bNumeric)
	{
		// convert all data to its numeric equivalent, sort, and put back into string vector
		// Note: this somewhate convoluted way of doing a numerical sort of strings is done to
		// ensure that at the end the field values strings are exactly as they originally appeared.
		// Any added training zeros or rounding may cause problems later on when exact string matching
		// is attempted.
		std::vector<NumericStringSorter> fieldNumeric;
		for(uint i = 0; i < fieldValues.size(); ++i)
		{
			fieldNumeric.push_back(NumericStringSorter(StringTools::ToDouble(fieldValues.at(i)), i));
		}

		std::sort(fieldNumeric.begin(), fieldNumeric.end(), NumericStringSorter::NumericStringPredicate); 

		std::vector<std::wstring> sortedFieldValues;
		foreach(NumericStringSorter numeric, fieldNumeric)
		{
			sortedFieldValues.push_back(fieldValues.at(numeric.index));
		}

		fieldValues = sortedFieldValues;
	}
	else
	{
		// sort in lexigraphical order
		std::sort(fieldValues.begin(), fieldValues.end());
	}
}

void SeqSelectionDlg::OnAdd( wxCommandEvent& event )
{
	wxArrayInt selections;
	m_lstInactiveValues->GetSelections(selections);

	wxArrayString selectionStr;
	for(uint i = 0; i < selections.Count(); ++i)
		selectionStr.Add(m_lstInactiveValues->GetString(selections[i]));		

	m_lstActiveValues->Append(selectionStr);

	for(uint i = 0; i < selectionStr.Count(); ++i)
		m_lstInactiveValues->Delete(m_lstInactiveValues->FindString(selectionStr[i]));
}

void SeqSelectionDlg::OnRemove( wxCommandEvent& event )
{
	wxArrayInt selections;
	m_lstActiveValues->GetSelections(selections);

	wxArrayString selectionStr;
	for(uint i = 0; i < selections.Count(); ++i)
		selectionStr.Add(m_lstActiveValues->GetString(selections[i]));		

	m_lstInactiveValues->Append(selectionStr);

	for(uint i = 0; i < selectionStr.Count(); ++i)
		m_lstActiveValues->Delete(m_lstActiveValues->FindString(selectionStr[i]));
}

void SeqSelectionDlg::Apply() 
{ 
	if(!m_locationSetLayer)
		return;

	wxBusyCursor wait;

	for(uint i = 0; i < m_locationSetLayer->GetNumLocationLayers(); ++i)
	{
		LocationLayerPtr locLayer = m_locationSetLayer->GetLocationLayer(i);
		for(uint j = 0; j < locLayer->GetNumSequenceLayers(); ++j)
		{			
			SequenceControllerPtr seqController =	locLayer->GetSequenceLayer(j)->GetSequenceController();
			std::map<std::wstring,std::wstring> data = seqController->GetData();

			std::map<std::wstring,std::wstring>::const_iterator it = data.find(m_cboFields->GetValue().c_str());
			
			if(m_lstActiveValues->FindString(it->second.c_str()) != wxNOT_FOUND)
				seqController->SetActive(true);
			else
				seqController->SetActive(false);
		}
	}

	// update pie charts
	m_locationSetLayer->GetChartSetView()->UpdateCharts();
	((GenGisFrame*)GenGIS::App::Inst().GetMainWindow())->FillSequenceLegend();

	// update layer tree controller
	App::Inst().GetLayerTreeController()->UpdateActiveState();

	App::Inst().GetViewport()->Refresh();
}
