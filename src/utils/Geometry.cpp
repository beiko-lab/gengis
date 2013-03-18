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

#include "../utils/Geometry.hpp"

using namespace GenGIS;

const double eps = 1e-6;

float Geometry::Sine(float rad)
{
	// Code from: http://www.devmaster.net/forums/showthread.php?t=5784
	const float B = 4/PI;
	const float C = -4/(PI_SQUARED);

	float y = B * rad + C * rad * abs(rad);

	const float Q = 0.775f;
	const float P = 0.225f;

	y = Q * y + P * y * abs(y);

	return y;
}

Point2D Geometry::VerticalIntersect(const Line2D& line, const double x)
{
	assert((line.end.x-line.start.x) != 0);

	double slope = (line.end.y - line.start.y) / (line.end.x-line.start.x);
	double run = x - line.start.x;

	double yInter = line.start.y+slope*run;

	return Point2D((float)x, (float)yInter);
}

Point2D Geometry::HorizontalIntersect(const Line2D& line, const double y)
{
	assert((line.end.y - line.start.y) != 0);

	double vertSlope = (line.end.x-line.start.x) / (line.end.y - line.start.y);
	double vertRun = y - line.start.y;

	double xInter = line.start.x+vertSlope*vertRun;

	return Point2D((float)xInter, (float)y);
}

Point2D Geometry::Intersect(const Line2D& line1, const Line2D& line2)
{
	// express line 1 in form: Ax+By=C
	double A1 = line1.end.y - line1.start.y;
	double B1 = line1.start.x - line1.end.x;
	double C1 = A1*line1.start.x + B1*line1.start.y;

	// express line 2 in form: Ax+By=C
	double A2 = line2.end.y - line2.start.y;
	double B2 = line2.start.x - line2.end.x;
	double C2 = A2*line2.start.x + B2*line2.start.y;

	double det = A1*B2 - A2*B1;
	if(det != 0)
	{
		double x = (B2*C1 - B1*C2)/det;
		double y = (A1*C2 - A2*C1)/det;

		return Point2D((float)x, (float)y);
	}

	return Point2D(0, 0);
}

bool Geometry::ClosestPointToLine(const Line3D& line, const Point3D& point, Point3D& closestPoint)
{
	/*
	[ Source unknown ]

	Subject 1.02: How do I find the distance from a point to a line?


			Let the point be C (Cx,Cy) and the line be AB (Ax,Ay) to (Bx,By).
			Let P be the point of perpendicular projection of C on AB.  The parameter
			r, which indicates P's position along AB, is computed by the dot product 
			of AC and AB divided by the square of the length of AB:
	    
			(1)     AC dot AB
					r = ---------  
							||AB||^2
	    
			r has the following meaning:
	    
					r=0      P = A
					r=1      P = B
					r<0      P is on the backward extension of AB
					r>1      P is on the forward extension of AB
					0<r<1    P is interior to AB
	*/

	double dx = line.end.x-line.start.x;
	double dz = line.end.z-line.start.z;

	double r_numerator = (point.x-line.start.x)*dx + (point.z-line.start.z)*dz;
	double r_denomenator = dx*dx + dz*dz;
	double r = r_numerator / r_denomenator;

	closestPoint.x = (float)(line.start.x + r*dx);
	closestPoint.y = 0.0f;
	closestPoint.z = (float)(line.start.z + r*dz);

	return (r >= 0 && r <= 1);
}

double Geometry::Distance(const Point3D& pt1, const Point3D& pt2)
{
	double dx = pt1.x - pt2.x;
	double dy = pt1.y - pt2.y;
	double dz = pt1.z - pt2.z;

	return sqrt(dx*dx + dy*dy + dz*dz);
}

bool Geometry::IsColinear(const Point3D& pt1, const Point3D& pt2, const Point3D& pt3)
{
	Point3D closestPoint;
	ClosestPointToLine(Line3D(pt1, pt2), pt3, closestPoint);

	double dx = pt3.x - closestPoint.x;
	double dz = pt3.z - closestPoint.z;

	return (dx*dx + dz*dz) < EPSILON*EPSILON;
}

double Geometry::AngleToX(const Line3D& line)
{
	Point3D vec = line.end - line.start;
	double angleToX = vec.Angle(Point3D(1, 0, 0));
	if(vec.z < 0)
	{
		// Angle() returns the smallest angle between two vectors. if the line goes in 
		// the positive z direction than it is a CW angle so should be negative
		angleToX = -angleToX;
	}	

	return angleToX;
}

Point3D Geometry::MidPoint(const Line3D& line)
{	
	return line.start + 0.5*(line.end-line.start);
}

