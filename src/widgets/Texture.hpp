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

#ifndef _GENGIS_TEXTURE_
#define _GENGIS_TEXTURE_

#include "../core/Precompiled.hpp"

namespace GenGIS
{
	/**
	* @class Texture
	* @brief Provides texturing for widgets.
	*/
	class Texture 
	{    
	public:  
		/** Constructor. */
		explicit Texture( GLubyte* data, int width, int height, int scaledWidth = 1024, int scaledHeight = 1024,
			bool bGenerateMipMaps = false, int channelMode = GL_RGBA );

		/** Destructor. */
		~Texture();

		/** Set the color chanel mode: RGB, RGBA, GL_LUMINANCE_ALPHA. */
		void SetChannelMode( int channelMode );

		/** Get name of texture. */
		unsigned int GetName() { return m_name; }

	protected:	
		/** Initialize texture. */  
		void Init();

	private:
		/** Width of texture in pixels. */
		int m_width;

		/** Height of texture in pixels. */
		int m_height;

		/** Scaled width of texture in pixels. */
		int m_scaledWidth;

		/** Scaled height of texture in pixels. */
		int m_scaledHeight;

		/** Texture data. */
		GLubyte* m_data;

		/** Scaled texture data. */
		GLubyte* m_scaledData;

		/** Name of texture. */
		GLuint m_name;

		/** Channel mode for texture. */
		int m_channelMode;

		/** Number of channels. */
		int m_numChannels;

		/** Flag indicating if mipmaps should be generated. */
		bool m_bGenerateMipMaps;
	};

}

#endif
