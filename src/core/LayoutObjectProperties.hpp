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

#ifndef _GENGIS_LAYOUT_OBJECT_PROPERTIES
#define _GENGIS_LAYOUT_OBJECT_PROPERTIES

#include "../core/Precompiled.hpp"

#include "../core/VisualLine.hpp"

#include "../utils/Colour.hpp"

namespace GenGIS
{

	class LayoutObjectProperties 
	{
	public:
		/** Constructor. */
		explicit LayoutObjectProperties() :
			m_objectLineThickness(1), m_objectLineColour(Colour(192, 192, 255)), m_objectLineStyle(VisualLine::SHORT_DASH),
			m_layoutLineThickness(1), m_layoutLineColour(Colour(192, 192, 255)), m_layoutLineStyle(VisualLine::SHORT_DASH),
			m_controlPointSize(4), m_controlPointColour(Colour(192, 192, 255))
		{}

		/** Destructor. */
		~LayoutObjectProperties() {}

		/** Get object line thickness. */
		float GetObjectLineThickness(bool bCorrected = true) const; 

		/** Set object line thickness. */
		void SetObjectLineThickness(float thickness) { m_objectLineThickness = thickness; }

		/** Set object line colour. */
		void SetObjectLineColour(const Colour& colour) { m_objectLineColour = colour; }

		/** Get object line colour. */
		const Colour& GetObjectLineColour() const { return m_objectLineColour; }

		/** Set object line style. */
		void SetObjectLineStyle(const VisualLine::LINE_STYLE& style) { m_objectLineStyle = style; }

		/** Get object line style. */
		const VisualLine::LINE_STYLE GetObjectLineStyle() const { return m_objectLineStyle; }

		/** Set layout line thickness. */
		void SetLayoutLineThickness(float thickness) { m_layoutLineThickness = thickness; }

		/** Get layout line thickness. */
		float GetLayoutLineThickness(bool bCorrected = true) const;

		/** Set layout line colour. */
		void SetLayoutLineColour(const Colour& colour) { m_layoutLineColour = colour; }

		/** Get layout line colour. */
		const Colour& GetLayoutLineColour() const { return m_layoutLineColour; }

		/** Set layout line style. */
		void SetLayoutLineStyle(const VisualLine::LINE_STYLE& style) { m_layoutLineStyle = style; }

		/** Get layout line style. */
		const VisualLine::LINE_STYLE GetLayoutLineStyle() const { return m_layoutLineStyle; }

		/** Set control point size. */
		void SetControlPointSize(float size) { m_controlPointSize = size; }

		/** Get control point size. */
		float GetControlPointSize(bool bCorrected = true) const;

		/** Set control point colour. */
		void SetControlPointColour(const Colour& colour) { m_controlPointColour = colour; }

		/** Get control point colour. */
		const Colour& GetControlPointColour() const { return m_controlPointColour; }

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	private:
		/** Thickness of object line. */
		float m_objectLineThickness;

		/** Colour of object line. */
		Colour m_objectLineColour;

		/** Line style of object line. */
		VisualLine::LINE_STYLE m_objectLineStyle;

		/** Thickness of layout line. */
		float m_layoutLineThickness;

		/** Colour of layout line. */
		Colour m_layoutLineColour;

		/** Line style of layout line. */
		VisualLine::LINE_STYLE m_layoutLineStyle;

		/** Size of control point. */
		float m_controlPointSize;

		/** Colour of control point. */
		Colour m_controlPointColour;
	};

}

#endif
