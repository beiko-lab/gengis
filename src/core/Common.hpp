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

#ifndef _GENGIS_COMMON_
#define _GENGIS_COMMON_

#pragma message("*** Compiling Common.hpp: This should happen once on a full build if precompiled headers are setup properly. ***\n")

#include "../utils/Point3D.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

namespace GenGIS
{

//--- Common Data Structures --------------------------------------------------

/** Projected coordinates. */
typedef struct sGEO_COORD 
{
	/** Constructor. */
	sGEO_COORD(float _easting = 0, float _northing = 0): easting(_easting), northing(_northing) {}

	/** Equality operator. */
	bool operator==(const sGEO_COORD& geoCoord) const { return (easting==geoCoord.easting && northing==geoCoord.northing); }

	/** Easting coordinate. */
	float easting;

	/** Northing coordinate. */
	float northing;
	
	/** Serialization for struct. */
//	template <typename Archive>
//	void serialize(Archive& ar, const unsigned int version)
//	{
//		ar & easting;  // float
//		ar & northing; // float
//	}
} GeoCoord;

/** 2D Bounding Box. */
typedef struct sBOX_2D 
{
	/** Constructor. */
	sBOX_2D(double _x = 0, double _y = 0, double _dx = 0, double _dy = 0): x(_x), y(_y), dx(_dx), dy(_dy) {}

	/** Width of box. */
	double Width() { return dx - x; }

	/** Height of box. */
	double Height() { return dy - y; }

	/** Lower, left x-coordinate of box. */
	double x;

	/** Lower, left y-coordinate of box. */
	double y;

	/** Width of bounding box. */
	double dx;

	/** Height of bounding box. */
	double dy;

	/** Serialization for struct. */
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & x;  // float
		ar & y;  // float
		ar & dx; // float
		ar & dy; // float
	}
} Box2D;

/** General 2D point. */
typedef struct sPOINT_2D
{
	/** Constructor. */
	sPOINT_2D(float _x = 0, float _y = 0): x(_x), y(_y) {}

	/** x-coordinate of point. */
	float x;

	/** y-coordinate of point. */
	float y;

	/** Serialization for struct. */
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & x; // float
		ar & y; // float
	}
} Point2D;

/** General 2D line. */
typedef struct sLINE_2D
{
	/** Constructor. */
	sLINE_2D(Point2D _start = Point2D(0,0), Point2D _end = Point2D(0,0)): start(_start), end(_end) {}

	/** 
	 * @brief Get point from line
	 * @param index Line to get (index = 0 for start point, index = anything except 0 for end point)
	 */
	const Point2D& GetPoint(int index) const
	{
		if(index == 0)
			return start;
		else
			return end;
	}

	/** Start point of line. */
	Point2D start;

	/** End point of line. */
	Point2D end;
	
	/** Serialization for struct. */
//	template <typename Archive>
//	void serialize(Archive& ar, const unsigned int version)
//	{
//		ar & start; // Point2D
//		ar & end;   // Point2D
//	}
} Line2D;

/** General 3D line. */
typedef struct sLINE_3D
{
	/** Constructor. */
	sLINE_3D(const Point3D& _start = Point3D(0,0,0), const Point3D& _end = Point3D(0,0,0)): start(_start), end(_end) {}

	/** 
	 * @brief Get point from line
	 * @param index Line to get (index = 0 for start point, index = anything except 0 for end point)
	 */
	const Point3D& GetPoint(int index) const
	{
		if(index == 0)
			return start;
		else
			return end;
	}

	/**
	 * @brief Calculate a line this is offset from this one.
	 * @param offset Point, P, indicate amount to offset in each direction.
	 * @return New line offset from this one by the amount specified in P.
	 */
	sLINE_3D Offset(const Point3D& pt) const
	{
		sLINE_3D offsetLine;
		offsetLine.start = start.Offset(pt);
		offsetLine.end = end.Offset(pt);		

		return offsetLine;
	}

	/** Start point of line. */
	Point3D start;

	/** End point of line. */
	Point3D end;

	/** General purpose id that can be used to associate start of line with another element with the same id. */
	std::wstring startId;

	/** General purpose id that can be used to associate end of line with another element with the same id. */
	std::wstring endId;
	
	/** Serialization for struct. */
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & start;   // Point3D
		ar & end;     // Point3D
		ar & startId; // std::wstring
		ar & endId;   // std::wstring
	}
} Line3D;

/** General 3D polyline. */
typedef struct sPOLYLINE_3D
{
	/** Constructor. */
	sPOLYLINE_3D() {}

	/** Get first point of polyline. */
	const Point3D& GetFirstPt() const { return pts.at(0); }

	/** Get point from polyline. */
	const Point3D& GetPoint(int index) const { return pts.at(index); }

	/** Get last point of polyline. */
	const Point3D& GetLastPt() const { return pts.at(pts.size()-1); }

	/** Get length of polyline. */
	float GetLength() const 
	{
		float length = 0;
		for(uint i = 0; i < pts.size()-1; ++i)
		{
			float dx = pts.at(i).x - pts.at(i+1).x;
			float dy = pts.at(i).y - pts.at(i+1).y;
			float dz = pts.at(i).z - pts.at(i+1).z;
			length += sqrt(dx*dx+dy*dy+dz*dz);
		}

		return length;
	}

	/** Points defining lines of polyline. */
	std::vector<Point3D> pts;
	
	/** Serialization for struct. */
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & pts; // std::vector<Point3D>
	}
} PolyLine3D;

/** Structure used to sort numerical strings. */
typedef struct sNUMERIC_STRING_SORTER
{
	sNUMERIC_STRING_SORTER(double _numericStr, uint _index): numericStr(_numericStr), index(_index) {}

	static bool NumericStringPredicate(sNUMERIC_STRING_SORTER elem1, sNUMERIC_STRING_SORTER elem2)
	{
		 return elem1.numericStr < elem2.numericStr;
	}

	double numericStr;
	uint index;
	
	/** Serialization for struct. */
//	template <typename Archive>
//	void serialize(Archive& ar, const unsigned int version)
//	{
//		ar & numericStr; // double
//		ar & index;      // uint
//	}
} NumericStringSorter;

//--- Common macros

// Allows for more natural iteration over a sequence
#define foreach BOOST_FOREACH

//--- Common Numeric Values ---------------------------------------------------

#define PI 3.14159265f
#define PI2 (2*PI)
#define PI_SQUARED (PI*PI)
#define PI_BY_2 (PI / 2.0f)
#define EPSILON 1e-6
#define DEG_TO_RAD (PI/180.0f)
#define RAD_TO_DEG (180.0f/PI)
#define TAN_30	0.5773502692f
#define COS_30	0.8660254038f
#define SIN_30	0.5f


//--- Useful flags ---

#define NOT_SET INT_MAX

#define SESSION_SAVED true
#define SESSION_NOT_SAVED false

#define OPEN_DIALOG true
#define DONT_OPEN_DIALOG false

//--- Globally used program-specific constants ---

#define TERRAIN_START_DEPTH 0.1f

}

#endif
