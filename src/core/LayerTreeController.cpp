//=======================================================================
// Author: Donovan Parks & Somayyeh Zangooei
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
#include "../core/LayerTreeController.hpp"
#include "../core/LayerTreeView.hpp"
#include "../core/LayerTreeItem.hpp"
#include "../core/GeoTreeView.hpp"
#include "../core/LocationView.hpp"
#include "../core/SequenceModel.hpp"
#include "../core/LayerTreeModel.hpp"
#include "../core/MapLayer.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/TreeLayer.hpp"
#include "../core/Viewport.hpp"
#include "../core/SequenceController.hpp"
#include "../core/StudyLayer.hpp"
#include "../core/VectorMapLayer.hpp"
#include "../core/VectorMapModel.hpp"
#include "../core/VectorMapController.hpp"
#include "../gui/StudyPropertiesDlg.hpp"
#include "../gui/MapPropertiesDlg.hpp"
#include "../gui/VectorMapPropertiesDlg.hpp"

#include "../gui/LocationSetPropertiesDlg.hpp"
#include "../gui/LocationPropertiesDlg.hpp"
#include "../gui/SequencePropertiesDlg.hpp"
#include "../gui/TreePropertiesDlg.hpp"

using namespace GenGIS;

LayerTreeController::LayerTreeController(LayerTreeModelPtr tree, LayerTreeViewPtr treeView) :
	m_layerTreeModel(tree), m_treeView(treeView)
{
	//
}

template<class Archive>
void LayerTreeController::serialize(Archive & ar, const unsigned int file_version)
{
	// we currently only support a single study
	ar & m_maps;         // std::vector<MapLayerPtr>
	ar & m_vectorMaps;   // std::vector<VectorMapLayerPtr>
	ar & m_locationSets; // std::vector<LocationSetLayerPtr>
	ar & m_locations;    // std::vector<LocationLayerPtr>
	ar & m_sequences;    // std::vector<SequenceLayerPtr>
	ar & m_trees;        // std::vector<TreeLayerPtr>
	ar & m_isBlankRaster;
}

template void LayerTreeController::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void LayerTreeController::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

std::wstring LayerTreeController::GetName(uint id) const 
{ 
	return m_layerTreeModel->GetLayer(id)->GetName(); 
}

std::wstring LayerTreeController::GetDescription(uint id) const 
{ 
	return m_layerTreeModel->GetLayer(id)->GetDescription(); 
}

void LayerTreeController::SetDescription(uint id, const std::wstring& description) 
{ 
	m_layerTreeModel->GetLayer(id)->SetDescription(description); 
}

std::wstring LayerTreeController::GetAuthours(uint id) const 
{ 
	return m_layerTreeModel->GetLayer(id)->GetAuthours(); 
}

void LayerTreeController::SetAuthours(uint id, const std::wstring& authours) 
{ 
	m_layerTreeModel->GetLayer(id)->SetAuthours(authours); 
}

std::wstring LayerTreeController::GetPath(uint id) const 
{ 
	return m_layerTreeModel->GetLayer(id)->GetPath(); 
}

std::wstring LayerTreeController::GetFilename(uint id) const 
{ 
	return m_layerTreeModel->GetLayer(id)->GetFilename(); 
}

const bool LayerTreeController::IsVisible(uint id) const 
{ 
	return m_layerTreeModel->GetLayer(id)->IsActive(); 
}

void LayerTreeController::OnRender() 
{ 
	m_layerTreeModel->Render(); 
}

uint LayerTreeController::GetNumStudyLayers() const
{ 
	return 1; // we currently only support a single study
}

StudyLayerPtr LayerTreeController::GetStudyLayer(uint index) const
{ 
	return m_layerTreeModel->GetStudy(index); 
}

uint LayerTreeController::GetNumMapLayers() const
{ 
	return m_maps.size(); 
}

MapLayerPtr LayerTreeController::GetMapLayer(uint index) const
{ 
	return m_maps.at(index);
}

uint LayerTreeController::GetNumVectorMapLayers() const
{ 
	return m_vectorMaps.size();
}

VectorMapLayerPtr LayerTreeController::GetVectorMapLayer(uint index) const
{ 
	return m_vectorMaps.at(index); 
}

uint LayerTreeController::GetNumLocationSetLayers() const
{ 
	return m_locationSets.size(); 
}

LocationSetLayerPtr LayerTreeController::GetLocationSetLayer(uint index) const
{ 
	return m_locationSets.at(index); 
}

uint LayerTreeController::GetNumLocationLayers() const
{ 
	return m_locations.size(); 
}

LocationLayerPtr LayerTreeController::GetLocationLayer(uint index) const
{ 
	return m_locations.at(index); 
}

LocationLayerPtr LayerTreeController::GetLocationLayerById( uint id ) const
{ 
	std::vector<LocationLayerPtr>::const_iterator iter;
	for(iter = m_locations.begin(); iter != m_locations.end(); iter++)
	{
		if( (*iter)->GetId() == id )
		{
			return *iter;
		}
	}

	return LocationLayerPtr();
}

uint LayerTreeController::GetNumSequenceLayers() const
{ 
	return m_sequences.size(); 
}

SequenceLayerPtr LayerTreeController::GetSequenceLayer(uint index) const
{ 
	return m_sequences.at(index); 
}

uint LayerTreeController::GetNumTreeLayers() const
{ 
	return m_trees.size(); 
}

TreeLayerPtr LayerTreeController::GetTreeLayer(uint index) const
{ 
	return m_trees.at(index); 
}

TreeLayerPtr LayerTreeController::GetTreeLayerById( uint id ) const
{ 
	std::vector<TreeLayerPtr>::const_iterator iter;
	for(iter = m_trees.begin(); iter != m_trees.end(); iter++)
	{
		if( (*iter)->GetId() == id )
		{
			return *iter;
		}
	}

	return TreeLayerPtr();
}

wxTreeCtrl* LayerTreeController::GetTreeCtrl() const
{ 
	return m_treeView->GetTreeCtrl(); 
}

LayerTreeViewPtr LayerTreeController::GetTreeView() const
{ 
	return m_treeView; 
}

LayerPtr LayerTreeController::GetSelectedLayer() const
{
	wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();
	if(!selectionId.IsOk())
		return LayerPtr();

	LayerTreeItem* selectionItem = (LayerTreeItem*)m_treeView->GetTreeCtrl()->GetItemData(selectionId);

	return selectionItem->GetLayer();
}

void LayerTreeController::SetName( LayerPtr layer, const std::wstring& name)
{
	m_treeView->GetTreeCtrl()->SelectItem( layer->GetWXTreeItemId() );
	layer->SetName( name.c_str() );
	m_treeView->SetName(layer->GetWXTreeItemId(), name);
}

void LayerTreeController::SetSelection( LayerPtr layer )
{
	m_treeView->GetTreeCtrl()->SelectItem( layer->GetWXTreeItemId() );
}

void LayerTreeController::SetVisible(uint id, bool visibility) 
{ 
	m_layerTreeModel->GetLayer(id)->SetActive(visibility); 

	wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();
	if(!selectionId.IsOk())
		return;

	m_treeView->SetChecked(selectionId, m_layerTreeModel->GetLayer(id)->IsActive());

	App::Inst().GetViewport()->Refresh(false);
}

