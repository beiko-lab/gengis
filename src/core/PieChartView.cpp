//=======================================================================
// Author: Donovan Parks
//
// Copyright 2010 Donovan Parks
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
#include "../core/PieChartView.hpp"
#include "../core/Camera.hpp"
#include "../core/DropLines.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/SequenceModel.hpp"
#include "../core/MapController.hpp"
#include "../core/OrthoLayers.hpp"
#include "../core/Viewport.hpp"
#include "../core/LocationModel.hpp"
#include "../core/LocationView.hpp"

#include "../utils/ColourMapDiscrete.hpp"
#include "../utils/Log.hpp"
#include "../utils/ErrorGL.hpp"
#include "../utils/Font.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/Geometry.hpp"

using namespace GenGIS;

void PieChartView::Render(const ChartProp& chartProp, const LocationViewPtr locationView)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDisable(GL_LIGHTING);
	glDisable(GL_LINE_SMOOTH);

	// render wedges
	glPushMatrix();

	// get screen coordinates of centre of chart
	Point3D chartCentre = chartProp.centre;

	// move chart to desired position
	glTranslated(floor(chartCentre.x+0.5f), floor(chartCentre.y+0.5f), -chartProp.depth);
	if(chartProp.bPlot3D)
		glRotated(90-chartProp.cameraPitch, 1.0, 0.0, 0.0);

	std::map<std::wstring, float>::const_iterator wedgeIt;
	float degreeStart = -chartProp.cameraYaw;
	float otherTaxaDegrees = 0;
	for(wedgeIt = chartProp.elementPercentage.begin(); wedgeIt != chartProp.elementPercentage.end(); ++wedgeIt)
	{
		std::wstring name = (*wedgeIt).first;
		float percentage = (*wedgeIt).second;

		if(percentage == 0.0f)
			continue;

		float wedgeDegrees = percentage * 360;
		if(percentage*100 <= chartProp.assignToOther || name == _T("Other") || name == _T("other"))
		{
			otherTaxaDegrees += wedgeDegrees;
		}
		else
		{
			// draw wedge
			Colour colour(0,0,0);
			chartProp.colourMap->GetColour(name, colour);
			glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());
			gluPartialDisk(chartProp.chartQuadric, 0, 0.5*chartProp.width, wedgeDegrees, 1, degreeStart, wedgeDegrees);

			// draw wedge border
			if(chartProp.bCategoryBorders)
			{
				float x = (0.5*chartProp.width)*sin(PI_BY_2 - (degreeStart-90)*DEG_TO_RAD);
				float y = -(0.5*chartProp.width)*sin((degreeStart-90)*DEG_TO_RAD);

				glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

				glLineWidth(1.0f);
				glBegin(GL_LINES);
				glVertex3f(0, 0, 0.01f);
				glVertex3f(x, y, 0.01f);
				glEnd();
			}

			degreeStart += wedgeDegrees;
		}
	}

	// render the other taxa wedge
	if(otherTaxaDegrees > 0)
	{
		// draw wedge
		Colour colour;
		chartProp.colourMap->GetColour(_T("Other"), colour);
		glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());
		gluPartialDisk(chartProp.chartQuadric, 0, 0.5*chartProp.width, otherTaxaDegrees, 1, degreeStart, otherTaxaDegrees);

		// draw wedge border
		if(chartProp.bCategoryBorders)
		{
			float x = (0.5*chartProp.width)*sin(PI_BY_2 - (degreeStart-90)*DEG_TO_RAD);
			float y = -(0.5*chartProp.width)*sin((degreeStart-90)*DEG_TO_RAD);

			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

			glLineWidth(1.0f);
			glBegin(GL_LINES);
			glVertex3f(0, 0, 0.01f);
			glVertex3f(x, y, 0.01f);
			glEnd();
		}
	}

	glPopMatrix();

	// render outer circle edge
	uint borderSize = chartProp.borderSize * App::Inst().GetResolutionFactor();
	if(borderSize != 0)
	{
		glPushMatrix();

		// place behind chart
		glTranslated(floor(chartCentre.x+0.5f), floor(chartCentre.y+0.5f), -(chartProp.depth+5));
		if(chartProp.bPlot3D)
			glRotated(90-chartProp.cameraPitch, 1.0, 0.0, 0.0);

		// get border colour
		Colour borderColour = chartProp.borderColour;
		if(chartProp.bAssignBorderLocationColour)
				borderColour = locationView->GetColour();

		// render border
		if(borderSize > 3 * App::Inst().GetResolutionFactor())
		{
			glColor4f(0, 0, 0, 1);
			gluDisk(chartProp.chartQuadric, 0, 0.5*chartProp.width + borderSize + 1, 180, 1);
		}

		glColor4f(borderColour.GetRed(), borderColour.GetGreen(), borderColour.GetBlue(), 1);
		gluDisk(chartProp.chartQuadric, 0, 0.5*chartProp.width + borderSize, 180, 1);

		if(borderSize > 3 * App::Inst().GetResolutionFactor())
		{
			glColor4f(0, 0, 0, 1);
			gluDisk(chartProp.chartQuadric, 0, 0.5*chartProp.width + 1, 180, 1);
		}

		glPopMatrix();
	}

	glEnable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
}

void PieChartView::RenderSelect(const ChartProp& chartProp)
{
	glDisable(GL_LIGHTING);

	glPushMatrix();
	float selectionThickness = App::Inst().GetSelectionThickness();
	Colour colour = App::Inst().GetSelectionColour();
	Point3D chartCentre = chartProp.centre;

	glTranslated(floor(chartCentre.x+0.5f), floor(chartCentre.y+0.5f), -(chartProp.depth+2));
	if(chartProp.bPlot3D)
		glRotated(90-chartProp.cameraPitch, 1.0, 0.0, 0.0);

	glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());
	gluDisk(chartProp.chartQuadric, 0, 0.5*chartProp.width + chartProp.borderSize+selectionThickness, 180, 1);
	glPopMatrix();

	glEnable(GL_LIGHTING);
}

bool PieChartView::IsClicked(const Point2D& mousePt, const Point3D& centrePt, float size)
{    
	float du = fabs( mousePt.x - centrePt.x );
	float dv = fabs( mousePt.y - centrePt.y );

	float distance = du * du + dv * dv;
	// Note: no need to take sqrt() since
	// it is a monotonic function

	float radius = 0.5*size;
	return (distance < radius*radius);
}