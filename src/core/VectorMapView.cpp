//=======================================================================
// Author: Timothy Mankowski & Somayyeh Zangooei
//
// Copyright 2012 Timothy Mankowski & Somayyeh Zangooei
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
#include "../core/VectorMapView.hpp"
#include "../core/VectorMapModel.hpp"
#include "../core/Viewport.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/Colour.hpp"
#include "../core/MapModel.hpp"
#include "../core/MapLayer.hpp"
#include "../core/MapController.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/LayerTreeModel.hpp"
#include "../core/StudyLayer.hpp"
#include "../core/VisualMarker.hpp"
#include "../core/Camera.hpp"
#include "../utils/glUtils.hpp"



using namespace GenGIS;

VectorMapView::VectorMapView( VectorMapModelPtr vectorMapModel ) : 
	m_vectorMapModel( vectorMapModel ),
	m_pointColour((float) rand() / RAND_MAX, (float) rand() / RAND_MAX,(float) rand() / RAND_MAX ),
	m_pointSize(3.0),
	m_pointBorderColour((float) rand() / RAND_MAX, (float) rand() / RAND_MAX,(float) rand() / RAND_MAX),
	m_pointBorderSize(0.0), 
	m_pointShape(VisualMarker::CIRCLE),
	m_lineColour((float) rand() / RAND_MAX, (float) rand() / RAND_MAX,(float) rand() / RAND_MAX),
	m_lineSize(1.0),
	m_lineBorderColour((float) rand() / RAND_MAX, (float) rand() / RAND_MAX,(float) rand() / RAND_MAX),
	m_lineBorderSize(0.0), 
	m_lineStyle(VisualLine::SOLID),
	m_polygonBorderColour((float) rand() / RAND_MAX, (float) rand() / RAND_MAX,(float) rand() / RAND_MAX),
	m_polygonBorderSize(1.0),
	m_polygonBorderStyle(VisualLine::SOLID)
{
	//
}

template<class Archive>
void VectorMapView::serialize(Archive & ar, const unsigned int file_version)
{
	ar & boost::serialization::base_object<View>(*this);	
	ar & m_pointColour;            
	ar & m_pointBorderColour;             
	ar & m_pointSize;           
	ar & m_pointBorderSize;           
	ar & m_pointShape;             
	ar & m_lineColour;       
	ar & m_lineBorderColour;      
	ar & m_lineSize;        
	ar & m_lineBorderSize;     
	ar & m_lineStyle;    
	ar & m_polygonBorderColour;                
	ar & m_polygonBorderSize;                
	ar & m_polygonBorderStyle;     	
	
}

template void VectorMapView::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void VectorMapView::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


void VectorMapView::RenderLineWithBorder(const Line3D& line, const Colour& colour, float thickness, 
										 VisualLine::LINE_STYLE style, const Colour& borderColour, float borderThickness)
{
	float SHORT_DASH_LEN = 5* App::Inst().GetResolutionFactor();
	float LONG_DASH_LEN = 15 * App::Inst().GetResolutionFactor();
	float DASH_SPACING = 5 * App::Inst().GetResolutionFactor();
	const int REPEAT_FACTOR = 2;

	int width= App::Inst().GetViewport()->GetCamera()->GetWindowWidth();
	int height=App::Inst().GetViewport()->GetCamera()->GetWindowHeight();
	double aspect=m_vectorMapModel->GetAspect();
	
	if(aspect<1)
	{
		SHORT_DASH_LEN=(2*SHORT_DASH_LEN/width);
		LONG_DASH_LEN=(2*LONG_DASH_LEN/width);
		DASH_SPACING=(2*DASH_SPACING/width);
	}
	else
	{
		SHORT_DASH_LEN= (SHORT_DASH_LEN*aspect)/height;
		LONG_DASH_LEN= (LONG_DASH_LEN*aspect)/height;
		DASH_SPACING= (DASH_SPACING*aspect)/height;


	}		
	
	glDepthRange( TERRAIN_START_DEPTH, 1.0 );

	if(style == VisualLine::HIDDEN)
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

		Point3D end=line.end;
		Point3D start=line.start;
		Point3D startToEnd = end - start;
		startToEnd.Normalize();
		Point3D normalToLine = Point3D(-startToEnd.z,0, startToEnd.x);

		if(style == VisualLine::SOLID)
		{
			RenderLineWithBorder(start, end, startToEnd, normalToLine, colour, thickness, borderColour, borderThickness);
		}
		else if(style == VisualLine::SHORT_DASH || style == VisualLine::LONG_DASH)
		{
			float offset = SHORT_DASH_LEN;
			if(style == VisualLine::LONG_DASH)
				offset = LONG_DASH_LEN;

			Point3D dashStart = start;
			Point3D dashEnd = start + startToEnd*offset;
			
			while(true)
			{
				Point3D endDashToEndPt = end - dashEnd;
				if(endDashToEndPt.x*startToEnd.x > 0 && endDashToEndPt.z*startToEnd.z > 0)
				{
					// end of dash is before the end point
					RenderLineWithBorder(dashStart, dashEnd, startToEnd, normalToLine, colour, thickness, borderColour, borderThickness);

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
					if(startDashToEndPt.x*startToEnd.x >=0 && startDashToEndPt.z*startToEnd.z >= 0)
					{
						// render final dash
						RenderLineWithBorder(dashStart, dashEnd, startToEnd, normalToLine, colour, thickness, borderColour, borderThickness);
					}

					break;
				}
			}
		}

}

