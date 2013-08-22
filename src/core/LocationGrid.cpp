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
#include "../core/LocationGrid.hpp"

#include "../utils/ErrorGL.hpp"

using namespace GenGIS;

LocationGrid::LocationGrid() :
	m_divisions( 15 ),
	m_elevation( 0.05f ),

	m_showTiles( true ),
	m_uniformColourOfTiles( 0.0f, 0.0f, 0.0f, 0.3f ),

	m_showBorders( true ),
	m_colourOfBorders( 0.0f, 0.0f, 0.0f, 0.3f ),
	m_thicknessOfBorders( 1 ),
	m_styleOfBorders( VisualLine::SOLID )
{
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

	const float tileSize = 2.0f / m_divisions;

	// Colour palette (temporary)
	float alphaOfTile = m_uniformColourOfTiles.GetAlpha();
	const GLfloat COLOUR[][4] = {{0, 0, 0, alphaOfTile}, {0, 0, 1, alphaOfTile},
	     {0, 1, 0, alphaOfTile}, {0, 1, 1, alphaOfTile}, {1, 0, 0, alphaOfTile},
         {1, 0, 1, alphaOfTile}, {1, 1, 0, alphaOfTile}, {1, 1, 1, alphaOfTile}};

	error::ErrorGL::Check();
	glDisable( GL_LIGHTING );

	// Render borders
	if ( m_showBorders )
	{
		double position = -1;

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
		for ( uint col = 0; col <= m_divisions; col++ )
		{
			glVertex3f( position, m_elevation, -1);
			glVertex3f( position, m_elevation,  1);
			glVertex3f( -1, m_elevation, position );
			glVertex3f(  1, m_elevation, position );

			position += tileSize;
		}
		glEnd();
	}

	// Render tiles
	if ( m_showTiles )
	{
		uint   iterator = 0;
		double x = -1; double z;
		glBegin( GL_QUADS );
		for ( uint col = 0; col < m_divisions; col++ )
		{
			z = -1;
			for ( uint row = 0; row < m_divisions; row++ )
			{
				glColor4fv( COLOUR[iterator] );
				( iterator < 7 ) ? iterator++ : iterator = 0;

				glVertex3f( x,            m_elevation, z            );
				glVertex3f( x + tileSize, m_elevation, z            );
				glVertex3f( x + tileSize, m_elevation, z + tileSize );
				glVertex3f( x,            m_elevation, z + tileSize );

				z += tileSize;
			}
			x += tileSize;
		}
		glEnd();
	}
}
