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

#include "../core/LocationLayer.hpp"
#include "../core/LocationView.hpp"
#include "../core/NodeGeoTree.hpp"

#include "../utils/TreeTools.hpp"

using namespace GenGIS;

template<class Archive>
void NodeGeoTree::serialize(Archive & ar, const unsigned int version)
{
	ar & boost::serialization::base_object<NodePhylo>(*this);
	ar & m_gridCoord;       // Point3D
	ar & m_locationLayerId; // uint
	ar & m_colour;          // Colour
	ar & m_bProcessed;      // bool
	ar & m_baryCenter;      // double
	ar & m_layoutPos;       // uint
	ar & m_layoutDist;      // float
	ar & m_bSelected;       // bool
	ar & m_crossings;       // uint
	ar & m_bActive;         // bool
}
template void NodeGeoTree::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void NodeGeoTree::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


std::vector<NodeGeoTree*> NodeGeoTree::GetNeighbors() const
{
	std::vector<NodeGeoTree*> neighbors;

	NodeGeoTree* parent = GetParent();
	for(unsigned int i = 0; i < parent->GetNumberOfChildren(); i++) 
	{
		if(parent->GetChild(i) != this)
			neighbors.push_back(parent->GetChild(i));
	}

	return neighbors;
}

std::vector<NodeGeoTree*> NodeGeoTree::GetChildren() const
{ 
	std::vector<NodeGeoTree*> children;

	for(unsigned int i = 0; i < GetNumberOfChildren(); ++i)
	{
		children.push_back((NodeGeoTree*)GetChild(i));
	}

	return children; 
}

void NodeGeoTree::SetSelectedSubtree(bool state)
{
	std::vector<NodeGeoTree*> nodes = TreeTools<NodeGeoTree>::GetNodes(this);
	foreach(NodeGeoTree* node, nodes)
		node->SetSelected(state);
}

LocationViewPtr NodeGeoTree::GetLocationView() const
{
	LocationLayerPtr locationLayer = App::Inst().GetLayerTreeController()->GetLocationLayerById( m_locationLayerId );

	// If the location layer is found, return a pointer to its corresponding location view class
	if ( locationLayer )
		return locationLayer->GetLocationController()->GetLocationView();

	// Return NULL pointer if the location is not found
	return LocationViewPtr();
}

void NodeGeoTree::SetLocationView( LocationViewPtr locationView )
{
	if ( locationView )
		m_locationLayerId = locationView->GetLocationLayerId();
}