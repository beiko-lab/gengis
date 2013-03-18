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

#ifndef _GENGIS_BAR_CHART_VIEW_
#define _GENGIS_BAR_CHART_VIEW_

#include "../core/Precompiled.hpp"

#include "../core/View.hpp"

namespace GenGIS
{
	// Forward class declarations
	class Point3D;
	struct ChartProp;

	/**
	 * @class BarChartView
	 * @brief Bar chart.
	 */
	class BarChartView : public View
	{

	public:
		/** Render chart. */
		static void Render(const ChartProp& chartProp, const LocationViewPtr locationView);

		/** Perform additional rendering so object appears selected. */
		static void RenderSelect(const ChartProp& chartProp);

		/** Check if current position of mouse overlays with chart. */
		static bool IsClicked(const Point2D& mousePt, const Point3D& centrePt, float size);
	};
}

#endif