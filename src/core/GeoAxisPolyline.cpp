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
#include "../core/GeoAxisPolyline.hpp"
#include "../core/Camera.hpp"
#include "../core/View.hpp"
#include "../core/MapController.hpp"
#include "../core/OrthoLayers.hpp"
#include "../core/PopupMenu.hpp"
#include "../core/Viewport.hpp"
#include "../core/LocationView.hpp"
#include "../core/LayoutObjectProperties.hpp"

#include "../gui/LayoutObjectPropertiesDlg.hpp"

#include "../utils/ErrorGL.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/Geometry.hpp"

using namespace GenGIS;

GeoAxisPolyline::GeoAxisPolyline(): m_stage(START), m_activePt(NONE)
{

}

template<class Archive>
void GeoAxisPolyline::serialize(Archive & ar, const unsigned int file_version)
{
	ar & boost::serialization::base_object<View>(*this);
	ar & m_tree;                // Tree<NodeGeoTree>::Ptr
	ar & m_polylines;           // std::vector<VisualPolyLinePtr>
	ar & m_modPolylines;        // std::vector<VisualPolyLinePtr>
	ar & m_activePolylineIndex; // uint
	ar & m_activePtIndex;       // uint
	ar & m_stage;               // STAGE
	ar & m_activePt;            // ACTIVE_POINT
	ar & m_length;              // float
	ar & m_geoAxisMap;          // std::map< LocationViewPtr, GeoPointData >
	ar & m_activeLeafNodes;     // uint
}
template void GeoAxisPolyline::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void GeoAxisPolyline::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void GeoAxisPolyline::Render()
{
	if(!IsVisible())
		return;

	error::ErrorGL::Check();

	glDisable(GL_LIGHTING);		
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		

	// set visual properties of control point
	Colour colourCP = App::Inst().GetLayoutObjProp()->GetControlPointColour();
	float sizeCP = App::Inst().GetLayoutObjProp()->GetControlPointSize();

	VisualMarker controlPt(colourCP, sizeCP, VisualMarker::SQUARE, Point3D());
	controlPt.SetSelected(IsSelected() && m_stage == COMPLETE && m_activePt == NONE);
	controlPt.SetRenderingStyle(VisualMarker::ALWAYS_VISIBLE);
	controlPt.SetRotation(0.0);
	controlPt.SetDepth(CONTROL_PT_LAYER);

	// get visual properties of polylines
	Colour colourLine = App::Inst().GetLayoutObjProp()->GetLayoutLineColour();
	VisualLine::LINE_STYLE lineStyle = App::Inst().GetLayoutObjProp()->GetLayoutLineStyle();
	float thicknessLine = App::Inst().GetLayoutObjProp()->GetLayoutLineThickness();

	if(m_activePt == NONE)
	{
		// set visual properties of polylines
		foreach(VisualPolyLinePtr polyLine, m_polylines)
		{
			polyLine->SetColour(colourLine);
			polyLine->SetLineStyle(lineStyle);
			polyLine->SetSize(thicknessLine);
			polyLine->SetSelected(IsSelected() && m_stage == COMPLETE && m_activePt == NONE);
		}

		// polyline is either in the process of being constructed or completely constructed
		// and in a static state	
		if(m_stage == START)
		{
			controlPt.SetPosition(App::Inst().GetMouseWorldPos());
			controlPt.Render();
		}
		else if(m_stage == LAYING_OUT_PTS)
		{		
			// render current point being laid out
			controlPt.SetPosition(App::Inst().GetMouseWorldPos());
			//m_polylines.SetNextPoint(App::Inst().GetMouseWorldPos());

			foreach(VisualPolyLinePtr polyLine, m_polylines)
			{
				polyLine->Render();
			}

			VisualPolyLinePtr polyLine( new VisualPolyLine(colourLine, thicknessLine, lineStyle));
			polyLine->SetFirstPt(m_polylines.at(m_polylines.size()-1)->GetLastPt());
			polyLine->SetNextPt(App::Inst().GetMouseWorldPos());
			polyLine->Render();
		}
		else if(m_stage == COMPLETE)
		{
			foreach(VisualPolyLinePtr polyLine, m_polylines)
			{
				polyLine->Render();
			}
		}
	}
	else
	{
		// set visual properties of modified polylines and render
		foreach(VisualPolyLinePtr polyLine, m_modPolylines)
		{
			polyLine->SetColour(colourLine);
			polyLine->SetLineStyle(lineStyle);
			polyLine->SetSize(thicknessLine);
			polyLine->SetSelected(IsSelected() && m_stage == COMPLETE && m_activePt == NONE);
			polyLine->Render();
		}
	}

	glEnable(GL_LIGHTING);

	error::ErrorGL::Check();
}

