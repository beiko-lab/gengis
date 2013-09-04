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
#include "../gui/LocationMergeDlg.hpp"
#include "../core/App.hpp"
#include "../utils/Log.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/LocationSetController.hpp"

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
	for (uint locSet = 0; locSet < layerTree->GetNumLocationSetLayers(); locSet++)
	{
		std::wstring layerName = layerTree->GetLocationSetLayer( locSet )->GetName();
		wxString const mystring(layerName);
		m_locationSetCheckbox->InsertItems(1,&mystring,0);
	}
	
}

void LocationMergeDlg::OnOK(wxCommandEvent& event)
{
	int numItems = m_locationSetCheckbox->GetCount();
	if( numItems > 0)
	{
		int * checkedIndexes = new int [numItems];
		LayerTreeControllerPtr layerTree = App::Inst().GetLayerTreeController();
		std::vector<LocationSetLayerPtr> LocationSets;
		//Finds the indexes of all checked boxes
		for(int locSet = 0; locSet < numItems; locSet++)
		{
			if(m_locationSetCheckbox->IsChecked(locSet))
			{
				LocationSetLayerPtr locationSet = layerTree->GetLocationSetLayer( locSet );
				LocationSets.push_back(locationSet);
				for(uint i = 0; i < locationSet->GetNumLocationLayers(); i++)
				{
					std::vector<LocationLayerPtr> locations = locationSet->GetAllActiveLocationLayers();
				}
			}
		}
		CreateLocationSet(LocationSets);

		Destroy();
	}
}

void LocationMergeDlg::CreateLocationSet( std::vector<LocationSetLayerPtr> LocationSets )
{
	App::Inst().GetLayerTreeController()->GetTreeCtrl()->Freeze();
	ChartSetViewPtr chartSetCtrl(new ChartSetView());
	LocationSetLayerPtr locationSet(new LocationSetLayer(UniqueId::Inst().GenerateId(), 
			App::Inst().GetLayerTreeController()->GetSelectedLayer(),
			chartSetCtrl));
	int numLayers = App::Inst().GetLayerTreeController()->GetNumLocationSetLayers();
	std::string result = "Combined_" + boost::lexical_cast<std::string>(numLayers+1);
	wxString mystring(result.c_str(),wxConvUTF8);
	locationSet->SetName(mystring);
	std::vector<LocationLayerPtr> locationLayers;
	foreach(LocationSetLayerPtr locpoint, LocationSets)
	{
		for(uint i = 0; i < locpoint->GetNumLocationLayers(); i++)
		{
			LocationLayerPtr locationLayer = locpoint->GetLocationLayer(i);
			locationSet->AddLocationLayer(locationLayer);
			locationLayers.push_back(locationLayer);
		}
	}
	App::Inst().GetLayerTreeController()->AddLocationSetLayer(locationSet);
	App::Inst().GetLayerTreeController()->AddSequence(locationSet);
	App::Inst().GetLayerTreeController()->GetTreeCtrl()->Thaw();
}