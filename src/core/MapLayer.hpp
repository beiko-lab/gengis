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

#ifndef _GENGIS_MAP_LAYER_
#define _GENGIS_MAP_LAYER_

#include "../core/Precompiled.hpp"

#include "../core/Layer.hpp"
//#include "../core/App.hpp"

class GenGIS::MapLayer;

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::MapLayer * t, const unsigned int file_version);
	}
}

namespace GenGIS
{

	/**
	* @brief A map consists of a digital elevation model (DEM) and 0 or more location sets. 
	*/
	class MapLayer : public Layer
	{
	public:
		/** 
		* @brief Constuctor.
		* @param id Unique identifier used to specify map.
		* @param parent Parent of this layer.
		*/
		explicit MapLayer( uint id, LayerPtr parent, MapControllerPtr mapController ) :
			Layer( id, Layer::MAP, parent ), m_mapController( mapController ) {}

		/** Destructor. */
		~MapLayer() {}

		/**
		* @brief Get layer from tree below this layer. 
		* @param id Unique id of desired layer.
		* @return layer at given id.
		*/
		LayerPtr GetLayer(uint id) const;

		/**
		* @brief Remove layer from tree below this layer. 
		* @param id Unique id of layer to remove.
		* @return True if layer removed, false if layer not found.
		*/
		bool RemoveLayer(uint id);

		/** Render contents of layer. */
		void Render();


		/** 
		* @brief Add location set to map. 
		* @param locationSite Location site to add to map.
		*/
		void AddLocationSetLayer(LocationSetLayerPtr locationSet)
		{
			m_locationSets.push_back(locationSet);
		}

		/** Get number of location set contained in map. */
		unsigned int GetNumLocationSetLayers() const
		{
			return m_locationSets.size();
		}

		/**
		* @brief Get location set from map.
		* @param index Index of desired location set.
		* @return Location set at given index.
		*/
		LocationSetLayerPtr GetLocationSetLayer(unsigned int index) const
		{
			return m_locationSets.at(index);
		}

		/**
		* @brief Get location set from map.
		* @param name Name of desired location set.
		* @return Location set with given name.
		*/
		LocationSetLayerPtr GetLocationSetLayer(const std::wstring& name) const;

		/**
		* @brief Remove location set from map.
		* @param index Location set to remove.
		*/
		void RemoveLocationSetLayerByIndex(unsigned int index);

		/**
		* @brief Remove location set from map.
		* @param name Name of location set.
		*/
		void RemoveLocationSetLayerByName(const std::wstring& name);

		/**
		* @brief Remove location set from map by id.
		* @param id Id of location set to remove.
		*/
		bool RemoveLocationSetLayerById(uint id);

		/**
		* @brief Remove all location set from map.
		* @param none.
		*/
		void RemoveAllLocationSetLayers();

		/** 
		* @brief Add tree to map. 
		* @param tree Tree to add to map.
		*/
		void AddTreeLayer(TreeLayerPtr tree)
		{
			m_trees.push_back(tree);
		}

		/** Get number of trees contained in map. */
		unsigned int GetNumTreeLayers() const
		{
			return m_trees.size();
		}

		/**
		* @brief Get tree from map.
		* @param index Index of desired tree.
		* @return Tree at given index.
		*/
		TreeLayerPtr GetTreeLayer(unsigned int index) const
		{
			return m_trees.at(index);
		}

		/**
		* @brief Get tree from map.
		* @param name Name of desired tree.
		* @return Tree with given name.
		*/
		TreeLayerPtr GetTreeLayer(const std::wstring& name) const;

		/**
		* @brief Remove tree from map.
		* @param index Tree to remove.
		*/
		void RemoveTreeLayer(unsigned int index);

		/**
		* @brief Remove tree from map.
		* @param name Name of tree to remove.
		*/
		void RemoveTreeLayer(const std::wstring& name);

		/**
		* @brief Remove all trees from map.
		* @param none.
		*/
		void RemoveAllTreeLayers()
		{
			m_trees.clear();
		}

		/** Get all location set layers. */
		const std::vector<LocationSetLayerPtr> GetAllLocationSetLayers() const
		{
			return m_locationSets;
		}

		/** Get all tree layers. */
		const std::vector<TreeLayerPtr> GetAllTreeLayers() const
		{
			return m_trees;
		}

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state);

		/** Get active state of layer. */
		bool IsActive() const;

		/** Toggle active state of layer. */
		void ToggleActive();

		/** Get map controller. */
		MapControllerPtr GetMapController() const;

	protected:
		/** Pointer to the MapController. */
		MapControllerPtr m_mapController;

		/** Location sets associated with this map. */
		std::vector<LocationSetLayerPtr> m_locationSets;

		/** Tree layers associated with this map. */
		std::vector<TreeLayerPtr> m_trees;

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const MapLayer * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

		template<class Archive>
		void load(Archive & ar, const unsigned int version);

		template<class Archive>
		void save(Archive & ar, const unsigned int version) const;
	};

}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data( Archive & ar, const MapLayer * t, const unsigned int file_version )
		{
			// Save data required to construct instance
			//uint id = t->GetId();            // m_id (uint, Layer class)
			//ar << id;
			//ar << t->GetParent();            // m_parent (LayerPtr, Layer class)

			ar << t->m_id;                // uint (Layer class)
			ar << t->m_parent;            // LayerPtr (Layer class)
			ar << t->m_mapController; // MapControllerPtr
		}

		template<class Archive>
		inline void load_construct_data( Archive & ar, MapLayer * t, const unsigned int file_version )
		{
			// Retrieve data from archive required to construct new instance
			uint _id;
			ar >> _id;

			LayerPtr _parent;
			ar >> _parent;

			MapControllerPtr _mapController;
			ar >> _mapController;

			::new(t)MapLayer( _id, _parent, _mapController );
		}
	}
}

#endif
