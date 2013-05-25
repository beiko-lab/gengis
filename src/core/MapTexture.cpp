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

#include "../core/MapTexture.hpp"
#include "../core/App.hpp"
#include "../core/MapController.hpp"
#include "../core/MapModel.hpp"

#include "../gui/ProgressDlg.hpp"

#include "../utils/Colour.hpp"
#include "../utils/ColourMapManager.hpp"
#include "../utils/ColourMap.hpp"
#include "../utils/ColourMapDiscrete.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/Log.hpp"

#include "../utils/ErrorGL.hpp"

using namespace std;
using namespace GenGIS;

/*
Author: Jacqui
Date: 27th Feb 2008
Version: 0.1
Comments: Added to allow procedural texture to be created
when coloring terrain using a Vertex-by-Vertex computed height map.
*/

template<class Archive>
void MapTexture::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_texWidth;               // int
	ar & m_texHeight;              // int
	ar & m_numChannels;            // int
	ar & m_channelMode;            // int
	ar & m_texName;                // GLuint
	ar & m_colourMap;              // ColourMapDiscretePtr
	ar & m_numColours;             // uint
	ar & m_interpolate;            // INTERPOLATE
	ar & m_transparencyPercentage; // uint

	if (Archive::is_loading::value)
	{
		m_colours.reset(new ColourPtr[m_numColours]);
		m_intervals.reset(new float[m_numColours]);
	}

	for (uint i = 0; i < m_numColours; i++)
	{
		ar & m_colours[i];
		ar & m_intervals[i];
	}
}
template void MapTexture::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void MapTexture::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

MapTexture::MapTexture() :
	m_texture(NULL), m_transparencyPercentage(0), m_numColours(0),m_texWidth(0), m_texHeight(0),
	m_numChannels(0), m_channelMode(0), m_texName(0), m_interpolate(LINEARLY)
{

}

MapTexture::~MapTexture() 
{
	FreeTexture();
}

void MapTexture::FreeTexture()
{
	if(m_texture)
	{
		free(m_texture);
		m_texture = NULL;
	}
}

bool MapTexture::AllocateTextureMemory(MapControllerPtr mapController)
{
	SetChannelMode( GL_RGBA );

	// the total number of pixels
	FileHeader* demHeader = mapController->GetMapModel()->GetHeader();
	m_texWidth = demHeader->nCols;
	m_texHeight = demHeader->nRows;
	int totalPixels = m_texWidth * m_texHeight;

	FreeTexture();
	m_texture = ( GLubyte* )malloc( sizeof( GLubyte ) * m_numChannels * totalPixels );
	if ( m_texture == NULL ) 
	{
		Log::Inst().Error("MapTexture::AllocateTextureMemory(): Insufficient memory to create terrain texture.");
		return false;
	}

	return true;
}

bool MapTexture::BuildTerrainTexture(MapControllerPtr mapController, ProgressDlgPtr progressDlg)
{
	if(!AllocateTextureMemory( mapController ))
		return false;

	MapModelPtr mapModel = mapController->GetMapModel();

	// Check if default colour map has been loaded
	if(!m_colourMap)
	{
		ColourMapManagerPtr colourMapManager = App::Inst().GetColourMapManager();
		ColourMapPtr defaultColourMap = colourMapManager->GetDefaultTerrainColourMap();
		m_colourMap.reset(new ColourMapDiscrete(defaultColourMap));
	}

	// If map is being deserialized, the following operations are not necessary
	if (m_numColours == 0)
	{
		// set colours
		float alpha = 1.0 - m_transparencyPercentage / 100.0;
		m_numColours = m_colourMap->GetSize();

		m_colours.reset(new ColourPtr[m_numColours]);
		for(uint i = 0; i < m_numColours; ++i)
		{
			Colour colour = m_colourMap->GetColour(i);
			m_colours[i] = ColourPtr(new Colour(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), alpha));
		}

		// set intervals
		m_intervals.reset(new float[m_numColours]);

		m_intervals[2] = 0.01 * mapModel->GetMaxElevationGridSpace();
		m_intervals[3] = 0.33 * mapModel->GetMaxElevationGridSpace();
		m_intervals[4] = 0.66 * mapModel->GetMaxElevationGridSpace();
		m_intervals[5] = mapModel->GetMaxElevationGridSpace();

		if(mapModel->GetMinElevationGridSpace() < 0)
		{
			m_intervals[0] = mapModel->GetMinElevationGridSpace();
			m_intervals[1] = 0.0f;
			m_intervals[2] = 0.0f;
			m_colours[1] = m_colours[0];
			m_colourMap->SetColour(Colour(m_colours[1]->GetRed(), m_colours[1]->GetGreen(), m_colours[1]->GetBlue()), 1);
		}
		else if(mapModel->GetMinElevationGridSpace() == 0)
		{
			m_intervals[0] = 0.0f;
			m_intervals[1] = 0.0f;
		}
		else
		{
			m_intervals[0] = mapModel->GetMinElevationGridSpace();
			m_intervals[1] = mapModel->GetMinElevationGridSpace();
		}

		m_interpolate = LINEARLY;
	}

	if(progressDlg)
	{
		if(!progressDlg->Update(0, _T("Building terrain texture: computing elevation colours.")))
			return false;
	}

	// Compute the colour for each vertex in the terrain
	bool b = ComputeColour(mapModel->GetGrid(), progressDlg);

	if(b)
		setTexturingStates();

	return b;
}

