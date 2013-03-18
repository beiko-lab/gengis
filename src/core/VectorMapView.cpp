//=======================================================================
// Author: Timothy Mankowski
//
// Copyright 2012 Timothy Mankowski
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



using namespace GenGIS;

VectorMapView::VectorMapView( VectorMapModelPtr vectorMapModel ) : 
	m_vectorMapModel( vectorMapModel ),
	m_pointColour((float) rand() / RAND_MAX, (float) rand() / RAND_MAX,(float) rand() / RAND_MAX ),
	m_pointSize(4.0),
	m_pointBorderColour((float) rand() / RAND_MAX, (float) rand() / RAND_MAX,(float) rand() / RAND_MAX),
	m_pointBorderSize(1.0), 
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



void VectorMapView::Render() 
{
	if( !m_bVisible )
		return;

	Render( false );
}

void VectorMapView::Render( bool bSimplified )
{
	glDepthRange( TERRAIN_START_DEPTH, 1.0 );
	// glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glDisable( GL_LIGHTING );
	glDisable( GL_BLEND );

	Colour bgColour = App::Inst().GetViewport()->GetBackgroundColour();
	glColor4ub( bgColour.GetRedInt(), bgColour.GetGreenInt(), bgColour.GetBlueInt(), 255 );

	// Set the colour of the brush
	Colour colour,bColour;
	float size,bSize;
	VisualMarker::MARKER_SHAPE shape;
	VisualLine::LINE_STYLE style;
	const int REPEAT_FACTOR = 3;
	//wxBusyCursor wait;

	for ( unsigned int currGeometry = 0; currGeometry < m_vectorMapModel->GetNumberOfGeometries(); currGeometry++ )
	{
		GeoVector* GeoVector = m_vectorMapModel->GetGeoVector( currGeometry );

		// Handle points
		if ( GeoVector->GetGeometryType() == wkbPoint )
		{
			//glEnable( GL_POINT_SMOOTH ) ;
			//glColor4f(pColour.GetRed(), pColour.GetGreen(), pColour.GetBlue(), pColour.GetAlpha());
			//glColor3f( 0.75f, 0.75f, 0.1f );			
			//glBegin( GL_POINTS );
			colour = GetPointColour();
			bColour = GetPointBorderColour();			
			size= GetPointSize();
			bSize= GetPointBorderSize();
			shape= GetPointShape();			
		}
		//Handle polylines
		else if ( GeoVector->GetGeometryType() == wkbLineString )
		{
			//glColor3f( 0.0f, 0.0f, 1.0f );
			//glLineWidth(1.0f);
			//glBegin( GL_LINES );
			colour = GetLineColour();
			bColour = GetLineBorderColour();			
			size=GetLineSize();
			bSize=GetLineBorderSize();
			style=GetLineStyle();			
		}
		// Handle rings
		else if ( GeoVector->GetGeometryType() == wkbLinearRing )
		{
			//glColor3f( 1.0f, 0.0f, 0.0f );
			//glColor3f( (float) rand() / RAND_MAX, (float) rand() / RAND_MAX,(float) rand() / RAND_MAX );
			//glBegin( GL_LINE_LOOP  );	
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//glEnable(GL_POLYGON_OFFSET_FILL); // Avoid Stitching!
			//glPolygonOffset(0.3f, 1.0f);
			//glColor3f(1.0f, 0.0f, 0.0f);
			//glBegin( GL_LINE_LOOP );
			colour = GetPolygonBorderColour();
			size =  GetPolygonBorderSize();
			style = GetPolygonBorderStyle();
			glColor4f(colour.GetRed(),colour.GetGreen(),colour.GetBlue(),colour.GetAlpha());
			glLineWidth(size * App::Inst().GetResolutionFactor());
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(REPEAT_FACTOR, style);
			//glBegin( GL_LINE_LOOP );
			glBegin( GL_LINE_STRIP );
			
          
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
			
			
			if ( GeoVector->GetGeometryType() == wkbLinearRing )
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
						VisualLine::RenderLineWithBorder(Line3D(sPoint, ePoint), colour, size, style, bColour, bSize, LAYOUT_PRIMATIVES_LAYER, m_bSelected);
						
					}
				}
			}
			/*
			else if ( GeoVector->GetGeometryType() == wkbLinearRing )
			{
				if(currPoint!=(GeoVector->GetNumberOfPoints()-1)){
					Point3D ePoint;				
					ePoint.x = GeoVector->pointX[currPoint+1];
					ePoint.z = GeoVector->pointY[currPoint+1];
					ePoint.y= mapController->GetExaggeratedElevation(ePoint.x, ePoint.z);
					VisualLine::RenderLineWithBorder(Line3D(sPoint, ePoint), colour, size, style, Colour(0,0,0), 0.0, LAYOUT_PRIMATIVES_LAYER, m_bSelected);
				}
				else{
					Point3D ePoint;
					ePoint.x = GeoVector->pointX[0];
					ePoint.z = GeoVector->pointY[0];
					ePoint.y= mapController->GetExaggeratedElevation(ePoint.x, ePoint.z);
					VisualLine::RenderLineWithBorder(Line3D(sPoint, ePoint), colour, size, style, Colour(0,0,0), 0.0, LAYOUT_PRIMATIVES_LAYER, m_bSelected);
				}
				
			}
			*/
			
		}
		if ( GeoVector->GetGeometryType() == wkbLinearRing)
		{			
			if (!flag)			
				glVertex3f(GeoVector->pointX[0], mapController->GetExaggeratedElevation(GeoVector->pointX[0], GeoVector->pointY[0]),GeoVector->pointY[0] );
			glEnd();
					
			
		}
		
	}
/*
	// Draw the outline of the map
		glBegin( GL_LINE_LOOP );
			glVertex3f( -1.0f, 0.0f,  height/2 );
			glVertex3f(  1.0f, 0.0f,  height/2 );
			glVertex3f(  1.0f, 0.0f, -height/2 );
			glVertex3f( -1.0f, 0.0f, -height/2 );		
		glEnd();
	}
	
	*/
	glEnable(GL_BLEND);
	glDisable(GL_LINE_STIPPLE);

	glFlush();
}



