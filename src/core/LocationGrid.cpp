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

#include "../core/LocationGrid.hpp"

#include "../utils/ErrorGL.hpp"

using namespace GenGIS;

LocationGrid::LocationGrid()
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

	m_divisions = 5;
	m_elevation = 0.05;
	m_alpha     = 0.3;

	const double tileSize  = 2.0 / m_divisions;

	// Colour palette (temporary)
	uint   iterator = 0;
	const  GLfloat COLOUR[][4] = {{0, 0, 0, m_alpha}, {0, 0, 1, m_alpha},
	          {0, 1, 0, m_alpha}, {0, 1, 1, m_alpha}, {1, 0, 0, m_alpha},
              {1, 0, 1, m_alpha}, {1, 1, 0, m_alpha}, {1, 1, 1, m_alpha}};

	error::ErrorGL::Check();
	glDisable( GL_LIGHTING );
	glBegin( GL_QUADS );

	double x = -1; double z;
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