bool MapTexture::BuildTerrainTexture(MapControllerPtr mapController, byte* red, byte* green, byte* blue, ProgressDlgPtr progressDlg) 
{
	if(!AllocateTextureMemory( mapController ))
		return false;

	m_interpolate = COLOUR_GRID;

	if(progressDlg)
	{
		if(!progressDlg->Update(0, _T("Building terrain texture: assigning colours to each map point.")))
			return false;
	}

	int texSize = m_texWidth * m_texHeight;
	uint j = 0;
	for(int i = 0; i < texSize; i++ ) 
	{
		// update progress
		if(progressDlg && i % 1000 == 0)
		{
			if(!progressDlg->Update(int((float(i)/texSize)*99)))
				return false;
		}

		m_texture[ j ] = GLubyte( red[i] );
		m_texture[ j + 1 ] = GLubyte( green[i] );
		m_texture[ j + 2 ] = GLubyte( blue[i] );
		m_texture[ j + 3 ] = GLubyte( 255 );

		j += 4; //keep track of where we are in rgba byte array - each Vertex as 4 entries
	}

	if(progressDlg)
	{
		if(!progressDlg->Update(0, _T("Building terrain texture: creating multiresolution textures.")))
			return false;
	}

	setTexturingStates();

	return true;
}

bool MapTexture::ComputeColour(Point3D* coords, ProgressDlgPtr progressDlg) 
{
	float currentHeight = 0;
	int j = 0;

	float alpha = 1.0 - m_transparencyPercentage / 100.0;

	int texSize = m_texWidth * m_texHeight;
	for(int i = 0; i < texSize; i++ ) 
	{
		// update progress
		if(progressDlg && i % 1000 == 0)
		{
			if(!progressDlg->Update(int((float(i)/texSize)*99)))
				return false;
		}

		currentHeight = coords[i].y;

		// find interval current point falls in
		uint intervalIndex;
		for(intervalIndex = 0; intervalIndex < m_numColours; ++intervalIndex) 
		{
			if(currentHeight <= m_intervals[intervalIndex]) 
			{
				break;
			}
		}

		if(intervalIndex == m_numColours)	// can occur due to rounding errors when calculating the maximum m_intervals value
			intervalIndex = m_numColours - 1;

		// linearly interpolate colour
		ColourPtr colour;
		if(intervalIndex == 0)
		{
			colour.reset(new Colour(m_colours[0]->GetRed(), m_colours[0]->GetGreen(), m_colours[0]->GetBlue(), alpha));
		}
		else
		{
			ColourPtr colour1 = m_colours[intervalIndex-1];
			ColourPtr colour2 = m_colours[intervalIndex];
			float interval1 = m_intervals[intervalIndex-1];
			float interval2 = m_intervals[intervalIndex];

			if(m_interpolate == LINEARLY)
			{
				Colour interpolatedColour = Colour::Interpolate(colour1, colour2, currentHeight, interval1, interval2);
				colour.reset(new Colour(interpolatedColour.GetRed(), interpolatedColour.GetGreen(), interpolatedColour.GetBlue(), alpha));
			}
			else
			{
				colour.reset(new Colour(colour1->GetRed(), colour1->GetGreen(), colour1->GetBlue(), alpha));
			}
		}

		m_texture[ j ] = GLubyte( colour->GetRedInt() );
		m_texture[ j + 1 ] = GLubyte( colour->GetGreenInt() );
		m_texture[ j + 2 ] = GLubyte( colour->GetBlueInt() );
		m_texture[ j + 3 ] = GLubyte( colour->GetAlphaInt() );

		j += 4; //keep track of where we are in rgba byte array - each Vertex as 4 entries
	}

	return true;
}

