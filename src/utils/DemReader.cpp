//=======================================================================
// Author: Donovan Parks & Somayyeh Zangooei
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

#include "../utils/DemReader.hpp"
#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/Colour.hpp"

#include "../core/App.hpp"
#include "../core/StudyController.hpp"
#include "../core/MapController.hpp"
#include "../core/MapModel.hpp"
#include "../core/MapTexture.hpp"
#include "../core/VectorMapLayer.hpp"
#include "../core/VectorMapModel.hpp"
#include "../core/VectorMapController.hpp"
#include "../gui/ProgressDlg.hpp"

using namespace GenGIS;

bool DemReader::Open(std::wstring fullPath, StudyControllerPtr studyController, MapControllerPtr mapController, ProgressDlgPtr progressDlg) 
{    
	GDALAllRegister();
	bool bElevationMap;
	uint numBands;
	double adfGeoTransform[6];
	GDALDataset* gdalDataset;
	int nBlockXSize, nBlockYSize;
	MapModelPtr mapModel = mapController->GetMapModel();
	FileHeader* header = mapModel->GetHeader();
	GDALRasterBand* gdalBand;

	if(App::Inst().GetLayerTreeController()->GetIsBlankRaster())
	{
		LayerTreeControllerPtr ltc = App::Inst().GetLayerTreeController();
		VectorMapLayerPtr vectorMapLayer =ltc->GetVectorMapLayer(0);
	    VectorMapModelPtr vectorMapModel= vectorMapLayer->GetVectorMapController()->GetVectorMapModel();			
		double vectorMinX = vectorMapModel->GetVectorBoundary_MinX();
		double vectorMinY = vectorMapModel->GetVectorBoundary_MinY();
		double vectorMaxX = vectorMapModel->GetVectorBoundary_MaxX();
		double vectorMaxY = vectorMapModel->GetVectorBoundary_MaxY();
		
		double bRaster_width = 1000;
		double bRaster_height=	1000;	

		double x_res = (vectorMaxX - vectorMinX) / bRaster_width;
		double y_res = (vectorMaxY - vectorMinY) / bRaster_height;
		numBands = 3;
		bElevationMap = false;
		header->nCols = 1000;
		header->nRows = 1000;
		header->noDataValue=0.0;
		//adfGeoTransform[6]= {vectorMinX, x_res, 0.0,  vectorMaxY, 0.0, -y_res};
		adfGeoTransform[0]= vectorMinX;
		adfGeoTransform[1]= x_res;
		adfGeoTransform[2]= 0.0;
		adfGeoTransform[3]= vectorMaxY;
		adfGeoTransform[4] = 0.0;
		adfGeoTransform[5]= -y_res;
	}	
		
	else
	{
		std::string tempStr(fullPath.begin(), fullPath.end());
		gdalDataset = (GDALDataset*) GDALOpen( tempStr.c_str(), GA_ReadOnly );
		if(gdalDataset == NULL) 
		{
			Log::Inst().Warning("(Warning) Failed to open file at " + tempStr + ".");
			return false;	
		}
	
	// determine type of file based on number of raster bands		
		numBands = gdalDataset->GetRasterCount();
		if(numBands == 1)
			bElevationMap = true;
		else if(numBands == 3 || numBands == 4)
			bElevationMap = false;
		else
		{
			Log::Inst().Warning("(Warning) File type not supported. Please contact the GenGIS to request support for the file format.");
		}
		CPLErr err = gdalDataset->GetGeoTransform( adfGeoTransform );

		// check if we have a map without any transformation information
		// in which case assume we have a custom image file where y should
		// be in the south (down) direction
		if(err == CE_Failure)
			adfGeoTransform[5] = -1;

		gdalBand = gdalDataset->GetRasterBand( 1 );		
		gdalBand->GetBlockSize( &nBlockXSize, &nBlockYSize );

		// get original data dimensions		
		header->nCols = gdalBand->GetXSize();
		header->nRows = gdalBand->GetYSize();
		header->noDataValue = gdalBand->GetNoDataValue();
		// Setup desired projection 
		if(!SetupProjection(gdalDataset, studyController, adfGeoTransform, header->nCols, header->nRows, bElevationMap))
		{
			GDALClose(gdalDataset);
			return false;
		}
	}
	if(progressDlg)
	{
		if(!App::Inst().GetLayerTreeController()->GetIsBlankRaster())
		{
			if(!progressDlg->Update(0, _T("Reading map file...")))
			{
				GDALClose(gdalDataset);
				return false;
			}
		}
		else
		{
			if(!progressDlg->Update(0, _T("Initializing the Visualization...")))
			{
				GDALClose(gdalDataset);
				return false;
			}
		}
	}
	// check whether it's too large to handle all the data
	float* elevations = NULL;
	if(bElevationMap && !App::Inst().GetLayerTreeController()->GetIsBlankRaster())
	{
		try
		{
			elevations = new float[gdalBand->GetXSize()* gdalBand->GetYSize()];
		}
		catch(std::bad_alloc&)
		{
			Log::Inst().Warning("(Warning) Insufficent memory for elevation map."); 

			GDALClose(gdalDataset);
			return false;
		}

		gdalBand->RasterIO( GF_Read, 0, 0, gdalBand->GetXSize(), gdalBand->GetYSize(), 
			elevations, gdalBand->GetXSize(), gdalBand->GetYSize(), 
			GDT_Float32, 
			0, 0 );
	}

	// project all map model cells
	Point3D* grid;
	try
	{
		grid = new Point3D[header->nCols*header->nRows];
	}
	catch(std::bad_alloc&)
	{
		Log::Inst().Warning("(Warning) Insufficent memory to load map."); 

		GDALClose(gdalDataset);
		delete[] elevations;
		return false;
	}

	// populate grid with projected points and find extents of projected map
	double xOffset = adfGeoTransform[0];
	double zOffset = adfGeoTransform[3];
	int index = 0;
	double x, z;
	
	header->projExtents.x = header->projExtents.y = std::numeric_limits<float>::max();
	header->projExtents.dx = header->projExtents.dy = -std::numeric_limits<float>::max();
	
	ProjectionToolPtr projTool = studyController->GetProjectionTool();
	
	for(int m = 0; m < header->nRows ; ++m) 
	{
		xOffset = adfGeoTransform[0];
		for(int n = 0; n < header->nCols; ++n) 
		{
			x = xOffset;
			z = zOffset;
			if(studyController->IsProjectData() && studyController->IsGeographic() && !App::Inst().GetLayerTreeController()->GetIsBlankRaster())
			{			
				double elevation = (double)elevations[index];				
				if(!projTool->Transform(1, &x, &z, &elevation))
				{
					Log::Inst().Warning("(Warning) Failed to project map.");
					GDALClose(gdalDataset);
					delete[] elevations;
					return false;
				}
				
			}

			xOffset += adfGeoTransform[1];
			if(App::Inst().GetLayerTreeController()->GetIsBlankRaster())
			{
				double X = x;
				double Z = z;

				grid[index].x = X;
				grid[index].z = Z;
				index++;	
				if(X < header->projExtents.x) header->projExtents.x = X;
				if(Z < header->projExtents.y) header->projExtents.y = Z;
				if(X > header->projExtents.dx) header->projExtents.dx = X;
				if(Z > header->projExtents.dy) header->projExtents.dy = Z;
			}
			else
			{
				float X = (float)x;
				float Z = (float)z;

				grid[index].x = X;
				grid[index].z = Z;
				index++;
				if(X < header->projExtents.x) header->projExtents.x = X;
				if(Z < header->projExtents.y) header->projExtents.y = Z;
				if(X > header->projExtents.dx) header->projExtents.dx = X;
				if(Z > header->projExtents.dy) header->projExtents.dy = Z;
			}
			
		}

		zOffset += adfGeoTransform[5];

		if(progressDlg)
		{
			if(!progressDlg->Update(int((float(m)/header->nRows)*50)))
			{
				GDALClose(gdalDataset);
				if(elevations != NULL)
					delete[] elevations;

				return false;
			}
		}
	}
		
	if(App::Inst().GetLayerTreeController()->GetIsBlankRaster())
	{
		
		LayerTreeControllerPtr ltc = App::Inst().GetLayerTreeController();
		VectorMapLayerPtr vectorMapLayer =ltc->GetVectorMapLayer(0);
	    VectorMapModelPtr vectorMapModel= vectorMapLayer->GetVectorMapController()->GetVectorMapModel();			
		header->projExtents.x = vectorMapModel->GetVectorBoundary_MinX();
		header->projExtents.dx = vectorMapModel->GetVectorBoundary_MaxX();
		header->projExtents.y = vectorMapModel->GetVectorBoundary_MinY();
		header->projExtents.dy = vectorMapModel->GetVectorBoundary_MaxY();
		
	}
	// transform all points into unit grid space	
	float minElevation = std::numeric_limits<float>::max();
	float maxElevation = -std::numeric_limits<float>::max();
		
	if(!TransformToGridSpace(grid, header, elevations, bElevationMap, minElevation, maxElevation, progressDlg))
	{ 
		GDALClose(gdalDataset);
		if(elevations != NULL)
			delete[] elevations;

		return false;
	}

	// setup map model
	mapModel->SetMinElevationGridSpace(minElevation);
	mapModel->SetMaxElevationGridSpace(maxElevation);

	mapModel->SetMinElevation(minElevation/header->scaleFactor);
	mapModel->SetMaxElevation(maxElevation/header->scaleFactor);

	
	mapModel->SetElevationMap(bElevationMap);	
	mapModel->SetGrid(grid);
	// report information in file to user
	if (!App::Inst().GetLayerTreeController()->GetIsBlankRaster())
		MetaDataInfo(gdalDataset, mapController);
	else
		MetaDataInfoForBlankRaster(mapController, adfGeoTransform);


	// build texture that will be mapped onto terrain
	if(!App::Inst().GetLayerTreeController()->GetIsBlankRaster()){
		if(!BuildTerrainTexture(gdalDataset, mapController, progressDlg))
		{
			GDALClose(gdalDataset);	
			if(elevations != NULL)
				delete[] elevations;

			return false;
		}
		// will free memory allocated to any GDALRasterBand objects
		GDALClose(gdalDataset);
	}

	// will free memory allocated to any GDALRasterBand objects		
	if(elevations != NULL)
		delete[] elevations;

	return true;
}

