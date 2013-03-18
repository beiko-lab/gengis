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
#include "../core/SequenceLayer.hpp"
#include "../core/LocationController.hpp"

using namespace GenGIS;

template<class Archive>
void LocationLayer::serialize(Archive & ar, const unsigned int version)
{
	ar & boost::serialization::base_object<Layer>(*this);
	ar & m_sequence; // std::vector<SequenceLayerPtr>
}
template void LocationLayer::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void LocationLayer::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void LocationLayer::RemoveSequenceLayerByIndex(unsigned int index)
{
	m_sequence.erase(m_sequence.begin() + index);
}

LayerPtr LocationLayer::GetLayer(uint id) const
{
	for(unsigned int index = 0; index < GetNumSequenceLayers(); index++)
	{
		SequenceLayerPtr layer = GetSequenceLayer(index);
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

bool LocationLayer::RemoveSequenceLayerById(uint id)
{
	std::vector<SequenceLayerPtr>::iterator iter;
	for ( iter = m_sequence.begin(); iter != m_sequence.end(); iter++ )
	{
		if( (*iter)->GetId() == id )
		{
			m_sequence.erase( iter );
			return true;
		}
	}
	return false;
}

bool LocationLayer::RemoveLayer(uint id)
{
	bool bFound = false;
	for(unsigned int index = 0; index < GetNumSequenceLayers() && !bFound; index++)
	{
		SequenceLayerPtr layer = GetSequenceLayer(index);
		if(layer->GetId() == id)
		{
			RemoveSequenceLayerByIndex(index);
			return true;
		}

		bFound = layer->RemoveLayer(id);
	}

	return bFound;
}

void LocationLayer::Render()
{
	if(!IsActive())
		return;

	m_locationController->Render();

	for(unsigned int index = 0; index < GetNumSequenceLayers(); index++)
	{
		GetSequenceLayer(index)->Render();
	}
}

std::vector<SequenceLayerPtr> LocationLayer::GetAllActiveSequenceLayers() const 
{ 
	std::vector<SequenceLayerPtr> activeSeqLayers;

	foreach(SequenceLayerPtr seqLayer, m_sequence)
	{
		if(seqLayer->IsActive())
			activeSeqLayers.push_back(seqLayer);
	}

	return activeSeqLayers;
}
