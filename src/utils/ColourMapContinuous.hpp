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

#ifndef _GENGIS_COLOUR_MAP_CONTINUOUS_
#define _GENGIS_COLOUR_MAP_CONTINUOUS_

#include "../utils/ColourMap.hpp"

#include "../utils/Colour.hpp"

namespace GenGIS
{

	/**
	* @class ColourMapContinuous
	* @brief Allow colours to be obtained from a continuous spectrum.
	*/
	class ColourMapContinuous: public ColourMap
	{
	public:
		/** Constructor. */
		explicit ColourMapContinuous(const std::wstring& name = _T("<no name>"), COLOUR_MAP_TYPE type = CONTINUOUS): ColourMap(name, type) {}

		/** Destructor. */
		~ColourMapContinuous() {}
	};

}

#endif
