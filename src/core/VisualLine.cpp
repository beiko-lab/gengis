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

#include "../core/VisualLine.hpp"
#include "../core/App.hpp"
#include "../core/Viewport.hpp"
#include "../core/Camera.hpp"
#include "../core/MapController.hpp"


#include "../utils/ErrorGL.hpp"
#include "../utils/Log.hpp"
#include "../utils/Point3D.hpp"

using namespace GenGIS;

const int REPEAT_FACTOR = 5;

template<class Archive>
void VisualLine::serialize(Archive & ar, const unsigned int file_version)
{
	ar & boost::serialization::base_object<VisualObject>(*this);
	ar & m_lineStyle; // LINE_STYLE
	ar & m_line;      // Line3D
	ar & m_depth;     // float
}
template void VisualLine::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void VisualLine::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void VisualLine::Render()
{
	if(!m_bVisible)
		return;

	RenderLineWithBorder(m_line, m_colour, GetSize(), m_lineStyle, m_colour, 0, m_depth, m_bSelected);
}

void VisualLine::RenderLineWithBorder(const Line3D& line, const Colour& colour, float thickness, 
                                      LINE_STYLE style, const Colour& borderColour, float borderThickness, 
                                      float depth, bool bSelected)
{
	const uint SHORT_DASH_LEN = 5 * App::Inst().GetResolutionFactor();
	const uint LONG_DASH_LEN = 15 * App::Inst().GetResolutionFactor();
	const uint DASH_SPACING = 5 * App::Inst().GetResolutionFactor();

	error::ErrorGL::Check();
	glDepthRange(0.0, TERRAIN_START_DEPTH);

	if(style == HIDDEN)
		return;

	// Special case: 1 px line with no border can be more rapidly rendered using GL_LINES
	if(thickness == 1.0f && borderThickness == 0.0f)
	{
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(REPEAT_FACTOR, style);

		glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());

		glLineWidth(1.0f);		
		glBegin(GL_LINES);
			glVertex3f(line.start.x, line.start.y, line.start.z);
			glVertex3f(line.end.x, line.end.y, line.end.z);
		glEnd();

		glDisable(GL_LINE_STIPPLE);

		return;
	}

	App::Inst().GetViewport()->GetCamera()->SetOrthoCamera();

	glPushMatrix();	
		Point3D start = App::Inst().GetMapController()->ProjectToScreen(line.start);
		Point3D end = App::Inst().GetMapController()->ProjectToScreen(line.end);

		Point3D startToEnd = end - start;
		startToEnd.Normalize();

		Point3D normalToLine = Point3D(-startToEnd.y, startToEnd.x, 0);

		if(style == SOLID)
		{
			RenderLineWithBorder(start, end, startToEnd, normalToLine, colour, thickness, borderColour, borderThickness, depth, bSelected);
		}
		else if(style == SHORT_DASH || style == LONG_DASH)
		{
			uint offset = SHORT_DASH_LEN;
			if(style == LONG_DASH)
				offset = LONG_DASH_LEN;

			Point3D dashStart = start;
			Point3D dashEnd = start + startToEnd*offset;
			
			while(true)
			{
				Point3D endDashToEndPt = end - dashEnd;
				if(endDashToEndPt.x*startToEnd.x > 0 && endDashToEndPt.y*startToEnd.y > 0)
				{
					// end of dash is before the end point
					RenderLineWithBorder(dashStart, dashEnd, startToEnd, normalToLine, colour, thickness, borderColour, borderThickness, depth, bSelected);

					dashStart += startToEnd*(offset+DASH_SPACING);
					dashEnd += startToEnd*(offset+DASH_SPACING);
				}
				else
				{
					// end of dask is after the end point

					// adjust the end of the dask to be at the end point
					dashEnd = end;

					// make sure start of the dask isn't also past the end point
					Point3D startDashToEndPt = end - dashStart;
					if(startDashToEndPt.x*startToEnd.x >=0 && startDashToEndPt.y*startToEnd.y >= 0)
					{
						// render final dash
						RenderLineWithBorder(dashStart, dashEnd, startToEnd, normalToLine, colour, thickness, borderColour, borderThickness, depth, bSelected);
					}

					break;
				}
			};
		}

	glPopMatrix();

	App::Inst().GetViewport()->GetCamera()->UnsetOrthoCamera();

	error::ErrorGL::Check();
}

