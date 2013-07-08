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

#include "../core/VisualMarker.hpp"
#include "../core/OrthoLayers.hpp"
#include "../core/App.hpp"
#include "../core/MapController.hpp"
#include "../core/Viewport.hpp"
#include "../core/Camera.hpp"

#include "../utils/ErrorGL.hpp"
#include "../utils/Log.hpp"
#include "../utils/glUtils.hpp"
#include "../utils/StringTools.hpp"

using namespace GenGIS;

VisualMarker::VisualMarker()
	: VisualObject(Colour(0.8f, 0.8f, 0.8f), 1, false), m_position(Point3D(0,0,0)), m_shape(CIRCLE), 
		m_bRenderingStyle(DECAL), m_renderingOrder(0), m_depth(1), m_borderSize(0), m_borderColour(Colour(0,0,0))
{

}

VisualMarker::VisualMarker(Colour colour, float size, MARKER_SHAPE shape, const Point3D& position)
		: VisualObject(colour, size, true), m_position(position), m_shape(shape), 
			m_bRenderingStyle(DECAL), m_renderingOrder(0), m_depth(1), m_borderSize(0), m_borderColour(Colour(0,0,0))
{

}

template<class Archive>
void VisualMarker::serialize(Archive & ar, const unsigned int file_version)
{
	ar & boost::serialization::base_object<VisualObject>(*this);
	ar & m_position;        // Point3D
	ar & m_shape;           // MARKER_SHAPE
	ar & m_rotation;        // float
	ar & m_bRenderingStyle; // MARKER_RENDERING_STYLE
	ar & m_depth;           // float
	ar & m_renderingOrder;  // float
	ar & m_borderSize;      // float
	ar & m_borderColour;    // Colour
}
template void VisualMarker::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void VisualMarker::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void VisualMarker::Render()
{
	if(!m_bVisible)
		return;

	if(m_bSelected)
	{
		if(m_bRenderingStyle == DECAL)
		{
			RenderDecal(m_position, App::Inst().GetSelectionColour(), m_size + App::Inst().GetSelectionThickness(), 
												m_shape, m_borderColour, 0, true, m_renderingOrder);
		}
		else if(m_bRenderingStyle == ALWAYS_VISIBLE)
		{	
			RenderAlwaysVisible(m_position, App::Inst().GetSelectionColour(), m_size + App::Inst().GetSelectionThickness(), 
														m_shape, m_rotation, m_borderColour, 0, m_depth+1);
		}
		else if(m_bRenderingStyle == MARKER_3D)
		{	
			Render3D(m_position, App::Inst().GetSelectionColour(), m_size + App::Inst().GetSelectionThickness(), 
														m_shape, m_rotation, true);
		}
		else if(m_bRenderingStyle == SCREEN_POS)
		{	
			RenderAtScreenPosition(m_position, App::Inst().GetSelectionColour(), m_size + App::Inst().GetSelectionThickness(), 
																	m_shape, m_depth+1);
		}
	}

	if(m_bRenderingStyle == DECAL)
	{
		RenderDecal(m_position, m_colour, m_size,  m_shape, m_borderColour, m_borderSize, false, m_renderingOrder);
	}
	else if(m_bRenderingStyle == ALWAYS_VISIBLE)
	{
		RenderAlwaysVisible(m_position, m_colour, m_size, m_shape, m_rotation, m_borderColour, m_borderSize, m_depth);
	}
	else if(m_bRenderingStyle == MARKER_3D)
	{	
		Render3D(m_position, m_colour, m_size, m_shape, m_rotation, false);
	}
	else if(m_bRenderingStyle == SCREEN_POS)
	{
		RenderAtScreenPosition(m_position, m_colour, m_size, m_shape, m_depth);
	}
}

