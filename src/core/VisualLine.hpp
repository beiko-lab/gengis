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

#ifndef _GENGIS_VISUAL_LINE_
#define _GENGIS_VISUAL_LINE_

#include "../core/Precompiled.hpp"

#include "../core/VisualObject.hpp"
#include "../core/OrthoLayers.hpp"

namespace GenGIS
{
	/** A line to be rendered. */
	class VisualLine : public VisualObject
	{
	public:
		/** Line styles. */
		enum LINE_STYLE { SOLID = 0xFFFF, SHORT_DASH = 0xAAAA, LONG_DASH = 0xEEEE, HIDDEN = 0x0000 };

	public:
		/** Constructor. */
		explicit VisualLine(const Colour& colour = Colour(1.0f, 1.0f, 1.0f), float size = 1.0, LINE_STYLE lineStyle = SOLID, const Line3D& line = Line3D())	:
			VisualObject(colour, size, true), m_lineStyle(lineStyle), m_line(line), m_depth(LAYOUT_PRIMATIVES_LAYER) {}

		/** Render line. */
		void Render();

		/** Render line with border. */
		static void RenderLineWithBorder(const Line3D& line, const Colour& colour, float thickness, LINE_STYLE style,
			const Colour& borderColour, float borderThickness, float depth,
			bool bSelected);

		/** Render antialiased line. Start and end points are in screen space. */
		static void RenderAntialiasedLine(const Point3D& start, const Point3D& end, const Colour& colour, 
			float thickness, LINE_STYLE style, float depth);

		/**
		* Renders a closed cylinder between two points.
		*/
		static void RenderCylinder(const Point3D& start, const Point3D& end, float radius,int subdivisions, const Colour& colour, bool bSelected);

		/** Get depth of line. */
		float GetDepth() const
		{
			return m_depth;
		}

		/** Set depth of line. */
		void SetDepth(float depth)
		{
			m_depth = depth;
		}

		/** Get line style. */
		LINE_STYLE GetLineStyle() const
		{
			return m_lineStyle;
		}

		/** Set line style. */
		void SetLineStyle(LINE_STYLE lineStyle)
		{
			m_lineStyle = lineStyle;
		}

		/** Get start point of line. */
		Point3D GetStartPt() const
		{
			return m_line.start;
		}

		/** Set start point of line. */
		void SetStartPt(const Point3D& pt)
		{
			m_line.start = pt;
		}

		/** Get end point of line. */
		Point3D GetEndPt() const
		{
			return m_line.end;
		}

		/** Set end point of line. */
		void SetEndPt(const Point3D& pt)
		{
			m_line.end = pt;
		}

		/** Get line. */
		Line3D GetLine() const
		{
			return m_line;
		}

		/** Set line. */
		void SetLine(const Line3D& line)
		{
			m_line = line;
		}

		/** Set line thickness. */
		void SetThickness(float thickness)
		{
			SetSize(thickness);
		}

		/** Get line thickness. */
		float GetThickness() const
		{
			return GetSize();
		}

	protected:
		static void RenderLineWithBorder(const Point3D& start, const Point3D& end, const Point3D& startToEnd, const Point3D& normalToLine,
			const Colour& colour, float thickness, const Colour& borderColour, float borderThickness, float depth,
			bool bSelected);

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Line style for line. */
		LINE_STYLE m_lineStyle;

		/** Lines to be rendered with above visual properties. */
		Line3D m_line;

		/** Depth of line. Determine drawing order of elements. */
		float m_depth;
	};

}

#endif
