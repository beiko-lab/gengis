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

		//Initializes polygons by putting the correct vertices in ModifiedVertices then applies the inflation
		void InitPolygon(float inflation, bool smooth);

		//Renders the border
		void RenderBorder(float thicknessOfBorder, bool smooth, float scale);

		//Scales and draws the vertices
		void RenderVertices(GLenum mode, bool smooth, float scale);

		//Draws polygon with straight lines between the vertices
		void DrawVertices(GLenum mode);

		//Draws polygon with curved lines between the vertices
		void DrawSmoothVertices(GLenum mode);

		//Offsets the points in modifiedVertices by the specified offset
		void OffsetPolygon(float offset, int lastPoint, bool smooth);

		//If smooth, calculates control points for polygons with only 2 points, 
		//if not smooth, then calculates points to add to the polygon
		std::vector<Point3D> CalculateHexPoints(bool smooth);

		/* Method for drawing circle from:
			http://slabode.exofire.net/circle_draw.shtml */
		void InitCircle(Point3D center, float radius, int num_segments);

		//Offsets a point the given offset along the given slope
		void OffsetAlongSlope(Point3D &point, float slope, float offset);

		/* Method for computing control points for bezier curves from: 
		   http://www.antigrain.com/research/bezier_interpolation/index.html?utm_source=twitterfeed&utm_medium=twitter */
		std::vector<Point3D> CalculateControls(bool smooth);

		//Gets a point for each vertex that is a weighted distance between the adjacent vertices
		//Note: these are points Bi in link above CalculateControls(bool smooth);
		std::vector<Point3D> GetInnerPoints();

		/* Calculates the control points then removes points on the polygon where 
			both sets of adjacent control points make counter-clockwise turns */
		std::vector<Point3D> CalculateClockwiseControlPoints(int &lastPoint, bool smooth);

		//Used to determine if 3 points are clockwise, counter-clockwise, or colinear
		double CCW(Point3D p1, Point3D p2, Point3D p3);

		//Calulates an average point of the polygon vertices up to the specified point
		Point3D AveragePoint();

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
		std::vector<Point3D>	controlPoints;
		Colour					polygonColour;
		int						lastVertexIndex;

	};
}

#endif