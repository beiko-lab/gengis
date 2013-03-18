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

#ifndef _GENGIS_UNIQUE_ID_
#define _GENGIS_UNIQUE_ID_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/**
	* @brief Singleton class for generating unique id's.
	*/
	class UniqueId
	{
	public:
		/** Orientation of graph (influences many layout algorithms). */ 
		enum ID_FLAGS { NO_ID = 0 }; 

	public:
		/** Destructor */
		~UniqueId() { }

		/** Get the one and only instance of the state class. */
		static UniqueId& Inst()
		{
			static UniqueId uniqueId;
			return uniqueId;
		}

		/** Get unique id. */
		uint GenerateId() 
		{ 
			return m_uniqueId++;
		}

		/** Custom serialization function for UniqueId. */
		template<class Archive>
		void UniqueIdSerialize(Archive & ar, const unsigned int file_version)
		{
			ar & m_uniqueId;
		}

	protected:
		/** Private singleton constructor (must instantiate  this class through Inst()). */
		UniqueId(): m_uniqueId(1) {}

		/** Private copy constructor (must instantiate  this class through Inst()). */
		UniqueId(const UniqueId&);

		/** Private assignment constructor (must instantiate  this class through Inst()). */
		UniqueId& operator= (const UniqueId&);


	private:
		/** Unique number. */
		uint m_uniqueId;

	};

}

#endif
