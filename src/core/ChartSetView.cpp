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

#include "../core/ChartSetView.hpp"
#include "../core/App.hpp"
#include "../core/ChartView.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/LayoutLine.hpp"
#include "../core/LayoutEllipse.hpp"
#include "../core/TreeLayer.hpp"

#include "../gui/GenGisFrame.hpp"

#include "../utils/ErrorGL.hpp"
#include "../utils/Geometry.hpp"
#include "../utils/StringTools.hpp"

using namespace GenGIS;

template<class Archive>
void ChartSetView::serialize(Archive & ar, const unsigned int file_version)
{
	ar & boost::serialization::base_object<View>(*this);
	ar & m_chartViews;            // std::vector<ChartViewPtr>
	ar & m_layoutLine;            // LayoutLinePtr
	ar & m_layoutEllipse;         // LayoutEllipsePtr;
	ar & m_layout;                // LAYOUT_CHART
	ar & m_bLayoutPrimativeMoved; // bool
	ar & m_selectedElement;       // SELECTED_ELEMENT
}
template void ChartSetView::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void ChartSetView::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void ChartSetView::UpdateCharts(const std::wstring& field)
{
	if(!IsVisible())
		return;

	std::wstring chartField = field;
	if(chartField.empty())
		chartField = m_chartViews[0]->GetField();

	float max = 0;
	float min = FLT_MAX;

	// find all values associated with this field
	std::set<std::wstring> values;
	foreach(ChartViewPtr chart, m_chartViews)
	{
		chart->GetValues(chartField, values);
	}

	bool bAccept = true;
	foreach(ChartViewPtr chart, m_chartViews)
	{
		bAccept = bAccept && chart->UpdateChart(chartField, values);

		float numSeq = chart->GetNumActiveSequences();
		if(numSeq < min)
			min = numSeq;
		if(numSeq > max)
			max = numSeq;
	}

	if(!bAccept)
		wxMessageBox(wxT("None numeric type found in specific quantitative field."), wxT("Abundance count error"), wxOK | wxICON_INFORMATION);

	
	foreach(ChartViewPtr chart, m_chartViews)
	{
		float normNumSeq = (float(chart->GetNumActiveSequences()) - min) / (max-min);
		// assert normNumSeq
		if( normNumSeq != normNumSeq ){ normNumSeq = 1; }
		chart->SetNormalizedNumSeq(normNumSeq);	
	}

	((GenGisFrame*)GenGIS::App::Inst().GetMainWindow())->FillSequenceLegend();
}

void ChartSetView::Render()
{
	error::ErrorGL::Check();

	if(m_layoutLine)	
	{
		m_layoutLine->SetSelected(IsSelected());
		m_layoutLine->Render();
	}

	if(m_layoutEllipse)
	{
		m_layoutEllipse->SetSelected(IsSelected());
		m_layoutEllipse->Render();
	}

	error::ErrorGL::Check();
}

void ChartSetView::SetLayout(LAYOUT_CHART layout)
{
	m_layout = layout;

	if(m_layout == NONE)
		return;

	// layout charts along the layout line
	if(m_layout == LAYOUT_LINE_EVEN_SPACING)
	{
		LayoutLineEvenlySpaced();
	}
	else if(m_layout == LAYOUT_LINE_PROJECTED)
	{
		LayoutLineProjected();
	}
	else if(m_layout == ELLIPSE_QUADRANT)
	{
		LayoutEllipseQuadrant();
	}
	else if(m_layout == ELLIPSE_EVEN_SPACING)
	{
		LayoutEllipseEvenlySpaced();
	}
	else if(m_layout == ELLIPSE_CLOSEST_PT)
	{
		LayoutEllipseClosestPt();
	}
	else if(m_layout == DEFAULT)
	{
		DefaultPosition();
	}
	else if(m_layout == LAYOUT_TREE_LEAVES)
	{
		LayoutTreeLeaves();
	}
	App::Inst().SetSaveStatus( false );
}

void ChartSetView::DefaultPosition()
{
	// Position charts over locations (default view)
	for(uint i = 0; i < m_chartViews.size(); ++i)
	{
		m_chartViews.at(i)->SetCentre(m_chartViews.at(i)->GetLocationPos());
	}
}