LocationControllerPtr LayerTreeController::GetLocationController(const std::wstring& id) const
{
	for(uint i = 0; i < GetNumLocationLayers(); ++i)
	{
		LocationLayerPtr locationLayer = GetLocationLayer(i);
		if(locationLayer->GetLocationController()->GetId() == id)
		{
			return locationLayer->GetLocationController();
		}
	}

	return LocationControllerPtr();
}

bool LayerTreeController::AddMapLayer(MapLayerPtr map)
{
	wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();
	LayerPtr selectedLayer = GetSelectedLayer();

	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::STUDY)
	{
		// At this point during deserialization, the map layer is already connected to the study layer
		if (!App::Inst().IsCurrentlySerializing())
		{
			// add map to tree controller so it can be easily accessed
			m_maps.push_back(map);

			// add map to tree model
			StudyLayerPtr studyPtr = boost::dynamic_pointer_cast<StudyLayer>(selectedLayer);
			studyPtr->AddMapLayer(map);
		}

			// add map to tree control
			LayerTreeItem* layerItem = new LayerTreeItem(map);
			wxString idStr(std::wstring(_T("Raster Map : ") + map->GetName()).c_str());
			wxTreeItemId newId = m_treeView->GetTreeCtrl()->AppendItem(selectionId, wxT(""), 
				map->IsActive(), -1, layerItem);
			m_treeView->GetTreeCtrl()->SetItemText(newId, idStr);
			map->SetWXTreeItemId( newId );

			// select the newly added layer
			m_treeView->GetTreeCtrl()->SelectItem(newId);
			m_treeView->GetTreeCtrl()->Expand(selectionId);
	}
	else
	{
		wxMessageBox(wxT("Please select a study node before adding a new map."),
			wxT("Select study node"), wxOK | wxICON_INFORMATION);

		return false;
	}

	return true;
}

bool LayerTreeController::AddVectorMapLayer( VectorMapLayerPtr vectorMap, LayerPtr selectedLayer,wxTreeItemId selectionId )
{
	//wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();
	//LayerPtr selectedLayer = GetSelectedLayer();
	if ( selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::STUDY )
	{
		// At this point during deserialization, the vector map layer is already connected to the study layer
		if ( !App::Inst().IsCurrentlySerializing() )
		{
			// add vector map to tree controller so it can be easily accessed
			m_vectorMaps.push_back( vectorMap );

			// add map to tree model
			StudyLayerPtr studyPtr = boost::dynamic_pointer_cast<StudyLayer>(selectedLayer);
			studyPtr->AddVectorMapLayer( vectorMap );
		}

		// add map to tree control
		LayerTreeItem* layerItem = new LayerTreeItem( vectorMap );
		wxString idStr( std::wstring( _T( "Vector Map : " ) + vectorMap->GetName() ).c_str() );
		wxTreeItemId newId = m_treeView->GetTreeCtrl()->AppendItem( selectionId, wxT(""), 
			vectorMap->IsActive(), -1, layerItem );
		m_treeView->GetTreeCtrl()->SetItemText( newId, idStr );
		vectorMap->SetWXTreeItemId( newId );

		// select the newly added layer
		m_treeView->GetTreeCtrl()->SelectItem( newId );
		m_treeView->GetTreeCtrl()->Expand( selectionId );

		//App::Inst().GetViewport()->Refresh(false);
	}
	else
	{
		wxMessageBox( wxT( "Please select a study node before adding a new vector map." ),
			wxT( "Select study node" ), wxOK | wxICON_INFORMATION );

		return false;
	}




	return true;

	// Add vector map to tree controller so it can be easily 
	//m_vectorMaps.push_back( vectorMap );
	//return true;
}

bool LayerTreeController::AddTreeLayer(TreeLayerPtr tree)
{
	wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();
	LayerPtr selectedLayer = GetSelectedLayer();

	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::MAP)
	{
		// At this point during deserialization, the map layer is already connected to the study layer
		if (!App::Inst().IsCurrentlySerializing())
		{
			// add tree to tree controller so it can be easily access
			m_trees.push_back(tree);

			// add tree layer to tree model
			MapLayerPtr mapPtr = boost::dynamic_pointer_cast<MapLayer>(selectedLayer);
			mapPtr->AddTreeLayer(tree);
		}

		// add tree layer to tree control
		LayerTreeItem* layerItem = new LayerTreeItem(tree);
		wxString idStr(std::wstring(_T("Tree : ") + tree->GetName()).c_str());
//		wxTreeItemId treeId = m_treeView->GetTreeCtrl()->AppendItem(selectionId, idStr, 
//			tree->IsActive(), -1, layerItem);
		wxTreeItemId treeId = m_treeView->GetTreeCtrl()->AppendItem(selectionId, wxT(""), 
			tree->IsActive(), -1, layerItem);
		m_treeView->GetTreeCtrl()->SetItemText(treeId, idStr);
		tree->SetWXTreeItemId( treeId );

		// select the newly added layer
		//m_treeView->GetTreeCtrl()->Expand(selectionId);
		m_treeView->GetTreeCtrl()->SelectItem(treeId);

		App::Inst().GetViewport()->Refresh(false);
	}
	else
	{
		wxMessageBox(wxT("Please select a map node before adding a new tree."),
			wxT("Select map node"), wxOK | wxICON_INFORMATION);

		return false;
	}

	return true;
}

