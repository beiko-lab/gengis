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
#include "../core/BarChartView.hpp"
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
#include "../utils/Point3D.hpp"

using namespace GenGIS;

void BarChartView::Render(const ChartProp& chartProp, const LocationViewPtr locationView)
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

	// plot each bar
	int stepX = (int)floor(chartProp.width / chartProp.elementPercentage.size());
	if(stepX < 1)	// ensure bars are at least 1 pixel wide
		stepX = 1;

	int chartWidth = stepX * chartProp.elementPercentage.size();
	int chartHeight = (int)floor(chartProp.height);

	int chartStartX = (int)floor(-0.5*chartWidth);
	int chartStartY = (int)floor(-0.5*chartHeight);

	// render background colour
	glColor4f(chartProp.backgroundColour.GetRed(), chartProp.backgroundColour.GetGreen(), 
							chartProp.backgroundColour.GetBlue(), chartProp.backgroundColour.GetAlpha());
	glBegin(GL_QUADS);
	glVertex3i(chartStartX, chartStartY, 0);
	glVertex3i(chartStartX, chartStartY + chartHeight, 0);
	glVertex3i(chartStartX + chartWidth, chartStartY + chartHeight, 0);
	glVertex3i(chartStartX + chartWidth, chartStartY, 0);
	glEnd();

	// draw all bars
	int barStart = chartStartX;
	float totalPercentage = 0;
	std::map<std::wstring, float>::const_iterator elementIt;
	for(elementIt = chartProp.elementPercentage.begin(); elementIt != chartProp.elementPercentage.end(); ++elementIt)
	{
		std::wstring name = (*elementIt).first;
		float percentage = (*elementIt).second;
		totalPercentage += percentage;

		int barHeight = int(floor(chartProp.height * percentage + 0.5));

		// draw bar
		Colour colour(0,0,0);
		chartProp.colourMap->GetColour(name, colour);
		glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());

		glBegin(GL_QUADS);
		glVertex3i(barStart, chartStartY, 0);
		glVertex3i(barStart, chartStartY + barHeight, 0);
		glVertex3i(barStart + stepX, chartStartY + barHeight, 0);
		glVertex3i(barStart + stepX, chartStartY, 0);
		glEnd();

		// draw bar borders
		if(chartProp.bCategoryBorders)
		{				
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

			glLineWidth(1.0f);
			glBegin(GL_LINES);
			glVertex3i(barStart, chartStartY, 0);
			glVertex3i(barStart, chartStartY + barHeight, 0);

			glVertex3i(barStart, chartStartY + barHeight, 0);
			glVertex3i(barStart + stepX, chartStartY + barHeight, 0);

			glVertex3i(barStart + stepX, chartStartY + barHeight, 0);
			glVertex3i(barStart + stepX, chartStartY, 0);
			glEnd();
		}

		barStart += stepX;
	}

	// sanity check
	if(totalPercentage < 0.99 || totalPercentage > 1.01)
		Log::Inst().Warning("(Warning) Total of all bars is not 1.0! Please contact the GenGIS development team.");

	// render axes
	if(chartProp.borderSize > 0)
	{
		Colour borderColour = chartProp.borderColour;
		if(chartProp.bAssignBorderLocationColour)
				borderColour = locationView->GetColour();

		if(chartProp.bBottomBorder)
		{
			Point3D start(chartStartX - chartProp.borderSize, chartStartY - 0.5*chartProp.borderSize, 0);
			Point3D end(chartStartX + chartWidth + chartProp.borderSize, chartStartY - 0.5*chartProp.borderSize, 0);
			VisualLine::RenderAntialiasedLine(start, end, borderColour, chartProp.borderSize, VisualLine::SOLID, 0);
		}

		if(chartProp.bLeftBorder)
		{
			Point3D start(chartStartX - 0.5*chartProp.borderSize, chartStartY - chartProp.borderSize, 0);
			Point3D end(chartStartX - 0.5*chartProp.borderSize, chartStartY + chartHeight + chartProp.borderSize, 0);
			VisualLine::RenderAntialiasedLine(start, end, borderColour, chartProp.borderSize, VisualLine::SOLID, 0);
		}

		if(chartProp.bTopBorder)
		{
			Point3D start(chartStartX - chartProp.borderSize, chartStartY + chartHeight + 0.5*chartProp.borderSize, 0);
			Point3D end(chartStartX + chartWidth + chartProp.borderSize, chartStartY + chartHeight + 0.5*chartProp.borderSize, 0);
			VisualLine::RenderAntialiasedLine(start, end, borderColour, chartProp.borderSize, VisualLine::SOLID, 0);
		}

		if(chartProp.bRightBorder)
		{
			Point3D start(chartStartX + chartWidth + 0.5*chartProp.borderSize, chartStartY - chartProp.borderSize, 0);
			Point3D end(chartStartX + chartWidth + 0.5*chartProp.borderSize, chartStartY + chartHeight + chartProp.borderSize, 0);
			VisualLine::RenderAntialiasedLine(start, end, borderColour, chartProp.borderSize, VisualLine::SOLID, 0);
		}
	}

	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
}

