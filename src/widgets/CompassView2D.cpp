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

#include "../core/Precompiled.hpp"

#include "../widgets/CompassView2D.hpp"

#include "../core/App.hpp"
#include "../core/Camera.hpp"
#include "../widgets/Texture.hpp"
#include "../core/OrthoLayers.hpp"

#include "../core/Viewport.hpp"

using namespace GenGIS;

CompassView2D::CompassView2D(float theta) : ViewWidget() 
{
	m_theta = theta;
}

void CompassView2D::Render() 
{
	if(m_bVisible && !App::Inst().IsSavingHiResImage())
	{
		glPushMatrix();

		glTranslated( m_screenCoordinates.x, m_screenCoordinates.y, -COMPASS_LAYER);
		glScaled( m_scale.x, m_scale.y, m_scale.z );

		glDisable( GL_LIGHTING );

		if ( m_textured ) 
		{	    
			glRotated( m_theta, 0, 0, 1 );                
			glTranslated( -m_pivot.x, -m_pivot.y, -1 );

			glBindTexture(GL_TEXTURE_2D, m_texture->GetName());
			glEnable(GL_TEXTURE_2D);

			glColor4f( 1, 1, 1, 1 );

			glBegin(GL_POLYGON);
			glTexCoord2f(0, 1);
			glVertex2f(-1, -1);

			glTexCoord2f(1, 1);
			glVertex2f(+1, -1);

			glTexCoord2f(1, 0);
			glVertex2f(+1, +1);

			glTexCoord2f(0, 0);
			glVertex2f(-1, +1);
			glEnd();

			glDisable(GL_TEXTURE_2D);           
		}
		else 
		{
			DrawNorthNeedle();
			DrawTickMarks();
		}

		glEnable( GL_LIGHTING );

		glPopMatrix();
	}
}

void CompassView2D::SetAngle( float theta ) 
{
	m_theta = theta;

	if ( m_theta >= 360 ) {
		m_theta -= 360;
	}
	else if ( m_theta < 0 ) {
		m_theta += 360;
	}
}

void CompassView2D::DrawNorthNeedle() 
{
	glLineWidth( 2 );

	// north hand
	glPushMatrix(); 
	glColor4f( 0.5f, 0.2f, 0.2f, 1.0f );

	glBegin( GL_LINES );
	glVertex2f( 0, 0 );
	glVertex2f( 0, 1 );
	glVertex2f( -0.1f, 0.8f );
	glVertex2f( 0, 1 );
	glVertex2f( 0.1f, 0.8f );
	glVertex2f( 0, 1 );
	glEnd();

	glPopMatrix();
	glLineWidth( 2 );
}

void CompassView2D::DrawTickMarks() 
{
	glPushMatrix();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glRotated( 90 + m_theta, 0, 0, WIDGETS_LAYER );
	glColor3f( 1.0f, 0.0f, 0.0f);

	// tick marks
	for ( int i = 0; i < 60; i++ ) {

		if ( i == 0 ) {
			glColor4f( 1.0f, 0.0f, 0.0f, 0.95f );
			glPointSize( 8 );// north is a little larger            

		} else if ( i % 15 == 0 ) {
			glPointSize( 4 );// minute marks

		} else {
			glPointSize( 1 );// hour marks
		}

		// draw the ticks in the compass
		glPushMatrix();
		glRotated( 6 * ( float ) i, 0, 0, 1 );
		glTranslated( 1, 0, 0 );

		glBegin( GL_POINTS );
		glVertex2f( 0, 0 );
		glEnd();

		if ( i == 0 ) {
			glColor4f( 0.5f, 0.2f, 0.2f, 0.95f );
		}

		glPopMatrix();
	}

	glPopMatrix();
}

float CompassView2D::AngleOnCompass(const Point2D& mousePt) 
{
	Box2D box = GetBoundingBox();
	Point2D origin = GetPivotPoint();

	// pivot coords of the compass
	float oX = box.x + (origin.x * box.dx + box.dx) * 0.5f;
	float oY = box.y + (origin.y * box.dy + box.dy) * 0.5f;

	// distance of passed params from origin of bounding box
	float du = mousePt.x - oX;
	float dv = mousePt.y - oY;

	// angle from center of compass to passed coordinates (u, v)
	if(du == 0 && dv > 0) 
	{
		return 0;
	}

	if (du == 0 && dv < 0) 
	{
		return 180;
	}

	float angle = atan( dv / du ) * RAD_TO_DEG;

	// adjust for correct quadrant
	if(du < 0) 
	{
		angle += 180;
	}

	// adjust for fact the 0 degrees is due north on a compass (not due east)
	angle -= 90;

	// bound angle between [0, 360]
	if ( angle < 0 ) 
	{
		angle += 360;
	}
	else if ( angle >= 360 ) 
	{
		angle -= 360;
	}

	// set the angle of the compass
	SetAngle(angle);

	return angle;
}

bool CompassView2D::MouseLeftDown(const Point2D& mousePt)
{    
	if(!IsVisible()) 
		return false;

	Box2D box = GetBoundingBox();
	Point2D origin = GetPivotPoint();
	Point2D radii = GetRadii();

	// pivot points of the compass
	float oX = box.x + (origin.x * box.dx + box.dx) * 0.5f;
	float oY = box.y + (origin.y * box.dy + box.dy) * 0.5f;

	// size of compass
	float radX = box.dx * 0.5f * radii.x;
	float radY = box.dy * 0.5f * radii.y;

	// distance of passed params from origin of compass
	float du = fabs( mousePt.x - oX );
	float dv = fabs( mousePt.y - oY );

	float distance = du * du + dv * dv;	// Note: no need to take sqrt() since
	// it is a monotonic function

	// approximate compass as perfect circle (very reasonable appoximation)
	float rad = radX;
	if(radY > radX)
		rad = radY;

	// not that the radius must be squared since we didn't take the square root of the distance
	if(distance < rad*rad)
	{
		// set camera and compass angle
		App::Inst().GetViewport()->GetCamera()->SetYaw(AngleOnCompass(mousePt));
		return true;
	}

	return false;
}