bool LayerTreeController::AddLocationSetLayerAtPosition(LocationSetLayerPtr locationSet, int pos)
{
	LayerPtr selectedLayer = GetSelectedLayer();

	if(selectedLayer == LayerPtr() || selectedLayer->GetType() != Layer::MAP)
	{
		if( GetNumMapLayers() == 1 )
		{
			// select the first map by default
			SetSelection( GetMapLayer(0) );
			selectedLayer = GetSelectedLayer();
		}
	}

	wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();

	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::MAP)
	{
		// At this point during deserialization, the location set layer is already connected to the map layer
		if (!App::Inst().IsCurrentlySerializing())
		{
			// add location set to tree controller so it can be easily access at begining
			//	NEEDS TO BE CHANGED TO ADD IT AT GIVEN POSITION
			m_locationSets.insert(m_locationSets.begin(),locationSet);

			// add location set to tree model
			MapLayerPtr mapPtr = boost::dynamic_pointer_cast<MapLayer>(selectedLayer);
			mapPtr->AddLocationSetLayer(locationSet);
		}

		// add location set to tree control
		LayerTreeItem* layerItem = new LayerTreeItem(locationSet);
		wxString idStr(std::wstring(_T("Location Set : ") + locationSet->GetName()).c_str());
		wxTreeItemId locationSetId = m_treeView->GetTreeCtrl()->PrependItem(selectionId, wxT(""),	//Change to add at position
			locationSet->IsActive(), -1, layerItem);

		m_treeView->GetTreeCtrl()->SetItemText(locationSetId, idStr);
		locationSet->SetWXTreeItemId( locationSetId );

		// select the location set layer
		m_treeView->GetTreeCtrl()->SelectItem(locationSetId);

		// add each location to tree control
		for(unsigned int i = 0; i < locationSet->GetNumLocationLayers(); ++i)
		{
			LocationLayerPtr location = locationSet->GetLocationLayer(i);

			if (!App::Inst().IsCurrentlySerializing())
			{
				m_locations.push_back(location);
			}

			LayerTreeItem* locationItem = new LayerTreeItem(location);
			wxString idStr(std::wstring(_T("Location : ") + location->GetName()).c_str());
			wxTreeItemId locationId = m_treeView->GetTreeCtrl()->AppendItem(locationSetId, wxT(""), 
				location->IsActive(), -1, locationItem);
			m_treeView->GetTreeCtrl()->SetItemText(locationId, idStr);
			location->SetWXTreeItemId( locationId );
		}

		App::Inst().GetViewport()->Refresh(false);
	}

	else
	{
		wxMessageBox(wxT("Please select a map node before adding a new location set."),
			wxT("Select map node"), wxOK | wxICON_INFORMATION);

		return false;
	}

	return true;
}
bool LayerTreeController::AddLocationSetLayer(LocationSetLayerPtr locationSet)
{
	LayerPtr selectedLayer = GetSelectedLayer();

	if(selectedLayer == LayerPtr() || selectedLayer->GetType() != Layer::MAP)
	{
		if( GetNumMapLayers() == 1 )
		{
			// select the first map by default
			SetSelection( GetMapLayer(0) );
			selectedLayer = GetSelectedLayer();
		}
	}

	wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();

	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::MAP)
	{
		// At this point during deserialization, the location set layer is already connected to the map layer
		if (!App::Inst().IsCurrentlySerializing())
		{
			// add location set to tree controller so it can be easily access
			m_locationSets.push_back(locationSet);

			// add location set to tree model
			MapLayerPtr mapPtr = boost::dynamic_pointer_cast<MapLayer>(selectedLayer);
			mapPtr->AddLocationSetLayer(locationSet);
		}

		// add location set to tree control
		LayerTreeItem* layerItem = new LayerTreeItem(locationSet);
		wxString idStr(std::wstring(_T("Location Set : ") + locationSet->GetName()).c_str());
		wxTreeItemId locationSetId = m_treeView->GetTreeCtrl()->AppendItem(selectionId, wxT(""), 
			locationSet->IsActive(), -1, layerItem);
		m_treeView->GetTreeCtrl()->SetItemText(locationSetId, idStr);
		locationSet->SetWXTreeItemId( locationSetId );

		// select the location set layer
		m_treeView->GetTreeCtrl()->SelectItem(locationSetId);

		// add each location to tree control
		for(unsigned int i = 0; i < locationSet->GetNumLocationLayers(); ++i)
		{
			LocationLayerPtr location = locationSet->GetLocationLayer(i);

			if (!App::Inst().IsCurrentlySerializing())
			{
				m_locations.push_back(location);
			}

			LayerTreeItem* locationItem = new LayerTreeItem(location);
			wxString idStr(std::wstring(_T("Location : ") + location->GetName()).c_str());
			wxTreeItemId locationId = m_treeView->GetTreeCtrl()->AppendItem(locationSetId, wxT(""), 
				location->IsActive(), -1, locationItem);
			m_treeView->GetTreeCtrl()->SetItemText(locationId, idStr);
			location->SetWXTreeItemId( locationId );
		}

		App::Inst().GetViewport()->Refresh(false);
	}

	else
	{
		wxMessageBox(wxT("Please select a map node before adding a new location set."),
			wxT("Select map node"), wxOK | wxICON_INFORMATION);

		return false;
	}

	return true;
}

void LayerTreeController::AddSequence(LocationSetLayerPtr locationSetPtr)
{
	wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();
	
	wxTreeItemIdValue cookie;
	wxTreeItemId locationTreeId = m_treeView->GetTreeCtrl()->GetFirstChild(selectionId, cookie);

	for(uint currLocId = 0; currLocId < locationSetPtr->GetNumLocationLayers(); ++currLocId)
	{
		LocationLayerPtr locationLayer = locationSetPtr->GetLocationLayer(currLocId);

		for (uint currSeqId = 0; currSeqId < locationLayer->GetNumSequenceLayers(); ++currSeqId)
		{
			SequenceLayerPtr sequenceLayer = locationLayer->GetSequenceLayer(currSeqId);

			if (sequenceLayer->GetSequenceController()->GetSiteId() == locationLayer->GetLocationController()->GetId())
			{
				// add sequence to tree control
				LayerTreeItem* layerItem = new LayerTreeItem(sequenceLayer);
				wxString idStr(std::wstring(_T("Sequence : ") + sequenceLayer->GetName()).c_str());
				wxTreeItemId sequenceId = m_treeView->GetTreeCtrl()->AppendItem(locationTreeId, wxT(""), LayerTreeView::SEQ_UNCHECKED + sequenceLayer->IsActive(), -1, layerItem);
				m_treeView->GetTreeCtrl()->SetItemText(sequenceId, idStr); 
				sequenceLayer->SetWXTreeItemId( sequenceId );
			}
			else
			{
				break;
			}
		}
		// move to next child in wxTreeCtrl
		locationTreeId = m_treeView->GetTreeCtrl()->GetNextChild(selectionId, cookie);
	}	
}

bool LayerTreeController::AddSequence(SequenceLayerPtr sequence, SequenceModelPtr sequenceModel, std::set<std::wstring>& missingLocations)
{
	wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();

	LayerPtr selectedLayer = GetSelectedLayer();

	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::LOCATION_SET)
	{
		// add sequence layer to appropriate location layer
		LocationSetLayerPtr locationSetPtr = boost::dynamic_pointer_cast<LocationSetLayer>(selectedLayer);

		bool bFoundLocationLayer = false;
		wxTreeItemIdValue cookie;
		wxTreeItemId locationTreeId = m_treeView->GetTreeCtrl()->GetFirstChild(selectionId, cookie);
		for(unsigned int j = 0; j < locationSetPtr->GetNumLocationLayers(); ++j)
		{
			LocationLayerPtr locationLayer = locationSetPtr->GetLocationLayer(j);
			if(locationLayer->GetLocationController()->GetId() == sequenceModel->GetSiteId())
			{
				locationLayer->AddSequence(sequence);
				locationLayer->GetLocationController()->AddSequence(sequenceModel);

				// add sequence to tree control
				LayerTreeItem* layerItem = new LayerTreeItem(sequence);
				wxString idStr(std::wstring(_T("Sequence : ") + sequence->GetName()).c_str());
				wxTreeItemId sequenceId = m_treeView->GetTreeCtrl()->AppendItem(locationTreeId, wxT(""), LayerTreeView::SEQ_CHECKED, -1, layerItem);
				m_treeView->GetTreeCtrl()->SetItemText(sequenceId, idStr);
				sequence->SetWXTreeItemId( sequenceId );

				bFoundLocationLayer = true;
				break;
			}

			// move to next child in wxTreeCtrl
			locationTreeId = m_treeView->GetTreeCtrl()->GetNextChild(selectionId, cookie);
		}	

		if(bFoundLocationLayer)
		{
			// add sequence layer to tree controller so it can be easily access
			m_sequences.push_back(sequence);
		}
		else
		{
			missingLocations.insert(sequenceModel->GetSiteId());
		}
	}
	else
	{
		wxMessageBox(wxT("Please select a location set node before adding new sequence data."),
			wxT("Select location set node"), wxOK | wxICON_INFORMATION);

		return false;
	}

	return true;
}

void LayerTreeController::OnSelect( wxTreeEvent& event )
{
	event.Skip();
}

