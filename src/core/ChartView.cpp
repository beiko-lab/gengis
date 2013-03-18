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
#include "../core/ChartView.hpp"
#include "../core/Camera.hpp"
#include "../core/DropLines.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/SequenceModel.hpp"
#include "../core/MapController.hpp"
#include "../core/PopupMenu.hpp"
#include "../core/OrthoLayers.hpp"
#include "../core/Viewport.hpp"
#include "../core/VisualLabel.hpp"
#include "../core/LocationModel.hpp"

#include "../core/PieChartView.hpp"
#include "../core/BarChartView.hpp"

#include "../utils/ColourMapDiscrete.hpp"
#include "../utils/Log.hpp"
#include "../utils/ErrorGL.hpp"
#include "../utils/Font.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/Geometry.hpp"
#include "../utils/Point3D.hpp"

#include "../gui/LocationPropertiesDlg.hpp"

using namespace GenGIS;
using namespace std;

ChartView::ChartView(LocationModelPtr locationModel, LocationViewPtr locationView, ColourMapDiscretePtr colourMap)
	: m_locationModel(locationModel), m_locationView(locationView), m_colourMap(colourMap), 
		m_width(50), m_height(50), m_backgroundColour(Colour(255,255,255,0)), m_borderColour(Colour(0,0,0,1)), m_assignBorderLocationColour(false),
		m_bSizeProportionalToSeq(false), m_normNumSeq(1.0), m_borderSize(2), m_bWedgeBorders(false),  
		m_chartType(PIE_CHART_3D), m_assignToOther(5.0), m_dropLines(new DropLines()), m_minSize(30), m_maxSize(60), m_bLabelBindToChart(false), m_bQuantitative(false),
		m_bBottomBorder(true), m_bLeftBorder(false), m_bTopBorder(false), m_bRightBorder(false), m_numActiveSeq(0)
{
	// used to draw location markers
	m_chartQuadric = gluNewQuadric();

	gluQuadricDrawStyle( m_chartQuadric, GLU_FILL );
	gluQuadricNormals( m_chartQuadric, GLU_SMOOTH );

	if (!App::Inst().IsCurrentlySerializing() )
	{
		App::Inst().GetMapController()->GeoToGrid(m_locationModel->GetProjection(), m_centre);
		Line3D line = Line3D(m_centre, m_centre);
		m_dropLines->Add(VisualLinePtr(new VisualLine(Colour(), 1.0f, VisualLine::SOLID, line)));

		m_bVisible = false;
	}
}

template<class Archive>
void ChartView::serialize(Archive & ar, const unsigned int file_version)
{
	ar & boost::serialization::base_object<View>(*this);
	ar & m_locationLayerId;            // uint
	//ar & m_chartQuadric;               // GLUquadricObj*
	ar & m_width;                      // float
	ar & m_height;                     // float
	ar & m_borderSize;                 // float
	ar & m_bWedgeBorders;              // bool
	ar & m_backgroundColour;           // Colour
	ar & m_borderColour;               // Colour
	ar & m_assignBorderLocationColour; // bool
	ar & m_chartType;                  // CHART_TYPE
	ar & m_field;                      // std::wstring
	ar & m_quantitativeField;          // std::wstring
	ar & m_bQuantitative;              // bool
	ar & m_bSizeProportionalToSeq;     // bool
	ar & m_normNumSeq;                 // float
	ar & m_elementPercentage;          // std::map<std::wstring, float>
	ar & m_centre;                     // Point3D
	ar & m_dropLines;                  // DropLinesPtr
	ar & m_assignToOther;              // float
	ar & m_mousePrevPt;                // Point2D
	ar & m_minSize;                    // uint
	ar & m_maxSize;                    // uint
	ar & m_label;                      // VisualLabelPtr
	ar & m_bLabelBindToChart;          // bool
	ar & m_numActiveSeq;               // float
	ar & m_bBottomBorder;              // bool
	ar & m_bLeftBorder;                // bool
	ar & m_bTopBorder;                 // bool
	ar & m_bRightBorder;               // bool
}
template void ChartView::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void ChartView::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

bool ChartView::IsVisible() 
{ 
	return (m_bVisible && m_locationModel->GetNumSequence() > 0); 
}

Point3D ChartView::GetScreenPos() const 
{ 
	return App::Inst().GetMapController()->ProjectToScreen(m_centre); 
}

Point3D ChartView::GetLocationPos() const
{ 
	Point3D locationPt;
	App::Inst().GetMapController()->GeoToGrid(m_locationModel->GetProjection(), locationPt);
	return locationPt; 
}

void ChartView::SetScreenPos(const Point3D& pos)
{
	m_centre = App::Inst().GetMapController()->ProjectToTerrain(Point2D(pos.x, pos.y), false);
	Line3D line = Line3D(m_centre, GetLocationPos());

	VisualLinePtr dropLine = m_dropLines->GetDropLine(0);
	dropLine->SetColour(Colour());
	dropLine->SetLine(line);
}