bool DemReader::TransformToGridSpace(Point3D*grid, FileHeader* header, float* elevations, bool bElevationMap, 
									 float& minElevation, float& maxElevation, ProgressDlgPtr progressDlg)
{
	// model (grid) space for map should go from x = -1 to 1, and z = -aspect to aspect
	double aspect = header->projExtents.Height() / header->projExtents.Width();
	header->cellSizeX = 2.0 / header->nCols;
	header->cellSizeZ = 2.0*aspect / header->nRows;
	header->width = 2.0;
	header->height = 2.0*aspect;
	header->scaleFactor = 2.0 / header->projExtents.Width();

	// transform all points into grid space
	uint index = 0;
	minElevation = std::numeric_limits<float>::max();
	maxElevation = -std::numeric_limits<float>::max();
	for(int m = 0; m < header->nRows ; m++) 
	{
		for(int n = 0; n < header->nCols; n++) 
		{
			float X = grid[index].x;
			float Z = grid[index].z;

			grid[index].x = -1.0f + 2*((X - header->projExtents.x) / header->projExtents.Width());
			grid[index].z = (1.0f + -2*((Z - header->projExtents.y) / header->projExtents.Height()))*aspect;

			if(bElevationMap)
			{
				double elevation = elevations[index];
				if(elevation == header->noDataValue)
				{
					grid[index].y = 0;
					if(grid[index].y < minElevation)
						minElevation = grid[index].y;
				}
				else
				{
					grid[index].y = elevation * header->scaleFactor;

					if(grid[index].y > maxElevation)
						maxElevation =  grid[index].y;
					else if(grid[index].y < minElevation)
						minElevation = grid[index].y;
				}	
			}
			else
			{
				grid[index].y = 0;
				minElevation = maxElevation = 0;
			}

			index++;
		}

		if(progressDlg)
		{
			if(!progressDlg->Update(int(50 + (float(m)/header->nRows)*50)))
				return false;
		}
	}

	return true;
}

