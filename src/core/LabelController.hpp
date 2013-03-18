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

#ifndef _GENGIS_LABEL_CONTROLLER_
#define _GENGIS_LABEL_CONTROLLER_

#include "../core/Precompiled.hpp"
#include "../core/VisualLabel.hpp"

#include "../utils/Colour.hpp"

namespace GenGIS
{

	/** Singleton class for handling storage, manipulation, and rendering of user-defined labels. */
	class LabelController
	{
	public:
		/** Get the one and only instance of this class. */
		static LabelController& Inst()
		{
			static LabelController singleton;
			return singleton;
		}

		/** Render all user defined labels. */
		void Render();

		/** 
		* @brief Add a new label. 
		* @param label Label to add.
		* @return Id used to identify this label.
		*/
		uint AddLabel(VisualLabelPtr label);

		/** Remove a label. */
		uint RemoveLabel(uint labelId) 	{ return m_labels.erase(labelId); }

		/** Clear all labels. */
		void ClearLabels() { m_labels.erase(m_labels.begin(), m_labels.end()); }

		/** 
		* @brief Process mouse left button down events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		bool MouseLeftDown(const Point2D& mousePt);

		/** 
		* @brief Process mouse right button down events. 
		* @param mousePt Screen position of mouse.
		* @param popupMenu Popup menu to populate.
		* @return True if view is processing the event, else false.
		*/
		bool MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu);

	protected:
		/** Private singleton constructor (must instantiate this class through Inst()). */
		LabelController()
		{

		}

		/** Private copy constructor (must instantiate this class through Inst()). */
		LabelController(const LabelController&);

		/** Private assignment constructor (must instantiate this class through Inst()). */
		LabelController& operator= (const LabelController&);

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** User defined labels. */
		std::map<uint, VisualLabelPtr> m_labels;
	};

}

#endif
