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

#include "../core/VisualLines.hpp"

#include "../core/App.hpp"

using namespace GenGIS;

template<class Archive>
void VisualLines::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_colour;    // Colour
	ar & m_lineStyle; // VisualLine::LINE_STYLE
	ar & m_thickness; // float
	ar & m_visible;   // bool
	ar & m_lines;     // std::vector<Line3D>
}
template void VisualLines::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void VisualLines::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void VisualLines::Render()
{

}

float VisualLines::GetThickness() const 
{ 
	return m_thickness * App::Inst().GetResolutionFactor(); 
}
