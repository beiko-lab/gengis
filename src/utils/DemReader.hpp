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

#ifndef _GENGIS_DEMREADER_
#define _GENGIS_DEMREADER_

#include "../core/Precompiled.hpp"

#include "../core/MapModel.hpp"

namespace GenGIS
{

	/**
	* @class DemReader
	* @brief Open digital elevation map (DEM) files.
	*/
	class DemReader: private Uncopyable 
	{
	public:
		/**
		* @brief Open DEM file.
		* @param fullPath File to open.
		* @param studyController Study controller which specified the projection to be used.
		* @param mapController Map controller corresponding to the map being loaded.
		* @param progressDlg Progress dialog used to indicate loading progress.
		* @return True if DEM successfullly built, otherwise False.
		*/
		bool Open(std::wstring fullPath, StudyControllerPtr studyController,
			MapControllerPtr mapController, ProgressDlgPtr progressDlg = ProgressDlgPtr());

	private:
		/**
		* @brief Report metadata information in DEM file.
		* @param gdalDataset Pointer to GDAL dataset.
		*/
		void MetaDataInfo(GDALDataset* gdalDataset, MapControllerPtr mapController);

		/**
		* @brief Setup the desired projection.
		* @param gdalDataset Pointer to GDAL dataset.
		* @param studyController Study controller which specified the projection to be used.
		* @param adfGeoTransform Trnsformation information read from file.
		* @param nCols Number of columns in input file.
		* @param nRows Number of rows in input file.
		* @param bElevationMap Flag indicating if we have an elevation (true) or flat (false) map
		*/
		bool SetupProjection(GDALDataset* gdalDataset, StudyControllerPtr studyController, double* adfGeoTransform, uint nCols, uint nRows, bool bElevationMap);

		bool TransformToGridSpace(Point3D* grid, FileHeader* header, float* elevations, bool bElevationMap, 
			float& minElevation, float& maxElevation, ProgressDlgPtr progressDlg = ProgressDlgPtr());

		/**
		* @brief Build texture for terrain.
		* @param progressDlg Progress dialog used to indicate loading progress.
		*/
		bool BuildTerrainTexture(GDALDataset* gdalDataset, MapControllerPtr mapController, ProgressDlgPtr progressDlg);
	};

}

#endif
