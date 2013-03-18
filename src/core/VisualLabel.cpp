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

#include "../core/VisualLabel.hpp"
#include "../core/App.hpp"
#include "../core/Viewport.hpp"
#include "../core/Camera.hpp"
#include "../core/PopupMenu.hpp"
#include "../core/MapController.hpp"
#include "../core/OrthoLayers.hpp"

#include "../gui/LabelToolDlg.hpp"

#include "../utils/ErrorGL.hpp"
#include "../utils/Log.hpp"

using namespace GenGIS;

VisualLabel::VisualLabel() :
	VisualObject(Colour(255,255,255), 12, false), m_text(_T("<none>")), 
		m_labelPosition(_T("Right")), m_labelStyle(_T("Dynamic")), m_screenPos(Point3D(0,0,0)), m_bRotateX(false),
		m_zRotation(0.0), m_bRotateZ(false), m_renderingStyle(PERSPECTIVE), m_bDragging(false), m_bGridPosSet(true)
{

}

VisualLabel::VisualLabel(const std::wstring& text, const Colour& colour, float size) :
	VisualObject(colour, size, true), m_text(text),
		m_labelPosition(_T("Right")), m_labelStyle(_T("Dynamic")), m_screenPos(Point3D(0,0,0)), 
		m_bRotateX(false), m_zRotation(0.0), m_bRotateZ(false), m_renderingStyle(PERSPECTIVE),
		m_bDragging(false), m_bGridPosSet(true)
{

}

VisualLabel::VisualLabel(const std::wstring& text, const Colour& colour, float size, LABEL_RENDERING_STYLE m_renderingStyle)
 : VisualObject(colour, size, true), m_text(text), m_labelPosition(_T("<USER>")), m_labelStyle(_T("Dynamic")), 
		m_screenPos(Point3D(0,0,0)), m_bRotateX(false), m_zRotation(0.0), m_bRotateZ(false), m_renderingStyle(PERSPECTIVE),
		m_bDragging(false), m_bGridPosSet(false)
{

}

template<class Archive>
void VisualLabel::serialize(Archive & ar, const unsigned int file_version)
{
	ar & boost::serialization::base_object<VisualObject>(*this);
	ar & m_id;             // uint
	ar & m_text;           // std::wstring
	ar & m_labelPosition;  // std::wstring
	ar & m_labelStyle;     // std::wstring
	ar & m_screenPos;      // Point3D
	ar & m_gridPos;        // Point3D
	ar & m_bRotateX;       // bool
	ar & m_bRotateZ;       // bool
	ar & m_zRotation;      // float
	ar & m_mousePrevPt;    // Point2D
	ar & m_renderingStyle; // LABEL_RENDERING_STYLE
	//ar & m_bDragging;      // bool
}
template void VisualLabel::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void VisualLabel::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void VisualLabel::SetSize(float size)
{
	m_size = size;
}

void VisualLabel::Render()
{
	if(App::Inst().GetMouseDragging() && !m_bDragging)
		return;

	error::ErrorGL::Check();

	glDepthRange(0.0f, TERRAIN_START_DEPTH);

	if(!m_bVisible)
		return;

	if(m_labelPosition == _T("<Custom>") || m_labelPosition == _T("<USER>"))
	{
		if(m_bGridPosSet)
		{
			Point3D screenPos = App::Inst().GetMapController()->ProjectToScreen(GetGridPosition());
			screenPos.z = LABEL_LAYER;
			SetScreenPosition(screenPos);
		}
	}

	App::Inst().GetViewport()->GetCamera()->SetOrthoCamera();

	// render label
	glPushMatrix();
	glColor4f(m_colour.GetRed(), m_colour.GetGreen(), m_colour.GetBlue(), m_colour.GetAlpha());

	glTranslatef(floor(m_screenPos.x+0.5f), floor(m_screenPos.y+0.5f), -m_screenPos.z);

	if(m_renderingStyle == PERSPECTIVE)
	{
		if(m_bRotateX)
			glRotatef(90-App::Inst().GetViewport()->GetCamera()->GetPitch(), 1.0f, 0.0f, 0.0f);	

		if(m_bRotateZ)
			glRotatef(m_zRotation+App::Inst().GetViewport()->GetCamera()->GetYaw(), 0.0f, 0.0f, 1.0f);		
	}

	App::Inst().GetFont()->Size(m_size * App::Inst().GetResolutionFactor());
	App::Inst().GetFont()->Render(m_text, 0, 0);
	glPopMatrix();


	App::Inst().GetViewport()->GetCamera()->UnsetOrthoCamera();

	error::ErrorGL::Check();
}

