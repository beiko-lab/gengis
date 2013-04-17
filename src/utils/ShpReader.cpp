//=======================================================================
// Author: Timothy Mankowski & Somayyeh Zangooei
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
#include "../utils/ShpReader.hpp"
#include "../utils/Log.hpp"
#include "../core/MapModel.hpp"
#include "../core/MapLayer.hpp"
#include "../utils/StringTools.hpp"
#include "../core/MapController.hpp"
#include "../core/VectorMapController.hpp"
#include "../core/VectorMapModel.hpp"
#include "../core/StudyController.hpp"
#include "../core/StudyLayer.hpp"
#include "../gui/ProgressDlg.hpp"
#include "../core/LayerTreeModel.hpp"

using namespace GenGIS;

bool ShpReader::Open(std::wstring fullPath, StudyControllerPtr studyController,
                     VectorMapControllerPtr vectorMapController, ProgressDlgPtr progressDlg) 
{
	// Registers all format drivers built into GDAL/OGR.
	OGRRegisterAll();

	OGRDataSource *OGRDataset;

	// Open vector file path
	std::string tempStr( fullPath.begin(), fullPath.end() );
	OGRDataset = OGRSFDriverRegistrar::Open( tempStr.c_str(), FALSE );

	// Return if no vector files are found
	if( OGRDataset == NULL )
	{
		Log::Inst().Warning("(Warning) Failed to open file at " + tempStr + ".");
		return false;
	}
	if ( App::Inst().GetLayerTreeController()->GetNumMapLayers() >0 )

		MapControllerPtr mapController= App::Inst().GetLayerTreeController()->GetLayerTreeModel()->GetStudy(0)->GetMapLayer(0)->GetMapController();

	// It appears that shapefiles (*.SHP) only support up to one layer per file
	// This will need to be further investigated for other vector filetypes (e.g., KML)
	// For now just grab layer at position 0
	
	OGRLayer *poLayer = OGRDataset->GetLayer( 0 );
	
	// Determine the XY boundaries for the entire vector dataset
	OGREnvelope psEnvelope;

	VectorMapModelPtr vectorMapModel = vectorMapController->GetVectorMapModel();
	poLayer->GetExtent( &psEnvelope );
	vectorMapModel->SetVectorBoundary(psEnvelope);

	if(!SetupVectorProjection(OGRDataset,studyController,poLayer,vectorMapController ))
	{
		OGRDataset->DestroyDataSource(OGRDataset);
		return false;
	}
	
	if(progressDlg)
	{
			if(!progressDlg->Update(0, _T("Reading Vector Map Information...")))
			{
				OGRDataset->DestroyDataSource(OGRDataset);
				return false;
			}
	}	
	GLdouble minX, minY, maxX, maxY;
	minX = minY = std::numeric_limits<float>::max();
	maxX = maxY = -std::numeric_limits<float>::max();

	// Retrieve features from the dataset
	OGRFeature *poFeature;
	poLayer->ResetReading();
	int numFeatures = poLayer->GetFeatureCount();
	int count=0;
    //Log::Inst().Write("Loading shapefile with the following meta data:");

	while ( ( poFeature = poLayer->GetNextFeature() ) != NULL )
	{
		/////////////////////////////////////////////////
		// PHASE 1: Retrieve METADATA from the dataset //
		/////////////////////////////////////////////////
		OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
		int iField;

		for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
		{
			OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

			//if( poFieldDefn->GetType() == OFTInteger )
			//    printf( "%d,", poFeature->GetFieldAsInteger( iField ) );
			//else if( poFieldDefn->GetType() == OFTReal )
			//    printf( "%.3f,", poFeature->GetFieldAsDouble(iField) );
			//else if( poFieldDefn->GetType() == OFTString )
			//    printf( "%s,", poFeature->GetFieldAsString(iField) );
			//else
			//    printf( "%s,", poFeature->GetFieldAsString(iField) );

			//ofs << poFeature->GetFieldAsString(iField) << ",";

			std::string metaData = poFeature->GetFieldAsString(iField);
			// do something with the meta data...
			//Log::Inst().Write(metaData);
		}
		count++;
		if(progressDlg)
		{
			if(!progressDlg->Update(int(50 + (float(count)/numFeatures)*50)))
				return false;
		}

		///////////////////////////////////////////////////
		// PHASE 2: Retrieve GEOMETRIES from the dataset //
		///////////////////////////////////////////////////
		OGRGeometry *poGeometry;
		poGeometry = poFeature->GetGeometryRef();

		// Move to the next feature in the set if no geometry present
		if( poGeometry == NULL )
		{
			OGRFeature::DestroyFeature( poFeature );
			continue;
		}

		OGRwkbGeometryType whatisit = poGeometry->getGeometryType();

		// Handle POINTS
		if ( wkbFlatten( poGeometry->getGeometryType() ) == wkbPoint )
		{
			GeoVector* geoVector = new GeoVector();
			OGRPoint *poPoint = (OGRPoint *) poGeometry;

			geoVector->SetGeometryType( wkbPoint );
			geoVector->SetNumberOfPoints( 1 );
			if(needProjection)
			{
				double x,y;
				x= poPoint->getX();
				y= poPoint->getY();
				if(!poTransform->Transform(1, &x, &y))
				{
					Log::Inst().Warning("(Warning) Failed to project vector map.");
					OGRDataset->DestroyDataSource(OGRDataset);
					return false;
				}
				
				// project and store the points
				geoVector->pointX[0] = x;
				geoVector->pointY[0] = y;

				if(x < minX) minX=x;
				if(y < minY) minY=y;
				if(x > maxX) maxX=x;
				if(y > maxY) maxY=y;
			}
			else 			
			{
				geoVector->pointX[0] = poPoint->getX();
				geoVector->pointY[0] = poPoint->getY();

			}
			vectorMapController->GetVectorMapModel()->AddGeoVector( geoVector );
			
		}
		//Handle MultiPoint
		else if ( wkbFlatten( poGeometry->getGeometryType() ) == wkbMultiPoint )
		{
			OGRMultiPoint *poMultiPoint = (OGRMultiPoint *) poGeometry;

			for ( int currGeometry = 0; currGeometry < poMultiPoint->getNumGeometries(); currGeometry++ )
			{
				GeoVector* geoVector = new GeoVector();				
				OGRPoint *poPoint = ( OGRPoint* )poMultiPoint->getGeometryRef( currGeometry );			
				geoVector->SetGeometryType( wkbPoint );
				geoVector->SetNumberOfPoints( 1 );
				if(needProjection)
				{
					double x,y;
					x= poPoint->getX();
					y= poPoint->getY();
					if(!poTransform->Transform(1, &x, &y))
					{
						Log::Inst().Warning("(Warning) Failed to project vector map.");
						OGRDataset->DestroyDataSource(OGRDataset);
						return false;
					}
					
					// project and store the points
					geoVector->pointX[0] = x;
					geoVector->pointY[0] = y;

					if(x < minX) minX=x;
					if(y < minY) minY=y;
					if(x > maxX) maxX=x;
					if(y > maxY) maxY=y;
				}
				else 			
				{
					geoVector->pointX[0] = poPoint->getX();
					geoVector->pointY[0] = poPoint->getY();

				}
				vectorMapController->GetVectorMapModel()->AddGeoVector( geoVector );
			}
			
		}

		//Handle Polylines
		else if ( wkbFlatten( poGeometry->getGeometryType() ) == wkbLineString )
		{
			GeoVector* geoVector = new GeoVector();
			OGRLineString  *poLine = (OGRLineString  *) poGeometry;

			geoVector->SetGeometryType( wkbLineString );
			geoVector->SetNumberOfPoints( poLine->getNumPoints() );

			// Convert and store the points

			for ( int currentPoint = 0; currentPoint < poLine->getNumPoints(); currentPoint++ )
			{
				// Convert and store the points

				if(needProjection)
				{
					double x,y;
					x= poLine->getX( currentPoint );
					y= poLine->getY( currentPoint );
					if(!poTransform->Transform(1, &x, &y))
					{
						Log::Inst().Warning("(Warning) Failed to project vector map.");
						OGRDataset->DestroyDataSource(OGRDataset);
						return false;
					}
				
					// project and store the points
					geoVector->pointX[currentPoint] = x;
					geoVector->pointY[currentPoint] = y;

					if(x < minX) minX=x;
					if(y < minY) minY=y;
					if(x > maxX) maxX=x;
					if(y > maxY) maxY=y;
				}
				else			
				{
					geoVector->pointX[currentPoint] = poLine->getX( currentPoint );
					geoVector->pointY[currentPoint] = poLine->getY( currentPoint );

				}

			}

			vectorMapController->GetVectorMapModel()->AddGeoVector( geoVector );			
		}
		
		// Handle MultiPolyLine
		else if ( wkbFlatten( poGeometry->getGeometryType() ) == wkbMultiLineString )
		{
			OGRMultiLineString *poMultiLine = (OGRMultiLineString *) poGeometry;

			for ( int currGeometry = 0; currGeometry < poMultiLine->getNumGeometries(); currGeometry++ )
			{
				GeoVector* geoVector = new GeoVector();
				
				OGRLineString *poLine = ( OGRLineString* )poMultiLine->getGeometryRef( currGeometry );

				geoVector->SetGeometryType( wkbLineString );
				geoVector->SetNumberOfPoints( poLine->getNumPoints() );				

				for ( int currentPoint = 0; currentPoint < poLine ->getNumPoints(); currentPoint++ )
				{

					 if(needProjection)
					{
						double x,y;
						x= poLine->getX( currentPoint );
						y= poLine->getY( currentPoint );
						if(!poTransform->Transform(1, &x, &y))
						{
							Log::Inst().Warning("(Warning) Failed to project vector map.");
							OGRDataset->DestroyDataSource(OGRDataset);
							return false;
						}
				
					// project and store the points
						geoVector->pointX[currentPoint] = x;
						geoVector->pointY[currentPoint] = y;

						if(x < minX) minX=x;
						if(y < minY) minY=y;
						if(x > maxX) maxX=x;
						if(y > maxY) maxY=y;
					}
					else			
					{
						geoVector->pointX[currentPoint] = poLine->getX( currentPoint );
						geoVector->pointY[currentPoint] = poLine->getY( currentPoint );

					}				
					
				}
				vectorMapController->GetVectorMapModel()->AddGeoVector( geoVector );				
				
			}
		}

		// Handle POLYGONS
		else if ( wkbFlatten( poGeometry->getGeometryType() ) == wkbPolygon )
		{
			GeoVector* geoVector = new GeoVector();

			OGRPolygon    *poPolygon    = ( OGRPolygon* )poGeometry;
			OGRLinearRing *poLinearRing = poPolygon->getExteriorRing();

			geoVector->SetGeometryType( wkbLinearRing );
			geoVector->SetNumberOfPoints( poLinearRing->getNumPoints() );

			for ( int currentPoint = 0; currentPoint < poLinearRing->getNumPoints(); currentPoint++ )
			{
				if(needProjection)
				{
					double x,y;
					x= poLinearRing->getX( currentPoint );
					y= poLinearRing->getY( currentPoint );
					if(!poTransform->Transform(1, &x, &y))
					{
						Log::Inst().Warning("(Warning) Failed to project vector map.");
						OGRDataset->DestroyDataSource(OGRDataset);
						return false;
					}
				
					// project and store the points
					geoVector->pointX[currentPoint] = x;
					geoVector->pointY[currentPoint] = y;

					if(x < minX) minX=x;
					if(y < minY) minY=y;
					if(x > maxX) maxX=x;
					if(y > maxY) maxY=y;
				}
				else 			
				{
					geoVector->pointX[currentPoint] = poLinearRing->getX( currentPoint );
					geoVector->pointY[currentPoint] = poLinearRing->getY( currentPoint );

				}
			}

			vectorMapController->GetVectorMapModel()->AddGeoVector( geoVector );			
		}

		// Handle MULTIPOLYGONS
		else if ( wkbFlatten( poGeometry->getGeometryType() ) == wkbMultiPolygon )
		{
			OGRMultiPolygon *poMultiPolygon = (OGRMultiPolygon *) poGeometry;

			for ( int currGeometry = 0; currGeometry < poMultiPolygon->getNumGeometries(); currGeometry++ )
			{
				GeoVector* geoVector = new GeoVector();

				// OGRPolygon http://www.gdal.org/ogr/classOGRPolygon.html
				OGRPolygon *poPolygon = ( OGRPolygon* )poMultiPolygon->getGeometryRef( currGeometry );

				// Retrieve the EXTERNAL ring of the multipolygon
				OGRLinearRing *poLinearRing = poPolygon->getExteriorRing();

				geoVector->SetGeometryType( wkbLinearRing );
				geoVector->SetNumberOfPoints( poLinearRing->getNumPoints() );

				for ( int currentPoint = 0; currentPoint < poLinearRing->getNumPoints(); currentPoint++ )
				{

					 if(needProjection)
					{
						double x,y;
						x= poLinearRing->getX( currentPoint );
						y= poLinearRing->getY( currentPoint );
						if(!poTransform->Transform(1, &x, &y))
						{
							Log::Inst().Warning("(Warning) Failed to project vector map.");
							OGRDataset->DestroyDataSource(OGRDataset);
							return false;
						}
				
					// project and store the points
						geoVector->pointX[currentPoint] = x;
						geoVector->pointY[currentPoint] = y;

						if(x < minX) minX=x;
						if(y < minY) minY=y;
						if(x > maxX) maxX=x;
						if(y > maxY) maxY=y;
					}
					else			
					{
						geoVector->pointX[currentPoint] = poLinearRing->getX( currentPoint );
						geoVector->pointY[currentPoint] = poLinearRing->getY( currentPoint );

					}				
					
				}
				vectorMapController->GetVectorMapModel()->AddGeoVector( geoVector );				
				
				// Retrieve all the INTERNAL rings of the multipolygon
				for ( int currentRing = 0; currentRing < poPolygon->getNumInteriorRings(); currentRing++ )
				{
					GeoVector* geoVector2 = new GeoVector();

					poLinearRing = poPolygon->getInteriorRing( currentRing );

					geoVector2->SetGeometryType( wkbLinearRing );
					geoVector2->SetNumberOfPoints( poLinearRing->getNumPoints() );

					for ( int currentPoint = 0; currentPoint < poLinearRing->getNumPoints(); currentPoint++ )
					{
						
						 if(needProjection)
						{
							double x,y;
							x= poLinearRing->getX( currentPoint );
							y= poLinearRing->getY( currentPoint );
							if(!poTransform->Transform(1, &x, &y))
							{
								Log::Inst().Warning("(Warning) Failed to project vector map.");
								OGRDataset->DestroyDataSource(OGRDataset);
								return false;
							}
				
						// project and store the points
							geoVector2->pointX[currentPoint] = x;
							geoVector2->pointY[currentPoint] = y;

							if(x < minX) minX=x;
							if(y < minY) minY=y;
							if(x > maxX) maxX=x;
							if(y > maxY) maxY=y;
						}
						else 		
						{
							geoVector2->pointX[currentPoint] = poLinearRing->getX( currentPoint );
							geoVector2->pointY[currentPoint] = poLinearRing->getY( currentPoint );

						}		
					}

					vectorMapController->GetVectorMapModel()->AddGeoVector( geoVector2 );
				}
			}
		}


		// Report a warning message for unhandled geometries
		else
		{
			Log::Inst().Warning("(Warning) Could not load vector data: unsupported geometry.");
		}
		OGRFeature::DestroyFeature( poFeature );
	}

	if (float(minX) == float(maxX) && float(minY) == float(maxY))
	{
		Log::Inst().Warning("(Warning) Failed to project vector map.");
		OGRDataset->DestroyDataSource(OGRDataset);
		return false;
	}

	if(needProjection)
	{
		vectorMapModel->SetVectorBoundary_MinX(minX);
		vectorMapModel->SetVectorBoundary_MaxX(maxX);
		vectorMapModel->SetVectorBoundary_MinY(minY);
		vectorMapModel->SetVectorBoundary_MaxY(maxY);
	}	
	
	if(!SetupVectorScaling(vectorMapModel,progressDlg))
	{ 
		OGRDataset->DestroyDataSource(OGRDataset);
		return false;
	}

	VectorMetaDataInfo(OGRDataset, studyController, vectorMapController);
	OGRDataSource::DestroyDataSource( OGRDataset );
	return true;
}



