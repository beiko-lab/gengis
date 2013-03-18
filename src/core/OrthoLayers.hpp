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

#ifndef _GENGIS_ORTHO_LAYERS_
#define _GENGIS_ORTHO_LAYERS_

#include "../core/Precompiled.hpp"

namespace GenGIS
{
	// Define layer (depth) of visual elements that are rendered with orthographic camera.
	// All values should be between (0, 1000) as this is the values for the near and far clipping planes.
	const int WIDGETS_LAYER           = 1;

	const int COMPASS_LAYER           = 500;
	// Under Windows XP/OS X a value of 1 was sufficent.
	// However, under Windows VISTA/7 visual artifacts occurs if the value is less than 500

	const int CONTROL_PT_LAYER        = 550;
	const int LABEL_LAYER             = 600;
	const int MARKER_LAYER            = 650;
	const int CHART_LAYER             = 700;
	const int GEO_LOCATION_LAYER      = 750;
	const int TREE_LAYER              = 800;
	const int LAYOUT_PRIMATIVES_LAYER = 850;

	const int SELECTED_LAYER_OFFSET   = 25;
}

#endif
