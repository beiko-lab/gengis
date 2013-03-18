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
#include "../core/LocationModel.hpp"
#include "../core/LocationView.hpp"
#include "../core/App.hpp"
#include "../core/Camera.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/MapController.hpp"
#include "../core/OrthoLayers.hpp"
#include "../core/PopupMenu.hpp"
#include "../core/NodeGeoTree.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/TreeLayer.hpp"
#include "../core/GeoTreeView.hpp"
#include "../core/Viewport.hpp"
#include "../core/VisualLabel.hpp"
#include "../core/VisualMarker.hpp"

#include "../gui/LocationPropertiesDlg.hpp"

#include "../utils/ErrorGL.hpp"
#include "../utils/Font.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/Log.hpp"
#include "../utils/Geometry.hpp"

using namespace GenGIS;

LocationView::LocationView(LocationModelPtr locationModel, CameraPtr camera, uint id) :
	m_location(locationModel), m_camera(camera), m_id(id), m_colour(Colour(217, 95, 2)),
	m_size(6), m_shape(VisualMarker::CIRCLE),  m_label(new VisualLabel()), m_flags(NUM_FLAGS), m_renderingOrder(0),
	m_borderSize(1), m_borderColour(Colour(0,0,0))
{
	m_depth = GEO_LOCATION_LAYER;
}

template<class Archive>
void LocationView::serialize(Archive & ar, const unsigned int version)
{
	ar & boost::serialization::base_object<View>(*this);

	ar & m_locationLayerId; // uint
	ar & m_leafNodes;       // std::vector< NodeGeoTree* >
	ar & m_colour;          // Colour
	ar & m_size;            // float
	ar & m_shape;           // VisualMarker::MARKER_SHAPE
	ar & m_renderingOrder;  // float
	ar & m_label;           // VisualLabelPtr
	ar & m_activeView;      // ACTIVE_VIEW
	ar & m_borderSize;      // uint
	ar & m_borderColour;    // Colour

	boost::serialization::split_member(ar, *this, version);
}
template void LocationView::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void LocationView::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

template<class Archive>
void LocationView::save(Archive & ar, const unsigned int version) const
{
	int size;
	size = m_flags.size();
	ar & size;

	ulong bits;
	bits = m_flags.to_ulong();
	ar & bits;
}
template void LocationView::save<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version) const; 

template<class Archive>
void LocationView::load(Archive & ar, const unsigned int version)
{
	int size;
	ar & size;

	ulong bits;
	ar & bits;

	boost::dynamic_bitset<> _flags(size, bits);
	m_flags = _flags;
}
template void LocationView::load<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

Point3D LocationView::GetPosition() const
{ 
	Point3D gridPos;
	App::Inst().GetMapController()->GeoToGrid(m_location->GetProjection(), gridPos);
	return gridPos; 
}

Box2D LocationView::GetBoundingBox() const
{
	Point3D pt = App::Inst().GetMapController()->ProjectToScreen(GetPosition());

	Box2D box;
	box.x = pt.x - m_size;
	box.y = pt.y - m_size;
	box.dx = 2*m_size;
	box.dy = 2*m_size;

	return box;
}

void LocationView::Render()
{
	if(!m_bVisible)
	{
		return;
	}

	/*
	// Note: Please keep this code here as it is useful for debugging.
	// directly plot 3D point
	glDisable(GL_LIGHTING);
	glColor3f(0.75f,0.75f,0.1f);

	glPushMatrix();
		Point3D gridCoord2;	
		App::Inst().GetMapController()->GeoToGrid(m_location->GetProjection(), gridCoord2);

		// move location marker to desired position
		glTranslated(gridCoord2.x, gridCoord2.y, gridCoord2.z);
		glRotated(90, 1.0, 0.0, 0.0);

		//gluSphere(m_markerQuadric, 0.05, 10, 10);
		gluDisk(m_markerQuadric, 0, 0.05, 10, 10);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	*/

	glDisable(GL_LIGHTING);

	float size = m_size * App::Inst().GetResolutionFactor();

	if(IsSelected())
	{
		VisualMarker::RenderDecal(GetPosition(), App::Inst().GetSelectionColour(), size + App::Inst().GetSelectionThickness(), 
			m_shape, m_borderColour, 0, true, GetRenderingOrder());
	}

	VisualMarker::RenderDecal(GetPosition(), m_colour, size, m_shape, m_borderColour, m_borderSize, false, GetRenderingOrder());

	RenderLabel();

	glEnable(GL_LIGHTING);
}

