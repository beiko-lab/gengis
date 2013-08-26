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
#include "../core/LayoutEllipse.hpp"
#include "../core/Camera.hpp"
#include "../core/MapController.hpp"
#include "../core/OrthoLayers.hpp"
#include "../core/PopupMenu.hpp"
#include "../core/Viewport.hpp"
#include "../core/VisualMarker.hpp"
#include "../core/LayoutObjectProperties.hpp"

#include "../gui/LayoutObjectPropertiesDlg.hpp"

#include "../utils/ErrorGL.hpp"
#include "../utils/Geometry.hpp"
#include "../utils/Log.hpp"

using namespace GenGIS;

typedef struct sANGLE_SORTER
{
	sANGLE_SORTER(int _index, double _angle): index(_index), angle(_angle) {}

	int index;
	double angle;
} AngleSorter;

bool AngleSortPredicate(AngleSorter elem1, AngleSorter elem2)
{
	 return elem1.angle < elem2.angle;
}

LayoutEllipse::LayoutEllipse(): m_stage(START), m_activePt(NONE)
{
	Point3D pt(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	m_primaryLayoutLine.SetLine(Line3D(pt, pt));
	m_orthoLayoutLine.SetLine(Line3D(pt, pt));
}

template<class Archive>
void LayoutEllipse::serialize(Archive & ar, const unsigned int file_version)
{
	ar & boost::serialization::base_object<View>(*this);
	ar & m_points;               // Point3D (array)
	ar & m_primaryLayoutLine;    // VisualLine
	ar & m_orthoLayoutLine;      // VisualLine
	ar & m_modPrimaryLayoutLine; // VisualLine
	ar & m_modOrthoLayoutLine;   // VisualLine
	ar & m_stage;                // STAGE
	ar & m_activePt;             // ACTIVE_POINT
	ar & m_xRadius;              // double
	ar & m_zRadius;              // double
	ar & m_rotation;             // double
	ar & m_centre;               // Point3D
}
template void LayoutEllipse::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void LayoutEllipse::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void LayoutEllipse::Render()
{
	if(!IsVisible())
		return;

	error::ErrorGL::Check();

	glDisable(GL_LIGHTING);		
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		

	// set visual properties of control points
	Colour colourCP = App::Inst().GetLayoutObjProp()->GetControlPointColour();
	float sizeCP = App::Inst().GetLayoutObjProp()->GetControlPointSize();

	VisualMarker primaryStartPt(colourCP, sizeCP, VisualMarker::SQUARE, m_primaryLayoutLine.GetStartPt());
	primaryStartPt.SetSelected(IsSelected() && m_stage == COMPLETE && m_activePt == NONE);
	primaryStartPt.SetRenderingStyle(VisualMarker::ALWAYS_VISIBLE);
	primaryStartPt.SetRotation(0.0);
	primaryStartPt.SetDepth(CONTROL_PT_LAYER);

	VisualMarker primaryEndPt(colourCP, sizeCP, VisualMarker::SQUARE, m_primaryLayoutLine.GetEndPt());
	primaryEndPt.SetSelected(IsSelected() && m_stage == COMPLETE && m_activePt == NONE);
	primaryEndPt.SetRenderingStyle(VisualMarker::ALWAYS_VISIBLE);
	primaryEndPt.SetRotation(0.0);
	primaryEndPt.SetDepth(CONTROL_PT_LAYER);

	VisualMarker orthoStartPt(colourCP, sizeCP, VisualMarker::SQUARE, m_orthoLayoutLine.GetStartPt());
	orthoStartPt.SetSelected(IsSelected() && m_stage == COMPLETE && m_activePt == NONE);
	orthoStartPt.SetRenderingStyle(VisualMarker::ALWAYS_VISIBLE);
	orthoStartPt.SetRotation(0.0);
	orthoStartPt.SetDepth(CONTROL_PT_LAYER);

	VisualMarker orthoEndPt(colourCP, sizeCP, VisualMarker::SQUARE, m_orthoLayoutLine.GetEndPt());
	orthoEndPt.SetSelected(IsSelected() && m_stage == COMPLETE && m_activePt == NONE);
	orthoEndPt.SetRenderingStyle(VisualMarker::ALWAYS_VISIBLE);
	orthoEndPt.SetRotation(0.0);
	orthoEndPt.SetDepth(CONTROL_PT_LAYER);

	// set visual properties of primary layout line
	m_primaryLayoutLine.SetColour(App::Inst().GetLayoutObjProp()->GetLayoutLineColour());
	m_primaryLayoutLine.SetSize(App::Inst().GetLayoutObjProp()->GetLayoutLineThickness());
	m_primaryLayoutLine.SetLineStyle(App::Inst().GetLayoutObjProp()->GetLayoutLineStyle());
	m_primaryLayoutLine.SetSelected(IsSelected() && m_stage == COMPLETE && m_activePt == NONE);

	// set visual properties of othogonal layout line
	m_orthoLayoutLine.SetColour(App::Inst().GetLayoutObjProp()->GetLayoutLineColour());
	m_orthoLayoutLine.SetSize(App::Inst().GetLayoutObjProp()->GetLayoutLineThickness());
	m_orthoLayoutLine.SetLineStyle(App::Inst().GetLayoutObjProp()->GetLayoutLineStyle());
	m_orthoLayoutLine.SetSelected(IsSelected() && m_stage == COMPLETE && m_activePt == NONE);

	if(m_activePt == NONE)
	{
		// ellipse is either in the process of being constructed or completely constructed
		// and in a static state
		if(m_stage == START)
		{
			primaryStartPt.SetPosition(App::Inst().GetMouseTerrainPlanePos());
			primaryStartPt.Render();
		}
		else if(m_stage == PRIMARY_LINE)
		{			
			primaryStartPt.Render();
			primaryEndPt.SetPosition(App::Inst().GetMouseTerrainPlanePos());
			primaryEndPt.Render();

			m_primaryLayoutLine.SetEndPt(App::Inst().GetMouseTerrainPlanePos());
			m_primaryLayoutLine.Render();

			float dist = Geometry::Distance(m_primaryLayoutLine.GetStartPt(), App::Inst().GetMouseTerrainPlanePos());
			Point3D midPt = Geometry::MidPoint(Line3D(m_primaryLayoutLine.GetStartPt(), App::Inst().GetMouseTerrainPlanePos()));
			float rotation = Geometry::AngleToX(Line3D(m_primaryLayoutLine.GetStartPt(), App::Inst().GetMouseTerrainPlanePos()));
			Draw(midPt, dist*0.5, dist*0.25, -rotation);
		}
		else if(m_stage == ORTHO_LINE)
		{
			Point3D endPt = OrthoLineEndPt(m_primaryLayoutLine.GetLine(), m_orthoLayoutLine.GetStartPt());

			primaryStartPt.Render();
			primaryEndPt.Render();
			orthoStartPt.Render();
			orthoEndPt.SetPosition(endPt);
			orthoEndPt.Render();

			m_primaryLayoutLine.Render();			
			m_orthoLayoutLine.SetEndPt(endPt);
			m_orthoLayoutLine.Render();

			float dist1 = Geometry::LineLength(m_primaryLayoutLine.GetLine());
			float dist2 = Geometry::Distance(m_orthoLayoutLine.GetStartPt(), endPt);
			float rotation = Geometry::AngleToX(m_primaryLayoutLine.GetLine());
			Draw(m_orthoLayoutLine.GetStartPt(), dist1*0.5, dist2, -rotation);
		}
		else if(m_stage == COMPLETE)
		{
			primaryStartPt.Render();
			primaryEndPt.Render();
			orthoStartPt.Render();
			orthoEndPt.Render();

			m_primaryLayoutLine.Render();
			m_orthoLayoutLine.Render();
		}
	}
	else
	{
		// ellipse is being interactively modified so render the tempory modified state
		primaryStartPt.SetPosition(m_modPrimaryLayoutLine.GetStartPt());
		primaryStartPt.Render();
		primaryEndPt.SetPosition(m_modPrimaryLayoutLine.GetEndPt());
		primaryEndPt.Render();

		orthoStartPt.SetPosition(m_modOrthoLayoutLine.GetStartPt());
		orthoStartPt.Render();
		orthoEndPt.SetPosition(m_modOrthoLayoutLine.GetEndPt());
		orthoEndPt.Render();

		m_modPrimaryLayoutLine.Render();
		m_modOrthoLayoutLine.Render();
	}

	// render ellipse
	if(m_stage != START && App::Inst().GetLayoutObjProp()->GetObjectLineStyle() != VisualLine::HIDDEN)
	{	
		Colour colour = App::Inst().GetLayoutObjProp()->GetObjectLineColour();
		glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());

		glLineWidth(App::Inst().GetLayoutObjProp()->GetObjectLineThickness());

		glBegin(GL_LINE_LOOP);
			for(uint i = 0; i < NUM_ELLIPSE_PTS; ++i)
			{
				glVertex3f(m_points[i].x, m_points[i].y, m_points[i].z);
			}
		glEnd();
	}

	glEnable(GL_LIGHTING);

	error::ErrorGL::Check();
}