void ChartSetView::LayoutTreeLeaves()
{
	// Grab the leaf node data for the tree layer 0
	// NOTE: this function only supports a session with a single loaded tree at this time!
	std::vector<NodeGeoTree*> leafNodes = App::Inst().GetLayerTreeController()->GetTreeLayer(0)->GetGeoTreeView()->GetTreeLayer()->GetLeaves();

	// Iterate over all charts (assuming there can be more charts than tree leaf nodes)
	for ( uint i = 0; i < m_chartViews.size(); ++i )
	{
		// Determine the location id of the current chart
		std::wstring chartLocationId = m_chartViews.at( i )->GetLocationView()->GetLocationId();

		// Iterate over all leaf nodes
		foreach( NodeGeoTree* leafNode, leafNodes )
		{
			// Determine the location id of the current leaf node
			std::wstring treeNodeLocationId = leafNode->GetLocationView()->GetLocationId();

			if ( chartLocationId == treeNodeLocationId)
			{
				// Position the chart over the matching tree leaf node
				m_chartViews.at( i )->SetCentre( leafNode->GetGridCoord() );
			}
		}
	}	
}

void ChartSetView::LayoutLineEvenlySpaced()
{
	// Layout charts from start to end of line. For each point along the line,
	// connect it to the chart with the largest signed angle. This ensures that
	// the there will no no crosses between droplines.
	float step = 0;
	if(m_chartViews.size() > 0)
		step = m_layoutLine->GetLength() / (m_chartViews.size()-1);
	Point3D ptOnLine = m_layoutLine->GetStartPt();

	// working copy of chart view vector
	std::vector<ChartViewPtr> chartViews = m_chartViews;

	// need to assigned a chart to each position along the layout line
	for(uint i = 0; i < m_chartViews.size(); ++i)
	{
		std::vector<ChartViewPtr>::iterator it;
		std::vector<ChartViewPtr>::iterator itLargest = chartViews.begin();
		double largestAngle = 0;
		for(it = chartViews.begin(); it != chartViews.end(); ++it)
		{
			ChartViewPtr chart = *it;
			double angle = Geometry::SignedAngleXZ(ptOnLine, m_layoutLine->GetEndPt(), chart->GetLocationPos());
			if(fabs(angle) > fabs(largestAngle))
			{
				largestAngle = angle;
				itLargest = it;
			}
		}

		(*itLargest)->SetCentre(ptOnLine);
		ptOnLine = ptOnLine + step*m_layoutLine->GetDirection();
		chartViews.erase(itLargest);
	}
}

void ChartSetView::LayoutLineProjected()
{
	// find position of each chart on the layout line	
	std::vector<Point3D> m_chartsOnLine;
	foreach(ChartViewPtr chart, m_chartViews)
	{
		Point3D centre = chart->GetLocationPos();

		// project grid coordinates onto layout line
		Point3D closestPoint;
		Geometry::ClosestPointToLine(m_layoutLine->GetLine(), centre, closestPoint);
	
		m_chartsOnLine.push_back(closestPoint);
	}

	// ** Layout charts to where they project along the extended layout line
	for(uint i = 0; i < m_chartViews.size(); ++i)
	{
		m_chartViews.at(i)->SetCentre(m_chartsOnLine.at(i));
	}
}

void ChartSetView::LayoutEllipseQuadrant()
{
	std::vector<Point3D> locationPts;
	foreach(ChartViewPtr chart, m_chartViews)
	{
		locationPts.push_back(chart->GetLocationPos());
	}

	std::vector<Point3D> layoutPts;
	m_layoutEllipse->LayoutPtsByQuadrant(locationPts, layoutPts);

	for(uint i = 0; i < layoutPts.size(); ++i)
	{
		m_chartViews.at(i)->SetCentre(layoutPts.at(i));		
	}
}

void ChartSetView::LayoutEllipseEvenlySpaced()
{
	std::vector<Point3D> locationPts;
	foreach(ChartViewPtr chart, m_chartViews)
	{
		locationPts.push_back(chart->GetLocationPos());
	}

	std::vector<Point3D> layoutPts;
	m_layoutEllipse->LayoutPtsEvenly(locationPts, layoutPts);

	for(uint i = 0; i < layoutPts.size(); ++i)
	{
		m_chartViews.at(i)->SetCentre(layoutPts.at(i));		
	}
}

void ChartSetView::LayoutEllipseClosestPt()
{
	foreach(ChartViewPtr chart, m_chartViews)
	{
		Point3D projPt = m_layoutEllipse->ProjectPt(chart->GetLocationPos());
		chart->SetCentre(projPt);
	}
}

void ChartSetView::SetLayoutLine(LayoutLinePtr layoutLine) 
{ 
	m_layoutLine = layoutLine; 
	
	// a chart set can only have either a layout line or ellipse associated with it
	if(m_layoutEllipse)
		m_layoutEllipse.reset();
}

