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

#include "../core/Lighting.hpp"

#include "../core/MapController.hpp"
#include "../core/MapView.hpp"
#include "../core/MapModel.hpp"

#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"

using namespace GenGIS;

const GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat ambient[]  = { 0.75f, 0.75f, 0.75f, 1.0f };
const GLfloat diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };

Lighting::Lighting() 
{
	m_lightPos[0] = -0.707f;
	m_lightPos[1] =  1.0f;
	m_lightPos[2] = -0.707f;
	m_lightPos[3] =  0.0f;

	m_globalAmbient[0] = 0.5f;
	m_globalAmbient[1] = 0.5f;
	m_globalAmbient[2] = 0.5f;
	m_globalAmbient[3] = 1.0f;
}

Lighting::Lighting( GLfloat* position ) 
{
	m_lightPos[0] = *position;
	m_lightPos[1] = *( position + 1 );
	m_lightPos[2] = *( position + 2 );
	m_lightPos[3] = *( position + 3 );

	m_globalAmbient[0] = 0.5f;
	m_globalAmbient[1] = 0.5f;
	m_globalAmbient[2] = 0.5f;
	m_globalAmbient[3] = 1.0f;
}

template<class Archive>
void Lighting::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_lightPos;      // GLfloat
	ar & m_globalAmbient; // GLfloat
}

template void Lighting::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void Lighting::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

Lighting::~Lighting() 
{

}

void Lighting::SetupLighting() 
{
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	glLightfv( GL_LIGHT0, GL_POSITION, m_lightPos );
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_globalAmbient);	
}

void Lighting::SetPosition( GLfloat* position ) 
{
	m_lightPos[0] = *position;
	m_lightPos[1] = *( position + 1 );
	m_lightPos[2] = *( position + 2 );
	m_lightPos[3] = *( position + 3 );
}

void Lighting::SetGlobalAmbient(GLfloat level)
{
	m_globalAmbient[0] = level;
	m_globalAmbient[1] = level;
	m_globalAmbient[2] = level;
}

void Lighting::SetDefaultValues()
{
	if(App::Inst().GetMapController()->GetMapModel()->GetElevationMap())
	{
		SetAltitude(0.667f);
		SetGlobalAmbient(0.5f);
	}
	else
	{
		SetAltitude(1.0f);
		SetGlobalAmbient(1.0f);
	}
}

