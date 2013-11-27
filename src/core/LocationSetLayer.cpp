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

#include "../core/LocationSetLayer.hpp"
#include "../core/App.hpp"
#include "../core/MapView.hpp"
#include "../core/MapLayer.hpp"
#include "../core/ChartSetView.hpp"
#include "../core/LocationGrid.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/SequenceController.hpp"
#include "../core/LocationSetController.hpp"

#include "../utils/ColourMapManager.hpp"
#include "../utils/ColourMap.hpp"
#include "../utils/ColourMapDiscrete.hpp"
#include "../utils/ShapeMapManager.hpp"
#include "../utils/ShapeMap.hpp"
#include "../utils/StringTools.hpp"

using namespace GenGIS;

LocationSetLayer::LocationSetLayer(uint id, LayerPtr parent, ChartSetViewPtr chartSetView) :
	Layer(id, Layer::LOCATION_SET, parent), m_chartSetView(chartSetView), m_locationGrid(new LocationGrid()),
		m_locationSetController(new LocationSetController())
{

}

template<class Archive>
void LocationSetLayer::serialize(Archive & ar, const unsigned int version)
{
	ar & boost::serialization::base_object<Layer>(*this);
	ar & m_locationGrid;          // LocationGridPtr
	ar & m_locationLayers;        // std::vector<LocationLayerPtr>
	ar & m_locationSetController; // LocationSetControllerPtr
}
template void LocationSetLayer::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void LocationSetLayer::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void LocationSetLayer::AddLocationLayer(LocationLayerPtr locationLayer) 
{ 
	m_locationLayers.push_back(locationLayer);
}

LocationLayerPtr LocationSetLayer::GetLocationLayer(const std::wstring& name) const
{
	std::vector<LocationLayerPtr>::const_iterator iter;
	for(iter = m_locationLayers.begin(); iter != m_locationLayers.end(); iter++)
	{
		if((*iter)->GetName() == name)
		{
			return *iter;
		}
	}

	return LocationLayerPtr();
}

void LocationSetLayer::RemoveLocationLayerByIndex(unsigned int index)
{
	m_locationLayers.at(index)->RemoveAllSequenceLayers();
	m_locationLayers.at(index)->GetLocationController()->GetLocationModel()->RemoveAllSequence();
	m_locationLayers.erase(m_locationLayers.begin() + index);
}

void LocationSetLayer::RemoveLocationLayerByName(const std::wstring& name)
{
	std::vector<LocationLayerPtr>::iterator iter;
	for(iter = m_locationLayers.begin(); iter != m_locationLayers.end(); iter++)
	{
		if((*iter)->GetName() == name)
		{
			(*iter)->RemoveAllSequenceLayers();
			m_locationLayers.erase(iter);
			break;
		}
	}
}

void LocationSetLayer::RemoveAllLocationLayers()
{
	std::vector<LocationLayerPtr>::iterator iter;
	for(iter = m_locationLayers.begin(); iter != m_locationLayers.end(); iter++)
		(*iter)->RemoveAllSequenceLayers();
	m_locationLayers.clear();

	m_locationSetController->RemoveAllLocationLayers();
}

LayerPtr LocationSetLayer::GetLayer(uint id) const
{
	for(unsigned int index = 0; index < GetNumLocationLayers(); index++)
	{
		LocationLayerPtr layer = GetLocationLayer(index);
		if(layer->GetId() == id)
			return layer;

		LayerPtr returnLayer = layer->GetLayer(id);
		if(returnLayer != LayerPtr())
		{
			return returnLayer;
		}
	}

	return LayerPtr();
}

bool LocationSetLayer::RemoveLayer(uint id)
{
	bool bFound = false;
	for(unsigned int index = 0; index < GetNumLocationLayers() && !bFound; index++)
	{
		LocationLayerPtr layer = GetLocationLayer(index);
		if(layer->GetId() == id)
		{
			RemoveLocationLayerByIndex(index);
			return true;
		}

		bFound = layer->RemoveLayer(id);
	}

	return bFound;
}

bool LocationSetLayer::RemoveLocationLayerById(uint id)
{
	std::vector<LocationLayerPtr>::iterator iter;
	for ( iter = m_locationLayers.begin(); iter != m_locationLayers.end(); iter++ )
	{
		if( (*iter)->GetId() == id )
		{
			(*iter)->RemoveAllSequenceLayers();
			m_locationLayers.erase( iter );
			return true;
		}
	}
	return false;
}

void LocationSetLayer::Render()
{
	if(!IsActive())
		return;

	m_chartSetView->Render();
	m_locationGrid->Render();

	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		locationLayer->Render();
	}
}

bool LocationSetLayer::IsSequencesData()
{
	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		if(locationLayer->GetNumSequenceLayers() > 0)
			return true;
	}

	return false;
}

std::vector<LocationLayerPtr> LocationSetLayer::GetAllActiveLocationLayers() const
{
	std::vector<LocationLayerPtr> activeLocLayers;

	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		if(locationLayer->IsActive())
			activeLocLayers.push_back(locationLayer);
	}

	return activeLocLayers;
}

void LocationSetLayer::SetActive(bool state) 
{ 
	m_locationSetController->SetActive(state);
}

bool LocationSetLayer::IsActive() const 
{ 
	return m_locationSetController->IsActive(); 
}

void LocationSetLayer::ToggleActive() 
{ 
	m_locationSetController->ToggleActive(); 
}