void LayoutEllipse::ShowPropertiesDlg()
{
	LayoutObjectPropertiesDlg* dlg = new LayoutObjectPropertiesDlg(App::Inst().GetMainWindow());
	dlg->Show();
}

Point3D LayoutEllipse::OrthoLineEndPt(const Line3D& primaryLayoutLine, const Point3D& midPt)
{
	Point3D closestPt;
	Geometry::ClosestPointToLine(primaryLayoutLine, App::Inst().GetMouseTerrainPlanePos(), closestPt);
	Point3D orthoVec = App::Inst().GetMouseTerrainPlanePos() - closestPt;
	Point3D orthoPt = midPt + orthoVec;

	return orthoPt;
}

void LayoutEllipse::Draw(const Point3D& centre, float xRadius, float zRadius, float rotation)
{
	// save parameters defining ellipse
	m_xRadius = xRadius;
	m_zRadius = zRadius;
	m_rotation = rotation;
	m_centre = centre;

	Point3D pt;
	pt.y = centre.y;

	// draw ellipse at (0,0) with axis along the x and z axes
	for(int i=0; i < NUM_ELLIPSE_PTS; i++)
	{	
		float rad = i*DEG_TO_RAD;
		pt.x = cos(rad)*xRadius;
		pt.z = sin(rad)*zRadius;
		m_points[i] = pt;
	}

	// rotate ellipse to desired orientation
	float cosAngle = cos(rotation);
	float sinAngle = sin(rotation);
	for(uint i = 0; i < NUM_ELLIPSE_PTS; ++i)
	{
		float originalX = m_points[i].x;
		m_points[i].x = m_points[i].x*cosAngle - m_points[i].z*sinAngle;
		m_points[i].z = -(originalX*sinAngle + m_points[i].z*cosAngle);
	}

	// translate ellipse to desired position
	for(uint i = 0; i < NUM_ELLIPSE_PTS; ++i)
	{
		m_points[i].x = m_points[i].x + centre.x;
		m_points[i].z = m_points[i].z + centre.z;
	}
}