void GeoAxisPolyline::ShowPropertiesDlg()
{
	LayoutObjectPropertiesDlg* dlg = new LayoutObjectPropertiesDlg(App::Inst().GetMainWindow());
	dlg->Show();
}

bool GeoAxisPolyline::MouseLeftDown(const Point2D& mousePt)
{
	if(!IsVisible())
		return false;

	if(App::Inst().GetMode() == App::GEO_AXIS_POLYLINE)
	{
		if(m_stage == START)
		{
			VisualPolyLinePtr polyline( new VisualPolyLine());
			polyline->SetNextPt(App::Inst().GetMouseWorldPos());
			m_polylines.push_back(polyline);
			m_stage = LAYING_OUT_PTS;
		}
		else if(m_stage == LAYING_OUT_PTS)
		{
			m_polylines.at(m_polylines.size()-1)->SetNextPt(App::Inst().GetMouseWorldPos());
			App::Inst().GetViewport()->Refresh(false);
		}

		return true;
	}

	// seems the geographix axis has already been assigned to a tree
	if(m_stage == LAYING_OUT_PTS)
	{
		m_polylines.at(m_polylines.size()-1)->SetNextPt(App::Inst().GetMouseWorldPos());
		App::Inst().GetViewport()->Refresh(false);
		return true;
	}

	return ModifyLineInteraction(mousePt);
}

void GeoAxisPolyline::MouseDragging(const Point2D& mousePt)
{
	// handle dragging event when in polyline construction mode
	if(m_stage == LAYING_OUT_PTS)
	{
		// need to update geographic axis to reflect current mouse position
		App::Inst().GetViewport()->Refresh(false);
		return;
	}

	// handle dragging event when in normal interaction mode
	if(m_activePt == CONTROL_PT)
	{
		m_modPolylines.at(m_activePolylineIndex)->SetPt(m_activePtIndex, App::Inst().GetMouseWorldPos());
	}

	App::Inst().GetViewport()->Refresh(false);
}

void GeoAxisPolyline::MouseLeftUp(const Point2D& mousePt)
{
	if(m_activePt != NONE)
	{
		m_activePt = NONE;
		m_polylines = m_modPolylines;
	}
}

bool GeoAxisPolyline::MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu)
{
	if(!IsVisible())
		return false;

	for(uint i = 0; i < m_polylines.size(); ++i)
	{
		for(uint j = 0; j < m_polylines.at(i)->GetNumPts(); ++j)
		{
			if(IsPointClicked(mousePt, m_polylines.at(i)->GetPt(j)))
			{
				if(popupMenu.GetMenuItems().size() == 0)
				{
					popupMenu.Append(ID_POPUP_MNU_PROPERTIES, wxT("Properties"));
					popupMenu.AppendSeparator();
					popupMenu.Append(ID_POPUP_MNU_EXTEND_POLYLINE, wxT("Extend geographic axis"));
				}

				m_activePtIndex = j;
				m_activePolylineIndex = i;

				return true;
			}
		}
	}

	return false;
}

void GeoAxisPolyline::MouseLeftDblClick(const Point2D& mousePt)
{
	ShowPropertiesDlg();
}

bool GeoAxisPolyline::MouseMove(const Point2D& mousePt)
{
	if(m_stage == START || m_stage == LAYING_OUT_PTS)
	{
		return true;
	}
	else if(m_activePt != NONE)
	{
		return true;
	}

	return false;
}

