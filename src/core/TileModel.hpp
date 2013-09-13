//=======================================================================
// Author: Alexander Keddy
//
// Copyright 2013 Alexander Keddy
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

#ifndef _GENGIS_TILE_MODEL_
#define _GENGIS_TILE_MODEL_

#include "../core/Precompiled.hpp"

#include "../core/Layer.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/LocationModel.hpp"
#include "../utils/StringTools.hpp"

namespace GenGIS
{
	/**
	*	@brief Tile of Locations in bounded coordinates. A single tile may be associated with multiple locations.
	*/
	class TileModel : public LocationModel
	{
	public:
		/** 
		* @brief Constuctor.
		* @param siteId Unique id for location site.
		* @param northing Northing coordinate of top left corner of tile.
		* @param easting Easting coordinate of top left corner of tile.
		* @param data Data associated with this location.
		*/
		explicit TileModel(std::wstring siteId, std::pair<float,float> topLeft, std::pair<float,float> bottomRight, std::map<std::wstring,std::wstring> data )
			: LocationModel(siteId, topLeft.first, topLeft.second, data),
			m_topLeft(topLeft), m_bottomRight(bottomRight), numLocations(0)
		{
		}

		/** Destructor. */
		~TileModel() {}

		/**
		* @brief Get top left coordinate.
		*/
		std::pair<float,float> GetTopLeft()
		{
			return m_topLeft;
		}

		/**
		* @brief Set top left coordinate.
		*/
		void SetTopLeft(std::pair<float,float> top)
		{
			m_topLeft = top;
		}

		/**
		* @brief Get bottom right coordinate.
		*/
		std::pair<float,float> GetBottomRight()
		{
			return m_bottomRight;
		}

		/**
		* @brief Set bottom right coordinate.
		*/
		void SetBottomRight(std::pair<float,float> bottom)
		{
			m_bottomRight = bottom;
		}
		
		/** 
		* @brief Set number of locations in tile.
		*/
		void SetNumberLocations(int num)
		{
			numLocations = num;
		}

		/**
		* @brief Increment the number of locations in tile by 1.
		*/
		void IncrementNumLocations()
		{
			numLocations++;
		}

		/**
		* @brief Get number of locations in Grid
		*/
		uint GetNumLocations()
		{
			return numLocations;
		}

		/**
		* @brief Add a location layer to the grid
		* @param LocationLayer to add
		*/
		void AddLocationLayer(LocationLayerPtr location);
		
		/**
		* @brief Add data to Tile
		* @param newData data to update current data with
		*/
		void UpdateData(std::map<std::wstring,std::wstring> newData);

		/**
		* @brief Add sequences to tile. uses pointers to save space
		* @param Sequence vector of sequences to add
		*/
		void UpdateSequences(std::vector<SequenceLayerPtr> sequences);

	private:

		/** Number of locations located in the tile. */
		uint numLocations;

		/** Latitude/Longitude pair for top left corner of the tile. */
		std::pair<float,float> m_topLeft;

		/** Latitude/Longitude pair for bottom right corner of the tile. */
		std::pair<float,float> m_bottomRight;
		
		/** Flag indicating if this sequence is currently in the active set. */
		bool m_bActive;
	};
}

#endif