void ShpReader::VectorMetaDataInfo(OGRDataSource* OGRDataset, StudyControllerPtr studyController, VectorMapControllerPtr vectorMapController)
{	
	vFileMetaData* vHeader = vectorMapController->GetVectorMapModel()->GetVectorMetaData();
    OGRLayer *poLayer = OGRDataset->GetLayer( 0 );	
	char *originalWkt = NULL;	
	char *destinationWkt = NULL;

	OGREnvelope psEnvelope;
	poLayer->GetExtent( &psEnvelope );
	vHeader->originalExtents.x = psEnvelope.MinX;
	vHeader->originalExtents.dx = psEnvelope.MaxX;
	vHeader->originalExtents.y = psEnvelope.MinY; 
	vHeader->originalExtents.dy = psEnvelope.MaxY;
	Log::Inst().Write("Original Extents: ");
	Log::Inst().Write(_T("Lower, Left (x,y): ") +wxString(StringTools::ToStringW(vHeader->originalExtents.x, 2).c_str()) + _T(", ") 
		+ wxString(StringTools::ToStringW(vHeader->originalExtents.y, 2).c_str()));	
	Log::Inst().Write(_T("Upper, Right (x,y): ") +wxString(StringTools::ToStringW(vHeader->originalExtents.dx, 2).c_str()) + _T(", ") 
		+ wxString(StringTools::ToStringW(vHeader->originalExtents.dy, 2).c_str()));		

	vHeader->numFeatures= poLayer->GetFeatureCount();
	Log::Inst().Write("Number of Features: " + StringTools::ToString(vHeader->numFeatures));
	
	std::string type;
	OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
	if ( wkbFlatten( poFDefn->GetGeomType() ) == wkbPoint || wkbFlatten( poFDefn->GetGeomType() ) == wkbMultiPoint )
		 type="Point";
	else if ( wkbFlatten(poFDefn->GetGeomType() ) == wkbLineString || wkbFlatten(poFDefn->GetGeomType() ) == wkbMultiLineString )
		type="Polyline";
	else if ( wkbFlatten( poFDefn->GetGeomType() ) == wkbPolygon || wkbFlatten( poFDefn->GetGeomType() ) == wkbMultiPolygon )
		 type="Polygon";  	
	else
		type="Unknown";

	 vHeader->geometryType=type;
	 Log::Inst().Write("Geometry Type: "+ type);

	OGRSpatialReference* originalShpSR = poLayer->GetSpatialRef();	
	if (originalShpSR != NULL)
	{
		originalShpSR->exportToWkt(&originalWkt);	
		vHeader->originalProjection = std::string(originalWkt);
	}
	Log::Inst().Write("Original Projection: " + vHeader->originalProjection);
	Log::Inst().Write("");
	ProjectionToolPtr projTool = studyController->GetProjectionTool();
	if (projTool != NULL)
	{
		OGRSpatialReference *destinationCS= projTool->GetTargetCS ();
		destinationCS->exportToWkt(&destinationWkt);
		vHeader->destinationProjection = std::string(destinationWkt);
	}

	vHeader->currentExtents.x = vectorMapController->GetVectorMapModel()->GetVectorBoundary_MinX();
	vHeader->currentExtents.y = vectorMapController->GetVectorMapModel()->GetVectorBoundary_MinY();	

	vHeader->currentExtents.dx = vectorMapController->GetVectorMapModel()->GetVectorBoundary_MaxX();
	vHeader->currentExtents.dy = vectorMapController->GetVectorMapModel()->GetVectorBoundary_MaxY();
	//Log::Inst().Write("Upper, Right (x,y): " +
	//	StringTools::ToString(vHeader->currentExtents.dx) + ", " +
		//StringTools::ToString(vHeader->currentExtents.dy));
	
}



