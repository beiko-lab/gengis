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

#ifndef _GENGIS_VISUAL_POLYLINE_
#define _GENGIS_VISUAL_POLYLINE_

#include "../core/Precompiled.hpp"

#include "../core/VisualObject.hpp"
#include "../core/VisualLine.hpp"
#include "../core/VisualMarker.hpp"

namespace GenGIS
{

	/** A polyline to be rendered. */
	class VisualPolyLine : public VisualObject
	{
	public:

		/** Constructor. */
		explicit VisualPolyLine(Colour colour = Colour(0.8f, 0.8f, 0.8f), float size = 1.0,
			VisualLine::LINE_STYLE lineStyle = VisualLine::SOLID);

		/** Destructor. */
		~VisualPolyLine();

		/** Render polyline. */
		void Render();

		/** Set flag indicating if control points should be rendered for polyline. */
		void SetRenderControlPts(bool bRender)
		{
			m_bRenderCtrlPts = bRender;
		}

		/** Get flag indicating if control points should be rendered for polyline. */
		bool GetRenderControlPts() const
		{
			return m_bRenderCtrlPts;
		}

		/** Get polyline style. */
		const VisualLine::LINE_STYLE GetLineStyle() const
		{
			return m_lineStyle;
		}

		/** Set polyline style. */
		void SetLineStyle(VisualLine::LINE_STYLE lineStyle)
		{
			m_lineStyle = lineStyle;
		}

		/** Get first point of polyline. */
		const Point3D& GetFirstPt() const
		{
			return m_polyline.GetFirstPt();
		}

		/** Set start point of polyline. */
		void SetFirstPt(const Point3D& pt);

		/** Get last point of polyline. */
		const Point3D& GetLastPt() const
		{
			return m_polyline.GetLastPt();
		}

		/** Set last point of polyline. */
		void SetLastPt(const Point3D& pt)
		{
			m_polyline.pts.at(m_polyline.pts.size()-1) = pt;
		}

		/** Set next point of polyline. */
		void SetNextPt(const Point3D& pt)
		{
			m_polyline.pts.push_back(pt);
		}

		/** Get point of polyline. */
		const Point3D& GetPt(int index) const
		{
			return m_polyline.GetPoint(index);
		}

		/** Set point of polyline. */
		void SetPt(int index, const Point3D& pt)
		{
			m_polyline.pts.at(index) = pt;
		}

		/** Get length of polyline. */
		float GetLength() const
		{
			return m_polyline.GetLength();
		}

		/** Get number of points in polyline. */
		uint GetNumPts() const
		{
			return m_polyline.pts.size();
		}

		/** Get modifiable copy of polyline. */
		PolyLine3D& PolyLine()
		{
			return m_polyline;
		}

		/** Get constant copy of polyline. */
		const PolyLine3D& PolyLine() const
		{
			return m_polyline;
		}

	protected:
		/** Render polyline. */
		void RenderPolyLine(const Colour& colour, VisualLine::LINE_STYLE lineStyle, float size);

		/** Render a control point of polyline. */
		void RenderControlPt(const Point3D& pt, const Colour& colour, float size, VisualMarker::MARKER_SHAPE shape, int depth);

		/** Render all control points of polyline. */
		void RenderControlPts(const Colour& colour, float size, int depth);

	private:
		/** serialization. */
		friend class boost::serialization::access;

		template<class archive>
		void serialize(archive & ar, const unsigned int version);

	protected:
		/** Line style for polyline. */
		VisualLine::LINE_STYLE m_lineStyle;

		/** Polyline to be rendered with above visual properties. */
		PolyLine3D m_polyline;

		/** Flag indicating if control points should be rendered for polyline. */
		bool m_bRenderCtrlPts; 
	};

}

#endif
