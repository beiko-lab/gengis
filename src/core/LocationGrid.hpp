//=======================================================================
// Author: Timothy Mankowski
//
// Copyright 2013 Timothy Mankowski
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

#ifndef _GENGIS_LOCATION_GRID_
#define _GENGIS_LOCATION_GRID_

#include "../core/Precompiled.hpp"

#include "../core/View.hpp"

namespace GenGIS
{
	class LocationGrid : public View
	{
	public:
		/**
		* @brief Constructor.
		*/
		explicit LocationGrid();

		/** Destructor. */
		~LocationGrid();

		/** Determine if object is visible. */
		bool IsVisible();

		/** Render location grid. */
		void Render();

	private:
		uint m_divisions;
		double m_elevation;
		double m_alpha;

		// Add serialization stuff, blah
	};
}
#endif
