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
#include "../utils/ColourMap.hpp"
#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"

using namespace GenGIS;

ColourMap::ColourMap(ColourMapPtr colourMap)
{
	CopyColourMap(colourMap);
}

template<class Archive>
void ColourMap::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_colourMap; // std::vector<Colour>
	ar & m_name;      // std::wstring
	ar & m_type;      // COLOUR_MAP_TYPE
	ar & m_bCustom;   // bool
}
template void ColourMap::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void ColourMap::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void ColourMap::CopyColourMap(ColourMapPtr colourMap)
{
	m_colourMap.clear();

	m_type = colourMap->GetType();
	m_name = colourMap->GetName();

	for(uint i = 0; i < colourMap->GetSize(); ++i)
		m_colourMap.push_back(colourMap->GetColour(i));
}

void ColourMap::SetColour(const Colour& colour, uint index)
{
	if(index >= m_colourMap.size())
		m_colourMap.resize(index+1, Colour(0,0,0));

	m_colourMap.at(index) = colour;
}

Colour ColourMap::GetColour(uint index) const
{ 
	if(index < m_colourMap.size())
	{
		return m_colourMap.at(index);
	}

	return Colour(0.0f,0.0f,0.0f,1.0f);
}

Colour ColourMap::GetInterpolatedColour(float value, float minValue, float maxValue) const
{
	if(maxValue == minValue)
	{
		return m_colourMap.at(0);
	}

	// determine offset of desired colour between [0, 1]
	float delta = (value - minValue) / (maxValue - minValue);

	// determine colour "bin" to interpolate colour over
	float binSize = 1.0 / (m_colourMap.size()-1);
	uint binIndex = floor(delta / binSize);
	if(value == maxValue)
	{
		// degenerate case that doesn't work with above formula
		binIndex = m_colourMap.size()-2;
	}

	// interpolate colour over colour bin
	delta = (delta - binIndex*binSize) / binSize;
	Colour startColour = m_colourMap.at(binIndex);
	Colour endColour = m_colourMap.at(binIndex+1);
	float red = startColour.GetRed() + (endColour.GetRed()-startColour.GetRed())*delta;
	float green = startColour.GetGreen() + (endColour.GetGreen()-startColour.GetGreen())*delta;
	float blue = startColour.GetBlue() + (endColour.GetBlue()-startColour.GetBlue())*delta;

	return Colour(red, green, blue, 1.0f);
}

uint ColourMap::GetSize() const
{
  return m_colourMap.size();
}

ColourMap::COLOUR_MAP_TYPE ColourMap::ReadType(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if(!file)
	{
		Log::Inst().Error("(Error) ColourMap::ReadType(): invalid colour map file.");
		return UNKNOWN;
	}

	const uint DATA_SIZE = 256;
	char data[DATA_SIZE];
	std::string dataStr;

	// read type of colour map
	file.getline(data, DATA_SIZE);
	dataStr = data;
	uint pos = dataStr.find(":");	
	std::string type = dataStr.substr(pos+1);
	type = StringTools::RemoveSurroundingWhiteSpaces(type);
	if(type == "DISCRETE")
		return DISCRETE;
	else if(type == "CONTINUOUS")
		return CONTINUOUS;

	return UNKNOWN;
}


bool ColourMap::Load(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if(!file)
		return false;

	const uint DATA_SIZE = 256;
	char data[DATA_SIZE];
	std::string dataStr;

	// read type of colour map
	file.getline(data, DATA_SIZE);
	dataStr = data;
	uint pos = dataStr.find(":");	
	std::string type = dataStr.substr(pos+1);
	type = StringTools::RemoveSurroundingWhiteSpaces(type);
	if(type == "DISCRETE")
		m_type = DISCRETE;
	else if(type == "CONTINUOUS")
		m_type = CONTINUOUS;

	// read name of colour map
	file.getline(data, DATA_SIZE);
	dataStr = data;
	pos = dataStr.find(":");	
	m_name = StringTools::ToStringW(dataStr.substr(pos+1));
	m_name = StringTools::RemoveSurroundingWhiteSpaces(m_name);

	// read number of colours in colour map
	file.getline(data, DATA_SIZE);
	dataStr = data;
	pos = dataStr.find(":");	
	std::string numColoursStr = dataStr.substr(pos+1);
	uint numColours = StringTools::ToInt(StringTools::RemoveSurroundingWhiteSpaces(numColoursStr));

	// read in colours
	int red, green, blue;
	for(uint i = 0; i < numColours; ++i)
	{
		file >> red >> green >> blue;
		m_colourMap.push_back(Colour(red, green, blue));
	}

	return true;
}

std::wstring ColourMap::GetExtendedName()
{
	std::wstring prefixed;
	if(m_type == CONTINUOUS)
	{
		prefixed = std::wstring(_T("Continuous: ")) + m_name;
		return prefixed;
	}

	prefixed = std::wstring(_T("Discrete: ")) + m_name;
	return prefixed;
}

std::wstring ColourMap::ReduceExtendedName(const std::wstring& extendedName)
{
	uint pos = extendedName.find(_T("Continuous: "));
	if(pos != std::string::npos)
	{
		pos += 12; // move to end of prefix
	}
	else
	{
		pos = extendedName.find(_T("Discrete: ")); 
		pos += 10; // move to end of prefix
	}
		
	std::wstring reducedName = extendedName.substr(pos).c_str();
	return reducedName;
}

void ColourMap::SetCustom(bool custom) 
{ 
	m_bCustom = custom;
	m_type = DISCRETE;

	if(m_bCustom)
		m_name = _T("Custom");
}