bool ShpReader::SetupVectorProjection(OGRDataSource* OGRDataset, StudyControllerPtr studyController, OGRLayer* poLayer, VectorMapControllerPtr vectorMapController)
{
	//If there exists a map layer, the vector file will be projected based on the projection of the map layer
	if ( App::Inst().GetLayerTreeController()->GetNumMapLayers() > 0 )
	{
		ProjectionToolPtr projTool = studyController->GetProjectionTool();
		OGRSpatialReference* currentShpSR = poLayer->GetSpatialRef();
		if (!studyController->IsProjectData() && !studyController->IsGeographic())
		{
			needProjection=false;			
			Log::Inst().Write("The projection information for this study is being ignored.");
		}
		else
		{
	
			if(currentShpSR != NULL) 
			{
				if(currentShpSR->IsGeographic())
				{
					Log::Inst().Write("Loading vector map with geographic coordinates.");
					studyController->SetProjectData(true);
					studyController->SetGeographic(true);
					needProjection=true;
					poTransform = studyController->GetProjectionTool();
				}
				else if(currentShpSR->IsProjected())
				{
					studyController->SetProjectData(true);
					studyController->SetGeographic(false);
					needProjection=true;
					Log::Inst().Write("Loading vector map with projected coordinates.");
					ProjectionToolPtr projTool = studyController->GetProjectionTool();
					//OGRSpatialReference* currentLatLong = currentShpSR->CloneGeogCS();
					OGRSpatialReference *targetCS= projTool->GetTargetCS ();
					if(targetCS != NULL && !currentShpSR->IsSame(targetCS)) 
						poTransform.reset(OGRCreateCoordinateTransformation(currentShpSR, targetCS));
					
					else
						needProjection=false;

				}
				else
				{
				// The user should probably be made aware of this warning.
					Log::Inst().Warning("(Warning) Unknown type of coordinate system.");
					return false;
				}
			}		
	

			else 
			{
				//studyController->SetProjectData(false);
				//studyController->SetGeographic(false);
				needProjection=false;		
				
				Log::Inst().Write("Coordinate system information is not available for this map.");
				Log::Inst().Write("As a result, the projection information for this study is being ignored.");
				Log::Inst().Write("");
			
			}

		return true;
		}
	}
	// App::Inst().GetLayerTreeController()->GetNumMapLayers() ==0
	else
	{
		OGRSpatialReference* currentShpSR = poLayer->GetSpatialRef();
	//OGRSpatialReference oSource;
		if(currentShpSR != NULL) 
		{
			if(currentShpSR->IsGeographic())
			{
			// lat/lon coordinates are using the geographic projection (aka, plate carrée) 
				Log::Inst().Write("Loading vector map with lat/long coordinates.");
				studyController->SetProjectData(true);
				studyController->SetGeographic(true);
				needProjection=true;

			// determine centre of map
				VectorMapModelPtr vectorMapModel = vectorMapController->GetVectorMapModel();
				float vectorMinX= vectorMapModel->GetVectorBoundary_MinX();
				float vectorMinY = vectorMapModel->GetVectorBoundary_MinY();
				float vectorMaxX = vectorMapModel->GetVectorBoundary_MaxX();
				float vectorMaxY = vectorMapModel->GetVectorBoundary_MaxY();
			
				studyController->SetCentreLongitude(float(vectorMinX + fabs(vectorMinX - vectorMaxX) / 2.0));			
				studyController->SetCentreLatitude(float(vectorMinY + fabs(vectorMinY - vectorMaxY) / 2.0));

				studyController->SetFirstStandardParallel(vectorMinY);
				studyController->SetSecondStandardParallel(vectorMaxY);

				studyController->CalculateProjectionTool(currentShpSR);
				poTransform = studyController->GetProjectionTool();

			}
			else if(currentShpSR->IsProjected())
			{
				studyController->SetProjectData(true);
				studyController->SetGeographic(false);
				Log::Inst().Write("Loading vector map with projected coordinates.");
				studyController->CalculateProjectionTool(currentShpSR);
				poTransform = studyController->GetProjectionTool();
				needProjection=false;
			}
			else
			{
				// The user should probably be made aware of this warning.
				Log::Inst().Warning("(Warning) Unknown type of coordinate system.");
				return false;
				}
		}
		else 
		{
			studyController->SetProjectData(false);
			studyController->SetGeographic(false);
			needProjection=false;
		
			
			Log::Inst().Write("Coordinate system information is not available for this map.");
			Log::Inst().Write("As a result, the projection information for this study is being ignored.");
			Log::Inst().Write("To overlaid a Vector map on top of this map, the vector map must be specified in the same coordinate system as this map");
			Log::Inst().Write("");
		
		}
		}

	return true;
}