bool GeoAxisPolyline::IsClicked(const Point2D& mousePt)
{
	if(!IsVisible())
		return false;

	for(uint i = 0; i < m_polylines.size(); ++i)
	{
		for(uint j = 0; j < m_polylines.at(i)->GetNumPts(); ++j)
		{
			if(IsPointClicked(mousePt, m_polylines.at(i)->GetPt(j)))			
			{

				return true;
			}
		}
	}

	return false;
}

bool GeoAxisPolyline::IsPointClicked(const Point2D& mousePt, const Point3D& controlPt)
{
	float size = App::Inst().GetLayoutObjProp()->GetControlPointSize();
	Point3D pt = App::Inst().GetMapController()->ProjectToScreen(controlPt);
	return (abs(pt.x - mousePt.x) < size && abs(pt.y - mousePt.y) < size);
}

bool GeoAxisPolyline::ModifyLineInteraction(const Point2D& mousePt)
{
	for(uint i = 0; i < m_polylines.size(); ++i)
	{
		for(uint j = 0; j < m_polylines.at(i)->GetNumPts(); ++j)
		{
			if(IsPointClicked(mousePt, m_polylines.at(i)->GetPt(j)))
			{
				m_modPolylines = m_polylines;
				m_activePt = CONTROL_PT;
				m_activePtIndex = j;
				m_activePolylineIndex = i;
				return true;
			}
		}
	}

	return false;
}

void GeoAxisPolyline::KeyboardEvent(wxKeyEvent& event)
{
	if(event.GetKeyCode() == WXK_ESCAPE)
	{
		if(m_stage == COMPLETE && m_activePt != NONE)
		{
			// reset polyline to position before interaction started
			m_activePt = NONE;
		}
	}
	else if(event.GetKeyCode() == WXK_DELETE)
	{
		m_activePt = NONE;
		SetVisibility(false);
	}
	else if(event.GetKeyCode() == WXK_RETURN)
	{
		m_activePt = NONE;
		m_stage = COMPLETE;
	}
}

void GeoAxisPolyline::PolylineFinished() 
{ 
	m_stage = COMPLETE; 
	m_activePt = NONE; 
}


// Structure used to sort geographical location
typedef struct sGEO_LOCATION_SORTER
{
	sGEO_LOCATION_SORTER(LocationViewPtr _locationView, float _distance): locationView(_locationView), distance(_distance) {}

	LocationViewPtr locationView;
	float distance;
} GeoLocationSorter;

// Predicate used to sort items in GeoLocationSorter.
bool GeoLocationPredicate(GeoLocationSorter elem1, GeoLocationSorter elem2)
{
	return elem1.distance < elem2.distance;
}

void GeoAxisPolyline::SetTree(Tree<NodeGeoTree>::Ptr tree) 
{ 
	m_tree = tree; 
}

void GeoAxisPolyline::OrderOfGeographicLocations()
{
	if(!m_tree)
		return;

	m_geoAxisMap.clear();

	// get list of all locations associated with the tree
	std::set< LocationViewPtr > locationViews;
	std::vector< NodeGeoTree* > leafNodes = m_tree->GetLeaves();
	foreach(NodeGeoTree* leafNode, leafNodes)
		locationViews.insert(leafNode->GetLocationView());

	// check if any of the geographic locations are intersected by the geographic axis polyline
	uint pos = 0;
	m_length = 0;
	for(uint i = 0; i < m_modPolylines.size(); ++i)
	{
		for(uint j = 0; j < m_modPolylines.at(i)->GetNumPts()-1; ++j)
		{
			std::vector<GeoLocationSorter> geoLocationSorter;

			std::set<LocationViewPtr>::iterator iter = locationViews.begin();
			std::vector< std::set<LocationViewPtr>::iterator > assignedLocations;
			while(iter != locationViews.end())
			{
				LocationViewPtr locationView = *iter;

				float dist;
				Line3D line(m_modPolylines.at(i)->GetPt(j), m_modPolylines.at(i)->GetPt(j+1));
				if(IntersectGeoLocation(locationView, line, dist))
				{
					geoLocationSorter.push_back(GeoLocationSorter(locationView, m_length + dist));
					assignedLocations.push_back(iter);
					++iter;
				}
				else
				{
					++iter;
				}
			}

			// erase all assigned locations
			foreach(std::set<LocationViewPtr>::iterator iter, assignedLocations)
				locationViews.erase(iter);

			// sort geographic locations in asending order of distance
			std::sort(geoLocationSorter.begin(), geoLocationSorter.end(), GeoLocationPredicate);

			// add geographic locations to map which indicates the position of each geographic location on the geographic axis
			for(uint k = 0; k < geoLocationSorter.size(); ++k)
			{
				m_geoAxisMap[geoLocationSorter.at(k).locationView] = GeoPointData(pos, geoLocationSorter.at(k).distance);
				pos++;
			}

			// accumulate length of geographic axis
			m_length += Geometry::LineLength(Line3D(m_modPolylines.at(i)->GetPt(j), m_modPolylines.at(i)->GetPt(j+1)));
		}
	}

	GetNumActiveLeafNodes(true);
}

