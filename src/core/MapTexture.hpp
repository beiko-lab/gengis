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

#ifndef _GENGIS_MAPTEXTURE_H_
#define _GENGIS_MAPTEXTURE_H_

#include "../core/Precompiled.hpp"
#include "../utils/Colour.hpp"

namespace GenGIS
{

	class MapTexture 
	{  
	public:
		/** Indicates how colours are assigned to cells in texture map. */
		enum INTERPOLATE { DISCRETE, LINEARLY, COLOUR_GRID };

	public:    
		/** Constructor. */
		explicit MapTexture();

		/** Destructor. */
		~MapTexture();

		/**
		* @brief Build texture to map onto map model using a default colour map.
		* @param progressDlg Dialog to track progress of terrain texture construction. 
		*/
		bool BuildTerrainTexture(MapControllerPtr mapController, ProgressDlgPtr progressDlg = ProgressDlgPtr());

		/**
		* @brief Build texture to map onto map model using the specified colours.
		* @param red Red channel of texture.
		* @param green Green channel of texture.
		* @param blue Blue channel of texture.
		* @param progressDlg Dialog to track progress of terrain texture construction. 
		*/
		bool BuildTerrainTexture(MapControllerPtr mapController, byte* red, byte* green, byte* blue, ProgressDlgPtr progressDlg = ProgressDlgPtr()); 

		/** Get OpenGL id of texture map. */
		GLuint GetTextureId() const { return m_texName; }

		/**
		* @brief Set colour map for terrain.
		* @param colourMap Colour map to use when rendering terrain.
		* @param intervals Defines interval value associated with each colour (in world space).
		* @param interpolate Specify method to use when interpolating colours of colour map.
		* @param parent Dialog calling this function.
		*/
		void SetColourMap(ColourMapDiscretePtr colourMap, const std::vector<float>& intervals, INTERPOLATE interpolate, wxWindow* parent);

		/**
		* @brief Set transparency percentage of terrain map (0 to 100)
		* @param transparency Value between 0 (no transparency) to 100 (full transparency)
		*/
		void SetTransparencyPercentage(uint transparency, wxWindow* parent);

		/** Get transparency percentage of terrain map (0 to 100). */
		uint GetTransparencyPercentage() const { return m_transparencyPercentage; }

		/** Get colour map. */
		ColourMapDiscretePtr GetColourMap() const { return m_colourMap; }

		void SetColourMap( ColourMapDiscretePtr colourMap ) { m_colourMap = colourMap; }

		/** Get number of colours in colour map. */
		uint GetNumColours() const { return m_numColours; }

		/** Get type of interpolation being performed. */
		INTERPOLATE GetInterpolation() const { return m_interpolate; }

		/** Get interval ranges (in world space). */
		void GetIntervals(std::vector<float>& intervals) const;

	private:
		/** Allocate memory for texture. */
		bool AllocateTextureMemory( MapControllerPtr mapController );

		void setTexturingStates();

		/** Specify the channel type of the texture map (i.e., GL_RGB, GL_RGBA, or GL_LUMINANCE_ALPHA) */
		void SetChannelMode( int channelMode );

		/** Assign colours to each pixel in the texture map based on elevation and the specifed colour map. */
		bool ComputeColour(Point3D* coords, ProgressDlgPtr progressDlg = ProgressDlgPtr());

		/** Set transpareny of terrain texture. */
		void SetTransparency(uint transparency, ProgressDlgPtr progressDlg = ProgressDlgPtr());

		/** Free memory allocated to terrain texture. */
		void FreeTexture();

		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	private:
		int m_texWidth;

		int m_texHeight;

		int m_numChannels;

		int m_channelMode;

		GLuint m_texName;

		GLubyte *m_texture;

		// Note: it would seem more appropriate to use std::vector
		// for m_colours and m_intervals, but the performance
		// penalty is noticable in this case. Please do not change back
		// to std::vector!
		boost::scoped_array<ColourPtr> m_colours;
		boost::scoped_array<float> m_intervals;

		ColourMapDiscretePtr m_colourMap;
		uint m_numColours;
		INTERPOLATE m_interpolate;

		uint m_transparencyPercentage;
	};

}

#endif
