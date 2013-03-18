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

#ifndef _GENGIS_GL_UTILS_H_
#define _GENGIS_GL_UTILS_H_

#include "../core/Precompiled.hpp"

/**
* @brief Helpful functions for working with OpenGL.
*/
class glUtils
{
public:
	static int UnProject(GLdouble winx, GLdouble winy, GLdouble winz,
		const GLdouble modelMatrix[16], 
		const GLdouble projMatrix[16],
		const GLint viewport[4],
		GLdouble *objx, GLdouble *objy, GLdouble *objz);

	static int UnProject4(GLdouble winx, GLdouble winy, GLdouble winz, GLdouble clipw,
		const GLdouble modelMatrix[16], 
		const GLdouble projMatrix[16],
		const GLint viewport[4],
		GLclampd nearVal, GLclampd farVal,		    
		GLdouble *objx, GLdouble *objy, GLdouble *objz,
		GLdouble *objw);

	static int UnProject4(GLdouble winx, GLdouble winy, GLdouble winz,
		const GLdouble modelMatrix[16], 
		const GLdouble projMatrix[16],
		const GLint viewport[4],
		GLclampd nearVal, GLclampd farVal,		    
		GLdouble *objx, GLdouble *objy, GLdouble *objz);

	static void MultMatrixVecd(const GLdouble matrix[16], const GLdouble in[4], GLdouble out[4]);
	static void MultMatricesd(const GLdouble a[16], const GLdouble b[16],	GLdouble r[16]);
	static int InvertMatrixd(const GLdouble m[16], GLdouble invOut[16]);
};

#endif
