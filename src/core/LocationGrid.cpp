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

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"
#include "../core/MapController.hpp"
#include "../core/LocationGrid.hpp"
#include "../core/TileModel.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/SequenceModel.hpp"
#include "../utils/ColourMap.hpp"
#include "../utils/ColourMapManager.hpp"
#include "../utils/Colour.hpp"
#include "../core/LocationSetController.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/ColourMapDiscrete.hpp"

#include "../utils/ErrorGL.hpp"

using namespace GenGIS;

LocationGrid::LocationGrid() :
	// General
	m_divideTilesAlong( LATITUDE ),
	m_divideTilesBy( AXIS ),
	m_divideTilesInto( DEGREE ),
	m_gridAlignmentStyle( ORIGIN ),
	m_mapOpenGLBoundaries( -1.0, -1.0, 1.0, 1.0 ),
	m_mapOffset( 0.0f, 0.0f ),
	m_divisions( 4 ),
	m_axisDivisions( 4 ),
	m_boxDivisions( 1 ),
	m_autoAdjustElevation( true ),
	m_elevation( 0.05f ),
	m_elevationUsed( 0.0f ),
	m_gridChanged( true ),

	// Tiles
	m_showTiles( true ),
	m_tileFillMode( UNIFORM ),
	m_uniformColourOfTiles( 0.0f, 0.5f, 0.0f, 0.3f ),
	m_defaultColourOfTiles( 0.5f, 0.5f, 0.5f, 0.3f ),
	m_combination ( TileModel::AVERAGE ),

	// Border
	m_showBorders( true ),
	m_colourOfBorders( 0.0f, 0.0f, 0.0f, 0.3f ),
	m_thicknessOfBorders( 1 ),
	m_styleOfBorders( VisualLine::SOLID )
	
{
	// Property of 'Layer' class
	m_bVisible = false;

	m_xCoordinates.clear();
	m_yCoordinates.clear();
	ColourMapManagerPtr colourMapManager = App::Inst().GetColourMapManager();
	ColourMapPtr defaultColourMap = colourMapManager->GetDefaultDiscreteColourMap();
	m_gridColourMap.reset(new ColourMapDiscrete(defaultColourMap));
}

template<class Archive> 
void LocationGrid::serialize(Archive & ar, const unsigned int version)
{
	// Serialize base class (View)
	ar & boost::serialization::base_object<View>(*this);

	// General variables
	ar & m_divideTilesAlong;     // DIVISION_AXIS
	ar & m_divideTilesBy;		 // DIVISION_TYPE
	ar & m_divideTilesInto;		 // DIVISION_BOX
	ar & m_gridAlignmentStyle;   // ALIGNMENT
	ar & m_mapOpenGLBoundaries;  // Box2D
	ar & m_mapOffset;            // Point2D
	ar & m_divisions;            // uint
	ar & m_axisDivisions;		 // uint
	ar & m_boxDivisions;		 // uint
	ar & m_autoAdjustElevation;  // bool
	ar & m_elevation;            // float
	ar & m_elevationUsed;        // float
	ar & m_xCoordinates;         // std::list<double>
	ar & m_yCoordinates;         // std::list<double>
	ar & m_locationSetLayer;     // LocationSetLayerPtr
	ar & m_gridChanged;			 // bool
	ar & m_field;				 // std::wstring
	ar & m_selectedFieldValues;	 // std::vector<double>
	ar & m_combination;			 // TileModel::DATA_COMBINE

	// Tile variables
	ar & m_showTiles;            // bool
	ar & m_tileFillMode;         // TILE_FILL
	ar & m_uniformColourOfTiles; // Colour
	ar & m_defaultColourOfTiles; // Colour
	ar & m_tileModels;           // std::vector<TileModelPtr>
	ar & m_gridColourMap;		 // ColourMapDiscretePtr

	// Border variables
	ar & m_showBorders;          // bool
	ar & m_colourOfBorders;      // Colour
	ar & m_thicknessOfBorders;   // uint
	ar & m_styleOfBorders;       // VisualLine::LINE_STYLE
}
template void LocationGrid::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void LocationGrid::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


LocationGrid::~LocationGrid()
{
}

bool LocationGrid::IsVisible() 
{ 
	return m_bVisible; 
}

void LocationGrid::SetSelectedFieldValues(std::vector<wxStaticText*> field)
{
	m_selectedFieldValues.clear();
	for( uint i = 0; i < field.size(); i++)
	{
		std::wstring fieldValue = field[i]->GetLabel();
		m_selectedFieldValues.push_back(  StringTools::ToDouble( fieldValue ) );
	}
}

