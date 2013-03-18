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

#ifndef _GENGIS_STUDY_LAYER_
#define _GENGIS_STUDY_LAYER_

#include "../core/Precompiled.hpp"

#include "../core/Layer.hpp"
#include "../core/StudyController.hpp"

class GenGIS::StudyLayer;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::StudyLayer * t, const unsigned int file_version);
	}
}

namespace GenGIS 
{

	/**
	* @brief A study consists of 1 or more maps. It is the highest level
	*				organizational concept. In a traditional GIS settings, a study 
	*				would be termed a project.
	*/
	class StudyLayer : public Layer
	{
	public:
		/** 
		* @brief Constuctor.
		* @param id Unique identifier used to specify study.
		* @param parent Parent of this layer.
		*/
		explicit StudyLayer(uint id, LayerPtr parent, StudyControllerPtr studyController)
			: Layer(id, Layer::STUDY, parent), m_studyController(studyController) {}

		/** Destructor. */
		~StudyLayer() {}

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

		///////////////////
		// Map functions //
		///////////////////

		/** 
		* @brief Add map to study. 
		* @param map Map to add to study.
		*/
		void AddMapLayer(MapLayerPtr map) { m_maps.push_back(map); }

		/** Get number of maps contained in study. */
		unsigned int GetNumMapLayers() const { return m_maps.size(); }

		/**
		* @brief Get map from study.
		* @param index Index of desired map.
		* @return Map at given index.
		*/
		MapLayerPtr GetMapLayer(unsigned int index) const { return m_maps.at(index); }

		/**
		* @brief Get map from study.
		* @param name Name of desired map.
		* @return Map with given name.
		*/
		MapLayerPtr GetMapLayer(const std::wstring& name) const;

		/**
		* @brief Get map from study.
		* @param id Id of the desired map.
		* @return Map with given id.
		*/
		MapLayerPtr GetMapLayerById( unsigned int id ) const;

		/**
		* @brief Remove map from study.
		* @param index Map to remove.
		*/
		void RemoveMapLayerByIndex(unsigned int index);

		/**
		* @brief Remove map from study.
		* @param name Name of map.
		*/
		void RemoveMapLayerByName(const std::wstring& name);

		/**
		* @brief Remove map by id from study.
		* @param layerId Map to remove.
		*/
		bool RemoveMapLayerById( unsigned int layerId );


		/**
		* @brief Remove all maps from study.
		* @param none.
		*/
		void RemoveAllMapLayers();

		//////////////////////////
		// Vector Map functions //
		//////////////////////////

		/** 
		* @brief Add vector map to study. 
		* @param vectorMap Vector map to add to study.
		*/
		void AddVectorMapLayer( VectorMapLayerPtr vectorMap ) { m_vectorMaps.push_back( vectorMap ); }

		/** Get number of vector maps contained in study. */
		unsigned int GetNumVectorMapLayers() const { return m_vectorMaps.size(); }

		/**
		* @brief Get vector map from study.
		* @param index Index of desired vector map.
		* @return Vector map at given index.
		*/
		VectorMapLayerPtr GetVectorMapLayer( unsigned int index ) const { return m_vectorMaps.at( index ); }

		/**
		* @brief Get vector map from study.
		* @param name Name of desired vector map.
		* @return Vector map with given name.
		*/
		VectorMapLayerPtr GetVectorMapLayer( const std::wstring& name ) const;

		/**
		* @brief Get vector map from study.
		* @param id Id of the desired vector map.
		* @return Vector map with given id.
		*/
		VectorMapLayerPtr GetVectorMapLayerById( unsigned int id ) const;

		/**
		* @brief Remove vector map from study.
		* @param index Vector map to remove.
		*/
		void RemoveVectorMapLayerByIndex( unsigned int index );

		/**
		* @brief Remove vector map from study.
		* @param name Name of vector map.
		*/
		void RemoveVectorMapLayerByName( const std::wstring& name );

		/**
		* @brief Remove vector map by id from study.
		* @param layerId Vector map to remove.
		*/
		bool RemoveVectorMapLayerById( unsigned int layerId );

		/**
		* @brief Remove all vector maps from study.
		* @param none.
		*/
		void RemoveAllVectorMapLayers();

		/** Get all vector maps in study. */
		const std::vector<VectorMapLayerPtr> GetAllVectorMapLayers() { return m_vectorMaps; }

		/////////////////////
		// Other functions //
		/////////////////////

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state) { m_studyController->SetActive(state); }

		/** Get active state of layer. */
		bool IsActive() const { return m_studyController->IsActive(); }

		/** Toggle active state of layer. */
		void ToggleActive() { m_studyController->ToggleActive(); }

		/** Get study controller associated with this layer. */
		StudyControllerPtr GetStudyController() const { return m_studyController; }

	private:
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const StudyLayer * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Controller for study model. */
		StudyControllerPtr m_studyController;

		/** Maps associated with this study. */
		std::vector<MapLayerPtr> m_maps;

		/** Vector maps associated with this study. */
		std::vector<VectorMapLayerPtr> m_vectorMaps;
	};
}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const StudyLayer * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			ar << t->m_studyController; // StudyControllerPtr
			ar << t->m_id;              // uint (Layer class)
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, StudyLayer * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			StudyControllerPtr _studyController;
			ar >> _studyController;

			uint _id;
			ar >> _id;

			::new(t)StudyLayer(_id, LayerPtr(), _studyController);
		}
	}
}

#endif
