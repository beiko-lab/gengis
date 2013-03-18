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

#include "../utils/Font.hpp"
#include "../utils/ErrorGL.hpp"
#include "../core/App.hpp"
#include "../utils/StringTools.hpp"

using namespace GenGIS;
using std::string;
using std::wstring;


//*** Member Functions***
GenGIS::Font::Font(const string& fontFile, uint size)		
{
	m_fontFile = fontFile.substr(fontFile.find_last_of("\\/")+1, string::npos);

	m_font = new FTTextureFont(fontFile.c_str());

	// check if font opened correctly
	assert(!m_font->Error());

	bool rtn = m_font->FaceSize(size, 96);
	assert(rtn);
}

GenGIS::Font::~Font()
{
	if(m_font != NULL)
	{
		//delete m_font;
		m_font = NULL;
	}
}

template<class Archive>
void GenGIS::Font::serialize(Archive & ar, const unsigned int file_version)
{
	// Needs to be here but does not do anything
}
template void GenGIS::Font::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void GenGIS::Font::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void GenGIS::Font::Render(const string& text, uint x, uint y)
{
	error::ErrorGL::Check();
	m_font->Render(text.c_str(), -1, FTPoint(x, y));
	error::ErrorGL::Check();
}

void GenGIS::Font::Render(const wstring& text, uint x, uint y)
{
	error::ErrorGL::Check();
	m_font->Render(text.c_str(), -1, FTPoint(x, y));
	error::ErrorGL::Check();
}

void GenGIS::Font::Size(uint size)
{
	bool rtn = m_font->FaceSize(size, 96);
	assert(rtn);
}

Box2D GenGIS::Font::BoundingBox(const std::string& text) 
{ 
	FTBBox bbox = m_font->BBox(text.c_str()); 

	return Box2D(bbox.Lower().Xf(), bbox.Lower().Yf(), bbox.Upper().Xf(), bbox.Upper().Yf());
}

Box2D GenGIS::Font::BoundingBox(const std::wstring& text) 
{ 
	FTBBox bbox = m_font->BBox(text.c_str()); 

	return Box2D(bbox.Lower().Xf(), bbox.Lower().Yf(), bbox.Upper().Xf(), bbox.Upper().Yf());
}