void LocationGrid::GenerateTileCoordinates()
{
	if( !m_gridChanged )
		return;

	// Real terrain values of DEM map
	MapControllerPtr mapController = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController();
	m_mapOpenGLBoundaries = Box2D( -( mapController->GetWidth() / 2 ), -( mapController->GetHeight() / 2 ),
		( mapController->GetWidth() / 2 ), ( mapController->GetHeight() / 2 ) );

	// Reset Origin if needed
	if ( GetGridAlignmentStyle() == ORIGIN )
		SetMapOffset( Point2D(0,0) );

	// Set the grid elevation
	if ( m_autoAdjustElevation )
	{
		m_elevationUsed = mapController->GetMaxElevationGridSpace()
		                * mapController->GetVerticalExaggeration()
		                + 0.005; // Add a minimal amount to avoid clashing
		                         // between colours of grid and map
	}
	else
	{
		m_elevationUsed = m_elevation;
	}

	// Determine appropriate tile size according to whether the user
	// wishes to divide the latitude or longitude 'n' number of times
	double tileSize;
	double tileSizeX;
	double tileSizeY;
	if (m_divideTilesBy == AXIS )
	{
		if ( m_divideTilesAlong == LATITUDE )
			tileSize = m_mapOpenGLBoundaries.Height() / m_divisions;
		else if ( m_divideTilesAlong == LONGITUDE )
			tileSize = m_mapOpenGLBoundaries.Width() / m_divisions;
		tileSizeX = tileSize;
		tileSizeY = tileSize;
	}
	else if(m_divideTilesBy == BOX )
	{
		// divide into degrees or pixels;
		if( m_divideTilesInto == DEGREE )
		{
			// convert map origin point to lat lon, add one (degree) 
			Point3D tileOrigin( m_mapOpenGLBoundaries.x, 0, m_mapOpenGLBoundaries.y); 
			GeoCoord tileCoord;
			App::Inst().GetMapController()->GetMapModel()->GridToGeo( tileOrigin, tileCoord );
			// go to next tile boundary
			Point3D nextTile;
			GeoCoord tileNext( tileCoord.easting + m_divisions, tileCoord.northing - m_divisions);
			App::Inst().GetMapController()->GetMapModel()->LatLongToGrid( tileNext, nextTile);
			
			tileSizeX = abs(nextTile.x - tileOrigin.x);
			tileSizeY = abs(tileOrigin.z- nextTile.z);			
		}
		else if(m_divideTilesInto == PIXEL)
		{
			float pixels = float(m_divisions) / 100.0f;
			Point3D tileOrigin( m_mapOpenGLBoundaries.x, 0, m_mapOpenGLBoundaries.y); 
			Point3D nextTile( m_mapOpenGLBoundaries.x , 0, m_mapOpenGLBoundaries.y );
			nextTile.x = nextTile.x + pixels;
			nextTile.z = nextTile.z + pixels;
			
			tileSizeX = abs(nextTile.x - tileOrigin.x);
			tileSizeY = abs(tileOrigin.z- nextTile.z);
		}
	}

	// Clear any previous tile coordinates
	m_xCoordinates.clear();
	m_yCoordinates.clear();

	double currentX = m_mapOpenGLBoundaries.x;
	double currentY = m_mapOpenGLBoundaries.y;

	// Set initial coordinates 'x' and 'y' to the map origin
	m_xCoordinates.push_back( currentX );
	m_yCoordinates.push_back( currentY );

	// [ OFFSET ] ( MIDDLE * N ) [ REMAINDER ] 

	// Calculate map OFFSET for OpenGL; fmod is modulus for floats
	double mapOffsetX = fmod( ( double )m_mapOffset.x, tileSizeX );
	if ( mapOffsetX > 0 )
	{
		currentX += mapOffsetX;
		m_xCoordinates.push_back( currentX );
	}

	double mapOffsetY = fmod( ( double )m_mapOffset.y, tileSizeY );
	if ( mapOffsetY > 0 )
	{
		currentY += mapOffsetY;
		m_yCoordinates.push_back( currentY );
	}

	uint numberOfMiddleTiles;
	
	// Calculate the number of MIDDLE divisions along each axis after the offset
	numberOfMiddleTiles = floor( abs( ( m_mapOpenGLBoundaries.Width() - mapOffsetX ) / tileSizeX ) );
	for ( uint col = 0; col < numberOfMiddleTiles; col++ )
	{
		currentX += tileSizeX;
		
		m_xCoordinates.push_back( currentX );
	}

	numberOfMiddleTiles = floor( abs( ( m_mapOpenGLBoundaries.Height() - mapOffsetY ) / tileSizeY ) );
	for ( uint row = 0; row < numberOfMiddleTiles; row++ )
	{
		currentY += tileSizeY;

		m_yCoordinates.push_back( currentY );
	}

	// Add the remainder if currentX is smaller than map width
	if ( currentX < m_mapOpenGLBoundaries.dx )
	{
		currentX += m_mapOpenGLBoundaries.dx - currentX;
		m_xCoordinates.push_back( currentX );
	}

	// Add the remainder if currentY is smaller than map height
	if ( currentY < m_mapOpenGLBoundaries.dy )
	{
		currentY += m_mapOpenGLBoundaries.dy - currentY;
		m_yCoordinates.push_back( currentY );
	}

	if ( m_gridChanged )
	{
		InitTiles();
	}
}

