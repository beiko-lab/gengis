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

	error::ErrorGL::Check();

	glDisable( GL_LIGHTING );

	GLfloat RED[4]     = {1, 0, 0, 0.5};
	GLfloat GREEN[4]   = {0, 1, 0, 0.5};
	GLfloat BLUE[4]    = {0, 0, 1, 0.5};
	GLfloat WHITE[4]   = {1, 1, 1, 0.5};
	GLfloat BLACK[4]   = {0, 0, 0, 0.5};
	GLfloat YELLOW[4]  = {1, 1, 0, 0.5};
	GLfloat CYAN[4]    = {0, 1, 1, 0.5};
	GLfloat MAGENTA[4] = {1, 0, 1, 0.5};

	glBegin( GL_QUADS );
	//glBegin(GL_LINE_LOOP);

		glColor4fv( GREEN );
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 1.0);

		glColor4fv( YELLOW );
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, -1.0);
		glVertex3f(0.0, 0.0, -1.0);

		glColor4fv( RED );
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);
		glVertex3f(-1.0, 0.0, 1.0);
		glVertex3f(-1.0, 0.0, 0.0);

		glColor4fv( BLUE );
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(-1.0, 0.0, 0.0);
		glVertex3f(-1.0, 0.0, -1.0);
		glVertex3f(0.0, 0.0, -1.0);
	glEnd();
}
