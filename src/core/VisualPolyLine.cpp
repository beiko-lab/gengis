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

#include "../core/VisualPolyLine.hpp"
#include "../core/VisualMarker.hpp"
#include "../core/App.hpp"
#include "../core/OrthoLayers.hpp"
#include "../core/LayoutObjectProperties.hpp"

#include "../utils/ErrorGL.hpp"
#include "../utils/Log.hpp"

using namespace GenGIS;

VisualPolyLine::VisualPolyLine(Colour colour, float size, VisualLine::LINE_STYLE lineStyle) :
	VisualObject(colour, size, true), m_lineStyle(lineStyle), m_bRenderCtrlPts(true)
{

}

VisualPolyLine::~VisualPolyLine() 
{ 

}

template<class Archive>
void VisualPolyLine::serialize(Archive & ar, const unsigned int file_version)
{
	ar & boost::serialization::base_object<VisualObject>(*this);
	ar & m_lineStyle;      // VisualLine::LINE_STYLE
	ar & m_polyline;       // PolyLine3D
	ar & m_bRenderCtrlPts; // bool
}
template void VisualPolyLine::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void VisualPolyLine::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


void VisualPolyLine::SetFirstPt(const Point3D& pt) 
{ 
	if(!m_polyline.pts.empty())
		m_polyline.pts.at(0) = pt; 
	else
		m_polyline.pts.push_back(pt);
}

void VisualPolyLine::RenderControlPt(const Point3D& pt, const Colour& colour, float size, VisualMarker::MARKER_SHAPE shape, int depth)
{
	VisualMarker::RenderAlwaysVisible(pt, colour, size, shape, 0.0f, Colour(0,0,0), 0, depth);
}

void VisualPolyLine::RenderPolyLine(const Colour& colour, VisualLine::LINE_STYLE lineStyle, float size)
{
	Point3D startPt = m_polyline.GetFirstPt();
	for(uint i = 1; i < m_polyline.pts.size(); ++i)
	{				
		Point3D endPt = m_polyline.pts.at(i);
		VisualLine::RenderLineWithBorder(Line3D(startPt, endPt), colour, size, lineStyle, colour, 0, LAYOUT_PRIMATIVES_LAYER, m_bSelected);

		startPt = endPt;
	}
}

void VisualPolyLine::RenderControlPts(const Colour& colour, float size, int depth)
{
	RenderControlPt(m_polyline.GetFirstPt(), colour, size, VisualMarker::TRIANGLE, depth);

	for(uint i = 1; i < m_polyline.pts.size(); ++i)
	{				
		RenderControlPt(m_polyline.pts.at(i), colour, size, VisualMarker::SQUARE, depth);
	}
}

void VisualPolyLine::Render()
{
	if(!m_bVisible)
		return;

	error::ErrorGL::Check();

	RenderPolyLine(m_colour, m_lineStyle, m_size);

	if(m_bRenderCtrlPts)
	{
		if(m_bSelected)
		{
			RenderControlPts(App::Inst().GetSelectionColour(), 
				App::Inst().GetLayoutObjProp()->GetControlPointSize() + App::Inst().GetSelectionThickness(), 
				CONTROL_PT_LAYER+1);
		}

		RenderControlPts(App::Inst().GetLayoutObjProp()->GetControlPointColour(), 
			App::Inst().GetLayoutObjProp()->GetControlPointSize(),
			CONTROL_PT_LAYER);
	}

	error::ErrorGL::Check();
}