void LayerTreeController::OnClick( wxMouseEvent& event )
{
	event.Skip();

	int hitPos;
	wxTreeItemId hitId = m_treeView->GetTreeCtrl()->HitTest(event.GetPosition(), hitPos);

	if(hitPos & wxTREE_HITTEST_ONITEMICON)
	{
		LayerTreeItem* treeItem = (LayerTreeItem*)m_treeView->GetTreeCtrl()->GetItemData(hitId);
		treeItem->GetLayer()->ToggleActive();
		m_treeView->SetChecked(hitId, treeItem->GetLayer()->IsActive());

		// project trees
		for(uint i = 0; i < m_trees.size(); ++i)
			m_trees[i]->GetGeoTreeView()->ProjectToActiveSet(m_locations);

		App::Inst().GetViewport()->Refresh(false);
	}
}

void LayerTreeController::OnLeftDClick( wxMouseEvent& event )
{
	// cause the tree item under the cursor to become selected
	wxTreeItemId id = GetTreeCtrl()->GetSelection();
	
	if(id.IsOk())
	{
		LayerTreeItem* layerItem = (LayerTreeItem*)GetTreeCtrl()->GetItemData(id);
		LayerPtr layer = layerItem->GetLayer();
		if(!App::Inst().GetLayerTreeController()->GetIsBlankRaster())
		{
		// If a properties dialog is already open, set its focus
			if ( layer->HasPropertiesDialogOpen() )
			{
				layer->GetPropertiesDialog()->SetFocus();
				layer->GetPropertiesDialog()->Raise();
				return;
			}

		// Otherwise, open a new properties dialog
			layer->SetPropertiesDialogOpenStatus( true );
		}

		wxMenu menu;
		if(layer->GetType() == Layer::STUDY)
		{
			StudyLayerPtr studyLayer = boost::dynamic_pointer_cast<StudyLayer>(layer);
			StudyPropertiesDlg* dlg = new StudyPropertiesDlg(App::Inst().GetMainWindow(), studyLayer);
			dlg->SetPage(StudyPropertiesDlg::PROJECTION);
			dlg->Show();
		}
		else if(layer->GetType() == Layer::MAP && !App::Inst().GetLayerTreeController()->GetIsBlankRaster())
		{
			MapLayerPtr mapLayer = boost::dynamic_pointer_cast<MapLayer>(layer);
			MapPropertiesDlg* dlg = new MapPropertiesDlg(App::Inst().GetMainWindow(), mapLayer);
			dlg->SetPage(MapPropertiesDlg::SYMBOLOGY);
			dlg->Show();
		}
		else if(layer->GetType() == Layer::VECTOR_MAP)
		{
			VectorMapLayerPtr vectorMapLayer = boost::dynamic_pointer_cast<VectorMapLayer>(layer);
			VectorMapPropertiesDlg* dlg = new VectorMapPropertiesDlg(App::Inst().GetMainWindow(), vectorMapLayer);
			 
			VectorMapControllerPtr vectorMapController= vectorMapLayer->GetVectorMapController();
			vFileMetaData* vHeader = vectorMapController->GetVectorMetaData();
			std::string type = vHeader->geometryType;
			if(type=="Point")
			{
				dlg->SetSubPage(VectorMapPropertiesDlg::POINT);
				dlg->DisablePolyLinePanel();
				dlg->DisablePolygonPanel();
			}

			else if (type=="Polyline")
			{
				dlg->SetSubPage(VectorMapPropertiesDlg::POLYLINE);
			    dlg->DisablePointPanel();
				dlg->DisablePolygonPanel();
			}
			else if(type=="Polygon")
			{
				dlg->SetSubPage(VectorMapPropertiesDlg::POLYGON);
				dlg->DisablePolyLinePanel();
				dlg->DisablePointPanel();
			}
			else
				dlg->SetPage(VectorMapPropertiesDlg::GENERAL);
			dlg->Show();
		}
		else if(layer->GetType() == Layer::LOCATION_SET)
		{
			LocationSetLayerPtr locationSetLayer = boost::dynamic_pointer_cast<LocationSetLayer>(layer);
			LocationSetPropertiesDlg* dlg = new LocationSetPropertiesDlg(App::Inst().GetMainWindow(), locationSetLayer);
			dlg->SetPage(LocationSetPropertiesDlg::SYMBOLOGY);
			dlg->Show();
		}
		else if(layer->GetType() == Layer::LOCATION)
		{
			LocationLayerPtr locationLayer = boost::dynamic_pointer_cast<LocationLayer>(layer);
			LocationPropertiesDlg* dlg = new LocationPropertiesDlg(App::Inst().GetMainWindow(), locationLayer);
			dlg->SetPage(LocationPropertiesDlg::SYMBOLOGY);
			dlg->Show();
		}
		else if(layer->GetType() == Layer::SEQUENCE)
		{
			SequenceLayerPtr sequenceLayer = boost::dynamic_pointer_cast<SequenceLayer>(layer);
			SequencePropertiesDlg* dlg = new SequencePropertiesDlg(App::Inst().GetMainWindow(), sequenceLayer);
			dlg->SetPage(SequencePropertiesDlg::GENERAL);
			dlg->Show();
		}
		else if(layer->GetType() == Layer::TREE)
		{
			TreeLayerPtr treeLayer = boost::dynamic_pointer_cast<TreeLayer>(layer);
			TreePropertiesDlg* dlg = new TreePropertiesDlg(App::Inst().GetMainWindow(), treeLayer);
			dlg->SetPage(TreePropertiesDlg::SYMBOLOGY);
			dlg->Show();
		}
	}
}

void LayerTreeController::OnLayerRemove( wxCommandEvent& event )
{
	wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();

	// delete selected item and all of its children from the tree layer model
	LayerTreeItem* selectionItem = (LayerTreeItem*)m_treeView->GetTreeCtrl()->GetItemData(selectionId);
	LayerPtr layer = selectionItem->GetLayer();

	// remove layer from quick access array
 	if(layer->GetType() == Layer::STUDY)
	{
		// Note:: studies can't be deleted at this point in time
		wxMessageBox(wxT("GenGIS currently supports only a single, persistent study layer."));
		return;
	}
	if(layer->GetType() == Layer::MAP && App::Inst().GetLayerTreeController()->GetIsBlankRaster())
	{
		// Note:: studies can't be deleted at this point in time
		wxMessageBox(wxT("This layer is requied to support vector map file and can not be removed."));
		return;
	}

	else
	{
		if(!EraseMap(layer->GetId()))
		{		
			if(!EraseVectorMap(layer->GetId()))
			{
				if(!EraseTree(layer->GetId()))
				{
					if(!EraseLocationSet(layer->GetId()))
					{
						if(!EraseLocation(layer->GetId()))
						{	
							if(!EraseSequence(layer->GetId()))
							{
								// should have erased something!
								Log::Inst().Error("OnLayerRemove(): Unhandle case encountered.");
							}
						}
					}
				}
			}
		}
	}

	// determine layer from layer tree
	m_layerTreeModel->RemoveLayer(layer->GetId());

	// delete selected item and all of its children from the tree control
	m_treeView->GetTreeCtrl()->DeleteChildren(selectionId);
	m_treeView->GetTreeCtrl()->Delete(selectionId);

	App::Inst().GetViewport()->Refresh();
}