bool ShpReader::SetupVectorScaling(VectorMapModelPtr vectorMapModel, ProgressDlgPtr progressDlg)
{
	double minX,minY,maxX,maxY;
	double aspect,width,height,scaleFactor;
	if ( App::Inst().GetLayerTreeController()->GetNumMapLayers() >0 )
	{
		MapControllerPtr mapController= App::Inst().GetLayerTreeController()->GetLayerTreeModel()->GetStudy(0)->GetMapLayer(0)->GetMapController();
		FileHeader* header=mapController->GetMapModel()->GetHeader();
		minX=header->projExtents.x;
		maxX=header->projExtents.dx;
		minY=header->projExtents.y;
		maxY=header->projExtents.dy;
		aspect = header->projExtents.Height() / header->projExtents.Width();		
		width = 2.0;
		height = 2.0*aspect;
		scaleFactor = 2.0 / header->projExtents.Width();
	}
	else{
		minX=vectorMapModel->GetVectorBoundary_MinX();
		minY=vectorMapModel->GetVectorBoundary_MinY();
		maxX=vectorMapModel->GetVectorBoundary_MaxX();
		maxY=vectorMapModel->GetVectorBoundary_MaxY();		
		aspect = fabs((maxY-minY) / (maxX-minX));
		width = 2.0;
		height = 2.0*aspect;
		scaleFactor = 2.0 / fabs(maxX-minX);
	}

	double scaledextent_minX= -1.0f + 2*((vectorMapModel->GetVectorBoundary_MinX() - minX) / fabs(maxX-minX));
	double scaledextent_minY = (1.0f + -2*((vectorMapModel->GetVectorBoundary_MinY() - minY) / fabs(maxY-minY)))*aspect;
	double scaledextent_maxX= -1.0f + 2*((vectorMapModel->GetVectorBoundary_MaxX() - minX) / fabs(maxX-minX));
	double scaledextent_maxY = (1.0f + -2*((vectorMapModel->GetVectorBoundary_MaxY() - minY) / fabs(maxY-minY)))*aspect;

	vectorMapModel->SetHeight(height);
	vectorMapModel->SetVectorBoundary_ScaledMinX(scaledextent_minX);
	vectorMapModel->SetVectorBoundary_ScaledMinY(scaledextent_minY);
	vectorMapModel->SetVectorBoundary_ScaledMaxX(scaledextent_maxX);
	vectorMapModel->SetVectorBoundary_ScaledMaxY(scaledextent_maxY);
	

	for ( unsigned int currGeometry = 0; currGeometry < vectorMapModel->GetNumberOfGeometries(); currGeometry++ )
	{
		GeoVector* GeoVector = vectorMapModel->GetGeoVector( currGeometry );

		for ( unsigned int currPoint = 0; currPoint < GeoVector->GetNumberOfPoints(); currPoint++ )
		{
			GeoVector->pointX[currPoint] = -1.0f + 2*((GeoVector->pointX[currPoint] - minX) / fabs(maxX-minX));
			GeoVector->pointY[currPoint] = (1.0f + -2*((GeoVector->pointY[currPoint] - minY) / fabs(maxY-minY)))*aspect;
		}

		if(progressDlg)
		{
			if(!progressDlg->Update(int(50 + (float(currGeometry)/vectorMapModel->GetNumberOfGeometries())*50)))
				return false;
		}
	}
	
	
	

	return true;
}




