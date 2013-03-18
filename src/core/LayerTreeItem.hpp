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

#ifndef _GENGIS_LAYER_TREE_ITEM_
#define _GENGIS_LAYER_TREE_ITEM_

#include "../core/Precompiled.hpp"

namespace GenGIS 
{

	/**
	* @brief View of tree structure which contains all layers.
	*/
	class LayerTreeItem : public wxTreeItemData
	{
	public:
		/** Constructor. */
		explicit LayerTreeItem(LayerPtr layer): m_layer(layer) {}

		/** Destructor. */
		~LayerTreeItem() {}

		/**
		* @brief Get layer from tree item.
		* @return Layer associated with tree item.
		*/
		LayerPtr GetLayer() const { return m_layer; }

	protected:
		/** Layer object associated with a given wxTreeCtrl item. */
		LayerPtr m_layer;
	};

}

#endif
