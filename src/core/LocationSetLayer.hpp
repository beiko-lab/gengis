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

#ifndef _GENGIS_LOCATION_SET_LAYER_
#define _GENGIS_LOCATION_SET_LAYER_

#include "../core/Precompiled.hpp"

#include "../core/ChartSetView.hpp"
#include "../core/Layer.hpp"
#include "../core/LocationController.hpp"

#include "../utils/ShapeMap.hpp"

class GenGIS::LocationSetLayer;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::LocationSetLayer * t, const unsigned int file_version);
	}
}

namespace GenGIS
{

	/**
	* @brief A location set consists of 1 or more locations. 
	*/
	class LocationSetLayer : public Layer
	{
	public:
		/** 
		* @brief Constuctor.
		* @param id Unique identifier used to specify location set.
		* @param parent Parent of this layer.
		* @param chartSetView Chart set view for laying out chart views associated with this location set.
		*/
		explicit LocationSetLayer(uint id, LayerPtr parent, ChartSetViewPtr chartSetView);

		/** Destructor. */
		~LocationSetLayer() {}

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

		/** Add location to location set. */
		void AddLocationLayer(LocationLayerPtr location);

		/** Get number of locations contained in location set. */
		unsigned int GetNumLocationLayers() const
		{
			return m_locationLayers.size();
		}

		/**
		* @brief Get location from location set.
		* @param index Index of desired location.
		* @return Location at given index.
		*/
		LocationLayerPtr GetLocationLayer(unsigned int index) const
		{
			return m_locationLayers.at(index);
		}

		/**
		* @brief Get location from location set.
		* @param name Unique name of desired location.
		* @return Location with given name.
		*/
		LocationLayerPtr GetLocationLayer(const std::wstring& name) const;

		/**
		* @brief Remove location by id from location set.
		* @param id Id of location to remove.
		*/
		bool RemoveLocationLayerById(uint id);

		/**
		* @brief Remove location from location set.
		* @param index Location to remove.
		*/
		void RemoveLocationLayerByIndex(unsigned int index);

		/**
		* @brief Remove location set from study.
		* @param name Name of location.
		*/
		void RemoveLocationLayerByName(const std::wstring& name);

		/**
		* @brief Remove all location layers from the location set.
		* @param none.
		*/
		void RemoveAllLocationLayers();

		/** Get vector of all location layers. */
		std::vector<LocationLayerPtr> GetAllLocationLayers() const
		{
			return m_locationLayers;
		}

		/** Get vector of all active location layers. */
		std::vector<LocationLayerPtr> GetAllActiveLocationLayers() const;

		/** Get chart set controller. */
		ChartSetViewPtr GetChartSetView() const
		{
			return m_chartSetView;
		}

		LocationGridPtr GetLocationGrid() const
		{
			return m_locationGrid;
		}

		LocationPolygonsPtr GetLocationPolygons() const
		{
			return m_locationPolygons;
		}

		void UpdateGridAndPolygons();

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state);

		/** Get active state of layer. */
		bool IsActive() const;

		/** Toggle active state of layer. */
		void ToggleActive();

		/** Get location set controller. */
		LocationSetControllerPtr GetLocationSetController() const
		{
			return m_locationSetController;
		}

		/** Check if there is sequence data associate with at least one location within this location set. */
		bool IsSequencesData();

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const LocationSetLayer * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Chart controller for laying out chart views associated with this location set. */
		ChartSetViewPtr m_chartSetView;

		/** Location grid associated with this location set. */
		LocationGridPtr m_locationGrid;

		/** Location layers beneath this location set layer. */
		std::vector<LocationLayerPtr> m_locationLayers;

		/** Location set controller for setting visual properties to entire set of locations. */
		LocationSetControllerPtr m_locationSetController;

		/** Location polygons associated with this location set */
		LocationPolygonsPtr m_locationPolygons;
	};
}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const LocationSetLayer * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//uint id = t->GetId();		// m_id (uint, Layer class)
			//ar << id;
			//ar << t->GetParent();       // m_parent (LayerPtr, Layer class)
			//ar << t->GetChartSetView(); // ChartSetViewPtr

			ar << t->m_id;	         // uint (Layer class)
			ar << t->m_parent;       // LayerPtr (Layer class)
			ar << t->m_chartSetView; // ChartSetViewPtr
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, LocationSetLayer * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			uint _id;
			ar >> _id;

			LayerPtr _parent;
			ar >> _parent;

			ChartSetViewPtr _chartSetView;
			ar >> _chartSetView;

			::new(t)LocationSetLayer(_id, _parent, _chartSetView);
		}
	}
}

#endif