bool DemReader::SetupProjection(GDALDataset* gdalDataset, StudyControllerPtr studyController, double* adfGeoTransform, uint nCols, uint nRows, bool bElevationMap)
{
	// getting the string about projection system for this dataset.
	const char* currentWkt = gdalDataset->GetProjectionRef();

	OGRSpatialReference oSource;
	if((currentWkt != NULL && oSource.importFromWkt( (char**)&currentWkt ) != OGRERR_CORRUPT_DATA)) 
	{
		if(oSource.IsGeographic())
		{
			// lat/lon coordinates are using the geographic projection (aka, plate carrée) 
			if(!App::Inst().GetLayerTreeController()->GetIsBlankRaster())
				Log::Inst().Write("Loading map with lat/long coordinates.");

			studyController->SetProjectData(true);
			studyController->SetGeographic(true);
			studyController->SetUsingProjection(true);
			studyController->SetUsingGeographic(true);

			// determine centre of map
			double longStart = adfGeoTransform[0];
			double longEnd = adfGeoTransform[0] + nCols*adfGeoTransform[1];
			studyController->SetCentreLongitude(float(longStart + fabs(longStart - longEnd) / 2.0));

			double latStart = adfGeoTransform[3] + nRows*adfGeoTransform[5];
			double latEnd = adfGeoTransform[3];
			studyController->SetCentreLatitude(float(latStart + fabs(latStart - latEnd) / 2.0));

			studyController->SetFirstStandardParallel(latStart);
			studyController->SetSecondStandardParallel(latEnd);

			studyController->CalculateProjectionTool(&oSource);
		}
		else if(oSource.IsProjected())
		{
			studyController->SetProjectData(true);
			studyController->SetGeographic(false);
			studyController->SetUsingProjection(true);
			studyController->SetUsingGeographic(false);
			if (!App::Inst().GetLayerTreeController()->GetIsBlankRaster())
				Log::Inst().Write("Loading map with projected coordinates.");
			studyController->CalculateProjectionTool(&oSource);
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
		studyController->SetUsingProjection(false);
		studyController->SetUsingGeographic(false);

		if(bElevationMap)
		{
			// The user should probably be made aware of this warning.
			Log::Inst().Warning("(Warning) Well-Known text which specified coordinate system information is missing or not recognized."); 
			Log::Inst().Warning("(Warning) The projection information for this study is being ignored.");
			Log::Inst().Warning("(Warning) Location sites and leaf nodes must be specified in the same coordinate system as this map.");
			Log::Inst().Warning("");
		}
		else
		{
			// Right now we assume that any 3-channel map (i.e., non-elevational) should be projected using the 
			// the geographic projection (aka, plate carrée)
			studyController->SetUsingProjection(false);
			studyController->SetUsingGeographic(true);
			Log::Inst().Write("Coordinate system information is not available for this map.");
			Log::Inst().Write("As a result, the projection information for this study is being ignored.");
			Log::Inst().Write("");
		}
	}

	return true;
}

void DemReader::MetaDataInfo(GDALDataset* gdalDataset, MapControllerPtr mapController)
{
	FileMetaData* header = mapController->GetMapModel()->GetMetaData();

	header->driverDesc = gdalDataset->GetDriver()->GetDescription();
	header->driverMetaData = gdalDataset->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME);
	Log::Inst().Write("Driver: " + header->driverDesc + " \\ " + header->driverMetaData);

	header->xSize = gdalDataset->GetRasterXSize();
	header->ySize = gdalDataset->GetRasterYSize();
	header->bands = gdalDataset->GetRasterCount();
	Log::Inst().Write("Size (x,y): " + 
		StringTools::ToString(header->xSize) + ", " +
		StringTools::ToString(header->ySize));
	Log::Inst().Write("Bands: " + StringTools::ToString(header->bands));

	header->projection = std::string(gdalDataset->GetProjectionRef());
	Log::Inst().Write("Projection: " + header->projection);

	double adfGeoTransform[6];
	gdalDataset->GetGeoTransform( adfGeoTransform );

	header->originX = adfGeoTransform[0];
	header->originY = adfGeoTransform[3];
	Log::Inst().Write("Origin: " +
		StringTools::ToString(adfGeoTransform[0]) + ", " +
		StringTools::ToString(adfGeoTransform[3]));

	header->pixelSizeX = adfGeoTransform[1];
	header->pixelSizeY = adfGeoTransform[5];
	Log::Inst().Write("Pixel Size: " +
		StringTools::ToString(adfGeoTransform[1]) + ", " +
		StringTools::ToString(adfGeoTransform[5]));

	GDALRasterBand* gdalBand = gdalDataset->GetRasterBand(1);
	int nBlockXSize, nBlockYSize;
	gdalBand->GetBlockSize(&nBlockXSize, &nBlockYSize);

	header->dataType = std::string(GDALGetDataTypeName(gdalBand->GetRasterDataType()));
	Log::Inst().Write("Block, Type: " + StringTools::ToString(nBlockXSize) + ", " +
		StringTools::ToString(nBlockYSize) + ", " +
		std::string(header->dataType));

	header->colourInterpretation = std::string(GDALGetColorInterpretationName(gdalBand->GetColorInterpretation()));	
	Log::Inst().Write("Color Interpretation: " + header->colourInterpretation);

	header->extents.x = adfGeoTransform[0];
	header->extents.y = adfGeoTransform[3] + gdalBand->GetYSize()*adfGeoTransform[5];
	Log::Inst().Write("Lower, Left (x,y): " +
		StringTools::ToString(header->extents.x) + ", " +
		StringTools::ToString(header->extents.y));

	header->extents.dx = adfGeoTransform[0]+gdalBand->GetXSize()*adfGeoTransform[1];
	header->extents.dy = adfGeoTransform[3];
	Log::Inst().Write("Upper, Right (x,y): " +
		StringTools::ToString(header->extents.dx) + ", " +
		StringTools::ToString(header->extents.dy));

	Log::Inst().Write("");
}

