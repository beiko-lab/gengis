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

#ifndef _GENGIS_USER_ELEMENTS_WRAPPER_
#define _GENGIS_USER_ELEMENTS_WRAPPER_

#include "../core/Precompiled.hpp"

#include "../core/UserElements.hpp"
#include "../core/LabelController.hpp"

namespace GenGIS
{
	/** Wrapper class for Python interface which provides access to user generated graphical elements. */
	class UserElementsWrapper: private Uncopyable 
	{
	public:
		explicit UserElementsWrapper() {}
		~UserElementsWrapper() {}

		/** 
		* @brief Add a new user defined line. 
		* @param line Line to add.
		* @return Id used to identify this line.
		*/
		uint AddLine(const VisualLinePtr line)
		{
			return UserElements::Inst().AddLine(line);
		}

		/** Remove line. */
		void RemoveLine(uint lineId)
		{
			UserElements::Inst().RemoveLine(lineId);
		}

		/** Clear all user defined lines. */
		void ClearLines()
		{
			UserElements::Inst().ClearLines();
		}

		/** 
		* @brief Add a new user defined marker. 
		* @param marker Marker to add.
		* @return Id used to identify this marker.
		*/
		uint AddMarker(const VisualMarkerPtr marker)
		{
			return UserElements::Inst().AddMarker(marker);
		}

		/** Remove marker. */
		void RemoveMarker(uint markerId)
		{
			UserElements::Inst().RemoveMarker(markerId);
		}

		/** Clear all user defined markers. */
		void ClearMarkers()
		{
			UserElements::Inst().ClearMarkers();
		}

		/** 
		* @brief Add a new user defined cylinder. 
		* @param line Line defining cylinder to add.
		* @param thickness Thickness (width) of cylinder.
		* @return Id used to identify this cylinder.
		*/
		uint AddCylinder(const VisualLinePtr line, float thickness)
		{
			return UserElements::Inst().AddCylinder(line, thickness);
		}

		/** Remove cylinder. */
		void RemoveCylinder(uint cylinderId)
		{
			UserElements::Inst().RemoveCylinder(cylinderId);
		}

		/** Clear all user defined cylinder. */
		void ClearCylinders()
		{
			UserElements::Inst().ClearCylinders();
		}

		/** 
		* @brief Add a new user label.
		* @param label Label to add.
		* @return Id used to identify this label.
		*/
		uint AddLabel(VisualLabelPtr label)
		{
			return LabelController::Inst().AddLabel(label);
		}

		/** Remove label. */
		void RemoveLabel(uint labelId)
		{
			LabelController::Inst().RemoveLabel(labelId);
		}

		/** Clear all user defined labels. */
		void ClearLabels(uint labelId)
		{
			LabelController::Inst().ClearLabels();
		}

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & UserElements::Inst();    // UserElements
			ar & LabelController::Inst(); // LabelController
		}
	};

}

#endif
