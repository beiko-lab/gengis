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

#ifndef _GENGIS_LAYOUT_LINE_
#define _GENGIS_LAYOUT_LINE_

#include "../core/Precompiled.hpp"

#include "../core/MapController.hpp"
#include "../core/View.hpp"
#include "../core/VisualLine.hpp"

#include "../utils/Point3D.hpp"

namespace GenGIS
{

	class LayoutLine : public View 
	{
	public:
		/** Stage of defining layout line. */
		enum STAGE { START = 0, SINGLE_PT, COMPLETE };

		/** Indentify the currently active control point. */
		enum ACTIVE_POINT { NONE = 0, START_PT, END_PT };

	public:
		/**
		* @brief Constructor.
		*/
		explicit LayoutLine();

		/** Destructor. */
		~LayoutLine() {}

		/** Render layout line. */
		void Render();

		/** Get stage of layout line construction. */
		STAGE GetStage() const { return m_stage; }

		/** Show properties dialog for this view. */
		void ShowPropertiesDlg();

		/** Get length of layout line that is devoted to laying out objects. */
		float GetLength() const;

		/** Get vector in direction of layout line (from start point to end point). */
		Point3D GetDirection() const 
		{ 
			Point3D dir = m_layoutLine.GetEndPt() - m_layoutLine.GetStartPt(); 
			return dir.Normalize();
		}

		/** Get vector perpendicular to layout line (on RHS as one walks from the start to end point).*/
		Point3D GetPerpDirection() const;

		/** Offset from control points before objects should start/finish being laid out. */
		// Note: this should really be calculated based on the size of the control point
		float GetLayoutOffset() const;

		/** Get start point for laying out objects on the layout line. */
		Point3D GetStartPt() const { return m_layoutLine.GetStartPt() + GetLayoutOffset()*GetDirection(); }

		/** Set start point for laying out objects on the layout line. */
		void SetStartPt(const Point3D& start)  { m_layoutLine.SetStartPt(start - GetLayoutOffset()*GetDirection()); }

		/** Get end point for laying out objects on the layout line. */
		Point3D GetEndPt() const { return m_layoutLine.GetEndPt() - GetLayoutOffset()*GetDirection(); }

		/** Set start point for laying out objects on the layout line.*/
		void SetEndPt(const Point3D& end)  { m_layoutLine.SetEndPt(end + GetLayoutOffset()*GetDirection()); }

		/** Get position of start control point. */
		Point3D GetStartControlPoint() const { return m_layoutLine.GetStartPt(); }

		/** Set position of start control point. */
		void SetStartControlPoint(const Point3D& start)  { m_layoutLine.SetStartPt(start); }

		/** Get position of end control points. */
		Point3D GetEndControlPoint() const { return m_layoutLine.GetEndPt(); }

		/** Set position of end control point. */
		void SetEndControlPoint(const Point3D& end)  { m_layoutLine.SetEndPt(end); }

		/** Get layout line. */
		Line3D GetLine() const { return m_layoutLine.GetLine(); }

		/** 
		* @brief Process mouse left button down events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		bool MouseLeftDown(const Point2D& mousePt);

		/** 
		* @brief Process mouse left button up events. 
		* @param mousePt Screen position of mouse.
		*/
		void MouseLeftUp(const Point2D& mousePt);

		/** 
		* @brief Process mouse left button float click events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		void MouseLeftDblClick(const Point2D& mousePt);

		/** 
		* @brief Process mouse right button down events. 
		* @param mousePt Screen position of mouse.
		* @param popupMenu Popup menu to populate.
		* @return True if view is processing the event, else false.
		*/
		bool MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu);

		/** 
		* @brief Process mouse dragging events. 
		* @param mousePt Screen position of mouse.
		*/
		void MouseDragging(const Point2D& mousePt);

		/** 
		* @brief Process mouse moving events. 
		* @param mousePt Screen position of mouse.
		* @return True if the OpenGL viewport should be refreshed.
		*/
		bool MouseMove(const Point2D& mousePt);

		/** 
		* @brief Process keyboard events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		void KeyboardEvent(wxKeyEvent& event);

	private:
		/** Handle mouse events when manipulating a pre-existing line. */
		bool ModifyLineInteraction(const Point2D& mousePt);

		/** Check if mouse cursor is over a control point. */
		bool IsPointClicked(const Point2D& mousePt, const Point3D& controlPt);

		/** Check if mouse cursor is over any active element of the layout line. */
		bool IsClicked(const Point2D& mousePt);

		/** serialization. */
		friend class boost::serialization::access;

		template<class archive>
		void serialize(archive & ar, const unsigned int version);

	private:
		/** Layout line. */
		VisualLine m_layoutLine;

		/** Layout line under modification. */
		VisualLine m_modLayoutLine;

		/** Indicates where objects should be laid out as a percent offset of the line length. */
		float m_layoutLineOffsetPercentage;

		/** Tracks stage of line construction. */
		STAGE m_stage;

		/** Tracks currently active control point. */
		ACTIVE_POINT m_activePt;
	};

}

#endif
