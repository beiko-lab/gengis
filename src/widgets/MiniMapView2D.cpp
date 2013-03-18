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

#include "../widgets/MiniMapView2D.hpp"

#include "../core/App.hpp"
#include "../core/Camera.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/LayerTreeModel.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/MapLayer.hpp"
#include "../core/StudyLayer.hpp"
#include "../core/MapController.hpp"
#include "../core/MapTexture.hpp"
#include "../core/MapView.hpp"
#include "../core/MapModel.hpp"
#include "../core/VectorMapModel.hpp"
#include "../core/VectorMapController.hpp"
#include "../core/VectorMapLayer.hpp"
#include "../core/OrthoLayers.hpp"

#include "../core/Viewport.hpp"

#include "../utils/Geometry.hpp"

using namespace GenGIS;

MiniMapView2D::MiniMapView2D() : ViewWidget() 
{
	m_origin.x = 0;
	m_origin.y = 0;
}

GeoCoord MiniMapView2D::MiniMapCoordToGeoCoord( Point3D& miniMapCoord ) 
{
	// This code has not been verified. Use with caution.

	GeoCoord coord;

	FileHeader* header = App::Inst().GetMapController()->GetHeader();

	coord.easting = ( -( ( miniMapCoord.x + 1 ) * ( header->projExtents.Width() ) /
		2 + header->projExtents.x ));

	coord.northing = (-( ( miniMapCoord.y + 1 ) * ( header->projExtents.Height()) /
		2 + header->projExtents.y ) );

	return coord;
}

void MiniMapView2D::MiniMapCoordToWorldCoord( Point3D& miniMapCoord, Point3D& worldCoord ) 
{
	FileHeader* header = App::Inst().GetMapController()->GetHeader();

	float spanX = (float)(header->nCols * header->cellSizeX);
	float spanY = (float)(header->nRows * header->cellSizeZ);

	worldCoord.x = ( miniMapCoord.x + 1 ) * ( header->nCols * header->cellSizeX ) / 2 - spanX*0.5f;
	worldCoord.y = ( miniMapCoord.y + 1 ) * ( header->nRows * header->cellSizeZ ) / 2 - spanY*0.5f;
	worldCoord.z = 0;
}

Point3D MiniMapView2D::GeoCoordToMiniMapCoord( GeoCoord& geoCoord ) 
{
	Point3D coord;

	FileHeader* header = App::Inst().GetMapController()->GetHeader();

	coord.x = 2*(geoCoord.easting - header->projExtents.x) /
		(header->projExtents.Width()) - 1;

	coord.y = 2*(geoCoord.northing - header->projExtents.y) /
		(header->projExtents.Height()) - 1;

	coord.z = 0;

	return coord;
}



Point3D MiniMapView2D::WorldCoordToMiniMapCoord( Point3D& worldCoord ) 
{
	// Note: this is almost certainly incorrect. Needs to take into account span of map. 
	// See miniMapCoordToWorldCoord.

	FileHeader* header = App::Inst().GetMapController()->GetHeader();
	Point3D mmCoord;

	mmCoord.x = -2 * worldCoord.x / ( header->nCols * header->cellSizeX ) + 1;
	mmCoord.y = 2 * worldCoord.z / ( header->nRows * header->cellSizeZ ) - 1;

	return mmCoord;
}

void MiniMapView2D::DrawTerrain()
{
	if(App::Inst().GetMapController()->IsLoaded())
	{
		glPushMatrix();
		glTranslated(0.0, 0.0, -WIDGETS_LAYER );
		glScaled(1,1,1);

		glDisable( GL_LIGHTING );

		glColor4f(1, 1, 1, 1);
		uint id = App::Inst().GetMapController()->GetMapModel()->GetTexture()->GetTextureId();
		glBindTexture(GL_TEXTURE_2D, App::Inst().GetMapController()->GetMapModel()->GetTexture()->GetTextureId());
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex2f(+1, -1);

		glTexCoord2f(0, 1);
		glVertex2f(+1, +1);

		glTexCoord2f(1, 1);   
		glVertex2f(-1, +1);

		glTexCoord2f(1, 0);    
		glVertex2f(-1, -1);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glEnable( GL_LIGHTING );
		glPopMatrix();
	}
}

