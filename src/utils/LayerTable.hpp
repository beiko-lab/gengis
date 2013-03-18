//=======================================================================
// Author: Timothy Mankowski
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

#ifndef LAYER_TABLE_H
#define LAYER_TABLE_H

#include "../core/Precompiled.hpp"

#include "../core/Layer.hpp"

namespace GenGIS
{
	template <class LayerPtr_T, class Layer_T>
	class LayerTable
	{
	public:
		/** Constructor. */
		LayerTable() : m_emptyPositions(0) {}
		
		/** Destructor. */
		~LayerTable() {}

		/** Returns the number of layers. */
		uint GetNumberOfLayers() const
		{
			return m_idMap.size();
		}

		/** Returns a pointer to the layer deque. */
		std::deque<LayerPtr_T> GetAllLayers() const
		{
			if ( m_emptyPositions > 0 )
				CompactDeque();
			return m_layerDeque;
		}

		/** Retrieve layer by id. */
		LayerPtr_T GetLayerById( uint layerId ) const
		{
			std::map<uint, uint>::const_iterator it;

			// Retrieve corresponding position in deque from map
			it = m_idMap.find( layerId );
			if ( it == m_idMap.end() )
			{
				// If the value is out of range, return a NULL shared_ptr pointer
				return boost::shared_ptr<Layer_T>();
			}

			return m_layerDeque.at( it->second );
		}

		/** Retrieve layer by deque index. */
		LayerPtr_T GetLayerByIndex( const uint index ) const
		{
			if ( m_emptyPositions > 0 )
				CompactDeque();
			return m_layerDeque.at( index );
		}

		/** Retrieve layer by deque index. */
		LayerPtr_T operator[] ( const uint index )
		{
			if ( m_emptyPositions > 0 )
				CompactDeque();
			return m_layerDeque.at( index );
		}

		/** Adds layer to LayerTable. */
		void AddLayer( LayerPtr_T layer )
		{
			// Store the layer in the deque
			m_layerDeque.push_back( layer );

			// Store the unique LAYER ID and its POSITION in the deque, in a map
			m_idMap.insert( std::pair<uint, uint>( layer->GetId(), m_layerDeque.size()-1 ) );
		}

		/** Removes layer specified by id from LayerTable. */
		bool RemoveLayer( uint layerId )
		{
			std::map<uint, uint>::iterator it;

			// Locate layer id in map
			it = m_idMap.find( layerId );

			// If a layer by the specified id is not found, return false
			if ( it == m_idMap.end() )		
				return false;

			// Retrive layer pointer and reset it
			LayerPtr_T layer = GetLayerById( layerId );
			layer.reset();

			// Remove layerId from the map
			m_idMap.erase( it );

			// Increment empty position count
			m_emptyPositions++;

			// If the number of unused positions exceeds 1000, compact the deque
			if ( m_emptyPositions > 1000 )
				CompactDeque();

			return true;
		}

		/** Removes layer specified by pointer from LayerTable. */
		bool RemoveLayer( LayerPtr_T layer )
		{
			// Return if the layer is NULL
			if (!layer)
				return false;

			std::map<uint, uint>::iterator it;

			// Retrieve the layer id
			uint layerId = layer->GetId();

			// Locate layer id in map
			it = m_idMap.find( layerId );

			// If a layer by the specified id is not found, return false
			if ( it == m_idMap.end() )		
				return false;

			// Remove layerId from the map
			m_idMap.erase( it );

			// Reset the layer pointer and increment empty position count
			layer.reset();

			m_emptyPositions++;

			// If the number of unused positions exceeds 1000, compact the deque
			if ( m_emptyPositions > 1000 )
				CompactDeque();

			return true;
		}

	private:

		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			// During save, if there are NULL positions in the deque, remove them
			if ( Archive::is_saving::value && ( m_emptyPositions > 0 ) )
				CompactDeque();

			ar & m_idMap;      // std::map<uint, uint>
			ar & m_layerDeque; // std::deque<LayerPtr_T>
		}

		/** Removes NULL pointers from the deque. */
		void CompactDeque()
		{
			// Clear existing map
			m_idMap.clear();

			// Iterate through the deque to find NULL pointers
			for ( uint index = 0; index < m_layerDeque.size(); index++ )
			{
				// If the pointer is NOT NULL, add its new position to the map
				if ( m_layerDeque[index] )
					m_idMap.insert( std::pair<uint, uint>( m_layerDeque[index]->GetId(), index ) );

				// Otherwise, remove it from the deque
				else
					m_layerDeque.erase( m_layerDeque.begin() + index );
			}
			m_emptyPositions = 0;
		}

		/** Mapping between unique layer id and position of layer in the deque. */
		std::map<uint, uint> m_idMap;

		/** Deque of layer pointers. */
		std::deque<LayerPtr_T> m_layerDeque;

		/** Number of empty positions (number of layers) in the deque. */
		uint m_emptyPositions;
	};
}

#endif
