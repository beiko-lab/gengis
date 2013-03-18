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

#ifndef _GENGIS_LAYER_TREE_MODEL_
#define _GENGIS_LAYER_TREE_MODEL_

#include "../core/Precompiled.hpp"

namespace GenGIS 
{

	/**
	* @brief Tree structure containing all layers.
	*/
	class LayerTreeModel: private Uncopyable
	{
	public:
		/** Constructor. */
		explicit LayerTreeModel();

		/** Destructor. */
		~LayerTreeModel() {}

		/** 
		* @brief Add study to tree. 
		* @param map Study to add to tree.
		*/
		void AddStudy(StudyLayerPtr study);

		/** Get number of studies contained in tree. */
		unsigned int GetNumStudies() const;

		/**
		* @brief Get study from tree.
		* @param index Index of desired study.
		* @return Study at given index.
		*/
		StudyLayerPtr GetStudy(uint index) const;

		/**
		* @brief Remove study from tree.
		* @param index Index of desired study.
		*/
		void RemoveStudy(uint index);

		/**
		* @brief Get layer from tree. 
		* @param id Unique id of desired layer.
		* @return layer at given id.
		*/
		LayerPtr GetLayer(uint id) const;

		/**
		* @brief Remove layer from tree. 
		* @param id Unique id of layer to remove.
		* @return True if layer removed, false if layer not found.
		*/
		bool RemoveLayer(uint id);

		/** Render all layers in tree. */
		void Render();

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Studies loaded into tree. */
		std::vector<StudyLayerPtr> m_studies;
	};

}

#endif
