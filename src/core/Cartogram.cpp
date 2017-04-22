//=======================================================================
// Author: Alexander Keddy
//
// Copyright 2015 Alexander Keddy
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
#include "../utils/StringTools.hpp"
#include "../core/Cartogram.hpp"
#include "../core/MapView.hpp"
#include "../core/MapModel.hpp"
#include "../core/MapController.hpp"
#include "../core/VectorMapModel.hpp"
#include "../core/VectorMapController.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/TreeLayer.hpp"
#include "../core/GeoTreeView.hpp"
#include "../core/PolygonModel.hpp"
#include "../core/LocationPolygons.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include<sstream>
#include <stdexcept>

extern "C" {
	#include "../utils/cart.h"
}

using namespace GenGIS;

template<class Archive> 

void Cartogram::serialize(Archive & ar, const unsigned int version)
{
	// Serialize base class (View)
//	ar & boost::serialization::base_object<View>(*this);

	// General variables

	ar & ysize;
	ar & xsize;
	ar & valFudge;
	ar & areaFudge;
	
	ar & m_measureLabel;
	ar & m_locationSetLayerIndex;
	ar & m_vectorMapIndex;
	ar & m_mapController;
	
	ar & m_resizePercent;
	ar & m_resize;
	ar & m_invert;
	ar & m_distorted;

	//	QUAGMIRE
	
	//	ar & m_originalGrid;
	/*
	FileHeader* header = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapModel()->GetHeader();
	for(int i = 0; i < (header->nRows*header->nCols) ; i++) 
	{	
		ar & m_originalGrid[i];
	}

	int load = 0;
//	ar & m_originalLocations;
	// Save Location layers 
	
	load = App::Inst().GetLayerTreeController()->GetNumLocationSetLayers();
	for( int i = 0; i < load; i++ )
	{
		LocationSetLayerPtr locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer(i);
		std::vector<LocationLayerPtr> locationLayers = locationSetLayer->GetAllActiveLocationLayers();
		for( uint l = 0; l < locationLayers.size() ; l++)
		{
			ar & m_originalLocations[i][l];
		}
		
	}

	load = App::Inst().GetLayerTreeController()->GetNumVectorMapLayers();
	for( int i = 0; i < load; i++ )
	{
		VectorMapModelPtr vMapModel = App::Inst().GetVectorMapController(i)->GetVectorMapModel();
		GeoVector* geoVector;
		for( uint g = 0; g< vMapModel->GetNumberOfGeometries(); g++ )
		{
			geoVector = vMapModel->GetGeoVector(g);
			m_originalVector[i][g] = new Point3D[geoVector->GetNumberOfPoints()];
			for( uint p = 0; p < geoVector->GetNumberOfPoints(); p++ )
			{
				ar & m_originalVector[i][g][p];
			}
		}
	}	
	*/
}
template void Cartogram::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void Cartogram::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


Cartogram::Cartogram()
{	
	valFudge=10;
	areaFudge=5;
	m_originalGrid = NULL;
	m_originalLocations = NULL;
	m_originalVector = NULL;
	m_distorted = false;
	m_resizePercent=0.50;
	m_resize = false;
	m_invert = false;
	xsize = 0;
	ysize = 0;
}

void Cartogram::InitCartogram(MapModelPtr mapModel, MapControllerPtr mapController)
//void Cartogram::InitCartogram()
{
	// this is a soft copy, need a hard one!
	FileHeader* header = mapModel->GetHeader();	
	//m_mapController = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController();
	m_mapController = mapController;
	xsize = header->nCols;
	ysize = header->nRows;

	m_originalGrid = new Point3D[header->nCols*header->nRows];
}	

Cartogram::Cartogram(MapControllerPtr mapController) :
	m_mapController(mapController),
	xsize(mapController->GetMapModel()->GetHeader()->nCols),
	ysize(mapController->GetMapModel()->GetHeader()->nRows)
{
	valFudge=30;
	areaFudge=5;

	// this is a soft copy, need a hard one!
	FileHeader* header = mapController->GetMapModel()->GetHeader();
	
	// this needs to be adjusted... everything needs an extra dimension...
	// set to the size of grid and location layer
	m_originalGrid = new Point3D[header->nCols*header->nRows];
	int load = App::Inst().GetLayerTreeController()->GetNumLocationSetLayers();
	if (load > 0)
		m_originalLocations = new Point3D*[App::Inst().GetLayerTreeController()->GetNumLocationSetLayers()];
	// need to account for No vector map
	load = App::Inst().GetLayerTreeController()->GetNumVectorMapLayers();
	if ( load > 0 )
		m_originalVector = new Point3D**[App::Inst().GetLayerTreeController()->GetNumVectorMapLayers()];
	SaveOriginalProjection();
}

