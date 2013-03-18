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

#ifndef _GENGIS_VISUAL_OBJECT_
#define _GENGIS_VISUAL_OBJECT_

#include "../core/Precompiled.hpp"
#include "../core/View.hpp"

#include "../utils/Colour.hpp"

namespace GenGIS
{

	/** 
	 * @class VisualObject
	 * @brief Abstract base class for graphical elements. */
	class VisualObject : public View
	{
	public:
		/** Constructor. */
		explicit VisualObject(Colour colour = Colour(1.0f, 1.0f, 1.0f), float size = 1.0, bool bVisible = true) :
			m_colour(colour), m_size(size)
		{
			SetSelected(false);
			SetVisibility(bVisible);
		}

		virtual ~VisualObject();

		/** Get colour of object. */
		virtual Colour GetColour() const
		{
			return m_colour;
		}

		/** Set colour of object. */
		virtual void SetColour(const Colour& colour)
		{
			m_colour = colour;
		}

		/** Get size of object. */
		float GetSize() const;

		/** Set size of object. */
		virtual void SetSize(float size)
		{
			m_size = size;
		}

	private:
		/** serialization. */
		friend class boost::serialization::access;
		template<class archive>
		void serialize(archive & ar, const unsigned int version);

	protected:
		/** Colour of object. */
		Colour m_colour;

		/** Size of object. */
		float m_size;
	};

}

#endif
