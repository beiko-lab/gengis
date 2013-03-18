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

#ifndef _GENGIS_SHAPE_MAP_MANAGER_
#define _GENGIS_SHAPE_MAP_MANAGER_

namespace GenGIS
{

	/**
	* @class ShapeMapManager
	* @brief Allow access to all shape maps supported by GenGIS.
	*/
	class ShapeMapManager: private Uncopyable 
	{
	public:
		typedef std::map<std::wstring, ShapeMapPtr>::const_iterator ShapeMapConstIter;
		typedef std::pair<std::wstring, ShapeMapPtr> ShapeMapPair;

	public:
		/** Constructor. */
		explicit ShapeMapManager() { BuildShapeMaps(); }

		/** Destructor. */
		~ShapeMapManager() {}

		/** Get shape map with the provided name. */
		ShapeMapPtr GetShapeMap(const std::wstring& name);

		/** Get default shape map. */
		ShapeMapPtr GetDefaultShapeMap();

		/** Get shape maps. */
		const std::map<std::wstring, ShapeMapPtr>& GetShapeMaps() const { return m_shapeMaps; }

	protected:
		/** Build all shape maps. */
		void BuildShapeMaps();

		/** Build standard shape map. */
		ShapeMapPtr BuildStandardShapeMap();

	protected:
		/** All supported shape maps. */
		std::map<std::wstring, ShapeMapPtr> m_shapeMaps;
	};

}

#endif