Point3D Geometry::NormalToLine(const Line3D& line)
{
	Point3D lineDir = (line.start - line.end);
	lineDir.Normalize();
	Point3D normDir = lineDir.Cross(Point3D(0,1,0));
	normDir.Normalize();

	return normDir;
}

bool Geometry::PointInTriangleXY(const Point3D& pt, const Point3D& v1, const Point3D& v2, const Point3D& v3)
{
	Point3D vec1 = pt - v1;
	Point3D vec2 = pt - v2;
	Point3D vec3 = pt - v3;

	vec1.z = vec2.z = vec3.z = 0;

	double angle1 = vec1.Angle(vec2);
	double angle2 = vec2.Angle(vec3);
	double angle3 = vec3.Angle(vec1);

	double sum = angle1 + angle2 + angle3;

	return (sum > PI2-eps && sum < PI2+eps);
}

bool Geometry::PointInPolygonXY(const Point3D& pt, const std::vector<Point3D>& polygon)
{
	// Taken from: http://local.wasp.uwa.edu.au/~pbourke/geometry/insidepoly/
	// Uses the crossing number (even-odd rule) algorithm
	unsigned int i, j;
	bool bInPolygon = false;
	for(i = 0, j = polygon.size()-1; i < polygon.size(); j = i++) 
	{
		if ((((polygon.at(i).y <= pt.y) && (pt.y < polygon.at(j).y)) ||
			((polygon.at(j).y <= pt.y) && (pt.y < polygon.at(i).y))) &&
			(pt.x < (polygon.at(j).x - polygon.at(i).x) * (pt.y - polygon.at(i).y) / (polygon.at(j).y - polygon.at(i).y) + polygon.at(i).x))
		{
			bInPolygon = !bInPolygon;
		}
	}

	return bInPolygon;
}

double Geometry::SignedAngleXZ(const Point3D& origin, const Point3D& p1, const Point3D& p2)
{
	Point3D vec1 = p1 - origin;
	vec1.y = 0;
	vec1.Normalize();

	Point3D vec2 = p2 - origin;
	vec2.y = 0;
	vec2.Normalize();

	double dot = vec1.Dot(vec2);
	double perpDot = vec1.x * vec2.z - vec1.z * vec2.x;

	return atan2(perpDot, dot);
}



double Geometry::DistancePointEllipseSpecialXZ(Point3D& pt, double xRadius, double zRadius,
	const double dEpsilon, const int iMax, int& iFinal, 
	double& closestX, double& closestZ)
{
	// http://www.geometrictools.com/Documentation/DistancePointToEllipse2.pdf

	// initial guess
	double dT = zRadius*(pt.z - zRadius);
	// Newton’s method
	int i;
	for (i = 0; i < iMax; i++)
	{
		double dTpASqr = dT + xRadius*xRadius;
		double dTpBSqr = dT + zRadius*zRadius;
		double dInvTpASqr = 1.0/dTpASqr;
		double dInvTpBSqr = 1.0/dTpBSqr;
		double dXDivA = xRadius*pt.x*dInvTpASqr;
		double dYDivB = zRadius*pt.z*dInvTpBSqr;
		double dXDivASqr = dXDivA*dXDivA;
		double dYDivBSqr = dYDivB*dYDivB;
		double dF = dXDivASqr + dYDivBSqr - 1.0;
		if (dF < dEpsilon)
		{
			// F(t0) is close enough to zero, terminate the iteration
			closestX = dXDivA*xRadius;
			closestZ = dYDivB*zRadius;
			iFinal = i;
			break;
		}

		double dFDer = 2.0*(dXDivASqr*dInvTpASqr + dYDivBSqr*dInvTpBSqr);
		double dRatio = dF/dFDer;
		if (dRatio < dEpsilon)
		{
			// t1-t0 is close enough to zero, terminate the iteration
			closestX = dXDivA*xRadius;
			closestZ = dYDivB*zRadius;
			iFinal = i;
			break;
		}

		dT += dRatio;
	}

	if (i == iMax)
	{
		// method failed to converge, let caller know
		iFinal = -1;
		return std::numeric_limits<float>::max();
	}

	double dDelta0 = closestX - pt.x, dDelta1 = closestZ - pt.z;
	return sqrt(dDelta0*dDelta0 + dDelta1*dDelta1);
}


