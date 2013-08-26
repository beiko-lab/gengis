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

#ifndef _GENGIS_LAYOUT_ELLIPSE_
#define _GENGIS_LAYOUT_ELLIPSE_

#include "../core/Precompiled.hpp"

#include "../core/MapController.hpp"
#include "../core/View.hpp"
#include "../core/VisualLine.hpp"

#include "../utils/Colour.hpp"
#include "../utils/Point3D.hpp"

namespace GenGIS
{

	class LayoutEllipse: public View 
	{
	public:
		/** Stage of defining ellipse. */
		enum STAGE { START = 0, PRIMARY_LINE, ORTHO_LINE, COMPLETE };

		/** Indentify the currently active control point. */
		enum ACTIVE_POINT { NONE = 0, PRIMARY_START, PRIMARY_END, CENTRE_PT, ORTHO_END, ROTATE };

		/** Number of points to use in defining the ellipse. */
		static const int NUM_ELLIPSE_PTS = 360;

	public:
		/**
		* @brief Constructor.
		*/
		explicit LayoutEllipse();

		/** Destructor. */
		~LayoutEllipse() {}

		/** Render layout ellipse. */
		void Render();

		/** Get stage of ellipse construction. */
		STAGE GetStage() const { return m_stage; }

		/** 
		* @brief Generate data for rendering ellipse. 
		* @param centre Centre point of ellipse.
		* @param xRadius Radius of ellipse along x-axis before rotation.
		* @param yRadius Radius of ellipse along y-axis before rotation.
		* @param rotation Amount to rotate ellipse by.	 
		*/
		void Draw(const Point3D& centre, float xRadius, float zRadius, float rotation);

		/** Get radius of ellipse along x-axis (before rotation). */
		double GetRadiusX() const { return m_xRadius; }

		/** Get radius of ellipse along z-axis (before rotation). */
		double GetRadiusZ() const { return m_zRadius; }

		/** Get rotation of ellipse in (x,z) plane (in radians). */
		double GetRotation() const { return m_rotation; }

		/** Get centre of ellipse in (x,z) plane. */
		const Point3D& GetCentrePt() const { return m_centre; }

		/** Project given point onto closest point of ellipse. */
		Point3D ProjectPt(const Point3D& pt) const;

		/** Determine the position of a given point in the canonical reference frame of the ellipse (no rotation, centered at origin) */
		Point3D CanonicalPos(const Point3D& pt) const;

		/** Transfer point in canonical reference frame to elliptical reference frame. */
		Point3D EllipseRefFramePos(const Point3D& canonicalPt) const;

		/** Check if a point in the canonical reference frame is within the ellipse or not. */
		bool InEllipse(const Point3D& canonicalPt) const;

		/** Project a vector of points onto the ellipse. */
		void ProjectPts(const std::vector<Point3D>& pts, std::vector<Point3D>& projPts);

		/** Layout points evenly around ellipse. */
		void LayoutPtsEvenly(const std::vector<Point3D>& pts, std::vector<Point3D>& laidOutPts);

		/** 
		* @brief Layout charts indepdently in each quadrant of an ellipse. 
		* @param pts Points to be laid out.
		* @param laidOutPts Position of points laid out on ellipse.
		* @param bSortOutputPts If true, than laidOutPts are sorted in CCW order starting from the x-axis,
		*												otherwise they coorespond to the order of the input points.
		*/
		void LayoutPtsByQuadrant(const std::vector<Point3D>& pts, std::vector<Point3D>& laidOutPts,
			bool bSortOutputPts = false);

		/** Show properties dialog for this view. */
		void ShowPropertiesDlg();

		/** 
		* @brief Process mouse left button down events. 
		* @param mousePt Screen position of mouse.
		* @ return True if view is processing the event, else false.
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
		/**
		* @brief Determine position of orthogonal layout line based on the current mouse position.
		* @param primaryLayoutLine The primary layout line this line should be orthogonal to.
		* @param midPt The middle point of the primary layout line.
		* @return End point of othogonal layout line.
		*/
		Point3D OrthoLineEndPt(const Line3D& primaryLayoutLine, const Point3D& midPt);

		/** Handle mouse events when constructing a new ellipse. */
		void DrawEllipseInteraction();

		/** Handle mouse events when manipulating a pre-existing ellipse. */
		bool ModifyEllipseInteraction(const Point2D& mousePt);

		/** Helper function for determine ellipse based on current mouse position and currently selected control point. */
		void CalculateModifiedEllipse();

		/** Check if mouse cursor is over a control point. */
		bool IsPointClicked(const Point2D& mousePt, const Point3D& controlPt);

		/** serialization. */
		friend class boost::serialization::access;

		template<class archive>
		void serialize(archive & ar, const unsigned int version);

	private:
		/** Array of points defining the ellipse. */
		Point3D m_points[NUM_ELLIPSE_PTS];	// Note: this is a fixed-size array and not a std::vector for efficency reasons

		/** Primary layout line defining ellipse. */
		VisualLine m_primaryLayoutLine;

		/** Othogonal layout line defining ellipse. */
		VisualLine m_orthoLayoutLine;

		/** Primary layout line under modification. */
		VisualLine m_modPrimaryLayoutLine;

		/** Orthogonal layout line under modification. */
		VisualLine m_modOrthoLayoutLine;

		/** Tracks stage of ellipse construction. */
		STAGE m_stage;

		/** Tracks currently active control point. */
		ACTIVE_POINT m_activePt;

		/** Radius of ellipse along x-axis (before rotation). */
		double m_xRadius;

		/** Radius of ellipse along z-axis (before rotation). */
		double m_zRadius;

		/** Rotation of ellipse in (x,z) plane. */
		double m_rotation;

		/** Centre of ellipse in (x,z) plane. */
		Point3D m_centre;
	};

}

#endif

