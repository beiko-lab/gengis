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

#ifndef _GENGIS_TREE_LAYER_
#define _GENGIS_TREE_LAYER_

#include "../core/Precompiled.hpp"

#include "../core/Layer.hpp"

class GenGIS::TreeLayer;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::TreeLayer * t, const unsigned int file_version);
	}
}

namespace GenGIS
{

	/**
	* @brief A tree layer displays geographical tree models. It is a terminal node in the layer tree model.
	*/
	class TreeLayer : public Layer
	{
	public:
		/** 
		* @brief Constuctor.
		* @param id Unique identifier used to specify tree layer.
		* @param parent Parent of this layer.
		* @param treeView Tree view associated with this tree layer.
		*/
		explicit TreeLayer(uint id, LayerPtr parent, GeoTreeViewPtr treeView) :
			Layer(id, Layer::TREE, parent), m_treeView(treeView) {}

		/** Destructor. */
		~TreeLayer() {}

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
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state);

		/** Get active state of layer. */
		bool IsActive() const;

		/** Toggle active state of layer. */
		void ToggleActive();

		/** Get geographic tree associated with this layer. */
		GeoTreeViewPtr GetGeoTreeView() const
		{
			return m_treeView;
		}

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const TreeLayer * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Controller for geographic tree model. */
		GeoTreeViewPtr m_treeView;
	};

}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const TreeLayer * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//uint id = t->GetId();      // m_id (uint, Layer class)
			//ar << id;
			//ar << t->GetParent();      // m_parent (LayerPtr, Layer class)
			//ar << t->GetGeoTreeView(); // m_treeView (GeoTreeViewPtr)

			ar << t->m_id;       // uint (Layer class)
			ar << t->m_parent;   // LayerPtr (Layer class)
			ar << t->m_treeView; // GeoTreeViewPtr
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, TreeLayer * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			uint _id;
			ar >> _id;

			LayerPtr _parent;
			ar >> _parent;

			GeoTreeViewPtr _treeView;
			ar >> _treeView;

			::new(t)TreeLayer(_id, _parent, _treeView);
		}
	}
}

#endif
