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

#ifndef _GENGIS_LOCATION_LAYER_
#define _GENGIS_LOCATION_LAYER_

#include "../core/Precompiled.hpp"

#include "../core/Layer.hpp"
#include "../core/LocationController.hpp"

class GenGIS::LocationLayer;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::LocationLayer * t, const unsigned int file_version);
	}
}

namespace GenGIS 
{
	/**
	* @brief Location of samples in projected coordinates. A single location may be associated with multiple samples.
	*/
	class LocationLayer : public Layer
	{
	public:
		/** 
		* @brief Constuctor.
		* @param id Unique identifier used to specify location.
		* @param parent Parent of this layer.
		* @param location Location controller to associate with this layer.
		*/
		explicit LocationLayer(uint id, LayerPtr parent, LocationControllerPtr locationController) :
			Layer(id, Layer::LOCATION, parent), m_locationController(locationController) {}

		/** Destructor. */
		~LocationLayer() {}

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

		/**
		* @brief Remove layer from tree below this layer by id.
		* @param id Unique id of layer to remove.
		* @return True if layer removed, false if layer not found.
		*/
		bool RemoveSequenceLayerById(uint id);

		/** Render contents of layer. */
		void Render();

		/**
		* @brief Add sequence to location.
		* @param sequence Sequence to add.
		*/
		void AddSequence(SequenceLayerPtr sequence)
		{
			m_sequence.push_back(sequence);
		}

		/** Get number of samples at this location. */
		unsigned int GetNumSequenceLayers() const
		{
			return m_sequence.size();
		}

		/**
		* @brief Get sample from location.
		* @param index Index of desired sample.
		* @return Sample at given index.
		*/
		SequenceLayerPtr GetSequenceLayer(uint index) const
		{
			return m_sequence.at(index);
		}

		/**
		* @brief Remove sample from location.
		* @param index Sample to remove.
		*/
		void RemoveSequenceLayerByIndex(uint index);

		/** Remove all sequence layers. */
		void RemoveAllSequenceLayers()
		{
			m_sequence.clear();
		}

		/** Get vector of all sequence layers. */
		std::vector<SequenceLayerPtr> GetAllSequenceLayers() const
		{
			return m_sequence;
		}

		/** Get vector of all active sequence layers. */
		std::vector<SequenceLayerPtr> GetAllActiveSequenceLayers() const;

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state)
		{
			m_locationController->SetActive(state);
		}

		/** Get active state of layer. */
		bool IsActive() const
		{
			return m_locationController->IsActive();
		}

		/** Toggle active state of layer. */
		void ToggleActive()
		{
			m_locationController->ToggleActive();
		}

		/** Get location controller associated with this layer. */
		LocationControllerPtr GetLocationController() const
		{
			return m_locationController;
		}

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const LocationLayer * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	private:
		/** Controller for location model. */
		LocationControllerPtr m_locationController;

		/** All samples collected at this location. */
		std::vector<SequenceLayerPtr> m_sequence;
	};
}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const LocationLayer * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//uint id = t->GetId();             // m_id (uint, Layer class)
			//ar << id; 
			//ar << t->GetParent();             // m_parent (LayerPtr, Layer class)
			//ar << t->GetLocationController(); // LocationControllerPtr

			ar << t->m_id;                 // uint (Layer class)
			ar << t->m_parent;             // LayerPtr (Layer class)
			ar << t->m_locationController; // LocationControllerPtr
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, LocationLayer * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			uint _id;
			ar >> _id;

			LayerPtr _parent;
			ar >> _parent;

			LocationControllerPtr _locationController;
			ar >> _locationController;

			::new(t)LocationLayer(_id, _parent, _locationController);
		}
	}
}

#endif
