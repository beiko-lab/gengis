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
	xsize(App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapModel()->GetHeader()->nCols),
	ysize(App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapModel()->GetHeader()->nRows)
{
//	MapViewPtr map = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()->GetMapView();
//	m_vertices = map->GetVertices();
//	map->SetVertices(m_vertices);

	// Set the x and y size
//	FileHeader* header = m_mapController->GetMapModel()->GetHeader();
//	xsize = ysize;
//	ysize = xsize;
}

Cartogram::Cartogram(float x,float y):
	m_dimension(257),
	m_mapController(App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController()),
	xsize(m_mapController->GetMapModel()->GetHeader()->nCols),
	ysize(m_mapController->GetMapModel()->GetHeader()->nRows)
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

		/********************
		*********************
		//DONT TRUST THIS YET
		*********************
		********************/
		int indexX = int(double(easting/mapWidth)*ysize);
		int indexY = int(double(northing/mapHeight)*xsize);
		int index = indexX + indexY*ysize;
		
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

void Cartogram::MakeDensityMatrix()
{
	FILE *outfp;
	outfp = fopen("C:/Users/admin/Desktop/mapOutTest.dat","w");
	FILE *tempOut;
	FILE *tempIn;
	tempOut = fopen("./temp.dat","w");
	tempIn = fopen("./temp.dat","r");
	double** gridx2d;
	double** gridy2d;

	cart_makews(xsize, ysize);
	double **rho = cart_dmalloc(xsize, ysize);
	PopulateRho(rho);
	
	cart_transform(rho,xsize,ysize);
	cart_dfree(rho);

	// create a grid of points
	double *gridx;
	double *gridy;
	int matrixSize = (ysize+1)*(xsize+1);
	gridx = (double*) malloc(matrixSize*sizeof(double));
	gridy = (double*) malloc(matrixSize*sizeof(double));
	CreateGrid(gridx,gridy,xsize, ysize);

	// make the cartogram
	cart_makecart(gridx, gridy, matrixSize, xsize, ysize, 0.0);
	
//	writepoints(tempOut,gridx,gridy,matrixSize);
	fclose(tempOut);
	
	writepoints(outfp,gridx,gridy,matrixSize);
	
	gridx2d = ArrayTransform(gridx);
	gridy2d = ArrayTransform(gridy);
	
	WriteMatrix(gridx2d, ysize, xsize, "gridX2DSafe");
	WriteMatrix(gridy2d, ysize, xsize, "gridY2DSafe");
	
	fclose(tempIn);

	// update map points
	Interpolate(gridx2d,gridy2d);
	// free up space again
	cart_freews(xsize, ysize);
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
	// Find grid squares with locations
	// C++ vector for debugging purposes
	std::vector<double> row;
	std::map<int,int> locationCounts = TranslateLocations();
	// Find average of these values (for empyt grid squares)
	int average = GetMapAverage(locationCounts);
	int index=0;
	double val = 0;
	int areaFudge = 30;
	int valFudge = 20;
	double rhoVal;
	for ( int i = 0; i < xsize; ++i)
	{
		for( int j = 0; j < ysize; ++j )
		{
			index = i*xsize + j;
			if ( locationCounts.count(index)>0 )
			{
				// areaFudge the value with OFFSET for cart
				val = locationCounts[index] + OFFSET*average;
		//		rho[j][i]+= val*70;
						
				for( int x = std::max(i-areaFudge,0); x <= std::min(i+areaFudge,xsize); x++)
				{
					for( int y = std::max(j-areaFudge,0); y <= std::min(j+areaFudge,ysize); y++)
					{
						rhoVal = rho[x][y];
						rho[y][x]+=val*valFudge;
						rhoVal = rho[x][y];
					}
				}
			}
			else
			{
				val = average + OFFSET*average;
				rho[i][j] = val;		
			}	
			row.push_back(val);
	
		}
	}
}

