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

#ifndef _GENGIS_WIDGET_CONTROLLER_
#define _GENGIS_WIDGET_CONTROLLER_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/**
	* @class WidgetController
	* @brief Manage OpenGL widgets.
	*/
	class WidgetController: private Uncopyable
	{
	public:
		/** Constructor. */
		explicit WidgetController(ViewportPtr viewport);

		/** Destructor. */
		~WidgetController() {}

		/** Set the viewport. */
		void SetViewport(ViewportPtr viewport) { m_viewport = viewport; }

		/** Set angle of compass widget. */
		void SetCompassAngle(float angle);

		/** Toggle visibility of compass widget. */
		void ToggleCompassVisibility();

		/** Set visibility of compass widget. */
		void SetCompassVisibility(bool bVisible);

		/** Toggle visibility of mini map widget. */
		void ToggleMiniMapVisibility();

		/** Set visibility of mini map widget. */
		void SetMiniMapVisibility(bool bVisible);

		/** Toggle visibility of navigation controller widget. */
		void ToggleNavVisibility();

		/** Set visibility of navigation controller widget. */
		void SetNavVisibility(bool bVisible);

		/**
		* @brief Determine if mouse is over a widget when mouse left down event occurs.
		* @param mousePt Position of mouse.
		*/
		bool MouseLeftDown(const Point2D& mousePt);

		/**
		* @brief Determine if mouse is over a widget when mouse left up event occurs.
		* @param mousePt Position of mouse.
		*/
		void MouseLeftUp(const Point2D& mousePt);

		/** 
		* @brief Process mouse right button down events. 
		* @param mousePt Screen position of mouse.
		* @param popupMenu Popup menu to populate.
		* @return True if view is processing the event, else false.
		*/
		bool MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu);

	protected:
		/** Initialize the compass widget. */
		void ViewCompass();

		/** Initialize the navigation control widget. */
		void ViewNavControl();

		/** Initialize the mini map widget. */
		void ViewMiniMap();

		/** Set navigation control texture image. */
		void SetNavImages(unsigned char* data, unsigned char* alphaData, int width, int height);

		/** Set compass control texture image. */
		void SetCompassImages( unsigned char* data, unsigned char* alphaData, int width, int height,
			unsigned char* data2, unsigned char* alphaData2, int width2, int height2 ) ;

		/** Set mini map control texture image. */
		void SetMiniMapImages(unsigned char* data, unsigned char* alphaData, int width, int height);

		/** Render all widgets. */
		void RenderWidgets();

	protected:
		/** OpenGL viewport generating events that will be handled by this controller. */
		ViewportPtr m_viewport;

		/** Compass widget. */
		CompassView2DPtr m_compass;

		/** Texture for compass widget. */
		ViewWidgetPtr m_compassDecoration;

		/** Navigation widget. */
		NavView2DPtr m_nav;

		/** Mini map widget. */
		MiniMapView2DPtr m_miniMap;

		/** Texture map for mini map widget. */
		ViewWidgetPtr m_miniMapDecoration;
	};

}

#endif