void VisualMarker::RenderAlwaysVisibleDecal(const Point3D& position, const Colour& colour, float size, MARKER_SHAPE shape, bool bSelection, int renderingOffset)
{
	error::ErrorGL::Check();

	// draw location in back section of depth buffer so they can appear occluded due to the terrain
	glDepthRange(0.0, TERRAIN_START_DEPTH);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();	
		
		glTranslated(position.x, position.y, position.z);
		glRotated(90, 1.0, 0.0, 0.0);	
		glRotated(App::Inst().GetViewport()->GetCamera()->GetYaw(), 0.0, 0.0, 1.0);

		if(bSelection)
			glPolygonOffset(1,renderingOffset+10);			// these are rather arbitrarily set magic numbers
		else
			glPolygonOffset(1,renderingOffset);	

		RenderMarker(GetMarkerSize(position, size), colour, shape);

	glPopMatrix();

	glDisable(GL_POLYGON_OFFSET_FILL);

	error::ErrorGL::Check();
}

void VisualMarker::RenderDecal(const Point3D& position, const Colour& colour, float size, MARKER_SHAPE shape, 
																		const Colour& borderColour, float borderSize, bool bSelection, float renderingOrder)
{
	error::ErrorGL::Check();

	// draw location in back section of depth buffer so they can appear occluded due to the terrain
	glDepthRange(TERRAIN_START_DEPTH, 1.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();	
		
		glTranslated(position.x, position.y, position.z);
		glRotated(90, 1.0, 0.0, 0.0);	
		glRotated(App::Inst().GetViewport()->GetCamera()->GetYaw(), 0.0, 0.0, 1.0);

		if(borderSize != 0.0f)
		{
			glPolygonOffset(-1, -100+renderingOrder);
			RenderMarker(GetMarkerSize(position, size + borderSize), borderColour, shape);
		}

		if(bSelection)
			glPolygonOffset(-1,-290+renderingOrder);			// these are rather arbitrarily set magic numbers
		else
			glPolygonOffset(-1,-300+renderingOrder);	

		RenderMarker(GetMarkerSize(position, size), colour, shape);

	glPopMatrix();
	
	glDisable(GL_POLYGON_OFFSET_FILL);

	error::ErrorGL::Check();
}

void VisualMarker::RenderAlwaysVisible(const Point3D& position, const Colour& colour, float size, MARKER_SHAPE shape, 
																float rotation, const Colour& borderColour, float borderSize, float depth)
{
	error::ErrorGL::Check();

	glDepthRange(0.0f, TERRAIN_START_DEPTH);

	App::Inst().GetViewport()->GetCamera()->SetOrthoCamera();

	glPushMatrix();	
		Point3D screenPt = App::Inst().GetMapController()->ProjectToScreen(position);

		glTranslated(screenPt.x, screenPt.y, -depth);
		glRotated(90-App::Inst().GetViewport()->GetCamera()->GetPitch() + rotation, 1.0, 0.0, 0.0);	
		glRotated(App::Inst().GetViewport()->GetCamera()->GetYaw(), 0.0, 0.0, 1.0);
	
		RenderMarker(size, colour, shape);

		if(borderSize != 0.0f)
		{
			glTranslated(0, 0, -1);
			RenderMarker(size + borderSize, borderColour, shape);
		}		

	glPopMatrix();

	App::Inst().GetViewport()->GetCamera()->UnsetOrthoCamera();

	error::ErrorGL::Check();
}

void VisualMarker::Render3D(const Point3D& position, const Colour& colour, float size, MARKER_SHAPE shape, 
																float rotation, bool bSelection)
{
	error::ErrorGL::Check();

	// draw location in back section of depth buffer so they can appear occluded due to the terrain
	glDepthRange(TERRAIN_START_DEPTH, 1.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);

	if(bSelection)
		glPolygonOffset(-1,-290);			// these are rather arbitrarily set magic numbers
	else
		glPolygonOffset(-1,-300);	

	float adjSize = GetMarkerSize(position, size);

	glPushMatrix();	
		
		glTranslated(position.x, position.y, position.z);
		glRotated(-App::Inst().GetViewport()->GetCamera()->GetYaw(), 0.0, 1.0, 0.0);
		glRotated(-App::Inst().GetViewport()->GetCamera()->GetPitch(), 1.0, 0.0, 0.0);	

		RenderMarker(adjSize, colour, shape);

	glPopMatrix();
	
	glDisable(GL_POLYGON_OFFSET_FILL);

	error::ErrorGL::Check();
}

void VisualMarker::RenderAtScreenPosition(const Point3D& position, const Colour& colour, float size, MARKER_SHAPE shape, 
																float depth)
{
	error::ErrorGL::Check();

	glDepthRange(0.0f, TERRAIN_START_DEPTH);

	App::Inst().GetViewport()->GetCamera()->SetOrthoCamera();

	glPushMatrix();	
		glTranslated(floor(position.x+0.5f), floor(position.y+0.5f), -depth);
		//glRotated(90, 1.0, 0.0, 0.0);	
	
		RenderMarker(size, colour, shape);

	glPopMatrix();

	App::Inst().GetViewport()->GetCamera()->UnsetOrthoCamera();

	error::ErrorGL::Check();
}

void VisualMarker::RenderMarker(float size, const Colour& colour, MARKER_SHAPE shape)
{
	glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());

	if(shape == CIRCLE)
	{
		// Render cicles as dodecagons to improve performance
		glBegin( GL_TRIANGLE_FAN );
			glVertex3f( size, 0.0f, 0.0f );
			glVertex3f( size*0.87f, size*0.5f, 0.0f );
			glVertex3f( size*0.5f, size*0.87f, 0.0f );
			glVertex3f( 0.0f, size, 0.0f );
			glVertex3f( size*-0.5f, size*0.87f, 0.0f );
			glVertex3f( size*-0.87f, size*0.5f, 0.0f );
			glVertex3f( size*-1.0f, 0.0f, 0.0f );
			glVertex3f( size*-0.87f, size*-0.5f, 0.0f );
			glVertex3f( size*-0.5f, size*-0.87f, 0.0f );
			glVertex3f( 0.0f, size*-1.0f, 0.0f );
			glVertex3f( size*0.5f, size*-0.87f, 0.0f );
			glVertex3f( size*0.87f, size*-0.5, 0.0f );
			glVertex3f( size, 0.0f, 0.0f );
		glEnd();

	//	GLUquadricObj*  quadric = gluNewQuadric();
	//	gluQuadricDrawStyle( quadric, GLU_FILL );
	//	gluQuadricNormals( quadric, GLU_SMOOTH );

	//	if(quadric != NULL)
	//	{
	//		gluDisk(quadric, 0, size, 120, 1);
	//	}
	//	else
	//		Log::Inst().Error("(Error) VisualShape::DrawShape(): m_quadric not allocated.");

	//	gluDeleteQuadric( quadric );
	}
	else if(shape == SQUARE)
	{
		glBegin(GL_QUADS);
			glVertex3f( -size, -size, 0.0f );
			glVertex3f( size, -size, 0.0f );
			glVertex3f( size, size, 0.0f );
			glVertex3f( -size, size, 0.0f );
		glEnd();
	}
	else if(shape == TRIANGLE)
	{		
		glBegin(GL_TRIANGLES);
			glVertex3f( 0.0f, -size, 0.0f );
			glVertex3f( -size, size, 0.0f );
			glVertex3f( size, size, 0.0f );
		glEnd();
	}
	else if(shape == STAR)
	{
		glBegin(GL_QUADS);
			glVertex3f( -size/5, -size, 0.0f );
			glVertex3f( size/5, -size, 0.0f );
			glVertex3f( size/5, size, 0.0f );
			glVertex3f( -size/5, size, 0.0f );
		glEnd();

		glBegin(GL_QUADS);
			glVertex3f( -size, -size/5, 0.0f );
			glVertex3f( size, -size/5, 0.0f );
			glVertex3f( size, size/5, 0.0f );
			glVertex3f( -size, size/5, 0.0f );
		glEnd();

		float threeForths = 3.0f / 4.0f;
		glBegin(GL_QUADS);
			glVertex3f( -size, -threeForths*size, 0.0f );
			glVertex3f( -threeForths*size, -size, 0.0f );
			glVertex3f( size, threeForths*size, 0.0f );
			glVertex3f( threeForths*size, size, 0.0f );
		glEnd();

		glBegin(GL_QUADS);
			glVertex3f( threeForths*size, -size, 0.0f );
			glVertex3f( size, -threeForths*size, 0.0f );
			glVertex3f( -threeForths*size, size, 0.0f );
			glVertex3f( -size, threeForths*size, 0.0f );
		glEnd();
	}
	else if(shape == PLUS_SIGN)
	{
		glBegin(GL_QUADS);
			glVertex3f( -size/3, -size, 0.0f );
			glVertex3f( size/3, -size, 0.0f );
			glVertex3f( size/3, size, 0.0f );
			glVertex3f( -size/3, size, 0.0f );
		glEnd();

		glBegin(GL_QUADS);
			glVertex3f( -size, -size/3, 0.0f );
			glVertex3f( size, -size/3, 0.0f );
			glVertex3f( size, size/3, 0.0f );
			glVertex3f( -size, size/3, 0.0f );
		glEnd();
	}
	else if(shape == OCTAGON)
	{
		glBegin(GL_QUADS);
			glVertex3f( -size/2, -size, 0.0f );
			glVertex3f( size/2, -size, 0.0f );
			glVertex3f( size/2, size, 0.0f );
			glVertex3f( -size/2, size, 0.0f );
		glEnd();

		glBegin(GL_QUADS);
			glVertex3f( -size, -size/2, 0.0f );
			glVertex3f( size, -size/2, 0.0f );
			glVertex3f( size, size/2, 0.0f );
			glVertex3f( -size, size/2, 0.0f );
		glEnd();

		glBegin(GL_QUADS);
			glVertex3f( -size, size/2, 0.0f );
			glVertex3f( -size/2, size, 0.0f );
			glVertex3f( size, -size/2, 0.0f );
			glVertex3f( size/2, -size, 0.0f );
		glEnd();

		glBegin(GL_QUADS);
			glVertex3f( size/2, size, 0.0f );
			glVertex3f( size, size/2, 0.0f );
			glVertex3f( -size/2, -size, 0.0f );
			glVertex3f( -size, -size/2, 0.0f );
		glEnd();
	}
	else if(shape == INVERTED_TRIANGLE)
	{
		glBegin(GL_TRIANGLES);
			glVertex3f( 0.0f, size, 0.0f );
			glVertex3f( -size, -size, 0.0f );
			glVertex3f( size, -size, 0.0f );
		glEnd();
	}
	else if(shape == DIAMOND)
	{
		glBegin(GL_QUADS);
			glVertex3f( 0.0f, size, 0.0f );
			glVertex3f( size, 0.0f, 0.0f );
			glVertex3f( 0.0f, -size, 0.0f );
			glVertex3f( -size, 0.0f, 0.0f );
		glEnd();
	}
}


float VisualMarker::GetMarkerSize(const Point3D& position, float size)
{
	// calculate distance of point to camera
	GLdouble* modelView = App::Inst().GetViewport()->GetModelViewMatrix();
	GLdouble* projection = App::Inst().GetViewport()->GetProjectionMatrix();
	GLint* viewport = App::Inst().GetViewport()->GetViewportDimensions();

	GLdouble in[4];
	in[0] = position.x;
	in[1] = position.y;
	in[2] = position.z;
	in[3] = 1;
	GLdouble out[4];
	glUtils::MultMatrixVecd(modelView, in, out);

	// scale point so it is a constant pixel size regardless of distance from camera
	double d = sqrt(out[0]*out[0] + out[1]*out[1] + out[2]*out[2]);
	double pixelSize = d / (viewport[3]*0.5*projection[5]); 

	return size * pixelSize;
}