void ChartView::SetCentre(const Point3D& pt) 
{ 
	m_centre = pt;
	Line3D line = Line3D(m_centre, GetLocationPos());

	VisualLinePtr dropLine = m_dropLines->GetDropLine(0);
	dropLine->SetColour(Colour());
	dropLine->SetLine(line);
}

Box2D ChartView::GetBoundingBox() const
{
	Point3D pt = GetScreenPos();

	Box2D box;
	box.x = pt.x - 0.5*GetAdjustedSize();
	box.y = pt.y - 0.5*GetAdjustedSize();
	box.dx = GetAdjustedSize();
	box.dy = GetAdjustedSize();

	return box;
}

std::vector<std::wstring> ChartView::GetNames() const 
{ 
	std::vector<std::wstring> names;

	std::map<std::wstring, float>::const_iterator it;
	for(it=m_elementPercentage.begin(); it != m_elementPercentage.end(); it++)
	{
		names.push_back((*it).first);
	}

	return names;
}

float ChartView::GetPercentage(const std::wstring& name) const 
{ 
	std::map<std::wstring, float>::const_iterator it = m_elementPercentage.find(name);
	if(it != m_elementPercentage.end())
		return (*it).second;
	else
		Log::Inst().Error("ChartView::GetPercentage(): Unknown field name.");

	return 0.0;
}

void ChartView::GetValues(const wstring& field, set<wstring>& values) const
{
	for(unsigned int i = 0; i < m_locationModel->GetNumSequence(); ++i)
	{
		SequenceModelPtr sequenceModel = m_locationModel->GetSequence(i);
		if(sequenceModel->IsActive())
		{
			std::wstring data = sequenceModel->GetData(field);
			values.insert(data);
		}
	}
}

bool ChartView::UpdateChart(const wstring& field, const set<wstring>& values)
{
	m_field = field;

	// initialize each chart element
	m_elementPercentage.clear();

	set<wstring>::const_iterator it;
	for(it=values.begin(); it != values.end(); it++)
		m_elementPercentage[*it] = 0.0f;

	// count number of items for each type (wedge)
	m_numActiveSeq = 0;
	for(unsigned int i = 0; i < m_locationModel->GetNumSequence(); ++i)
	{
		SequenceModelPtr sequenceModel = m_locationModel->GetSequence(i);
		if(sequenceModel->IsActive())
		{
			std::wstring data = sequenceModel->GetData(field);

			double abundance = 1.0;
			if(m_bQuantitative)
			{
				std::wstring abundanceStr = sequenceModel->GetData(m_quantitativeField);

				if(StringTools::IsDecimalNumber(abundanceStr))
					abundance = StringTools::ToDouble(abundanceStr);
				else
					return false;
			}

			m_numActiveSeq += abundance;

			std::map<std::wstring, float>::iterator wedgeIt = m_elementPercentage.find(data);
			(*wedgeIt).second += abundance;
		}
	}

	// normalize wedge counts to a percentage
	std::map<std::wstring, float>::iterator wedgeIt;
	for(wedgeIt = m_elementPercentage.begin(); wedgeIt != m_elementPercentage.end(); ++wedgeIt)
	{
		(*wedgeIt).second /= m_numActiveSeq;
	}

	return true;
}

void ChartView::Render()
{
	if(!IsVisible())
		return;

	// render dropline
	m_dropLines->Render();

	// render chart
	m_depth = CHART_LAYER;
	if(IsSelected())
		m_depth -= SELECTED_LAYER_OFFSET;

	App::Inst().GetViewport()->GetCamera()->SetOrthoCamera();

	ChartProp chartProp;
	chartProp.elementPercentage = m_elementPercentage;

	chartProp.width = GetAdjustedSize();
	chartProp.height = m_height;

	chartProp.borderSize = m_borderSize;
	chartProp.bBottomBorder = m_bBottomBorder;
	chartProp.bLeftBorder = m_bLeftBorder;
	chartProp.bTopBorder = m_bTopBorder;
	chartProp.bRightBorder = m_bRightBorder;
	chartProp.bCategoryBorders = m_bWedgeBorders;

	chartProp.backgroundColour = m_backgroundColour;
	chartProp.borderColour = m_borderColour;
	chartProp.bAssignBorderLocationColour = m_assignBorderLocationColour;
	chartProp.centre = GetScreenPos();
	chartProp.depth = m_depth;
	chartProp.cameraPitch = App::Inst().GetViewport()->GetCamera()->GetPitch();
	chartProp.cameraYaw = App::Inst().GetViewport()->GetCamera()->GetYaw();
	chartProp.bPlot3D = (m_chartType == PIE_CHART_3D || m_chartType == BAR_CHART_3D);
	chartProp.assignToOther = m_assignToOther;
	chartProp.colourMap = m_colourMap;
	chartProp.chartQuadric = m_chartQuadric;

	if(m_chartType == PIE_CHART_2D || m_chartType == PIE_CHART_3D)
	{
		PieChartView::Render(chartProp, m_locationView);

		if(IsSelected())
			PieChartView::RenderSelect(chartProp);
	}
	else
	{
		BarChartView::Render(chartProp, m_locationView);

		if(IsSelected())
			BarChartView::RenderSelect(chartProp);
	}
	
	App::Inst().GetViewport()->GetCamera()->UnsetOrthoCamera();

	// render labels
	RenderLabel();
}

