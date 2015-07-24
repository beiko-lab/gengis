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
#include <stdexcept>

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
	valFudge=30;
	areaFudge=20;
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


//	MakeDensityMatrix();
	MakeRho();
	StdMain();	
	
	/* Now redraw the map. */
	// Need to do everything after loading terrain
	m_mapController->GetMapView()->SetVertices(m_mapController);
//	MapView::QuadtreeNode* root = 
//	m_mapController->GetMapView()->createQuadtree( m_mapController->GetMapView()->GetDimension(), 0 );
//	m_mapController->GetMapView()->SetRoot(root);
	
	m_mapController->GetMapView()->SetRoot( m_mapController->GetMapView()->createQuadtree( m_mapController->GetMapView()->GetDimension(), 0 ) );

	m_mapController->GetMapView()->RefreshQuadtree();
}

std::map<int,boost::array<int,3>> Cartogram::TranslateLocations()
{

	std::ofstream myfile;
	myfile.open("C:/Users/Admin/Desktop/cartlog.txt");

	std::map<int,boost::array<int,3>> gridDensity;
	Box2D mapBox = m_mapController->GetMapModel()->GetProjectionExtents();
	double mapHeight = mapBox.dy - mapBox.y ;
	double mapWidth = mapBox.dx - mapBox.x ;
//	double mapHeight = mapBox.dy;
//	double mapWidth = mapBox.dx;
	LocationSetLayerPtr locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer(0);
	std::vector<LocationLayerPtr> locationLayers = locationSetLayer->GetAllActiveLocationLayers();
	LocationLayerPtr loc;
	
	for( uint i = 0; i < locationLayers.size() ; i++)
	{
		loc = locationLayers[i];
		
		// simple count metric. Will need to be buffed up!
//		int count = std::max(1,int(loc->GetNumSequenceLayers()*10));
		int count = std::max(1,int(loc->GetNumSequenceLayers()));
		
		// map box x and y are the lower left corner of the map.
		// map box dx and dy are the width and height of the box.
		double easting = loc->GetLocationController()->GetEasting() - mapBox.x;
	// to get the smallest y value, y - dy
	//	double northing = loc->GetLocationController()->GetNorthing() - mapBox.y;
		double northing = loc->GetLocationController()->GetNorthing() - mapBox.y;

		// Oh Boy does this look right!
		int indexX = Round(double(easting/mapWidth)*xsize);
		int indexY = Round(double((mapHeight - northing)/mapHeight)*ysize);
	//	int indexY = Round(double((northing)/mapHeight)*ysize);
		int index = indexX + indexY*xsize;

		myfile << "Cell: " + boost::lexical_cast<std::string>(index) + " Count: " + boost::lexical_cast<std::string>(count) + " lat: " + boost::lexical_cast<std::string>(northing) + " lon: " + boost::lexical_cast<std::string>(easting) + "\n";
		myfile << "\t\tgridX: "+boost::lexical_cast<std::string>(indexX) + "\tgridY: " + boost::lexical_cast<std::string>(indexY)+"\n";

		// check gridDensity
		// if already index, add count
		// else add index
	/*	if(gridDensity.count(index) > 0)
		{
			gridDensity[index]+=count;
		}
		else
		{
			gridDensity[index] = count;
		} */
		if(gridDensity.count(index) > 0)
		{
			gridDensity[index][2]+=count;

		}
		else
		{
			boost::array<int,3> a = {indexX,indexY,count};
			gridDensity[index] = a;
		}
	}
	myfile.close();
	return gridDensity;
}

