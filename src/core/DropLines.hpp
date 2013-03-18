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

#ifndef _GENGIS_DROPLINES_
#define _GENGIS_DROPLINES_

#include "../core/Precompiled.hpp"

#include "../core/View.hpp"
#include "../core/VisualLines.hpp"

#include "../utils/Colour.hpp"

namespace GenGIS
{

	/**
	* @class DropLines
	* @brief Simple collection of droplines.
	*/
	class DropLines : public View
	{
	public:
		/** 
		* @brief Constructor. 
		*/
		explicit DropLines() :
			m_colour(Colour(255, 255, 255, 255)),
			m_thickness(1),
			m_style(VisualLine::SOLID) {}

		/** Destructor. */
		~DropLines() {}

		/** Render chart. */
		void Render();

		/** Get dropline thickness. */
		float GetThickness() const
		{
			return m_thickness;
		}

		/** Set dropline thickness. */
		void SetThickness(float thickness)
		{
			m_thickness = thickness;
		}

		/** Get dropline colour. */
		Colour GetColour() const
		{
			return m_colour;
		}

		/** Set dropline colour. */
		void SetColour(const Colour& colour)
		{
			m_colour = colour;
		}

		/** Get dropline line style. */
		VisualLine::LINE_STYLE GetStyle() const
		{
			return m_style;
		}

		/** Set dropline line style. */
		void SetStyle(VisualLine::LINE_STYLE style)
		{
			m_style = style;
		}

		/** Clear all droplines. */
		void Clear()
		{
			m_dropLines.clear();
		}

		/** Add dropline. */
		void Add(const VisualLinePtr line)
		{
			m_dropLines.push_back(line);
		}

		/** Set dropline specified by the index. */
		void SetDropLine(int index, const VisualLinePtr line)
		{
			m_dropLines.at(index) = line;
		}

		/** Get dropline specified by the index. */
		VisualLinePtr GetDropLine(int index) const
		{
			return m_dropLines.at(index);
		}

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Thickness of droplines. */
		float m_thickness;

		/** Colour of droplines. */
		Colour m_colour;

		/** Line style of droplines. */
		VisualLine::LINE_STYLE m_style;

		/** Droplines. */
		std::vector<VisualLinePtr> m_dropLines;
	};

}

#endif