void DemReader::MetaDataInfoForBlankRaster(MapControllerPtr mapController,double* adfGeoTransform)
{	
	FileMetaData* header = mapController->GetMapModel()->GetMetaData();

	header->driverDesc = "";
	header->driverMetaData = "";	

	header->xSize = 1000;
	header->ySize = 1000;
	header->bands = 3;
	header->projection = "";
	header->originX = adfGeoTransform[0];
	header->originY = adfGeoTransform[3];
	header->pixelSizeX = adfGeoTransform[1];
	header->pixelSizeY = adfGeoTransform[5];

	header->dataType ="";
	header->colourInterpretation = "";	
	header->extents.x = adfGeoTransform[0];
	header->extents.y = adfGeoTransform[3] + 1000*adfGeoTransform[5];
	header->extents.dx = adfGeoTransform[0]+1000*adfGeoTransform[1];
	header->extents.dy = adfGeoTransform[3];

}

bool DemReader::BuildTerrainTexture(GDALDataset* gdalDataset, MapControllerPtr mapController, ProgressDlgPtr progressDlg)
{
	MapTexturePtr texture = mapController->GetMapModel()->GetTexture();

	// check if GDAL dataset specifies colour information for each cell
	bool bTerrain;
	uint numBands = gdalDataset->GetRasterCount();
	if(numBands == 3 || numBands == 4)
	{
		try
		{
			GDALRasterBand* gdalBandRed = gdalDataset->GetRasterBand( 1 );
			byte* red = new byte[gdalBandRed->GetXSize() * gdalBandRed->GetYSize()];
			gdalBandRed->RasterIO( GF_Read, 0, 0, gdalBandRed->GetXSize(), gdalBandRed->GetYSize(), 
				red, gdalBandRed->GetXSize(), gdalBandRed->GetYSize(), 
				GDT_Byte, 0, 0 );

			GDALRasterBand* gdalBandGreen = gdalDataset->GetRasterBand( 2 );
			byte* green = new byte[gdalBandGreen->GetXSize() * gdalBandGreen->GetYSize()];
			gdalBandGreen->RasterIO( GF_Read, 0, 0, gdalBandGreen->GetXSize(), gdalBandGreen->GetYSize(), 
				green, gdalBandGreen->GetXSize(), gdalBandGreen->GetYSize(), 
				GDT_Byte, 0, 0 );

			GDALRasterBand* gdalBandBlue = gdalDataset->GetRasterBand( 3 );
			byte* blue = new byte[gdalBandBlue->GetXSize() * gdalBandBlue->GetYSize()];
			gdalBandBlue->RasterIO( GF_Read, 0, 0, gdalBandBlue->GetXSize(), gdalBandBlue->GetYSize(), 
				blue, gdalBandBlue->GetXSize(), gdalBandBlue->GetYSize(), 
				GDT_Byte, 0, 0 );

			if(gdalBandRed->GetXSize() != gdalBandGreen->GetXSize() || gdalBandRed->GetXSize() != gdalBandBlue->GetXSize()
				|| gdalBandRed->GetYSize() != gdalBandGreen->GetYSize() || gdalBandRed->GetYSize() != gdalBandBlue->GetYSize())
			{
				Log::Inst().Error("DemReader::BuildTerrainTexture(): Failed to build terrain texture.");
				delete[] red;
				delete[] green;
				delete[] blue;

				return false;
			}

			/*
			// flip image about y-axis
			for(int r = 0; r < int(0.5*gdalBandRed->GetYSize()); ++r)
			{
				for(int c = 0; c < gdalBandRed->GetXSize(); ++c)
				{
					uint topRow = c + r*gdalBandRed->GetXSize();
					uint bottomRow = c + (gdalBandRed->GetYSize()-r-1)*gdalBandRed->GetXSize();

					byte temp = red[topRow];
					red[topRow] = red[bottomRow];
					red[bottomRow] = temp;

					temp = green[topRow];
					green[topRow] = green[bottomRow];
					green[bottomRow] = temp;

					temp = blue[topRow];
					blue[topRow] = blue[bottomRow];
					blue[bottomRow] = temp;
				}
			}
			*/

			bTerrain = texture->BuildTerrainTexture(mapController, red, green, blue, progressDlg);

			// free memory
			delete[] red;
			delete[] green;
			delete[] blue;
			red = green = blue = NULL;
		}
		catch(std::bad_alloc&)
		{
			Log::Inst().Warning("(Warning) Insufficent memory for colour map."); 
			return false;
		}
	}
	else
	{
		bTerrain = texture->BuildTerrainTexture(mapController, progressDlg);
	}

	if(!bTerrain)
	{
		Log::Inst().Error("DemReader::BuildTerrainTexture(): Failed to build terrain texture.");
		return false;
	}

	return true;
}