void VectorMapView::RenderLineWithBorder(const Point3D& start, const Point3D& end, const Point3D& startToEnd, const Point3D& normalToLine,
													const Colour& colour, float thickness, const Colour& borderColour, float borderThickness)
{
	int width= App::Inst().GetViewport()->GetCamera()->GetWindowWidth();
	int height=App::Inst().GetViewport()->GetCamera()->GetWindowHeight();

	double aspect=m_vectorMapModel->GetAspect();
	float halfThickness;
	/*
	if(aspect<1)
		halfThickness=thickness/width;
	else
		halfThickness= (thickness*aspect)/height;
	// Render solid line with border
	//float halfThickness = 0.5*screenthickness;
	//halfThickness=0.005;
	*/
	GLdouble* modelView = App::Inst().GetViewport()->GetModelViewMatrix();
	GLdouble* projection = App::Inst().GetViewport()->GetProjectionMatrix();
	GLint* viewport = App::Inst().GetViewport()->GetViewportDimensions();
	GLdouble in[4];
	in[0] = start.x;
	in[1] = start.y;
	in[2] = start.z;
	in[3] = 1;
	GLdouble out[4];
	glUtils::MultMatrixVecd(modelView, in, out);
	// scale point so it is a constant pixel size regardless of distance from camera
	double d = sqrt(out[0]*out[0] + out[1]*out[1] + out[2]*out[2]);
	double pixelSize = d / (viewport[3]*0.5*projection[5]); 
	halfThickness= (thickness * pixelSize)/2;

	glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());
	glBegin(GL_QUADS);
		glVertex3f(start.x + halfThickness*normalToLine.x, start.y , start.z+ halfThickness*normalToLine.z );
		glVertex3f(end.x + halfThickness*normalToLine.x, end.y , end.z+ halfThickness*normalToLine.z);
		
		glVertex3f(end.x - halfThickness*normalToLine.x, end.y , end.z- halfThickness*normalToLine.z);
		glVertex3f(start.x - halfThickness*normalToLine.x, start.y, start.z - halfThickness*normalToLine.z);			
	glEnd();

	if(borderThickness > 0.0f)
	{
		Point3D start1(start.x + halfThickness*normalToLine.x, start.y, start.z + halfThickness*normalToLine.z);
		Point3D end1(end.x + halfThickness*normalToLine.x,end.y, end.z + halfThickness*normalToLine.z);
		RenderAntialiasedLine(start1, end1, borderColour, borderThickness, VisualLine::SOLID);

		Point3D start2(start.x - halfThickness*normalToLine.x, start.y,start.z - halfThickness*normalToLine.z);
		Point3D end2(end.x - halfThickness*normalToLine.x,end.y, end.z - halfThickness*normalToLine.z);
		RenderAntialiasedLine(start2, end2, borderColour, borderThickness, VisualLine::SOLID);
	}
	
}