double Geometry::DistancePointEllipseXZ(Point3D& pt, double xRadius, double zRadius,
	const double dEpsilon, const int iMax, int& iFinal, 
	double& closestX, double& closestZ)
{
	// Source: http://www.geometrictools.com/Documentation/DistancePointToEllipse2.pdf

	// special case of circle
	if (fabs(xRadius-zRadius) < dEpsilon)
	{
		double dLength = sqrt(pt.x*pt.x+pt.z*pt.z);
		return fabs(dLength - xRadius);
	}

	// reflect U = -U if necessary, clamp to zero if necessary
	bool bXReflect;
	if (pt.x > dEpsilon)
	{
		bXReflect = false;
	}
	else if (pt.x < -dEpsilon)
	{
		bXReflect = true;
		pt.x = -pt.x;
	}
	else
	{
		bXReflect = false;
		pt.x = 0.0;
	}

	// reflect V = -V if necessary, clamp to zero if necessary
	bool bYReflect;
	if (pt.z > dEpsilon)
	{
		bYReflect = false;
	}
	else if (pt.z < -dEpsilon)
	{
		bYReflect = true;
		pt.z = -pt.z;
	}
	else
	{
		bYReflect = false;
		pt.z = 0.0;
	}

	// transpose if necessary
	double dSave;
	bool bTranspose;
	if (xRadius >= zRadius)
	{
		bTranspose = false;
	}
	else
	{
		bTranspose = true;
		dSave = xRadius;
		xRadius = zRadius;
		zRadius = dSave;
		dSave = (double)pt.x;
		pt.x = pt.z;
		pt.z = (float)dSave;
	}

	double dDistance;
	if (pt.x != 0.0)
	{
		if (pt.z != 0.0)
		{
			dDistance = Geometry::DistancePointEllipseSpecialXZ(pt, xRadius, zRadius, dEpsilon, iMax,
			            iFinal, closestX, closestZ);
		}
		else
		{
			double dBSqr = zRadius*zRadius;
			if (pt.x < xRadius - dBSqr/xRadius)
			{
				double dASqr = xRadius*xRadius;
				closestX = dASqr*pt.x/(dASqr-dBSqr);
				double dXDivA = closestX/xRadius;
				closestZ = zRadius*sqrt(fabs(1.0-dXDivA*dXDivA));
				double dXDelta = closestX - pt.x;
				dDistance = sqrt(dXDelta*dXDelta+closestZ*closestZ);
				iFinal = 0;
			}
			else
			{
				dDistance = fabs(pt.x - xRadius);
				closestX = xRadius;
				closestZ = 0.0;
				iFinal = 0;
			}
		}
	}
	else
	{
		dDistance = fabs(pt.z - zRadius);
		closestX = 0.0;
		closestZ = zRadius;
		iFinal = 0;
	}

	if (bTranspose)
	{
		dSave = closestX;
		closestX = closestZ;
		closestZ = dSave;
	}

	if (bYReflect)
	{
		closestZ = -closestZ;
	}

	if (bXReflect)
	{
		closestX = -closestX;
	}

	return dDistance;
}

Point3D Geometry::EllipseOriginLineIntersectXZ(double xRadius, double zRadius, Point3D& lineEnd)
{
	double a = xRadius;
	double b = zRadius;
	double denom = sqrt((b*lineEnd.x)*(b*lineEnd.x) + (a*lineEnd.z)*(a*lineEnd.z));
	double x = (a*b*lineEnd.x)/denom;
	double z = (a*b*lineEnd.z)/denom;

	return Point3D((float)x, 0, (float)z);
}

Point3D Geometry::GetEllipsePointAtAngleXZ(double xRadius, double zRadius, double angle)
{
	// get a distance that is longer than either radii of the ellipse
	double dist = xRadius + zRadius;

	// find intersection between line from (0,0) to (xx,zz) and the ellipse 
	double xx = cos(angle)*dist;
	double zz = sin(angle)*dist;
  Point3D point((float)xx, 0, (float)zz);
	return Geometry::EllipseOriginLineIntersectXZ(xRadius, zRadius, point);
}

double Geometry::AngleBisector(double angle1, double angle2)
{
	if(angle1 < 0) 
		angle1 = PI2 + angle1;
	
	if(angle2 < 0) 
		angle2 = PI2 + angle2;
	
	double midAngle;
	if(angle1 > angle2)
	{
		double deltaAngle = angle1 - angle2;
		if(deltaAngle > PI) 
			deltaAngle = deltaAngle - PI2;
		midAngle = angle2 + deltaAngle * 0.5;
	}
	else
	{
		double deltaAngle = angle2 - angle1;
		if(deltaAngle > PI) 
			deltaAngle = deltaAngle - PI2;
		midAngle = angle1 + deltaAngle * 0.5;
	}

	return midAngle;
}

