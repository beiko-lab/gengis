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

#ifndef _GENGIS_GEOMETRY_H_
#define _GENGIS_GEOMETRY_H_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/**
	* @brief Functions for calculating basic geometric relationships.
	*/
	class Geometry
	{
	public:
		static float Sine(float rad);

		/**
		* @brief Find intersection between a vertical and diagonal line.
		* @param line Diagonal line.
		* @param x X-coordinate of vertical line.
		* @return Point of intersections.
		* @throw ZeroDivisionException If a division by zero occurs.
		*/
		static Point2D VerticalIntersect(const Line2D& line, const double x);

		/**
		* @brief Find intersection between a horizontal and diagonal line.
		* @param line Diagonal line.
		* @param y Y-coordinate of horizontal line.
		* @return Point of intersections.
		* @throw ZeroDivisionException If a division by zero occurs.
		*/
		static Point2D HorizontalIntersect(const Line2D& line, const double y);

		/**
		* @brief Find intersection between two lines.
		* @param line Diagonal line 1.
		* @param line Diagonal line 2.
		* @return Point of intersections between lines.
		*/
		static Point2D Intersect(const Line2D& line1, const Line2D& line2);

		/**
		* @brief Find the closest point on a line, L, to a given point, P. Note: the point and line are assumed to by in the (x,z) plane.
		* @param line Line L.
		* @param point Point P.
		* @param closestPoint Set to the closest point on L to P.
		* @return True if closest point on the line is on the line segment specified by L, otherwise false.
		*/
		static bool ClosestPointToLine(const Line3D& line, const Point3D& point, Point3D& closestPoint);

		/**
		* @brief Calculate the distance between two points.
		* @param pt1 First point.
		* @param pt2 Second point.
		* @return Distance between points.
		*/
		static double Distance(const Point3D& pt1, const Point3D& pt2);

		/**
		* @brief Check if a set of 3 points are colinear. Note: the point and line are assumed to by in the (x,z) plane.
		* @param pt1 First point.
		* @param pt2 Second point.
		* @param pt3 Third point.
		* @return True if points are colinear
		*/
		static bool IsColinear(const Point3D& pt1, const Point3D& pt2, const Point3D& pt3);

		/**
		* @brief Calculate length of line
		* @param line Line to calculate length of
		* @return Distance between end points of line.
		*/
		static double LineLength(const Line3D& line) { return Distance(line.start, line.end); }

		/**
		* @brief Signed angle between given line and x-axis. Positive angles are CCW. Note: the point and line are assumed to by in the (x,z) plane.
		* @param line Line of interest. 
		* @return Signed angle (between -PI and PI).
		*/
		static double AngleToX(const Line3D& line);

		/** 
		* @brief Find mid-point of a line.
		* @param line Line of interest.
		* @return Mid-point of line.
		*/
		static Point3D MidPoint(const Line3D& line);

		/**
		* @brief Normal to line. The normal is on the RHS as one goes from the start to end points of the line. Note: the line is assumed to be in the (x,z) plane.
		* @param line Line of interest. 
		* @return Normal to line.
		*/
		static Point3D NormalToLine(const Line3D& line);

		/**
		* @brief Determine if a point is within a triangle. Note: the point and triangle are assumed to be in the (x, y) plane.
		* @param pt Point to test.
		* @param x1 First vertex of triangle.
		* @param x2 Second vertex of triangle.
		* @param x3 Thrid vertex of triangle.
		* @return True if the point is contained in the triangle.
		*/
		static bool PointInTriangleXY(const Point3D& pt, const Point3D& v1, const Point3D& v2, const Point3D& v3);

		/**
		* @brief Determine if a point is within a polygon. Note: the point and polygon are assumed to be in the (x, y) plane.
		* @param pt Point to test.
		* @param polygon Vector of points defining polygon. All points should be specified only once.
		* @return True if the point is contained in the polygon.
		*/
		static bool PointInPolygonXY(const Point3D& pt, const std::vector<Point3D>& polygon);

		/**
		* @brief Determine signed angle between two vectors Note: the vectors are assumed to be in the (x, z) plane.
		* @param origin Origin of vectors.
		* @param p1 End of vector 1.
		* @param p2 End of vector 2.
		* @return Signed angle (ie. 0 to PI or -PI) between vectors.
		*/
		static double SignedAngleXZ(const Point3D& origin, const Point3D& p1, const Point3D& p2);

		/**
		* @brief Determine closest point on ellipse to a given point. Note: the ellipse and point are assumed to be in the (x, z) plane.
		* @param pt Point of interest.
		* @param xRadius Radius of ellipse along x-axis.
		* @param zRadius Radius of ellipse along z-axis.
		* @param dEpsilon Tolerance for Newton's method.
		* @param iMax Maximum number of iterations of Newton's method to perform.
		* @param iFinal Number of iterations of Newton's method performed.
		* @param closestX X-coordinate of closest point on ellipse to the given point.
		* @param closestZ z-coordinate of closest point on ellipse to the given point.
		* @return Distance from given point and closest point on ellipse.
		*/
		static double DistancePointEllipseXZ(Point3D& pt, double xRadius, double zRadius,
			const double dEpsilon, const int iMax, int& iFinal, double& closestX, double& closestZ);

		/**
		* @brief Determine intersection between an ellipse and line extending from the ellipse origin. Note: the ellipse and point are assumed to be in the (x, z) plane.
		* @param xRadius Radius of ellipse along x-axis.
		* @param zRadius Radius of ellipse along z-axis.
		* @param lineEnd End point of line.
		* @return Intersection point.
		*/
		static Point3D EllipseOriginLineIntersectXZ(double xRadius, double zRadius, Point3D& lineEnd);

		/**
		* @brief Determine point on ellipse at a given angle. Note: the ellipse is assumed to be in the (x, z) plane.
		* @param xRadius Radius of ellipse along x-axis.
		* @param zRadius Radius of ellipse along z-axis.
		* @param angle Angle from x-axis.
		* @return Point on ellipse at given angle.
		*/
		static Point3D GetEllipsePointAtAngleXZ(double xRadius, double zRadius, double angle);

		/**
		* @brief Find intersection between an ellipse and a line. Note: the ellipse and line are assumed to be in the (x, z) plane.
		* @param xRadius Radius of ellipse along x-axis.
		* @param zRadius Radius of ellipse along z-axis.
		* @param line Line to find intersection with.
		* @param intersectPts Intersection point between line and ellipse (may be 0, 1, or 2 points).
		* @return True if there is 1 or more intersection points.
		*/
		static bool EllipseLineIntersectXZ(double xRadius, double zRadius, Line3D& line, std::vector<Point3D>& intersectPts);

		/**
		* @brief Determine normal vector at a given point to an ellipse in standard position. Note: the ellipse and point are assumed to be in the (x, z) plane.
		* @param xRadius Radius of ellipse along x-axis.
		* @param zRadius Radius of ellipse along z-axis.
		* @param pt Point to determine tangent at.
		* @return Vector indicating tangent direction at the given point.
		*/
		static Point3D EllipseNormalXZ(double xRadius, double zRadius, const Point3D& canonicalPt);

		/** 
		* @brief Determine if the smallest angle between a start and stop angle is in the CW or CCW direction.
		* @param start First angle of interest.
		* @param end Second angle of interest.
		* @return -1/1 if the smallest angle going from start to end is in the CW/CCW direction.
		*/
		static int SmallestAngleDir(double start, double end);

		/** 
		* @brief Find angle bisector of two angles.
		* @param angle1 First angle.
		* @param angle2 Second angle.
		* @return Angle halfway between angle1 and angle2.
		*/
		static double AngleBisector(double angle1, double angle2);

		/**
		* @brief Find the angle bisector of a set of angles. For set with more
		*					than 2 angles, the largest angle between any two adjacent angles is identified.
		*					The angle bisector is than set half-way between these two angles such that it 
		*					is in the "middle" of the other angles.
		* @param angles Angles to find mid-point of (all angles must be between 0 and 2PI)
		* @param ccwAngles Order indices going in CCW order from the two angles used to calculate the bisector.
		* @return Angle bisector of set of angles.
		*/
		static double AngleBisector(std::vector<double> angles, std::vector<uint>& ccwIndices);

	private:
		/**
		* @brief Helper function for determine closest point on ellipse to a given point. 
		* @param pt Point of interest.
		* @param xRadius Radius of ellipse along x-axis.
		* @param zRadius Radius of ellipse along z-axis.
		* @param dEpsilon Tolerance for Newton's method.
		* @param iMax Maximum number of iterations of Newton's method to perform.
		* @param iFinal Number of iterations of Newton's method performed.
		* @param closestX X-coordinate of closest point on ellipse to the given point.
		* @param closestZ z-coordinate of closest point on ellipse to the given point.
		* @return Distance from given point and closest point on ellipse.
		*/
		static double DistancePointEllipseSpecialXZ(Point3D& pt, double xRadius, double zRadius,
			const double dEpsilon, const int iMax, int& iFinal, double& closestX, double& closestZ);
	};

}

#endif
