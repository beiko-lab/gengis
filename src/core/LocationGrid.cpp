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

#include "../utils/ErrorGL.hpp"

using namespace GenGIS;

LocationGrid::LocationGrid() :
	// General
	m_divideTilesAlong( LONGITUDE ),
	m_gridAlignmentStyle( ORIGIN ),
	m_mapOpenGLBoundaries( -1.0, -1.0, 1.0, 1.0 ),
	m_mapOffset( 0.0f, 0.0f ),
	m_divisions( 15 ),
	m_autoAdjustElevation( true ),
	m_elevation( 0.05f ),

	// Tiles
	m_showTiles( true ),
	m_tileFillMode( NONE ),
	m_uniformColourOfTiles( 0.0f, 0.5f, 0.0f, 0.3f ),

	// Border
	m_showBorders( true ),
	m_colourOfBorders( 0.0f, 0.0f, 0.0f, 0.3f ),
	m_thicknessOfBorders( 1 ),
	m_styleOfBorders( VisualLine::SOLID )
{
	// Property of 'Layer' class
	m_bVisible = false;
}

LocationGrid::~LocationGrid()
{
}

bool LocationGrid::IsVisible() 
{ 
	return m_bVisible; 
}

void LocationGrid::Render()
{
	if( !IsVisible() )
		return;

	// Real terrain values of DEM map
	//mapController->GetMapBorders();
	//mapController->GetProjectionExtents();

	MapControllerPtr mapController = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController();
	m_mapOpenGLBoundaries.x  = -( mapController->GetWidth() / 2 );
	m_mapOpenGLBoundaries.dx =  ( mapController->GetWidth() / 2 );
	m_mapOpenGLBoundaries.y  = -( mapController->GetHeight() / 2 );
	m_mapOpenGLBoundaries.dy =  ( mapController->GetHeight() / 2 );

	// Set the grid elevation
	float elevation;
	if ( m_autoAdjustElevation )
	{
		elevation  = mapController->GetMaxElevationGridSpace()
		           * mapController->GetVerticalExaggeration()
		           + 0.005; // Add a minimal amount to avoid colour
		                    // clashing between grid and map
	}
	else
	{
		elevation = m_elevation;
	}

	// Determine appropriate tile size
	double tileSize;
	if ( m_divideTilesAlong == LATITUDE )
		tileSize = m_mapOpenGLBoundaries.Height() / m_divisions;
	else if ( m_divideTilesAlong == LONGITUDE )
		tileSize = m_mapOpenGLBoundaries.Width() / m_divisions;
	else // there will be other cases, this is just a safe 'else' for now
		tileSize = 2.0 / m_divisions;

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
		m_mapOffset.x = fmod( 0.2, tileSize );
		m_mapOffset.y = fmod( 0.3, tileSize );

		double xCurrentPosition = m_mapOpenGLBoundaries.x + m_mapOffset.x;
		double zCurrentPosition = m_mapOpenGLBoundaries.y + m_mapOffset.y;

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

		// Render a first (left) column line if there is an offset
		if ( m_mapOffset.x > 0)
		{
			glVertex3f( m_mapOpenGLBoundaries.x,  elevation, m_mapOpenGLBoundaries.y );
			glVertex3f( m_mapOpenGLBoundaries.dx, elevation, m_mapOpenGLBoundaries.y );
		}

		// Render a first (top) row line if there is an offset
		if ( m_mapOffset.y > 0)
		{
			glVertex3f( m_mapOpenGLBoundaries.x, elevation, m_mapOpenGLBoundaries.y  );
			glVertex3f( m_mapOpenGLBoundaries.x, elevation, m_mapOpenGLBoundaries.dy );
		}

		// Render column lines
		for ( uint col = 0; col < m_divisions; col++ )
		{
			if ( xCurrentPosition > m_mapOpenGLBoundaries.dx )
				break;
			glVertex3f( xCurrentPosition, elevation, m_mapOpenGLBoundaries.y  );
			glVertex3f( xCurrentPosition, elevation, m_mapOpenGLBoundaries.dy );
			xCurrentPosition += tileSize;
		}

		// Render row lines
		for ( uint row = 0; row < m_divisions; row++ )
		{
			if ( zCurrentPosition > m_mapOpenGLBoundaries.dy )
				break;
			glVertex3f( m_mapOpenGLBoundaries.x,  elevation, zCurrentPosition );
			glVertex3f( m_mapOpenGLBoundaries.dx, elevation, zCurrentPosition );
			zCurrentPosition += tileSize;
		}

		// Render last (right) column line
		glVertex3f( m_mapOpenGLBoundaries.x,  elevation, m_mapOpenGLBoundaries.dy );
		glVertex3f( m_mapOpenGLBoundaries.dx, elevation, m_mapOpenGLBoundaries.dy );

		// Render last (bottom) row line
		glVertex3f( m_mapOpenGLBoundaries.dx, elevation, m_mapOpenGLBoundaries.y  );
		glVertex3f( m_mapOpenGLBoundaries.dx, elevation, m_mapOpenGLBoundaries.dy );

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

			glVertex3f( m_mapOpenGLBoundaries.x,  elevation, m_mapOpenGLBoundaries.y  );
			glVertex3f( m_mapOpenGLBoundaries.x,  elevation, m_mapOpenGLBoundaries.dy );
			glVertex3f( m_mapOpenGLBoundaries.dx, elevation, m_mapOpenGLBoundaries.dy );
			glVertex3f( m_mapOpenGLBoundaries.dx, elevation, m_mapOpenGLBoundaries.y  );

			glEnd();
		}
		else if ( m_tileFillMode == MAPPED )
		{
			uint   iterator = 0;
			double xCurrentPosition = m_mapOpenGLBoundaries.x + m_mapOffset.x;
			double zCurrentPosition = m_mapOpenGLBoundaries.y + m_mapOffset.y;

			glBegin( GL_QUADS );
			for ( uint col = 0; col < m_divisions; col++ )
			{
				for ( uint row = 0; row < m_divisions; row++ )
				{
					glColor4fv( COLOUR[iterator] );
					( iterator < 7 ) ? iterator++ : iterator = 0;

					glVertex3f( xCurrentPosition,            elevation, zCurrentPosition            );
					glVertex3f( xCurrentPosition + tileSize, elevation, zCurrentPosition            );
					glVertex3f( xCurrentPosition + tileSize, elevation, zCurrentPosition + tileSize );
					glVertex3f( xCurrentPosition,            elevation, zCurrentPosition + tileSize );

					zCurrentPosition += tileSize;
				}
				zCurrentPosition  = m_mapOpenGLBoundaries.y + m_mapOffset.y;
				xCurrentPosition += tileSize;
			}
			glEnd();
		}
	}
}