void Cartogram::MakeCartogram()
{
	wxBeginBusyCursor();
	bool timing = false;
	//		Log run times
	std::clock_t start;
	double duration;
	if( timing )
	{
		start = std::clock();
	}

	// If there's no location layer, we're done!
	if( App::Inst().GetLayerTreeController()->GetNumLocationSetLayers() == 0 || boost::size(m_locationSetLayerIndex) == 0 )
		return;		

	// If a cartogram has not been made before
	if ( m_originalLocations == NULL )
	{ 
		// Store all of the location and vector layers available
		int load = App::Inst().GetLayerTreeController()->GetNumLocationSetLayers();
		if (load > 0)
			m_originalLocations = new Point3D*[App::Inst().GetLayerTreeController()->GetNumLocationSetLayers()];
		
		// need to account for No vector map
		load = App::Inst().GetLayerTreeController()->GetNumVectorMapLayers();
		if ( load > 0 )
			m_originalVector = new Point3D**[App::Inst().GetLayerTreeController()->GetNumVectorMapLayers()];
		SaveOriginalProjection();
	}
	
	
	FileMetaData* meta = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapModel()->GetMetaData();
	if( meta->projection == "")
	{
		m_mapController->GetMapView()->SetCartogramState(true);
	}

	bool exaggerationChange = false;
	double exaggeration = m_mapController->GetMapModel()->GetVerticalExaggeration();
	// Set vertical exageration to 1.5
	if( exaggeration == 0.0f)
	{	
		exaggerationChange = true;
		m_mapController->GetMapModel()->SetVerticalExaggeration(1.5);
	}

	// Pre time
	if( timing) 
	{
		Log::Inst().Write( "Pre");
		duration = 	( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		Log::Inst().Write( StringTools::ToString(duration,4) );
	}
	
	double **rho;
	rho = cart_dmalloc(xsize,ysize);
	MakeRho(rho);

	// MakeRho time
	if (timing) 
	{
		duration = 	( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		Log::Inst().Write( "MakeRho");
		Log::Inst().Write( StringTools::ToString(duration,4) );
	}	

	if( m_resize ){
		// Here is where I shrink the RHO. Also needs to reset xsize and ysize to the reduced sizes
		double origXSize = xsize, origYSize = ysize;
		
		xsize = xsize * m_resizePercent;
		ysize = ysize * m_resizePercent;
		
		// resized rho
		double ** newRho;
		newRho = cart_dmalloc(xsize, ysize);
		// mallocs and fills newRho with reduced rho
		ResizeRho(rho, newRho);
		cart_dfree(rho);

		StdMain(newRho,origXSize,origYSize);
	}
	else
		StdMain(rho);
	
	// STDMain time
	if(timing)
	{
		Log::Inst().Write( "Main");
		duration = 	( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		Log::Inst().Write( StringTools::ToString(duration,4) );
	}
	
	// map is now distorted
	m_distorted = true;

	/* Now redraw the map. */
	// Need to do everything after loading terrain
	m_mapController->GetMapView()->SetVertices(m_mapController);
	m_mapController->GetMapView()->SetRoot( m_mapController->GetMapView()->createQuadtree( m_mapController->GetMapView()->GetDimension(), 0 ) );
	m_mapController->GetMapView()->RefreshQuadtree();
	App::Inst().GetViewport()->Refresh(false);

	// Turn vertical exageration back to 0?
	if( exaggerationChange )
	{
		m_mapController->GetMapModel()->SetVerticalExaggeration(exaggeration);
	}
	
	if( meta->projection == "" )
	{
		m_mapController->GetMapView()->SetCartogramState(false);
	}
	
	for( uint i = 0; i < App::Inst().GetLayerTreeController()->GetNumTreeLayers(); i++ )
	{
		GeoTreeViewPtr geoTreeView = App::Inst().GetLayerTreeController()->GetTreeLayer(i)->GetGeoTreeView();
		geoTreeView->ProjectTree(geoTreeView->GetLeafNames());
	}

	// Complete
	if (timing)
	{
		Log::Inst().Write("Complete");
		duration = 	( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		Log::Inst().Write( StringTools::ToString(duration,4) );
	}
	wxEndBusyCursor();
}

void Cartogram::UndoCartogram()
{
	// If there's no location layer, we're done!
	if( App::Inst().GetLayerTreeController()->GetNumLocationSetLayers() == 0 || boost::size(m_locationSetLayerIndex) == 0 )
		return;	

	// restore map grid
	Point3D* grid = m_mapController->GetMapModel()->GetGrid();
	FileHeader* header = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapModel()->GetHeader();
	
	// latitude
	for(int i = 0; i < (header->nRows*header->nCols) ; i++) 
	{	
		grid[i] = m_originalGrid[i];
	}

	// restore location positions
	/* Save Location layer */
	for( uint i = 0; i < App::Inst().GetLayerTreeController()->GetNumLocationSetLayers(); i++ )
	{
		LocationSetLayerPtr locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer(i);
		std::vector<LocationLayerPtr> locationLayers = locationSetLayer->GetAllActiveLocationLayers();
		LocationLayerPtr loc;
		
		if( i == 0 )
		{
			locationSetLayer->GetLocationPolygons()->SetPolygons(m_originalPolys);
		}
		for( uint l = 0; l < locationLayers.size() ; l++)
		{
			loc = locationLayers[l];
			loc->GetLocationController()->GetLocationModel()->SetEasting(m_originalLocations[i][l].x);
			loc->GetLocationController()->GetLocationModel()->SetNorthing(m_originalLocations[i][l].z);
		}
	}
	int load = App::Inst().GetLayerTreeController()->GetNumVectorMapLayers();
	// for vector map
	for ( int i = 0; i < load; i++ )
	{
		//restore vector positions
		VectorMapModelPtr vMapModel = App::Inst().GetVectorMapController(i)->GetVectorMapModel();
		GeoVector* geoVector;
		
		// g for geometries
		for( uint g = 0; g < vMapModel->GetNumberOfGeometries(); g++ )
		{
			geoVector = vMapModel->GetGeoVector(g);
			// p for points in geometry
			for( uint p = 0; p < geoVector->GetNumberOfPoints(); p++ )
			{
				geoVector->pointX[p] = m_originalVector[i][g][p].x;
				geoVector->pointY[p] = m_originalVector[i][g][p].z;
			}
		}
	}
	m_mapController->GetMapView()->SetVertices(m_mapController);
	m_mapController->GetMapView()->SetRoot( m_mapController->GetMapView()->createQuadtree( m_mapController->GetMapView()->GetDimension(), 0 ) );
	m_mapController->GetMapView()->RefreshQuadtree();
	App::Inst().GetViewport()->Refresh(false);
	
	// refresh tree projection
	for( uint i = 0; i < App::Inst().GetLayerTreeController()->GetNumTreeLayers(); i++ )
	{
		GeoTreeViewPtr geoTreeView = App::Inst().GetLayerTreeController()->GetTreeLayer(i)->GetGeoTreeView();
		geoTreeView->ProjectTree(geoTreeView->GetLeafNames());
	}

	// map is no longer distorted
	m_distorted = false;
}

std::map<int,boost::array<double,4>> Cartogram::TranslateLocations()
{

	std::map<int,boost::array<double,4>> gridDensity;
	Box2D mapBox = m_mapController->GetMapModel()->GetProjectionExtents();
	double mapHeight = mapBox.dy - mapBox.y ;
	double mapWidth = mapBox.dx - mapBox.x ;
	LocationSetLayerPtr locationSetLayer;
	std::vector<LocationLayerPtr> locationLayers;
	for( int lSLIndex = 0; lSLIndex < boost::size(m_locationSetLayerIndex); lSLIndex++ )
	{

		locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer(m_locationSetLayerIndex[lSLIndex] );
		locationLayers = locationSetLayer->GetAllActiveLocationLayers();
		LocationLayerPtr loc;

		for( uint i = 0; i < locationLayers.size() ; i++)
		{
			loc = locationLayers[i];
			
			double count;
			if( m_measureLabel.compare( StringTools::ToStringW("Sequence Count") ) == 0 )
			{
				// simple count metric. Will need to be buffed up!
				count = std::max(1,int(loc->GetNumSequenceLayers()));
			}
			else
			{
			//int count = (int)(loc->GetLocationController()->GetData()["PhylogeneticDiversity"]);
				std::map<std::wstring,std::wstring> data = loc->GetLocationController()->GetData();
				std::wstring weightStr = data[m_measureLabel];
				count = StringTools::ToDouble(weightStr);
			}

			// map box x and y are the lower left corner of the map.
			// map box dx and dy are the width and height of the box.
			double easting = loc->GetLocationController()->GetEasting() - mapBox.x;
			// to get the smallest y value, y - dy
			double northing = loc->GetLocationController()->GetNorthing() - mapBox.y;

			// Oh Boy does this look right!
			int indexX = (double(easting/mapWidth)*xsize);
			int indexY = (double((mapHeight - northing)/mapHeight)*ysize);
			int index = indexX + indexY*xsize;

			// check gridDensity
			// if already index, add count
			// else add index
			if(gridDensity.count(index) > 0)
			{
				gridDensity[index][2]+=count;
			}
			else
			{
				boost::array<double,4> a = {indexX,indexY,count,i};
				gridDensity[index] = a;
			}
		}
	}
	return gridDensity;
}

void Cartogram::MakeRho(double **a)
{
	double **b = cart_dmalloc(ysize,xsize);
	PopulateRho(b);
	TransposeMatrix(b,a);
	cart_dfree(b);
}

double Cartogram::GetMapAverage(std::map<int,boost::array<double,4>> map)
{
	int count = 0;
	double average = 0;
	std::map<int,boost::array<double,4>>::iterator it;
	for( it = map.begin(); it != map.end(); ++it )
	{
		average += it->second[2];
		count++;
	}
	double res = average/count;
	return res;

}

// Taken from Newman main file
void Cartogram::CreateGrid(double *gridx, double *gridy, int xsize, int ysize)
{
	int ix,iy;
	int i;

	for (iy=0,i=0; iy<=ysize; iy++) {
		for (ix=0; ix<=xsize; ix++) {	
			gridx[i] = ix;
			gridy[i] = iy;
			i++;
		}
	}

}

/*
std::vector<std::vector<double>> Cartogram::PopulateRho()
{
	// Find grid squares with locations
	std::map<int,boost::array<double,4>> locationCounts = TranslateLocations();
	std::vector<std::vector<double>> prime(ysize,std::vector<double>(xsize));
	// Find average of these values (for empyt grid squares)
	double average = GetMapAverage(locationCounts);
	double avgVal = average + OFFSET*average;
	int index=0;
	double val = 0;
	int i,j;

	Point2D xy;	
	for ( std::map<int,boost::array<double,4>>::iterator iter = locationCounts.begin(); iter != locationCounts.end();++iter ) 
	{
		i = iter->second[0];
		j = iter->second[1];
		// areaFudge the value with OFFSET for cart
		val = iter->second[2] + OFFSET*average;
		for( int x = std::max(i-areaFudge,0); x <= std::min(i+areaFudge,xsize-1); x++)
		{
			for( int y = std::max(j-areaFudge,0); y <= std::min(j+areaFudge,ysize-1); y++)
			{
					prime[y][x] += val*valFudge;
			}
		}
	}
	// xsize = number of columns
	for ( int i = 0; i < xsize; ++i)
	{
		// ysize = number of rows
		for( int j = 0; j < ysize; ++j )
		{
			// only assign val if it hasn't been visited by areaFudge
			if(prime[j][i] == 0 )
			{
				prime[j][i] = avgVal;
			}	
		}
	}
	return prime;
}
*/
void Cartogram::PopulateRho(double **rho)
{
	// Find grid squares with locations
	std::map<int,boost::array<double,4>> locationCounts = TranslateLocations();
	// Find average of these values (for empyt grid squares)
	double average = GetMapAverage(locationCounts);
	double avgVal = average + OFFSET*average;
	int index=0;
	double val = 0;
	int i,j;
	double invertValue;
	if(m_invert)
	{
		invertValue = GetLargestDataValue();
	}
	Point2D xy;	
	for ( std::map<int,boost::array<double,4>>::iterator iter = locationCounts.begin(); iter != locationCounts.end();++iter ) 
	{
		i = iter->second[0];
		j = iter->second[1];
		// areaFudge the value with OFFSET for cart
		val = iter->second[2] + OFFSET*average;
		val = val;
		for( int x = std::max(i-areaFudge,0); x <= std::min(i+areaFudge,xsize-1); x++)
		{
			for( int y = std::max(j-areaFudge,0); y <= std::min(j+areaFudge,ysize-1); y++)
			{
	//			prime[y][x] += val*valFudge;
				if(m_invert)
				{
						rho[y][x] += abs( invertValue - val ) *valFudge ;
				}
				else
					rho[y][x] += val*valFudge;
			
			}
		}
	}
	// xsize = number of columns
	for ( int i = 0; i < xsize; ++i)
	{
		// ysize = number of rows
		for( int j = 0; j < ysize; ++j )
		{
			// only assign val if it hasn't been visited by areaFudge
			if(rho[j][i] == 0 )
			{
				rho[j][i] = avgVal;
			}	
		}
	}
}

template< typename Matrix >
void Cartogram::InterpolateGrid(Matrix gridx, Matrix gridy)
{
	Point3D* grid = m_mapController->GetMapModel()->GetGrid();
	FileHeader* header = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapModel()->GetHeader();
	double aspect = header->projExtents.Height() / header->projExtents.Width();
	float X,Z,nX,nZ;
	int index=0;
	Point3D gridTest;

	// latitude
	for(int m = 0; m < header->nRows ; m++) 
	{	
		//longitude
		for(int n = 0; n < header->nCols; n++) 
		{
			Z = gridy[n][m];
			X = gridx[n][m];

			nX = -1.0f + 2*((X - 0) / header->nCols );
			nZ = (1.0f + 2*((Z - (header->nRows-1)) / header->nRows))*aspect;

			grid[index].x = nX ;
			grid[index].z = nZ;		
			index++;
		}
	}

}

void Cartogram::InterpolateGrid(double* gridx, double* gridy)
{
	Point3D* grid = m_mapController->GetMapModel()->GetGrid();
	FileHeader* header = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapModel()->GetHeader();
	double aspect = header->projExtents.Height() / header->projExtents.Width();
	float X,Z,nX,nZ;
	int index=0;
	int distIndex = 0;
	Point3D gridTest;
	double inv1, inv2;
	
	// latitude
	for(int m = 0; m < header->nRows ; m++) 
	{	
		//longitude
		for(int n = 0; n < header->nCols; n++) 
		{
			distIndex = m*(header->nCols+1) + n;
			Z = gridy[distIndex];
			X = gridx[distIndex];

			nX = -1.0f + 2*((X - 0) / header->nCols );
		//	nZ = (1.0f + 2*((Z - (header->nRows)) / header->nRows))*aspect;
			nZ = (1.0f + 2*((Z - (header->nRows-1)) / header->nRows))*aspect;
			inv1 = grid[index].x;
			inv2 = grid[index].z;
			grid[index].x = nX ;
			grid[index].z = nZ;	
			index++;
		}
	}

}


template< typename Matrix >
void Cartogram::InterpolateVector(Matrix gridx, Matrix gridy, VectorMapControllerPtr vectorMap)
{
	double width = m_mapController->GetMapModel()->GetWidth();
	double height = m_mapController->GetMapModel()->GetHeight();
	double minWidth = -1*width/2;
	double minHeight = -1*height/2;

	float X,Z,nX,nZ,Xout,Zout;
	VectorMapModelPtr vMapModel = vectorMap->GetVectorMapModel();
	GeoVector* geoVector;
	
	// code to write the vector points before and after the interpolation
	int names = 0;
//	std::ofstream myfile;
	std::string fileName = "C:/Users/Admin/Documents/GCPDPaper/Distance/252ReducedRunVectorPoints.csv";
//	try{
//	myfile.open(fileName.c_str(), std::ofstream::out);
//	}catch(std::ifstream::failure &writeErr){
//		Log::Inst().Warning("Could not open RHO file.");
//	}

	// g for geometries
	for( uint g = 0; g< vMapModel->GetNumberOfGeometries(); g++ )
	{
		geoVector = vMapModel->GetGeoVector(g);
		// p for points in geometry
		for( uint p = 0; p < geoVector->GetNumberOfPoints(); p++ )
		{
			X = (geoVector->pointX[p] - minWidth) / width * xsize;
			Z = (geoVector->pointY[p] - minHeight) / height *ysize;
			if( X < xsize && X >=0 && Z < ysize && Z >= 0 )
			{	
				nX = gridx[X][Z];
				nZ = gridy[X][Z];
				Xout  = nX/xsize*width + minWidth;
				Zout = (nZ)/ysize*height + minHeight;

/*				myfile << names + "," +
					boost::lexical_cast<std::string>(geoVector->pointX[p]) + "," +
					boost::lexical_cast<std::string>(geoVector->pointY[p]) + "," +
					boost::lexical_cast<std::string>(Xout) + "," +
					boost::lexical_cast<std::string>(Zout) + "\n";
*/
				geoVector->pointX[p] = Xout;
				geoVector->pointY[p] = Zout;
			}
//			myfile.flush();
		}
	}
//	myfile.close();
}

void Cartogram::InterpolatePolygons(double* gridx, double* gridy, LocationPolygonsPtr locationGons, bool resize = false)
{
	double width = m_mapController->GetMapModel()->GetWidth();
	double height = m_mapController->GetMapModel()->GetHeight();
	double minWidth = -1*width/2;
	double minHeight = -1*height/2;
	int index = 0;
	float X,Z,nX,nZ,Xout,Zout;

	std::vector<PolygonModelPtr> polys = locationGons->GetPolygons();
	for(int i=0; i< boost::size(polys); i++)
	{
		std::vector<Point3D> verts = polys[i]->modifiedVertices;
		for(int vert=0; vert < boost::size(verts); vert++)
		{
			//warp the modified vertices and we're golden!		
			Point3D gridCell = verts[vert];
		
			X = (gridCell.x - minWidth) / width * xsize;
			Z = (gridCell.z - minHeight) / height *ysize;
			if( X < xsize && X >=0 && Z < ysize && Z >= 0 )
			{	
				index = floor(Z)*(xsize+1) + floor(X);
				nX = gridx[index];
				nZ = gridy[index];
				Xout  = nX/xsize*width + minWidth;
				Zout = (nZ)/ysize*height + minHeight;
				verts[vert].x = Xout;
				verts[vert].z = Zout;
			}
		}
		polys[i]->modifiedVertices = verts;
	}
	locationGons->SetPolygons(polys);
}

template< typename Matrix >
void Cartogram::InterpolatePolygons(Matrix gridx, Matrix gridy, LocationPolygonsPtr locationGons, bool resize = false)
{
	double width = m_mapController->GetMapModel()->GetWidth();
	double height = m_mapController->GetMapModel()->GetHeight();
	double minWidth = -1*width/2;
	double minHeight = -1*height/2;

	float X,Z,nX,nZ,Xout,Zout;

	std::vector<PolygonModelPtr> polys = locationGons->GetPolygons();
	for(int i=0; i< boost::size(polys); i++)
	{
		std::vector<Point3D> verts = polys[i]->modifiedVertices;
		for(int vert=0; vert < boost::size(verts); vert++)
		{
			//warp the modified vertices and we're golden!		
			Point3D gridCell = verts[vert];
		
			X = (gridCell.x - minWidth) / width * xsize;
			Z = (gridCell.z - minHeight) / height *ysize;
			if( X < xsize && X >=0 && Z < ysize && Z >= 0 )
			{	
				nX = gridx[X][Z];
				nZ = gridy[X][Z];
				Xout  = nX/xsize*width + minWidth;
				Zout = (nZ)/ysize*height + minHeight;
				verts[vert].x = Xout;
				verts[vert].z = Zout;
			}
		}
		polys[i]->modifiedVertices = verts;
	}
	locationGons->SetPolygons(polys);
}

void Cartogram::InterpolateVector(double* gridx, double* gridy, VectorMapControllerPtr vectorMap)
{
	double width = m_mapController->GetMapModel()->GetWidth();
	double height = m_mapController->GetMapModel()->GetHeight();
	double minWidth = -1*width/2;
	double minHeight = -1*height/2;

	float X,Z,nX,nZ,Xout,Zout;
	VectorMapModelPtr vMapModel = vectorMap->GetVectorMapModel();
	GeoVector* geoVector;
	int index = 0;
	// code to write the vector points before and after the interpolation
	int names = 0;
//	std::ofstream myfile;
//	std::string fileName = "C:/Users/Admin/Documents/GCPDPaper/Distance/FullRunVectorPoints.csv";
//	try{
//	myfile.open(fileName.c_str(), std::ofstream::out);
//	}catch(std::ifstream::failure &writeErr){
//		Log::Inst().Warning("Could not open RHO file.");
//	}

	// g for geometries
	for( uint g = 0; g< vMapModel->GetNumberOfGeometries(); g++ )
	{
		geoVector = vMapModel->GetGeoVector(g);
		// p for points in geometry
		for( uint p = 0; p < geoVector->GetNumberOfPoints(); p++ )
		{
			X = (geoVector->pointX[p] - minWidth) / width * xsize;
			Z = (geoVector->pointY[p] - minHeight) / height *ysize;
			if( X < xsize && X >=0 && Z < ysize && Z >= 0 )
			{	
				index = floor(Z)*(xsize+1) + floor(X);
				nX = gridx[index];
				nZ = gridy[index];
				
				Xout  = nX/xsize*width + minWidth;
				Zout = (nZ)/ysize*height + minHeight;
			/*	myfile << names + "," +
					boost::lexical_cast<std::string>(geoVector->pointX[p]) + "," +
					boost::lexical_cast<std::string>(geoVector->pointY[p]) + "," +
					boost::lexical_cast<std::string>(Xout) + "," +
					boost::lexical_cast<std::string>(Zout) + "\n";
			*/	geoVector->pointX[p] = Xout;
				geoVector->pointY[p] = Zout;
			}
		//	myfile.flush();
		}
	}
//	myfile.close();
}


template< typename Matrix >
void Cartogram::InterpolateLocations(Matrix gridx, Matrix gridy, LocationSetLayerPtr locationSetLayer, bool resize = false)
{
//	grid -1,-1 = top left corner (max y, min x)
	double resizePercent = 0.5;
	int ix,iy;
	double xin,yin;
	double xout,yout,yCorrection;
	double dx,dy;
	double width;
	double height;
	double minWidth;
	double minHeight;
	Point3D gridCell;
	Box2D mapBox = m_mapController->GetMapModel()->GetProjectionExtents();
	height = mapBox.dy - mapBox.y;
	width = mapBox.dx - mapBox.x;
	minWidth = mapBox.x;
	minHeight = mapBox.y;
	std::vector<LocationLayerPtr> locationLayers = locationSetLayer->GetAllActiveLocationLayers();
	LocationLayerPtr loc;
	
	for( uint i = 0; i < locationLayers.size() ; i++)
	{
		loc = locationLayers[i];
		gridCell.x = loc->GetLocationController()->GetEasting();
		gridCell.z = loc->GetLocationController()->GetNorthing();
		xin = (gridCell.x - minWidth)/(width) * double(xsize);
		yin = (ysize) - (gridCell.z - minHeight)/(height) * double(ysize);
	
		// Check if we are outside bounds 
		
		if ((xin<0.0)||(xin>=xsize)||(yin<0.0)||(yin>=ysize)) {
			xout = xin;
			yout = yin;
		} 
		else 
		{
			ix = xin;
			dx = xin - ix;
			iy = yin;
			dy = yin - iy;
			int iyy = yin;
			if(!resize)
			{
				xout = gridx[ix][iy];
				yout = gridy[ix][iy];
			}
			else
			{
	//			double x1 = gridx[ix][iy];
	//			double y1 = gridy[ix][iy];
				double x1 = (1-dx)*(1-dy)*gridx[ix][iy] + dx*(1-dy)*gridx[ix+1][iy] + (1-dx)*dy*gridx[ix][iy+1] + dx*dy*gridx[ix+1][iy+1];
				double y1 = (1-dx)*(1-dy)*gridy[ix][iy] + dx*(1-dy)*gridy[ix+1][iy] + (1-dx)*dy*gridy[ix][iy+1] + dx*dy*gridy[ix+1][iy+1];
				double deltax = xin - x1;
				double deltay = yin - y1;
				
		//		xout = gridx[ix][iy] + abs(xin - gridx[ix][iy])*resizePercent;
		//		yout = gridy[ix][iy] + abs(yin - gridy[ix][iy])*resizePercent;
				xout = x1;// + deltax*resizePercent;
				yout = y1;// + deltay*resizePercent;
			}
		}
		// Y direction shifts are occuring at the correct magnitude but in the wrong direction. Correct it!
		yCorrection = (ysize) - yout;
		gridCell.x = ( xout ) / double(xsize) * (width) + minWidth;//-1;	
		gridCell.z = (yCorrection) / double(ysize) * (height) + minHeight;//-1;
		loc->GetLocationController()->GetLocationModel()->SetEasting(gridCell.x);
		loc->GetLocationController()->GetLocationModel()->SetNorthing(gridCell.z);
	}
}
void Cartogram::InterpolateLocations(double *gridx, double *gridy, LocationSetLayerPtr locationSetLayer)
{
	
//	grid -1,-1 = top left corner (max y, min x)
	int ix,iy;
	int index = 0;
	int ind1 =0, ind2=0,ind3=0;
	double xin,yin;
	double xout,yout,yCorrection;
	double dx,dy;
	double width;
	double height;
	double minWidth;
	double minHeight;
	Point3D gridCell;
	Box2D mapBox = m_mapController->GetMapModel()->GetProjectionExtents();
	height = mapBox.dy - mapBox.y;
	width = mapBox.dx - mapBox.x;
	minWidth = mapBox.x;
	minHeight = mapBox.y;
	std::vector<LocationLayerPtr> locationLayers = locationSetLayer->GetAllActiveLocationLayers();
	LocationLayerPtr loc;
	
	for( uint i = 0; i < locationLayers.size() ; i++)
	{
		loc = locationLayers[i];
		gridCell.x = loc->GetLocationController()->GetEasting();
		gridCell.z = loc->GetLocationController()->GetNorthing();
		xin = (gridCell.x - minWidth)/(width) * double(xsize);
		yin = (ysize) - (gridCell.z - minHeight)/(height) * double(ysize);
	
		// Check if we are outside bounds 
		
		if ((xin<0.0)||(xin>=xsize)||(yin<0.0)||(yin>=ysize)) {
			xout = xin;
			yout = yin;
		} 
		else 
		{
			ix = xin;
			dx = xin - ix;
			iy = yin;
			dy = yin - iy;
		
			index = iy*(xsize+1) + ix;
			ind1 = iy*(xsize+1) + ix+1;
			ind2 = (iy+1)*(xsize+1) + ix;
			ind3 = (iy+1)*(xsize+1) + ix+1;
	//		xout = gridx[index];
	//		yout = gridy[index];
			// Interpolate the locations
			xout = (1-dx)*(1-dy)*gridx[index] + dx*(1-dy)*gridx[ind1]
             + (1-dx)*dy*gridx[ind2] + dx*dy*gridx[ind3];
			yout = (1-dx)*(1-dy)*gridy[index] + dx*(1-dy)*gridy[ind1]
             + (1-dx)*dy*gridy[ind2] + dx*dy*gridy[ind3];
		}
		// Y direction shifts are occuring at the correct magnitude but in the wrong direction. Correct it!
		yCorrection = (ysize) - yout;
		gridCell.x = ( xout ) / double(xsize) * (width) + minWidth;//-1;	
		gridCell.z = (yCorrection) / double(ysize) * (height) + minHeight;//-1;
		loc->GetLocationController()->GetLocationModel()->SetEasting(gridCell.x);
		loc->GetLocationController()->GetLocationModel()->SetNorthing(gridCell.z);
	}
}


template< typename Matrix >
void Cartogram::ArrayTransform(double * grid, Matrix &transGrid)
{
	int index = 0;
	for( int y =0; y <= ysize ; ++y)
	{
		for( int x = 0; x<= xsize ; ++x)
		{
		//	double blarg = grid[index];
			transGrid[x][y] = grid[index];
			index++;
		}
	}
}

template< typename Matrix >
void Cartogram::TransposeMatrix(Matrix &original, Matrix &trans)
{
	for( int i =0; i < ysize ; ++i)
	{
		for( int j = 0; j< xsize ; ++j)
		{
			double blarg = original[i][j];
			trans[j][i] = blarg;
		}
	}
}

template< typename Matrix >
void Cartogram::WriteMatrix(Matrix rho, int row, int col, std::string name)
{
	std::ofstream myfile;
	std::string fileName, temp;
	
	if(boost::starts_with(name,"temp"))
	{	
		// Gets the working directory of the executable 
		fileName = GenGIS::App::Inst().GetExeDir().mb_str();
		fileName += name;
	}
	else
	{
		fileName = "C:/Users/Admin/Desktop/" + name + ".txt";
	}
	/*
	try{
	myfile.open(fileName.c_str(), std::ofstream::out);
	}catch(std::ifstream::failure &writeErr){
		Log::Inst().Warning("Could not open RHO file.");
	}

	if(!myfile)
	{
		Log::Inst().Warning("Could not open RHO file.");
	}
	*/
	for(int i = 0; i < row; i++)
	{
		for( int j = 0; j < col; j++)
		{
			try{
				myfile << boost::lexical_cast<std::string>(rho[i][j]) + "\t"; 
			}catch(std::ifstream::failure &writeErr){
				Log::Inst().Warning("Writing to file failed.");
			}
		}
		myfile << "\n";
		myfile.flush();
	}

	myfile.close();
}

/*
	Following the structure of Newmans cartogram code.
*/
/*
void Cartogram::StdMain()
{
	double *gridx,*gridy;  // Array for grid points
	double **rho;          // Initial population density
	FILE *tempIn;
	std::string fileName = GenGIS::App::Inst().GetExeDir().mb_str();
	fileName += "temp.dat";
	tempIn = fopen(fileName.c_str(),"r");
	int matrixSize = (xsize+1)*(ysize+1);

	cart_makews(xsize,ysize);
	rho = cart_dmalloc(xsize,ysize);
	if (readpop(tempIn,rho,xsize,ysize)) {
		Log::Inst().Warning("Density matrix was not loaded properly.");
	}
	cart_transform(rho,xsize,ysize);
    cart_dfree(rho);

	// Create the grid of points 
	gridx = (double*) malloc(matrixSize*sizeof(double));
	gridy = (double*) malloc(matrixSize*sizeof(double));

	CreateGrid(gridx,gridy,xsize,ysize);
	cart_makecart(gridx, gridy, matrixSize, xsize, ysize, 0.0);

	std::vector<std::vector<double>> gridx2d(xsize+1,std::vector<double>(ysize+1));
	std::vector<std::vector<double>> gridy2d(xsize+1,std::vector<double>(ysize+1));
	ArrayTransform(gridx, gridx2d);
	ArrayTransform(gridy, gridy2d);
	
	// update map points
	Interpolate(gridx2d,gridy2d);

	LocationSetLayerPtr locationSetLayer;

	// Interpolate All location sets
	for( int i = 0; i < boost::size(m_locationSetLayerIndex); i++ )
	{
		locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer( m_locationSetLayerIndex[i] );
		InterpolateLocations(gridx2d, gridy2d, locationSetLayer);
	}

	// If there's a vector map, transform it!
	VectorMapControllerPtr vectorController;
	if( App::Inst().GetLayerTreeController()->GetNumVectorMapLayers() > 0 || boost::size(m_vectorMapIndex) > 0 )
	{
		for( int i = 0; i < boost::size(m_vectorMapIndex); i++ )
		{
			vectorController = App::Inst().GetVectorMapController( m_vectorMapIndex[i] );
			InterpolateVector(gridx2d, gridy2d, vectorController);
		}
	}

	// free up space again
	cart_freews(xsize, ysize);
	free(gridx);
	free(gridy);
}
*/

void Cartogram::StdMain(double **rho)
{
	int matrixSize = (xsize+1)*(ysize+1);
	double *gridx,*gridy;  // Array for grid points
	/* Create the grid of points */
	try{
		gridx = new double[matrixSize];
	}catch(std::bad_alloc &ba){
		Log::Inst().Write("Could not allocate enough memory to create a cartogram. X dimension is too large." + StringTools::ToString(ba.what()));
		return;
	}
	try{
		gridy = new double[matrixSize];
	}catch(std::bad_alloc &ba){
		Log::Inst().Write("Could not allocate enough memory to create a cartogram. Y dimension is too large." + StringTools::ToString(ba.what()));
		return;
	}
	
	
	
	cart_makews(xsize,ysize);
	cart_transform(rho,xsize,ysize);
	cart_dfree(rho);

	
	CreateGrid(gridx,gridy,xsize,ysize);
	try{
		cart_makecart(gridx, gridy, matrixSize, xsize, ysize, 0.0);
	}catch(std::bad_alloc &ba ){
		Log::Inst().Write("Cartogram failed due to memory limitations. Using a smaller map is suggested." + StringTools::ToString(ba.what()));
		return;
	}
	// free up the space used by cart
	cart_freews(xsize, ysize);
/*
	std::vector<std::vector<double>> gridx2d(xsize+1,std::vector<double>(ysize+1));
	std::vector<std::vector<double>> gridy2d(xsize+1,std::vector<double>(ysize+1));
	ArrayTransform(gridx, gridx2d);
	ArrayTransform(gridy, gridy2d);
*/
	// free up space again
	/*
	delete(gridx);
	delete(gridy);
	*/
	/*
	// update map points
	try{
		InterpolateGrid(gridx2d,gridy2d);
	}catch(std::bad_alloc &ba){
		Log::Inst().Write("Bad Memory 4 " + StringTools::ToString(ba.what()));
	}
	
	*/	
//	InterpolateGrid(gridx2d,gridy2d);
	InterpolateGrid(gridx,gridy);

	LocationSetLayerPtr locationSetLayer;

	// Interpolate All location sets
	for( int i = 0; i < boost::size(m_locationSetLayerIndex); i++ )
	{
		locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer( m_locationSetLayerIndex[i] );
	//	InterpolateLocations(gridx2d, gridy2d, locationSetLayer);
	//	WriteLocations(locationSetLayer, "preDistort.txt");
		InterpolateLocations(gridx, gridy, locationSetLayer);
	//	WriteLocations(locationSetLayer, "postDistort.txt");

		LocationPolygonsPtr locationsGons = locationSetLayer->GetLocationPolygons();
		InterpolatePolygons(gridx, gridy,locationsGons);
		locationsGons->Render();
	}
	// If there's a vector map, transform it!
	VectorMapControllerPtr vectorController;
	if( App::Inst().GetLayerTreeController()->GetNumVectorMapLayers() > 0 || boost::size(m_vectorMapIndex) > 0 )
	{
		for( int i = 0; i < boost::size(m_vectorMapIndex); i++ )
		{
			vectorController = App::Inst().GetVectorMapController( m_vectorMapIndex[i] );
		//	InterpolateVector(gridx2d, gridy2d, vectorController);
			InterpolateVector(gridx, gridy, vectorController);
		}
	}
	
	delete(gridx);
	delete(gridy);
}


void Cartogram::StdMain(double **rho, int origXSize, int origYSize)
{
	double *gridx,*gridy;  // Array for grid points
	int matrixSize = (xsize+1)*(ysize+1);
	// Create the grid of points 
	try{
		gridx = new double[matrixSize];
	}catch(std::bad_alloc &ba){
		Log::Inst().Write("Could not allocate enough memory to create a cartogram. X dimension is too large." + StringTools::ToString(ba.what()));
		return;
	}
	try{
		gridy = new double[matrixSize];
	}catch(std::bad_alloc &ba){
		Log::Inst().Write("Could not allocate enough memory to create a cartogram. Y dimension is too large." + StringTools::ToString(ba.what()));
		return;
	}
	
	cart_makews(xsize,ysize);
	cart_transform(rho,xsize,ysize);
    cart_dfree(rho);

	

	CreateGrid(gridx,gridy,xsize,ysize);
	try{
		cart_makecart(gridx, gridy, matrixSize, xsize, ysize, 0.0);
	}catch(std::bad_alloc &ba ){
		Log::Inst().Write("Cartogram failed due to memory limitations. Using a smaller map is suggested." + StringTools::ToString(ba.what()));
		return;
	}
	cart_freews(xsize, ysize);

	std::vector<std::vector<double>> gridx2d(xsize+1,std::vector<double>(ysize+1));
	std::vector<std::vector<double>> gridy2d(xsize+1,std::vector<double>(ysize+1));
	ArrayTransform(gridx, gridx2d);
	ArrayTransform(gridy, gridy2d);

	// resize the grid
	
	// Fractions make a restoration back to the original size is troublesome
	// Lets get them back to "close" to the original size
	std::vector<std::vector<double>> origXGrid = RestoreGrid(gridx2d,origXSize,origYSize);
	std::vector<std::vector<double>> origYGrid = RestoreGrid(gridy2d,origXSize,origYSize);
//	double* origXGrid = RestoreGrid(gridx, gridx2d, origXSize, origYSize);
//	double* origYGrid = RestoreGrid(gridy, origXSize, origYSize);
	xsize = origXSize;
	ysize = origYSize;
	
	free(gridx);
	free(gridy);

	// update map points
	InterpolateGrid(origXGrid,origYGrid);


	LocationSetLayerPtr locationSetLayer;

	// Interpolate All location sets
	for( int i = 0; i < boost::size(m_locationSetLayerIndex); i++ )
	{
		locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer( m_locationSetLayerIndex[i] );	
		InterpolateLocations(origXGrid, origYGrid, locationSetLayer,true);

		LocationPolygonsPtr locationsGons = locationSetLayer->GetLocationPolygons();
		InterpolatePolygons(origXGrid, origYGrid,locationsGons);
		locationsGons->Render();

	}

	// If there's a vector map, transform it!
	VectorMapControllerPtr vectorController;
	if( App::Inst().GetLayerTreeController()->GetNumVectorMapLayers() > 0 || boost::size(m_vectorMapIndex) > 0 )
	{
		for( int i = 0; i < boost::size(m_vectorMapIndex); i++ )
		{
			vectorController = App::Inst().GetVectorMapController( m_vectorMapIndex[i] );
			InterpolateVector(origXGrid, origYGrid, vectorController);
		}
	}
	
	// free up space again
//	free(origXGrid);
//	free(origYGrid);
}


/* Function to read population data into the array rho.  Returns 1 if there
 * was a problem, zero otherwise */
/*
int Cartogram::readpop(FILE *stream, double **rho, int xsize, int ysize)
{
  int ix,iy;
  int n;
  double mean;
  double sum=0.0;

  for (iy=0; iy<ysize; iy++) {
    for (ix=0; ix<xsize; ix++) {
      n = fscanf(stream,"%lf",&rho[ix][iy]);
      if (n!=1) return 1;
      sum += rho[ix][iy];
    }
  }

  mean = sum/(xsize*ysize);
  for (iy=0; iy<ysize; iy++) {
    for (ix=0; ix<xsize; ix++) {
      rho[ix][iy] += OFFSET*mean;
    }
  }

  return 0;
}

void Cartogram::writepoints(FILE *stream, double *gridx, double *gridy, int npoints)
{
  int i;

  for (i=0; i<npoints; i++) fprintf(stream,"%g %g\n",gridx[i],gridy[i]);
}

template< typename Matrix >
int Cartogram::readpoints(FILE *stream, Matrix &gridx, Matrix &gridy, int xsize, int ysize)
{
  int ix,iy;

  for (iy=0; iy<=ysize; iy++) {
    for (ix=0; ix<=xsize; ix++) {
      if (fscanf(stream,"%lf %lf\n",&gridx[ix][iy],&gridy[ix][iy])<2) {
		return 1;
      }
    }
  }

  return 0;
}
*/
void Cartogram::SaveOriginalProjection()
{
	/* Save grid */
	Point3D* grid = m_mapController->GetMapModel()->GetGrid();
	FileHeader* header = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapModel()->GetHeader();
	
	// latitude
	for(int i = 0; i < (header->nRows*header->nCols) ; i++) 
	{	
		m_originalGrid[i] = grid[i];
	}
	/* Save Location layers */
	int load = App::Inst().GetLayerTreeController()->GetNumLocationSetLayers();
	for( int i = 0; i < load; i++ )
	{
		m_originalLocations[i] = new Point3D[App::Inst().GetLayerTreeController()->GetLocationSetLayer(0)->GetNumLocationLayers()];
		LocationSetLayerPtr locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer(i);
		std::vector<LocationLayerPtr> locationLayers = locationSetLayer->GetAllActiveLocationLayers();
		LocationLayerPtr loc;	

		if( i == 0 )
		{
			std::vector<PolygonModelPtr> polys = locationSetLayer->GetLocationPolygons()->GetPolygons();
			m_originalPolys = polys;
		}
		for( uint l = 0; l < locationLayers.size() ; l++)
		{
			loc = locationLayers[l];
			m_originalLocations[i][l].x = loc->GetLocationController()->GetEasting();
			m_originalLocations[i][l].z = loc->GetLocationController()->GetNorthing();
		}
	}
	load = App::Inst().GetLayerTreeController()->GetNumVectorMapLayers();
//	if ( load > 0 )
	for( int i = 0; i < load; i++ )
	{
		/*Save Vector Layer  */
		m_originalVector[i] = new Point3D*[ App::Inst().GetVectorMapController(i)->GetVectorMapModel()->GetNumberOfGeometries()];
		VectorMapModelPtr vMapModel = App::Inst().GetVectorMapController(i)->GetVectorMapModel();
		GeoVector* geoVector;
		
		// g for geometries
		for( uint g = 0; g< vMapModel->GetNumberOfGeometries(); g++ )
		{
			geoVector = vMapModel->GetGeoVector(g);
			m_originalVector[i][g] = new Point3D[geoVector->GetNumberOfPoints()];
			// p for points in geometry
			for( uint p = 0; p < geoVector->GetNumberOfPoints(); p++ )
			{
				m_originalVector[i][g][p].x = geoVector->pointX[p];
				m_originalVector[i][g][p].z = geoVector->pointY[p];
			}
		}
	}
}

void Cartogram::SetLocationSetLayer( int *indexes, int max )
{
	m_locationSetLayerIndex.resize(max);
	for( int i = 0; i < max; i++ )
		m_locationSetLayerIndex[i] = indexes[i];
}

void Cartogram::SetVectorMap( int *indexes, int max )
{
	m_vectorMapIndex.resize(max);
	for( int i = 0; i < max; i++ )
		m_vectorMapIndex[i] = indexes[i];
}

void Cartogram::SetResizePercent(int percent)
{
	double perc = double(percent)/100;
	m_resizePercent = 1.00 - perc;
}

void Cartogram::SetResize(boolean state)
{
	m_resize = state;
}

void Cartogram::SetInvert(boolean state)
{
	m_invert = state;
}

void Cartogram::ResizeRho(double **rho, double **newRho)
{
	double xgap = (xsize/m_resizePercent )/ xsize;
	double ygap = (ysize/m_resizePercent )/ ysize;
	
	int xlimit = xsize*xgap;
	int ylimit = ysize*ygap;

	int xindex = 0;
	int yindex = 0;
	

	std::vector< std::vector<double> > subsection (floor(xgap), std::vector<double> (floor(ygap),0) );

	int i = 0, j = 0,k=0,l=0;
	int lastX=0,lastY=0;
	double res = 0;
	boolean test = false;

	// rho rows
	for( i =0; i < xsize/m_resizePercent; i++ )
	{
		for( j = 0; j < ysize/m_resizePercent; j++ )
		{
			k = floor(i/xgap);
			l = floor(j/ygap);
			// may need to normalize by xgap*ygap after the fact
			if( k!=lastX && l!=lastY && k < xsize-1 && l < ysize-1 )
			{
				newRho[k+1][l+1] += rho[i][j];	
			}
			else if( k!=lastX && k < xsize-1 )
			{
				newRho[k+1][l] += rho[i][j];	
			}
			else if( l!=lastY && l < ysize-1 )
			{
				newRho[k][l+1] += rho[i][j];
			}

			newRho[k][l] += rho[i][j];		
			test = false;
		}
	}

}

/*
void Cartogram::ResizeRho(double **rho, double **newRho)
{
	int xgap = (xsize/m_resizePercent )/ xsize;
	int ygap = (ysize/m_resizePercent )/ ysize;

	std::vector< std::vector<double> > subsection (xgap, std::vector<double> (ygap,0) );

	int i = 0, j = 0,k=0,l=0;

	double res = 0;
	
	// rho rows
	while(i < xsize*xgap )
	{
		//rho cols
		while (j < ysize*ygap)
		{
			for( k=0; k < xgap; k++ )
			{
				for(l=0; l < ygap; l++ )
				{
					subsection[k][l] = rho[i+k][j+l];
				}
			//	l=0;
			}
		//	k=0;
			j+=ygap;	
		
			res = Combine(subsection, xgap, ygap, "AVG");

			newRho[i/xgap][j/ygap] = res;

		}
		j=0;
		i+=xgap;
	}

}
*/
template< typename Matrix >
double Cartogram::Combine(Matrix mat,int xsize, int ysize, std::string method)
{
	if( method == "AVG" )
	{
		double avg = 0;
		for(int i = 0; i < xsize; i++ )
		{
			for (int j = 0; j < ysize; j++ )
			{
				avg += mat[i][j];	
			}
		}
		return avg/(xsize*ysize);
	}
	return NULL;
	
}

std::vector<std::vector<double>> Cartogram::RestoreGrid(std::vector<std::vector<double>> grid, int origX, int origY)
{
	std::vector<std::vector<double>> newGrid(origX+1,std::vector<double>(origY+1));
	double xgap = (xsize/m_resizePercent )/ xsize;
	double ygap = (ysize/m_resizePercent )/ ysize;
	int maxReducedX = xsize * xgap;
	int maxReducedY = ysize * ygap;

	int maxOriginalX = xgap*(origX*m_resizePercent);
	int maxOriginalY = ygap*(origY*m_resizePercent);
	// Max index from the reduced grid base on fraction loss
	int reducedMaxIndex = ysize*(xsize+1) + xsize;

	int xIndex=0;
	int yIndex=0;

	int originalMaxIndex = ysize*(xsize+1) + xsize;
//	for(int i = 0; i < origX; i++ )
	for(int i = 0; i < maxOriginalX; i++ )
	{
//		for( int j = 0; j < origY; j++ )
		for( int j = 0; j < maxOriginalY; j++ )
		{
			xIndex = floor(i / xgap);
			yIndex = floor(j / ygap);
			// Divide by the resize percent otherwise the extents of the grid
			// do not cover the extents of the original grid
			if ( i < maxReducedX && j < maxReducedY )
				newGrid[i][j] = grid[xIndex][yIndex] / m_resizePercent;
			else
				newGrid[i][j] = grid[(maxReducedX-1)/xgap][(maxReducedY-1)/ygap];
		}
	}
	return newGrid;

}

double* Cartogram::RestoreGrid(double* grid, std::vector<std::vector<double>> grid2, int origX, int origY)
{
	double* newGrid = new double[(origX+1)*(origY+1)];
	
	int xgap = origX / (origX*m_resizePercent );
	int ygap = origY / (origY*m_resizePercent );
	int xIndex=0;
	int yIndex=0;
	int gridIndex = 0;
	int index = 0;

	
	for(int i = 0; i < origX; i++ )
	{
		for( int j = 0; j < origY; j++ )
		{
			// looks like I'm getting a left shift
			xIndex = i / xgap;
			yIndex = j / ygap;
			gridIndex = yIndex*(xsize+1) + xIndex;	
			// Divide by the resize percent otherwise the extents of the grid
			// do not cover the extents of the original grid
			double gridval1 = grid[gridIndex] / m_resizePercent;
			double gridval2 = grid2[xIndex][yIndex] / m_resizePercent;
			newGrid[index] = grid[gridIndex] / m_resizePercent;
			index++;
		}
	}
	return newGrid;

}


double* Cartogram::RestoreGrid(double* grid, int origX, int origY)
{
	double* newGrid = new double[(origX+1)*(origY+1)];

	int xgap = origX / (origX*m_resizePercent );
	int ygap = origY / (origY*m_resizePercent );
	int xIndex=0;
	int yIndex=0;
	int gridIndex = 0;
	int index = 0;
	for(int i = 0; i < origX; i++ )
	{
		for( int j = 0; j < origY; j++ )
		{
			// looks like I'm getting a left shift
			xIndex = i / xgap;
			yIndex = j / ygap;
			gridIndex = yIndex*(xsize+1) + xIndex;
			
			// Divide by the resize percent otherwise the extents of the grid
			// do not cover the extents of the original grid
			newGrid[index] = grid[gridIndex] / m_resizePercent;
			index++;
		}
	}
	return  newGrid;

}

void Cartogram::WriteLocations(LocationSetLayerPtr locationSetLayer,std::string fh)
{
	std::ofstream myfile;
	std::string fileName, temp;
	
	if(boost::starts_with(fh,"temp"))
	{	
		// Gets the working directory of the executable 
		fileName = GenGIS::App::Inst().GetExeDir().mb_str();
		fileName += fh;
	}
	else
	{
		fileName = "C:/Users/Admin/Desktop/" + fh + ".txt";
	}
	/*
	try{
	myfile.open(fileName.c_str(), std::ofstream::out);
	}catch(std::ifstream::failure &writeErr){
		Log::Inst().Warning("Could not open RHO file.");
	}
	if(!myfile)
	{
		Log::Inst().Warning("Could not open RHO file.");
	}
	*/
	std::vector<LocationLayerPtr> locationLayers = locationSetLayer->GetAllActiveLocationLayers();
	LocationLayerPtr loc;
	double easting;
	double northing;
	for( uint i = 0; i < locationLayers.size() ; i++)
	{
		loc = locationLayers[i];
		easting = loc->GetLocationController()->GetEasting();
		northing = loc->GetLocationController()->GetNorthing();
		try{
			myfile << boost::lexical_cast<std::string>(easting) + "\t"; 
			myfile << boost::lexical_cast<std::string>(northing) + "\t"; 
		}catch(std::ifstream::failure &writeErr){
			Log::Inst().Warning("Writing to file failed.");
		}
		myfile << "\n";
		myfile.flush();
	}

	myfile.close();	
}

double Cartogram::GetLargestDataValue()
{
	double count;
	double maxValue = 0;
	LocationSetLayerPtr locationSetLayer;
	std::vector<LocationLayerPtr> locationLayers;
	for( int lSLIndex = 0; lSLIndex < boost::size(m_locationSetLayerIndex); lSLIndex++ )
	{

		locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer(m_locationSetLayerIndex[lSLIndex] );
		locationLayers = locationSetLayer->GetAllActiveLocationLayers();
		LocationLayerPtr loc;

		for( uint i = 0; i < locationLayers.size() ; i++)
		{
			loc = locationLayers[i];
		
			//int count = (int)(loc->GetLocationController()->GetData()["PhylogeneticDiversity"]);
			std::map<std::wstring,std::wstring> data = loc->GetLocationController()->GetData();
			std::wstring weightStr = data[m_measureLabel];
			count = StringTools::ToDouble(weightStr);
		//	if( count > maxValue )
		//		maxValue = count;
			maxValue = (count > maxValue ) ? count : maxValue;

		}
	}
	return maxValue;
}
