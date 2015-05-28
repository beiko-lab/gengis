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
#include "../core/MapController.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/LocationLayer.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include<sstream>
//#include "../utils/cart.h"
extern "C" {
//	#include <cart.h>
	#include "../utils/cart.h"
}
//#include "../utils/Cart.hpp"

using namespace GenGIS;

Cartogram::Cartogram() :
	m_dimension(257),
	m_mapController(App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController())
{
//	MapViewPtr map = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapView();
//	m_vertices = map->GetVertices();
//	map->SetVertices(m_vertices);

	// Set the x and y size
//	FileHeader* header = m_mapController->GetMapModel()->GetHeader();
//	xsize = header->nCols;
//	ysize = header->nRows;
	
}

Cartogram::Cartogram(float x,float y):
	m_dimension(257),
	m_mapController(App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController())
{
	xsize = x;
	ysize = y;
//	MapViewPtr map = m_mapController->GetMapView();
//	m_vertices = map->GetVertices();
//	map->SetVertices(m_vertices);
}

void Cartogram::MakeCartogram()
{

	/** Count Location Distribtion. */
	/** Make Density Matrix. */
	MakeDensityMatrix();
	/* Now redraw the map. */
	// Need to do everything after loading terrain
	m_mapController->GetMapView()->SetVertices(m_mapController);
	m_mapController->GetMapView()->createQuadtree( m_mapController->GetMapView()->GetDimension(), 0 );
//	m_mapController->GetMapModel()->SetMaxElevation(180.0);
	float camExt = App::Inst().GetViewport()->GetCamera()->GetMapExtentsZoom();
	float bkarbare= App::Inst().GetViewport()->GetCamera()->GetHeight();
	App::Inst().GetViewport()->GetCamera()->SetHeight(bkarbare * 5);

	m_mapController->GetMapView()->RefreshQuadtree();
}

std::map<int,int> Cartogram::TranslateLocations()
{

	std::ofstream myfile;
	myfile.open("C:/Users/Admin/Desktop/cartlog.txt");

	std::map<int,int> gridDensity;
	Box2D mapBox = m_mapController->GetMapModel()->GetProjectionExtents();
	double mapHeight = mapBox.dy - mapBox.y;
	double mapWidth = mapBox.dx - mapBox.x;
	int nCols = m_mapController->GetMapModel()->GetHeader()->nCols;
	int nRows = m_mapController->GetMapModel()->GetHeader()->nRows;
	LocationSetLayerPtr locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer(0);
	std::vector<LocationLayerPtr> locationLayers = locationSetLayer->GetAllActiveLocationLayers();
	LocationLayerPtr loc;
	
	for( uint i = 0; i < locationLayers.size() ; i++)
	{
		loc = locationLayers[i];
		
		// simple count metric. Will need to be buffed up!
//		int count = std::max(1,int(loc->GetNumSequenceLayers()*10));
		int count = std::max(1,int(loc->GetNumSequenceLayers()));
		
		double easting = loc->GetLocationController()->GetEasting() - mapBox.x;
		double northing = loc->GetLocationController()->GetNorthing() - mapBox.y;

		//DONT TRUST THIS YET
		int indexX = int(double(easting/mapWidth)*nCols);
		int indexY = int(double(northing/mapHeight)*nRows);
		int index = indexX + indexY*nCols;
		
		myfile << "Cell: " + boost::lexical_cast<std::string>(index) + " Count: " + boost::lexical_cast<std::string>(count) + " lat: " + boost::lexical_cast<std::string>(northing) + " lon: " + boost::lexical_cast<std::string>(easting) + "\n";
		myfile << "\t\tgridX: "+boost::lexical_cast<std::string>(indexX) + "\tgridY: " + boost::lexical_cast<std::string>(indexY)+"\n";

		// check gridDensity
		// if already index, add count
		// else add index
		if(gridDensity.count(index) > 0)
		{
			gridDensity[index]+=count;
		}
		else
		{
			gridDensity[index] = count;
		}
	}
	myfile.close();
	return gridDensity;
}


