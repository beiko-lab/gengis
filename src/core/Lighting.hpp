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

#ifndef _GENGIS_LIGHTING_
#define _GENGIS_LIGHTING_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	class Lighting 
	{
	public:
		explicit Lighting();
		explicit Lighting( GLfloat* position );
		~Lighting();

		
		/** Set default lighting values. */
		void SetDefaultValues();

		void SetupLighting();
		void SetPosition( GLfloat* position );

		void SetPositionX(float x) { m_lightPos[0] = x; }
		GLfloat GetPositionX() const { return m_lightPos[0]; }
		void SetPositionY(float y) { m_lightPos[1] = y; }
		GLfloat GetPositionY() const { return m_lightPos[1]; }
		void SetPositionZ(float z) { m_lightPos[2] = z; }
		GLfloat GetPositionZ() const { return m_lightPos[2]; }

		void SetGlobalAmbient(GLfloat level);
		GLfloat GetGlobalAmbient() const { return m_globalAmbient[0]; }

		void SetAltitude(GLfloat altitude) { m_lightPos[1] = altitude; }
		GLfloat GetAltitude() const { return m_lightPos[1]; }

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	private:
		GLfloat m_lightPos[4];
		GLfloat m_globalAmbient[4];
	};

}

#endif