void VectorMapView::RenderAntialiasedLine(const Point3D& start, const Point3D& end, const Colour& colour, 
										  float thickness, VisualLine::LINE_STYLE style)
{
	const int REPEAT_FACTOR = 2;

	if(style == VisualLine::HIDDEN)
		return;

	glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());

	if(thickness == 1.0f)
	{
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(REPEAT_FACTOR, style);

		glLineWidth(1.0f);
		glBegin(GL_LINES);
			glVertex3f(start.x, start.y, start.z);
			glVertex3f(end.x, end.y, end.z);
		glEnd();

		glDisable(GL_LINE_STIPPLE);

		return;
	}

	Point3D startToEnd = end - start;
	startToEnd.Normalize();

	Point3D normalToLine = Point3D(-startToEnd.z, 0,startToEnd.x);

	int width= App::Inst().GetViewport()->GetCamera()->GetWindowWidth();
	int height=App::Inst().GetViewport()->GetCamera()->GetWindowHeight();

	double aspect=m_vectorMapModel->GetAspect();
	float halfThickness;
	/*
	if(aspect<1)
		halfThickness=thickness/width;
	else
		halfThickness= (thickness*aspect)/height;
	// Render solid line with border
	//float halfThickness = 0.5*screenthickness;
	//halfThickness=0.05;
*/
	GLdouble* modelView = App::Inst().GetViewport()->GetModelViewMatrix();
	GLdouble* projection = App::Inst().GetViewport()->GetProjectionMatrix();
	GLint* viewport = App::Inst().GetViewport()->GetViewportDimensions();
	GLdouble in[4];
	in[0] = start.x;
	in[1] = start.y;
	in[2] = start.z;
	in[3] = 1;
	GLdouble out[4];
	glUtils::MultMatrixVecd(modelView, in, out);
	// scale point so it is a constant pixel size regardless of distance from camera
	double d = sqrt(out[0]*out[0] + out[1]*out[1] + out[2]*out[2]);
	double pixelSize = d / (viewport[3]*0.5*projection[5]); 
	halfThickness= (thickness * pixelSize)/2;

	glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());
	glBegin(GL_QUADS);
		glVertex3f(start.x + halfThickness*normalToLine.x, start.y , start.z+ halfThickness*normalToLine.z );
		glVertex3f(end.x + halfThickness*normalToLine.x, end.y , end.z+ halfThickness*normalToLine.z);
		
		glVertex3f(end.x - halfThickness*normalToLine.x, end.y , end.z- halfThickness*normalToLine.z);
		glVertex3f(start.x - halfThickness*normalToLine.x, start.y, start.z - halfThickness*normalToLine.z);			
	glEnd();

	// render antialiased border
	glLineWidth(1.0f);			
	glBegin(GL_LINES);
		glVertex3f(start.x + halfThickness*normalToLine.x,start.y, start.z + halfThickness*normalToLine.z);
		glVertex3f(end.x + halfThickness*normalToLine.x, end.y, end.z + halfThickness*normalToLine.z);

		glVertex3f(start.x - halfThickness*normalToLine.x,start.y, start.z - halfThickness*normalToLine.z);
		glVertex3f(end.x - halfThickness*normalToLine.x, end.y, end.z - halfThickness*normalToLine.z);
	glEnd();
}


void VectorMapView::Render() 
{
	if( !m_bVisible )
		return;

	Render( false );
}