void LocationGrid::Render()
{
	if( !IsVisible() )
		return;

	ColourMapDiscretePtr m_colourMap = m_locationSetLayer->GetLocationSetController()->GetColourMap();

	// Colour palette for tiles (temporary)
	float alphaOfTile = m_uniformColourOfTiles.GetAlpha();
	error::ErrorGL::Check();
	glDisable( GL_LIGHTING );

	// Render borders (lines)
	if ( m_showBorders )
	{
		// Set OpenGL options
		glColor4f( m_colourOfBorders.GetRed(), m_colourOfBorders.GetGreen(),
			m_colourOfBorders.GetBlue(), m_colourOfBorders.GetAlpha() );
		glEnable (GL_LINE_SMOOTH);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
		glLineWidth( m_thicknessOfBorders * App::Inst().GetResolutionFactor() );
		glEnable( GL_LINE_STIPPLE );
		glLineStipple( 2, m_styleOfBorders );

		glBegin( GL_LINES );
		std::list<double>::iterator col;
		std::list<double>::iterator row;
		for ( col = m_xCoordinates.begin(); col != m_xCoordinates.end(); ++col )
		{
			// Draw column borders
			glVertex3f( *col,  m_elevationUsed, m_mapOpenGLBoundaries.y  );
			glVertex3f( *col,  m_elevationUsed, m_mapOpenGLBoundaries.dy );
		}
		for ( row = m_yCoordinates.begin(); row != m_yCoordinates.end(); ++row )
		{
			// Draw row borders
			glVertex3f( m_mapOpenGLBoundaries.x,  m_elevationUsed, *row );
			glVertex3f( m_mapOpenGLBoundaries.dx, m_elevationUsed, *row );
		}
		glEnd();
	}

	// Render tiles
	if ( m_showTiles )
	{
		if ( m_tileFillMode == UNIFORM )
		{
			glBegin( GL_QUADS );

			glColor4f( m_uniformColourOfTiles.GetRed(), m_uniformColourOfTiles.GetGreen(),
				m_uniformColourOfTiles.GetBlue(), m_uniformColourOfTiles.GetAlpha() );

			glVertex3f( m_mapOpenGLBoundaries.x,  m_elevationUsed, m_mapOpenGLBoundaries.y  );
			glVertex3f( m_mapOpenGLBoundaries.x,  m_elevationUsed, m_mapOpenGLBoundaries.dy );
			glVertex3f( m_mapOpenGLBoundaries.dx, m_elevationUsed, m_mapOpenGLBoundaries.dy );
			glVertex3f( m_mapOpenGLBoundaries.dx, m_elevationUsed, m_mapOpenGLBoundaries.y  );

			glEnd();
		}
		else if ( m_tileFillMode == MAPPED )
		{
			// Render tiles from a stored list of previously generated coordinates
			glBegin( GL_QUADS );
			
			int i = 0;
			MapModelPtr mapModel = App::Inst().GetMapController()->GetMapModel();
			std::list<double>::iterator row1, row2, col1, col2;
			row2 = m_yCoordinates.begin(); row2++;
			//row heads of each tile
			for ( row1 = m_yCoordinates.begin(); row2 != m_yCoordinates.end(); ++row1 )
			{
				col2 = m_xCoordinates.begin(); col2++;
					
				//column heads of each tile
				for ( col1 = m_xCoordinates.begin(); col2 != m_xCoordinates.end(); ++col1 )
				{
					Colour tileColour = m_tileModels[i]->GetColour();
					glColor4f( tileColour.GetRed(), tileColour.GetGreen(), tileColour.GetBlue(), tileColour.GetAlpha() );
					glVertex3f( *col1, m_elevationUsed, *row1 );
					glVertex3f( *col2, m_elevationUsed, *row1 );
					glVertex3f( *col2, m_elevationUsed, *row2 );
					glVertex3f( *col1, m_elevationUsed, *row2 );
					col2++;
					i++;
				}
				row2++;
			}	
			glEnd();
		}
	}
}