void LayerTreeController::OnAllLayersRemove( wxCommandEvent& event )
{
	for(uint i = 0; i < m_maps.size(); ++i)
	{
		App::Inst().GetLayerTreeController()->SetSelection( App::Inst().GetLayerTreeController()->GetMapLayer(i) );
		wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();
		LayerTreeItem* selectionItem = (LayerTreeItem*)m_treeView->GetTreeCtrl()->GetItemData(selectionId);
		LayerPtr layer = selectionItem->GetLayer();
		EraseMap(layer->GetId());
		m_layerTreeModel->RemoveLayer(layer->GetId());	
		m_treeView->GetTreeCtrl()->DeleteChildren(selectionId);
		m_treeView->GetTreeCtrl()->Delete(selectionId);	
		App::Inst().GetViewport()->Refresh();
	}
		
	

}

bool LayerTreeController::EraseMap(uint layerId)
{
	for(uint i = 0; i < m_maps.size(); ++i)
	{
		if(m_maps.at(i)->GetId() == layerId)
		{
			EraseLocationSets(m_maps.at(i));
			EraseTrees(m_maps.at(i));
			m_maps.erase(m_maps.begin()+i);

			// Remove corresponding map and tree(s) from study layer
			StudyLayerPtr studyLayer = App::Inst().GetLayerTreeController()->GetStudyLayer(0);
			studyLayer->GetMapLayerById( layerId )->RemoveAllTreeLayers();
			studyLayer->RemoveMapLayerById( layerId );

			// Remove coresponding layers from GenGisFrame
			((GenGisFrame*)App::Inst().GetMainWindow())->ClearMapLayer();

			if(m_maps.size()==0 && App::Inst().GetLayerTreeController()->GetNumVectorMapLayers()>0)
			{				
				uint index = 0;				
				while (index < m_vectorMaps.size())
				{
					App::Inst().GetLayerTreeController()->SetSelection( App::Inst().GetLayerTreeController()->GetVectorMapLayer(index) );
					wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();
					LayerTreeItem* selectionItem = (LayerTreeItem*)m_treeView->GetTreeCtrl()->GetItemData(selectionId);
					LayerPtr layer = selectionItem->GetLayer();
					EraseVectorMap(layer->GetId());
					m_layerTreeModel->RemoveLayer(layer->GetId());
					m_treeView->GetTreeCtrl()->DeleteChildren(selectionId);
					m_treeView->GetTreeCtrl()->Delete(selectionId);
					App::Inst().GetViewport()->Refresh();
				}

			}
			
			return true;
		}
	}
	
	return false;
}

bool LayerTreeController::EraseVectorMap(uint layerId)
{
	for(uint i = 0; i < m_vectorMaps.size(); ++i)
	{
		if(m_vectorMaps.at(i)->GetId() == layerId)
		{
		//	EraseLocationSets(m_vectorMaps.at(i));
		//	EraseTrees(m_vectorMaps.at(i));
			m_vectorMaps.erase(m_vectorMaps.begin()+i);

			// Remove corresponding map and tree(s) from study layer
			StudyLayerPtr studyLayer = App::Inst().GetLayerTreeController()->GetStudyLayer(0);
			//studyLayer->GetVectorMapLayerById( layerId )->RemoveAllTreeLayers();
			studyLayer->RemoveVectorMapLayerById( layerId );

			// Remove coresponding layers from GenGisFrame
			((GenGisFrame*)App::Inst().GetMainWindow())->ClearVectorMapLayer();
			if(m_vectorMaps.size()==0 && App::Inst().GetLayerTreeController()->GetIsBlankRaster() && m_maps.size() >0)
			{
				App::Inst().GetLayerTreeController()->SetSelection( App::Inst().GetLayerTreeController()->GetMapLayer(0) );
				wxTreeItemId selectionId = m_treeView->GetTreeCtrl()->GetSelection();
				LayerTreeItem* selectionItem = (LayerTreeItem*)m_treeView->GetTreeCtrl()->GetItemData(selectionId);
				LayerPtr layer = selectionItem->GetLayer();
				EraseMap(layer->GetId());
				m_layerTreeModel->RemoveLayer(layer->GetId());
				m_treeView->GetTreeCtrl()->DeleteChildren(selectionId);
				m_treeView->GetTreeCtrl()->Delete(selectionId);
				App::Inst().GetViewport()->Refresh();
			}
			
			return true;
		}
		
	}
	
	
	return false;
}


void LayerTreeController::EraseTrees(MapLayerPtr mapLayer)
{
	for(uint i = 0; i < mapLayer->GetNumTreeLayers(); ++i)
	{
		EraseTree(mapLayer->GetTreeLayer(i)->GetId());
	}
}

bool LayerTreeController::EraseTree(uint layerId)
{
	for(uint i = 0; i < m_trees.size(); ++i)
	{
		if(m_trees.at(i)->GetId() == layerId)
		{
			// Remove coresponding layer from GenGisFrame
			((GenGisFrame*)App::Inst().GetMainWindow())->ClearTreeLayer();

			// Remove reference from LayerTreeController
			m_trees.erase(m_trees.begin()+i);
			return true;
		}
	}

	return false;
}

void LayerTreeController::EraseLocationSets(MapLayerPtr mapLayer)
{
	for(uint i = 0; i < mapLayer->GetNumLocationSetLayers(); ++i)
	{
		EraseLocationSet(mapLayer->GetLocationSetLayer(i)->GetId());
	}
}

bool LayerTreeController::EraseLocationSet(uint layerId)
{
	for(uint i = 0; i < m_locationSets.size(); ++i)
	{
		if(m_locationSets.at(i)->GetId() == layerId)
		{
			// Remove coresponding layers from GenGisFrame
			((GenGisFrame*)App::Inst().GetMainWindow())->ClearLocationSetLayer();
			((GenGisFrame*)App::Inst().GetMainWindow())->ClearLocationLayer();

			EraseLocations(m_locationSets.at(i));
			m_locationSets.erase(m_locationSets.begin()+i);
			return true;
		}
	}

	return false;
}

void LayerTreeController::EraseLocations(LocationSetLayerPtr locationSetLayer)
{
	for(uint i = 0; i < locationSetLayer->GetNumLocationLayers(); ++i)
	{
		EraseLocation(locationSetLayer->GetLocationLayer(i)->GetId());
	}
}

bool LayerTreeController::EraseLocation(uint layerId)
{
	for(uint i = 0; i < m_locations.size(); ++i)
	{
		if(m_locations.at(i)->GetId() == layerId)
		{
			EraseSequences(m_locations.at(i));
			m_locations.erase(m_locations.begin()+i);
			return true;
		}
	}

	return false;
}

void LayerTreeController::EraseSequences(LocationLayerPtr locationLayer)
{
	for(uint i = 0; i < locationLayer->GetNumSequenceLayers(); ++i)
	{
		EraseSequence(locationLayer->GetSequenceLayer(i)->GetId());
	}
}

bool LayerTreeController::EraseSequence(uint layerId)
{
	for(uint i = 0; i < m_sequences.size(); ++i)
	{
		if(m_sequences.at(i)->GetId() == layerId)
		{
			// Remove coresponding layer from GenGisFrame
			((GenGisFrame*)App::Inst().GetMainWindow())->ClearSequenceLayer();

			m_sequences.erase(m_sequences.begin()+i);
			return true;
		}
	}

	return false;
}

