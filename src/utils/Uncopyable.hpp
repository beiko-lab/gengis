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

#ifndef _GENGIS_UNCOPYABLE_
#define _GENGIS_UNCOPYABLE_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/**
	* @class Uncopyable
	* @brief Inheriting this class as private prevents a class from being copied.
	*/
	class Uncopyable
	{
	protected:
		Uncopyable() {}
		~Uncopyable() {}

	private:
		Uncopyable(const Uncopyable&);
		Uncopyable& operator=(const Uncopyable&);
	};

}

#endif