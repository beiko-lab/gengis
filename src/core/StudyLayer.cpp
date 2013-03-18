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

#include "../core/StudyLayer.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/MapLayer.hpp"
#include "../core/MapController.hpp"
#include "../core/MapView.hpp"
#include "../core/VectorMapLayer.hpp"
#include "../core/VectorMapController.hpp"

using namespace GenGIS;

template<class Archive>
void StudyLayer::serialize(Archive & ar, const unsigned int version)
{
	ar & boost::serialization::base_object<Layer>(*this);
	ar & m_maps;       // std::vector<MapLayerPtr>
	ar & m_vectorMaps; // std::vector<VectorMapLayerPtr>
}
template void StudyLayer::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void StudyLayer::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

MapLayerPtr StudyLayer::GetMapLayer(const std::wstring& name) const
{
	std::vector<MapLayerPtr>::const_iterator iter;
	for(iter = m_maps.begin(); iter != m_maps.end(); iter++)
	{
		if((*iter)->GetName() == name)
		{
			return *iter;
		}
	}

	return MapLayerPtr();
}

MapLayerPtr StudyLayer::GetMapLayerById( unsigned int id ) const
{
	std::vector<MapLayerPtr>::const_iterator iter;
	for(iter = m_maps.begin(); iter != m_maps.end(); iter++)
	{
		if( (*iter)->GetId() == id )
		{
			return *iter;
		}
	}

	return MapLayerPtr();
}

void StudyLayer::RemoveMapLayerByIndex(unsigned int index)
{
	// erase study and all children 
	for(unsigned int i = 0; i < m_maps.at(index)->GetNumLocationSetLayers(); ++i)
	{
		// keep removing the first element
		m_maps.at(index)->RemoveLocationSetLayerByIndex(0);
	}

	m_maps.erase(m_maps.begin() + index);
}

bool StudyLayer::RemoveMapLayerById( unsigned int layerId )
{
	std::vector<MapLayerPtr>::iterator iter;
	for(iter = m_maps.begin(); iter != m_maps.end(); iter++)
	{
		if((*iter)->GetId() == layerId)
		{
			(*iter)->RemoveAllLocationSetLayers();
			m_maps.erase( iter );
			return true;
		}
	}

	return false;
}

void StudyLayer::RemoveMapLayerByName(const std::wstring& name)
{
	std::vector<MapLayerPtr>::iterator iter;
	for(iter = m_maps.begin(); iter != m_maps.end(); iter++)
	{
		if((*iter)->GetName() == name)
		{
			// erase study and all children 
			for(unsigned int i = 0; i < (*iter)->GetNumLocationSetLayers(); ++i)
			{
				// keep removing the first element
				(*iter)->RemoveLocationSetLayerByIndex(0);
			}

			m_maps.erase(iter);
			break;
		}
	}
}

void StudyLayer::RemoveAllMapLayers()
{
	std::vector<MapLayerPtr>::iterator iter;
	for ( iter = m_maps.begin(); iter != m_maps.end(); iter++ )
		(*iter)->RemoveAllLocationSetLayers();
	m_maps.clear();
}

void StudyLayer::RemoveAllVectorMapLayers()
{
	m_vectorMaps.clear();
}

bool StudyLayer::RemoveVectorMapLayerById( unsigned int layerId )
{
	std::vector<VectorMapLayerPtr>::iterator iter;
	for(iter = m_vectorMaps.begin(); iter != m_vectorMaps.end(); iter++)
	{
		if((*iter)->GetId() == layerId)
		{
			//(*iter)->RemoveAllLocationSetLayers();
			m_vectorMaps.erase( iter );
			return true;
		}
	}

	return false;
}


LayerPtr StudyLayer::GetLayer(uint id) const
{
	for(unsigned int index = 0; index < GetNumMapLayers(); index++)
	{
		MapLayerPtr layer = GetMapLayer(index);
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

bool StudyLayer::RemoveLayer(uint id)
{
	bool bFound = false;
	for(unsigned int index = 0; index < GetNumMapLayers() && !bFound; index++)
	{
		MapLayerPtr layer = GetMapLayer(index);
		if(layer->GetId() == id)
		{
			RemoveMapLayerByIndex(index);
			return true;
		}

		bFound = layer->RemoveLayer(id);
	}

	return bFound;
}

void StudyLayer::Render()
{
	if(!IsActive())
		return;

	for ( unsigned int index = 0; index < GetNumMapLayers(); index++ )
	{
		GetMapLayer(index)->Render();
	}

	for ( unsigned int index = 0; index < GetNumVectorMapLayers(); index++ )
	{
		GetVectorMapLayer( index )->Render();
	}
}
