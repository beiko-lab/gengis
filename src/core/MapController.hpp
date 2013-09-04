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

#ifndef _GENGIS_MAP_CONTROLLER_
#define _GENGIS_MAP_CONTROLLER_

#include "../core/Precompiled.hpp"
#include "../core/MapModel.hpp"
#include "../core/MapView.hpp"

namespace GenGIS
{

	/**
	* @brief Controller for map model.
	*/
	class MapController
	{
	public:
		/** 
		* @brief Construction. 
		*/
		explicit MapController(): m_bMapLoaded(false)
		{
			m_mapModel = MapModelPtr(new MapModel());
			m_mapView  = MapViewPtr(new MapView(m_mapModel));
		}

		/** Destructor. */
		~MapController() {}

		/**
		* @brief Get array of grid coordinates.
		* @return Grid coordinate array.
		*/
		Point3D* GetGrid() const
		{
			return m_mapModel->GetGrid();
		}

		/**
		* @brief Convert geographic coordinates to grid coordinates.
		* @param mapCoord Geographic coordinates.
		* @param gridCoord Grid coordinate.
		*/
		void GeoToGrid(const GeoCoord &mapCoord, Point3D &gridCoord)
		{
			return m_mapModel->GeoToGrid(mapCoord, gridCoord);
		}

		/**
		* @brief Convert lat/long coordinates to grid coordinates.
		* @param mapCoord Lat/long coordinates.
		* @param gridCoord Grid coordinate.
		*/
		void LatLongToGrid(const GeoCoord &mapCoord, Point3D &gridCoord)
		{
			return m_mapModel->LatLongToGrid(mapCoord, gridCoord);
		}

		/**
		* @brief Convert grid coordinates to geographic coordinates.
		* @param gridCoord Grid coordinate.
		* @param mapCoord Geographic coordinates.
		*/
		void GridToGeo(const Point3D &gridCoord, GeoCoord &mapCoord)
		{
			return m_mapModel->GridToGeo(gridCoord, mapCoord);
		}

		/**
		* @brief Get DEM file header information.
		* @return DEM header information.
		*/
		FileHeader* GetHeader() const
		{
			return m_mapModel->GetHeader();
		}

		/**
		* @brief Get DEM metadata information.
		* @return DEM metadata information.
		*/
		FileMetaData* GetMetaData() const
		{
			return m_mapModel->GetMetaData();
		}

		/**
		* @brief Get transformation tool.
		* @return Transformation tool.
		*/
		OGRCoordinateTransformation* GetTransformationTool()
		{
			return m_mapModel->GetTransformationTool();
		}

		/**
		* @brief Lower, left and upper, right corners of map after projection.
		* @return Width.
		*/
		Box2D GetProjectionExtents() const { return m_mapModel->GetProjectionExtents(); }

		Box2D GetMapBorders() const { return m_mapModel->GetMetaData()->extents; };

		/**
		* @brief Width (x-dimension) of DEM map. 
		* @return Width.
		*/
		float GetWidth() const
		{
			return m_mapModel->GetWidth();
		}

		/**
		* @brief Height (z-dimension) of DEM map. 
		* @return Height.
		*/
		float GetHeight() const
		{
			return m_mapModel->GetHeight();
		}

		/**
		* @brief Elevation (y-dimension) of DEM map at specified point.
		* @return Elevation.
		*/
		float GetElevation(float gridX, float gridZ) const
		{
			return m_mapModel->GetElevation(gridX, gridZ);
		}

		/**	 
		* @brief Elevation (y-dimension) of DEM map at specified point multiplied by the vertical exaggeration.	 
		* @return Exaggerated elevation.	 
		*/	
		float GetExaggeratedElevation(float gridX, float gridZ) const
		{
			return m_mapModel->GetExaggeratedElevation(gridX, gridZ);
		}	

		/** Set flag indicating if map model contains elevation information. */	
		void SetElevationMap(bool bElevationMap)
		{
			m_mapModel->SetElevationMap(bElevationMap);
		}	

		/** Get flag indicating if map model contains elevation information. */	
		bool GetElevationMap() const
		{
			return m_mapModel->GetElevationMap();
		}	

		/** Get OpenGL (3D) map view. */
		MapViewPtr GetMapView() const
		{
			return m_mapView;
		}

		/** Set minimum elevation (in grid space). */
		void SetMinElevationGridSpace(float min)
		{
			m_mapModel->SetMinElevationGridSpace(min);
		}

		/** Get minimum elevation (in grid space). */
		float GetMinElevationGridSpace() const
		{
			return m_mapModel->GetMinElevationGridSpace();
		}

		/** Set maximum elevation (in grid space). */
		void SetMaxElevationGridSpace(float max)
		{
			m_mapModel->SetMaxElevationGridSpace(max);
		}

		/** Get maximum elevation (in grid space). */
		float GetMaxElevationGridSpace() const
		{
			return m_mapModel->GetMaxElevationGridSpace();
		}

		/** Set minimum elevation (in world space). */
		void SetMinElevation(float min)
		{
			m_mapModel->SetMinElevation(min);
		}

		/** Get minimum elevation (in world space). */
		float GetMinElevation() const
		{
			return m_mapModel->GetMinElevation();
		}

		/** Set maximum elevation (in world space). */
		void SetMaxElevation(float max)
		{
			m_mapModel->SetMaxElevation(max);
		}

		/** Get maximum elevation (in world space). */
		float GetMaxElevation() const
		{
			return m_mapModel->GetMaxElevation();
		}

		/** Get vertical exaggeration of elevation. */	
		float GetVerticalExaggeration() const
		{
			return m_mapModel->GetVerticalExaggeration();
		}

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state)
		{
			m_mapModel->SetActive(state);
		}

		/** Get active state of layer. */
		bool IsActive() const
		{
			return m_mapModel->IsActive();
		}

		/** Toggle active state of layer. */
		void ToggleActive()
		{
			m_mapModel->ToggleActive();
		}

		/** Get Map Model. */
		MapModelPtr GetMapModel() const
		{
			return m_mapModel;
		}

		/** 
		* @brief Project screen coodinates onto terrain. 
		* @param screenPt Point to project onto terrain.
		* @param bInvertY Flag indicating if y coordinate should be inverted (i.e., should be true if Y is taken to be the top of the screen.)*/
		Point3D ProjectToTerrain(const Point2D& screenPt, bool bInvertY = false);

		/** Project 3D screen coodinates to screen coodinates. */
		Point3D ProjectToScreen(const Point3D& worldPt);

		/** Check if terrain is being shown as a wireframe. */
		bool IsWireFrame();

		/** Set whether terrain should be rendered as a wireframe (true) or with texturing (false). */
		void SetWireFrame(bool state);

//		/** Set vertical exaggeration of elevation. */
//		void SetVerticalExaggeration(float exaggeration);

//		/** Get vertical exaggeration of elevation. */	
//		float GetVerticalExaggeration() const;

		/** Flag indicating if a map is loaded. */
		bool IsLoaded() const
		{
			return m_bMapLoaded;
		}

		/** Set flag indicating if a map is loaded. */
		void SetLoaded(bool state)
		{
			m_bMapLoaded = state;
		}

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Map model. */
		MapModelPtr m_mapModel;

		/** OpenGL (3D) view of map model. */
		MapViewPtr m_mapView;

		/** Flag indicating if a map is loaded. */
		bool m_bMapLoaded;
	};
}

#endif
