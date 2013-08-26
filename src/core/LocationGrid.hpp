//=======================================================================
// Author: Timothy Mankowski
//
// Copyright 2013 Timothy Mankowski
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

#ifndef _GENGIS_LOCATION_GRID_
#define _GENGIS_LOCATION_GRID_

#include "../core/Precompiled.hpp"

#include "../core/View.hpp"
#include "../core/VisualLine.hpp"

#include "../utils/Colour.hpp"

namespace GenGIS
{
	class LocationGrid : public View
	{
	public:
		enum ALIGNMENT { LATITUDE, LONGITUDE, LOCATION, CUSTOM };

	public:
		/**
		* @brief Constructor.
		*/
		explicit LocationGrid();

		/** Destructor. */
		~LocationGrid();

		/** Determine if object is visible. */
		bool IsVisible();

		/** Render location grid. */
		void Render();

		// General
		uint   GetNumberOfDivisions() { return m_divisions; }
		double GetElevation()         { return m_elevation; }

		void SetDivisions( uint divisions )   { m_divisions = divisions; }
		void SetElevation( double elevation ) { m_elevation = elevation; }

		// Tiles
		float GetTileAlpha()      { return m_uniformColourOfTiles.GetAlpha(); }
		bool  GetTileFillStatus() { return m_showTiles; }

		void SetTileAlpha( float alpha )    { m_uniformColourOfTiles.SetAlpha( alpha ); }
		void ShowTiles( bool status )       { m_showTiles = status; }

		// Borders
		bool   GetBorderVisibility() { return m_showBorders; }
		Colour GetBorderColour()     { return m_colourOfBorders; }
		float  GetBorderAlpha()      { return m_colourOfBorders.GetAlpha(); }
		uint   GetBorderThickness()  { return m_thicknessOfBorders; }
		VisualLine::LINE_STYLE GetBorderStyle() { return m_styleOfBorders; }

		void SetBorderVisibility( bool status ) { m_showBorders = status; }
		void SetBorderColour( Colour colour )   { m_colourOfBorders = colour; }
		void SetBorderAlpha( float alpha )      { m_colourOfBorders.SetAlpha( alpha ); }
		void SetBorderThickness( uint width )   { m_thicknessOfBorders = width; }
		void SetBorderStyle( VisualLine::LINE_STYLE style ) { m_styleOfBorders = style; }

	private:
		// General
		ALIGNMENT m_gridAlignment;
		Box2D   m_mapOpenGLBoundaries;
		Point2D m_mapOffset;
		uint    m_divisions;
		float   m_elevation;

		// Tiles
		bool   m_showTiles;
		Colour m_uniformColourOfTiles;
		
		// Border
		bool   m_showBorders;
		Colour m_colourOfBorders;
		uint   m_thicknessOfBorders;
		VisualLine::LINE_STYLE m_styleOfBorders;

		// Point3D ...

		// Add serialization stuff, blah
	};
}
#endif