void VisualLine::RenderLineWithBorder(const Point3D& start, const Point3D& end, const Point3D& startToEnd, const Point3D& normalToLine,
													const Colour& colour, float thickness, const Colour& borderColour, float borderThickness, float depth,
													bool bSelected)
{
	// Render solid line with border
	float halfThickness = 0.5*thickness;

	glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());
	glBegin(GL_QUADS);
		glVertex3f(start.x + halfThickness*normalToLine.x, start.y + halfThickness*normalToLine.y, -depth);
		glVertex3f(end.x + halfThickness*normalToLine.x, end.y + halfThickness*normalToLine.y, -depth);
		glVertex3f(end.x - halfThickness*normalToLine.x, end.y - halfThickness*normalToLine.y, -depth);
		glVertex3f(start.x - halfThickness*normalToLine.x, start.y - halfThickness*normalToLine.y, -depth);			
	glEnd();

	if(borderThickness > 0.0f)
	{
		Point3D start1(start.x + halfThickness*normalToLine.x, start.y + halfThickness*normalToLine.y, 0);
		Point3D end1(end.x + halfThickness*normalToLine.x, end.y + halfThickness*normalToLine.y, 0);
		RenderAntialiasedLine(start1, end1, borderColour, borderThickness, VisualLine::SOLID, depth);

		Point3D start2(start.x - halfThickness*normalToLine.x, start.y - halfThickness*normalToLine.y, 0);
		Point3D end2(end.x - halfThickness*normalToLine.x, end.y - halfThickness*normalToLine.y, 0);
		RenderAntialiasedLine(start2, end2, borderColour, borderThickness, VisualLine::SOLID, depth);
	}

	if(bSelected)
	{
		halfThickness = 0.5f*(thickness + borderThickness);
		
		Colour selectionColour = App::Inst().GetSelectionColour();

		Point3D start1(start.x + halfThickness*normalToLine.x, start.y + halfThickness*normalToLine.y, 0);
		Point3D end1(end.x + halfThickness*normalToLine.x, end.y + halfThickness*normalToLine.y, 0);
		RenderAntialiasedLine(start1, end1, selectionColour, App::Inst().GetSelectionThickness(), VisualLine::SOLID, depth);

		Point3D start2(start.x - halfThickness*normalToLine.x, start.y - halfThickness*normalToLine.y, 0);
		Point3D end2(end.x - halfThickness*normalToLine.x, end.y - halfThickness*normalToLine.y, 0);
		RenderAntialiasedLine(start2, end2, selectionColour, App::Inst().GetSelectionThickness(), VisualLine::SOLID, depth);
	}
}

void VisualLine::RenderAntialiasedLine(const Point3D& start, const Point3D& end, const Colour& colour, 
																			 float thickness, LINE_STYLE style, float depth)
{
	if(style == HIDDEN)
		return;

	glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());

	if(thickness == 1.0f)
	{
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(REPEAT_FACTOR, style);

		glLineWidth(1.0f);
		glBegin(GL_LINES);
			glVertex3f(start.x, start.y, -depth);
			glVertex3f(end.x, end.y, -depth);
		glEnd();

		glDisable(GL_LINE_STIPPLE);

		return;
	}

	Point3D startToEnd = end - start;
	startToEnd.Normalize();

	Point3D normalToLine = Point3D(-startToEnd.y, startToEnd.x, 0);

	float halfThickness = 0.5*thickness;
	
	glBegin(GL_QUADS);
		glVertex3f(start.x + halfThickness*normalToLine.x, start.y + halfThickness*normalToLine.y, -depth);
		glVertex3f(end.x + halfThickness*normalToLine.x, end.y + halfThickness*normalToLine.y, -depth);
		glVertex3f(end.x - halfThickness*normalToLine.x, end.y - halfThickness*normalToLine.y, -depth);
		glVertex3f(start.x - halfThickness*normalToLine.x, start.y - halfThickness*normalToLine.y, -depth);			
	glEnd();

	// render antialiased border
	glLineWidth(1.0f);			
	glBegin(GL_LINES);
		glVertex3f(start.x + halfThickness*normalToLine.x, start.y + halfThickness*normalToLine.y, -depth);
		glVertex3f(end.x + halfThickness*normalToLine.x, end.y + halfThickness*normalToLine.y, -depth);

		glVertex3f(start.x - halfThickness*normalToLine.x, start.y - halfThickness*normalToLine.y, -depth);
		glVertex3f(end.x - halfThickness*normalToLine.x, end.y - halfThickness*normalToLine.y, -depth);
	glEnd();
}

void VisualLine::RenderCylinder(const Point3D& start, const Point3D& end, float radius, int subdivisions, const Colour& colour, bool bSelected)
{
	error::ErrorGL::Check();

	glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());
	if(bSelected)
	{
		Colour selectedColour = App::Inst().GetSelectionColour();
		glColor4f(selectedColour.GetRed(), selectedColour.GetGreen(), selectedColour.GetBlue(), selectedColour.GetAlpha());
	}

	float vx = end.x - start.x;
	float vy = end.y - start.y;
	float vz = end.z - start.z;

	//handle the degenerate case with an approximation
	if(vz == 0)
		vz = .00000001f;

	float v = sqrt( vx*vx + vy*vy + vz*vz );
	float ax = RAD_TO_DEG*acos( vz/v );
	if ( vz < 0.0 )
		ax = -ax;

	float rx = -vy*vz;
	float ry = vx*vz;

	GLUquadricObj *quadric=gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);

	glPushMatrix();
		glTranslatef(start.x, start.y, start.z);
		glRotatef(ax, rx, ry, 0.0);

		//draw the cylinder
		gluCylinder(quadric, radius, radius, v, 32, 1);
		gluQuadricOrientation(quadric,GLU_INSIDE);

		//draw the first cap
		gluDisk( quadric, 0.0, radius, 32, 1);
		glTranslatef( 0,0,v );

		//draw the second cap
		gluQuadricOrientation(quadric,GLU_OUTSIDE);
		gluDisk( quadric, 0.0, radius, 32, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);

	error::ErrorGL::Check();
}
