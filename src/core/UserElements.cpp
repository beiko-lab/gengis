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

#include "../core/UserElements.hpp"

#include "../utils/ErrorGL.hpp"
#include "../utils/Log.hpp"
#include "../utils/UniqueId.hpp"

using namespace GenGIS;

template<class Archive> 
void UserElements::serialize(Archive & ar, const unsigned int version)
{
	ar & m_lines;     // std::map<uint, VisualLinePtr>
	ar & m_markers;   // std::map<uint, VisualMarkerPtr>
	ar & m_cylinders; // std::map<uint, VisualLinePtr>
}
template void UserElements::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void UserElements::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

uint UserElements::AddLine(const VisualLinePtr line)
{ 
	uint uniqueId = UniqueId::Inst().GenerateId();
	m_lines.insert(std::pair<uint, VisualLinePtr>(uniqueId, line));

	return uniqueId;
}

uint UserElements::AddCylinder(const VisualLinePtr line, float thickness)
{ 
	uint uniqueId = UniqueId::Inst().GenerateId();

	line->SetThickness(thickness);
	m_cylinders.insert(std::pair<uint, VisualLinePtr>(uniqueId, line));

	return uniqueId;
}

uint UserElements::AddMarker(const VisualMarkerPtr marker)
{ 
	uint uniqueId = UniqueId::Inst().GenerateId();
	m_markers.insert(std::pair<uint, VisualMarkerPtr>(uniqueId, marker));

	return uniqueId;
}

void UserElements::Render()
{
	glDisable(GL_LIGHTING);

	std::map<uint, VisualLinePtr>::const_iterator itLine;
	for(itLine = m_lines.begin(); itLine != m_lines.end(); ++itLine)
	{
		itLine->second->Render();
	}

	std::map<uint, VisualMarkerPtr>::const_iterator itMarker;
	for(itMarker = m_markers.begin(); itMarker != m_markers.end(); ++itMarker)
	{
		itMarker->second->Render();
	}

	std::map<uint, VisualLinePtr>::const_iterator itCylinder;
	for(itCylinder = m_cylinders.begin(); itCylinder != m_cylinders.end(); ++itCylinder)
	{
		VisualLinePtr visualLine = itCylinder->second;
		VisualLine::RenderCylinder(visualLine->GetStartPt(), visualLine->GetEndPt(), visualLine->GetThickness(), 36, visualLine->GetColour(), false);
	}

	glEnable(GL_LIGHTING);
}