void LocationGrid::InitTiles()
{
	m_tileModels.clear();
	MapModelPtr mapModel = App::Inst().GetMapController()->GetMapModel();
	std::list<double>::iterator row1, row2, col1, col2;
	row2 = m_yCoordinates.begin(); row2++;
	//row heads of each tile
	for ( row1 = m_yCoordinates.begin(); row2 != m_yCoordinates.end(); ++row1 )
	{
		col2 = m_xCoordinates.begin(); col2++;
			
		//column heads of each tile
		for ( col1 = m_xCoordinates.begin(); col2 != m_xCoordinates.end(); ++col1 )
		{
			Point3D topLeftCoord(*col1,m_elevationUsed,*row1);
			Point3D bottomRightCoord(*col2,m_elevationUsed,*row2);
			GeoCoord topLeftGeo;
			GeoCoord bottomRightGeo;
			mapModel->GridToGeo(topLeftCoord,topLeftGeo);
			mapModel->GridToGeo(bottomRightCoord,bottomRightGeo);
			std::wstring ID = StringTools::ToStringW(topLeftGeo.easting,2); 
			ID.append(StringTools::ToStringW("_"));
			ID.append(StringTools::ToStringW(topLeftGeo.northing,2));
			std::map<std::wstring,std::wstring> data;
			TileModelPtr tile(new TileModel(ID,std::make_pair(topLeftGeo.easting,topLeftGeo.northing),std::make_pair(bottomRightGeo.easting,bottomRightGeo.northing),data));
			m_tileModels.push_back(tile);
			col2++;
		}
		row2++;
	}
	AssertCombinationMethod();
}

void LocationGrid::FillTiles()
{
	std::vector<LocationLayerPtr> locationLayers = m_locationSetLayer->GetAllActiveLocationLayers();
	for( uint i = 0; i < locationLayers.size() ; i++)
	{
		std::map<std::wstring,std::wstring> data = locationLayers[i]->GetLocationController()->GetData();
		float easting = StringTools::ToDouble(data[StringTools::ToStringW("Longitude")]);
		float northing = StringTools::ToDouble(data[StringTools::ToStringW("Latitude")]);
		// check if long and lat values exist. if they don't use easting and westing
		if( easting != easting || northing != northing || !boost::math::isfinite(easting) || !boost::math::isfinite(northing) )
		{
			easting = locationLayers[i]->GetLocationController()->GetEasting();
			northing = locationLayers[i]->GetLocationController()->GetNorthing();
		}
		
		for ( uint j = 0; j < m_tileModels.size(); j++)
		{
			std::pair<float,float> top = m_tileModels[j]->GetTopLeft();
			std::pair<float,float> bottom = m_tileModels[j]->GetBottomRight();
			
			// floating point was proving not to be precise enough at this stage. comparing 33.180000 < 33.1800000000 was equating to true
			int eastingP = floor(easting * 100);
			int northingP = floor(northing * 100);
			int topFirst = floor(top.first * 100);
			int topSecond = floor(top.second * 100);
			int bottomFirst = floor(bottom.first * 100);
			int bottomSecond = floor(bottom.second * 100);


			if ( topFirst <= eastingP
				&& topSecond >= northingP
				&& bottomFirst > eastingP
				&& bottomSecond < northingP )
			{
				m_tileModels[j]->AddLocationLayer(locationLayers[i]);
				break;
			}
		}
	}
	// now set tile true values for every tile
	for ( uint j = 0; j < m_tileModels.size(); j++)
	{
		if(m_tileModels[j]->GetNumLocations() > 0 )
		{
			m_tileModels[j]->CombineData();
			//combine sequence layers if there are any
			if(m_tileModels[j]->GetNumSequence() > 0)
				m_tileModels[j]->CombineSequenceData();

		}
	}
}