//void Cartogram::Interpolate(double* gridx1D, double* gridy1D)
void Cartogram::Interpolate(double** gridx, double** gridy)
{
	Point3D* grid = m_mapController->GetMapModel()->GetGrid();
//	FileHeader* header = m_mapController->GetMapModel()->GetHeader();
//	int xsize = ysize;
//	int	ysize = xsize;
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

	//	double indexX = (gridCell.x - minWidth)/width * double(ysize);
	//	double indexY = (gridCell.z - minHeight)/height * double(xsize);

		double indexY = (gridCell.x - minWidth)/width * double(ysize);
		double indexX = (gridCell.z - minHeight)/height * double(xsize);

		//	int index = indexX + indexY*nCols;
		// x and y in are the Row and column of every point
	//	xin = gridCell.x;
	//	yin = gridCell.z;
		xin = indexX;
		yin = indexY;
	/* Check if we are outside bounds */
		
		if ((xin<0.0)||(xin>=ysize)||(yin<0.0)||(yin>=xsize)) {
//		if ((xin<0.0)||(xin>=xsize)||(yin<0.0)||(yin>=ysize)) {
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
	double** gridx = new double*[xsize];
	int index = 0;
	for( int i =0; i <= ysize ; ++i)
	{
		gridx[i] = new double[xsize];
		for( int j = 0; j<= xsize ; ++j)
		{
		//	index = i*ysize + j;
			double blarg = grid[index];
			gridx[i][j] = blarg;
			index++;
		}
	}
	return gridx;
}

// xsize = nCols, ysize = nRows
void Cartogram::WriteMatrix(double ** rho, int row, int col, std::string name)
{
	std::ofstream myfile;
	std::string fileName = "C:/Users/Admin/Desktop/" + name + ".txt";
//	myfile.open("C:/Users/Admin/Desktop/cartlogGRID.txt");
	myfile.open(fileName.c_str());

//	for(int i = 0; i < xsize; i++)
	for(int i = 0; i < row; i++)
	{
//		for( int j = 0; j < ysize; j++)
		for( int j = 0; j < col; j++)
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
	FILE *tempOut;
	FILE *tempIn;
	tempOut = fopen("./temp.dat","w");
	tempIn = fopen("./temp.dat","r");

	int matrixSize = (xsize+1)*(ysize+1);

	std::string popdat = "C:/Users/admin/Documents/cartograms/cart-1.2.2/cart-1.2.2/uspop.dat";
	outfp = fopen("C:/Users/admin/Desktop/outpop.dat","w");
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
	
	writepoints(tempOut,gridx,gridy,matrixSize);
//	close(tempOut);
	/*
	*	Cartogram stuff is done, lets interpolate junk!	
	*/


	double** gridx2d;
	double** gridy2d;
	gridx2d = (double**)malloc((xsize+1)*sizeof(double*));
	for (int i=0; i<=xsize; i++) gridx2d[i] = (double *)malloc((ysize+1)*sizeof(double));
	gridy2d = (double**)malloc((xsize+1)*sizeof(double*));
	for (int i=0; i<=xsize; i++) gridy2d[i] = (double *)malloc((ysize+1)*sizeof(double));
	
	if( readpoints(tempIn, gridx2d, gridy2d, xsize, ysize)!=0 )
	{
		Log::Inst().Warning("Didn't recover the temp file!");
	}
//	close(tempIn);

//	double** gridx2d = ArrayTransformSafe(gridx, 512, 1024);
//	double** gridy2d = ArrayTransformSafe(gridy, 512, 1024);
	WriteMatrix(gridx2d, ysize, xsize, "gridXUSpop");
	WriteMatrix(gridy2d, ysize, xsize, "gridYUSpop");
//	writepoints(outfp,gridx,gridy,(xsize+1)*(ysize+1));
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

int Cartogram::readpoints(FILE *stream, double **gridx, double **gridy, int xsize, int ysize)
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