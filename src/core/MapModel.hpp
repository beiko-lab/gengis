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

#ifndef _GENGIS_MAP_MODEL_
#define _GENGIS_MAP_MODEL_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/** Structure specifying DEM header information. */
	typedef struct sFILE_HEADER 
	{
		Box2D projExtents; // lower, left and upper, right corners of map after projection

		int nCols;         // number of columns
		int nRows;         // number of rows
		double width;       // width of map in grid space
		double height;      // height of map in grid space
		double cellSizeX;   // in grid coordinates
		double cellSizeZ;   // in grid coordinates
		double scaleFactor; // scale between projection space and grid (model) space 
		// (1 projection space unit = scaleFactor grid units)

		double noDataValue; // value assigned to DEM cells that have no actual value
		


		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & projExtents; // Box2D
			ar & nCols;       // int
			ar & nRows;       // int
			ar & width;       // float
			ar & height;      // float
			ar & cellSizeX;   // float
			ar & cellSizeZ;   // float
			ar & scaleFactor; // float
			ar & noDataValue; // float
		}
	} FileHeader;


	/** Structure specifying DEM metadata information. */
	typedef struct sFILE_METADATA
	{
		std::string driverDesc;           // driver description
		std::string driverMetaData;       // addition driver information
		std::string projection;           // string indicating projection parameters
		std::string colourInterpretation; // not sure what this is?
		std::string dataType;             // type of data stored in DEM

		int xSize;        // number columns in DEM
		int ySize;        // number rows in DEM
		int bands;        // number of bands in DEM

		double originX;    // x-coordinate origin of DEM
		double originY;    // y-coordinate origin of DEM
		Box2D extents;    // extents of DEM (lower, left corner to upper, right)

		double pixelSizeX; // size of cells in geographical coordinate space
		double pixelSizeY; // size of cells in geographical coordinate space

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & driverDesc;           // std::string
			ar & driverMetaData;       // std::string
			ar & projection;           // std::string
			ar & colourInterpretation; // std::string
			ar & dataType;             // std::string

			ar & xSize;      // int
			ar & ySize;      // int
			ar & bands;      // int

			ar & originX;    // float
			ar & originY;    // float
			ar & extents;    // Box2D

			ar & pixelSizeX; // float
			ar & pixelSizeY; // float
		}
	} FileMetaData;

	/**
	* @brief Model of digital elevation map (DEM).
	*/
	class MapModel: private Uncopyable  
	{
	public:
		/** Construction. */
		explicit MapModel();

		/** Destructor. */
		~MapModel();

		/**
		* @brief Get array of grid coordinates.
		* @return Grid coordinate array.
		*/
		Point3D* GetGrid() { return m_grid; }

		/**
		* @brief Set array of grid coordinates.
		* @param grid Grid coordinate array.
		*/
		void SetGrid( Point3D* grid );

		/**
		* @brief Convert geographic coordinates to grid coordinates.
		* @param mapCoord Geographic coordinates.
		* @param gridCoord Grid coordinate.
		*/
		void GeoToGrid(const GeoCoord &mapCoord, Point3D &gridCoord);

		/**
		* @brief Convert lat/long coordinates to grid coordinates.
		* @param mapCoord Lat/long coordinates.
		* @param gridCoord Grid coordinate.
		*/
		void LatLongToGrid(const GeoCoord &mapCoord, Point3D &gridCoord);

		/**
		* @brief Convert grid coordinates to geographic coordinates.
		* @param gridCoord Grid coordinate.
		* @param mapCoord Geographic coordinates.
		*/
		void GridToGeo(const Point3D &gridCoord, GeoCoord &mapCoord);

		/**
		* @brief Get DEM file header information.
		* @return DEM header information.
		*/
		FileHeader* GetHeader() { return &m_fileHeader; }

		/**
		* @brief Get DEM file header information.
		* @return DEM header information.
		*/
		const FileHeader* const GetHeader() const { return &m_fileHeader; }

		/**
		* @brief Set transformation tool.
		* @param tool New transformation tool.
		*/
		void SetTransformationTool(OGRCoordinateTransformation* tool) {	m_oCT = tool; }

		/**
		* @brief Get transformation tool.
		* @return Transformation tool.
		*/
		OGRCoordinateTransformation* GetTransformationTool()	{	return m_oCT;	}

		/**
		* @brief Lower, left and upper, right corners of map after projection.
		* @return Width.
		*/
		Box2D GetProjectionExtents() const { return m_fileHeader.projExtents; }

		/**
		* @brief Width (x-dimension) of DEM map. 
		* @return Width.
		*/
		float GetWidth() const {  return m_fileHeader.width; }

		/**
		* @brief Height (z-dimension) of DEM map. 
		* @return Height.
		*/
		float GetHeight() const { return m_fileHeader.height; }

		/**
		* @brief Elevation (y-dimension) of DEM map at specified point.
		* @return Elevation.
		*/
		float GetElevation(float gridX, float gridZ) const;

		/**
		* @brief Elevation (y-dimension) of DEM map at specified point multiplied by the vertical exaggeration.
		* @return Exaggerated elevation.
		*/
		float GetExaggeratedElevation(float gridX, float gridZ) const;

		/**
		* @brief Get DEM file metadata information.
		* @return Metadata information.
		*/
		FileMetaData* GetMetaData() { return &m_metaData; }

		/** Set minimum elevation (in grid space). */
		void SetMinElevationGridSpace(float min) { m_minElevationGridSpace = min; }

		/** Get minimum elevation (in grid space). */
		float GetMinElevationGridSpace() const { return m_minElevationGridSpace; }

		/** Set maximum elevation (in grid space). */
		void SetMaxElevationGridSpace(float max) { m_maxElevationGridSpace = max; }

		/** Get maximum elevation (in grid space). */
		float GetMaxElevationGridSpace() const { return m_maxElevationGridSpace; }

		/** Set minimum elevation. */
		void SetMinElevation(float min) { m_minElevation = min; }

		/** Get minimum elevation. */
		float GetMinElevation() const { return m_minElevation; }

		/** Set maximum elevation (in grid coordinates). */
		void SetMaxElevation(float max) { m_maxElevation = max; }

		/** Get maximum elevation (in grid coordinates). */
		float GetMaxElevation() const { return m_maxElevation; }

		/** Set vertical exaggeration of elevation. */	
		void SetVerticalExaggeration(float exaggeration) { m_verticalExaggeration = exaggeration; }	

		/** Get vertical exaggeration of elevation. */	
		float GetVerticalExaggeration() const { return m_verticalExaggeration; } 	

		/** Set flag indicating if map model contains elevation information. */	
		void SetElevationMap(bool bElevationMap) { m_bElevationMap = bElevationMap; }	

		/** Get flag indicating if map model contains elevation information. */	
		bool GetElevationMap() const { return m_bElevationMap; }	

		/** Get texture for terrain. */		
		MapTexturePtr GetTexture() const { return m_mapTexture; }

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state) { m_bActive = state; }

		/** Get active state of layer. */
		bool IsActive() const { return m_bActive; }

		/** Toggle active state of layer. */
		void ToggleActive() { m_bActive = !m_bActive; }

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Flag indicating if this sequence is currently in the active set. */
		bool m_bActive;

		/** DEM header information. */
		FileHeader m_fileHeader;

		/** DEM metadata information. */
		FileMetaData m_metaData;

		/** DEM grid specifying (x,y,z) coordinated. */
		Point3D* m_grid;

		/** GDAL projection transformation utility. */
		OGRCoordinateTransformation *m_oCT;

		/** Minimum elevation (in grid space). */
		float m_minElevationGridSpace;

		/** Maximum elevation (in grid space). */
		float m_maxElevationGridSpace;

		/** Minimum elevation. */
		float m_minElevation;

		/** Maximum elevation. */
		float m_maxElevation;

		/** Amount to exaggerate elevation by. */	
		float m_verticalExaggeration;	

		/** Flag indicating if map model contains elevation information. */	
		bool m_bElevationMap;	

		/** Texture for map. */	
		MapTexturePtr m_mapTexture;
	};
}

#endif
