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
	m_mapController(App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()),
	nRow(App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapModel()->GetHeader()->nRows),
	nCol(App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapModel()->GetHeader()->nCols)
{
//	MapViewPtr map = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapView();
//	m_vertices = map->GetVertices();
//	map->SetVertices(m_vertices);

	// Set the x and y size
//	FileHeader* header = m_mapController->GetMapModel()->GetHeader();
//	xsize = nCol;
//	ysize = nRow;
}

Cartogram::Cartogram(float x,float y):
	m_dimension(257),
	m_mapController(App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()),
	nRow(m_mapController->GetMapModel()->GetHeader()->nRows),
	nCol(m_mapController->GetMapModel()->GetHeader()->nCols)
{
//	MapViewPtr map = m_mapController->GetMapView();
//	m_vertices = map->GetVertices();
//	map->SetVertices(m_vertices);
}

void Cartogram::MakeCartogram()
{

	/** Count Location Distribtion. */
	/** Make Density Matrix. */
	MakeDensityMatrix();
//	StdMain();	
	
	/* Now redraw the map. */
	// Need to do everything after loading terrain
	m_mapController->GetMapView()->SetVertices(m_mapController);
	m_mapController->GetMapView()->createQuadtree( m_mapController->GetMapView()->GetDimension(), 0 );
//	m_mapController->GetMapModel()->SetMaxElevation(180.0);
//	float camExt = App::Inst().GetViewport()->GetCamera()->GetMapExtentsZoom();
//	float bkarbare= App::Inst().GetViewport()->GetCamera()->GetHeight();
//	App::Inst().GetViewport()->GetCamera()->SetHeight(bkarbare * 5);

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
//	int nCols = m_mapController->GetMapModel()->GetHeader()->nCols;
//	int nRows = m_mapController->GetMapModel()->GetHeader()->nRows;
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
		int indexX = int(double(easting/mapWidth)*nCol);
		int indexY = int(double(northing/mapHeight)*nRow);
		int index = indexX + indexY*nCol;
		
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
	myfile.open("C:/Users/keddy/Desktop/cartlogBRUTEFORCE.txt");
	
	std::map<int,int> gridDensity;
	Box2D mapBox = m_mapController->GetMapModel()->GetProjectionExtents(); 
	
	
	double mapHeight = mapBox.dy - mapBox.y;
	double mapWidth = mapBox.dx - mapBox.x;
	double indexX = mapBox.x;
	double indexY = mapBox.y;
	int index = 0;
//	int nCols = m_mapController->GetMapModel()->GetHeader()->nCols;
//	int nRows = m_mapController->GetMapModel()->GetHeader()->nRows;

	float cellSizeX = mapHeight / nRow;
	float cellSizeZ = mapWidth / nCol;

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
//	FileHeader* header = m_mapController->GetMapModel()->GetHeader();
	FILE *outfp;
	outfp = fopen("C:/Users/keddy/Desktop/mapOutTest.dat","w");

	cart_makews(nCol, nRow);
	double **rho = cart_dmalloc(nCol, nRow);
	PopulateRho(rho);

	// transform cart. RHO dis, xSize, ySize
	cart_transform(rho,nCol,nRow);
//	WriteMatrix(rho, "RHO");
	cart_dfree(rho);
	// create a grid of points
	double *gridx;
	double *gridy;
	int matrixSize = (nCol+1)*(nRow+1);
	gridx = (double*) malloc(matrixSize*sizeof(double));
	gridy = (double*) malloc(matrixSize*sizeof(double));
	CreateGrid(gridx,gridy,nCol, nRow);

	// make the cartogram
	cart_makecart(gridx, gridy, matrixSize, nCol, nRow, 0.0);

	writepoints(outfp,gridx,gridy,matrixSize);

	double** gridx2d = ArrayTransformSafe(gridx);
	double** gridy2d = ArrayTransform(gridy);
	WriteMatrix(gridx2d, "gridX2DSafe");
	// update map points
	Interpolate(gridx2d,gridy2d);
	// free up space again
	cart_freews(nCol, nRow);
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
//	FileHeader* header = m_mapController->GetMapModel()->GetHeader();
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
	for ( int i = 0; i < nRow; ++i)
	{
		for( int j = 0; j < nCol; ++j )
		{
			index = i*nRow + j;
			if ( locationCounts.count(index)>0 )
			{
				// fudge the value with OFFSET for cart
				val = locationCounts[index] + OFFSET*average;
				for( int x = std::max(i-fudge,0); x <= std::min(i+fudge,nRow); x++)
				{
					for( int y = std::max(j-fudge,0); y <= std::min(j+fudge,nCol); y++)
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
			for( int x = std::max(i-fudge,0); x <= std::min(i+fudge,nRow); x++)
			{
				for( int y = std::max(j-fudge,0); y <= std::min(j+fudge,nCol); y++)
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
//	FileHeader* header = m_mapController->GetMapModel()->GetHeader();
//	int xsize = nCol;
//	int	ysize = nRow;
	int gridSize = (nRow)*(nCol);
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
	//	double indexX = double(coord.easting/mapWidth)*nCol;
	//	double indexY = double(coord.northing/mapHeight)*nRow;
		double indexX = (gridCell.x - minWidth)/width * double(nCol);
		double indexY = (gridCell.z - minHeight)/height * double(nRow);

		//	int index = indexX + indexY*nCols;
		// x and y in are the Row and column of every point
	//	xin = gridCell.x;
	//	yin = gridCell.z;
		xin = indexX;
		yin = indexY;
	/* Check if we are outside bounds */
		
		if ((xin<0.0)||(xin>=nCol)||(yin<0.0)||(yin>=nRow)) {
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
//	FileHeader* header = m_mapController->GetMapModel()->GetHeader();
//	int row = nRow;
//	int col = nCol;
	std::vector<std::vector<double> > fakeGrid;
//	std::vector<double> fakeRow;
	//	double ** gridx;
//	double** gridx = new double*[nCol];
	double** gridx = new double*[nRow];
//	double[][] gridx = new double[col][row];
	int index = 0;
//	gridx = (double**) malloc((col+1)*sizeof(double**));
//	gridx = new (double*)[col];
//	for( int i =0; i < col; ++i)
	for( int i =0; i < nRow + 1; ++i)
	{
	//	fakeRow.clear();
		std::vector<double> fakeRow;
		fakeGrid.push_back(fakeRow);
	
	//	gridx[i] = new double[row];
		gridx[i] = new double[nCol];
//		*gridx[i] [row];
//		gridx[i] = (double*) malloc((row+1)*sizeof(double*));
//		for( int j = 0; j< row; ++j)
		for( int j = 0; j< nCol + 1; ++j)
		{
			index = i*nCol + j;
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

double ** Cartogram::ArrayTransformSafe(double * grid)
{
	double** gridx = new double*[nRow];
	int x=0,y=0;
	for( int i =0; i < (nRow+1)*(nCol+1); ++i)
	{
		if ( i % nCol == 0 )
		{
			gridx[x] = new double[nCol];
			y=0;
			if ( i != 0 )
			{
				x++;
			}
		}
			double blarg = grid[i];
			gridx[x][y] = blarg;
			y++;
	}
	return gridx;
}

// xsize = nCols, ysize = nRows
void Cartogram::WriteMatrix(double ** rho, std::string name)
{
	std::ofstream myfile;
	std::string fileName = "C:/Users/keddy/Desktop/" + name + ".txt";
//	myfile.open("C:/Users/Admin/Desktop/cartlogGRID.txt");
	myfile.open(fileName.c_str());

//	for(int i = 0; i < xsize; i++)
	for(int i = 0; i < nRow; i++)
	{
//		for( int j = 0; j < ysize; j++)
		for( int j = 0; j < nCol; j++)
		{
			myfile << boost::lexical_cast<std::string>(rho[i][j]) + "\t"; 
		}
		myfile << "\n";
	}

	myfile.close();
}

/*
	Taken straight from Newmans main implementation for debugging purposes.
	Essentially just make sure my grid making process isn't garbage.

*/
void Cartogram::StdMain()
{
	int xsize,ysize;
	xsize = 1024;
	ysize = 512;
	double *gridx,*gridy;  // Array for grid points
	double **rho;          // Initial population density
	FILE *infp;
	FILE *outfp;
	int matrixSize = (xsize+1)*(ysize+1);

	std::string popdat = "C:/Users/keddy/Documents/cartograms/cart-1.2.2/cart-1.2.2/uspop.dat";
	outfp = fopen("C:/Users/keddy/Desktop/outpop.dat","w");
	infp = fopen(popdat.c_str(),"r");
	cart_makews(xsize,ysize);

	rho = cart_dmalloc(xsize,ysize);
	if (readpop(infp,rho,xsize,ysize)) {
		Log::Inst().Warning("Density file b f'ed son");
	}
	cart_transform(rho,xsize,ysize);
    cart_dfree(rho);

	/* Create the grid of points */

	gridx = (double*) malloc(matrixSize*sizeof(double));
	gridy = (double*) malloc(matrixSize*sizeof(double));
	CreateGrid(gridx,gridy,xsize,ysize);

	cart_makecart(gridx, gridy, matrixSize, xsize, ysize, 0.0);

	double** gridx2d = ArrayTransform(gridx);
	double** gridy2d = ArrayTransform(gridy);
//	WriteMatrix(gridx2d, nCol, nRow, "gridXUSpop");
//	WriteMatrix(gridy2d, nCol, nRow, "gridYUSpop");
	writepoints(outfp,gridx,gridy,(xsize+1)*(ysize+1));
}

/* Function to read population data into the array rho.  Returns 1 if there
 * was a problem, zero otherwise */

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