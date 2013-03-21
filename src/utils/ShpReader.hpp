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

#ifndef _GENGIS_SHPREADER_
#define _GENGIS_SHPREADER_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/**
	* @class ShpReader
	* @brief Open ESRI Shapefiles.
	*/
	class ShpReader: private Uncopyable 
	{
	public:
		/**
		* @brief Open Shapefile.
		* @param fullPath File to open.
		* @param studyController Study controller which specified the projection to be used.
		* @param vectorMapController Vector Map controller corresponding to the vector map being loaded.
		* @param progressDlg Progress dialog used to indicate loading progress.
		* @return True if DEM successfullly built, otherwise False.
		*/
		bool Open(std::wstring fullPath, StudyControllerPtr studyController,
			VectorMapControllerPtr vectorMapController, ProgressDlgPtr progressDlg = ProgressDlgPtr());

		bool SetupVectorProjection(OGRDataSource* OGRDataset, StudyControllerPtr studyController, OGRLayer* poLayer, VectorMapControllerPtr vectorMapController);
		bool SetupVectorScaling(VectorMapModelPtr vectorMapModel, ProgressDlgPtr progressDlg);
        void VectorMetaDataInfo(OGRDataSource* OGRDataset, StudyControllerPtr studyController, VectorMapControllerPtr vectorMapController);


	private:
		ProjectionToolPtr poTransform;
		bool needProjection;

	};
}

#endif
