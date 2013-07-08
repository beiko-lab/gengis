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

#ifndef _GENGIS_LAYER_TREE_VIEW_
#define _GENGIS_LAYER_TREE_VIEW_

#include "../core/Precompiled.hpp"
#include "../gui/GenGisFrame.hpp"

class GenGIS::LayerTreeView;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::LayerTreeView * t, const unsigned int file_version);
	}
}

namespace GenGIS 
{

	/**
	* @class LayerTreeView
	* @brief View of tree structure which contains all layers.
	*/
	class LayerTreeView: private Uncopyable
	{
	public:
		//** Possible image states for the layer tree view. */
		enum IMAGE_STATE { UNCHECKED = 0, CHECKED = 1, SEQ_UNCHECKED = 2, SEQ_CHECKED = 3  };

	public:
		/** Constructor. */
		explicit LayerTreeView(LayerTreeModelPtr tree, wxTreeCtrl* treeCtrl);

		/** Destructor. */
		~LayerTreeView() {}

		/** Get layer tree control. */
		wxTreeCtrl* GetTreeCtrl() const { return m_treeCtrl; }

		/** 
		* @brief Set checked/unchecked state of tree item. 
		* @param treeId Id of tree item to change state of.
		* @param state State to set tree item to.
		*/
		void SetChecked(const wxTreeItemId& treeId, bool bVisible);

		/** 
		* @brief Set label name of tree item. 
		* @param treeId Id of tree item to change name of
		* @param name New name of tree item.
		*/
		void SetName(const wxTreeItemId& treeId, const std::wstring& name);

		/**
		* @brief Search wxTreeControl for an item by name
		* @param treeId of tree item to change name of
		* @param name of node
		*/
		wxTreeItemId FindItem( wxTreeItemId root, const wxString& sSearchFor );
		// Code for this method adapted from: http://wiki.wxwidgets.org/WxTreeCtrl

		wxTreeItemId GetRootItem() { return m_treeCtrl->GetRootItem(); }

	protected:
		/**
		* @brief Traverse all children and set their text colour.
		* @param id Id of top node to traverse from.
		* @param cookie Required to make function reentrant (see wxTreeCtrl documentation).
		* @param stopAtUnchecked Flag indicating if traversal should stop once an unchecked child is encountered.
		* @param colour Colour to set text to.
		*/
		void SetColourAllChildren(wxTreeItemId id, wxTreeItemIdValue cookie, bool stopAtUnchecked, wxColour colour);

	protected:
		/** Layer tree model. */
		LayerTreeModelPtr m_tree;

		/** View of layer tree model. */
		wxTreeCtrl* m_treeCtrl;

		/** Root id of the layer tree. */
		wxTreeItemId m_rootItemId;

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const LayerTreeView * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int file_version);
	};
}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const LayerTreeView * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//ar << t->GetLayerTree(); // m_tree (LayerTreeModelPtr)
			ar << t->m_tree; // LayerTreeModelPtr
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, LayerTreeView * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			LayerTreeModelPtr _tree;		
			ar >> _tree;

			wxTreeCtrl* _treeCtrl = ((GenGisFrame*)GenGIS::App::Inst().GetMainWindow())->GetTreeCtrlLayer();

			::new(t)LayerTreeView(_tree, _treeCtrl);
		}
	}
}

#endif
