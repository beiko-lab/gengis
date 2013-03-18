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

#include "../core/LabelController.hpp"
#include "../core/OrthoLayers.hpp"
#include "../core/Viewport.hpp"
#include "../core/MapController.hpp"

#include "../gui/GenGisFrame.hpp"

#include "../utils/ErrorGL.hpp"
#include "../utils/Log.hpp"
#include "../utils/UniqueId.hpp"

using namespace GenGIS;

template<class Archive> 
void LabelController::serialize(Archive & ar, const unsigned int version)
{
	ar & m_labels; // std::map<uint, VisualLabelPtr>
}

template void LabelController::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void LabelController::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

uint LabelController::AddLabel(VisualLabelPtr label) 
{ 
	uint uniqueId = UniqueId::Inst().GenerateId();
	label->SetId(uniqueId);
	m_labels.insert(std::pair<uint, VisualLabelPtr>(uniqueId, label));

	return uniqueId;
}

void LabelController::Render()
{
	glDisable(GL_LIGHTING);

	std::map<uint, VisualLabelPtr>::const_iterator it;
	for(it = m_labels.begin(); it != m_labels.end(); ++it)
	{		
		it->second->Render();
	}

	glEnable(GL_LIGHTING);
}

bool LabelController::MouseLeftDown(const Point2D& mousePt)
{
	std::map<uint, VisualLabelPtr>::const_iterator it;
	for(it = m_labels.begin(); it != m_labels.end(); ++it)
	{
		if(it->second->MouseLeftDown(mousePt))
		{
			App::Inst().SetSelectedView(it->second);
			return true;
		}
	}

	return false;
}

bool LabelController::MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu)
{
	std::map<uint, VisualLabelPtr>::const_iterator it;
	for(it = m_labels.begin(); it != m_labels.end(); ++it)
	{
		if(it->second->MouseRightDown(mousePt, popupMenu))
		{
			App::Inst().SetRightClickView(it->second);
			return true;
		}
	}

	return false;
}