bool LayoutEllipse::InEllipse(const Point3D& canonicalPt) const
{
	double xTerm = (canonicalPt.x*canonicalPt.x) / (GetRadiusX()*GetRadiusX());
	double zTerm = (canonicalPt.z*canonicalPt.z) / (GetRadiusZ()*GetRadiusZ());
	
	return (xTerm + zTerm) < 1;

} 

bool LayoutEllipse::MouseLeftDown(const Point2D& mousePt)
{
	if(App::Inst().GetMode() == App::LAYOUT_ELLIPSE)
	{
		DrawEllipseInteraction();
		return true;
	}

	return ModifyEllipseInteraction(mousePt);
}

void LayoutEllipse::MouseLeftUp(const Point2D& mousePt)
{
	if(m_activePt != NONE)
	{
		m_activePt = NONE;

		m_primaryLayoutLine = m_modPrimaryLayoutLine;
		m_orthoLayoutLine = m_modOrthoLayoutLine;
	}
}

void LayoutEllipse::MouseDragging(const Point2D& mousePt)
{
	if(App::Inst().GetMode() == App::LAYOUT_ELLIPSE
			&& (m_stage == PRIMARY_LINE || m_stage == ORTHO_LINE))
	{	
		// need to update layout lines to reflect current mouse position
		App::Inst().GetViewport()->Refresh(false);
		return;
	}

	CalculateModifiedEllipse();
	App::Inst().GetViewport()->Refresh(false);
}