void MiniMapView2D::Render() 
{
	const float LOCATION_MARKER_SIZE = 3.0;

	if ( App::Inst().GetMapController() == NULL )
		return;

	if(m_bVisible && App::Inst().GetMapController()->IsLoaded() && !App::Inst().IsSavingHiResImage())
	{
		glPushMatrix();  

		glTranslated( m_screenCoordinates.x, m_screenCoordinates.y, -(WIDGETS_LAYER+1) );
		glScaled( m_scale.x, m_scale.y, m_scale.z );  
		glRotated( 180, 0, 0, 1 );

		DrawTerrain();

		// draw location markers
		if(App::Inst().GetLayerTreeController())
		{
			for(unsigned int studyIndex = 0; studyIndex < App::Inst().GetLayerTreeController()->GetNumStudyLayers(); studyIndex++)
			{
				StudyLayerPtr study = App::Inst().GetLayerTreeController()->GetStudyLayer(studyIndex);
				if(study->IsActive())
				{
					for(unsigned int mapIndex = 0; mapIndex < study->GetNumMapLayers(); ++mapIndex)
					{
						MapLayerPtr map = study->GetMapLayer(mapIndex);
						if(map->IsActive())
						{
							for(unsigned int locationSetIndex = 0; locationSetIndex < map->GetNumLocationSetLayers(); ++locationSetIndex)
							{
								LocationSetLayerPtr locationSet = map->GetLocationSetLayer(locationSetIndex);
								if(locationSet->IsActive())
								{
									for(unsigned int locationIndex = 0; locationIndex < locationSet->GetNumLocationLayers(); ++locationIndex)
									{
										LocationLayerPtr location = locationSet->GetLocationLayer(locationIndex);
										if(location->IsActive())
										{
											GeoCoord temp = location->GetLocationController()->GetProjection();
											Point3D mapCoord = GeoCoordToMiniMapCoord(temp);

											if(fabs(mapCoord.x) <= 1.0 && fabs(mapCoord.y) <= 1.0)
											{
												glDisable( GL_LIGHTING );
												glColor3f(1.0f, 0.0f, 0.0f);
												glPointSize(LOCATION_MARKER_SIZE);
												glBegin(GL_POINTS);	
												glVertex3f(-mapCoord.x, -mapCoord.y, 1.0);
												glEnd();
												glEnable( GL_LIGHTING );  
											}
										}
									}
								}
							}
						}
					}
					
					for(unsigned int vectorMapIndex = 0; vectorMapIndex < study->GetNumVectorMapLayers(); ++vectorMapIndex)
					{
						
						VectorMapLayerPtr vectorMapLayer = study->GetVectorMapLayer(vectorMapIndex);
						VectorMapControllerPtr m_vectorMapController= vectorMapLayer->GetVectorMapController();
						VectorMapModelPtr m_vectorMapModel = m_vectorMapController->GetVectorMapModel();
						double aspect;
						if ( App::Inst().GetLayerTreeController()->GetNumMapLayers() >0 )
						{
							MapControllerPtr mapController= App::Inst().GetLayerTreeController()->GetLayerTreeModel()->GetStudy(0)->GetMapLayer(0)->GetMapController();
							FileHeader* header=mapController->GetMapModel()->GetHeader();		
							aspect = header->projExtents.Height() / header->projExtents.Width();
		
						}
					else{
							double minX=m_vectorMapModel->GetVectorBoundary_MinX();
							double minY=m_vectorMapModel->GetVectorBoundary_MinY();
							double maxX=m_vectorMapModel->GetVectorBoundary_MaxX();
							double maxY=m_vectorMapModel->GetVectorBoundary_MaxY();
							aspect = fabs((maxY-minY) / (maxX-minX));		
						}
						if(vectorMapLayer->IsActive())
						{
							
							glDisable( GL_LIGHTING );							
							for ( unsigned int currGeometry = 0; currGeometry < m_vectorMapModel->GetNumberOfGeometries(); currGeometry++ )
								{
									
									GeoVector* GeoVector = m_vectorMapModel->GetGeoVector( currGeometry );									
									if ( GeoVector->GetGeometryType() == wkbPoint )
									{
										
										glEnable( GL_POINT_SMOOTH ) ;
										glPointSize(3);
										//glPointSize( m_vectorMapController->GetVectorMapView()->GetPointSize());
										Colour colour = m_vectorMapController->GetVectorMapView()->GetPointColour();
										glColor3f(colour.GetRed(), colour.GetGreen(), colour.GetBlue());
										//glColor3f( 1.0f, 0.0f, 0.0f );			
										glBegin( GL_POINTS );																																															
										

									}
									else if ( GeoVector->GetGeometryType() == wkbLineString )
									{
										Colour colour = m_vectorMapController->GetVectorMapView()->GetLineColour();
										glColor3f(colour.GetRed(), colour.GetGreen(), colour.GetBlue());
										glLineWidth(1.0f);
										glBegin( GL_LINES );
									}
									else if ( GeoVector->GetGeometryType() == wkbLinearRing )
									{
										Colour colour = m_vectorMapController->GetVectorMapView()->GetPolygonBorderColour();
										glColor3f(colour.GetRed(), colour.GetGreen(), colour.GetBlue());
										//glLineWidth(0.5f);
										glLineWidth(m_vectorMapController->GetVectorMapView()->GetPolygonBorderSize());
										//glBegin( GL_LINE_LOOP  );
										glBegin( GL_LINE_STRIP );
									}
									bool flag_map=false, flag_minimap=false, closedLoop=true ;
									float ePointX=GeoVector->pointX[0], ePointY=GeoVector->pointY[0];
									for ( unsigned int currPoint = 0; currPoint < GeoVector->GetNumberOfPoints(); currPoint++ )
									{										
										float sPointX = GeoVector->pointX[currPoint];
										float sPointY = GeoVector->pointY[currPoint];
										float sPointZ = 1;										

										if ( GeoVector->GetGeometryType() == wkbLinearRing )
										{
											double dist = sqrt( pow( ePointX - sPointX, 2 ) + pow( ePointY - sPointY, 2 ) );
											double scaledMinX  = m_vectorMapModel->GetVectorBoundary_ScaledMinX();
											double scaledMaxX  = m_vectorMapModel->GetVectorBoundary_ScaledMaxX();		

											if (fabs(sPointX) <= 1.0 && fabs(sPointY/aspect) <= 1.0 )
											{
												if( dist < 7.0/8.0*(fabs(scaledMaxX-scaledMinX)))
												{
													glVertex3f(-sPointX,sPointY/aspect,sPointZ );
													ePointX=sPointX;
													ePointY=sPointY;
													flag_minimap=true;
												}
												else
												{
													flag_map=true;
													glEnd();
													glBegin( GL_LINE_STRIP );
													glVertex3f(-sPointX,sPointY/aspect,sPointZ );
													ePointX=sPointX;
													ePointY=sPointY;
												}
											}
											else
											{
												closedLoop=false;
												if (flag_minimap)
												{
													if(sPointX >= 1.0)
														sPointX=1;
													else if(sPointX <= -1.0)
														sPointX=-1;
													sPointY=sPointY/aspect;
													if(sPointY >= 1.0)
														sPointY=1;
													else if(sPointY <= -1.0)
														sPointY=-1;
													if(dist < 7.0/8.0*(fabs(scaledMaxX-scaledMinX)))
														glVertex3f(-sPointX,sPointY,sPointZ );
												}
												
											}

										}
										else
										{
											if(fabs(sPointX) <= 1.0 && fabs(sPointY/aspect) <= 1.0 )
												glVertex3f(-sPointX,sPointY/aspect,sPointZ );
										}


										/*
												
										if(fabs(sPointX) <= 1.0 && fabs(sPointY/aspect) <= 1.0 && dist < 1.8)
										{
											glVertex3f(-sPointX,sPointY/aspect,sPointZ );
											flag=true;
											ePointX=sPointX;
											ePointY=sPointY;
										}
										if(flag=true && GeoVector->GetGeometryType() == wkbLinearRing)
										{
											if(sPointX >= 1.0)
												sPointX=1;
											else if(sPointX <= -1.0)
												sPointX=-1;
											sPointY=sPointY/aspect;
											if(sPointY >= 1.0)
												sPointY=1;
											else if(sPointY <= -1.0)
												sPointY=-1;
											if(dist < 7.0/8.0*(fabs(scaledMaxX-scaledMinX)))
												glVertex3f(-sPointX,sPointY,sPointZ );
										}
											
									}
									*/
									}
									if ( GeoVector->GetGeometryType() == wkbLinearRing)
									{			
										if (!flag_map && closedLoop)			
											glVertex3f(-(GeoVector->pointX[0]),(GeoVector->pointY[0])/aspect,1 );							
																				
									}	
									glEnd();
									
								}
							glEnable( GL_LIGHTING );  
						
						}
						
					}
				
					
				}
			}
		}

		glPopMatrix();
	}
	glFlush();
}

