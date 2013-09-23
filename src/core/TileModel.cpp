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

#include "../core/LocationLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/TileModel.hpp"
#include "../core/LocationModel.hpp"
//#include "../core/LocationController.hpp"

using namespace GenGIS;

void TileModel::AddLocationLayer(LocationLayerPtr location)
{
	// Update number of locations in Tile
	numLocations++;
	// Add sequence layers to tile?
	std::vector<SequenceLayerPtr> sequences = location->GetAllSequenceLayers();
	UpdateSequences(sequences);
	// Add fields to tile
	UpdateData(location->GetLocationController()->GetData());
}

template<class Archive> 
void TileModel::serialize(Archive & ar, const unsigned int version)
{
	// Serialize base class (LocationModel)
	ar & boost::serialization::base_object<LocationModel>(*this);

	ar & numLocations;  // uint
	ar & m_bActive;     // bool
	ar & m_topLeft;		// std::pair<float,float>
	ar & m_bottomRight;	// std::pair<float,float>
}
template void TileModel::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void TileModel::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void TileModel::UpdateData(std::map<std::wstring,std::wstring> newData)
{
	std::map<std::wstring,std::wstring>::iterator dataIter;
	for(dataIter = newData.begin(); dataIter != newData.end(); ++dataIter)
	{
		std::wstring field = dataIter->first;
		std::wstring value = dataIter->second;
		// check if tile's data currently has a value
		// if not make a new one
		
		//	USE EMPLACE INSTEAD
		
		if( FindField(field) == 1 )
		{
			std::wstring curValue = GetData( field );
			if(StringTools::IsDecimalNumber( value ))
			{
				curValue =  StringTools::ToStringW( ( StringTools::ToInt(curValue) + StringTools::ToInt(value) ) / 2 );
				AddData(field, curValue);
			}
		}
		else
		{
			AddData(field,value);
		}
	}
}

void TileModel::UpdateSequences(std::vector<SequenceLayerPtr> sequences)
{
	for(uint i = 0; i < sequences.size(); i++)
	{
		AddSequence(sequences[i]->GetSequenceController()->GetSequenceModel());
	}
}