std::vector<std::wstring> LocationGrid::GetSelectedValues( std::wstring field )
{
	std::vector<std::wstring> values;
	std::vector<TileModelPtr>::iterator it;
	for( it = m_tileModels.begin(); it != m_tileModels.end(); ++it )
	{
		std::map<std::wstring,std::wstring> data = (*it)->GetData();
		if(! data[field].empty() )
			values.push_back(data[field]);
	}

	return values;
}

void LocationGrid::SetLocationColours()
{
	for(unsigned int i = 0; i < m_tileModels.size(); ++i)
	{
		Colour colour;
		if( m_tileModels[i]->GetNumLocations() == 0 )
		{
			colour = m_defaultColourOfTiles;
		}
		else
		{
			// find colour in colour map corrsponding to field value of location
			std::map<std::wstring,std::wstring> data = m_tileModels.at(i)->GetData();
			std::map<std::wstring,std::wstring>::const_iterator it = data.find(m_field);

			if( it == data.end() )
			{
				data = m_tileModels.at(i)->GetSequence(0)->GetData();
				it = data.find(m_field);
			}
			if(!m_gridColourMap->GetColour(it->second, colour))
			{
				Log::Inst().Error("(Error) LocationGrid::SetLocationColours(): no colour associated with name.");
			}

			// set alpha of colour
			colour.SetAlpha( GetTileAlpha() );
		}

		m_tileModels.at(i)->SetColour(colour);
	}
}

TileModelPtr LocationGrid::FindLocationTile(Point2D loc)
{
//  TESTING

//	Point3D locationCoord; //(xAxisPosition,0.5f,yAxisPosition);
//	GeoCoord locationGeo( -71.52,-31.31 );
//	GeoCoord locationGeo( -71.5,-32.11 );
//	GeoCoord locationGeo( -70.69,-31.97 );
//	GeoCoord locationGeo( -70.91,-31.21 );
//	GeoCoord locationGeo( -70.52,-33.61 );
//	GeoCoord locationGeo( -66.44,-31.31 );
//	GeoCoord locationGeo( -66.52,-33.46 );

//	App::Inst().GetMapController()->GetMapModel()->LatLongToGrid(locationGeo,locationCoord);
//	loc.x = locationCoord.x;
//	loc.y = locationCoord.z;

//	END TESTING

	// shift x and y to only positive values
	loc.x = loc.x - m_mapOpenGLBoundaries.x;
	loc.y = loc.y - m_mapOpenGLBoundaries.y;

	int whichColumn = 0;
	int whichRow = 0;
	// Determine appropriate tile size according to whether the user
	// wishes to divide the latitude or longitude 'n' number of times
	double tileSize;
	if ( m_divideTilesAlong == LATITUDE )
		tileSize = m_mapOpenGLBoundaries.Height() / m_divisions;
	else if ( m_divideTilesAlong == LONGITUDE )
		tileSize = m_mapOpenGLBoundaries.Width() / m_divisions;

	// calculate tile row
	if( (loc.x + m_mapOpenGLBoundaries.dx) < m_mapOffset.x )
	{
		whichColumn = 0;
	}
	else
	{
		double x = loc.x - m_mapOffset.x; 
		float divisions = m_mapOpenGLBoundaries.Height() / tileSize ; 
		float tileRatio =  m_mapOpenGLBoundaries.Height() / divisions;
		whichColumn = x / tileRatio;;
	}
	// calculate tile column
	if( (loc.y + m_mapOpenGLBoundaries.dy) < m_mapOffset.y )
	{
		whichRow = 0;
	}
	else
	{
		double y = ( loc.y - m_mapOffset.y );
		float divisions = m_mapOpenGLBoundaries.Width() / tileSize ; 
		float tileRatio = m_mapOpenGLBoundaries.Width() / divisions;
		whichRow = y / tileRatio ;
	}
	uint divisionIndex = 0;
	if ( m_divideTilesAlong == LATITUDE )
	{
		float divisions = floor( m_mapOpenGLBoundaries.Width() / tileSize ); 
		divisionIndex = whichRow * (divisions) + whichRow + whichColumn;
	}
	else if ( m_divideTilesAlong == LONGITUDE )
	{
		float divisions = floor( m_mapOpenGLBoundaries.Width() / tileSize ); 
		divisionIndex = whichRow * divisions + whichColumn;
	}

	TileModelPtr tile = m_tileModels[divisionIndex];

	return tile;
}