void ChartSetView::SetLayoutEllipse(LayoutEllipsePtr ellipse) 
{ 
	m_layoutEllipse = ellipse; 

	// a chart set can only have either a layout line or ellipse associated with it
	if(m_layoutLine)
		m_layoutLine.reset();
}

void ChartSetView::ShowPropertiesDlg()
{
	if(m_layoutLine)
		m_layoutLine->ShowPropertiesDlg();
	else if(m_layoutEllipse)
		m_layoutEllipse->ShowPropertiesDlg();
}

bool ChartSetView::MouseLeftDown(const Point2D& mousePt) 
{ 
	if(m_layoutLine && m_layoutLine->MouseLeftDown(mousePt))
	{
		m_selectedElement = LAYOUT_LINE;
		return true;
	}
	else if(m_layoutEllipse && m_layoutEllipse->MouseLeftDown(mousePt))
	{
		m_selectedElement = LAYOUT_ELLIPSE;
		return true;
	}

	m_selectedElement = NO_SELECTED_ELEMENT;
	return false;
}


void ChartSetView::MouseLeftUp(const Point2D& mousePt)
{
	if(m_layoutLine)
		m_layoutLine->MouseLeftUp(mousePt);
	else if(m_layoutEllipse)
		m_layoutEllipse->MouseLeftUp(mousePt);

	// need to layout the tree if the layout primative have moved
	if(m_bLayoutPrimativeMoved)
	{		
		m_bLayoutPrimativeMoved = false;
		SetLayout(m_layout);				
	}
}

void ChartSetView::MouseLeftDblClick(const Point2D& mousePt) 
{ 
	if(m_layoutLine)
		m_layoutLine->MouseLeftDblClick(mousePt);
	else if(m_layoutEllipse)
		m_layoutEllipse->MouseLeftDblClick(mousePt);
}

bool ChartSetView::MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu) 
{ 
	if(m_layoutLine && m_layoutLine->MouseRightDown(mousePt, popupMenu))
	{
		m_selectedElement = LAYOUT_LINE;
		return true;
	}
	else if(m_layoutEllipse && m_layoutEllipse->MouseRightDown(mousePt, popupMenu))
	{
		m_selectedElement = LAYOUT_ELLIPSE;
		return true;
	}

	m_selectedElement = NO_SELECTED_ELEMENT;
	return false;
}

void ChartSetView::MouseDragging(const Point2D& mousePt) 
{ 
	m_bLayoutPrimativeMoved = true;

	if(m_layoutLine)
		m_layoutLine->MouseDragging(mousePt);
	else if(m_layoutEllipse)
		m_layoutEllipse->MouseDragging(mousePt);
}

bool ChartSetView::MouseMove(const Point2D& mousePt)
{
	m_bLayoutPrimativeMoved = true;

	if(m_layoutLine)
		return m_layoutLine->MouseMove(mousePt);
	else if(m_layoutEllipse)
		return m_layoutEllipse->MouseMove(mousePt);

	return true;
}

void ChartSetView::KeyboardEvent(wxKeyEvent& event)
{
	if(m_layoutLine)
	{
		m_layoutLine->KeyboardEvent(event);
		m_bLayoutPrimativeMoved = false;
	}
	else if(m_layoutEllipse)
	{
		m_layoutEllipse->KeyboardEvent(event);
		m_bLayoutPrimativeMoved = false;
	}
}

bool ChartSetView::GetLayoutLineVisibility() const
{ 
	if(m_layoutLine)
		return m_layoutLine->IsVisible(); 

	return false;
}

void ChartSetView::SetLayoutLineVisibility(bool state) 
{ 
	if(m_layoutLine)
		m_layoutLine->SetVisibility(state); 
}


bool ChartSetView::GetLayoutEllipseVisibility() const
{ 
	if(m_layoutEllipse)
		return m_layoutEllipse->IsVisible(); 

	return false;
}

void ChartSetView::SetLayoutEllipseVisibility(bool state)
{ 
	if(m_layoutEllipse)
		m_layoutEllipse->SetVisibility(state); 
}

void ChartSetView::SetSelectionVisiblity( bool visible ) 
{ 
	if(m_selectedElement == CHART)
		SetVisibility(visible); 
	else if(m_selectedElement == LAYOUT_LINE)
		m_layoutLine->SetVisibility(visible); 
	else if(m_selectedElement == LAYOUT_ELLIPSE)
		m_layoutEllipse->SetVisibility(visible); 
}
