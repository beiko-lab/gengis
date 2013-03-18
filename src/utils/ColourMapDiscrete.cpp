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
#include "../utils/ColourMapDiscrete.hpp"
#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"

using namespace GenGIS;

ColourMapDiscrete::ColourMapDiscrete(ColourMapPtr colourMap)
{
	m_colourMap.clear();
	for(uint i = 0; i < colourMap->GetSize(); ++i)
		m_colourMap.push_back(colourMap->GetColour(i));		

	m_name = colourMap->GetName();

	// Note: the type of a discrete colour map can be CONTINUOUS
	// as the type refers to the underlying colour map and how
	// colours should be obtained from it
	m_type = colourMap->GetType();
}

ColourMapDiscrete::ColourMapDiscrete(ColourMapDiscretePtr colourMap)
{
	CopyColourMap(colourMap);
}

template<class Archive>
void ColourMapDiscrete::serialize(Archive & ar, const unsigned int file_version)
{
	ar & boost::serialization::base_object<ColourMap>(*this);
	ar & m_colourNames; // std::map<std::wstring, Colour>
}
template void ColourMapDiscrete::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void ColourMapDiscrete::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void ColourMapDiscrete::CopyColourMap(ColourMapDiscretePtr colourMap)
{
	m_colourMap.clear();
	for(uint i = 0; i < colourMap->GetSize(); ++i)
		m_colourMap.push_back(colourMap->GetColour(i));		

	m_colourNames = colourMap->GetNameToColourMap();

	m_name = colourMap->GetName();

	// Note: the type of a discrete colour map can be CONTINUOUS
	// as the type refers to the underlying colour map and how
	// colours should be obtained from it
	m_type = colourMap->GetType();

	m_bCustom = colourMap->IsCustom();
}

void ColourMapDiscrete::SetColour(const std::wstring& name, const Colour& colour) 
{ 
	std::map<std::wstring, Colour>::iterator it = m_colourNames.find(name);
	if(it != m_colourNames.end())
	{
		it->second = colour;
	}
	else
	{
		m_colourNames[name] = colour;
	}
}

bool ColourMapDiscrete::GetColour(const std::wstring& name, Colour& colour) 
{
	std::map<std::wstring, Colour>::const_iterator it = m_colourNames.find(name);
	if(it == m_colourNames.end())
	{
		colour = Colour(0.0f,0.0f,0.0f,1.0f);
		m_colourNames[name] = colour;
		SetCustom(true);
		return false;
	}

	colour = it->second;

	return true;
}