void ChartView::RenderLabel()
{
	if(m_bLabelBindToChart && m_label && m_label->IsVisible() && !App::Inst().GetMouseDragging())
	{
		glDisable(GL_LIGHTING);

		// get screen coordinates for chart
		Point3D winPos = GetScreenPos();

		Box2D bb = m_label->GetBoundingBox();

		// compensate for projection
		float chartRadius = 0.5*GetAdjustedSize() + m_borderSize* App::Inst().GetResolutionFactor();
		float perspectiveChartRadius, perspectiveTextHeight;
		if(m_chartType == PIE_CHART_3D)
		{
			float s = sin(App::Inst().GetViewport()->GetCamera()->GetPitch() * DEG_TO_RAD);
			perspectiveChartRadius = chartRadius*s;
			perspectiveTextHeight = bb.Height()*s;
		}
		else
		{
			perspectiveChartRadius = chartRadius;
			perspectiveTextHeight = bb.Height();
		}

		// calculate screen position of label
		uint offset = 2 * App::Inst().GetResolutionFactor();

		Point3D screenPos;
		if(m_label->GetLabelPosition() == _T("Right"))
			screenPos = Point3D(winPos.x + chartRadius + offset, winPos.y-bb.Height()/3, m_depth);
		else if(m_label->GetLabelPosition() == _T("Left"))
			screenPos = Point3D(winPos.x-bb.Width() - chartRadius - offset, winPos.y-bb.Height()/3, m_depth);
		else if(m_label->GetLabelPosition() == _T("Top"))
			screenPos = Point3D(winPos.x-bb.Width()/2, winPos.y + perspectiveChartRadius + offset, m_depth);
		else if(m_label->GetLabelPosition() == _T("Bottom"))
			screenPos = Point3D(winPos.x-bb.Width()/2, winPos.y-perspectiveTextHeight-perspectiveChartRadius, m_depth);

		m_label->SetScreenPosition(screenPos);
		m_label->Render();

		glEnable(GL_LIGHTING);
	}
}

void ChartView::ShowPropertiesDlg()
{
	LocationLayerPtr locationLayer;
	for (uint i = 0; i < App::Inst().GetLayerTreeController()->GetNumLocationLayers(); i++)
	{
		locationLayer = App::Inst().GetLayerTreeController()->GetLocationLayer(i);
		if (locationLayer->GetId() == m_locationLayerId)
			break;
	}

	LocationPropertiesDlg* dlg = new LocationPropertiesDlg(App::Inst().GetMainWindow(), locationLayer);
	dlg->SetPage(LocationPropertiesDlg::SYMBOLOGY);
	dlg->Show();
}

bool ChartView::MouseLeftDown(const Point2D& mousePt) 
{ 
	m_mousePrevPt = mousePt;

	return IsClicked(mousePt); 
}

bool ChartView::IsClicked(const Point2D& mousePt)
{    
	if(!IsVisible())
		return false;

	if(m_chartType == PIE_CHART_2D || m_chartType == PIE_CHART_3D)
		return PieChartView::IsClicked(mousePt, GetScreenPos(), GetAdjustedSize());
	else
		return BarChartView::IsClicked(mousePt, GetScreenPos(), GetAdjustedSize());

	return false;
}

void ChartView::MouseLeftDblClick(const Point2D& mousePt) 
{ 
	ShowPropertiesDlg();
}

bool ChartView::MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu) 
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

void ChartView::MouseDragging(const Point2D& mousePt) 
{ 
	int dx = mousePt.x - m_mousePrevPt.x;
	int dy = mousePt.y - m_mousePrevPt.y;

	Point3D point = GetScreenPos();
	SetScreenPos(Point3D(point.x+dx, point.y+dy, 0));

	m_mousePrevPt = mousePt;
}

void ChartView::KeyboardEvent(wxKeyEvent& event)
{
	if(event.GetKeyCode() == WXK_DELETE)
	{
		SetVisibility(false);
	}
}

float ChartView::GetAdjustedSize() const 
{ 
	if(m_bSizeProportionalToSeq)
		return (m_minSize + (m_maxSize-m_minSize)*m_normNumSeq) * App::Inst().GetResolutionFactor();
	else
		return m_width * App::Inst().GetResolutionFactor();
}