void LayerTreeController::OnHideAll( wxCommandEvent& event )
{
	wxTreeItemIdValue cookie;
	wxTreeItemId rootId = m_treeView->GetTreeCtrl()->GetFirstChild(m_treeView->GetTreeCtrl()->GetRootItem(), cookie);
	SetVisibilityAllChildren(rootId, cookie, false);

	App::Inst().GetViewport()->Refresh(false);
}

void LayerTreeController::OnShowAll( wxCommandEvent& event )
{
	wxTreeItemIdValue cookie;
	wxTreeItemId rootId = m_treeView->GetTreeCtrl()->GetFirstChild(m_treeView->GetTreeCtrl()->GetRootItem(), cookie);
	SetVisibilityAllChildren(rootId, cookie, true);

	App::Inst().GetViewport()->Refresh(false);
}

void LayerTreeController::SetVisibilityAllChildren(wxTreeItemId id, wxTreeItemIdValue cookie, bool visibility)
{
	if(id.IsOk())
	{
		// modify current node
		if(visibility)
		{
			m_treeView->GetTreeCtrl()->SetItemTextColour(id, *wxBLACK);
		}
		else
		{
			m_treeView->GetTreeCtrl()->SetItemTextColour(id, *wxLIGHT_GREY);
		}

		LayerTreeItem* layerItem = (LayerTreeItem*)m_treeView->GetTreeCtrl()->GetItemData(id);
		layerItem->GetLayer()->SetActive(visibility);
		m_treeView->SetChecked(id, visibility);

		// depth first traversal of tree
		wxTreeItemIdValue childCookie;
		wxTreeItemId childId = m_treeView->GetTreeCtrl()->GetFirstChild(id, childCookie);
		SetVisibilityAllChildren(childId, childCookie, visibility);

		wxTreeItemId siblingId = m_treeView->GetTreeCtrl()->GetNextSibling(id);
		SetVisibilityAllChildren(siblingId, cookie, visibility);
	}

	// no more children
	return;	
}

bool LayerTreeController::OnLayoutTree(GeoTreeView::LAYOUT layout, LayoutLinePtr layoutLine)
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::TREE)
	{
		// change layout style of tree
		TreeLayerPtr treePtr = boost::dynamic_pointer_cast<TreeLayer>(selectedLayer);

		GeoTreeViewPtr geoTreeView = treePtr->GetGeoTreeView();

		if(layout == GeoTreeView::SLANTED_CLADOGRAM_3D || layout == GeoTreeView::CLADOGRAM_3D || layout == GeoTreeView::SLANTED_PHYLOGRAM_3D)
		{
			geoTreeView->SetLayout(layout);
			App::Inst().GetViewport()->Refresh(false);
			return false;
		}

		// check if a layout line has already been assigned to the tree or not
		if(geoTreeView->IsLayoutLine())
		{
			geoTreeView->SetLayout(layout);
			App::Inst().GetViewport()->Refresh(false);
			return false;
		}
		else
		{
			// a layout line is needed so make sure that there is a valid one to assign to the tree
			if(layoutLine)
			{
				geoTreeView->SetLayoutLine(layoutLine);
				geoTreeView->SetLayout(layout);
				App::Inst().GetViewport()->Refresh(false);
				return true;
			}
			else
			{
				wxMessageBox(wxT("Please draw a layout line."),
					wxT("Missing layout line"), wxOK | wxICON_INFORMATION);
			}
		}
	}
	else
	{
		// it should not be possible to call this function for a non-tree layer
		Log::Inst().Error("OnLayoutTree(): Unhandle case encountered.");
	}

	return false;
}

bool LayerTreeController::OnLayoutTree(GeoTreeView::LAYOUT layout, LayoutEllipsePtr layoutEllipse)
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::TREE)
	{
		// change layout style of tree
		TreeLayerPtr treePtr = boost::dynamic_pointer_cast<TreeLayer>(selectedLayer);

		GeoTreeViewPtr geoTreeView = treePtr->GetGeoTreeView();

		if(layout == GeoTreeView::SLANTED_CLADOGRAM_3D || layout == GeoTreeView::CLADOGRAM_3D || layout == GeoTreeView::SLANTED_PHYLOGRAM_3D)
		{
			geoTreeView->SetLayout(layout);
			App::Inst().GetViewport()->Refresh(false);
			return false;
		}

		// check if a layout ellipse has already been assigned to the tree or not
		if(geoTreeView->IsLayoutEllipse())
		{
			geoTreeView->SetLayout(layout);
			App::Inst().GetViewport()->Refresh(false);
			return false;
		}
		else
		{
			// a layout ellipse is needed so make sure that there is a valid one to assign to the tree
			if(layoutEllipse)
			{
				geoTreeView->SetLayoutEllipse(layoutEllipse);
				geoTreeView->SetLayout(layout);
				App::Inst().GetViewport()->Refresh(false);
				return true;
			}
			else
			{
				wxMessageBox(wxT("Please draw a layout ellipse."),
					wxT("Missing layout ellipse"), wxOK | wxICON_INFORMATION);
			}
		}
	}
	else
	{
		// it should not be possible to call this function for a non-tree layer
		Log::Inst().Error("OnLayoutTree(): Unhandle case encountered.");
	}

	return false;
}

bool LayerTreeController::OnAssignGeoAxisToTree(GeoAxisPolylinePtr geoAxisPolyline)
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::TREE)
	{
		// assign geographical axis to tree
		TreeLayerPtr treePtr = boost::dynamic_pointer_cast<TreeLayer>(selectedLayer);

		GeoTreeViewPtr geoTreeView = treePtr->GetGeoTreeView();

		// check that the geographical axis has been defined before assigning it to the tree
		if(geoAxisPolyline)
		{
			geoTreeView->SetGeographicAxis(geoAxisPolyline);
			App::Inst().GetViewport()->Refresh(false);
			return true;
		}
		else
		{
			wxMessageBox(wxT("A geographic axis polyline must be constructed before it can be assigned to a tree."),
				wxT("Missing geographical axis"), wxOK | wxICON_INFORMATION);
		}
	}
	else
	{
		// it should not be possible to call this function for a non-tree layer
		Log::Inst().Error("OnAssignGeoAxisToTree(): Unhandle case encountered.");
	}

	return false;
}

void LayerTreeController::OnReverseGeoAxisOrder(bool state)
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::TREE)
	{
		// assign geographical axis to tree
		TreeLayerPtr treePtr = boost::dynamic_pointer_cast<TreeLayer>(selectedLayer);

		GeoTreeViewPtr geoTreeView = treePtr->GetGeoTreeView();

		geoTreeView->SetReverseOrderGeographicPts(state);
	}
	else
	{
		// it should not be possible to call this function for a non-tree layer
		Log::Inst().Error("OnReverseGeoAxisOrder(): Unhandle case encountered.");
	}
}

