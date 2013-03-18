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

#ifndef _GENGIS_LOCATION_SET_IO_
#define _GENGIS_LOCATION_SET_IO_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/*!
	\class LocationSetIO
	\brief Read location data from file.
	*/

	class LocationSetIO
	{
	public:
		/**
		* @brief Load location data from file.
		* @param path Path to sample data.
		* @param filename Filename of sample data.
		* @param studyController Study controller associated with sample data.
		* @param locationModels Location models to populate from file.
		*/
		static bool Read(const wxString& fullPath, StudyControllerPtr studyController,
			std::vector<LocationModelPtr>& locationModels);
	};

}

#endif
