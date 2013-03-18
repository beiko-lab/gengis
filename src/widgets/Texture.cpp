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

#include "../widgets/Texture.hpp"

using namespace GenGIS;

Texture::Texture( GLubyte* data, int width, int height, int scaledWidth, 
                  int scaledHeight, bool bGenerateMipMaps, int channelMode ) :
	m_data(data), m_width(width), m_height(height), m_scaledWidth(scaledWidth),
	m_scaledHeight(scaledHeight), m_bGenerateMipMaps(bGenerateMipMaps)
{    
	SetChannelMode(channelMode);

	m_scaledData = (GLubyte*)malloc(sizeof(GLubyte) * m_scaledWidth * m_scaledHeight * m_numChannels);

	Init();
}

Texture::~Texture() 
{
	free( m_data );
	free( m_scaledData );
}

void Texture::Init() 
{
	// set alignment for start of each pixel row in memory
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	glGenTextures( 1, &m_name );

	// bind texture to its ID
	glBindTexture( GL_TEXTURE_2D, m_name );

	// scale the image
	gluScaleImage( m_channelMode, m_width, m_height, GL_UNSIGNED_BYTE, m_data, 
		m_scaledWidth, m_scaledHeight, GL_UNSIGNED_BYTE, m_scaledData );

	// build mipmaps
	if ( m_bGenerateMipMaps ) 
	{
		gluBuild2DMipmaps( GL_TEXTURE_2D, m_numChannels, m_scaledWidth, m_scaledHeight,
			m_channelMode, GL_UNSIGNED_BYTE, m_scaledData );	    
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	}
	else 
	{
		glTexImage2D( GL_TEXTURE_2D, 0, m_channelMode, m_scaledWidth, m_scaledHeight,
			0, m_channelMode, GL_UNSIGNED_BYTE, m_scaledData ); 
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	}

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}


void Texture::SetChannelMode( int channelMode ) 
{
	// store the channel mode (RGB, RGBA, GL_LUMINANCE_ALPHA)
	m_channelMode = channelMode;

	// number of channels, given the mode
	switch ( m_channelMode ) 
	{
	case GL_RGB:
		m_numChannels = 3;
		break;
	case GL_RGBA:
		m_numChannels = 4;
		break;
	case GL_LUMINANCE_ALPHA:
		m_numChannels = 2;
		break;
	}
}
