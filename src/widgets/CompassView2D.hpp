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

#ifndef _GENGIS_COMPASSVIEW2D_
#define _GENGIS_COMPASSVIEW2D_

#include "../core/Precompiled.hpp"

#include "../widgets/ViewWidget.hpp"

namespace GenGIS
{

	/**
	* @class CompassView2D
	* @brief Compass widget.
	*/
	class CompassView2D : public ViewWidget
	{
	public:     
		/** Default constructor. */
		explicit CompassView2D() {}

		/** Constructor. */
		explicit CompassView2D( float theta );

		/** Set angle of compass. */
		void SetAngle( float theta );

		/** Get angle of compass. */
		float GetAngle() const { return m_theta; }

		/** Set center point of compass in screen coordinates. */
		void SetPivotPoint( float x, float y ) 
		{
			m_pivot.x = x;
			m_pivot.y = y;
		}

		/** Get center point of compass in screen coordinates. */
		const Point2D& GetPivotPoint() const { return m_pivot; }

		/** Get radius of compass in screen coordinates. */
		const Point2D& GetRadii() const { return m_radii; }

		/** Set radius of compass in screen coordinates. */
		void SetRadii( float x, float y ) 
		{
			m_radii.x = x;
			m_radii.y = y;
		}

		/** Render compass. */
		void Render();

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

		/** 
		* @brief Process mouse dragging events. 
		* @param mousePt Screen position of mouse.
		*/
		void MouseDragging(const Point2D& mousePt) { MouseLeftDown(mousePt); return; }

	private:
		/** 
		* @brief Set angle on compass based on mouse click.
		* @param mousePt Screen position of mouse.
		*/
		float AngleOnCompass(const Point2D& mousePt);

		/** Draw tick marks on compass. */
		void DrawTickMarks();

		/** Draw north mark on compass. */
		void DrawNorthNeedle();       

	private:
		/** Angle of compass. */
		float m_theta;

		/** Center point of compass in screen coordinates. */
		Point2D m_pivot;

		/** Radius of compass in screen coordinates. */
		Point2D m_radii;		 
	};

}

#endif
