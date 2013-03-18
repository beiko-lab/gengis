//=======================================================================
// Author: Donovan Parks
// Modified by: Timothy Mankowski (2011)
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

#ifndef _GENGIS_COLOR_
#define _GENGIS_COLOR_

#include "../core/Precompiled.hpp"

namespace GenGIS
{
	/**
	* @class Colour
	* @brief Specify an RGBA colour.
	*/
	class Colour 
	{
	public:
		/** Constructor. */
		explicit Colour();

		/** 
		* @brief Constructor. Specify colour components between 0 and 255. 
		* @param r,g,b Colour channels (0 to 255).
		* @param alpha Alpha channel (0 to 255).
		*/
		explicit Colour( int r, int g, int b, int alpha = 255 );

		/** 
		* @brief Constructor. Specify colour components between 0 and 1. 
		* @param r,g,b Colour channels (0 to 1).
		* @param alpha Alpha channel (0 to 1).
		*/
		explicit Colour( float r, float g, float b, float alpha = 1.0f );

		/** 
		* @brief Constructor. 
		* @param colour wxWidgets colour object.
		*/
		explicit Colour( const wxColour& colour );

		/** Copy constructor. */
		Colour(const Colour& rhs);

		/** Destructor. */
		~Colour() {}

		/** Assignment operator. */
		Colour& operator=(const Colour& rhs);

		/** Equality operator. */
		bool operator==(const Colour &other) const;

		/** Inequality operator. */
		bool operator!=(const Colour &other) const;

		/** Set red colour channel (0 to 1). */
		void SetRed( float r ) { m_red = r; }

		/** Set blue colour channel (0 to 1). */
		void SetBlue( float b ) { m_blue = b; }

		/** Set green colour channel (0 to 1). */
		void SetGreen( float g ) { m_green = g; }	

		/** Set red colour channel (0 to 1). */
		void SetAlpha( float a ) { m_alpha = a; }

		/** Get red colour channel (0 to 1). */
		float GetRed() const { return m_red; }  

		/** Get green colour channel (0 to 1). */
		float GetGreen() const { return m_green; }

		/** Get blue colour channel (0 to 1). */
		float GetBlue() const { return m_blue; }

		/** Get alpha colour channel (0 to 1). */
		float GetAlpha() const { return m_alpha; }

		/** Get red colour channel (0 to 255). */
		int GetRedInt() const;

		/** Get green colour channel (0 to 255). */
		int GetGreenInt() const;

		/** Get blue colour channel (0 to 255). */
		int GetBlueInt() const;

		/** Get alpha channel (0 to 255). */
		int GetAlphaInt() const;

		/** Create a random colour with alpha = 1. */
		void SetRndColour();				

		/** 
		* @brief Create a random colour with a specific alpha channel value. 
		* @param alpha Value of alpha channel.
		*/
		void SetRndColour( float alpha );

		/** Set RGB values (0 to 1). */
		void SetRGB( float r, float g, float b );

		/** Set RGB values (0 to 255). */
		void SetRGB( int r, int g, int b );

		/** Set RGBA values (0 to 1). */
		void SetRGBA( float r, float g, float b, float alpha );

		/** Interpolate colour. */
		static Colour Interpolate(const Colour& colour1, const Colour& colour2, float curValue, float startValue, float endValue); 

		/** Interpolate colour. */
		static Colour Interpolate(ColourPtr colour1, ColourPtr colour2, float curValue, float startValue, float endValue); 

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

		/** Red colour channel. */
		float m_red;

		/** Blue colour channel. */
		float m_blue;

		/** Green colour channel. */
		float m_green;

		/** Alpha channel. */
		float m_alpha;
	};

}

#endif
