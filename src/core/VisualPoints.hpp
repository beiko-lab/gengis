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

#ifndef _GENGIS_VISUAL_POINTS_
#define _GENGIS_VISUAL_POINTS_

#include "../core/Precompiled.hpp"

#include "../utils/Colour.hpp"

namespace GenGIS
{

	/** A collection of similar points to be rendered. */
	class VisualPoints
	{
	public:

		/** Constructor. */
		explicit VisualPoints(Colour _colour = Colour(1.0f, 1.0f, 1.0f), float _size = 5.0, bool _visible = true) :
			m_colour(_colour), m_size(_size), m_visible(_visible) {}

		/** Render all lines. */
		void Render();

		/** Get colour. */
		Colour GetColour() const
		{
			return m_colour;
		}

		/** Set colour. */
		void SetColour(const Colour& colour)
		{
			m_colour = colour;
		}

		/** Get size. */
		float GetSize() const;

		/** Set size. */
		void SetSize(float size)
		{
			m_size = size;
		}

		/** Get visibility. */
		bool GetVisibility() const
		{
			return m_visible;
		}

		/** Set visibility. */
		void SetVisibility(bool visible)
		{
			m_visible = visible;
		}

		/** Get/set points. */
		std::vector<Point3D>& Points()
		{
			return m_points;
		}

		/** Get/set ids. */
		std::vector<std::wstring>& Ids()
		{
			return m_ids;
		}

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Colour of point. */
		Colour m_colour;

		/** Size of point. */
		float m_size;

		/** Flag indicating if the point should be rendered. */
		bool m_visible;

		/** Points to be rendered with above visual properties. */
		std::vector<Point3D> m_points;

		/** Id to associate with each point. */
		std::vector<std::wstring> m_ids;
	};

}

#endif
