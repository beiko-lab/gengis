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

#ifndef _GENGIS_POINT_3D_
#define _GENGIS_POINT_3D_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/**
	* @brief General 3D point. Also used as a vector depending on context.
	*/
	class Point3D
	{
	public:
		/** Constructor. */
		explicit Point3D(double _x = 0, double _y = 0, double _z = 0): x(_x), y(_y), z(_z) {}

		/** Desturctor. */
		~Point3D() {}

		/**
		* @brief Calculate a point this is offset from this one.
		* @param offset Point, P, indicate amount to offset in each direction.
		* @return New point the is offset from this one by the amount specified in P.
		*/
		Point3D Offset(const Point3D& pt) const;

		/** Length of point (vector). */
		float Length() const { return sqrt(SquaredLength()); }

		/** Squared length of point (vector). */
		float SquaredLength() const { return (x*x + y*y + z*z); }

		/** Normalize point (vector) to unit length. */
		const Point3D& Normalize();

		/** Cross product. */
		Point3D Cross(const Point3D& pt) const;

		/** Dot product. Can also use * operator. */
		double Dot(const Point3D& pt) const { return x * pt.x + y * pt.y + z * pt.z; }

		/** Calculate the smallest angle between two vectors. */
		float Angle(const Point3D& pt) const;

	public:
		/** Compound addition operator. */
		Point3D& operator+=(const Point3D& pt)
		{
			x += pt.x;
			y += pt.y;
			z += pt.z;
			return *this;
		}

		/** Compound subtraction operator. */
		Point3D& operator-=(const Point3D& pt)
		{
			x -= pt.x;
			y -= pt.y;
			z -= pt.z;
			return *this;
		}

		/** Add two points. */
		const Point3D operator+(const Point3D& pt) const { return Point3D(x + pt.x, y + pt.y, z + pt.z); }

		/** Subtract two points. */
		const Point3D operator-(const Point3D& pt) const { return Point3D(x - pt.x, y - pt.y, z - pt.z); }

		/** Inner dot product. */
		const float operator*(const Point3D& pt) const { return x * pt.x + y * pt.y + z * pt.z; }

		/** Scalar multiplication. */
		friend Point3D operator*(float c, Point3D pt) { return Point3D(c*pt.x, c*pt.y, c*pt.z); }

		/** Scalar multiplication. */
		friend Point3D operator*(const Point3D& pt, float c) { return Point3D(c*pt.x, c*pt.y, c*pt.z); }

		/** Scalar division. */
		friend Point3D operator/(const Point3D& pt, float c) { return Point3D(pt.x/c, pt.y/c, pt.z/c); }

		/** Unary negation. */
		const Point3D operator-() const { return Point3D(-x, -y, -z); }

		/** Equality operator. */
		bool operator==(const Point3D& pt) const { return (x==pt.x && y==pt.y && z==pt.z); }


		/** Not equal operator. */
		bool operator!=(const Point3D& pt) const { return !(*this == pt); }

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	public:
		/** x position of point. */
		double x;

		/** y position of point. */
		double y;

		/** z position of point. */
		double z;
	};

}

#endif
