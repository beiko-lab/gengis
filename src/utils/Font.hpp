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

#ifndef _GENGIS_FONT_
#define _GENGIS_FONT_

#include "../core/Precompiled.hpp"
#include "../core/App.hpp"
#include <FTGL/ftgl.h>

class GenGIS::Font;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::Font * t, const unsigned int file_version);
	}
}

namespace GenGIS
{

	/**
	* @brief Render TrueType fonts on OpenGL canvas.
	*/
	class Font : private Uncopyable 
	{
	public:
		/**
		* @brief Build a new font.
		* @param fontFile TrueType font file (e.g., times.ttf, arial.ttf).
		* @param size Size of font.
		* @return True if an error has occured.
		* @throw IOException If font file can not be opened.
		* @throw Exception If font can not be set to specified size.
		*/
		explicit Font(const std::string& fontFile, uint size);

		/** Destructor. */
		~Font();

		/**
		* @brief Render text to current OpenGL canvas.
		* @param text Text to render.
		* @param x,y Location to render bottom, left corner of text.
		*/
		void Render(const std::string& text, uint x, uint y);

		/**
		* @brief Render text to current OpenGL canvas.
		* @param text Text to render.
		* @param x,y Location to render bottom, left corner of text.
		*/
		void Render(const std::wstring& text, uint x, uint y);

		/**
		* @brief Set size of font.
		* @param size Desired size of font.
		* @throw Exception If font can not be set to specified size.
		*/
		void Size(uint size);

		/**
		* @brief Get size of font.
		* @return Size of font.
		*/
		uint Size() const { return m_font->FaceSize(); }

		/**
		* @brief Get descender amount for font.
		* @return Descender of font.
		*/
		float Descender() { return m_font->Descender(); }

		/**
		* @brief Get ascender amount for font.
		* @return Ascender of font.
		*/
		float Ascender() { return m_font->Ascender(); }

		/**
		* @brief Get bounding box of a string.
		* @param text Text to determine bounding box of.
		* @return Bounding box.
		*/
		Box2D BoundingBox(const std::string& text);

		/**
		* @brief Get bounding box of a string.
		* @param text Text to determine bounding box of.
		* @return Bounding box.
		*/
		Box2D BoundingBox(const std::wstring& text);

		/**
		* @brief Type of font (e.g., Times.ttf, Arial.ttf).
		* @return TrueType filename.
		*/
		std::string& FontFile() { return m_fontFile; }

		/**
		* @brief Type of font (e.g., Times.ttf, Arial.ttf) (read-only).
		* @return TrueType filename.
		*/
		const std::string& FontFile() const { return m_fontFile; }

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const Font * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	private:
		/** TTF font file. */
		std::string m_fontFile;

		/** Smart pointer to a FTFont. */
		FTTextureFont* m_font;
	};

}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const Font * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			ar << t->m_fontFile;   // std::string
			uint size = t->Size(); // uint
			ar << size;
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, Font * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			std::string _fontFile;
			ar >> _fontFile;

			uint size;
			ar >> size;

			wxString path = App::Inst().GetExeDir();
			std::string fontFile = std::string(path.begin(), path.end()) + "fonts/" + _fontFile;

			::new(t)Font(fontFile, size);
		}
	}
}

#endif