bool LayerTreeController::OnLayoutCharts(ChartSetView::LAYOUT_CHART layout, LayoutLinePtr layoutLine)
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::LOCATION_SET)
	{
		// assign layout line to chart set
		LocationSetLayerPtr locationSetLayer = boost::dynamic_pointer_cast<LocationSetLayer>(selectedLayer);

		ChartSetViewPtr chartSetView = locationSetLayer->GetChartSetView();

		// check if a layout line has already been assigned 
		if(chartSetView->IsLayoutLine())
		{
			chartSetView->SetLayout(layout);
			App::Inst().GetViewport()->Refresh(false);
			return false;
		}
		else
		{
			// a layout line is needed so make sure that there is a valid one to assign
			if(layoutLine)
			{
				chartSetView->SetLayoutLine(layoutLine);
				chartSetView->SetLayout(layout);
				App::Inst().GetViewport()->Refresh(false);
				return true;
			}
			else
			{
				wxMessageBox(wxT("Please draw a layout line."),
					wxT("Missing layout line"), wxOK | wxICON_INFORMATION);
			}
		}
	}
	else
	{
		// it should not be possible to call this function for a location set layer
		Log::Inst().Error("OnLayoutCharts(): Unhandle case encountered.");
	}

	return false;
}

bool LayerTreeController::OnLayoutCharts(ChartSetView::LAYOUT_CHART layout, LayoutEllipsePtr layoutEllipse)
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::LOCATION_SET)
	{
		// assign layout ellipse to chart set
		LocationSetLayerPtr locationSetLayer = boost::dynamic_pointer_cast<LocationSetLayer>(selectedLayer);

		ChartSetViewPtr chartSetView = locationSetLayer->GetChartSetView();

		// check if a layout ellipse has already been assigned 
		if(chartSetView->IsLayoutEllipse())
		{
			chartSetView->SetLayout(layout);
			App::Inst().GetViewport()->Refresh(false);
			return false;
		}
		else
		{
			// a layout ellipse is needed so make sure that there is a valid one to assign
			if(layoutEllipse)
			{
				chartSetView->SetLayoutEllipse(layoutEllipse);
				chartSetView->SetLayout(layout);
				App::Inst().GetViewport()->Refresh(false);
				return true;
			}
			else
			{
				wxMessageBox(wxT("Please draw a layout ellipse."),
					wxT("Missing layout ellipse"), wxOK | wxICON_INFORMATION);
			}
		}
	}
	else
	{
		// it should not be possible to call this function for a location set layer
		Log::Inst().Error("OnLayoutCharts(): Unhandle case encountered.");
	}

	return false;
}

bool LayerTreeController::OnLayoutCharts(ChartSetView::LAYOUT_CHART layout)
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::LOCATION_SET)
	{
		// assign layout ellipse to chart set
		LocationSetLayerPtr locationSetLayer = boost::dynamic_pointer_cast<LocationSetLayer>(selectedLayer);

		ChartSetViewPtr chartSetView = locationSetLayer->GetChartSetView();

		chartSetView->SetLayout(layout);
		App::Inst().GetViewport()->Refresh(false);
		return true;
	}
	else
	{
		// it should not be possible to call this function for a location set layer
		Log::Inst().Error("OnLayoutCharts(): Unhandle case encountered.");
	}

	return false;
}

void LayerTreeController::OnTreeLayoutLineVisibility()
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::TREE)
	{
		// change visibility of layout line
		TreeLayerPtr treePtr = boost::dynamic_pointer_cast<TreeLayer>(selectedLayer);
		GeoTreeViewPtr geoTreeView = treePtr->GetGeoTreeView();
		geoTreeView->SetLayoutLineVisibility(!geoTreeView->GetLayoutLineVisibility());		

		App::Inst().GetViewport()->Refresh(false);
	}
	else
	{
		// it should not be possible to call this function for a non-tree layer
		Log::Inst().Error("OnTreeLayoutLineVisibility(): Unhandle case encountered.");
	}
}

void LayerTreeController::OnTreeFlip()
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::TREE)
	{
		// change visibility of layout line
		TreeLayerPtr treePtr = boost::dynamic_pointer_cast<TreeLayer>(selectedLayer);
		GeoTreeViewPtr geoTreeView = treePtr->GetGeoTreeView();
		geoTreeView->OnTreeFlip();		

		App::Inst().GetViewport()->Refresh(false);
	}
	else
	{
		// it should not be possible to call this function for a non-tree layer
		Log::Inst().Error("OnTreeFlip(): Unhandle case encountered.");
	}
}

void LayerTreeController::OnTreeLayoutEllipseVisibility()
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::TREE)
	{
		// change visibility of layout ellipse
		TreeLayerPtr treePtr = boost::dynamic_pointer_cast<TreeLayer>(selectedLayer);
		GeoTreeViewPtr geoTreeView = treePtr->GetGeoTreeView();
		geoTreeView->SetLayoutEllipseVisibility(!geoTreeView->GetLayoutEllipseVisibility());		

		App::Inst().GetViewport()->Refresh(false);
	}
	else
	{
		// it should not be possible to call this function for a non-tree layer
		Log::Inst().Error("OnTreeLayoutEllipseVisibility(): Unhandle case encountered.");
	}
}

void LayerTreeController::OnTreeGeographicAxisVisibility()
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::TREE)
	{
		// change visibility of geographic axis
		TreeLayerPtr treePtr = boost::dynamic_pointer_cast<TreeLayer>(selectedLayer);
		GeoTreeViewPtr geoTreeView = treePtr->GetGeoTreeView();
		geoTreeView->SetGeographicAxisVisibility(!geoTreeView->GetGeographicAxisVisibility());		

		App::Inst().GetViewport()->Refresh(false);
	}
	else
	{
		// it should not be possible to call this function for a non-tree layer
		Log::Inst().Error("OnTreeGeographicAxisVisibility(): Unhandle case encountered.");
	}
}

void LayerTreeController::OnChartSetLayoutLine()
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::LOCATION_SET)
	{
		// change layout style of tree
		LocationSetLayerPtr locationSet = boost::dynamic_pointer_cast<LocationSetLayer>(selectedLayer);
		ChartSetViewPtr chartSetView = locationSet->GetChartSetView();
		chartSetView->SetLayoutLineVisibility(!chartSetView->GetLayoutLineVisibility());		

		App::Inst().GetViewport()->Refresh(false);
	}
	else
	{
		// it should not be possible to call this function for a non-tree layer
		Log::Inst().Error("OnChartSetLayoutLine(): Unhandle case encountered.");
	}
}

void LayerTreeController::OnChartSetLayoutEllipse()
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::LOCATION_SET)
	{
		// change layout style of tree
		LocationSetLayerPtr locationSet = boost::dynamic_pointer_cast<LocationSetLayer>(selectedLayer);
		ChartSetViewPtr chartSetView = locationSet->GetChartSetView();
		chartSetView->SetLayoutEllipseVisibility(!chartSetView->GetLayoutEllipseVisibility());		

		App::Inst().GetViewport()->Refresh(false);
	}
	else
	{
		// it should not be possible to call this function for a non-tree layer
		Log::Inst().Error("OnChartSetLayoutEllipse(): Unhandle case encountered.");
	}
}

void LayerTreeController::OnLeafNodeLabels()
{
	LayerPtr selectedLayer = GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::TREE)
	{
		// change layout style of tree
		TreeLayerPtr treePtr = boost::dynamic_pointer_cast<TreeLayer>(selectedLayer);
		GeoTreeViewPtr geoTreeView = treePtr->GetGeoTreeView();
		geoTreeView->SetLabelVisibility(!geoTreeView->GetLabelVisibility());		

		App::Inst().GetViewport()->Refresh(false);
	}
	else
	{
		// it should not be possible to call this function for a non-tree layer
		Log::Inst().Error("OnLeafNodeLabels(): Unhandle case encountered.");
	}
}