Point3D Geometry::EllipseNormalXZ(double xRadius, double zRadius, const Point3D& canonicalPt)
{
	double x = (2*canonicalPt.x) / (xRadius*xRadius);
	double z = (2*canonicalPt.z) / (zRadius*zRadius);
	Point3D normal((float)x, 0, (float)z);
	normal.Normalize();

	return normal;     
}

int Geometry::SmallestAngleDir(double start, double end)
{
	int dir = 1;
	double dAngle = start - end;
	if(dAngle > 0 && dAngle < PI)
			dir = -1;
	else if(dAngle > 0 && dAngle < -PI)
			dir = -1;

	return dir;
}

double Geometry::AngleBisector(std::vector<double> angles, std::vector<uint>& ccwIndices)
{
	// Set angle bisector so it at the "middle angle" of the set of angles. For 
	// sets with 2 angles this is simple the angle bisector. For set with more
	// than 2 angles, the largest angle between any two adjacent angles is identified.
	// The angle bisector is than set half-way between these two angles such that it 
	// is in the "middle" of the other angles.

	assert(angles.size() > 1);

	if(angles.size() == 2)
	{
		int dir = SmallestAngleDir(angles.at(0), angles.at(1));
		if(dir == 1)
		{
			ccwIndices.push_back(0);
			ccwIndices.push_back(1);
		}
		else
		{
			ccwIndices.push_back(1);
			ccwIndices.push_back(0);
		}

		return AngleBisector(angles.at(0), angles.at(1));
	}
	else
	{
		// find largest angle between any two adjacent angles in the set
		sort(angles.begin(), angles.end());

		double maxAngle = 0;
		uint maxIndex;
		for(uint i = 1; i < angles.size(); ++i)
		{
			double angle = angles.at(i) - angles.at(i-1);
			if(angle > maxAngle)
			{
				maxAngle = angle;
				maxIndex = i;

			}
		}

		// check angle between first and last angle in set
		double angle = angles.at(angles.size()-1) - angles.at(0);
		angle = PI2 - angle;
		if(angle > maxAngle)
		{
			maxAngle = angle;
			maxIndex = 0;
		}
	
		// find bisector of angles with largest angle between them
		double dAngle;
		if(maxIndex > 0)
		{			
			dAngle = angles.at(maxIndex) - angles.at(maxIndex-1);
		}
		else
		{
			dAngle = angles.at(angles.size()-1) - angles.at(0);
			dAngle = PI2 - dAngle;
		}

		double bisector = angles.at(maxIndex) - dAngle * 0.5;
		
		// rotate bisector 180 degrees so it is in the "middle" of the angle set
		bisector += PI;
		if(bisector > PI2)
			bisector -= PI2;

		// create list of angles going in CCW direction between angles used to define the bisector
		for(uint i = maxIndex; i < angles.size(); ++i)
		{
			ccwIndices.push_back(i);
		}

		for(uint i = 0; i < maxIndex; ++i)
		{
			ccwIndices.push_back(i);
		}

		return bisector;
	}

	// the compiler has failed if this point is every reached!
	assert(false);
	return 0;
}

bool Geometry::EllipseLineIntersectXZ(double xRadius, double zRadius, Line3D& line, std::vector<Point3D>& intersectPts)
{
	// http://www.codecogs.com/reference/maths/analytical_geometry/the_ellipse.php#sec4

	double slope = (line.end.z - line.start.z) / (line.end.x - line.start.x);
	double zInter = line.start.z - slope*line.start.x;

	double xRadius2 = xRadius*xRadius;
	double zRadius2 = zRadius*zRadius;
	double slope2 = slope*slope;
	double zInter2 = zInter*zInter;

	double a = xRadius2*slope2 + zRadius2;
	double b = 2*xRadius2*slope*zInter;
	double c = xRadius2*(zInter2 - zRadius2);

	double discriminant = b*b - 4*a*c;
	if(discriminant > 0)
	{
		double sqrtRoot = sqrt(discriminant);

		double x = (-b + sqrtRoot) / (2*a);
		double z = slope*x + zInter;
		intersectPts.push_back(Point3D((float)x, 0, (float)z));

		x = (-b - sqrtRoot) / (2*a);
		z = slope*x + zInter;
		intersectPts.push_back(Point3D((float)x, 0, (float)z));

		return true;
	}
	else if(discriminant == 0)
	{
		double x = -b / (2*a);
		double z = slope*x + zInter;
		intersectPts.push_back(Point3D((float)x, 0, (float)z));

		return true;
	}

	return false;
}