std::map<int,int> Cartogram::BruteForceTranslate()
{
	std::ofstream myfile;
	myfile.open("C:/Users/Admin/Desktop/cartlogBRUTEFORCE.txt");
/*	float maxY = ;
	float maxY = ;
	float minX = ;
	float minY = ;
*/	
	std::map<int,int> gridDensity;
	Box2D mapBox = m_mapController->GetMapModel()->GetProjectionExtents(); 
	
	
//	float cellSizeX = m_mapController->GetMapModel()->GetHeader()->cellSizeX;   // float
//	float cellSizeZ = m_mapController->GetMapModel()->GetHeader()->cellSizeZ;   // float
//	double mapHeight = m_mapController->GetMapModel()->GetHeader()->height;
//	double mapWidth = m_mapController->GetMapModel()->GetHeader()->width;
	double mapHeight = mapBox.dy - mapBox.y;
	double mapWidth = mapBox.dx - mapBox.x;
	double indexX = mapBox.x;
	double indexY = mapBox.y;
	int index = 0;
	int nCols = m_mapController->GetMapModel()->GetHeader()->nCols;
	int nRows = m_mapController->GetMapModel()->GetHeader()->nRows;

	float cellSizeX = mapHeight / nRows;
	float cellSizeZ = mapWidth / nCols;

	LocationSetLayerPtr locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer(0);
	std::vector<LocationLayerPtr> locationLayers = locationSetLayer->GetAllActiveLocationLayers();
	LocationLayerPtr loc;
	// for each row
	while(indexX < mapBox.dx)
	{
		// for each column
		while( indexY < mapBox.dy )
		{
			Point2D bR(indexX+cellSizeX,indexY);
			Point2D tL(indexX, indexY+cellSizeZ);
			for(uint i = 0; i < locationSetLayer->GetNumLocationLayers(); i++)
			{
				loc = locationLayers[i];
				Point2D point(loc->GetLocationController()->GetEasting(),loc->GetLocationController()->GetNorthing());
				if( BoxBound(bR,tL,point) ){
					int count = loc->GetNumSequenceLayers() * 10;
					gridDensity[index] = count;
					myfile << "Cell: " + boost::lexical_cast<std::string>(index) + " Count: " + boost::lexical_cast<std::string>(count) + " lat: " + boost::lexical_cast<std::string>(loc->GetLocationController()->GetNorthing()) + " lon: " + boost::lexical_cast<std::string>(loc->GetLocationController()->GetEasting()) + "\n";
					myfile << "\t\tgridX: "+boost::lexical_cast<std::string>(indexX) + "\tgridY: " + boost::lexical_cast<std::string>(indexY)+"\n";

					
				}
			}


			indexY += cellSizeZ;
			index++;
		}
		indexY = mapBox.y;
		indexX += cellSizeX;
	}
	myfile.close();
	return gridDensity;
}

bool Cartogram::BoxBound(Point2D botRight, Point2D topLeft, Point2D loc)
{
// if loc is within tile
	float x1 = loc.x;
	float y1 = loc.y;
	float x2 = topLeft.x;
	float y2 = topLeft.y;
	if( (x1 > 794804) && (x1 < 794806) && (x2  > (794805-18501)) && (y2 > (-918385 - 18239)) )
	{
	//	Debugger.Break();
		x1=x1;

	}
	if( ( topLeft.x <= loc.x && loc.x <= botRight.x) && ( botRight.y <= loc.y && loc.y <= topLeft.y ) )
	{
		return true;
	}
	return false;
}
//void Cartogram::MakeDensityMatrix(double **rho)
void Cartogram::MakeDensityMatrix()
{
//	CartPtr cart(new Cart());
	FileHeader* header = m_mapController->GetMapModel()->GetHeader();
	
	cart_makews(header->nCols, header->nRows);
	double **rho = cart_dmalloc(header->nCols, header->nRows);
	PopulateRho(rho);

	// transform cart. RHO dis, xSize, ySize
	cart_transform(rho,header->nCols,header->nRows);
	WriteMatrix(rho, header->nRows, header->nCols, "RHO");
	cart_dfree(rho);
	// create a grid of points
	double *gridx;
	double *gridy;
	int matrixSize = (header->nCols+1)*(header->nRows+1);
	gridx = (double*) malloc(matrixSize*sizeof(double));
	gridy = (double*) malloc(matrixSize*sizeof(double));
	CreateGrid(gridx,gridy,header->nCols, header->nRows);

	// make the cartogram
	cart_makecart(gridx, gridy, matrixSize, header->nCols, header->nRows, 0.0);

	double** gridx2d = ArrayTransform(gridx);
	double** gridy2d = ArrayTransform(gridy);
	WriteMatrix(gridx2d, header->nCols, header->nRows, "gridX");
	// update map points
	Interpolate(gridx2d,gridy2d);
	// free up space again
	cart_freews(header->nCols, header->nRows);
	free(gridx);
	free(gridy);
}

