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

#include "../utils/glUtils.hpp"
#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"

int glUtils::UnProject(GLdouble winx, GLdouble winy, GLdouble winz,
	const GLdouble modelMatrix[16], const GLdouble projMatrix[16], const GLint viewport[4],
	GLdouble *objx, GLdouble *objy, GLdouble *objz)
{
	double finalMatrix[16];
	double in[4];
	double out[4];

	MultMatricesd(modelMatrix, projMatrix, finalMatrix);
	if (!InvertMatrixd(finalMatrix, finalMatrix)) 
		return(GL_FALSE);

	in[0] = winx;
	in[1] = winy;
	in[2] = winz;
	in[3] = 1.0;

	/* Map x and y from window coordinates */
	in[0] = (in[0] - viewport[0]) / viewport[2];
	in[1] = (in[1] - viewport[1]) / viewport[3];

	/* Map to range -1 to 1 */
	in[0] = in[0] * 2 - 1;
	in[1] = in[1] * 2 - 1;
	in[2] = in[2] * 2 - 1;

	MultMatrixVecd(finalMatrix, in, out);
	if (out[3] == 0.0) 
		return(GL_FALSE);

	out[0] /= out[3];
	out[1] /= out[3];
	out[2] /= out[3];

	*objx = out[0];
	*objy = out[1];
	*objz = out[2];

	return(GL_TRUE);
}

int glUtils::UnProject4(GLdouble winx, GLdouble winy, GLdouble winz, GLdouble clipw,
	const GLdouble modelMatrix[16],	const GLdouble projMatrix[16], const GLint viewport[4],
	GLclampd nearVal, GLclampd farVal, GLdouble *objx, GLdouble *objy, GLdouble *objz,
	GLdouble *objw)
{
	double finalMatrix[16];
	double in[4];
	double out[4];

	MultMatricesd(modelMatrix, projMatrix, finalMatrix);
	if (!InvertMatrixd(finalMatrix, finalMatrix)) 
		return(GL_FALSE);

	in[0] = winx;
	in[1] = winy;
	in[2] = winz;
	in[3] = clipw;

	/* Map x and y from window coordinates */
	in[0] = (in[0] - viewport[0]) / viewport[2];
	in[1] = (in[1] - viewport[1]) / viewport[3];
	in[2] = (in[2] - nearVal) / (farVal - nearVal);

	/* Map to range -1 to 1 */
	in[0] = in[0] * 2 - 1;
	in[1] = in[1] * 2 - 1;
	in[2] = in[2] * 2 - 1;

	MultMatrixVecd(finalMatrix, in, out);
	if (out[3] == 0.0) 
		return(GL_FALSE);

	*objx = out[0];
	*objy = out[1];
	*objz = out[2];
	*objw = out[3];

	return(GL_TRUE);
}

int glUtils::UnProject4(GLdouble winx, GLdouble winy, GLdouble winz,
	const GLdouble modelMatrix[16], const GLdouble projMatrix[16], const GLint viewport[4],
	GLclampd nearVal, GLclampd farVal, GLdouble *objx, GLdouble *objy, GLdouble *objz)
{
	GLdouble objw;
	UnProject4(winx, winy, winz, 1.0, modelMatrix, projMatrix, viewport, 
		nearVal, farVal, objx, objy, objz, &objw);

	*objx /= objw;
	*objy /= objw;
	*objz /= objw;

	return(GL_TRUE);
}

void glUtils::MultMatricesd(const GLdouble a[16], const GLdouble b[16],	GLdouble r[16])
{
	int i, j;

	for (i = 0; i < 4; i++) 
	{
		for (j = 0; j < 4; j++) 
		{
			r[i*4+j] = 
				a[i*4+0]*b[0*4+j] +
				a[i*4+1]*b[1*4+j] +
				a[i*4+2]*b[2*4+j] +
				a[i*4+3]*b[3*4+j];
		}
	}
}

void glUtils::MultMatrixVecd(const GLdouble matrix[16], const GLdouble in[4], GLdouble out[4])
{
	int i;
	for (i=0; i<4; i++) 
	{
		out[i] = 
			in[0] * matrix[0*4+i] +
			in[1] * matrix[1*4+i] +
			in[2] * matrix[2*4+i] +
			in[3] * matrix[3*4+i];
	}
}

int glUtils::InvertMatrixd(const GLdouble m[16], GLdouble invOut[16])
{
	double inv[16], det;
	int i;

	inv[0] =  m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15] +
	          m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
	inv[4] = -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15] -
	          m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
	inv[8] =  m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15] +
	          m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
	inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14] -
	          m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
	inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15] -
	          m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
	inv[5] =  m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15] +
	          m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
	inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15] -
	          m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
	inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14] +
	           m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
	inv[2] =  m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15] +
	          m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
	inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15] -
	          m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
	inv[10] = m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15] +
	          m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
	inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14] -
	          m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
	inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11] -
	          m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
	inv[7] =  m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11] +
	          m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
	inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11] -
	          m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];

	inv[15] = m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10] +
	          m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];

	det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
	if (det == 0)
		return GL_FALSE;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return GL_TRUE;
}