bool LayoutEllipse::MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu)
{
	if(!IsVisible())
		return false;

	if(IsPointClicked(mousePt, m_primaryLayoutLine.GetStartPt()) 
						|| IsPointClicked(mousePt, m_primaryLayoutLine.GetEndPt())
						|| IsPointClicked(mousePt, m_orthoLayoutLine.GetStartPt())
						|| IsPointClicked(mousePt, m_orthoLayoutLine.GetEndPt()))
	{
		if(popupMenu.GetMenuItems().size() == 0)
		{
			popupMenu.Append(ID_POPUP_MNU_PROPERTIES, wxT("Properties"));
		}

		return true;
	}

	return false;
}

void LayoutEllipse::MouseLeftDblClick(const Point2D& mousePt)
{
	ShowPropertiesDlg();
}

bool LayoutEllipse::MouseMove(const Point2D& mousePt)
{
	if(App::Inst().GetMode() == App::LAYOUT_ELLIPSE)
	{
		if(m_stage != COMPLETE)
			return true;
	}
	else if(m_activePt != NONE)
	{
		return true;
	}

	return false;
}

void LayoutEllipse::DrawEllipseInteraction()
{
	if(m_stage == START)
	{
		m_primaryLayoutLine.SetStartPt(App::Inst().GetMouseTerrainPlanePos());
		m_orthoLayoutLine.SetStartPt(App::Inst().GetMouseTerrainPlanePos());
		m_stage = PRIMARY_LINE;
	}
	else if(m_stage == PRIMARY_LINE)
	{
		m_primaryLayoutLine.SetEndPt(App::Inst().GetMouseTerrainPlanePos());
		m_orthoLayoutLine.SetStartPt(Geometry::MidPoint(Line3D(m_primaryLayoutLine.GetStartPt(), m_primaryLayoutLine.GetEndPt())));
		m_stage = ORTHO_LINE;
	}
	else if(m_stage == ORTHO_LINE)
	{
		Point3D pt = OrthoLineEndPt(m_primaryLayoutLine.GetLine(), m_orthoLayoutLine.GetStartPt());
		m_orthoLayoutLine.SetEndPt(pt);
		m_stage = COMPLETE;
		App::Inst().GetViewport()->Refresh(false);
	}
}

bool LayoutEllipse::IsPointClicked(const Point2D& mousePt, const Point3D& controlPt)
{
	float size = App::Inst().GetLayoutObjProp()->GetControlPointSize();
	Point3D pt = App::Inst().GetMapController()->ProjectToScreen(controlPt);
	return (abs(pt.x - mousePt.x) < 5 && abs(pt.y - mousePt.y) < 5);
}

bool LayoutEllipse::ModifyEllipseInteraction(const Point2D& mousePt)
{
	// primary layout line start point
	if(IsPointClicked(mousePt, m_primaryLayoutLine.GetStartPt()))
	{
		m_modPrimaryLayoutLine = m_primaryLayoutLine;
		m_modOrthoLayoutLine = m_orthoLayoutLine;
		m_activePt = PRIMARY_START;
		return true;
	}

	// primary layout line end point
	if(IsPointClicked(mousePt, m_primaryLayoutLine.GetEndPt()))
	{
		m_modPrimaryLayoutLine = m_primaryLayoutLine;
		m_modOrthoLayoutLine = m_orthoLayoutLine;
		m_activePt = PRIMARY_END;
		return true;
	}

	// centre point
	if(IsPointClicked(mousePt, m_orthoLayoutLine.GetStartPt()))
	{
		m_modPrimaryLayoutLine = m_primaryLayoutLine;
		m_modOrthoLayoutLine = m_orthoLayoutLine;
		m_activePt = CENTRE_PT;
		return true;
	}
		
	// orthogonal layout line end point		
	if(IsPointClicked(mousePt, m_orthoLayoutLine.GetEndPt()))
	{
		m_modPrimaryLayoutLine = m_primaryLayoutLine;
		m_modOrthoLayoutLine = m_orthoLayoutLine;
		m_activePt = ORTHO_END;
		return true;
	}

	return false;
}

