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

#ifndef _GENGIS_ERROR_GL_
#define _GENGIS_ERROR_GL_

#include "../core/Precompiled.hpp"

namespace error
{

	/**
	* @brief OpenGL error checking routines.
	*/
	class ErrorGL
	{
	public:
		/**
		* @brief Assert that a boolean expression is true.
		* @return True if an error has occured.
		*/
		static void Check()
		{
			#ifndef NDEBUG
			static const int MAX_STRING_LEN = 4096;

			GLenum errCode = glGetError();
			const GLubyte *errString = gluErrorString(errCode);

			char strBuffer[MAX_STRING_LEN];
			sprintf(strBuffer, "(OpenGL Error) %s", errString);

			// ~mikep. Ignoring all GL_INVALID_VALUE errors, for some (as of yet unknown)
			// reason they are showing up on the Mac build (and killing the program).
			// ~mikep. Changing the code to ignore them on Windows too... I think these
			// errors appeared (on windows) when I removed "NDEBUG" to the release build's
			// compiler flags. Note that the debug build doesn't have this error.
			if(errCode == GL_INVALID_VALUE)
			{
				return;
			}

			assert(errCode == GL_NO_ERROR);
			#endif
		}

		/** Clear all OpenGL errors. */
		static void Clear()
		{
			GLenum errCode;
			do
			{
				errCode = glGetError();
			} while(errCode != GL_NO_ERROR);
		}

	};

}

#endif
