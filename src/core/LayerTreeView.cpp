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

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"
#include "../core/LayerTreeModel.hpp"
#include "../core/LayerTreeView.hpp"
#include "../core/LayerTreeItem.hpp"
#include "../core/MapLayer.hpp"
#include "../core/StudyController.hpp"
#include "../core/StudyModel.hpp"
#include "../core/StudyLayer.hpp"
#include "../core/MapController.hpp"

#include "../gui/GenGisFrame.hpp"

#include "../utils/UniqueId.hpp"

#include "../../bin/images/checked.xpm"
#include "../../bin/images/unchecked.xpm"
#include "../../bin/images/SampleDataDesaturated.xpm"
#include "../../bin/images/SampleData.xpm"

using namespace GenGIS;

LayerTreeView::LayerTreeView(LayerTreeModelPtr tree, wxTreeCtrl* treeCtrl): m_tree(tree), m_treeCtrl(treeCtrl)
{
	if(!App::Inst().IsCurrentlySerializing())
	{
		// setup tree control
		const int IMAGE_SIZE = 16;
		wxImageList *images = new wxImageList(IMAGE_SIZE, IMAGE_SIZE, true);

		wxIcon image[4];
		image[0] = wxIcon(unchecked_xpm);
		image[1] = wxIcon(checked_xpm);
		image[2] = wxIcon(SampleDataDesaturated_xpm);
		image[3] = wxIcon(SampleData_xpm);
	
		for(size_t i = 0; i < WXSIZEOF(image); i++)
		{
			images->Add(image[i]);
		}

		m_treeCtrl->AssignImageList(images);
		m_rootItemId = m_treeCtrl->AddRoot(wxT("Root"), 0);

		// Create a new study layer
		StudyModelPtr studyModel(new StudyModel());
		StudyControllerPtr studyController(new StudyController(studyModel));
		StudyLayerPtr studyLayer(new StudyLayer(UniqueId::Inst().GenerateId(), LayerPtr(), studyController));
		studyLayer->SetName( wxT( "New Study" ) );
		m_tree->AddStudy(studyLayer);

		// Add study layer to tree view (wxTreeCtrl)
		LayerTreeItem* layerItem = new LayerTreeItem(studyLayer);
		wxString idStr( wxT( "Study : " ) + studyLayer->GetWXName() );
		wxTreeItemId studyItemId = m_treeCtrl->AppendItem(m_treeCtrl->GetRootItem(), wxT(""), LayerTreeView::CHECKED, -1, layerItem);
		m_treeCtrl->SetItemText(studyItemId, idStr);

		// Add Study wxTreeItemId to study layer for easy reference
		studyLayer->SetWXTreeItemId( studyItemId );
	}
	else
	{
		// Remove any existing items from tree view (wxTreeCtrl)
		wxTreeItemId x = m_treeCtrl->GetRootItem();
		m_treeCtrl->DeleteChildren(m_treeCtrl->GetRootItem());
		
		// Fetch existing study layer
		StudyLayerPtr studyLayer = m_tree->GetStudy(0);

		// Add study layer to tree view (wxTreeCtrl)
		LayerTreeItem* layerItem = new LayerTreeItem(studyLayer);
		wxString idStr( wxT( "Study : " ) + studyLayer->GetWXName() );
		wxTreeItemId studyItemId = m_treeCtrl->AppendItem(m_treeCtrl->GetRootItem(), wxT(""), studyLayer->IsActive(), -1, layerItem);
		m_treeCtrl->SetItemText(studyItemId, idStr);

		// Add Study wxTreeItemId to study layer for easy reference
		studyLayer->SetWXTreeItemId( studyItemId );
		
		// Select the study layer
		m_treeCtrl->SelectItem(layerItem->GetId(), true);
	}
}

