//=======================================================================
// Author: Timothy Mankowski & Somayyeh Zangooei
//
// Copyright 2012 Timothy Mankowski & Somayyeh Zangooei
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

#ifndef _GENGIS_VECTOR_MAP_VIEW_
#define _GENGIS_VECTOR_MAP_VIEW_

#include "../core/Precompiled.hpp"
#include "../core/VisualMarker.hpp"
//#include "../core/App.hpp"
#include "../core/View.hpp"
//#include "../core/Viewport.hpp"
//#include "../core/Layer.hpp"
#include "../core/VisualLine.hpp"
#include "../utils/Colour.hpp"


class GenGIS::VectorMapView;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::VectorMapView * t, const unsigned int file_version);
	}
}

namespace GenGIS
{
	class VectorMapView : public View
	{
	public:
		/** Constructor. */
		explicit VectorMapView( VectorMapModelPtr vectorMapModel );

		/** Render terrain. */
		void Render();
		void Render(bool bSimplified);

		Colour GetPointColour() const
		{
			return m_pointColour;
		}

		/** Set point feature colour. */
		void SetPointColour(const Colour& colour)
		{
			m_pointColour = colour;
		}
		Colour GetPointBorderColour() const
		{
			return m_pointBorderColour;
		}

		/** Set point feature border colour. */
		void SetPointBorderColour(const Colour& colour)
		{
			m_pointBorderColour = colour;
		}
		float GetPointSize() const
		{
			return m_pointSize;
		}

		/** Set point feature size. */
		void SetPointSize(float size)
		{
			m_pointSize = size;
		}
		float GetPointBorderSize() const
		{
			return m_pointBorderSize;
		}

		/** Set point feature border size. */
		void SetPointBorderSize(float size)
		{
			m_pointBorderSize = size;
		}
		VisualMarker::MARKER_SHAPE GetPointShape() const
		{
			return m_pointShape;
		}

		/** Set point feature shape. */
		void SetPointShape(VisualMarker::MARKER_SHAPE shape)
		{
			m_pointShape = shape;
		}	


		Colour GetLineColour() const
		{
			return m_lineColour;
		}

		/** Set line feature colour. */
		void SetLineColour(const Colour& colour)
		{
			m_lineColour = colour;
		}
		Colour GetLineBorderColour() const
		{
			return m_lineBorderColour;
		}

		/** Set line feature border colour. */
		void SetLineBorderColour(const Colour& colour)
		{
			m_lineBorderColour = colour;
		}
		float GetLineSize() const
		{
			return m_lineSize;
		}

		/** Set line feature size. */
		void SetLineSize(float size)
		{
			m_lineSize = size;
		}
		float GetLineBorderSize() const
		{
			return m_lineBorderSize;
		}

		/** Set line feature border size. */
		void SetLineBorderSize(float size)
		{
			m_lineBorderSize = size;
		}
		VisualLine::LINE_STYLE GetLineStyle() const
		{
			return m_lineStyle;
		}

		/** Set line feature style. */
		void SetLineStyle(VisualLine::LINE_STYLE style)
		{
			m_lineStyle = style;
		}


		Colour GetPolygonBorderColour() const
		{
			return m_polygonBorderColour;
		}

		/** Set polygon feature border colour. */
		void SetPolygonBorderColour(const Colour& colour)
		{
			m_polygonBorderColour = colour;
		}
				
		float GetPolygonBorderSize() const
		{
			return m_polygonBorderSize;
		}

		/** Set polygon feature border size. */
		void SetPolygonBorderSize(float size)
		{
			m_polygonBorderSize = size;
		}
		/** Set maximum allowed width size for polygon . */
		void SetPolygonMaxWidthSize(float size)
		{
			m_polygonMaxWidthSize=size;
		}
		float GetPolygonMaxWidthSize()
		{
			return m_polygonMaxWidthSize;
		}

		void SetPolygonMinWidthSize(float size)
		{
			m_polygonMinWidthSize=size;
		}
		float GetPolygonMinWidthSize()
		{
			return m_polygonMinWidthSize;
		}
		
		VisualLine::LINE_STYLE GetPolygonBorderStyle() const
		{
			return m_polygonBorderStyle;
		}

		/** Set polygon feature border style. */
		void SetPolygonBorderStyle(VisualLine::LINE_STYLE style)
		{
			m_polygonBorderStyle = style;
		}
		


	private:
		
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const VectorMapView * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int file_version);

		VectorMapModelPtr m_vectorMapModel;
		Colour m_pointColour;
		Colour m_pointBorderColour;
		float m_pointSize;
		float m_pointBorderSize;
		VisualMarker::MARKER_SHAPE m_pointShape;
		Colour m_lineColour;
		Colour m_lineBorderColour;
		float m_lineSize;
		float m_lineBorderSize;
		VisualLine::LINE_STYLE m_lineStyle;
		Colour m_polygonBorderColour;
		float m_polygonBorderSize;
		float m_polygonMaxWidthSize;
		float m_polygonMinWidthSize;
		VisualLine::LINE_STYLE m_polygonBorderStyle;
	};
}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const VectorMapView * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			ar << t->m_vectorMapModel;
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, VectorMapView * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			VectorMapModelPtr _vectorMapModel;
			ar >> _vectorMapModel;

			::new(t)VectorMapView( _vectorMapModel );
		}
	}
}

#endif