void Cartogram::MakeRho()
{
	std::string tempOut = "temp.dat";
//	std::string tempOut = "C:/Users/admin/Desktop/HOOOOOOOOOOOOOLLLLYYYYCOOOOOOOOOOOOOOOW.dat";
	//	cart_makews(xsize, ysize);
//	double **rho = cart_dmalloc(xsize, ysize);
	std::vector<std::vector<double>> rho = PopulateRho();
	WriteMatrix(rho,ysize,xsize, tempOut);
}
/*
void Cartogram::MakeDensityMatrix()
{
	FILE *outfp;
	outfp = fopen("C:/Users/admin/Desktop/mapOutTest.dat","w");
//	FILE *tempOut;
//	FILE *tempIn;
//	tempOut = fopen("./temp.dat","w");
//	tempIn = fopen("./temp.dat","r");
	double** gridx2d;
	double** gridy2d;

	cart_makews(xsize, ysize);
	double **rho = cart_dmalloc(xsize, ysize);
	PopulateRho(rho);
	
//	WriteMatrix(rho,ysize,xsize, "currentRho");

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
//	fclose(tempOut);
	
	writepoints(outfp,gridx,gridy,matrixSize);
	
	gridx2d = ArrayTransform(gridx);
	gridy2d = ArrayTransform(gridy);
	
	WriteMatrix(gridx2d, ysize, xsize, "gridX2DSafe");
	WriteMatrix(gridy2d, ysize, xsize, "gridY2DSafe");
	
//	fclose(tempIn);

	// update map points
	Interpolate(gridx2d,gridy2d);
	InterpolateLocations(gridx2d, gridy2d);


	// free up space again
	cart_freews(xsize, ysize);
	free(gridx);
	free(gridy);
}
*/
int Cartogram::GetMapAverage(std::map<int,boost::array<int,3>> map)
{
	int count = 0;
	int average = 0;
	std::map<int,boost::array<int,3>>::iterator it;
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

std::vector<std::vector<double>> Cartogram::PopulateRho()
{
	// Find grid squares with locations
	std::map<int,boost::array<int,3>> locationCounts = TranslateLocations();
	std::vector<std::vector<double>> prime(ysize,std::vector<double>(xsize));
	// Find average of these values (for empyt grid squares)
	int average = GetMapAverage(locationCounts);
	double avgVal = average + OFFSET*average;
	int index=0;
	double val = 0;
	int i,j;

	Point2D xy;	
	for ( std::map<int,boost::array<int,3>>::iterator iter = locationCounts.begin(); iter != locationCounts.end();++iter ) 
	{
		// iter->second[0] = x value
		// iter->second[1] = y value
		// iter->second[2] = count
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

template< typename Matrix >
void Cartogram::Interpolate(Matrix gridx, Matrix gridy)
{
	Point3D* grid = m_mapController->GetMapModel()->GetGrid();
	int gridSize = (xsize)*(ysize);
	int ix,iy;
	double xin,yin;
	double xout,yout;
	double v1,v2,v3,v4;
	double coeff1,coeff2,coeff3,coeff4;
	double dx,dy;
	int index = 0;
	double width;
	double height;
	double minWidth;
	double minHeight;
	
	Point3D gridCell;
	Point3D debugCell;
	GeoCoord coord;

	width = m_mapController->GetMapModel()->GetWidth();
	height = m_mapController->GetMapModel()->GetHeight();

	minWidth = -1*width/2;
	minHeight = -1*height/2;

	for( int i=0; i < gridSize; ++i)
	{
		gridCell=grid[i];
		
	//	double indexY = (gridCell.x - minWidth)/width * double(xsize);
	//	double indexX = (gridCell.z - minHeight)/height * double(ysize);
		double indexY = (gridCell.z - minHeight)/height * double(ysize);
		double indexX = (gridCell.x - minWidth)/width * double(xsize);
		xin = indexX;
		yin = indexY;
	/* Check if we are outside bounds */
		
		if ((xin<0.0)||(xin>=xsize)||(yin<0.0)||(yin>=ysize)) {
			xout = gridCell.x;
			yout = gridCell.z;
		} 
		else 
		{
			
			ix = xin;
			dx = xin - ix;
			iy = yin;
			dy = yin - iy;
			
			coeff1 = (1-dx)*(1-dy);
			coeff2 = dx*(1-dy);
			coeff3 = (1-dx)*dy;
			coeff4 = dx*dy;
			v1 = gridx[ix][iy];
			v2 = gridx[ix+1][iy];
			v3 = gridx[ix][iy+1];
			v4 = gridx[ix+1][iy+1];
			xout = (1-dx)*(1-dy)*gridx[ix][iy] + dx*(1-dy)*gridx[ix+1][iy]
				+ (1-dx)*dy*gridx[ix][iy+1] + dx*dy*gridx[ix+1][iy+1];
			yout = (1-dx)*(1-dy)*gridy[ix][iy] + dx*(1-dy)*gridy[ix+1][iy]
				+ (1-dx)*dy*gridy[ix][iy+1] + dx*dy*gridy[ix+1][iy+1];
			// reusing grid cell
			debugCell = gridCell;
			gridCell.x = (xout * width) / double(xsize) + minWidth;
			gridCell.z = (yout * height) / double(ysize) + minHeight;

			grid[i].x = gridCell.x;
			grid[i].z = gridCell.z;
		}
	}
}

template< typename Matrix >
void Cartogram::InterpolateLocations(Matrix gridx, Matrix gridy)
{
	/*
	grid -1,-1 = top left corner (max y, min x)
	*/ 

	int ix,iy;
	double xin,yin;
	double xout,yout,yCorrection;
	double dx,dy;
	double width;
	double height;
	double minWidth;
	double minHeight;
	
	Point3D gridCell;
	
	Point3D tempCell;
	GeoCoord tempCoord;

	Box2D mapBox = m_mapController->GetMapModel()->GetProjectionExtents();
	height = mapBox.dy - mapBox.y;
	width = mapBox.dx - mapBox.x;
	minWidth = mapBox.x;
	minHeight = mapBox.y;
	
	LocationSetLayerPtr locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer(0);
	std::vector<LocationLayerPtr> locationLayers = locationSetLayer->GetAllActiveLocationLayers();
	LocationLayerPtr loc;
	
	for( uint i = 0; i < locationLayers.size() ; i++)
	{
		loc = locationLayers[i];
		
		gridCell.x = loc->GetLocationController()->GetEasting();
		gridCell.z = loc->GetLocationController()->GetNorthing();

	//	yin = abs(gridCell.x - minWidth)/width * double(xsize);
	//	xin = abs((gridCell.z - minHeight))/height * double(ysize);
		xin = (gridCell.x - minWidth)/width * double(xsize);
		yin = (gridCell.z - minHeight)/height * double(ysize);
	//	yin = abs(gridCell.z) / height * double(ysize);
	// Check if we are outside bounds 
		
		if ((xin<0.0)||(xin>=ysize)||(yin<0.0)||(yin>=xsize)) {
		//	xout = gridCell.x;
		//	yout = gridCell.z;
			continue;
		} 
		else 
		{
			ix = xin;
			dx = xin - ix;
			iy = yin;
			dy = yin - iy;
			int iyy = yin;
			
			xout = (1-dx)*(1-dy)*gridx[ix][iy] + dx*(1-dy)*gridx[ix+1][iy]
				+ (1-dx)*dy*gridx[ix][iy+1] + dx*dy*gridx[ix+1][iy+1];
			yout = (1-dx)*(1-dy)*gridy[ix][iyy] + dx*(1-dy)*gridy[ix+1][iyy]
				+ (1-dx)*dy*gridy[ix][iyy+1] + dx*dy*gridy[ix+1][iyy+1];
		
		}	
	
		// just checking positions
		tempCoord.easting = gridCell.x;
		tempCoord.northing = gridCell.z;
		m_mapController->GetMapModel()->GeoToGrid(tempCoord, tempCell);

		tempCell.x = gridCell.x;
		tempCell.z = gridCell.z;
		
		// Y direction shifts are occuring at the correct magnitude but in the wrong direction. Correct it!
	//	double xCorrection = yin - xout + yin;
	//	yCorrection  = xin - yout + xin ;
		yCorrection =  yout;
		gridCell.x = ( xout ) / double(xsize) * width + minWidth;	
		gridCell.z = (yCorrection) / double(ysize) *height + minHeight;
		gridCell.y = yout / double(ysize) * height + minHeight;
		loc->GetLocationController()->GetLocationModel()->SetEasting(gridCell.x);
		loc->GetLocationController()->GetLocationModel()->SetNorthing(gridCell.z);
	}
}

/*
template< typename Matrix >
void Cartogram::InterpolateLocations(Matrix gridx, Matrix gridy)
{
	
	Point3D* grid = m_mapController->GetMapModel()->GetGrid();
	double easting,northing,diffEast,diffNorth;

	Point3D tempCell;
	GeoCoord tempCoord;
	GeoCoord newCoord;
	double yTemp,xTemp;

	Box2D mapBox = m_mapController->GetMapModel()->GetProjectionExtents();
	double mapHeight = mapBox.dy - mapBox.y ;
	double mapWidth = mapBox.dx - mapBox.x ;
	
	LocationSetLayerPtr locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer(0);
	std::vector<LocationLayerPtr> locationLayers = locationSetLayer->GetAllActiveLocationLayers();
	LocationLayerPtr loc;
	
	for( uint i = 0; i < locationLayers.size() ; i++)
	{
		loc = locationLayers[i];
		tempCoord.easting = loc->GetLocationController()->GetEasting();
		tempCoord.northing = loc->GetLocationController()->GetNorthing();
		
		easting = loc->GetLocationController()->GetEasting() - mapBox.x;
		// to get the smallest y value, y - dy
		northing = loc->GetLocationController()->GetNorthing() - mapBox.y;
		
	//	easting = loc->GetLocationController()->GetLocationModel()->GetEasting();
	//	northing = loc->GetLocationController()->GetLocationModel()->GetNorthing();

	//	int indexX = ceil(double(easting/mapWidth)*xsize);
	//	int indexY = ceil(double((mapHeight - northing)/mapHeight)*ysize);

	//	int indexX = Round(double(easting/mapWidth)*xsize);
		int indexX = Round(double(easting/mapWidth)*(xsize));
	//	int indexY = Round(double((mapHeight - northing)/mapHeight)*ysize);
		int indexY = ysize - Round(double(northing/mapHeight)*(ysize));
	//	int indexY = mapHeight - Round(double(northing/mapHeight)*ysize);
		int index = indexX + indexY*xsize;
		
		tempCell = grid[index];

	//	m_mapController->GetMapModel()->GeoToGrid(tempCoord, tempCell);
		m_mapController->GetMapModel()->GridToGeo(tempCell,newCoord);
		
		// adjust the Y value to be bottom up instead of top down
	//	newCoord.northing = mapBox.y - newCoord.northing;
	//	yCorrection  = xin - yout + xin ;
	//	diffNorth = 2 * ( northing + mapBox.y ) - newCoord.northing;
		diffNorth = newCoord.northing;
		diffEast= newCoord.easting;

		loc->GetLocationController()->GetLocationModel()->SetEasting( diffEast );
		loc->GetLocationController()->GetLocationModel()->SetNorthing( diffNorth );
	}
}
*/
/*
std::vector<std::vector<double>> Cartogram::ArrayTransform(double * grid)
{
	std::vector<std::vector<double>> gridx(ysize+1,std::vector<double>(xsize+1));
//	boost::array<boost::array<double,xsize+1>,ysize+1> gridx;
	int index = 0;
	for( int i =0; i <= ysize ; ++i)
//	for( int i = ysize; i >= 0 ; --i)
	{
	//	gridx[i] = new double[xsize];
		for( int j = 0; j<= xsize ; ++j)
		{
			double blarg = grid[index];
			gridx[i][j] = blarg;
			index++;
		}
	}
	return gridx;
}
*/
template< typename Matrix >
void Cartogram::ArrayTransform(double * grid, Matrix &transGrid)
{
//	boost::array<boost::array<double,xsize+1>,ysize+1> gridx;
	int index = 0;
	for( int i =0; i <= ysize ; ++i)
//	for( int i = ysize; i >= 0 ; --i)
	{
	//	gridx[i] = new double[xsize];
		for( int j = 0; j<= xsize ; ++j)
		{
			double blarg = grid[index];
			transGrid[j][i] = blarg;
			index++;
		}
	}
//	return transGrid;
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
	
	try{
	myfile.open(fileName.c_str(), std::ofstream::out);
	}catch(std::ifstream::failure &writeErr){
		Log::Inst().Warning("Could not open RHO file.");
	}
	if(!myfile)
	{
		Log::Inst().Warning("Could not open RHO file.");
	}

	for(int i = 0; i < row; i++)
	{
		for( int j = 0; j < col; j++)
		{
			myfile << boost::lexical_cast<std::string>(rho[i][j]) + "\t"; 
		}
		myfile << "\n";
		myfile.flush();
	}

	myfile.close();
}
/*
	Taken straight from Newmans main implementation for debugging purposes.
	Essentially just make sure my grid making process isn't garbage.

*/
void Cartogram::StdMain()
{
	double *gridx,*gridy;  // Array for grid points
	double **rho;          // Initial population density
//	FILE *infp;
	FILE *outfp;
	FILE *tempOut;
	FILE *tempIn;
	std::string fileName = GenGIS::App::Inst().GetExeDir().mb_str();
	fileName += "temp.dat";
	

	tempOut = fopen("./tempRho.dat","w");
	tempIn = fopen(fileName.c_str(),"r");

	int matrixSize = (xsize+1)*(ysize+1);

//	std::string popdat = "C:/Users/admin/Documents/cartograms/cart-1.2.2/cart-1.2.2/uspop.dat";
//	std::string popdat = "C:/Users/admin/Dropbox/currentRho.txt";
//	outfp = fopen("C:/Users/admin/Desktop/outpop.dat","w");
	outfp = fopen("C:/Users/admin/Desktop/standardTest.dat","w");
//	infp = fopen(popdat.c_str(),"r");
	cart_makews(xsize,ysize);

	rho = cart_dmalloc(xsize,ysize);
//	if (readpop(infp,rho,xsize,ysize)) {
	if (readpop(tempIn,rho,xsize,ysize)) {
		Log::Inst().Warning("Density file b f'ed son");
	}
	cart_transform(rho,xsize,ysize);
    cart_dfree(rho);

	/* Create the grid of points */

	gridx = (double*) malloc(matrixSize*sizeof(double));
	gridy = (double*) malloc(matrixSize*sizeof(double));

	CreateGrid(gridx,gridy,xsize,ysize);

	cart_makecart(gridx, gridy, matrixSize, xsize, ysize, 0.0);
	
//	writepoints(tempOut,gridx,gridy,matrixSize);

	writepoints(outfp,gridx,gridy,matrixSize);
	
	fclose( outfp );
	outfp = fopen("C:/Users/admin/Desktop/standardTest.dat","r");
	/*
	*	Cartogram stuff is done, lets interpolate junk!	
	*/

	// My Adaptation
//	std::vector<std::vector<double>> gridx2dMe = ArrayTransform(gridx);
//	std::vector<std::vector<double>> gridy2dMe = ArrayTransform(gridy);

	std::vector<std::vector<double>> gridx2d(xsize+1,std::vector<double>(ysize+1));
	std::vector<std::vector<double>> gridy2d(xsize+1,std::vector<double>(ysize+1));
	std::vector<std::vector<double>> gridy2dFlip(xsize+1,std::vector<double>(ysize+1));
	ArrayTransform(gridx, gridx2d);
	ArrayTransform(gridy, gridy2d);

	MatrixFlip( gridy2d, gridy2dFlip );
	// Load in matrix like Newman code
//	readpoints(outfp, gridx2d, gridy2d,xsize,ysize);

	
	// matrix, numRow, numCol, name
	WriteMatrix(gridx2d, xsize+1, ysize+1, "gridX2DSafe");
	WriteMatrix(gridy2dFlip, xsize+1, ysize+1, "gridY2DSafe");

//	fclose(tempIn);

	// update map points
	Interpolate(gridx2d,gridy2d);
	InterpolateLocations(gridx2d, gridy2d);
//	Interpolate(gridx2d,gridy2d);
	
	// free up space again
	cart_freews(xsize, ysize);
	free(gridx);
	free(gridy);
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

int Cartogram::Round(double val)
{
	return floor(val);
	if( val - floor(val) >= 0.5 )
		return int(ceil(val));
	else
		return int(floor(val));
}

template< typename Matrix >
void Cartogram::MatrixFlip(Matrix matrix, Matrix &flip)
{
	int flipx = xsize;
	for(int x = 0; x <= xsize; x++)
	{
		flip[flipx-x] = matrix[x];
	}
}