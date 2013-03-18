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

#include "../widgets/NavView2D.hpp"

#include "../core/App.hpp"
#include "../core/Camera.hpp"

#include "../core/Viewport.hpp"

using namespace GenGIS;

NavView2D::NavView2D() : ViewWidget() 
{
	m_zoomInOrigin.x = 0;
	m_zoomInOrigin.y = 0;

	m_zoomOutOrigin.x = 0;
	m_zoomOutOrigin.y = 0;

	m_zoomInRadii.x = 0;
	m_zoomInRadii.y = 0;

	m_zoomOutRadii.x = 0;
	m_zoomOutRadii.y = 0;
}

bool NavView2D::MouseLeftDown(const Point2D& mousePt)
{
	const float ZOOM_SENSITIVITY = 0.25f;
	const float TRANSLATION_SENSITIVITY = 0.03f;

	if(!IsVisible()) 
		return false;

	if(IsOnZoomIn(mousePt))
	{
		App::Inst().GetViewport()->GetCamera()->ZoomDelta((float)(-ZOOM_SENSITIVITY));
		return true;
	}
	else if(IsOnZoomOut(mousePt))
	{
		App::Inst().GetViewport()->GetCamera()->ZoomDelta((float)(ZOOM_SENSITIVITY));
		return true;
	}
	else if(IsOnDownArrow(mousePt))
	{
		App::Inst().GetViewport()->GetCamera()->TranslateDelta(0, TRANSLATION_SENSITIVITY);
		return true;
	}
	else if(IsOnUpArrow(mousePt))
	{
		App::Inst().GetViewport()->GetCamera()->TranslateDelta(0, -TRANSLATION_SENSITIVITY);
		return true;
	}
	else if(IsOnLeftArrow(mousePt))
	{
		App::Inst().GetViewport()->GetCamera()->TranslateDelta(-TRANSLATION_SENSITIVITY, 0);
		return true;
	}
	else if(IsOnRightArrow(mousePt))
	{
		App::Inst().GetViewport()->GetCamera()->TranslateDelta(TRANSLATION_SENSITIVITY, 0);
		return true;
	}

	return false;
}

bool NavView2D::IsOnZoomIn(const Point2D& mousePt) 
{   
	Point2D origin = GetZoomInOrigin();
	Point2D radii = GetZoomInRadii();

	return IsInEllipse(mousePt,origin,radii);
}

bool NavView2D::IsOnZoomOut(const Point2D& mousePt) 
{
	Point2D origin = GetZoomOutOrigin();
	Point2D radii = GetZoomOutRadii();

	return IsInEllipse(mousePt,origin,radii);
}
bool NavView2D::IsOnLeftArrow(const Point2D& mousePt) 
{
	Point2D origin = GetLeftArrowOrigin();
	Point2D radii = GetLeftArrowRadii();

	return IsInEllipse(mousePt,origin,radii);
}

bool NavView2D::IsOnRightArrow(const Point2D& mousePt) 
{
	Point2D origin = GetRightArrowOrigin();
	Point2D radii = GetRightArrowRadii();

	return IsInEllipse(mousePt,origin,radii);
}

bool NavView2D::IsOnUpArrow(const Point2D& mousePt) 
{
	Point2D origin = GetUpArrowOrigin();
	Point2D radii = GetUpArrowRadii();

	return IsInEllipse(mousePt,origin,radii);
}

bool NavView2D::IsOnDownArrow(const Point2D& mousePt) 
{
	Point2D origin = GetDownArrowOrigin();
	Point2D radii = GetDownArrowRadii();

	return IsInEllipse(mousePt,origin,radii);
}

bool NavView2D::IsInEllipse(const Point2D& mousePt, const Point2D& origin, const Point2D& radii ) 
{
	// origin of button
	Box2D box = GetBoundingBox();
	float oX = box.x + (origin.x * box.dx + box.dx) * 0.5f;
	float oY = box.y + (origin.y * box.dy + box.dy) * 0.5f;

	// size of button
	float radX = box.dx * 0.5f * radii.x;
	float radY = box.dy * 0.5f * radii.y;

	// quick bounds check
	if(fabs(mousePt.x - oX) > radX) 
		return false;
	if(fabs(mousePt.y - oY) > radY) 
		return false;

	return true;
}
