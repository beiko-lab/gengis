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
#include "../utils/StringTools.hpp"
//#include "../utils/ColourMapDiscrete.hpp"

namespace GenGIS
{
	class LocationGrid : public View
	{
	public:
		// Fill style for tiles
		enum TILE_FILL { NONE, UNIFORM, MAPPED };

		// Whether size of tiles is determined by dividing along
		// latitude or longitude
		enum DIVISION_AXIS { LATITUDE, LONGITUDE };

		// Whether tiles/grid are alied at the map origin, along
		// a location, or at specific coordinates
		enum ALIGNMENT { ORIGIN, LOCATIONS, COORDINATES };

	public:
		/**
		* @brief Constructor.
		*/
		explicit LocationGrid();

		/** Destructor. */
		~LocationGrid();

		/** Determine if object is visible. */
		bool IsVisible();

		/** Generate coordinates or grid. */
		void GenerateTileCoordinates();

		/** Generate tiles for grid. */
		void InitTiles();

		/** Fill tiles with location values. */
		void FillTiles();

		/** Render location grid. */
		void Render();



		// Functions for accessing 'general' variables
		DIVISION_AXIS GetTileDivisionAxis()           { return m_divideTilesAlong; }
		ALIGNMENT     GetGridAlignmentStyle()         { return m_gridAlignmentStyle; }
		uint          GetNumberOfDivisions()          { return m_divisions; }
		bool          GetAutoAdjustElevationStatus () { return m_autoAdjustElevation; }
		double        GetElevation()                  { return m_elevation; }

		void SetTileDivisionAxis( DIVISION_AXIS divideAlong )            { m_divideTilesAlong = divideAlong; }
		void SetGridAlignmentStyle( ALIGNMENT alignment )                { m_gridAlignmentStyle = alignment; }
		void SetDivisions( uint divisions )                              { m_divisions = divisions; }
		void SetAutoAdjustElevationStatus ( bool status )                { m_autoAdjustElevation = status; }
		void SetElevation( double elevation )                            { m_elevation = elevation; }
		void SetLocationSetLayer( LocationSetLayerPtr locationSetLayer ) { m_locationSetLayer = locationSetLayer; } 
		void SetSelectedFieldValues(std::vector<wxStaticText*> field);
		void SetField( std::wstring field )								 { m_field = field; }
		std::wstring GetField()											 { return m_field; }
		bool GetGridChanged()											 { return m_gridChanged; }
		void SetGridChanged( bool val )									 { m_gridChanged = val; }
		std::vector<std::wstring> GetSelectedValues( std::wstring field);


		// Functions for accessing 'tile' variables
		bool      GetTileFillStatus()    { return m_showTiles; }
		TILE_FILL GetTileFillMode()      { return m_tileFillMode; }
		float     GetTileAlpha()         { return m_uniformColourOfTiles.GetAlpha(); }
		float	  GetDefaultTileAlpha()  { return m_defaultColourOfTiles.GetAlpha(); }
		Colour    GetTileUniformColour() { return m_uniformColourOfTiles; }
		Colour	  GetTileDefaultColour() { return m_defaultColourOfTiles; }
		bool GetVisible()												 { return IsVisible(); }
		std::vector<TileModelPtr> GetTileModels()						 { return m_tileModels; }
		ColourMapDiscretePtr GetColourMap()								 { return m_gridColourMap; }
		Point2D GetMapOffset()											 { return m_mapOffset; }

		void ShowTiles( bool status )              { m_showTiles = status; }
		void SetTileFillMode( TILE_FILL mode )     { m_tileFillMode = mode; }
		void SetTileAlpha( float alpha )           { m_uniformColourOfTiles.SetAlpha( alpha ); }
		void SetDefaultTileAlpha( float alpha )		   { m_defaultColourOfTiles.SetAlpha( alpha ); }
		void SetTileUniformColour( Colour colour ) { m_uniformColourOfTiles = colour; }
		void SetTileDefaultColour( Colour colour ) { m_defaultColourOfTiles = colour; }

		void SetLocationColours();
		void SetColourMap( ColourMapDiscretePtr colourMap )				 { m_gridColourMap = colourMap; }
		void SetMapOffset( Point2D newOffset )							 { m_mapOffset = newOffset; }

		int LocationGrid::FindLocationTile(Point2D loc);

		// Functions for accessing 'border' variables
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
		/* Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	private:
		// General variables
		DIVISION_AXIS m_divideTilesAlong;
		ALIGNMENT     m_gridAlignmentStyle;
		Box2D         m_mapOpenGLBoundaries;
		Point2D       m_mapOffset;
		uint          m_divisions;
		bool          m_autoAdjustElevation;
		float         m_elevation;
		float         m_elevationUsed;
		std::list<double> m_xCoordinates;
		std::list<double> m_yCoordinates;
		LocationSetLayerPtr m_locationSetLayer;
		std::vector<double> m_selectedFieldValues;
		std::wstring m_field;
		bool		 m_gridChanged;

		// Tile variables
		bool      m_showTiles;
		TILE_FILL m_tileFillMode;
		Colour    m_uniformColourOfTiles;
		Colour	  m_defaultColourOfTiles;
		std::vector<TileModelPtr> m_tileModels;
		ColourMapDiscretePtr m_gridColourMap;
		
		// Border variables
		bool   m_showBorders;
		Colour m_colourOfBorders;
		uint   m_thicknessOfBorders;
		VisualLine::LINE_STYLE m_styleOfBorders;
	};
}
#endif
