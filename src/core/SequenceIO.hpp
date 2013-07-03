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

#ifndef _GENGIS_SEQUENCE_IO_
#define _GENGIS_SEQUENCE_IO_

#include "../core/Precompiled.hpp"

#include "../core/SequenceModel.hpp"

namespace GenGIS
{

	/*!
	\class SequenceIO
	\brief Read sequence data from file.
	*/

	class SequenceIO
	{
	public:
		/**
		* @brief Load sequence data from file.
		* @param path Path to sample data.
		* @param filename Filename of sample data.
		* @param sequenceModels Sequence models to populate from file.
		*/
		static bool Read( const wxString& fullPath, std::vector<SequenceModelPtr>& sequenceModels, bool& bCancel );

		static bool ParseCSVFile( const std::vector<std::wstring> csvTableRows, std::vector<SequenceModelPtr>& sequenceModels, bool& bCancel);
	};

}

#endif