bool GeoAxisPolyline::IntersectGeoLocation(const LocationViewPtr geoLocationPos, const Line3D& line, float& distFromStart)
{
	double MAGIC_THRESHOLD = 0.075;

	Point3D gridPos = geoLocationPos->GetPosition();

	Point3D closestPt;
	bool bOnLineSegment = Geometry::ClosestPointToLine(line, gridPos, closestPt);

	if(bOnLineSegment)
	{
		double dist = Geometry::Distance(closestPt, gridPos);
		if(dist < MAGIC_THRESHOLD*Geometry::LineLength(line))
		{
			distFromStart = Geometry::Distance(closestPt, line.start);
			return true;
		}
	}

	distFromStart = std::numeric_limits<float>::max();
	return false;
}

bool GeoAxisPolyline::GeographicLocationPos(LocationViewPtr locationView, uint& pos, float& dist)
{
	std::map< LocationViewPtr, GeoPointData >::const_iterator it = m_geoAxisMap.find(locationView);
	if(it != m_geoAxisMap.end())
	{
		pos = (*it).second.pos;
		dist = (*it).second.dist;
		return true;
	}

	return false;
}

void GeoAxisPolyline::ExtendGeographicAxis()
{
	VisualPolyLinePtr polyline( new VisualPolyLine());
	polyline->SetNextPt(m_polylines.at(m_activePolylineIndex)->GetPt(m_activePtIndex));
	m_polylines.push_back(polyline);
	m_stage = LAYING_OUT_PTS;
}

uint GeoAxisPolyline::GetNumActiveLeafNodes(bool bUpdate)
{ 
	if(bUpdate)
	{
		m_activeLeafNodes = 0;
		std::vector< NodeGeoTree* > leafNodes = m_tree->GetLeaves();

		std::map< LocationViewPtr, GeoPointData >::iterator it;
		for(it = m_geoAxisMap.begin(); it != m_geoAxisMap.end(); ++it)
		{
			foreach(NodeGeoTree* leafNode, leafNodes)
			{
				if(leafNode->GetLocationView() == it->first)
				{
					m_activeLeafNodes++;
				}
			}
		}
	}

	return m_activeLeafNodes;
}

uint GeoAxisPolyline::GetNumPolyLineOrderings() const
{
	uint numOrderings = 1;
	for(uint i = 1; i <= m_polylines.size(); ++i)
		numOrderings *= i;

	return numOrderings;
}

std::vector<uint> GeoAxisPolyline::SetPolyLineOrdering(uint ordering)
{
	// create temporary permutation vector
	std::vector<uint> permutationVec;
	for(uint i = 0; i < m_polylines.size(); ++i)
		permutationVec.push_back(i);

	// move to desired permutation
	for(uint i = 0; i < ordering; ++i)
		next_permutation(permutationVec.begin(), permutationVec.end());

	// put polylines in order specified by permutation vector
	m_modPolylines.clear();

	for(uint i = 0; i < m_polylines.size(); ++i)
		m_modPolylines.push_back(m_polylines.at(permutationVec.at(i)));

	OrderOfGeographicLocations();

	return permutationVec;
}