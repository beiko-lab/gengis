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

#include "../utils/Point3D.hpp"

using namespace GenGIS;

template<class Archive>
void Point3D::serialize(Archive & ar, const unsigned int file_version)
{
	ar & x; // float
	ar & y; // float
	ar & z; // float
}
template void Point3D::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void Point3D::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

const Point3D&  Point3D::Normalize() 
{                     
	float len = Length();
	if (len == 0) 
		return *this; 

	x /= len;
	y /= len;
	z /= len;

	return *this;
}

Point3D Point3D::Offset(const Point3D& pt) const
{
	Point3D offsetPt;
	offsetPt.x = x + pt.x;
	offsetPt.y = y + pt.y;
	offsetPt.z = z + pt.z;

	return offsetPt;
}

Point3D Point3D::Cross(const Point3D& pt) const
{
	Point3D v;
	v.x = y * pt.z - z * pt.y;
	v.y = z * pt.x - x * pt.z;
	v.z = x * pt.y - y * pt.x;

	return v;
}

float Point3D::Angle(const Point3D& pt) const
{
	float len = Length();
	float len2 = pt.Length();

	if(len == 0 || len2 == 0)
		return 0.0;

	float dp = (x * pt.x + y * pt.y + z * pt.z) / (len * len2);
	if (dp >= 1.0) 
		return 0.0f;
  
	if (dp <= -1.0f) 
		return 3.14159265f;

	return acos(dp);
}
