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

#include "../core/Camera.hpp"

using namespace GenGIS;

Camera::Camera(float aspect, float fov, float viewingNear, float viewingFar) 
{
	m_aspect = aspect;
	m_fov = fov;
	m_viewingNear = viewingNear;
	m_viewingFar = viewingFar;

	m_pitch = 90;
	m_yaw = 0;

	m_translation = Point3D(0,0,0);

	ApplyIntrinsicParams();
	ApplyExtrinsicParams();
}

template<class Archive>
void Camera::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_fov;            // float
	ar & m_viewingNear;    // float
	ar & m_viewingFar;     // float
	ar & m_translation;    // Point3D
	ar & m_pitch;          // float
	ar & m_yaw;            // float
	ar & m_minX;           // float
	ar & m_maxX;           // float
	ar & m_minY;           // float
	ar & m_maxY;           // float
	ar & m_width;          // float
	ar & m_height;         // float
	ar & m_viewportWidth;  // unsigned int
	ar & m_viewportHeight; // unsigned int
}
template void Camera::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void Camera::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void Camera::ApplyIntrinsicParams() 
{
	// specify which matrix is the current matrix (projection matrix stack)
	glMatrixMode( GL_PROJECTION );
	// replace the current matrix with the identity matrix
	glLoadIdentity();
	// set up a perspective projection matrix
	gluPerspective(m_fov, m_aspect, m_viewingNear, m_viewingFar);
}

void Camera::SetAspectRatio(unsigned int width, unsigned int height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;
	m_aspect = float(m_viewportWidth) / m_viewportHeight;
	ApplyIntrinsicParams();
}

void Camera::ApplyExtrinsicParams()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(-m_translation.x, -m_translation.y, -m_translation.z);
	glRotated(m_pitch, 1, 0, 0);
	glRotated(m_yaw, 0, 1, 0);
}

void Camera::SetPosition(float x, float y, float z)
{
	m_translation.x = x;
	m_translation.y = y;
	m_translation.z = z;
}

void Camera::SetHeight(float z)
{
	m_translation.z = z;
}

void Camera::SetModelSpaceExtents(float minX, float maxX, float minY, float maxY) 
{ 
	m_width = maxX - minX;
	m_height = maxY - minY;

	float max = m_width;
	if(m_height > m_width)
		max = m_height;

	m_minX = minX; 
	m_maxX = maxX; 
	m_minY = minY; 
	m_maxY = maxY; 

	ApplyIntrinsicParams();
}

void Camera::ZoomDelta(float fraction)
{
	//const float MIN_ZOOM = 0.25;
	const float MIN_ZOOM = 0.03f;
	//const float MAX_ZOOM = 10.0;
	const float MAX_ZOOM = 20.0f;

	m_translation.z += fraction	* m_translation.z;

	// keep zoom within a reasonable range
	if(m_translation.z < MIN_ZOOM)
		m_translation.z = MIN_ZOOM;

	if(m_translation.z > MAX_ZOOM)
		m_translation.z = MAX_ZOOM;
}

void Camera::TranslateDelta(float dx, float dy)
{
	m_translation.x -= dx * m_translation.z;
	m_translation.y += dy * m_translation.z;
}

void Camera::PitchDelta(float angle)
{
	m_pitch += angle;

	// limit range of camera
	if(m_pitch > 90) m_pitch = 90;
	if(m_pitch < 0) m_pitch = 0;
}

void Camera::YawDelta(float angle)
{
	m_yaw += angle;

	// keep y angle between 0 and 360 degrees
	if(m_yaw > 360) m_yaw = m_yaw - 360;
	if(m_yaw < 0) m_yaw = 360 - m_yaw;
}

void Camera::ResetView()
{
	const float DEFAULT_VIEW_Z_MULTIPLER = 1.75;
	const float DEFALUT_PITCH = 30.0;
	const float DEFAULT_ANGLE_Y = 30.0;

	float max = m_width;
	if(m_height > m_width)
		max = m_height;

	m_translation = Point3D(0, 0, max*DEFAULT_VIEW_Z_MULTIPLER);
	m_pitch = DEFALUT_PITCH;
	m_yaw = DEFAULT_ANGLE_Y;
}

void Camera::TopView()
{
	m_translation = Point3D(0, 0, GetMapExtentsZoom());
	m_pitch = 90;
	m_yaw = 0;
}

float Camera::GetMapExtentsZoom()
{
	// Note from Donovan Parks: This function is suppose to calculate the exact
	// z-position of the camera required so the map will just fit within the viewport. 
	// If you understand how to do this, please fix this code and comment it.

	float mapAspect = m_width / m_height;
	float z = (0.5f*m_height) / atan(0.5f*m_fov*DEG_TO_RAD);

	return z;
}

void Camera::SetOrthoCamera()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, GetWindowWidth(), 0.0, GetWindowHeight(), 1, 1000 );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
}

void Camera::UnsetOrthoCamera()
{
	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
}
