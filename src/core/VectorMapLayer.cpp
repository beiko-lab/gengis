//=======================================================================
// Author: Timothy Mankowski
//
// Copyright 2012 Timothy Mankowski
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

#include "../core/VectorMapView.hpp"
#include "../core/VectorMapLayer.hpp"
#include "../core/VectorMapController.hpp"

using namespace GenGIS;

template<class Archive>
void VectorMapLayer::load(Archive & ar, const unsigned int version)
{
	//
}
template void VectorMapLayer::load<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

template<class Archive>
void VectorMapLayer::save(Archive & ar, const unsigned int version) const
{
	//
}
template void VectorMapLayer::save<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version) const; 

template<class Archive>
void VectorMapLayer::serialize(Archive & ar, const unsigned int version)
{
	ar & boost::serialization::base_object<Layer>(*this);
	ar & m_vectorMapController; // VectorMapControllerPtr	

	boost::serialization::split_member(ar, *this, version);
}
template void VectorMapLayer::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void VectorMapLayer::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


VectorMapControllerPtr VectorMapLayer::GetVectorMapController() const
{
	return m_vectorMapController;
}

void VectorMapLayer::Render()
{
	if ( !IsActive() )
		return;

	m_vectorMapController->GetVectorMapView()->Render();
}

void VectorMapLayer::SetActive(bool state)
{
	//App::Inst().GetMapController()->SetActive(state);
	m_vectorMapController->SetActive(state);
}

bool VectorMapLayer::IsActive() const
{
	//return App::Inst().GetMapController()->IsActive();
	return m_vectorMapController->IsActive();
}

void VectorMapLayer::ToggleActive()
{
	//App::Inst().GetMapController()->ToggleActive();
	m_vectorMapController->ToggleActive();
}