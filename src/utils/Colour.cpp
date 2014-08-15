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
#include "../utils/Colour.hpp"
#include "assert.h"

using namespace GenGIS;

Colour::Colour( float r, float g, float b, float alpha )
	: m_red(r), m_green(g), m_blue(b), m_alpha(alpha), m_orderingNumber(0)
{
	assert(m_alpha >= 0 && m_alpha <= 1.0); 
	assert(m_red   >= 0 && m_red   <= 1.0);
	assert(m_green >= 0 && m_green <= 1.0);
	assert(m_blue  >= 0 && m_blue  <= 1.0);
}

Colour::Colour( int r, int g, int b, int alpha )
	: m_red(r/255.0f), m_green(g/255.0f), m_blue(b/255.0f), m_alpha(alpha/255.0f), m_orderingNumber(0)
{
	assert(m_alpha >= 0 && m_alpha <= 1.0); 
	assert(m_red   >= 0 && m_red   <= 1.0);
	assert(m_green >= 0 && m_green <= 1.0);
	assert(m_blue  >= 0 && m_blue  <= 1.0);
}

Colour::Colour( const wxColour& colour )
	: m_red(colour.Red()/255.0f), m_green(colour.Green()/255.0f), 
		m_blue(colour.Blue()/255.0f), m_alpha(colour.Alpha()/255.0f), m_orderingNumber(0)
{
	assert(m_alpha >= 0 && m_alpha <= 1.0); 
	assert(m_red   >= 0 && m_red   <= 1.0);
	assert(m_green >= 0 && m_green <= 1.0);
	assert(m_blue  >= 0 && m_blue  <= 1.0);
}

Colour::Colour()
	: m_red(1.0f), m_green(1.0f), m_blue(1.0f), m_alpha(1.0f), m_orderingNumber(0)
{

}

template<class Archive>
void Colour::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_red;				// float
	ar & m_blue;			// float
	ar & m_green;			// float
	ar & m_alpha;			// float
	ar & m_orderingNumber;	// int
}
template void Colour::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void Colour::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

Colour::Colour(const Colour& rhs)
{
	m_red   = rhs.GetRed();
	m_green = rhs.GetGreen();
	m_blue  = rhs.GetBlue();
	m_alpha = rhs.GetAlpha();
	m_orderingNumber = rhs.GetOrderingNumber();
}

Colour& Colour::operator=(const Colour& rhs)
{
	if(this == &rhs)
		return *this;

	m_red   = rhs.GetRed();
	m_green = rhs.GetGreen();
	m_blue  = rhs.GetBlue();
	m_alpha = rhs.GetAlpha();
	m_orderingNumber = rhs.GetOrderingNumber();

	return *this;
}

bool Colour::operator==(const Colour &other) const 
{
	return (this->GetRed()  == other.GetRed())  && (this->GetGreen() == other.GetGreen()) &&
	       (this->GetBlue() == other.GetBlue()) && (this->GetAlpha() == other.GetAlpha()); 
}

bool Colour::operator!=(const Colour &other) const 
{
	return !(*this == other);
}

bool Colour::operator<(const Colour &other) const
{
	return (this->GetOrderingNumber() < other.GetOrderingNumber());
}

void Colour::SetRndColour(float alpha) 
{
	assert(alpha >= 0 && alpha <= 1.0);
	m_alpha = alpha;

	//create random r,g,b values
	m_red   = ( float )rand() / RAND_MAX;
	m_green = ( float )rand() / RAND_MAX;
	m_blue  = ( float )rand() / RAND_MAX;
}

void Colour::SetRndColour() 
{
	m_red   = ( float )rand() / RAND_MAX;
	m_green = ( float )rand() / RAND_MAX;
	m_blue  = ( float )rand() / RAND_MAX;
	m_alpha = ( float )rand() / RAND_MAX;
}

void Colour::SetRGBA( float r, float g, float b, float a ) 
{
	m_red   = r;
	m_green = g;
	m_blue  = b;
	m_alpha = a;
}

void Colour::SetRGB( float r, float g, float b ) 
{
	m_red   = r;
	m_green = g;
	m_blue  = b;
	m_alpha = 1.0f;
}

void Colour::SetRGB( int r, int g, int b ) 
{
	m_red   = r / 255.0f;
	m_green = g / 255.0f;
	m_blue  = b / 255.0f;
	m_alpha = 1.0f;
}

int Colour::GetRedInt() const
{
	return (int)(m_red * 255);
}

int Colour::GetGreenInt() const
{
	return (int)(m_green * 255);
}

int Colour::GetBlueInt() const
{
	return (int)(m_blue * 255);
}

int Colour::GetAlphaInt() const
{
	return (int)(m_alpha * 255);
}

Colour Colour::Interpolate(const Colour& colour1, const Colour& colour2, float curValue, float startValue, float endValue)
{
	float offset = curValue - startValue;
	float range  = endValue - startValue;
	float delta  = offset / range;

	return Colour(
		colour1.GetRed()   + delta * ( colour2.GetRed()   - colour1.GetRed() ), 
		colour1.GetGreen() + delta * ( colour2.GetGreen() - colour1.GetGreen() ),
		colour1.GetBlue()  + delta * ( colour2.GetBlue()  - colour1.GetBlue() ) ,
		colour1.GetAlpha() + delta * ( colour2.GetAlpha() - colour1.GetAlpha() ) );
}

Colour Colour::Interpolate(ColourPtr colour1, ColourPtr colour2, float curValue, float startValue, float endValue)
{
	float offset = curValue - startValue;
	float range  = endValue - startValue;
	float delta  = offset / range;

	return Colour(
		colour1->GetRed()   + delta * ( colour2->GetRed()   - colour1->GetRed() ), 
		colour1->GetGreen() + delta * ( colour2->GetGreen() - colour1->GetGreen() ),
		colour1->GetBlue()  + delta * ( colour2->GetBlue()  - colour1->GetBlue() ) ,
		colour1->GetAlpha() + delta * ( colour2->GetAlpha() - colour1->GetAlpha() ) );
}
