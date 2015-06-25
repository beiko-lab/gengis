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
#include "../utils/Colour.hpp"

#include <numeric>

class GenGIS::TileModel;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::TileModel * t, const unsigned int file_version);
	}
}

namespace GenGIS
{
	/**
	*	@brief Tile of Locations in bounded coordinates. A single tile may be associated with multiple locations.
	*/
	class TileModel : public LocationModel
	{
	public:

		enum DATA_COMBINE { AVERAGE, STDEV, GINI, SUM };
		
		bool operator >( const TileModelPtr t)
		{
			if( this->GetTopLeft().y < t->GetTopLeft().y )
				return true;
			else if( this->GetTopLeft().y < t->GetTopLeft().y
				&& this->GetTopLeft().x > t->GetTopLeft().x )
				return true;
			else
				return false;
		}

		bool operator >( const Point2D point )
		{
			if(this->GetTopLeft().y < point.y)
				return true;
			else if( this->GetTopLeft().y < point.y 
				&& this->GetTopLeft().x > point.x )
				return true;
			else
				return false;
		}

		// if left is less than right
		bool operator <( const TileModelPtr t )
		{
			if( this->GetTopLeft().y > t->GetTopLeft().y )
				return true;
			else if( this->GetTopLeft().y < t->GetTopLeft().y
				&& this->GetTopLeft().x < t->GetTopLeft().x )
				return true;
			else
				return false;
		}

		bool operator ==( const TileModelPtr t )
		{
			// throw in some floating point tolerance
			if ( (abs(this->GetTopLeft().x - t->GetTopLeft().x) < 0.001)
				&& (abs(this->GetTopLeft().y - t->GetTopLeft().y) < 0.001)
				&& (abs(this->GetBottomRight().x - t->GetBottomRight().x) < 0.001)
				&& (abs(this->GetBottomRight().y - t->GetBottomRight().y ) < 0.001 ) )
				return true;
			else
				return false;
		}
	
		bool operator==( const Point2D loc )
		{
		/*	if ( ( abs(this->GetTopLeft().x - t.x) < 0.00001)
				&& (abs(this->GetTopLeft().y - t.y) < 0.00001)
				&& (abs(this->GetBottomRight().x - t.x) < 0.00001)
				&& (abs(this->GetBottomRight().y - t.y) < 0.00001 ) )
				return true;
		*/	
			if( ( this->GetTopLeft().x <= loc.x 
				&& loc.x <= this->GetBottomRight().x) 
				&& ( this->GetBottomRight().y <= loc.y 
				&& loc.y <= this->GetTopLeft().y ) )
				return 0;
			else
				return false;
		}
	
		/** 
		* @brief Constuctor.
		* @param siteId Unique id for location site.
		* @param northing Northing coordinate of top left corner of tile.
		* @param easting Easting coordinate of top left corner of tile.
		* @param data Data associated with this location.
		*/
		explicit TileModel(
			std::wstring siteId,
			Point2D topLeft,
			Point2D bottomRight,
			std::map<std::wstring,std::wstring> data
		)
			: LocationModel(siteId, topLeft.x, topLeft.y, data),
			m_topLeft(topLeft),
			m_bottomRight(bottomRight),
			numLocations(0),
			m_bActive(true),
			m_combinationMethod( AVERAGE )
		{
		}
	
		TileModel();

		/** Destructor. */
		~TileModel() {}

		/**
		* @brief Get top left coordinate.
		*/
		Point2D GetTopLeft()
		{
			return m_topLeft;
		}

		/**
		* A function to find if a point is with a tile
		* Returns <0 if point is smaller, 0 if contains point and >0 if point is larger.
		*/
		/*
		int Compare(Point2D Location);

		int Compare(TileModelPtr tile);
		*/
		/**
		* @brief Set top left coordinate.
		*/
		void SetTopLeft(Point2D top)
		{
			m_topLeft = top;
		}

		/**
		* @brief Get bottom right coordinate.
		*/
		Point2D GetBottomRight()
		{
			return m_bottomRight;
		}

		/**
		* @brief Set bottom right coordinate.
		*/
		void SetBottomRight(Point2D bottom)
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
		* @brief check if field exists in tile
		* @param field to search for
		*/
		int FindField(std::wstring field)
		{
			std::map<std::wstring,std::wstring> data = GetData();
			std::map<std::wstring,std::wstring>::iterator it = data.find(field);
			if( it == data.end() )
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}

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

		/**
		* @brief Change the colour of the tile
		* @param Colour to change the tile
		*/
		void SetColour(Colour colour) { m_colour = colour; }
		
		/**
		* @brief Get colour of the tile
		*/
		Colour GetColour() { return m_colour; }

		/**
		* @brief Combine tile data fields as specified by the user.
		*/
		void CombineData();
	
		/**
		*	@brief Combine tile sequence data fields as specified by the user.
		*/
		void CombineSequenceData();

		/**
		* @brief Set combination method for tile fields.
		* @param Combination method to be set to.
		*/
 		void SetCombinationMethod(DATA_COMBINE dat) { m_combinationMethod = dat; }

		/**
		* @brief Get combination method for tile fields.
		*/
		DATA_COMBINE GetCombinationMethod() { return m_combinationMethod; }

		/**
		* @brief Compute the Gini-Simpson Index for a sample.
		* @param Sample values to be computer.
		*/
		double GiniSimpson(std::vector<std::wstring> values);

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const TileModel * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	private:

		/** Number of locations located in the tile. */
		uint numLocations;

		/** Latitude/Longitude pair for top left corner of the tile. */
		Point2D m_topLeft;

		/** Latitude/Longitude pair for bottom right corner of the tile. */
		Point2D m_bottomRight;
		
		/** Flag indicating if this sequence is currently in the active set. */
		bool m_bActive;

		/** Colour of tile if Mapped colouring is selected. */
		Colour m_colour;

		/** Method for data combination of tile. */
		DATA_COMBINE m_combinationMethod;
	};

}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const TileModel * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			ar << t->m_id;          // std::wstring&
			ar << t->m_topLeft;     // std::pair<float,float>
			ar << t->m_bottomRight; // std::pair<float,float>
			ar << t->m_data;        // std::map<std::wstring,std::wstring>
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, TileModel * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			std::wstring _id;
			ar >> _id;

			Point2D _topLeft;
			ar >> _topLeft;

			Point2D _bottomRight;
			ar >> _bottomRight;

			std::map<std::wstring,std::wstring> _data;
			ar >> _data;

			::new(t)TileModel( _id, _topLeft, _bottomRight, _data );
		}
	}
}

#endif
