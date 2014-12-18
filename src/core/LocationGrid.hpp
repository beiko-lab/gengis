//=======================================================================
// Author: Timothy Mankowski & Alexander Keddy
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
#include "../core/TileModel.hpp"

#include "../core/BST.hpp"
//#include "../red-black/RedBlackTree.h"
#include "../red-black/PokeTree.h"

//#include "../utils/ColourMapDiscrete.hpp"

namespace GenGIS
{
	// A simpe struct to store tile bounds.
	struct locBound
	{
		locBound( Point2D tL, Point2D bR ): topLeft(tL), botRight(bR){ }
		Point2D topLeft;
		Point2D botRight;

		// custom contains function: 0 = contains, 1 = larger, -1 = smaller
		int Contains(Point2D loc)
		{
			// if loc is within tile
			if( ( topLeft.x <= loc.x && loc.x <= botRight.x) && ( botRight.y <= loc.y && loc.y <= topLeft.y ) )
			{
				return 0;
			}
			// if less than
			else if( ( loc.x < topLeft.x && loc.y > botRight.y ) || ( loc.x > topLeft.x && loc.y > topLeft.y ) )
				return -1;
			else if( (loc.x < botRight.x && loc.y < topLeft.y ) || (loc.x > botRight.x && loc.y < topLeft.y ) )
				return 1;
			// some error has happened
			return 99;	
		}

		Point2D GetTopLeft(){ return topLeft;}
		Point2D GetBottomRight(){ return botRight;}
	};

	// A simple struct to store all possible tile bounds
	struct locationBoundArray
	{
		locationBoundArray(){}

		// Search all tiles for a location point, return the bounds
		locBound Search(Point2D point)
		{	
			bool found = false;
			int it  = locArr.size()/2;
			int min = 0;
			int max = locArr.size()-1;
	//		while( !found && ( min != max ) )
			while( !found && ( min <= max )  )
			{
				it = ( max + min )/2;
				int contained = locArr[it].Contains( point );
				if( contained == 0 )
					return locArr[it];
				else if ( contained < 0 )
				{
					max = it - 1;
				}
				else
				{
					min = it + 1;
				}
			}
			// return impossible point if it doesn't work
			return locBound(Point2D(-190,-100),Point2D(-190,-100));
		}

		void Add(locBound b)
		{
			locArr.push_back(b);
		}

		void Add(Point2D tL, Point2D bR)
		{
			locArr.push_back( locBound(tL,bR) );
		}

		void Clear()
		{
			locArr.clear();
		}

		locBound At(int i)
		{
			return locArr[i];
		}

		std::vector<locBound> locArr;
		
	};

	class LocationGrid : public View
	{
	public:
		// Fill style for tiles
		enum TILE_FILL { NONE, UNIFORM, MAPPED };

		// Whether size of tiles is determined by dividing along
		// latitude or longitude
		enum DIVISION_AXIS { LATITUDE, LONGITUDE };

		// Whether size of tiles is determined by an axis
		//  (lat/lon) or by display properties 
		// (degree/ pixel)
		enum DIVISION_TYPE { AXIS, BOX };

		// Wether size of tiles is determined by  dividing into
		// degrees or pixels
		enum DIVISION_BOX { DEGREE, PIXEL };

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

		/** Updates the colour map of the grid for qualitative data */
		void UpdateQualitativeColourMap();

		/** Updates the colour map of the grid for numeric data - 
		*** interpolates colour between the min and max values */
		void UpdateNumericColourMap(float min, float max);

		/** Initializes m_minFieldValue and m_maxField value
		*** to the absolute min/max value that a tile in the grid can have */
		void InitTileMinMax();

		/** Updates the colour of the grid tiles */
		void UpdateGridColours();

		/** Render location grid. */
		void Render();



		// Functions for accessing 'general' variables
		DIVISION_AXIS GetTileDivisionAxis()           { return m_divideTilesAlong; }
		DIVISION_TYPE GetTileDivisionType()			  { return m_divideTilesBy; }
		DIVISION_BOX  GetTileDivisionBox()			  { return m_divideTilesInto;}
		ALIGNMENT     GetGridAlignmentStyle()         { return m_gridAlignmentStyle; }
		uint          GetNumberOfDivisions()          { return m_divisions; }
		uint		  GetNumberOfAxisDivisions()	  { return m_axisDivisions; }
		uint		  GetNumberOfBoxDivisions()		  { return m_boxDivisions; }
		bool          GetAutoAdjustElevationStatus () { return m_autoAdjustElevation; }
		double        GetElevation()                  { return m_elevation; }
		float		  GetMinFieldValue()			  { return m_minFieldValue; }
		float		  GetMaxFieldValue()			  { return m_maxFieldValue; }