void VectorMapView::Render( bool bSimplified )
{
	glDepthRange( TERRAIN_START_DEPTH, 1.0 );

	glDisable( GL_LIGHTING );
	//glDisable( GL_BLEND );

	Colour bgColour = App::Inst().GetViewport()->GetBackgroundColour();
	glColor4ub( bgColour.GetRedInt(), bgColour.GetGreenInt(), bgColour.GetBlueInt(), 255 );

	// Set the colour of the brush
	Colour colour,bColour;
	float size,bSize;
	VisualMarker::MARKER_SHAPE shape;
	VisualLine::LINE_STYLE style;
	const int REPEAT_FACTOR = 2;
	GLfloat fSizes[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE,fSizes);
	SetPolygonMaxWidthSize(fSizes[1]);
	//SetPolygonMinWidthSize(fSizes[0]);
	SetPolygonMinWidthSize(1);
	//wxBusyCursor wait;

	for ( unsigned int currGeometry = 0; currGeometry < m_vectorMapModel->GetNumberOfGeometries(); currGeometry++ )
	{
		GeoVector* GeoVector = m_vectorMapModel->GetGeoVector( currGeometry );

		// Handle points
		if ( GeoVector->GetGeometryType() == wkbPoint )
		{			
			colour = GetPointColour();
			bColour = GetPointBorderColour();			
			size= GetPointSize();
			bSize= GetPointBorderSize();
			shape= GetPointShape();			
		}
		//Handle polylines
		else if ( GeoVector->GetGeometryType() == wkbLineString )
		{			
			colour = GetLineColour();
			bColour = GetLineBorderColour();			
			size=GetLineSize();
			bSize=GetLineBorderSize();
			style=GetLineStyle();		
			
			if(bSize==0 && GetPolygonMinWidthSize()<= size && size<= GetPolygonMaxWidthSize())
			{
				glColor4f(colour.GetRed(),colour.GetGreen(),colour.GetBlue(),colour.GetAlpha());
				glEnable (GL_LINE_SMOOTH);
				glEnable (GL_BLEND);
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
				glLineWidth(size * App::Inst().GetResolutionFactor());
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(REPEAT_FACTOR, style);
				glBegin( GL_LINE_STRIP);	
			}
			
		}
		// Handle rings
		else if ( GeoVector->GetGeometryType() == wkbLinearRing )
		{			
			colour = GetPolygonBorderColour();
			size =  GetPolygonBorderSize();
			style = GetPolygonBorderStyle();						
			
			if( GetPolygonBorderSize() >= GetPolygonMinWidthSize() &&  GetPolygonBorderSize() <= GetPolygonMaxWidthSize())
			{
				glColor4f(colour.GetRed(),colour.GetGreen(),colour.GetBlue(),colour.GetAlpha());
				glEnable (GL_LINE_SMOOTH);
				glEnable (GL_BLEND);
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
				glLineWidth(size * App::Inst().GetResolutionFactor());
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(REPEAT_FACTOR, style);
				glBegin( GL_LINE_STRIP );	
			}
			
          
		}
		// Skip for unsupported types
		else
		{
			continue;
		}
		MapControllerPtr mapController= App::Inst().GetLayerTreeController()->GetLayerTreeModel()->GetStudy(0)->GetMapLayer(0)->GetMapController();
		float ePointX=GeoVector->pointX[0], ePointY=GeoVector->pointY[0];
		bool flag=false;

		for ( unsigned int currPoint = 0; currPoint < GeoVector->GetNumberOfPoints(); currPoint++ )
		{
			Point3D sPoint;
			sPoint.x = GeoVector->pointX[currPoint];
			sPoint.z = GeoVector->pointY[currPoint];
			sPoint.y= mapController->GetExaggeratedElevation(sPoint.x, sPoint.z);

			//if ( GeoVector->GetGeometryType() == wkbLinearRing )
			float sPointX = GeoVector->pointX[currPoint];
			float sPointY = GeoVector->pointY[currPoint];
			float sPointZ = mapController->GetExaggeratedElevation(sPointX, sPointY);
								
			//Visualize rings whose thicknesses are between the line width range
			if ( GeoVector->GetGeometryType() == wkbLinearRing && GetPolygonBorderSize() >= GetPolygonMinWidthSize() &&  GetPolygonBorderSize() <= GetPolygonMaxWidthSize())
			{
				double dist = sqrt( pow( ePointX - sPointX, 2 ) + pow( ePointY - sPointY, 2 ) );
				double scaledMinX  = m_vectorMapModel->GetVectorBoundary_ScaledMinX();
				double scaledMaxX  = m_vectorMapModel->GetVectorBoundary_ScaledMaxX();		

				if (dist < 7.0/8.0*(fabs(scaledMaxX-scaledMinX)))
				{
					glVertex3f(sPointX,sPointZ,sPointY );
					ePointX=sPointX;
					ePointY=sPointY;
				}
				else
				{
					flag=true;
					glEnd();
					glBegin( GL_LINE_STRIP );
					glVertex3f(sPointX,sPointZ,sPointY );
					ePointX=sPointX;
					ePointY=sPointY;
					
				}

			}
			//Visualize rings whose thicknesses are outside the line width range
			else if ( GeoVector->GetGeometryType() == wkbLinearRing && (GetPolygonBorderSize() < GetPolygonMinWidthSize() || GetPolygonBorderSize() > GetPolygonMaxWidthSize() ) )
			{
			
			//if ( GeoVector->GetGeometryType() == wkbLinearRing)
			//{
				if(currPoint!=(GeoVector->GetNumberOfPoints()-1)){
					Point3D ePoint;				
					ePoint.x = GeoVector->pointX[currPoint+1];
					ePoint.z = GeoVector->pointY[currPoint+1];
					ePoint.y= mapController->GetExaggeratedElevation(ePoint.x, ePoint.z);
					double dist = sqrt( pow( ePoint.x - sPoint.x, 2 ) + pow( ePoint.y - sPoint.y, 2 ) );
					double scaledMinX  = m_vectorMapModel->GetVectorBoundary_ScaledMinX();
					double scaledMaxX  = m_vectorMapModel->GetVectorBoundary_ScaledMaxX();		

					if (dist < 7.0/8.0*(fabs(scaledMaxX-scaledMinX)))
					{
						RenderLineWithBorder(Line3D(sPoint, ePoint), colour, size, style, Colour(0,0,0), 0.0);
					}
				}
				else{
					Point3D ePoint;
					ePoint.x = GeoVector->pointX[0];
					ePoint.z = GeoVector->pointY[0];
					ePoint.y= mapController->GetExaggeratedElevation(ePoint.x, ePoint.z);
					double dist = sqrt( pow( ePoint.x - sPoint.x, 2 ) + pow( ePoint.y - sPoint.y, 2 ) );
					double scaledMinX  = m_vectorMapModel->GetVectorBoundary_ScaledMinX();
					double scaledMaxX  = m_vectorMapModel->GetVectorBoundary_ScaledMaxX();	
					if (dist < 7.0/8.0*(fabs(scaledMaxX-scaledMinX)))
					{
						RenderLineWithBorder(Line3D(sPoint, ePoint), colour, size, style, Colour(0,0,0), 0.0);
					}
				}
				
			}		
			else if(GeoVector->GetGeometryType() == wkbPoint)	
			{
				VisualMarker::RenderDecal(sPoint, colour, size, shape, bColour, bSize, m_bSelected, 0);
			}
			
			else if ( GeoVector->GetGeometryType() == wkbLineString )
			{
				if(currPoint!=(GeoVector->GetNumberOfPoints()-1)){
					Point3D ePoint;				
					ePoint.x = GeoVector->pointX[currPoint+1];
					ePoint.z = GeoVector->pointY[currPoint+1];
					ePoint.y= mapController->GetExaggeratedElevation(ePoint.x, ePoint.z);
					double dist = sqrt( pow( ePoint.x - sPoint.x, 2 ) + pow( ePoint.y - sPoint.y, 2 ) );
					double scaledMinX  = m_vectorMapModel->GetVectorBoundary_ScaledMinX();
					double scaledMaxX  = m_vectorMapModel->GetVectorBoundary_ScaledMaxX();		

					if (dist < 7.0/8.0*(fabs(scaledMaxX-scaledMinX)))
					{
						if (bSize==0 && GetPolygonMinWidthSize()<= size && size<= GetPolygonMaxWidthSize())	
							glVertex3f(sPointX,sPointZ,sPointY );
						else
							RenderLineWithBorder(Line3D(sPoint, ePoint), colour, size, style, bColour, bSize);
						
					}
				}
			}			
			
		}
		
		if ( GeoVector->GetGeometryType() == wkbLinearRing && GetPolygonBorderSize() >= GetPolygonMinWidthSize() &&  GetPolygonBorderSize() <= GetPolygonMaxWidthSize())
		{			
			if (!flag)			
				glVertex3f(GeoVector->pointX[0], mapController->GetExaggeratedElevation(GeoVector->pointX[0], GeoVector->pointY[0]),GeoVector->pointY[0] );
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			
		}
		if ( GeoVector->GetGeometryType() == wkbLineString && bSize==0 && GetPolygonMinWidthSize()<= size && size<= GetPolygonMaxWidthSize())
		{
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
		
		
	}
	//glEnable(GL_BLEND);
	//glDisable(GL_BLEND);
	
	glFlush();
}



