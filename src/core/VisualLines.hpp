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

#ifndef _GENGIS_VISUAL_LINES_
#define _GENGIS_VISUAL_LINES_

#include "../core/Precompiled.hpp"

#include "../core/VisualLine.hpp"

#include "../utils/Colour.hpp"

namespace GenGIS
{

	/** A collection of similar lines to be rendered. */
	class VisualLines
	{
	public:

		/** Constructor. */
		explicit VisualLines(Colour _colour = Colour(1.0f, 1.0f, 1.0f), float _thickness = 1.0, 
			VisualLine::LINE_STYLE _lineStyle = VisualLine::SOLID, bool _visible = true) :
				m_colour(_colour), m_thickness(_thickness), m_lineStyle(_lineStyle), m_visible(_visible) {}

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

		/** Get line style. */
		VisualLine::LINE_STYLE GetLineStyle() const
		{
			return m_lineStyle;
		}

		/** Set line style. */
		void SetLineStyle(VisualLine::LINE_STYLE lineStyle)
		{
			m_lineStyle = lineStyle;
		}

		/** Get thickness. */
		float GetThickness() const;

		/** Set thickness. */
		void SetThickness(float thickness)
		{
			m_thickness = thickness;
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

		/** Get/set lines. */
		std::vector<Line3D>& Lines()
		{
			return m_lines;
		}

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Colour of line. */
		Colour m_colour;

		/** Line style for line. */
		VisualLine::LINE_STYLE m_lineStyle;

		/** Thickness of line. */
		float m_thickness;

		/** Flag indicating if the line should be rendered. */
		bool m_visible;

		/** Lines to be rendered with above visual properties. */
		std::vector<Line3D> m_lines;
	};

}

#endif
