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

#include "../utils/ErrorGL.hpp"

using namespace GenGIS;

LocationGrid::LocationGrid() :
	// General
	m_divideTilesAlong( LONGITUDE ),
	m_gridAlignmentStyle( ORIGIN ),
	m_mapOpenGLBoundaries( -1.0, -1.0, 1.0, 1.0 ),
	//m_mapOffset( 0.2f, 0.3f ),
	m_mapOffset( 0.0f, 0.0f ),
	m_divisions( 12 ),
	m_autoAdjustElevation( true ),
	m_elevation( 0.05f ),
	m_elevationUsed( 0.0f ),

	// Tiles
	m_showTiles( true ),
	m_tileFillMode( MAPPED ),
	m_uniformColourOfTiles( 0.0f, 0.5f, 0.0f, 0.3f ),

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
}

LocationGrid::~LocationGrid()
{
}

bool LocationGrid::IsVisible() 
{ 
	return m_bVisible; 
}

void LocationGrid::GenerateTileCoordinates()
{
	if( !IsVisible() )
		return;

	// Real terrain values of DEM map
	//mapController->GetMapBorders();
	//mapController->GetProjectionExtents();
	
	MapControllerPtr mapController = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController();
	m_mapOpenGLBoundaries = Box2D( -( mapController->GetWidth() / 2 ), -( mapController->GetHeight() / 2 ),
		( mapController->GetWidth() / 2 ), ( mapController->GetHeight() / 2 ) );

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
	if ( m_divideTilesAlong == LATITUDE )
		tileSize = m_mapOpenGLBoundaries.Height() / m_divisions;
	else if ( m_divideTilesAlong == LONGITUDE )
		tileSize = m_mapOpenGLBoundaries.Width() / m_divisions;

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
	double mapOffsetX = fmod( ( double )m_mapOffset.x, tileSize );
	if ( mapOffsetX > 0 )
	{
		currentX += mapOffsetX;
		m_xCoordinates.push_back( currentX );
	}

	double mapOffsetY = fmod( ( double )m_mapOffset.y, tileSize );
	if ( mapOffsetY > 0 )
	{
		currentY += mapOffsetY;
		m_yCoordinates.push_back( currentY );
	}

	uint numberOfMiddleTiles;
	
	// Calculate the number of MIDDLE divisions along each axis after the offset
	numberOfMiddleTiles = floor( abs( ( m_mapOpenGLBoundaries.Width() - mapOffsetX ) / tileSize ) );
	for ( uint col = 0; col < numberOfMiddleTiles; col++ )
	{
		currentX += tileSize;
		m_xCoordinates.push_back( currentX );
	}

	numberOfMiddleTiles = floor( abs( ( m_mapOpenGLBoundaries.Height() - mapOffsetY ) / tileSize ) );
	for ( uint row = 0; row < numberOfMiddleTiles; row++ )
	{
		currentY += tileSize;
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

	InitTiles();
	FillTiles();
}

void LocationGrid::Render()
{
	if( !IsVisible() )
		return;

	// Colour palette for tiles (temporary)
	float alphaOfTile = m_uniformColourOfTiles.GetAlpha();
	const GLfloat COLOUR[][4] = {{0, 0, 0, alphaOfTile}, {0, 0, 1, alphaOfTile},
	     {0, 1, 0, alphaOfTile}, {0, 1, 1, alphaOfTile}, {1, 0, 0, alphaOfTile},
         {1, 0, 1, alphaOfTile}, {1, 1, 0, alphaOfTile}, {1, 1, 1, alphaOfTile}};

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
			uint iterator = 0;
			glBegin( GL_QUADS );

			std::list<double>::iterator row1, row2, col1, col2;
			row2 = m_yCoordinates.begin(); row2++;

			for ( row1 = m_yCoordinates.begin(); row2 != m_yCoordinates.end(); ++row1 )
			{
				col2 = m_xCoordinates.begin(); col2++;

				for ( col1 = m_xCoordinates.begin(); col2 != m_xCoordinates.end(); ++col1 )
				{
					glColor4fv( COLOUR[iterator] );
					( iterator < 7 ) ? iterator++ : iterator = 0;

					glVertex3f( *col1, m_elevationUsed, *row1 );
					glVertex3f( *col2, m_elevationUsed, *row1 );
					glVertex3f( *col2, m_elevationUsed, *row2 );
					glVertex3f( *col1, m_elevationUsed, *row2 );

					col2++;
				}
				row2++;
			}
			glEnd();
		}
	}
}

void LocationGrid::InitTiles()
{
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
}

void LocationGrid::FillTiles()
{
	std::vector<LocationLayerPtr> locationLayers = m_locationSetLayer->GetAllLocationLayers();
	for( uint i = 0; i < locationLayers.size() ; i++)
	{
		for ( uint j = 0; j < m_tileModels.size(); j++)
		{
			std::pair<float,float> top = m_tileModels[j]->GetTopLeft();
			std::pair<float,float> bottom = m_tileModels[j]->GetBottomRight();
			std::map<std::wstring,std::wstring> data = locationLayers[i]->GetLocationController()->GetData();
			float easting = StringTools::ToDouble(data[StringTools::ToStringW("Longitude")]);
			float northing = StringTools::ToDouble(data[StringTools::ToStringW("Latitude")]);

			if ( top.first <= easting
				&& top.second >= northing
				&& bottom.first > easting
				&& bottom.second < northing )
			{
				m_tileModels[j]->AddLocationLayer(locationLayers[i]);
			}
		}
	}
}