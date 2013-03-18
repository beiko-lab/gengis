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

#ifndef _GENGIS_SEQUENCE_LAYER_
#define _GENGIS_SEQUENCE_LAYER_

#include "../core/Precompiled.hpp"

#include "../core/Layer.hpp"
#include "../core/SequenceController.hpp"

class GenGIS::SequenceLayer;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::SequenceLayer * t, const unsigned int file_version);
	}
}

namespace GenGIS
{

	/**
	* @brief Sequence data collected from a location.
	*/
	class SequenceLayer : public Layer
	{
	public:
		/** 
		* @brief Constuctor.
		* @param id Unique identifier used to specify sample.
		* @param parent Parent of this layer.
		* @param sequenceController Sequence controller to associate with this layer.
		*/
		explicit SequenceLayer(uint id, LayerPtr parent, SequenceControllerPtr sequenceController) :
			Layer(id, Layer::SEQUENCE, parent), m_sequenceController(sequenceController) {}

		/** Destructor. */
		~SequenceLayer() {}

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
		bool RemoveLayerById(uint id);

		/** Render contents of layer. */
		void Render();

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state)
		{
			m_sequenceController->SetActive(state);
		}

		/** Get active state of layer. */
		bool IsActive() const
		{
			return m_sequenceController->IsActive();
		}

		/** Toggle active state of layer. */
		void ToggleActive()
		{
			m_sequenceController->ToggleActive();
		}

		/** Get location controller associated with this layer. */
		SequenceControllerPtr GetSequenceController() const
		{
			return m_sequenceController;
		}

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const SequenceLayer * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	private:
		SequenceControllerPtr m_sequenceController;
	};

}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const SequenceLayer * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//uint id = t->GetId();             // m_id (uint, Layer class)
			//ar << id;
			//ar << t->GetParent();             // m_parent (LayerPtr, Layer class)
			//ar << t->GetSequenceController(); // SequenceControllerPtr

			ar << t->m_id;                 // uint (Layer class)
			ar << t->m_parent;             // LayerPtr (Layer class)
			ar << t->m_sequenceController; // SequenceControllerPtr
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, SequenceLayer * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			uint _id;
			ar >> _id;

			LayerPtr _parent;
			ar >> _parent;

			SequenceControllerPtr _sequenceController;
			ar >> _sequenceController;

			::new(t)SequenceLayer(_id, _parent, _sequenceController);
		}
	}
}

#endif
