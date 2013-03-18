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

#include "../core/TreeLayer.hpp"
#include "../core/GeoTreeView.hpp"

using namespace GenGIS;

template<class Archive>
void TreeLayer::serialize(Archive & ar, const unsigned int version)
{
	ar & boost::serialization::base_object<Layer>(*this);
}
template void TreeLayer::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void TreeLayer::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


LayerPtr TreeLayer::GetLayer(uint id) const
{
	// tree node is terminal
	return LayerPtr();
}

bool TreeLayer::RemoveLayer(uint id)
{
	// tree node is terminal
	return false;
}

void TreeLayer::Render()
{
	if(!IsActive())
		return;

	m_treeView->Render();
}


void TreeLayer::SetActive(bool state) 
{ 
	m_treeView->SetVisibility(state); 
}

bool TreeLayer::IsActive() const 
{ 
	return m_treeView->IsVisible(); 
}

void TreeLayer::ToggleActive() 
{ 
	m_treeView->SetVisibility(!m_treeView->IsVisible()); 
}