void LayoutEllipse::CalculateModifiedEllipse()
{
	if(m_activePt == PRIMARY_START || m_activePt == PRIMARY_END)
	{
		// moving the primary start point allows the ellipse to be
		// scaled along a single axis and rotated
		Point3D mousePos = App::Inst().GetMouseTerrainPlanePos();
		Point3D startPos;
		if(m_activePt == PRIMARY_START)
			startPos = m_primaryLayoutLine.GetStartPt();
		else if(m_activePt == PRIMARY_END)
			startPos = m_primaryLayoutLine.GetEndPt();

		float dx = mousePos.x - startPos.x;
		float dy = mousePos.y - startPos.y;
		float dz = mousePos.z - startPos.z;
		Point3D offsetPt(dx, dy, dz);

		if(m_activePt == PRIMARY_START)
		{
			m_modPrimaryLayoutLine.SetStartPt(m_primaryLayoutLine.GetStartPt().Offset(offsetPt));
			m_modPrimaryLayoutLine.SetEndPt(m_primaryLayoutLine.GetEndPt());
		}
		else if(m_activePt == PRIMARY_END)
		{
			m_modPrimaryLayoutLine.SetStartPt(m_primaryLayoutLine.GetStartPt());
			m_modPrimaryLayoutLine.SetEndPt(m_primaryLayoutLine.GetEndPt().Offset(offsetPt));
		}

		m_modOrthoLayoutLine.SetStartPt(Geometry::MidPoint(Line3D(m_modPrimaryLayoutLine.GetStartPt(), m_modPrimaryLayoutLine.GetEndPt())));

		// calculate normal vector to modified primary layout line 
		Point3D normDir = Geometry::NormalToLine(m_modPrimaryLayoutLine.GetLine());

		// check if orthonal layout line was original drawn on the left or right
		Point3D originalNormDir = Geometry::NormalToLine(m_primaryLayoutLine.GetLine());
		float angle = originalNormDir.Angle(m_orthoLayoutLine.GetEndPt() - m_orthoLayoutLine.GetStartPt());

		if(angle > PI_BY_2)
		{
			normDir.x = -normDir.x;
			normDir.y = -normDir.y;
			normDir.z = -normDir.z;
		}

		// calculate length of line
		float length = Geometry::LineLength(m_orthoLayoutLine.GetLine());

		offsetPt = Point3D(normDir.x*length, normDir.y*length, normDir.z*length);
		m_modOrthoLayoutLine.SetEndPt(m_modOrthoLayoutLine.GetStartPt().Offset(offsetPt));
	}
	else if(m_activePt == CENTRE_PT)
	{
		// moving the centre control point translates the entire ellipse
		Point3D mousePos = App::Inst().GetMouseTerrainPlanePos();
		Point3D centrePos = m_orthoLayoutLine.GetStartPt();

		float dx = mousePos.x - centrePos.x;
		float dy = mousePos.y - centrePos.y;
		float dz = mousePos.z - centrePos.z;

		m_modPrimaryLayoutLine.SetLine(m_primaryLayoutLine.GetLine().Offset(Point3D(dx, dy, dz)));
		m_modOrthoLayoutLine.SetLine(m_orthoLayoutLine.GetLine().Offset(Point3D(dx,dy,dz)));
	}
	else if(m_activePt == ORTHO_END)
	{
		// moving the end point of the orthogonal layout line simply adjusts a single axis
		m_modPrimaryLayoutLine = m_primaryLayoutLine;
		m_modOrthoLayoutLine.SetStartPt(m_orthoLayoutLine.GetStartPt());
		m_modOrthoLayoutLine.SetEndPt(OrthoLineEndPt(m_modPrimaryLayoutLine.GetLine(), m_modOrthoLayoutLine.GetStartPt()));
	}

	float dist1 = Geometry::LineLength(m_modPrimaryLayoutLine.GetLine());
	float dist2 = Geometry::LineLength(m_modOrthoLayoutLine.GetLine());
	float rotation = Geometry::AngleToX(m_modPrimaryLayoutLine.GetLine());
	Draw(m_modOrthoLayoutLine.GetStartPt(), dist1*0.5, dist2, -rotation);
}

