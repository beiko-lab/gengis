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

#ifndef _GENGIS_USER_ELEMENTS_
#define _GENGIS_USER_ELEMENTS_

#include "../core/Precompiled.hpp"
#include "../core/VisualLine.hpp"
#include "../core/VisualMarker.hpp"

#include "../utils/Colour.hpp"

namespace GenGIS
{
	/** Singleton class for handling storage, manipulation, and rendering of graphical elements defined by the user. */
	class UserElements
	{
	public:
		/** Get the one and only instance of this class. */
		static UserElements& Inst()
		{
			static UserElements singleton;
			return singleton;
		}

		/** Render all user defined graphical elements. */
		void Render();

		/** 
		* @brief Add a new user defined line. 
		* @param line Line to add.
		* @return Id used to identify this line.
		*/
		uint AddLine(const VisualLinePtr line);

		/** Remove line. */
		uint RemoveLine(uint lineId)
		{
			return m_lines.erase(lineId);
		}

		/** Clear all user defined lines. */
		void ClearLines()
		{
			m_lines.erase( m_lines.begin(), m_lines.end() );
		}

		/** 
		* @brief Add a new user defined marker. 
		* @param marker Marker to add.
		* @return Id used to identify this marker.
		*/
		uint AddMarker(const VisualMarkerPtr marker);

		/** Remove marker. */
		uint RemoveMarker(uint markerId)
		{
			return m_markers.erase(markerId);
		}

		/** Clear all user defined markers. */
		void ClearMarkers()
		{
			m_markers.erase( m_markers.begin(), m_markers.end() );
		}

		/** 
		* @brief Add a new user defined cylinder. 
		* @param line Line defining cylinder to add.
		* @param thickness Thickness (width) of cylinder.
		* @return Id used to identify this cylinder.
		*/
		uint AddCylinder(const VisualLinePtr line, float thickness);

		/** Remove cylinder. */
		uint RemoveCylinder(uint cylinderId)
		{
			return m_lines.erase(cylinderId);
		}

		/** Clear all user defined cylinder. */
		void ClearCylinders()
		{
			m_cylinders.erase( m_cylinders.begin(), m_cylinders.end() );
		}


	protected:
		/** Private singleton constructor (must instantiate this class through Inst()). */
		UserElements()
		{

		}

		/** Private copy constructor (must instantiate this class through Inst()). */
		UserElements(const UserElements&);

		/** Private assignment constructor (must instantiate this class through Inst()). */
		UserElements& operator= (const UserElements&);

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** User defined lines. */
		std::map<uint, VisualLinePtr> m_lines;

		/** User defined markers. */
		std::map<uint, VisualMarkerPtr> m_markers;

		/** User defined cylinders. */
		std::map<uint, VisualLinePtr> m_cylinders;
	};

}

#endif