void LocationView::RenderLabel()
{
	if(!GetLabelBindToChart() && m_label->IsVisible() && !App::Inst().GetMouseDragging())
	{
		// get screen coordinates for location
		Point3D winPos = App::Inst().GetMapController()->ProjectToScreen(GetPosition());

		Box2D bb = m_label->GetBoundingBox();

		float size = m_size * App::Inst().GetResolutionFactor();

		// compensate for perspective projection
		float angle = sin(App::Inst().GetViewport()->GetCamera()->GetPitch()*DEG_TO_RAD);
		float perspectiveSize = size * angle;
		float perspectiveTextHeight = bb.Height()*angle;

		// calculate screen position of label
		uint offset = 2 * App::Inst().GetResolutionFactor();

		if(m_label->GetLabelPosition() == _T("Right"))
			m_label->SetScreenPosition(Point3D(winPos.x+size+offset, winPos.y-bb.Height()/3, LABEL_LAYER));
		else if(m_label->GetLabelPosition() == _T("Left"))
			m_label->SetScreenPosition(Point3D(winPos.x-bb.Width()-size-offset, winPos.y-bb.Height()/3, LABEL_LAYER));
		else if(m_label->GetLabelPosition() == _T("Top"))
			m_label->SetScreenPosition(Point3D(winPos.x-bb.Width()/2, winPos.y+perspectiveSize+offset, LABEL_LAYER));
		else if(m_label->GetLabelPosition() == _T("Bottom"))
			m_label->SetScreenPosition(Point3D(winPos.x-bb.Width()/2, winPos.y-perspectiveTextHeight-perspectiveSize, LABEL_LAYER));

		m_label->Render();
	}
}

void LocationView::ShowPropertiesDlg()
{
	LocationLayerPtr locationLayer;
	for (uint i = 0; i < App::Inst().GetLayerTreeController()->GetNumLocationLayers(); i++)
	{
		locationLayer = App::Inst().GetLayerTreeController()->GetLocationLayer(i);
		if (locationLayer->GetId() == m_locationLayerId)
			break;
	}

	if ( locationLayer->HasPropertiesDialogOpen() )
	{
		locationLayer->GetPropertiesDialog()->SetFocus();
		locationLayer->GetPropertiesDialog()->Raise();
	}
	else
	{
		LocationPropertiesDlg* dlg = new LocationPropertiesDlg(App::Inst().GetMainWindow(), locationLayer);
		dlg->SetPage(LocationPropertiesDlg::SYMBOLOGY);
		dlg->Show();
		dlg->SetFocus();
		dlg->Raise();
		locationLayer->SetPropertiesDialogOpenStatus( true );
	}
}

bool LocationView::IsClicked(const Point2D& mousePt)
{
	Box2D box = GetBoundingBox();
	if(mousePt.x >= box.x && mousePt.x <= box.x + box.dx && 
					mousePt.y >= box.y && mousePt.y <= box.y + box.dy )
	{
		return true;
	}

	return false;
}
bool LocationView::MouseLeftDown(const Point2D& mousePt) 
{ 
	if(!IsVisible())
		return false;

	if(IsClicked(mousePt))
	{
		m_activeView = LOCATION_VIEW;
		return true;
	}

	if(!GetLabelBindToChart() && m_label->MouseLeftDown(mousePt))
	{
		m_activeView = LABEL;
		return true;
	}

	m_activeView = NO_ACTIVE_VIEW;
	return false;
}

void LocationView::MouseLeftUp(const Point2D& mousePt)
{
	if(m_activeView == LABEL)
		m_label->MouseLeftUp(mousePt);

	m_activeView = NO_ACTIVE_VIEW;
}

bool LocationView::MouseMove(const Point2D& mousePt)
{
	if(m_activeView == LABEL)
	return true;

	return false;
}

void LocationView::MouseDragging(const Point2D& mousePt)
{
	if(m_activeView == LABEL)
	{
		m_label->MouseDragging(mousePt);
		SetLabelPositionModified(true);
	}
}

void LocationView::MouseLeftDblClick(const Point2D& mousePt) 
{ 
	ShowPropertiesDlg();
}

bool LocationView::MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu) 
{ 	
	if(IsClicked(mousePt))
	{
		if(popupMenu.GetMenuItems().size() == 0)
		{
			popupMenu.Append(ID_POPUP_MNU_PROPERTIES, wxT("Properties"));
		}

		return true;
	}

	return false;
}

void LocationView::KeyboardEvent(wxKeyEvent& event)
{
	if(event.GetKeyCode() == WXK_DELETE)
	{
		SetVisibility(false);
	}
}

NodeGeoTree* LocationView::GetTreeNode(const std::wstring& id)
{
	LayerTreeControllerPtr layerTreeCtrl = App::Inst().GetLayerTreeController();

	for(uint i = 0; i < layerTreeCtrl->GetNumTreeLayers(); ++i)
	{
		TreeLayerPtr treeLayer = layerTreeCtrl->GetTreeLayer(i);
		GeoTreeViewPtr geoTreeView = treeLayer->GetGeoTreeView(); 

		NodeGeoTree* node = geoTreeView->GetNode(id);
		if(node)
			return node;
	}

	return NULL;
}
