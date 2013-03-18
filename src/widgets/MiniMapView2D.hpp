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

#ifndef _GENGIS_MINIMAPVIEW2D_
#define _GENGIS_MINIMAPVIEW2D_

#include "../core/Precompiled.hpp"

#include "../widgets/ViewWidget.hpp"
#include "../core/VectorMapModel.hpp"

namespace GenGIS
{

	/**
	* @class MiniMapView2D
	* @brief Minimap widget.
	*/
	class MiniMapView2D : public ViewWidget 
	{
	public:
		/** Constructor. */
		explicit MiniMapView2D();

		/** Destructor. */
		~MiniMapView2D() {}

		/** Render minimap. */
		void Render();

		/** Set origin of minimap in screen coordinates. */
		void SetOrigin( float x, float y )	{ m_origin.x = x; m_origin.y = y; }

		/** Get origin of minimap in screen coordinates. */
		const Point2D& GetOrigin() const { return m_origin; }

		/** Convert geographic coordinates to minimap coordinates. */
		Point3D GeoCoordToMiniMapCoord( GeoCoord& geoCoord );		


		/** Convert minimap coordinates to geographic coordinates. */
		GeoCoord MiniMapCoordToGeoCoord( Point3D& miniMapCoord );

		/** Convert minimap coordinates to world coordinates. */
		void MiniMapCoordToWorldCoord( Point3D& miniMapCoord, Point3D& worldCoord );  

		/** Convert world coordinates to minimap coordinates. */
		Point3D WorldCoordToMiniMapCoord( Point3D& worldCoord );

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
		/** Check if minimap was clicked. */
		bool IsClicked(const Point2D& mousePt);

		/** Render terrain in minimap. */
		void DrawTerrain();

	private:  
		/** Origin of widget in screen coordinates. */
		Point2D m_origin;
	};

}

#endif