template<class Archive>
void LayerTreeView::serialize(Archive & ar, const unsigned int file_version)
{
	// Needs to be here
}
template void LayerTreeView::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void LayerTreeView::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void LayerTreeView::SetChecked(const wxTreeItemId& treeId, bool bVisible)
{
	if(m_treeCtrl->GetItemImage(treeId) != SEQ_UNCHECKED && m_treeCtrl->GetItemImage(treeId) != SEQ_CHECKED)
	{
		// do not toggle icons for sequence layers
		if(bVisible)
			m_treeCtrl->SetItemImage(treeId, CHECKED);
		else
			m_treeCtrl->SetItemImage(treeId, UNCHECKED);
	}

	// set to black or light grey all items below checked/unchecked node
	wxColour colour;
	bool stopAtUnchecked;
	if(bVisible)
	{
		colour = *wxBLACK;
		stopAtUnchecked = true;
	}
	else
	{
		colour = *wxLIGHT_GREY;
		stopAtUnchecked = false;
	}

	// traverse all nodes below the hit node
	wxTreeItemIdValue cookie;
	wxTreeItemId childId = m_treeCtrl->GetFirstChild(treeId, cookie);
	SetColourAllChildren(childId, cookie, stopAtUnchecked, colour);		
}

void LayerTreeView::SetColourAllChildren(wxTreeItemId id, wxTreeItemIdValue cookie, bool stopAtUnchecked, wxColour colour)
{
	if(id.IsOk())
	{
		if(m_treeCtrl->GetItemImage(id) == SEQ_UNCHECKED || m_treeCtrl->GetItemImage(id) == SEQ_CHECKED)
		{
			// handle sequence layers

			if(stopAtUnchecked)
			{	
				// a parent layer was checked so active/deactive sequence accordingly
				LayerTreeItem* layerItem = (LayerTreeItem*)m_treeCtrl->GetItemData(id);
				if(layerItem->GetLayer()->IsActive())
				{
					m_treeCtrl->SetItemImage(id, SEQ_CHECKED);
					m_treeCtrl->SetItemTextColour(id, *wxBLACK);
				}
				else
				{
					m_treeCtrl->SetItemImage(id, SEQ_UNCHECKED);
					m_treeCtrl->SetItemTextColour(id, *wxLIGHT_GREY);
				}
			}
			else
			{
				// a parent layer was unchecked so deactivate sequence
				m_treeCtrl->SetItemTextColour(id, *wxLIGHT_GREY);
			}
		}
		else
		{
			if(!stopAtUnchecked || m_treeCtrl->GetItemImage(id) == LayerTreeView::CHECKED)
			{
				// do not travere children that are currently unchecked when we have just checked a parent node
				m_treeCtrl->SetItemTextColour(id, colour);
				wxTreeItemIdValue childCookie;
				wxTreeItemId childId = m_treeCtrl->GetFirstChild(id, childCookie);
				SetColourAllChildren(childId, childCookie, stopAtUnchecked, colour);
			}
		}
		
		wxTreeItemId siblingId = m_treeCtrl->GetNextSibling(id);
		SetColourAllChildren(siblingId, cookie, stopAtUnchecked, colour);
	}

	// no more children
	return;
}

void LayerTreeView::SetName(const wxTreeItemId& treeId, const std::wstring& name) 
{ 
	LayerTreeItem* layerItem = (LayerTreeItem*)m_treeCtrl->GetItemData(treeId);
	Layer::LAYER_TYPE layerType = layerItem->GetLayer()->GetType();

	std::wstring typeStr = Layer::GetLayerTypeString(layerType);
	std::wstring treeName = typeStr + std::wstring(_T(" : ")) + name;

	m_treeCtrl->SetItemText(treeId, wxString(treeName.c_str()));
}

wxTreeItemId LayerTreeView::FindItem( wxTreeItemId root, const wxString& sSearchFor )
{
	// Code for this method adapted from: http://wiki.wxwidgets.org/WxTreeCtrl
	wxTreeItemIdValue cookie;
	wxTreeItemId search;
	wxTreeItemId item = m_treeCtrl->GetFirstChild( root, cookie );
	wxTreeItemId child;
 
	while( item.IsOk() )
	{
		wxString sData = m_treeCtrl->GetItemText(item);
		if( sSearchFor.CompareTo(sData) == 0 )
		{
			return item;
		}
		if( m_treeCtrl->ItemHasChildren( item ) )
		{
			wxTreeItemId search = FindItem( item, sSearchFor );
			if( search.IsOk() )
			{
				return search;
			}
		}
		item = m_treeCtrl->GetNextChild( root, cookie);
	}
 
	/* Not found */
	wxTreeItemId dummy;
	return dummy;
}