void VisualLabel::ShowPropertiesDlg()
{
	LabelToolDlg* dlg = new LabelToolDlg(App::Inst().GetMainWindow());

	// set current values
	std::wstring text = GetText();
	dlg->SetText(wxString(text.c_str()));
	dlg->SetSize(GetSize());
	dlg->SetColour(wxColour(GetColour().GetRedInt(),
		GetColour().GetGreenInt(),
		GetColour().GetBlueInt()));
	dlg->SetStyle(wxString(GetLabelStyle().c_str(), wxConvISO8859_1));
	dlg->SetCustomColourButton();

	// set new values
	if(dlg->ShowModal())
	{
		if(!dlg->GetText().IsEmpty() && dlg->SaveChanges())
		{
			SetText(dlg->GetText().c_str());
			SetSize(dlg->GetSize());
			SetColour(Colour(dlg->GetColour()));
			SetVisibility(true);
			SetLabelPosition(_T("<Custom>"));

			SetLabelStyle(dlg->GetStyle().c_str());
		}
	}
}

bool VisualLabel::MouseLeftDown(const Point2D& mousePt)
{
	m_bDragging = false;

	Box2D bb = GetBoundingBox();

	if(mousePt.x >= m_screenPos.x + bb.x && mousePt.x <= bb.x + bb.dx + m_screenPos.x 
		&& mousePt.y >= bb.y + m_screenPos.y && mousePt.y <= bb.y + bb.dy + m_screenPos.y )
	{
		m_bDragging = true;
		m_mousePrevPt = mousePt;
		return true;
	}

	return false;
}

void VisualLabel::MouseLeftUp(const Point2D& mousePt)
{
	m_bDragging = false;
}

bool VisualLabel::MouseMove(const Point2D& mousePt)
{
	return true;
}

void VisualLabel::MouseDragging(const Point2D& mousePt)
{
	m_bDragging = true;
	m_labelPosition = _T("<Custom>");
	m_gridPos = App::Inst().GetMouseWorldPos();
}

bool VisualLabel::MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu)
{
	Box2D bb = GetBoundingBox();

	if(mousePt.x >= m_screenPos.x + bb.x && mousePt.x <= bb.x + bb.dx + m_screenPos.x 
		&& mousePt.y >= bb.y + m_screenPos.y && mousePt.y <= bb.y + bb.dy + m_screenPos.y )
	{			
		if(popupMenu.GetMenuItems().size() == 0)
		{
			popupMenu.Append(ID_POPUP_MNU_PROPERTIES, wxT("Properties"));
			popupMenu.AppendSeparator();
			popupMenu.Append(ID_POPUP_MNU_REMOVE, wxT("Remove"));
		}

		return true;
	}

	return false;	
}

void VisualLabel::SetLabelStyle(const std::wstring& style) 
{ 
	m_labelStyle = style; 

	if(m_labelStyle == _T("Billboard"))
	{
		SetRotateLabel(false);
	}
	else if(m_labelStyle == _T("Dynamic"))
	{
		SetRotateLabel(true);
	}	
}

Box2D VisualLabel::GetBoundingBox() const 
{ 
	App::Inst().GetFont()->Size(m_size * App::Inst().GetResolutionFactor());
	return App::Inst().GetFont()->BoundingBox(m_text); 
}