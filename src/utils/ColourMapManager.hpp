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

#ifndef _GENGIS_COLOUR_MAP_MANAGER_
#define _GENGIS_COLOUR_MAP_MANAGER_

namespace GenGIS
{

	/**
	* @class ColourMapManager
	* @brief Provides access to all colour maps supported by GenGIS.
	*/
	class ColourMapManager: private Uncopyable 
	{
	public:
		typedef std::map<std::wstring, ColourMapPtr>::const_iterator ColourMapConstIter;
		typedef std::pair<std::wstring, ColourMapPtr> ColourMapPair;

	public:
		/** Constructor. */
		ColourMapManager() {}

		/** Destructor. */
		~ColourMapManager() {}

		/** Get colour map with the provided name. */
		ColourMapPtr GetColourMap(const std::wstring& name);

		/** Get default discrete colour map. */
		ColourMapPtr GetDefaultDiscreteColourMap();

		/** Get default terrain colout map. */
		ColourMapPtr GetDefaultTerrainColourMap();

		/** 
		* @brief Load all colour maps in a directory. 
		* @param colorMapPath Path to directory containing predefined colour maps.
		* @return True if colour maps loaded successfully, otherwise False.
		*/
		bool LoadColourMaps(const std::string& colorMapPath);

		/** Get name of all colour maps. */
		std::vector<std::wstring> GetColourMapNames() const;

		/** Get name of all discrete colour maps. */
		std::vector<std::wstring> GetColourMapDiscreteNames() const;

		/** Get name of all continuous colour maps. */
		std::vector<std::wstring> GetColourMapContinuousNames() const;

		/** Get colour maps. */
		const std::map<std::wstring, ColourMapPtr>& GetColourMaps() const { return m_colourMaps; }

	protected:
		/** All supported colour maps. */
		std::map<std::wstring, ColourMapPtr> m_colourMaps;
	};

}

#endif
