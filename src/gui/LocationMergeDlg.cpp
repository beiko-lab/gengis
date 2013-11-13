//=======================================================================
// Author: Alexander Keddy
//
// Copyright 2013 Alexander Keddy
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
#include "../core/LocationSetLayer.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/LocationSetController.hpp"
#include "../core/StudyLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/ChartSetView.hpp"

#include "../gui/LocationMergeDlg.hpp"

#include "../utils/Log.hpp"
#include "../utils/ColourMapManager.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/UniqueId.hpp"

using namespace GenGIS;

LocationMergeDlg::LocationMergeDlg(wxWindow* parent): LocationMergeLayout(parent)
{
	SetIcon( wxIcon( App::Inst().GetExeDir() + wxT( "images/CrazyEye.ico" ), wxBITMAP_TYPE_ICO ) );

	Init();
	Fit();

}

LocationMergeDlg::~LocationMergeDlg()
{

}

void LocationMergeDlg::Init()
{
	//get location layers
	//for each location layer create a checkbox for them
	LayerTreeControllerPtr layerTree = App::Inst().GetLayerTreeController();
	for (uint locSet = layerTree->GetNumLocationSetLayers(); locSet > 0; locSet--)
	{
		std::wstring layerName = layerTree->GetLocationSetLayer( locSet - 1 )->GetName();
		wxString const mystring = layerName;
		m_locationSetCheckbox->InsertItems(1,&mystring,0);
	}

	m_description->SetLabel(wxT("Combine two or more Location Sets into one and insert it into top of the location stack. This allows the new layer to be used with GenGIS plugins. Selecting only one Location Set creates an exact copy of that Location Set."));
	wxSize size;
	size = bSizer1->GetSize();
	
	m_description->Wrap(size.GetWidth());
}
void LocationMergeDlg::OnOK(wxCommandEvent& event)
{
	int numItems = m_locationSetCheckbox->GetCount();
	if( numItems > 0)
	{
		int * checkedIndexes = new int [numItems];
		LayerTreeControllerPtr layerTree = App::Inst().GetLayerTreeController();
		std::vector<LocationSetLayerPtr> LocationSets;
		std::vector<LocationModelPtr> LocationModels;
		std::vector<SequenceModelPtr> SequenceModels;
		std::vector<ChartSetViewPtr> ChartViews;
		
		// Get Intersect of metadata keys
		std::vector<std::wstring> keysIntersect;
		std::vector<std::wstring> keysIntersectSeq;
		bool firstLay = true;
		bool firstSeq = true;
		for(int locSet = 0; locSet < numItems; locSet++)
		{
			if(m_locationSetCheckbox->IsChecked(locSet))
			{
				LocationSetLayerPtr locationSet = layerTree->GetLocationSetLayer( locSet );
				std::vector<std::wstring> fields = locationSet->GetLocationSetController()->GetMetadataFields();
				if(firstLay)
				{
					keysIntersect = fields;	
					firstLay = false;
				}
				else
				{
					std::vector<std::wstring> locs;
					set_intersection(keysIntersect.begin(),keysIntersect.end(), fields.begin(),fields.end(),
						std::inserter(locs,locs.begin()) );
					keysIntersect = locs;
					
				}
				
				//now do the same for sequences
				// IF SEQUENCES
				std::vector<std::wstring> seqFields;
				if( locationSet->GetLocationLayer(0)->GetNumSequenceLayers() > 1 ) 
				{
					seqFields  = locationSet-> GetLocationLayer(0) -> GetSequenceLayer(0) -> GetSequenceController()->GetMetadataFields();
				}

				if(firstSeq)
				{
					keysIntersectSeq = seqFields;	
					firstSeq = false;
				}
				else
				{
					std::vector<std::wstring> locs;
					set_intersection(keysIntersectSeq.begin(),keysIntersectSeq.end(), seqFields.begin(),seqFields.end(),
						std::inserter(locs,locs.begin()) );
					keysIntersectSeq = locs;
					
				}
				
			}
		}
		
		//Finds the indexes of all checked boxes
		for(int locSet = 0; locSet < numItems; locSet++)
		{
			if(m_locationSetCheckbox->IsChecked(locSet))
			{
				LocationSetLayerPtr locationSet = layerTree->GetLocationSetLayer( locSet );
				ChartSetViewPtr chartView = layerTree->GetLocationSetLayer( locSet ) -> GetChartSetView();
				ChartViews.push_back(chartView);
				LocationSets.push_back(locationSet);
				for(uint i = 0; i < locationSet->GetNumLocationLayers(); i++)
				{
					LocationLayerPtr locLayer = locationSet -> GetLocationLayer(i);
					LocationModelPtr locationModel = locationSet -> GetLocationLayer(i) -> GetLocationController() -> GetLocationModel();
					LocationModels.push_back(locationModel);
					for(uint j = 0; j < locLayer->GetNumSequenceLayers(); j++)
					{
						SequenceModelPtr seqMod = locLayer -> GetSequenceLayer(j) -> GetSequenceController() -> GetSequenceModel();
						SequenceModels.push_back(seqMod);
					}
				}
			}
		}
		CreateLocationSet(LocationModels,ChartViews,keysIntersect);
		CreateSequenceSet(SequenceModels,keysIntersectSeq);
		// if m_remove is checked remove the selected layers
		if( m_remove->GetValue() == true )
		{
			RemoveLocationSetLayers(LocationSets);
		}
		Destroy();
	}
}
void LocationMergeDlg::CreateLocationSet( std::vector<LocationModelPtr> locationModels, std::vector<ChartSetViewPtr> ChartViews, std::vector<std::wstring> keysIntersect )
{
	// get selected layer
	LayerPtr selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer == LayerPtr() || selectedLayer->GetType() != Layer::MAP)
	{
		if(App::Inst().GetLayerTreeController()->GetNumMapLayers() == 1)
		{
			// select the first map by default
			App::Inst().GetLayerTreeController()->SetSelection( App::Inst().GetLayerTreeController()->GetMapLayer(0) );
		}
	}

	selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::MAP)
	{
		std::vector<LocationModelPtr> copyLocationModels;
		StudyControllerPtr studyController = App::Inst().GetLayerTreeController()->GetStudyLayer(0)->GetStudyController();
		
		// stop refreshing the tree until all sequences are loaded (this is purely for efficency)
		App::Inst().GetLayerTreeController()->GetTreeCtrl()->Freeze();

		ChartSetViewPtr chartSetCtrl(new ChartSetView());
		for(uint chartset =0; chartset < ChartViews.size(); chartset++)
		{
			for(uint views = 0; views < ChartViews[chartset]->GetSize(); views++)
			{
				ChartViewPtr cView = ChartViews[chartset]->GetChart(views);
				chartSetCtrl->AddChart(cView);
			}
		}
		LocationSetLayerPtr locationSet(new LocationSetLayer(UniqueId::Inst().GenerateId(), 
			App::Inst().GetLayerTreeController()->GetSelectedLayer(),
			chartSetCtrl));
		int numLayers = App::Inst().GetLayerTreeController()->GetNumLocationSetLayers();
		std::string result = "Combined_" + boost::lexical_cast<std::string>(numLayers+1);
		wxString mystring(result.c_str(),wxConvUTF8);
		locationSet->SetName(mystring);
		locationSet->SetFullPath(mystring);
		
		// assign default colour map to location
		ColourMapManagerPtr colourMapManager = App::Inst().GetColourMapManager();
		ColourMapPtr defaultColourMap = colourMapManager->GetDefaultDiscreteColourMap();
		ColourMapDiscretePtr newColourMap(new ColourMapDiscrete(defaultColourMap));

		std::vector<LocationLayerPtr> locationLayers;
		foreach(LocationModelPtr locationModel, locationModels)
		{
			// get intersect of data fields
			std::map<std::wstring,std::wstring> newData;
			for(uint interField = 0; interField < keysIntersect.size(); interField++)
			{
				std::wstring field = keysIntersect[interField];
				newData[field] = locationModel->GetData(field);
			}

			LocationModelPtr copyLocationModel(new LocationModel(locationModel->GetId(),locationModel->GetNorthing(),locationModel->GetEasting(),newData));
			LocationViewPtr locationView(new LocationView(copyLocationModel, App::Inst().GetViewport()->GetCamera(), UniqueId::Inst().GenerateId()));
			ChartViewPtr pieChartView(new ChartView(copyLocationModel, locationView, newColourMap));
			chartSetCtrl->AddChart(pieChartView);
			LocationControllerPtr locationController(new LocationController(copyLocationModel, locationView, pieChartView));

			LocationLayerPtr locationLayer(new LocationLayer(UniqueId::Inst().GenerateId(), locationSet, locationController));			
			locationLayer->SetName(wxString(locationController->GetId().c_str()));
			locationLayer->SetFullPath( mystring );
			locationLayers.push_back(locationLayer);

			locationView->SetLocationLayerId(locationLayer->GetId());
			pieChartView->SetLocationLayerId(locationLayer->GetId());

			locationSet->AddLocationLayer(locationLayer);
		}

		locationSet->GetLocationSetController()->SetLocationSetLayers(locationLayers);

		App::Inst().GetLayerTreeController()->AddLocationSetLayerAtPosition(locationSet,0);

		App::Inst().GetLayerTreeController()->GetTreeCtrl()->Thaw();

		// Update Samples legend on sidebar
		App::Inst().GetLayerTreeController()->GetLocationSetLayer(0)->GetLocationSetController()->GetNumericMetadataFields();
	}
	else
	{
		wxMessageBox(wxT("Please select a map node before adding a location sites layer."),
			wxT("Select map node"), wxOK | wxICON_INFORMATION);
	}
}
void LocationMergeDlg::CreateSequenceSet( std::vector<SequenceModelPtr> sequenceModels, std::vector<std::wstring> keysIntersectSeq )
{
	if( keysIntersectSeq.size() == 0 )
		return;

	LayerPtr selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer == LayerPtr() || selectedLayer->GetType() != Layer::LOCATION_SET)
	{
		if(App::Inst().GetLayerTreeController()->GetNumLocationSetLayers() == 1)
		{
			// select the first location set layer by default
			App::Inst().GetLayerTreeController()->SetSelection( App::Inst().GetLayerTreeController()->GetLocationSetLayer(0) );
		}
	}

	selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::LOCATION_SET)
	{	
			wxBusyCursor wait;

			// stop refreshing the tree until all sequences are loaded (this is purely for efficency)
			App::Inst().GetLayerTreeController()->GetTreeCtrl()->Freeze();

			std::set<std::wstring> missingLocations;
			foreach(SequenceModelPtr sequenceModel, sequenceModels)
			{
				// get intersect of data fields
				std::map<std::wstring,std::wstring> newData;
				for(uint interField = 0; interField < keysIntersectSeq.size(); interField++)
				{
					std::wstring field = keysIntersectSeq[interField];
					newData[field] = sequenceModel->GetData(field);
				}

				SequenceModelPtr copySequenceModel(new SequenceModel(sequenceModel->GetSequenceId(),sequenceModel->GetSiteId(),newData));
				
				SequenceControllerPtr sequenceController(new SequenceController(copySequenceModel));

				SequenceLayerPtr sequenceLayer(new SequenceLayer(UniqueId::Inst().GenerateId(), selectedLayer, sequenceController));
				sequenceLayer->SetName(sequenceController->GetSequenceId().c_str());
				sequenceLayer->SetFullPath( wxT("combination") );

				if(!App::Inst().GetLayerTreeController()->AddSequence(sequenceLayer, copySequenceModel, missingLocations))
				{			
					// The above function should never return false!
					wxMessageBox(wxT("Critial error: Failed to load sequence data. Please report this error to the GenGIS team."), wxT("Failed to load sequence data."), wxOK | wxICON_ERROR);
					return;
				}
			}

			// report any locations that are specified in the sequence file, but do not exist
			foreach(std::wstring locationStr, missingLocations)
			{
				Log::Inst().Warning(wxT("(Warning) Site id '") + wxString(locationStr.c_str()) + wxT("' was not found in the location set. Sequences from this location will be ignored."));
			}

			// all tree to refresh
			App::Inst().GetLayerTreeController()->GetTreeCtrl()->Thaw();
	}
	else
	{
		wxMessageBox(wxT("Please select a location set node before adding sequence data."), wxT("Select location set node"), wxOK | wxICON_INFORMATION);
	}

	App::Inst().GetViewport()->Refresh(false);
}

void LocationMergeDlg::RemoveLocationSetLayers ( std::vector<LocationSetLayerPtr> locationSets  )
{
	for( uint i = 0; i < locationSets.size(); i++ )
	{
		App::Inst().GetLayerTreeController()->SetSelection( locationSets[i] );
		wxCommandEvent dummy;
		App::Inst().GetLayerTreeController()->OnLayerRemove(dummy);
	}
}