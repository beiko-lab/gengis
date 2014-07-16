#ifndef _GENGIS_POLYGON_
#define _GENGIS_POLYGON_

#include "../core/Precompiled.hpp"

#include "../utils/Point3D.hpp"
#include "../utils/Colour.hpp"


namespace GenGIS
{
	class Polygon
	{
	public: 
		//Constructor: a lastVertexIndex of -1 means all points in the polygon will be drawn
		Polygon(): lastVertexIndex(-1), bVisible(true) {}
		
		//Destructor
		~Polygon(){}

		/* Any polygon with less than 2 points isn't rendered, 2 points are rendered as a line 
		and more than 2 points are rendered as a triangle fan */
		void RenderConvex(bool smooth, float inflation, float scale);

		//Renders the border
		void RenderBorder(float thicknessOfBorder, bool smooth, float inflation, float scale);

		//Scales, inflates and draws the vertices
		void RenderVertices(GLenum mode, bool smooth, float inflation, float scale);

		//Draws polygon with straight lines between the vertices
		void DrawVertices(GLenum mode, int lastPoint);

		//Draws polygon with curved lines between the vertices
		void DrawSmoothVertices(GLenum mode, int lastPoint, std::vector<Point3D> controlPoints);

		//Offsets the points in modifiedVertices by the specified offset
		void OffsetPolygon(float offset, int lastPoint, bool smooth);

		//Offsets a point the given offset along the given slope
		void OffsetAlongSlope(Point3D &point, float slope, float offset);

		/* Method for computing control points for bezier curves from: 
		   http://www.antigrain.com/research/bezier_interpolation/index.html?utm_source=twitterfeed&utm_medium=twitter */
		std::vector<Point3D> CalculateControls(int lastPoint, bool smooth);

		/* Calculates the control points then removes points on the polygon where 
			both sets of adjacent control points make counter-clockwise turns */
		std::vector<Point3D> CalculateClockwiseControlPoints(int &lastPoint, bool smooth);

		//Used to determine if 3 points are clockwise, counter-clockwise, or colinear
		double CCW(Point3D p1, Point3D p2, Point3D p3);

		//Calulates an average point of the polygon vertices up to the specified point
		Point3D AveragePoint(int lastPoint, bool smooth);

		//Calculates the point between p1 and p2 that is the given percentage from p1
		Point3D Midpoint(Point3D p1, Point3D p2, float percentage);

		//Calculates the length of the line between p1 and p2
		double Distance(Point3D p1, Point3D p2);

		//Draws a bezier curve using points p1 and p4 as anchor points and points p2 and p3 as control points
		void DrawBezier(Point3D p1, Point3D p2, Point3D p3, Point3D p4, GLenum mode);

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);


	public:
		bool					bVisible;
		std::vector<Point3D>	originalVertices;
		std::vector<Point3D>	modifiedVertices; 
		Colour					polygonColour;
		int						lastVertexIndex;

	};
}

#endif