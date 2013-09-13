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

void TileModel::AddLocationLayer(LocationLayerPtr location){
	// Update number of locations in Tile
	numLocations++;
	// Add sequence layers to tile?
	std::vector<SequenceLayerPtr> sequences = location->GetAllSequenceLayers();
	UpdateSequences(sequences);
	// Add fields to tile
	UpdateData(location->GetLocationController()->GetData());

}

void TileModel::UpdateData(std::map<std::wstring,std::wstring> newData)
{
	std::map<std::wstring,std::wstring>::iterator dataIter;
	for(dataIter = newData.begin(); dataIter != newData.end(); ++dataIter)
	{
		std::wstring field = dataIter->first;
		std::wstring value = dataIter->second;
		// check if tile's data currently has a value
		// if not make a new one
		try
		{
			std::wstring curValue = GetData(field);
			if(StringTools::IsDecimalNumber(value))
			{
				curValue =  StringTools::ToStringW( ( StringTools::ToInt(curValue) + StringTools::ToInt(value) ) / 2 );
				AddData(field, curValue);
			}
		}
		catch(int e)
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