void BarChartView::RenderSelect(const ChartProp& chartProp)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_LINE_SMOOTH);

	glPushMatrix();
	int stepX = (int)floor(chartProp.width / chartProp.elementPercentage.size());
	if(stepX < 1)	// ensure bars are at least 1 pixel wide
		stepX = 1;

	int chartWidth = stepX * chartProp.elementPercentage.size();
	int chartHeight = (int)floor(chartProp.height);

	int chartStartX = (int)floor(-0.5*chartWidth);
	int chartStartY = (int)floor(-0.5*chartHeight);

	Point3D chartCentre = chartProp.centre;
	glTranslated(floor(chartCentre.x+0.5f), floor(chartCentre.y+0.5f), -(chartProp.depth+2));
	if(chartProp.bPlot3D)
		glRotated(90-chartProp.cameraPitch, 1.0, 0.0, 0.0);

	Colour colour = App::Inst().GetSelectionColour();
	float selectionThickness = App::Inst().GetSelectionThickness();

	Point3D start(chartStartX - chartProp.borderSize, chartStartY - chartProp.borderSize, 0);
	Point3D end(chartStartX + chartWidth + chartProp.borderSize, chartStartY - chartProp.borderSize, 0);
	VisualLine::RenderAntialiasedLine(start, end, colour, selectionThickness, VisualLine::SOLID, 0);

	start = Point3D(chartStartX - chartProp.borderSize, chartStartY, 0);
	end = Point3D(chartStartX - chartProp.borderSize, chartStartY + chartHeight, 0);
	VisualLine::RenderAntialiasedLine(start, end, colour, selectionThickness, VisualLine::SOLID, 0);

	start = Point3D(chartStartX - chartProp.borderSize, chartStartY + chartHeight + chartProp.borderSize, 0);
	end = Point3D(chartStartX + chartWidth + chartProp.borderSize, chartStartY + chartHeight + chartProp.borderSize, 0);
	VisualLine::RenderAntialiasedLine(start, end, colour, selectionThickness, VisualLine::SOLID, 0);

	start = Point3D(chartStartX + chartWidth + chartProp.borderSize, chartStartY, 0);
	end = Point3D(chartStartX + chartWidth + chartProp.borderSize, chartStartY + chartHeight, 0);
	VisualLine::RenderAntialiasedLine(start, end, colour, selectionThickness, VisualLine::SOLID, 0);

	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
}

bool BarChartView::IsClicked(const Point2D& mousePt, const Point3D& centrePt, float size)
{    
	float du = fabs( mousePt.x - centrePt.x );
	float dv = fabs( mousePt.y - centrePt.y );

	return du < 0.5*size && dv < 0.5*size;
}