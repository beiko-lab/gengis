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

#ifndef _GENGIS_NAVVIEW2D_
#define _GENGIS_NAVVIEW2D_

#include "../core/Precompiled.hpp"

#include "../widgets/ViewWidget.hpp"

namespace GenGIS
{
	/**
	* @class NavView2D
	* @brief Navigation widget.
	*/
	class NavView2D : public ViewWidget
	{
	public:
		/** Constructor. */
		explicit NavView2D();

		/** Get origin of zoom in button in screen coordinates. */
		const Point2D& GetZoomInOrigin() const { return m_zoomInOrigin; }

		/** Set origin of zoomin  button in screen coordinates. */
		void SetZoomInOrigin( float x, float y ) 
		{
			m_zoomInOrigin.x = x;
			m_zoomInOrigin.y = y;
		}

		/** Get origin of zoom out button in screen coordinates. */
		const Point2D& GetZoomOutOrigin() const { return m_zoomOutOrigin; }

		/** Set origin of zoom out button in screen coordinates. */
		void SetZoomOutOrigin( float x, float y ) 
		{
			m_zoomOutOrigin.x = x;
			m_zoomOutOrigin.y = y;
		}

		/** Get radius of zoom in button in screen coordinates. */
		const Point2D& GetZoomInRadii() const { return m_zoomInRadii; }

		/** Set radius of zoom in button in screen coordinates. */
		void SetZoomInRadii( float x, float y ) 
		{
			m_zoomInRadii.x = x;
			m_zoomInRadii.y = y;
		}

		/** Get radius of zoom out button in screen coordinates. */
		const Point2D& GetZoomOutRadii() const { return m_zoomOutRadii; }

		/** Set radius of zoom out button in screen coordinates. */
		void SetZoomOutRadii( float x, float y ) 
		{
			m_zoomOutRadii.x = x;
			m_zoomOutRadii.y = y;
		}

		/** Get origin of left button in screen coordinates. */
		const Point2D& GetLeftArrowOrigin() const { return m_leftArrowOrigin; }

		/** Set origin of left button in screen coordinates. */
		void SetLeftArrowOrigin( float x, float y ) 
		{
			m_leftArrowOrigin.x = x;
			m_leftArrowOrigin.y = y;
		}

		/** Get origin of right button in screen coordinates. */
		const Point2D& GetRightArrowOrigin() const { return m_rightArrowOrigin; }

		/** Set origin of right button in screen coordinates. */
		void SetRightArrowOrigin( float x, float y ) 
		{
			m_rightArrowOrigin.x = x;
			m_rightArrowOrigin.y = y;
		}

		/** Get radius of left button in screen coordinates. */
		const Point2D& GetLeftArrowRadii() const { return m_leftArrowRadii; }

		/** Set radius of left button in screen coordinates. */
		void SetLeftArrowRadii( float x, float y ) 
		{
			m_leftArrowRadii.x = x;
			m_leftArrowRadii.y = y;
		}

		/** Get radius of right button in screen coordinates. */
		const Point2D& GetRightArrowRadii() const { return m_rightArrowRadii; }

		/** Set radius of right button in screen coordinates. */
		void SetRightArrowRadii( float x, float y ) 
		{
			m_rightArrowRadii.x = x;
			m_rightArrowRadii.y = y;
		}

		/** Get origin of up button in screen coordinates. */
		const Point2D& GetUpArrowOrigin() const { return m_upArrowOrigin; }

		/** Set origin of up button in screen coordinates. */
		void SetUpArrowOrigin( float x, float y ) 
		{
			m_upArrowOrigin.x = x;
			m_upArrowOrigin.y = y;
		}

		/** Get origin of down button in screen coordinates. */
		const Point2D& GetDownArrowOrigin() const { return m_downArrowOrigin; }

		/** Set origin of down button in screen coordinates. */
		void SetDownArrowOrigin( float x, float y ) 
		{
			m_downArrowOrigin.x = x;
			m_downArrowOrigin.y = y;
		}

		const Point2D& GetUpArrowRadii() const { return m_upArrowRadii; }
		void SetUpArrowRadii( float x, float y ) {
			m_upArrowRadii.x = x;
			m_upArrowRadii.y = y;
		}

		/** Get radiius of down button in screen coordinates. */
		const Point2D& GetDownArrowRadii() const { return m_downArrowRadii; }

		/** Set radiius of down button in screen coordinates. */
		void SetDownArrowRadii( float x, float y ) 
		{
			m_downArrowRadii.x = x;
			m_downArrowRadii.y = y;
		}

		/** 
		* @brief Process mouse left button down events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		bool MouseLeftDown(const Point2D& mousePt);

		/** 
		* @brief Process mouse left button float click events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		void MouseLeftDblClick(const Point2D& mousePt) { MouseLeftDown(mousePt); }

	protected:
		/** Check if mouse click occured on zoom in button. */
		bool IsOnZoomIn(const Point2D& mousePt);

		/** Check if mouse click occured on zoom out button. */
		bool IsOnZoomOut(const Point2D& mousePt);

		/** Check if mouse click occured on left button. */
		bool IsOnLeftArrow(const Point2D& mousePt);

		/** Check if mouse click occured on right button. */
		bool IsOnRightArrow(const Point2D& mousePt);

		/** Check if mouse click occured on up button. */
		bool IsOnUpArrow(const Point2D& mousePt);

		/** Check if mouse click occured on down button. */
		bool IsOnDownArrow(const Point2D& mousePt);

		/** Check if mouse click occured within the specified ellipse. */
		bool IsInEllipse(const Point2D& mousePt, const Point2D& origin, const Point2D& radii);

	protected:
		/** Origin of zoom in button in screen coordinates. */
		Point2D m_zoomInOrigin;

		/** Origin of zoom in button out screen coordinates. */
		Point2D m_zoomOutOrigin;

		/** Radius of zoom in button in screen coordinates. */
		Point2D m_zoomInRadii;

		/** Radius of zoom out button in screen coordinates. */
		Point2D m_zoomOutRadii;

		/** Origin of left button in screen coordinates. */
		Point2D m_leftArrowOrigin;

		/** Origin of right button in screen coordinates. */
		Point2D m_rightArrowOrigin;

		/** Radius of left button in screen coordinates. */
		Point2D m_leftArrowRadii;

		/** Radius of right button in screen coordinates. */
		Point2D m_rightArrowRadii;

		/** Origin of up button in screen coordinates. */
		Point2D m_upArrowOrigin;

		/** Origin of down button in screen coordinates. */
		Point2D m_downArrowOrigin;

		/** Radius of up button in screen coordinates. */
		Point2D m_upArrowRadii;

		/** Radius of down button in screen coordinates. */
		Point2D m_downArrowRadii;
	};

}

#endif
