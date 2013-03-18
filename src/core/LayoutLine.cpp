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
#include "../core/LayoutLine.hpp"
#include "../core/Camera.hpp"
#include "../core/View.hpp"
#include "../core/MapController.hpp"
#include "../core/OrthoLayers.hpp"
#include "../core/PopupMenu.hpp"
#include "../core/Viewport.hpp"
#include "../core/VisualMarker.hpp"
#include "../core/LayoutObjectProperties.hpp"

#include "../gui/LayoutObjectPropertiesDlg.hpp"

#include "../utils/ErrorGL.hpp"
#include "../utils/Geometry.hpp"

using namespace GenGIS;

LayoutLine::LayoutLine(): m_stage(START), m_activePt(NONE),	m_layoutLineOffsetPercentage(0.05f)
{
	Point3D pt(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	m_layoutLine.SetStartPt(pt);
	m_layoutLine.SetEndPt(pt);

	m_depth = LAYOUT_PRIMATIVES_LAYER;
}

template<class Archive>
void LayoutLine::serialize(Archive & ar, const unsigned int file_version)
{
	ar & boost::serialization::base_object<View>(*this);
	ar & m_layoutLine;                 // VisualLine
	ar & m_modLayoutLine;              // VisualLine
	ar & m_layoutLineOffsetPercentage; // float
	ar & m_stage;                      // STAGE
	ar & m_activePt;                   // ACTIVE_POINT
}
template void LayoutLine::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void LayoutLine::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void LayoutLine::Render()
{
	if(!IsVisible())
		return;

	error::ErrorGL::Check();

	glDisable(GL_LIGHTING);		
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		

	// set visual properties of control points
	Colour colourCP = App::Inst().GetLayoutObjProp()->GetControlPointColour();
	float sizeCP = App::Inst().GetLayoutObjProp()->GetControlPointSize();

	VisualMarker startCP(colourCP, sizeCP, VisualMarker::SQUARE, m_layoutLine.GetStartPt());
	startCP.SetSelected(IsSelected() && m_stage == COMPLETE && m_activePt == NONE);
	startCP.SetRenderingStyle(VisualMarker::ALWAYS_VISIBLE);
	startCP.SetRotation(0.0);
	startCP.SetDepth(CONTROL_PT_LAYER);

	VisualMarker endCP(colourCP, sizeCP, VisualMarker::SQUARE, m_layoutLine.GetEndPt());
	endCP.SetSelected(IsSelected() && m_stage == COMPLETE && m_activePt == NONE);
	endCP.SetRenderingStyle(VisualMarker::ALWAYS_VISIBLE);
	endCP.SetRotation(0.0);
	endCP.SetDepth(CONTROL_PT_LAYER);

	// set  visual properties of layout line
	m_layoutLine.SetColour(App::Inst().GetLayoutObjProp()->GetLayoutLineColour());
	m_layoutLine.SetSize(App::Inst().GetLayoutObjProp()->GetLayoutLineThickness(false));
	m_layoutLine.SetLineStyle(App::Inst().GetLayoutObjProp()->GetLayoutLineStyle());
	m_layoutLine.SetSelected(IsSelected() && m_stage == COMPLETE && m_activePt == NONE);

	if(m_activePt == NONE)
	{
		// layout line is either in the process of being constructed or completely constructed
		// and in a static state
		if(m_stage == START)
		{
			startCP.SetPosition(App::Inst().GetMouseTerrainPlanePos());
			startCP.Render();
		}
		else if(m_stage == SINGLE_PT)
		{			
			startCP.Render();

			endCP.SetPosition(App::Inst().GetMouseTerrainPlanePos());
			endCP.Render();

			m_layoutLine.SetEndPt(App::Inst().GetMouseTerrainPlanePos());
			m_layoutLine.Render();
		}
		else if(m_stage == COMPLETE)
		{
			startCP.Render();
			endCP.Render();

			m_layoutLine.Render();
		}
	}
	else
	{
		startCP.SetPosition(m_modLayoutLine.GetStartPt());
		startCP.Render();

		endCP.SetPosition(m_modLayoutLine.GetEndPt());
		endCP.Render();

		m_modLayoutLine.Render();
	}

	glEnable(GL_LIGHTING);
		
	error::ErrorGL::Check();
}

void LayoutLine::ShowPropertiesDlg()
{
	LayoutObjectPropertiesDlg* dlg = new LayoutObjectPropertiesDlg(App::Inst().GetMainWindow());
  dlg->Show();
}

bool LayoutLine::MouseLeftDown(const Point2D& mousePt)
{
	if(!IsVisible())
		return false;

	if(App::Inst().GetMode() == App::LAYOUT_LINE)
	{
		if(m_stage == START)
		{
			m_layoutLine.SetStartPt(App::Inst().GetMouseTerrainPlanePos());
			m_stage = SINGLE_PT;
		}
		else if(m_stage == SINGLE_PT)
		{
			m_layoutLine.SetEndPt(App::Inst().GetMouseTerrainPlanePos());
			m_stage = COMPLETE;
			App::Inst().GetViewport()->Refresh(false);
		}

		return true;
	}

	return ModifyLineInteraction(mousePt);
}

void LayoutLine::MouseDragging(const Point2D& mousePt)
{
	// handle dragging event when in layout line construction mode
	if(App::Inst().GetMode() == App::LAYOUT_LINE && m_stage == SINGLE_PT)
	{
		// need to update layout lines to reflect current mouse position
		App::Inst().GetViewport()->Refresh(false);
		return;
	}

	// handle dragging event when in normal interaction mode
	if(m_activePt == START_PT)
	{
		m_modLayoutLine.SetStartPt(App::Inst().GetMouseTerrainPlanePos());
		m_modLayoutLine.SetEndPt(m_layoutLine.GetEndPt());
	}
	else if(m_activePt == END_PT)
	{
		m_modLayoutLine.SetStartPt(m_layoutLine.GetStartPt());
		m_modLayoutLine.SetEndPt(App::Inst().GetMouseTerrainPlanePos());
	}

	App::Inst().GetViewport()->Refresh(false);
}

void LayoutLine::MouseLeftUp(const Point2D& mousePt)
{
	if(m_activePt != NONE)
	{
		m_activePt = NONE;
		m_layoutLine = m_modLayoutLine;
	}
}

bool LayoutLine::MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu)
{
	if(!IsVisible())
		return false;
	
	if(IsPointClicked(mousePt, m_layoutLine.GetStartPt()) 
						|| IsPointClicked(mousePt, m_layoutLine.GetEndPt()))
	{
		if(popupMenu.GetMenuItems().size() == 0)
		{
			popupMenu.Append(ID_POPUP_MNU_PROPERTIES, wxT("Properties"));
		}

		return true;
	}

	return false;
}

