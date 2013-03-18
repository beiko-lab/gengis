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

#include "../core/LayerTreeModel.hpp"
#include "../core/StudyLayer.hpp"

using namespace GenGIS;

template<class Archive> 
void LayerTreeModel::serialize(Archive & ar, const unsigned int version)
{
	ar & m_studies; // std::vector<StudyLayerPtr>
}

template void LayerTreeModel::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void LayerTreeModel::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

LayerTreeModel::LayerTreeModel()
{
}

void LayerTreeModel::AddStudy(StudyLayerPtr study) 
{ 
	m_studies.push_back(study); 
}

unsigned int LayerTreeModel::GetNumStudies() const 
{ 
	return m_studies.size(); 
}

StudyLayerPtr LayerTreeModel::GetStudy(uint index) const 
{ 
	return m_studies.at(index); 
}

void LayerTreeModel::RemoveStudy(uint index)
{
	// erase study and all children 
	for(unsigned int i = 0; i < m_studies.at(index)->GetNumMapLayers(); ++i)
	{
		m_studies.at(index)->RemoveMapLayerByIndex(0); // keep removing the first element
	}

	m_studies.erase(m_studies.begin() + index);
}

LayerPtr LayerTreeModel::GetLayer(uint id) const
{
	for(unsigned int studyIndex = 0; studyIndex < GetNumStudies(); studyIndex++)
	{
		StudyLayerPtr study = GetStudy(studyIndex);
		if(study->GetId() == id)
			return study;

		LayerPtr layer = study->GetLayer(id);
		if(layer != LayerPtr())
		{
			return layer;
		}
	}

	return LayerPtr();
}

bool LayerTreeModel::RemoveLayer(uint id)
{
	bool bFound = false;
	for(unsigned int studyIndex = 0; studyIndex < GetNumStudies() && !bFound; studyIndex++)
	{
		StudyLayerPtr study = GetStudy(studyIndex);
		if(study->GetId() == id)
		{
			RemoveStudy(studyIndex);
			return true;
		}

		bFound = study->RemoveLayer(id);
	}

	return bFound;
}

void LayerTreeModel::Render()
{
	for(unsigned int index = 0; index < GetNumStudies(); index++)
	{
		GetStudy(index)->Render();
	}
}