bool LayerTreeController::MouseLeftDown(const Point2D& mousePt)
{
	// Note: Although it might seem this function could return once if finds a valid hit,
	// this is not the case. Many visual objects need to be informed when a new object is
	// selected (i.e., that they have become unselected).
	bool bHit = false;

	App::Inst().SetSelectedView(ViewPtr());

	// iterate through all views in location set layers
	for(uint i = 0; i < GetNumLocationSetLayers(); ++i)
	{
		LocationSetLayerPtr locationSetLayer = GetLocationSetLayer(i);

		if(locationSetLayer->IsActive())
		{
			if(locationSetLayer->GetChartSetView()->MouseLeftDown(mousePt))
			{
				App::Inst().SetSelectedView(locationSetLayer->GetChartSetView());
				bHit = true;
			}
		}
	}

	// iterate through all views in location layers
	for(uint i = 0; i < GetNumLocationLayers(); ++i)
	{
		LocationLayerPtr locationLayer = GetLocationLayer(i);

		if(locationLayer->IsActive())
		{
			LocationControllerPtr locationController = locationLayer->GetLocationController();

			// check chart view
			if(locationController->GetChartView()->MouseLeftDown(mousePt))
			{
				App::Inst().SetSelectedView(locationController->GetChartView());
				bHit = true;
				break;
			}

			// check location view
			if(locationController->GetLocationView()->MouseLeftDown(mousePt))
			{
				App::Inst().SetSelectedView(locationController->GetLocationView());
				bHit = true;
				break;
			}
		}
	}

	// iterate through all views in tree layers
	for(uint i = 0; i < GetNumTreeLayers(); ++i)
	{
		TreeLayerPtr treeLayer = GetTreeLayer(i);

		if(treeLayer->IsActive())
		{
			// check tree view
			if(treeLayer->GetGeoTreeView()->MouseLeftDown(mousePt))
			{
				App::Inst().SetSelectedView(treeLayer->GetGeoTreeView());
				bHit = true;
				break;
			}
		}
	}

	return bHit;
}

bool LayerTreeController::MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu, LayerPtr& layerPtr)
{
	// Note: Although it would seem this function could return once if finds a valid hit,
	// this is not the case. Many visual objects need to be informed when a new object is
	// selected. 
	bool bHit = false;

	// iterate through all views in tree layers
	for(uint i = 0; i < GetNumTreeLayers(); ++i)
	{
		TreeLayerPtr treeLayer = GetTreeLayer(i);

		if(treeLayer->IsActive())
		{
			// check tree view
			if(treeLayer->GetGeoTreeView()->MouseRightDown(mousePt, popupMenu))
			{
				if(!bHit)
				{
					App::Inst().SetRightClickView(treeLayer->GetGeoTreeView());
					layerPtr = treeLayer;
				}

				bHit = true;
			}
		}
	}

	// iterate through all views in location set layers
	for(uint i = 0; i < GetNumLocationSetLayers(); ++i)
	{
		LocationSetLayerPtr locationSetLayer = GetLocationSetLayer(i);

		if(locationSetLayer->IsActive())
		{
			if(locationSetLayer->GetChartSetView()->MouseRightDown(mousePt, popupMenu))
			{
				if(!bHit)
				{
					App::Inst().SetRightClickView(locationSetLayer->GetChartSetView());
					layerPtr = locationSetLayer;
				}

				bHit = true;
			}
		}
	}

	// iterate through all views in location layers
	for(uint i = 0; i < GetNumLocationLayers(); ++i)
	{
		LocationLayerPtr locationLayer = GetLocationLayer(i);

		if(locationLayer->IsActive())
		{
			// check location view
			if(locationLayer->GetLocationController()->GetLocationView()->MouseRightDown(mousePt, popupMenu))
			{
				if(!bHit)
				{
					App::Inst().SetRightClickView(locationLayer->GetLocationController()->GetLocationView());
					layerPtr = locationLayer;
				}

				bHit = true;
			}

			// check chart view
			if(locationLayer->GetLocationController()->GetChartView()->MouseRightDown(mousePt, popupMenu))
			{
				if(!bHit)
				{
					App::Inst().SetRightClickView(locationLayer->GetLocationController()->GetChartView());
					layerPtr = locationLayer;
				}

				bHit = true;
			}
		}
	}

	return bHit;
}

LayerTreeModelPtr LayerTreeController::GetLayerTreeModel() const
{
	return m_layerTreeModel;
}

void LayerTreeController::UpdateActiveState()
{
	m_treeView->GetTreeCtrl()->Freeze();

	wxTreeItemId rootId = m_treeView->GetTreeCtrl()->GetRootItem();
	wxTreeItemIdValue cookie;
	wxTreeItemId studyId = m_treeView->GetTreeCtrl()->GetFirstChild(rootId, cookie);
	UpdateActiveState(studyId, cookie);

	m_treeView->GetTreeCtrl()->Thaw();
}

void LayerTreeController::UpdateActiveState(wxTreeItemId subtreeId, wxTreeItemIdValue& cookie)
{
	if(!subtreeId.IsOk())
		return;

	wxTreeItemData* currentData = m_treeView->GetTreeCtrl()->GetItemData(subtreeId);
	if(currentData != NULL)
	{
		LayerTreeItem* currentItem = (LayerTreeItem*)currentData;
		LayerPtr currentLayer = currentItem->GetLayer();
		if(currentLayer->IsActive())
		{
			if(currentLayer->GetType() != Layer::SEQUENCE)
			{
				m_treeView->GetTreeCtrl()->SetItemImage(subtreeId, LayerTreeView::CHECKED);
				m_treeView->GetTreeCtrl()->SetItemTextColour(subtreeId, *wxBLACK);
			}
			else
			{
				m_treeView->GetTreeCtrl()->SetItemImage(subtreeId, LayerTreeView::SEQ_CHECKED);
				m_treeView->GetTreeCtrl()->SetItemTextColour(subtreeId, *wxBLACK);
			}
		}
		else
		{
			if(currentLayer->GetType() != Layer::SEQUENCE)
			{
				m_treeView->GetTreeCtrl()->SetItemImage(subtreeId, LayerTreeView::UNCHECKED);
				m_treeView->GetTreeCtrl()->SetItemTextColour(subtreeId, *wxLIGHT_GREY);
			}
			else
			{
				m_treeView->GetTreeCtrl()->SetItemImage(subtreeId, LayerTreeView::SEQ_UNCHECKED);
				m_treeView->GetTreeCtrl()->SetItemTextColour(subtreeId, *wxLIGHT_GREY);
			}
		}
	}

	if(subtreeId == m_treeView->GetTreeCtrl()->GetRootItem() || m_treeView->GetTreeCtrl()->ItemHasChildren(subtreeId))
	{
		wxTreeItemId currentItemId = m_treeView->GetTreeCtrl()->GetFirstChild(subtreeId, cookie);
		UpdateActiveState(currentItemId, cookie);
	}

	wxTreeItemId currentItemId = m_treeView->GetTreeCtrl()->GetNextSibling(subtreeId);
	UpdateActiveState(currentItemId, cookie);
}

void LayerTreeController::SetIsBlankRaster(bool isBlank)
{
	m_isBlankRaster = isBlank;
}

bool LayerTreeController::GetIsBlankRaster()
{
	return m_isBlankRaster;
}