int Cartogram::GetMapAverage(std::map<int,int> map)
{
	int count = 0;
	int average = 0;
	std::map<int,int>::iterator it;
	for( it = map.begin(); it != map.end(); ++it )
	{
		average += it->second;
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

void Cartogram::PopulateRho(double **rho)
{
	FileHeader* header = m_mapController->GetMapModel()->GetHeader();
	// Find grid squares with locations
	// C++ vector for debugging purposes
	std::vector<double> row;
	std::map<int,int> locationCounts = TranslateLocations();
//	std::map<int,int> locationCounts = BruteForceTranslate();
	// Find average of these values (for empyt grid squares)
	int average = GetMapAverage(locationCounts);
	int index=0;
	double val = 0;
	int fudge = 20;
	double rhoVal;
	for ( int i = 0; i < header->nRows; ++i)
	{
		for( int j = 0; j < header->nCols; ++j )
		{
			index = i*header->nRows + j;
			if ( locationCounts.count(index)>0 )
			{
				// fudge the value with OFFSET for cart
				val = locationCounts[index] + OFFSET*average;
				for( int x = std::max(i-fudge,0); x <= std::min(i+fudge,header->nRows); x++)
				{
					for( int y = std::max(j-fudge,0); y <= std::min(j+fudge,header->nCols); y++)
					{
						rhoVal = rho[x][y];
						rho[x][y]+=val*30;
						rhoVal = rho[x][y];
					}
				}
			}
			else
			{
				val = average + OFFSET*average;
				rho[i][j] = val;		
			}
		/*	rho[i][j] = val;
			// Lets try making big grid blobs of locations
			for( int x = std::max(i-fudge,0); x <= std::min(i+fudge,header->nRows); x++)
			{
				for( int y = std::max(j-fudge,0); y <= std::min(j+fudge,header->nCols); y++)
				{
					rhoVal = rho[x][y];
					rho[x][y]+=val;
					rhoVal = rho[x][y];
				}
			}
		*/	
			row.push_back(val);
	
		}
	}
}

//void Cartogram::Interpolate(double* gridx1D, double* gridy1D)
void Cartogram::Interpolate(double** gridx, double** gridy)
{
	Point3D* grid = m_mapController->GetMapModel()->GetGrid();
	FileHeader* header = m_mapController->GetMapModel()->GetHeader();
	int xsize = header->nCols;
	int	ysize = header->nRows;
	int gridSize = (xsize)*(ysize);
	int ix,iy;
	double xin,yin;
	double xout,yout;
	double dx,dy;
//	double **gridx,**gridy;
	int index = 0;
	double width;
	double height;
	double minWidth;
	double minHeight;
//	Box2D mapBox = m_mapController->GetMapModel()->GetProjectionExtents();
//	double mapHeight = mapBox.dy - mapBox.y;
//	double mapWidth = mapBox.dx - mapBox.x;
	
	Point3D gridCell;
	GeoCoord coord;

	width = m_mapController->GetMapModel()->GetWidth();
	height = m_mapController->GetMapModel()->GetHeight();
	minWidth = m_mapController->GetMapModel()->GetGrid()->x;
	minHeight = m_mapController->GetMapModel()->GetGrid()->z;

	for( int i=0; i < gridSize; ++i)
	{
		gridCell=grid[i];
	//	m_mapController->GetMapModel()->GridToGeo(gridCell, coord);
	//	GridToGeo(gridCell, coord);
	//	coord.easting-= mapBox.x;
	//	coord.northing-= mapBox.y;
	//	double indexX = double(coord.easting/mapWidth)*xsize;
	//	double indexY = double(coord.northing/mapHeight)*ysize;
		double indexX = (gridCell.x - minWidth)/width * double(xsize);
		double indexY = (gridCell.z - minHeight)/height * double(ysize);

		//	int index = indexX + indexY*nCols;
		// x and y in are the Row and column of every point
	//	xin = gridCell.x;
	//	yin = gridCell.z;
		xin = indexX;
		yin = indexY;
	/* Check if we are outside bounds */
		
		if ((xin<0.0)||(xin>=xsize)||(yin<0.0)||(yin>=ysize)) {
		//	xout = xin;
		//	yout = yin;
			xout = gridCell.x;
			yout = gridCell.z;
		} else {
			ix = xin;
			dx = xin - ix;
			iy = yin;
			dy = yin - iy;
			
			xout = (1-dx)*(1-dy)*gridx[ix][iy] + dx*(1-dy)*gridx[ix+1][iy]
				+ (1-dx)*dy*gridx[ix][iy+1] + dx*dy*gridx[ix+1][iy+1];
			yout = (1-dx)*(1-dy)*gridy[ix][iy] + dx*(1-dy)*gridy[ix+1][iy]
				+ (1-dx)*dy*gridy[ix][iy+1] + dx*dy*gridy[ix+1][iy+1];
			// reusing grid cell!
			coord.easting = xout;
			coord.northing = yout;
			m_mapController->GetMapModel()->LatLongToGrid( coord, gridCell);
		//	m_mapController->GetMapModel()->GeoToGrid( coord, gridCell);
		}
		grid[i].x = gridCell.x;
		grid[i].z = gridCell.z;
	//	Log::Inst().Warning(StringTools::ToString(i));
	}
}

double ** Cartogram::ArrayTransform(double * grid)
{
	FileHeader* header = m_mapController->GetMapModel()->GetHeader();
	int row = header->nRows;
	int col = header->nCols;
	std::vector<std::vector<double> > fakeGrid;
//	std::vector<double> fakeRow;
	//	double ** gridx;
	double** gridx = new double*[col];
//	double[][] gridx = new double[col][row];
	int index = 0;
//	gridx = (double**) malloc((col+1)*sizeof(double**));
//	gridx = new (double*)[col];
	for( int i =0; i < col; ++i)
	{
	//	fakeRow.clear();
		std::vector<double> fakeRow;
		fakeGrid.push_back(fakeRow);
		gridx[i] = new double[row];
//		*gridx[i] [row];
//		gridx[i] = (double*) malloc((row+1)*sizeof(double*));
		for( int j = 0; j< row; ++j)
		{
			index = i*row + j;
			double blarg = grid[index];
			gridx[i][j] = blarg;
//			fakeRow.push_back(blarg);
			fakeGrid[i].push_back(blarg);
		//	gridx[i][j] = grid[index];
		}
//		fakeGrid.push_back(fakeRow);
	}
	return gridx;
}

void Cartogram::WriteMatrix(double ** rho,int xsize, int ysize, std::string name)
{
	std::ofstream myfile;
	std::string fileName = "C:/Users/Admin/Desktop/" + name + ".txt";
//	myfile.open("C:/Users/Admin/Desktop/cartlogGRID.txt");
	myfile.open(fileName.c_str());

	for(int i = 0; i < xsize; i++)
	{
		for( int j = 0; j < ysize; j++)
		{
			myfile << boost::lexical_cast<std::string>(rho[i][j]) + "\t"; 
		}
		myfile << "\n";
	}

	myfile.close();
}