bool MiniMapView2D::IsClicked(const Point2D& mousePt) 
{
	if (!IsVisible())
		return false;

	Box2D box = GetBoundingBox();

	return (mousePt.x >= box.x && mousePt.x <= box.x + box.dx && 
		mousePt.y >= box.y && mousePt.y <= box.y + box.dy );
}

bool MiniMapView2D::MouseLeftDown(const Point2D& mousePt)
{
	if(!App::Inst().GetMapController()->IsLoaded() || !IsClicked(mousePt))
		return false;

	// convert screen cords to minimap grid coordinates
	Box2D box = GetBoundingBox();

	float u = mousePt.x;
	float v = mousePt.y;

	u -= box.x + box.dx / 2;
	v -= box.y + box.dy / 2;

	u = 2 * u / box.dx;
	v = 2 * v / box.dy;

	Point3D gridCoord;
	gridCoord.x = u;
	gridCoord.y = v;

	Point3D newWorld;
	MiniMapCoordToWorldCoord( gridCoord, newWorld );

	Point3D rotatedWorldCoord;
	float theta = App::Inst().GetViewport()->GetCamera()->GetYaw();
	rotatedWorldCoord.x = sin(PI_BY_2 - theta * DEG_TO_RAD ) * newWorld.x - sin( theta * DEG_TO_RAD ) * newWorld.y;
	rotatedWorldCoord.y = sin( theta * DEG_TO_RAD ) * newWorld.x + sin(PI_BY_2 - theta * DEG_TO_RAD ) * newWorld.y;

	float height = App::Inst().GetViewport()->GetCamera()->GetHeight();
	App::Inst().GetViewport()->GetCamera()->SetPosition(rotatedWorldCoord.x, rotatedWorldCoord.y, height);

	return true;
}
