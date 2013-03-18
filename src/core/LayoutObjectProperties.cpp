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

#include "../core/LayoutObjectProperties.hpp"

#include "../core/App.hpp"

using namespace GenGIS;

template<class Archive>
void LayoutObjectProperties::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_objectLineThickness; // float
	ar & m_objectLineColour;    // Colour
	ar & m_objectLineStyle;     // VisualLine::LINE_STYLE
	ar & m_layoutLineThickness; // float
	ar & m_layoutLineColour;    // Colour
	ar & m_layoutLineStyle;     // VisualLine::LINE_STYLE
	ar & m_controlPointSize;    // float
	ar & m_controlPointColour;  // Colour
}
template void LayoutObjectProperties::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void LayoutObjectProperties::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

float LayoutObjectProperties::GetObjectLineThickness(bool bCorrected) const 
{ 
	if(bCorrected)
		return m_objectLineThickness * App::Inst().GetResolutionFactor(); 

	return m_objectLineThickness;
}

float LayoutObjectProperties::GetLayoutLineThickness(bool bCorrected) const 
{ 
	if(bCorrected)
		return m_layoutLineThickness * App::Inst().GetResolutionFactor(); 

	return m_layoutLineThickness;
}


float LayoutObjectProperties::GetControlPointSize(bool bCorrected) const 
{ 
	if(bCorrected)
		return m_controlPointSize * App::Inst().GetResolutionFactor(); 

	return m_controlPointSize;
}