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

#include "../core/MapModel.hpp"
#include "../core/MapTexture.hpp"
#include "../core/StudyController.hpp"
#include "../core/App.hpp"

#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"

using namespace GenGIS;


MapModel::MapModel( void ) :
	m_oCT(NULL), m_verticalExaggeration(0.0), m_grid(NULL), m_mapTexture(new MapTexture()), 
	m_bElevationMap(false), m_bActive(true)
{

}

MapModel::~MapModel( void ) 
{
	if(m_grid != NULL) 
		delete[] m_grid; 
}

void MapModel::SetGrid( Point3D* grid ) 
{
	if(m_grid != NULL) 
		delete[] m_grid;
	
	m_grid = grid;
}

template<class Archive>
void MapModel::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_bActive;               // bool
	ar & m_fileHeader;            // FileHeader
	ar & m_metaData;              // FileMetaData
	ar & m_minElevationGridSpace; // float
	ar & m_maxElevationGridSpace; // float
	ar & m_minElevation;          // float
	ar & m_maxElevation;          // float
	ar & m_verticalExaggeration;  // float
	ar & m_bElevationMap;         // bool
	ar & m_mapTexture;            // MapTexturePtr
}
template void MapModel::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void MapModel::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void MapModel::GeoToGrid(const GeoCoord &mapCoord, Point3D &gridCoord) 
{
	FileHeader* header = GetHeader();
	Box2D projExtents = header->projExtents;

	double dougtheslug = projExtents.Width();
	double dougtheslag = projExtents.Height();
	gridCoord.x = ((mapCoord.easting - projExtents.x) / projExtents.Width()) * header->width - header->width*0.5f;
	gridCoord.z = ((projExtents.dy - mapCoord.northing) / projExtents.Height()) * header->height - header->height*0.5f;

	gridCoord.y = GetExaggeratedElevation(gridCoord.x, gridCoord.z);
}

void MapModel::LatLongToGrid(const GeoCoord &mapCoord, Point3D &gridCoord) 
{
	double latitude = mapCoord.northing;
	double longitude = mapCoord.easting;

	// transform lat/lon coordinates to projected coordinates
	StudyControllerPtr studyController = App::Inst().GetStudyController();
	if(studyController->IsProjectData())
	{
		ProjectionToolPtr projTool = studyController->GetProjectionTool();
		if(!projTool->Transform(1, &longitude, &latitude))
		{
			Log::Inst().Warning("(Warning) Failed to project data.");
		}
	}

	GeoToGrid(GeoCoord(longitude, latitude), gridCoord);
}

void MapModel::GridToGeo(const Point3D &gridCoord, GeoCoord &mapCoord) 
{
	FileHeader* header = GetHeader();
	Box2D projExtents = header->projExtents;
	StudyControllerPtr studyController = App::Inst().GetStudyController();
	if(studyController->IsProjectData())
	{
		ProjectionToolPtr projTool = studyController->GetProjectionTool();
		OGRCoordinateTransformation *poTransform;
		if(projTool!= NULL)
		{
			OGRSpatialReference* sourceProj = projTool-> GetSourceCS();
			OGRSpatialReference* targetProj = projTool-> GetTargetCS() ;
			poTransform = OGRCreateCoordinateTransformation(targetProj, sourceProj);			
		}

		if(!poTransform->Transform(1, &projExtents.x, &projExtents.y))
		{
			Log::Inst().Warning("(Warning) Failed to project data.");
		}
		if(!poTransform->Transform(1, &projExtents.dx, &projExtents.dy))
		{
			Log::Inst().Warning("(Warning) Failed to project data.");
		}
	}

	// x grid coordinates go from -1 to 1
	mapCoord.easting = projExtents.x + 0.5*(gridCoord.x + 1)* projExtents.Width();

	// z grid coordinates go from -height/2 to height/2
	float scaleFactor = 1.0f / (header->height*0.5);
	mapCoord.northing = projExtents.y + 0.5*(-gridCoord.z*scaleFactor + 1)*projExtents.Height();
}

float MapModel::GetElevation(float gridX, float gridZ) const
{
	if ( m_grid == NULL )
		return 0;

	float indexX = (gridX + GetHeader()->width*0.5)  / GetHeader()->cellSizeX;
	float indexY = (gridZ + GetHeader()->height*0.5) / GetHeader()->cellSizeZ;
	float iX = floor(indexX);
	float iY = floor(indexY);
	float dX = indexX - iX;
	float dY = indexY - iY;

	if(iX >= 0 && iX < (GetHeader()->nCols-1) && iY >= 0 && iY < (GetHeader()->nRows-1))
	{
		// find 2D interpolation of elevation
		float bottomStartX = m_grid[int(iX + iY*GetHeader()->nCols)].y;
		float bottomEndX = m_grid[int(iX+1 + iY*GetHeader()->nCols)].y;
		float elevBottom = bottomStartX + (bottomEndX - bottomStartX)*dX;

		float topStartX = m_grid[int(iX + (iY+1)*GetHeader()->nCols)].y;
		float topEndX = m_grid[int(iX+1 + (iY+1)*GetHeader()->nCols)].y;
		float elevTop = topStartX + (topEndX - topStartX)*dX;

		float elev = elevBottom + (elevTop - elevBottom)*dY;

		return elev;
	}
	else
	{
		// place any object that are beyond the map extents at an elevation of zero
		return 0;
	}
}

float MapModel::GetExaggeratedElevation(float gridX, float gridZ) const
{ 
	return m_verticalExaggeration*GetElevation(gridX, gridZ); 
}
