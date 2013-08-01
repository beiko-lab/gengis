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

#include "../utils/ShapeMap.hpp"
#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"

using namespace GenGIS;

ShapeMap::ShapeMap(ShapeMapPtr shapeMap)
{
	CopyShapeMap(shapeMap);
}


template<class Archive>
void ShapeMap::serialize(Archive & ar, const unsigned int version)
{
	ar & m_shapeMap;   // std::vector<VisualMarker::MARKER_SHAPE>
	ar & m_shapeNames; // std::map<std::wstring, VisualMarker::MARKER_SHAPE>
	ar & m_name;       // std::wstring
	ar & m_bCustom;    // bool
}
template void ShapeMap::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void ShapeMap::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


void ShapeMap::CopyShapeMap(ShapeMapPtr shapeMap)
{
	m_name = shapeMap->GetName();

	m_shapeMap.clear();
	for(uint i = 0; i < shapeMap->GetSize(); ++i)
		m_shapeMap.push_back(shapeMap->GetShape(i));

	m_shapeNames = shapeMap->GetNameToShapeMap();
	m_bCustom    = shapeMap->IsCustomShapeMap();
}

void ShapeMap::SetShape(const std::wstring& name, VisualMarker::MARKER_SHAPE shape) 
{ 
	std::map<std::wstring, VisualMarker::MARKER_SHAPE>::iterator it = m_shapeNames.find(name);
	if(it != m_shapeNames.end())
	{
		it->second = shape;
	}
	else
	{
		m_shapeNames[name] = shape;
	}
}

bool ShapeMap::GetShape(std::wstring name, VisualMarker::MARKER_SHAPE& shape) const 
{ 
	std::map<std::wstring, VisualMarker::MARKER_SHAPE>::const_iterator it = m_shapeNames.find(name);
	if(it == m_shapeNames.end())
	{
		shape = m_shapeMap.at(0);
		return false;
	}

	shape = it->second;
	return true;
}

const VisualMarker::MARKER_SHAPE ShapeMap::GetShape(uint index) const
{ 
	if(index < m_shapeMap.size())
	{
		return m_shapeMap.at(index);
	}

	return VisualMarker::CIRCLE;
}

void ShapeMap::SetCustom(bool custom) 
{ 
	m_bCustom = custom;

	if(m_bCustom)
		m_name = _T("Custom");
}

VisualMarker::MARKER_SHAPE ShapeMap::ToShape(std::wstring str)
{
	if(str == _T("Circle"))
		return VisualMarker::CIRCLE;
	else if(str == _T("Circle (fast)"))
		return VisualMarker::CIRCLE_FAST;
	else if(str == _T("Square"))
		return VisualMarker::SQUARE;
	else if(str == _T("Triangle"))
		return VisualMarker::TRIANGLE;
	else if(str == _T("Star"))
		return VisualMarker::STAR;
	else if(str == _T("Plus sign"))
		return VisualMarker::PLUS_SIGN;
	else if(str == _T("Octagon"))
		return VisualMarker::OCTAGON;
	else if(str == _T("Diamond"))
		return VisualMarker::DIAMOND;
	else if(str == _T("Inverted triangle"))
		return VisualMarker::INVERTED_TRIANGLE;

	Log::Inst().Error("ShapeMap::ToShape(): unrecognized shape string.");
	return VisualMarker::CIRCLE_FAST;
}

std::wstring ShapeMap::ToStringW(VisualMarker::MARKER_SHAPE shape)
{
	if(shape == VisualMarker::CIRCLE)
		return _T("Circle");
	else if(shape == VisualMarker::CIRCLE_FAST)
		return _T("Circle (fast)");
	else if(shape == VisualMarker::SQUARE)
		return _T("Square");
	else if(shape == VisualMarker::TRIANGLE)
		return _T("Triangle");
	else if(shape == VisualMarker::STAR)
		return _T("Star");
	else if(shape == VisualMarker::PLUS_SIGN)
		return _T("Plus sign");
	else if(shape == VisualMarker::OCTAGON)
		return _T("Octagon");
	else if(shape == VisualMarker::DIAMOND)
		return _T("Diamond");
	else if(shape == VisualMarker::INVERTED_TRIANGLE)
		return _T("Inverted triangle");

	Log::Inst().Error("ShapeMap::ToStringW(): unrecognized shape.");
	return _T("Circle (fast)");
}