void MapTexture::setTexturingStates() 
{
	// bind texture to its ID
	glGenTextures( 1, &m_texName );
	glBindTexture( GL_TEXTURE_2D, m_texName );

	// build mipmaps
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	GLint rval = gluBuild2DMipmaps( GL_TEXTURE_2D, m_numChannels, m_texWidth, m_texHeight,
		m_channelMode, GL_UNSIGNED_BYTE, m_texture );

	error::ErrorGL::Check();

	if ( rval != 0 )
		return;
}

// set the color chanel mode (RGB, RGBA, GL_LUMINANCE_ALPHA)
void MapTexture::SetChannelMode( int channelMode ) 
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

void MapTexture::SetTransparencyPercentage(uint transparency, wxWindow* parent) 
{ 
	if(transparency != m_transparencyPercentage)
	{
		m_transparencyPercentage = transparency; 

		ProgressDlgPtr progressDlg(new ProgressDlg(wxT("Setting transparency"), wxT("Modifying terrain texture: settings transparency."), 
			100, parent));

		SetTransparency((1.0-m_transparencyPercentage/100.0)*255);
		setTexturingStates();
	}
}

void MapTexture::SetTransparency(uint transparency, ProgressDlgPtr progressDlg)
{
	int texSize = m_texWidth * m_texHeight;
	uint j = 0;
	for(int i = 0; i < texSize; i++ ) 
	{
		// update progress
		if(progressDlg && i % 1000 == 0)
		{
			if(!progressDlg->Update(int((float(i)/texSize)*100)))
				return;
		}

		m_texture[ j + 3 ] = GLubyte( transparency );

		j += 4; //keep track of where we are in rgba byte array - each Vertex as 4 entries
	}
}

void MapTexture::SetColourMap(ColourMapDiscretePtr colourMap, const std::vector<float>& intervals, INTERPOLATE interpolate, wxWindow* parent)
{
	m_colourMap = colourMap;
	//m_numColours = colourMap->GetNameToColourMapSize();
	m_numColours = intervals.size();
	m_colours.reset(new ColourPtr[m_numColours]);
	for(uint i = 0; i < m_numColours; ++i)
	{
		Colour colour;
		if(!colourMap->GetColour(StringTools::ToStringW(i), colour))
		{
			Log::Inst().Error("(Error) MapTexture::SetColourMap(): no colour associated with name.");
		}

		m_colours[i] = ColourPtr(new Colour(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha()));
	}

	m_intervals.reset(new float[m_numColours]);
	for(uint i = 0; i < m_numColours; ++i)
		m_intervals[i] = intervals.at(i) * App::Inst().GetMapController()->GetMapModel()->GetHeader()->scaleFactor;

	m_interpolate = interpolate;

	glDeleteTextures( 1, &m_texName );
	ProgressDlgPtr progressDlg(new ProgressDlg(wxT("Computing colour map"), wxT("Building terrain texture: computing elevation colours."), 
		100, parent));

	ComputeColour(App::Inst().GetMapController()->GetMapModel()->GetGrid(), progressDlg);
	setTexturingStates();

	error::ErrorGL::Check();
}

void MapTexture::GetIntervals(std::vector<float>& intervals) const
{
	if(m_interpolate == COLOUR_GRID)
	{
		intervals.clear();
		return;
	}

	for(uint i = 0; i < m_numColours; ++i)
	{
		intervals.push_back(m_intervals[i] / App::Inst().GetMapController()->GetMapModel()->GetHeader()->scaleFactor);
	}
}