		void SetTileDivisionAxis( DIVISION_AXIS divideAlong )            { m_divideTilesAlong = divideAlong; }
		void SetTileDivisionType( DIVISION_TYPE divideType )			 { m_divideTilesBy = divideType; }
		void SetTileDivisionBox ( DIVISION_BOX divideBox )				 { m_divideTilesInto = divideBox; }
		void SetGridAlignmentStyle( ALIGNMENT alignment )                { m_gridAlignmentStyle = alignment; }
		void SetDivisions( uint divisions )                              { m_divisions = divisions; }
		void SetAxisDivisions( uint divisions );
		void SetBoxDivisions( uint divisions);
		void SetAutoAdjustElevationStatus ( bool status )                { m_autoAdjustElevation = status; }
		void SetElevation( double elevation )                            { m_elevation = elevation; }
		void SetMinFieldValue ( float minValue )						 { m_minFieldValue = minValue; }
		void SetMaxFieldValue ( float maxValue )						 { m_maxFieldValue = maxValue; }
		void SetLocationSetLayer( LocationSetLayerPtr locationSetLayer ) { m_locationSetLayer = locationSetLayer; } 
		void SetSelectedFieldValues(std::vector<wxStaticText*> field);
		void SetField( std::wstring field )								 { m_field = field; }
		std::wstring GetField()											 { return m_field; }
		bool GetGridChanged()											 { return m_gridChanged; }
		void SetGridChanged( bool val )									 { m_gridChanged = val; }
		std::vector<std::wstring> GetSelectedValues( std::wstring field);
		void SetCombinationMethod( TileModel::DATA_COMBINE combination) { m_combination = combination; }
		void AssertCombinationMethod();
		TileModel::DATA_COMBINE GetCombinationMethod();


		// Functions for accessing 'tile' variables
		bool      GetTileFillStatus()    { return m_showTiles; }
		TILE_FILL GetTileFillMode()      { return m_tileFillMode; }
		float     GetTileAlpha()         { return m_uniformColourOfTiles.GetAlpha(); }
		float	  GetDefaultTileAlpha()  { return m_defaultColourOfTiles.GetAlpha(); }
		Colour    GetTileUniformColour() { return m_uniformColourOfTiles; }
		Colour	  GetTileDefaultColour() { return m_defaultColourOfTiles; }
		bool GetVisible()												 { return IsVisible(); }
	
		std::vector<TileModelPtr> GetTileModels()						 { return m_tileModels; }
		std::vector<TileModelPtr> GetPokeModels( );
		std::vector<TileModelPtr> GetBSTModels();
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


		TileModelPtr FindLocationTile(Point2D loc);
		TileModelPtr BinarySearch(Point2D loc);

		TileModelPtr BSTSearch(Point2D loc);
		int GetPokeCount(){ return m_tilePokeTree.GetLocCount(); }
		void MakeBST();
		void InitBoundArray();
		void MakePokeTree();


		void SetOriginOffset( std::wstring selectedName );
		void SetOriginOffset( Point2D coord );

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
		DIVISION_TYPE m_divideTilesBy;
		DIVISION_BOX  m_divideTilesInto;
		ALIGNMENT     m_gridAlignmentStyle;
		Box2D         m_mapOpenGLBoundaries;
		Point2D       m_mapOffset;
		uint          m_divisions;
		uint		  m_axisDivisions;
		uint		  m_boxDivisions;
		bool          m_autoAdjustElevation;
		float         m_elevation;
		float         m_elevationUsed;
		float		  m_minFieldValue;
		float		  m_maxFieldValue;
		std::list<double> m_xCoordinates;
		std::list<double> m_yCoordinates;
		LocationSetLayerPtr m_locationSetLayer;
		std::vector<double> m_selectedFieldValues;
		std::wstring m_field;
		bool		 m_gridChanged;
		TileModel::DATA_COMBINE m_combination;

		// Tile variables
		bool      m_showTiles;
		TILE_FILL m_tileFillMode;
		Colour    m_uniformColourOfTiles;
		Colour	  m_defaultColourOfTiles;
		std::vector<TileModelPtr> m_tileModels;
		BST m_tileBST;
		PokeTree m_tilePokeTree;
		//	RedBlackTree<TileModelPtr> m_tilePokeTree;
		locationBoundArray m_tileBounds;
		

		ColourMapDiscretePtr m_gridColourMap;
		
		// Border variables
		bool   m_showBorders;
		Colour m_colourOfBorders;
		uint   m_thicknessOfBorders;
		VisualLine::LINE_STYLE m_styleOfBorders;
	};
}
#endif
