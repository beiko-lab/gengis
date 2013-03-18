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

#include "../utils/ColourMapManager.hpp"
#include "../utils/ColourMap.hpp"
#include "../utils/ColourMapContinuous.hpp"
#include "../utils/ColourMapDiscrete.hpp"
#include "../utils/Log.hpp"

using namespace GenGIS;

ColourMapPtr ColourMapManager::GetDefaultDiscreteColourMap()
{
	ColourMapConstIter it = m_colourMaps.find(_T("Default Discrete"));
	if(it == m_colourMaps.end())
	{
		Log::Inst().Error("ColourMapManager::GetDefaultColourMap: default colour map is missing.");
		return m_colourMaps.begin()->second;
	}

	return it->second;
}

ColourMapPtr ColourMapManager::GetDefaultTerrainColourMap()
{
	ColourMapConstIter it = m_colourMaps.find(_T("Default Terrain"));
	if(it == m_colourMaps.end())
	{
		Log::Inst().Error("ColourMapManager::GetDefaultTerrainColourMap: default terrain map is missing.");
		return m_colourMaps.begin()->second;
	}

	return it->second;
}

ColourMapPtr ColourMapManager::GetColourMap(const std::wstring& name)
{
	ColourMapConstIter it = m_colourMaps.find(name);
	if(it == m_colourMaps.end())
	{
		Log::Inst().Error("ColourMapManager::GetColourMap: unrecognized colour map name.");
		return m_colourMaps.begin()->second;
	}

	return it->second;
}

bool ColourMapManager::LoadColourMaps(const std::string& colorMapPath)
{
	using namespace boost::filesystem; 

	// check if colour map directory exists
	path colorMapDir(colorMapPath);
	if(!exists(colorMapDir)) 
	{
		Log::Inst().Error("ColourMapManager::LoadColourMaps(): failed to open colour map directory.");
		return false;
	}

	// iterate over all files in colour map directory
	directory_iterator end_itr; 
	ColourMapPtr tempColourMap(new ColourMap());
	bool bSuccess;
	for(directory_iterator itr(colorMapDir); itr != end_itr; ++itr)
	{
		if (!is_directory(itr->status()))
		{
			std::string filename = itr->path().string();
			ColourMap::COLOUR_MAP_TYPE type = tempColourMap->ReadType(filename);
			ColourMapPtr colourMap;
			if(type == ColourMap::CONTINUOUS)
			{
				colourMap.reset(new ColourMapContinuous());
				bSuccess = colourMap->Load(filename);
			}
			else if(type == ColourMap::DISCRETE)
			{
				colourMap.reset(new ColourMapDiscrete());
				bSuccess = colourMap->Load(filename);
			}
			else
			{
				Log::Inst().Error("ColourMapManager::LoadColourMaps(): invalid colour map found in colour map directory.");
				return false;
			}


			if(bSuccess)
			{
				m_colourMaps[colourMap->GetName()] = colourMap;
			}
			else
			{
				Log::Inst().Error("ColourMapManager::LoadColourMaps(): failed to parse colour map found in colour map directory.");
				return false;
			}
		}
	}

	return true;
}

std::vector<std::wstring> ColourMapManager::GetColourMapNames() const
{
	std::vector<std::wstring> colourMapNames;

	ColourMapConstIter it;
	for(it = m_colourMaps.begin(); it != m_colourMaps.end(); ++it)
		colourMapNames.push_back(it->first);

	return colourMapNames;
}

std::vector<std::wstring> ColourMapManager::GetColourMapDiscreteNames() const
{
	std::vector<std::wstring> colourMapNames;

	ColourMapConstIter it;
	for(it = m_colourMaps.begin(); it != m_colourMaps.end(); ++it)
	{
		if (it->second->GetType() == ColourMap::DISCRETE)
			colourMapNames.push_back(it->first);
	}

	return colourMapNames;
}

std::vector<std::wstring> ColourMapManager::GetColourMapContinuousNames() const
{
	std::vector<std::wstring> colourMapNames;

	ColourMapConstIter it;
	for(it = m_colourMaps.begin(); it != m_colourMaps.end(); ++it)
	{
		if (it->second->GetType() == ColourMap::CONTINUOUS)
			colourMapNames.push_back(it->first);
	}

	return colourMapNames;
}
