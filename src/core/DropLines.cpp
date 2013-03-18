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
#include "../core/DropLines.hpp"
#include "../core/Camera.hpp"
#include "../core/MapController.hpp"

#include "../core/Viewport.hpp"

using namespace GenGIS;

template<class Archive>
void DropLines::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_thickness; // float
	ar & m_colour;    // Colour
	ar & m_style;     // VisualLine::LINE_STYLE
	ar & m_dropLines; // std::vector<VisualLinePtr>
}
template void DropLines::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void DropLines::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


void DropLines::Render()
{
	if(!IsVisible())
		return;

	glDisable(GL_LIGHTING);

	foreach(VisualLinePtr line, m_dropLines)
	{
		line->SetColour(m_colour);
		line->SetLineStyle(m_style);
		line->SetSize(m_thickness);
		line->SetSelected(IsSelected());
		line->Render();
	}

	glEnable(GL_LIGHTING);			
}
