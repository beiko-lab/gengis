//=======================================================================
// Author: Justin Trainor
//
// Copyright 2014 Justin Trainor
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

#ifndef _GENGIS_LOCATION_POLYGONS_
#define _GENGIS_LOCATION_POLYGONS_

#include "../core/Precompiled.hpp"

#include "../utils/Colour.hpp"
#include "../core/PolygonModel.hpp"


namespace GenGIS
{
	class LocationPolygons
	{
	public:
		
		//Determines sorting algorithm for locations
		//Descriptions can be found in LocationSetPropertiesDlg::SetAlgorithmDescription(int algorithm);
		//Editing the options in the choice box can be done in the LocationSetProperties wxFormBuilder
		enum ALGORITHM {CONVEX_HULL, BLANK};


	public:

		//Constructor
		LocationPolygons();

		//Destructor
		~LocationPolygons();

		//Generate polygons
		void InitPolygons();

		//Render polygons
		void Render();

		/*Sorts the given polygon so that its convex hull is given by the first points up to the member variable lastVertexIndex 
		  source for algorithm: http://en.wikipedia.org/wiki/Graham_scan */
		void ConvexHull(PolygonModelPtr poly);	

		//Function used for std::sort to ensure the same colours are grouped together
		static bool SortByColour(LocationLayerPtr p1, LocationLayerPtr p2);
		
		//Used to determine if 3 points are clockwise, counter-clockwise, or colinear
		double CCW(Point3D p1, Point3D p2, Point3D p3);

		//Swaps the contents of a given vector of points at indices a and b
		void SwapIndices(std::vector<Point3D> &v, int a, int b);

		//Functions for accessing 'general' variables
		bool IsVisible()												{ return m_bVisible; }
		bool GetPolygonsChanged()										{ return m_polygonsChanged; }
		bool GetAutoAdjustElevationStatus()								{ return m_autoAdjustElevation; }
		bool GetIncreasingElevationStatus()								{ return m_increasingElevation; }
		bool GetSmoothPolygonStatus()									{ return m_smoothPolygons; }
		float GetElevationOffset()										{ return m_elevationOffset; }
		float GetElevation()											{ return m_inputElevation; }
		float GetFillOpacity()											{ return m_fillOpacity; }
		ALGORITHM GetAlgorithm()										{ return m_sortBy; }
		std::vector<PolygonModelPtr> GetPolygons()						{ return m_polygons; }
		std::vector<LocationLayerPtr> GetCurrentLocationSet()			{ return m_currentLocationSet; }

		void SetVisibility (bool visible)												{ m_bVisible = visible; }
		void SetPolygonsChanged (bool changed)											{ m_polygonsChanged = changed; }
		void SetAutoAdjustElevationStatus (bool status)									{ m_autoAdjustElevation = status; }
		void SetIncreasingElevationStatus (bool status)									{ m_increasingElevation = status; }
		void SetSmoothPolygonStatus (bool status)										{ m_smoothPolygons = status; }
		void SetElevationOffset (float offset)											{ m_elevationOffset = offset; }
		void SetElevation (float elevation)												{ m_inputElevation = elevation; }
		void SetFillOpacity (float alpha)												{ m_fillOpacity = alpha; }
		void SetAlgorithm (ALGORITHM sortingAlgorithm)									{ m_sortBy = sortingAlgorithm; }
		void SetPolygons (std::vector<PolygonModelPtr> newPolygons)						{ m_polygons = newPolygons; }
		void SetCurrentLocationSet (std::vector<LocationLayerPtr> locations)			{ m_currentLocationSet = locations; }

		//Functions for accessing 'sizing' variables
		float GetPolygonInflation()		{ return m_polygonInflation; }
		float GetPolygonScaling()		{ return m_polygonScaling; }

		void SetPolygonInflation(float inflation)	{ m_polygonInflation = inflation; }
		void SetPolygonScaling(float scaleBy)		{ m_polygonScaling = scaleBy; }

		//Functions for accessing 'border' variables
		bool GetBorderVisibility()		{ return m_showBorders; }
		uint GetBorderThickness()	{ return m_borderThickness; }
		float GetBorderOpacity()		{ return m_borderOpacity; }

		void SetBorderVisibility (bool borderVisibility)	{ m_showBorders = borderVisibility; }
		void SetBorderThickness (uint borderThickness)		{ m_borderThickness = borderThickness; }
		void SetBorderOpacity (float borderOpacity)			{ m_borderOpacity = borderOpacity; }

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);


	private:

		//General variables
		bool								m_bVisible;
		bool								m_polygonsChanged;
		bool								m_autoAdjustElevation;
		bool								m_increasingElevation;
		bool								m_smoothPolygons;
		float								m_elevationOffset;
		float								m_inputElevation;
		float								m_elevationUsed;
		float								m_fillOpacity;
		float								m_polygonInflation;
		float								m_polygonScaling;
		ALGORITHM							m_sortBy;
		std::vector<LocationLayerPtr>		m_currentLocationSet;
		std::vector<PolygonModelPtr>				m_polygons;

		//Border variables
		bool	m_showBorders;
		uint	m_borderThickness;
		float   m_borderOpacity;


	};
}

#endif
