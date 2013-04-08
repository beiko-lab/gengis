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

#include "../core/StudyModel.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/Log.hpp"

using namespace GenGIS;


template<class Archive>
void StudyModel::save(Archive & ar, const unsigned int version) const
{
	// Manually saving m_oDestination (OGRSpatialReference class)
	char * _ppszResult = NULL;
	m_oDestination.exportToWkt(&_ppszResult);
	int nElements = strlen(_ppszResult) + 1;

	ar & nElements;
	for (int i = 0; i < nElements; i++)
		ar & _ppszResult[i];

	// manually saving m_projectionTool 	
	char * _souResult = NULL;
	char * _targResult = NULL;

	if(m_projectionTool!=NULL)
	{
		OGRSpatialReference* sourceProj = m_projectionTool->GetSourceCS();
		sourceProj->exportToWkt( &_souResult );
		int nSourceElements = strlen(_souResult) + 1;
		ar & nSourceElements;
		for (int i = 0; i < nSourceElements; i++)
			ar & _souResult[i];
		
		
		OGRSpatialReference* targetProj = m_projectionTool-> GetTargetCS() ;	
		targetProj->exportToWkt( &_targResult );	
		int nTargetElements = strlen(_targResult) + 1;
		ar & nTargetElements;
		for (int i = 0; i < nTargetElements; i++)
			ar & _targResult[i];
	}
	else
	{
		int nSourceElements=0;
		int nTargetElements=0;
		ar & nSourceElements;
		ar & nTargetElements;

	}

}
template void StudyModel::save<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version) const; 

template<class Archive>
void StudyModel::load(Archive & ar, const unsigned int version)
{
	// Manually loading m_oDestination (OGRSpatialReference class)
	int nElements;
	ar & nElements;

	char * _ppszResult = new char[nElements];

	for (int i = 0; i < nElements; i++)
		ar & _ppszResult[i];

	m_oDestination.importFromWkt(&_ppszResult);

	// manually loading m_projectionTool
	OGRSpatialReference sourceProj;
	OGRSpatialReference targetProj;

	int nSourceElements;	
	ar & nSourceElements;
	
	char * _souResult = new char[nSourceElements];
	if (nSourceElements!= 0)
	{
		for (int i = 0; i < nSourceElements; i++)
			ar & _souResult[i];
		
		if( sourceProj.importFromWkt(&_souResult) == OGRERR_CORRUPT_DATA)
		{
			Log::Inst().Write("err");
			return;
		}
	}
	
	int nTargetElements;
	ar & nTargetElements;
	char * _targResult = new char[nTargetElements];
	if (nTargetElements!= 0 )
	{
		for (int i = 0; i < nTargetElements; i++)
			ar & _targResult[i];

		targetProj.importFromWkt(&_targResult);
	
	OGRCoordinateTransformation *poTransform = OGRCreateCoordinateTransformation(&sourceProj, &targetProj);
	m_projectionTool.reset(poTransform);
	}

}
template void StudyModel::load<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

//#ifndef BOOST_SERIALIZATION_ASSUME_ABSTRACT(ProjectionToolPtr)
//BOOST_CLASS_EXPORT(ProjectionToolPtr)

template<class Archive>
void StudyModel::serialize(Archive & ar, const unsigned int version)
{	
	
	ar & m_bActive;              // bool
	//ar & m_projectionTool;       // ProjectionToolPtr
	//ar & m_bGeographic;          // bool
	ar & m_bProjectData;         // bool
	ar & m_datum;                // std::wstring
	ar & m_projection;           // std::wstring
	ar & m_centreLat;            // float
	ar & m_centreLong;           // float
	ar & m_standardParallel1;    // float
	ar & m_standardParallel2;    // float

	boost::serialization::split_member(ar, *this, version);
	
}


template void StudyModel::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void StudyModel::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void StudyModel::CalculateProjectionTool(OGRSpatialReference* oSource)
{
	if(m_bProjectData)
	{
		SetDestinationProjection();

		OGRCoordinateTransformation *poTransform;
		if(oSource != NULL)
		{
			if(IsGeographic())
				poTransform = OGRCreateCoordinateTransformation(oSource, &m_oDestination);
			else
			{
				OGRSpatialReference* poLatLong = oSource->CloneGeogCS();
				poTransform = OGRCreateCoordinateTransformation(poLatLong, oSource);
			}
		}
		else
		{
			// assume the source is in lat/lon with the same datum as specified for the destination
			OGRSpatialReference* poLatLong = m_oDestination.CloneGeogCS();
			poTransform = OGRCreateCoordinateTransformation(poLatLong, &m_oDestination);
		}

		if(poTransform == NULL)
		{
			Log::Inst().Warning("(Warning) Failed to create projection tool.");
			return;
		}

		m_projectionTool.reset(poTransform);
	}
}