void LocationGrid::SetOriginOffset( std::wstring selectedName )
{
	if( GetGridAlignmentStyle() == LOCATIONS )
	{
		float xAxisPosition = m_locationSetLayer->GetLocationLayer( selectedName )->GetLocationController()->GetEasting();
		float yAxisPosition = m_locationSetLayer->GetLocationLayer( selectedName )->GetLocationController()->GetNorthing();

		//convert axis positions to open gl coordinate
		Point3D locationCoord;
		GeoCoord locationGeo(xAxisPosition,yAxisPosition);
		App::Inst().GetMapController()->GetMapModel()->GeoToGrid(locationGeo,locationCoord);

		//find which tile this location belongs in
		TileModelPtr tile = FindLocationTile( Point2D(locationCoord.x,locationCoord.z) );

		GeoCoord tileOrigin( tile->GetTopLeft().first, tile->GetTopLeft().second ); 
		Point3D tileCoord;
		App::Inst().GetMapController()->GetMapModel()->LatLongToGrid( tileOrigin, tileCoord );

		// negative offsets mean the wrong tile has been selected
		float xOffset =  locationCoord.x - tileCoord.x;
		float yOffset =  locationCoord.z - tileCoord.z;
	
		SetMapOffset( Point2D(xOffset,yOffset) );
	}
}

void LocationGrid::SetOriginOffset( Point2D coord )
{
	if( GetGridAlignmentStyle() == COORDINATES )
	{
		float xAxisPosition = coord.x;
		float yAxisPosition = coord.y;

		//convert axis positions to open gl coordinate
		Point3D locationCoord;
		GeoCoord locationGeo(xAxisPosition,yAxisPosition);
		App::Inst().GetMapController()->GetMapModel()->LatLongToGrid(locationGeo,locationCoord);

		
		double tileSize;
		if ( m_divideTilesAlong == LATITUDE )
			tileSize = m_mapOpenGLBoundaries.Height() / m_divisions;
		else if ( m_divideTilesAlong == LONGITUDE )
			tileSize = m_mapOpenGLBoundaries.Width() / m_divisions;

		// handle boundries
		if ((locationCoord.z > m_mapOpenGLBoundaries.dy) || (locationCoord.z < m_mapOpenGLBoundaries.y))	// lat boundary
		{		
			double tileSizeY = tileSize;
			if (locationCoord.z > m_mapOpenGLBoundaries.y)
				tileSizeY = tileSize * -1;
			while ( ( locationCoord.z > m_mapOpenGLBoundaries.dy) || (locationCoord.z < m_mapOpenGLBoundaries.y ) )
				locationCoord.z = locationCoord.z + tileSizeY;
		}
		if ( (locationCoord.x > m_mapOpenGLBoundaries.dx) || (locationCoord.x < m_mapOpenGLBoundaries.x) )	// lon boundary
		{		
			double tileSizeX = tileSize;
			if (locationCoord.x > m_mapOpenGLBoundaries.x)
				tileSizeX = tileSize * -1;
			while( (locationCoord.x > m_mapOpenGLBoundaries.dx) || (locationCoord.x < m_mapOpenGLBoundaries.x) )
				locationCoord.x = locationCoord.x + tileSizeX;
		}


		//find which tile this location belongs in
		TileModelPtr tile = FindLocationTile( Point2D(locationCoord.x,locationCoord.z) );

		GeoCoord tileOrigin( tile->GetTopLeft().first, tile->GetTopLeft().second ); 
		Point3D tileCoord;
		App::Inst().GetMapController()->GetMapModel()->LatLongToGrid( tileOrigin, tileCoord );

		float xOffset =  locationCoord.x - tileCoord.x;
		float yOffset =  locationCoord.z - tileCoord.z;

		SetMapOffset( Point2D(xOffset,yOffset) );
	}
}

void LocationGrid::AssertCombinationMethod()
{
	for(uint i = 0; i < m_tileModels.size(); i++)
	{
		m_tileModels[i]->SetCombinationMethod(m_combination);
	}
}

TileModel::DATA_COMBINE LocationGrid::GetCombinationMethod()
{
	return m_tileModels[0]->GetCombinationMethod();
}

void LocationGrid::SetAxisDivisions( uint divisions )
{
	m_axisDivisions = divisions;
	SetDivisions( divisions );

}
void LocationGrid::SetBoxDivisions( uint divisions )
{
	m_boxDivisions = divisions;
	SetDivisions( divisions );

}