void LayoutEllipse::KeyboardEvent(wxKeyEvent& event)
{
	if(event.GetKeyCode() == WXK_ESCAPE)
	{
		if(m_stage != COMPLETE)
		{
			// cancel construction of the layout ellipse
			m_stage = START;
		}
		else if(m_stage == COMPLETE && m_activePt != NONE)
		{
			// reset ellipse to position before interaction started
			m_activePt = NONE;

			float dist1 = Geometry::LineLength(m_primaryLayoutLine.GetLine());
			float dist2 = Geometry::LineLength(m_orthoLayoutLine.GetLine());
			float rotation = Geometry::AngleToX(m_primaryLayoutLine.GetLine());
			Draw(m_orthoLayoutLine.GetStartPt(), dist1*0.5, dist2, -rotation);
		}
	}
	else if(event.GetKeyCode() == WXK_DELETE)
	{
		// delete the ellipse
		m_activePt = NONE;
		m_stage = START;
	}
}

Point3D LayoutEllipse::ProjectPt(const Point3D& pt) const
{
	// translate point to origin
	Point3D canonicalPt = CanonicalPos(pt);

	// find closest point to ellipse
	int iterations;
	double closestX, closestZ;
	Geometry::DistancePointEllipseXZ(canonicalPt, GetRadiusX(), GetRadiusZ(),
																	1e-08, 32, iterations, closestX, closestZ);		
	if(iterations == -1)
	{
		Log::Inst().Error("(Error) ChartSetView::LayoutEllipseClosestPt(): failed to project point to ellipse.");
	}

	return EllipseRefFramePos(Point3D(closestX, 0, closestZ));
}

void LayoutEllipse::ProjectPts(const std::vector<Point3D>& pts, std::vector<Point3D>& projPts)
{
	foreach(const Point3D pt, pts)
	{
		Point3D projPt = ProjectPt(pt);
		projPts.push_back(projPt);
	}
}

void LayoutEllipse::LayoutPtsEvenly(const std::vector<Point3D>& pts, std::vector<Point3D>& laidOutPts)
{
	// project all points onto the ellipse and find order of points
	// as one goes around the ellipse CW starting from the x-axis of the ellipse
	std::vector<AngleSorter> angleSorter;
	for(uint i = 0; i < pts.size(); ++i)
	{
		Point3D projPt = ProjectPt(pts.at(i));
		Point3D xAxis(pts.at(i).x+1, 0, pts.at(i).z);
		double angle = Geometry::SignedAngleXZ(GetCentrePt(), xAxis, projPt);
		if(angle < 0)
			angle = PI2 + angle;
		angleSorter.push_back(AngleSorter(i, angle));
	}

	std::sort(angleSorter.begin(), angleSorter.end(), AngleSortPredicate);

	// evenly space points around ellipse
	double dAngle = PI2 / pts.size();
	double angle = 0;
	laidOutPts.resize(pts.size());
	foreach(AngleSorter sorter, angleSorter)
	{
		// get a distance that is longer than either radii of the ellipse
		double dist = GetRadiusX() + GetRadiusZ();

		// find intersection between line from (0,0) to (xx,zz) and the ellipse 
		double xx = cos(angle)*dist;
		double zz = sin(angle)*dist;
    Point3D point(xx, 0, zz);
		Point3D canonicalPt = Geometry::EllipseOriginLineIntersectXZ(GetRadiusX(), GetRadiusZ(), point);
		Point3D pt = EllipseRefFramePos(canonicalPt);

		laidOutPts.at(sorter.index) = pt;

		angle += dAngle;
	}
}

typedef struct sQUADRANT_LAYOUT_INFO
{
	/** Constructor. */
	sQUADRANT_LAYOUT_INFO(uint _index, Point3D _canonicalPos, bool _bInEllipse)
		: index(_index), canonicalPos(_canonicalPos), bInEllipse(_bInEllipse) {}

	uint index;
	Point3D canonicalPos;
	bool bInEllipse;
} QuadrantLayoutInfo;