void StudyModel::SetDestinationProjection()
{
	std::string projInfoStr = "";

	// set datum
	if(m_datum == _T("WGS84"))
	{
		projInfoStr = "WGS84";
		m_oDestination.SetWellKnownGeogCS("WGS84");
	}
	else if(m_datum == _T("WGS72"))
	{
		projInfoStr = "WGS72";
		m_oDestination.SetWellKnownGeogCS("WGS72");
	}
	else if(m_datum == _T("NAD83"))
	{
		projInfoStr = "NAD83";
		m_oDestination.SetWellKnownGeogCS("NAD83");
	}
	else if(m_datum == _T("NAD27"))
	{
		projInfoStr = "NAD27";
		m_oDestination.SetWellKnownGeogCS("NAD27");
	}

	// set projection
	if(m_projection == _T("Albers Conic Equal Area"))
	{
		projInfoStr += " / Albers Conic Equal Area";
		m_oDestination.SetACEA(m_standardParallel1, m_standardParallel2, m_centreLat, m_centreLong, 0, 0); 	
	}
	else if(m_projection == _T("Azimuthal Equidistant"))
	{
		projInfoStr += " / Azimuthal Equidistant";
		m_oDestination.SetAE(m_centreLat, m_centreLong, 0, 0);
	}
	else if(m_projection == _T("Eckert IV"))
	{
		projInfoStr += " / Eckert IV";
		m_oDestination.SetEckertIV(m_centreLong, 0, 0);
	}
	else if(m_projection == _T("Equidistant Conic"))
	{
		projInfoStr += " / Equidistant Conic";
		m_oDestination.SetEC(m_standardParallel1, m_standardParallel2, m_centreLat, m_centreLong, 0, 0);
	}
	else if(m_projection == _T("Gall Stereograpic"))
	{
		projInfoStr += " / Gall Stereograpic";
		m_oDestination.SetGS(m_centreLong, 0, 0); 
	}
	else if(m_projection == _T("Lambert Azimuthal Equal-Area"))
	{
		projInfoStr += " / Lambert Azimuthal Equal-Area";
		m_oDestination.SetLAEA(m_centreLat, m_centreLong, 0, 0); 	
	}
	else if(m_projection == _T("Lambert Conformal Conic"))
	{
		projInfoStr += " / Lambert Conformal Conic";
		m_oDestination.SetLCC(m_standardParallel1, m_standardParallel2, m_centreLat, m_centreLong, 0, 0);
	}
	else if(m_projection == _T("Mercator"))
	{
		projInfoStr += " / Mercator";
		m_oDestination.SetMercator(m_centreLat, m_centreLong, 1, 0, 0);
	}
	else if(m_projection == _T("Mollweide"))
	{
		projInfoStr += " / Mollweide";
		m_oDestination.SetMollweide(m_centreLong, 0, 0);
	}
	else if(m_projection == _T("Orthographic"))
	{
		projInfoStr += " / Orthographic";
		m_oDestination.SetOrthographic(m_centreLat, m_centreLong, 0, 0); 
	}
	else if(m_projection == _T("Plate Carree (Equirectangular)"))
	{
		projInfoStr += " / Plate Carree (Equirectangular)";
		m_oDestination.SetEquirectangular(m_centreLat, m_centreLong, 0, 0);
	}
	else if(m_projection == _T("Robinson"))
	{
		projInfoStr += " / Robinson";
		m_oDestination.SetRobinson(m_centreLong, 0, 0);
	}
	else if(m_projection == _T("Stereographic"))
	{
		projInfoStr += " / Stereographic";
		m_oDestination.SetStereographic(m_centreLat, m_centreLong, 1, 0, 0); 
	}
	else if(m_projection == _T("Transverse Mercator"))
	{
		projInfoStr += " / Transverse Mercator";
		m_oDestination.SetTM(m_centreLat, m_centreLong, 1, 0, 0);
	}
	else if(m_projection == _T("Universal Transverse Mercator (UTM)"))
	{
		int utmZone = int(m_centreLong/6.0f) + 31;
		projInfoStr += " / UTM Zone " + StringTools::ToString(utmZone);
		m_oDestination.SetUTM(utmZone, m_centreLat > 0);
	}

	m_oDestination.SetProjCS(projInfoStr.c_str());
}

void StudyModel::SetCentreLatitude(float centre)
{
	m_centreLat = centre;
}


void StudyModel::SetCentreLongitude(float centre)
{
	m_centreLong = centre;
}

void StudyModel::SetFirstStandardParallel(float sp)
{
	m_standardParallel1 = sp;
}

void StudyModel::SetSecondStandardParallel(float sp)
{
	m_standardParallel2 = sp;
}
