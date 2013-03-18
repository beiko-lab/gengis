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

#include "../core/App.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/SequenceController.hpp"
#include "../core/SequenceLayer.hpp"

#include "../utils/StringTools.hpp"

using namespace GenGIS;

std::wstring SequenceController::m_activeField = _T("");

template<class Archive>
void SequenceController::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_activeField; // static std::wstring
}
template void SequenceController::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void SequenceController::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

std::vector<std::wstring> SequenceController::GetMetadataFields() const
{
	std::vector<std::wstring> fields;

	std::map<std::wstring,std::wstring> metadata = GetData();
	std::map<std::wstring,std::wstring>::iterator it;
	for(it = metadata.begin(); it != metadata.end(); ++it)
		fields.push_back(it->first);

	return fields;
}

std::vector<std::wstring> SequenceController::GetNumericMetadataFields() const
{
	std::vector<SequenceLayerPtr> seqLayers = App::Inst().GetLayerTreeController()->GetSequenceLayers();

	if(seqLayers.size() == 0)
	{
		Log::Inst().Error("(Error) SequenceController::GetNumericMetadataFields(): no sequence layers.");
		return std::vector<std::wstring>();
	}

	std::vector<std::wstring> numericFields;
	foreach(const std::wstring& field, GetMetadataFields())
	{
		bool bNumeric = true;
		foreach(SequenceLayerPtr seqLayer, seqLayers)
		{
			std::wstring value = seqLayer->GetSequenceController()->GetData()[field];
			
			if(!StringTools::IsDecimalNumber(value))
			{
				bNumeric = false;
				break;
			}
		}

		if(bNumeric)
			numericFields.push_back(field);
	}

	return numericFields;
}
