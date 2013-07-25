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

#include "../utils/ShapeMapManager.hpp"
#include "../utils/ShapeMap.hpp"
#include "../utils/Log.hpp"

using namespace GenGIS;

ShapeMapPtr ShapeMapManager::GetDefaultShapeMap()
{
	ShapeMapConstIter it = m_shapeMaps.find(_T("Standard"));
	if(it == m_shapeMaps.end())
	{
		Log::Inst().Error("ShapeMapManager::GetDefaultShapeMap: default shape map is missing.");
		return m_shapeMaps.begin()->second;
	}

	return it->second;
}

ShapeMapPtr ShapeMapManager::GetShapeMap(const std::wstring& name)
{
	ShapeMapConstIter it = m_shapeMaps.find(name);
	if(it == m_shapeMaps.end())
	{
		Log::Inst().Error("ShapeMapManager::GetShapeMap: unrecognized shape map name.");
		return m_shapeMaps.begin()->second;
	}

	return it->second;
}

void ShapeMapManager::BuildShapeMaps()
{
	ShapeMapPtr standardShapeMap = BuildStandardShapeMap();
	m_shapeMaps[_T("Standard")] = standardShapeMap;
}

ShapeMapPtr ShapeMapManager::BuildStandardShapeMap()
{
	ShapeMapPtr shapeMap(new ShapeMap(_T("Standard")));

	// set all possible shapes supported by shape map
	shapeMap->AddShape(VisualMarker::CIRCLE);
	shapeMap->AddShape(VisualMarker::CIRCLE_FAST);
	shapeMap->AddShape(VisualMarker::SQUARE);
	shapeMap->AddShape(VisualMarker::TRIANGLE);
	shapeMap->AddShape(VisualMarker::DIAMOND);
	shapeMap->AddShape(VisualMarker::INVERTED_TRIANGLE);
	shapeMap->AddShape(VisualMarker::PLUS_SIGN);
	shapeMap->AddShape(VisualMarker::OCTAGON);
	shapeMap->AddShape(VisualMarker::STAR);
	
	return shapeMap;
}
