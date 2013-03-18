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

#include "../core/MapLayer.hpp"
#include "../core/TreeLayer.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/MapController.hpp"
#include "../core/MapView.hpp"

#include "../gui/GenGisFrame.hpp"

using namespace GenGIS;

template<class Archive>
void MapLayer::load(Archive & ar, const unsigned int version)
{
	//
}
template void MapLayer::load<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

template<class Archive>
void MapLayer::save(Archive & ar, const unsigned int version) const
{
	//
}
template void MapLayer::save<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version) const; 

template<class Archive>
void MapLayer::serialize(Archive & ar, const unsigned int version)
{
	ar & boost::serialization::base_object<Layer>(*this);
	ar & m_mapController; // MapControllerPtr
	ar & m_locationSets;  // std::vector<LocationSetLayerPtr>
	ar & m_trees;         // std::vector<TreeLayerPtr>

	boost::serialization::split_member(ar, *this, version);
}
template void MapLayer::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void MapLayer::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


LocationSetLayerPtr MapLayer::GetLocationSetLayer(const std::wstring& name) const
{
	std::vector<LocationSetLayerPtr>::const_iterator iter;
	for(iter = m_locationSets.begin(); iter != m_locationSets.end(); iter++)
	{
		if((*iter)->GetName() == name)
		{
			return *iter;
		}
	}

	return LocationSetLayerPtr();
}

void MapLayer::RemoveLocationSetLayerByIndex(unsigned int index)
{
	// erase study and all children 
	for(unsigned int i = 0; i < m_locationSets.at(index)->GetNumLocationLayers(); ++i)
	{
		// keep removing the first element
		m_locationSets.at(index)->RemoveLocationLayerByIndex(0);
	}

	m_locationSets.erase(m_locationSets.begin() + index);
}

void MapLayer::RemoveLocationSetLayerByName(const std::wstring& name)
{
	std::vector<LocationSetLayerPtr>::iterator iter;
	for(iter = m_locationSets.begin(); iter != m_locationSets.end(); iter++)
	{
		if((*iter)->GetName() == name)
		{
			// erase study and all children 
			for(unsigned int i = 0; i < (*iter)->GetNumLocationLayers(); ++i)
			{
				// keep removing the first element
				(*iter)->RemoveLocationLayerByIndex(0);
			}

			m_locationSets.erase(iter);
			break;
		}
	}
}

bool MapLayer::RemoveLocationSetLayerById(uint id)
{
	std::vector<LocationSetLayerPtr>::iterator iter;
	for ( iter = m_locationSets.begin(); iter != m_locationSets.end(); iter++ )
	{
		if ( (*iter)->GetId() == id )
		{
			(*iter)->RemoveAllLocationLayers();
			(*iter)->GetChartSetView()->ClearAllCharts();
			m_locationSets.erase( iter );
			return true;
		}
	}
	return false;
}

void MapLayer::RemoveAllLocationSetLayers()
{
	std::vector<LocationSetLayerPtr>::iterator iter;
	for ( iter = m_locationSets.begin(); iter != m_locationSets.end(); iter++ )
		(*iter)->RemoveAllLocationLayers();
	m_locationSets.clear();
}

void MapLayer::RemoveTreeLayer(unsigned int index)
{
	m_trees.erase(m_trees.begin() + index);
}

void MapLayer::RemoveTreeLayer(const std::wstring& name)
{
	std::vector<TreeLayerPtr>::iterator iter;
	for(iter = m_trees.begin(); iter != m_trees.end(); iter++)
	{
		if((*iter)->GetName() == name)
		{
			m_trees.erase(iter);
			break;
		}
	}
}

LayerPtr MapLayer::GetLayer(uint id) const
{
	for(unsigned int index = 0; index < GetNumLocationSetLayers(); index++)
	{
		LocationSetLayerPtr layer = GetLocationSetLayer(index);
		if(layer->GetId() == id)
			return layer;

		LayerPtr returnLayer = layer->GetLayer(id);
		if(returnLayer != LayerPtr())
		{
			return returnLayer;
		}
	}

	for(unsigned int index = 0; index < GetNumTreeLayers(); index++)
	{
		TreeLayerPtr layer = GetTreeLayer(index);
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

bool MapLayer::RemoveLayer(uint id)
{
	bool bFound = false;

	// Check if layer is a location set:
	for(unsigned int index = 0; index < GetNumLocationSetLayers() && !bFound; index++)
	{
		LocationSetLayerPtr layer = GetLocationSetLayer(index);
		if(layer->GetId() == id)
		{
			RemoveLocationSetLayerByIndex(index);
			return true;
		}

		bFound = layer->RemoveLayer(id);
	}

	// Check if layer is a tree:
	for(unsigned int index = 0; index < GetNumTreeLayers() && !bFound; index++)
	{
		TreeLayerPtr layer = GetTreeLayer(index);
		if(layer->GetId() == id)
		{
			RemoveTreeLayer(index);
			return true;
		}

		bFound = layer->RemoveLayer(id);
	}

	return bFound;
}

void MapLayer::Render()
{
	if(!IsActive())
		return;

	App::Inst().GetMapController()->GetMapView()->Render();

	/*
	// Note: Please leave this code in as it is extremely useful for debugging projection problems.
	// Render a simple point-based map just to verify the actual rendering is correct
	if(GetNumLocationSetLayers() > 0)
	{
		glDisable(GL_LIGHTING);
		
		GLUquadricObj* m_markerQuadric = gluNewQuadric();
		gluQuadricDrawStyle( m_markerQuadric, GLU_FILL );
		gluQuadricNormals( m_markerQuadric, GLU_SMOOTH );

		Point3DArrayPtr grid = App::Inst().GetMapController()->GetGrid();
		for ( int m = 0; m < App::Inst().GetMapController()->GetHeader()->nRows ; m += 25 ) 
		{
			for ( int n = 0; n < App::Inst().GetMapController()->GetHeader()->nCols; n += 25 ) 
			{
				glPushMatrix();
				int index = n + m*App::Inst().GetMapController()->GetHeader()->nCols;
				glTranslated(grid[index].x, grid[index].y, grid[index].z);

				if(grid[index].y > 0.0)
				{
					glColor3f(1.0f,0.1f,0.1f);
				}
				else
				{
					glColor3f(0.1f,0.1f,1.0f);
				}
				gluSphere(m_markerQuadric, 0.01, 10, 10);
				glPopMatrix();
			}
		}

		glEnable(GL_LIGHTING);
	}
	*/

	for(unsigned int index = 0; index < GetNumLocationSetLayers(); index++)
	{
		unsigned int num = m_locationSets.size();
		LocationSetLayerPtr layer = m_locationSets.at(0);

		GetLocationSetLayer(index)->Render();
	}

	for(unsigned int index = 0; index < GetNumTreeLayers(); index++)
	{
		GetTreeLayer(index)->Render();
	}
}

void MapLayer::SetActive(bool state)
{
	//App::Inst().GetMapController()->SetActive(state);
	m_mapController->SetActive(state);
}

bool MapLayer::IsActive() const
{
	//return App::Inst().GetMapController()->IsActive();
	return m_mapController->IsActive();
}

void MapLayer::ToggleActive()
{
	//App::Inst().GetMapController()->ToggleActive();
	m_mapController->ToggleActive();
}

MapControllerPtr MapLayer::GetMapController() const
{
	//return App::Inst().GetMapController();
	return m_mapController;
}