void LayoutEllipse::LayoutPtsByQuadrant(const std::vector<Point3D>& pts, std::vector<Point3D>& laidOutPts,
																						bool bSortOutputPts)
{
	// find quadrant of each chart
	std::vector<QuadrantLayoutInfo> quadInfo[4];
	for(uint i = 0; i < pts.size(); ++i)
	{
		Point3D canonicalPt = CanonicalPos(pts.at(i));
		if(canonicalPt.x >= 0 && canonicalPt.z >= 0)
		{
			quadInfo[0].push_back(QuadrantLayoutInfo(i, canonicalPt, InEllipse(canonicalPt)));
		}			
		else if(canonicalPt.x < 0 && canonicalPt.z >= 0)
		{
			quadInfo[1].push_back(QuadrantLayoutInfo(i, canonicalPt, InEllipse(canonicalPt)));
		}
		else if(canonicalPt.x < 0 && canonicalPt.z < 0)
		{
			quadInfo[2].push_back(QuadrantLayoutInfo(i, canonicalPt, InEllipse(canonicalPt)));
		}
		else
		{
			quadInfo[3].push_back(QuadrantLayoutInfo(i, canonicalPt, InEllipse(canonicalPt)));
		}
	}

	// space charts in each quandrant evenly around that quadrant of the ellipse
	if(!bSortOutputPts)
		laidOutPts.resize(pts.size());
	for(uint quad = 0; quad < 4; ++quad)
	{
		uint numPts = quadInfo[quad].size();
		double step = PI_BY_2 / (numPts+1);
		double angle = PI_BY_2 * quad + step;
		for(uint pos = 0; pos < numPts; ++pos)
		{
			Point3D ellipsePos;
			ellipsePos.x = cos(angle)*GetRadiusX();
			ellipsePos.z = sin(angle)*GetRadiusZ();
			ellipsePos.y = 0;

			Point3D nextEllipsePos;
			nextEllipsePos.x = cos(angle+step)*GetRadiusX();
			nextEllipsePos.z = sin(angle+step)*GetRadiusZ();
			nextEllipsePos.y = 0;

			uint index = 0;
			for(uint i = 0; i < quadInfo[quad].size(); ++i)
			{
				bool bPlaceChart = true;
				for(uint j = 0; j < quadInfo[quad].size(); ++j)
				{
					if(i == j)
						continue;

					// determine if geographic position of the j_th chart is to the left/right of the line going from the geographic position of the i_th
					// chart to the current position on the ellipse
					Point3D vec1 = ellipsePos - quadInfo[quad].at(i).canonicalPos;
					Point3D vec2 = quadInfo[quad].at(j).canonicalPos - quadInfo[quad].at(i).canonicalPos;
					Point3D test1 = vec1.Cross(vec2);
		
					// determine if geographic position of the j_th chart is to the left/right of the line going from the geographic position of the i_th
					// chart to the next position on the ellipse
					vec1 = nextEllipsePos - quadInfo[quad].at(i).canonicalPos;
					Point3D test2 = vec1.Cross(vec2);
		
					if(quadInfo[quad].at(i).bInEllipse && test1.y > 0 && test2.y > 0) 
					{
						// the i_th chart should not be assigned to this position along the ellipse
						bPlaceChart = false;
						break;
					}
					else if(!quadInfo[quad].at(i).bInEllipse && test1.y < 0 && test2.y < 0)
					{
						// the i_th chart should not be assigned to this position along the ellipse
						bPlaceChart = false;
						break;
					}
				}

				if(bPlaceChart)
				{
					index = i;
					break;
				}
			}

			if(bSortOutputPts)
				laidOutPts.push_back(EllipseRefFramePos(ellipsePos));
			else
				laidOutPts.at(quadInfo[quad].at(index).index) = EllipseRefFramePos(ellipsePos);

			quadInfo[quad].erase(quadInfo[quad].begin() + index);
			
			angle += step;
		}
	}
}

Point3D LayoutEllipse::CanonicalPos(const Point3D& pt) const
{
	// translate point to origin
	Point3D canonicalPt = pt-GetCentrePt();

	// rotate point into canonical reference frame
	float cosAngle = cos(GetRotation());
	float sinAngle = sin(GetRotation());
	float originalX = canonicalPt.x;
	canonicalPt.x = canonicalPt.x*cosAngle - canonicalPt.z*sinAngle;
	canonicalPt.z = originalX*sinAngle + canonicalPt.z*cosAngle;

	return canonicalPt;
}

Point3D LayoutEllipse::EllipseRefFramePos(const Point3D& canonicalPt) const
{
	Point3D newPt;

	// rotate closest point 
	double cosAngle = cos(-GetRotation());
	double sinAngle = sin(-GetRotation());
	newPt.x = canonicalPt.x*cosAngle - canonicalPt.z*sinAngle;
	newPt.z = canonicalPt.x*sinAngle + canonicalPt.z*cosAngle;
	
	// translate closest point
	newPt += GetCentrePt();

	return newPt;
}