void LayoutLine::MouseLeftDblClick(const Point2D& mousePt)
{
	ShowPropertiesDlg();
}

bool LayoutLine::MouseMove(const Point2D& mousePt)
{
	if(App::Inst().GetMode() == App::LAYOUT_LINE)
	{
		if(m_stage == START || m_stage == SINGLE_PT)
			return true;
	}
	else if(m_activePt != NONE)
	{
		return true;
	}

	return false;
}

bool LayoutLine::IsClicked(const Point2D& mousePt)
{
	if(!IsVisible())
		return false;

	return IsPointClicked(mousePt, m_layoutLine.GetStartPt()) || IsPointClicked(mousePt, m_layoutLine.GetEndPt());
}

bool LayoutLine::IsPointClicked(const Point2D& mousePt, const Point3D& controlPt)
{
	float size = App::Inst().GetLayoutObjProp()->GetControlPointSize();
	Point3D pt = App::Inst().GetMapController()->ProjectToScreen(controlPt);
	return (abs(pt.x - mousePt.x) < size && abs(pt.y - mousePt.y) < size);
}

bool LayoutLine::ModifyLineInteraction(const Point2D& mousePt)
{
	// layout line start point
	if(IsPointClicked(mousePt, m_layoutLine.GetStartPt()))
	{
		m_modLayoutLine = m_layoutLine;
		m_activePt = START_PT;
		return true;
	}

	// layout line end point
	if(IsPointClicked(mousePt, m_layoutLine.GetEndPt()))
	{
		m_modLayoutLine = m_layoutLine;
		m_activePt = END_PT;
		return true;
	}

	return false;
}

void LayoutLine::KeyboardEvent(wxKeyEvent& event)
{
	if(event.GetKeyCode() == WXK_ESCAPE)
	{
		if(m_stage == COMPLETE && m_activePt != NONE)
		{
			// reset line to position before interaction started
			m_activePt = NONE;
		}
	}
	else if(event.GetKeyCode() == WXK_DELETE)
	{
		m_activePt = NONE;
		SetVisibility(false);
	}
}

Point3D LayoutLine::GetPerpDirection() const
{	
	return Geometry::NormalToLine(m_layoutLine.GetLine());
}

float LayoutLine::GetLength() const 
{ 
	float lineLen = float(Geometry::LineLength(m_layoutLine.GetLine()));
	return lineLen - 2*m_layoutLineOffsetPercentage*lineLen; 
} 

float LayoutLine::GetLayoutOffset() const 
{ 
	float lineLen = float(Geometry::LineLength(m_layoutLine.GetLine()));
	return m_layoutLineOffsetPercentage*lineLen; 
}
