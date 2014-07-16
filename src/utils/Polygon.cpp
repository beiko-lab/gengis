#include "../core/Precompiled.hpp"

#include "../utils/Polygon.hpp"

using namespace GenGIS;

template<class Archive>
void Polygon::serialize(Archive & ar, const unsigned int file_version)
{
	//General
	ar & originalVertices;	//std::vector<Point3D>
	ar & modifiedVertices;	//std::vector<Point3D>
	ar & polygonColour;		//colour
	ar & lastVertexIndex;	//int
	ar & bVisible;			//bool
}
template void Polygon::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void Polygon::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


void Polygon::RenderConvex(bool smooth, float inflation, float scale) {

	if (!bVisible || originalVertices.size() < 2)
		return;	

	RenderVertices(GL_TRIANGLE_FAN, smooth, inflation, scale);

}

void Polygon::RenderBorder(float thicknessOfBorder, bool smooth, float inflation, float scale) {

	if (!bVisible || originalVertices.size() < 2)
		return;
	
	glLineWidth(thicknessOfBorder);
	if (smooth)
		RenderVertices(GL_LINE_STRIP, smooth, inflation, scale);
	else 
		RenderVertices(GL_LINE_LOOP, smooth, inflation, scale);

}

void Polygon::RenderVertices(GLenum mode, bool smooth, float inflation, float scale) {

	/* Calculate stopping point */
	int lastPoint = lastVertexIndex;

	if (lastVertexIndex == -1)
		lastPoint = originalVertices.size();

	//Fills modifiedVertices with points to be modified
	modifiedVertices.clear();
	for (int i = 0; i < lastPoint; i++) {
		modifiedVertices.push_back(originalVertices[i]);
	}

	/* Used for scaling the polygon:
	-first translates polygon to origin using an average of the points,
	-then scales the polygon
	-then translates back to original position */
	Point3D averagePoint = AveragePoint(lastPoint, smooth);

	glPushMatrix();
	glTranslatef(averagePoint.x, averagePoint.y, averagePoint.z);
	glScalef(scale, scale, scale);
	glTranslatef(-averagePoint.x, -averagePoint.y, -averagePoint.z);

	OffsetPolygon(inflation, lastPoint, smooth);
	std::vector<Point3D> controlPoints;

	if (smooth) {
		controlPoints = CalculateClockwiseControlPoints(lastPoint, smooth);
		DrawSmoothVertices(mode, lastPoint, controlPoints);
	} else {
		DrawVertices(mode, lastPoint);
	}


	glPopMatrix();

}

void Polygon::DrawVertices(GLenum mode, int lastPoint) {

	//Special case when drawing only 2 points
	if (lastPoint == 2) {

		std::vector<Point3D> controlPoints = CalculateControls(lastPoint, false);

		modifiedVertices.insert(modifiedVertices.begin() + 1, controlPoints[1]);
		modifiedVertices.insert(modifiedVertices.begin() + 1, controlPoints[0]);
		modifiedVertices.push_back(controlPoints[2]);
		modifiedVertices.push_back(controlPoints[3]);

		lastPoint = 6;

	}


	/* Draws the polygon */
	glBegin(mode);

	glColor4f(polygonColour.GetRed(), polygonColour.GetGreen(), polygonColour.GetBlue(), polygonColour.GetAlpha());

	for (int i = 0; i < lastPoint; i++) {
		glVertex3f(modifiedVertices[i].x, modifiedVertices[i].y, modifiedVertices[i].z);
	}

	glEnd();

}

void Polygon::DrawSmoothVertices(GLenum mode, int lastPoint, std::vector<Point3D> controlPoints) {

	
	glColor4f(polygonColour.GetRed(), polygonColour.GetGreen(), polygonColour.GetBlue(), polygonColour.GetAlpha());

	//Draws the curved outside parts
	for (int i = 0; i < lastPoint; i++) {

		DrawBezier(modifiedVertices[i], 
				   controlPoints[(i*2)], 
				   controlPoints[(i*2+1)], 
				   modifiedVertices[(i+1)%lastPoint], 
				   mode);

	}

	//Draws the inside
	if (mode == GL_TRIANGLE_FAN) {
		
		glBegin(mode);
			for (int i = 0; i < lastPoint; i++) {

				glVertex3f(modifiedVertices[i].x, modifiedVertices[i].y, modifiedVertices[i].z);

			}
		glEnd();

	}

	//Draws control points for testing

	//glBegin(GL_LINES);
	//	
	//uint x = controlPoints.size();
	//for (uint i = 0; i < x; i++) {

	//	glColor3f(0,1,0);
	//	glVertex3f(controlPoints[i%x].x, controlPoints[i%x].y, controlPoints[i%x].z);

	//}

	//glEnd();

}

void Polygon::OffsetPolygon(float offset, int lastPoint, bool smooth) {

	if (lastPoint < 2)
		return;

	std::vector<Point3D> midpoints;

	//Calculate points which will be used to find the slope along which to offset the vertices
	for (int i = 0; i < lastPoint; i++) {

		Point3D mid1 = Midpoint(modifiedVertices.at(i),
								modifiedVertices.at((i+1)%lastPoint), 
								0.5f);

		Point3D mid2 = Midpoint(modifiedVertices.at((i+1)%lastPoint), 
								modifiedVertices.at((i+2)%lastPoint), 
								0.5f);

		double len1 = Distance(modifiedVertices.at(i), 
							   modifiedVertices.at((i+1)%lastPoint));

		double len2 = Distance(modifiedVertices.at((i+1)%lastPoint), 
							   modifiedVertices.at((i+2)%lastPoint));

		float percentage = ( ((float)len1/(len1+len2)) );

		Point3D betweenMids = Midpoint(mid1, mid2, percentage);

		midpoints.push_back(betweenMids);

	}

	std::vector<Point3D> offsetVertices;

	//Calculate the offset points
	uint size = modifiedVertices.size();

	for (uint i = 1; i < size+1; i++){

		Point3D *vertex = &modifiedVertices.at(i%size);
	
		Point3D outer = midpoints.at(i-1);

		float slope = (outer.z - vertex->z) / (outer.x - vertex->x);


		if (midpoints.at(i-1).x < vertex->x) {
			OffsetAlongSlope(*vertex, slope, offset);
		} else {
			OffsetAlongSlope(*vertex, slope, -offset);
		}
		
	}

}

std::vector<Point3D> Polygon::CalculateControls(int lastPoint, bool smooth) {

	std::vector<Point3D> controlPoints;

	//Special case with only 2 points
	if (lastPoint == 2) {

		float slope = ( (modifiedVertices[0].z - modifiedVertices[1].z) /
					    (modifiedVertices[0].x - modifiedVertices[1].x) );

		float distance = Distance( modifiedVertices[0], modifiedVertices[1] );

		int scalingFactor = 5;
		if (smooth)
			scalingFactor = 3;
			

		//calculate perpendicular slope
		slope = -1.0f / slope;

		Point3D control1 = Midpoint( modifiedVertices[0], modifiedVertices[1], 0.25f );
		Point3D control2 = Midpoint( modifiedVertices[0], modifiedVertices[1], 0.75f );
		Point3D control3 = control2;
		Point3D control4 = control1;

		OffsetAlongSlope( control1, slope, distance/scalingFactor );
		OffsetAlongSlope( control2, slope, distance/scalingFactor );
		OffsetAlongSlope( control3, slope, -distance/scalingFactor );
		OffsetAlongSlope( control4, slope, -distance/scalingFactor );

		controlPoints.push_back(control1);
		controlPoints.push_back(control2);
		controlPoints.push_back(control3);
		controlPoints.push_back(control4);

	} else {
		controlPoints.push_back( Point3D(0,0,0) );

		for (int i = 0; i < lastPoint; i++) {

			Point3D mid1 = Midpoint(modifiedVertices[i],
									modifiedVertices[(i+1)%lastPoint], 
									0.5f);

			Point3D mid2 = Midpoint(modifiedVertices[(i+1)%lastPoint], 
									modifiedVertices[(i+2)%lastPoint], 
									0.5f);

			double len1 = Distance(modifiedVertices[i], 
								   modifiedVertices[(i+1)%lastPoint]);

			double len2 = Distance(modifiedVertices[(i+1)%lastPoint], 
								   modifiedVertices[(i+2)%lastPoint]);

			float percentage = ( ((float)len1/(len1+len2)) );

			Point3D betweenMids = Midpoint(mid1, mid2, percentage);

			Point3D control1 ( modifiedVertices[(i+1)%lastPoint].x + (mid1.x - betweenMids.x),
							   modifiedVertices[(i+1)%lastPoint].y,
							   modifiedVertices[(i+1)%lastPoint].z + (mid1.z - betweenMids.z) );

			Point3D control2 ( modifiedVertices[(i+1)%lastPoint].x + (mid2.x - betweenMids.x),
							   modifiedVertices[(i+1)%lastPoint].y,
							   modifiedVertices[(i+1)%lastPoint].z + (mid2.z - betweenMids.z) );

			if (i != lastPoint-1) {
				controlPoints.push_back(control1);
				controlPoints.push_back(control2);
			} else {
				controlPoints.push_back(control1);
				controlPoints[0] = control2;
			}
		}
	}

	return controlPoints;

}

void Polygon::OffsetAlongSlope(Point3D &point, float slope, float offset) {

	float dx = 1.0f / sqrt(1+slope*slope);
	float dy = slope / sqrt(1+slope*slope);

	point.x = point.x + offset * dx;
	point.y = point.y;
	point.z = point.z + offset * dy;

}

double Polygon::CCW(Point3D p1, Point3D p2, Point3D p3) {

	return (p2.x-p1.x) * (p3.z-p1.z) - (p2.z-p1.z) * (p3.x-p1.x);

}

Point3D Polygon::Midpoint(Point3D p1, Point3D p2, float percentage) {

	return ((1-percentage) * p1 + percentage * p2);

}

double Polygon::Distance(Point3D p1, Point3D p2) {

	return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.z-p2.z)*(p1.z-p2.z));

}

Point3D Polygon::AveragePoint(int lastPoint, bool smooth) {

	Point3D average (0,0,0);

	if (smooth) {
		for (int i = 0; i < lastPoint; i++) {
			average += modifiedVertices[i];
		}
	} else {
		for (int i = 0; i < lastPoint; i++) {
			average += originalVertices[i];
		}
	}

	average = average / lastPoint;
	return average;
}

std::vector<Point3D> Polygon::CalculateClockwiseControlPoints(int &lastPoint, bool smooth) {

	if (lastPoint < 3)
		return CalculateControls(lastPoint, smooth);

	//Initializes control points
	std::vector<Point3D> controlPoints = CalculateControls(lastPoint, smooth);
	int j = 0;
	bool reset = false;

	//Removes points for which both sets of adjacent control points make a counter clockwise turn
	for (std::vector<Point3D>::iterator it = modifiedVertices.begin(); it != modifiedVertices.end(); ++it) {

		int previous = (j-1)%lastPoint; if (previous < 0) previous += lastPoint; //modulus operator

		if (reset) {
			it--;
			reset = false;
		}

		if ( CCW(controlPoints[previous*2], controlPoints[previous*2+1], *it) <= 0 
		  && CCW(*it, controlPoints[j*2], controlPoints[j*2+1]) <= 0 ) {
		
			if (lastPoint > 1) {
				modifiedVertices.erase(it);
				lastPoint--;
				controlPoints = CalculateControls(lastPoint, smooth);
				it = modifiedVertices.begin();
				reset = true;
				j = -1;
			}
		
		}
		j = (j+1)%lastPoint;
	}

	return controlPoints;

}

void Polygon::DrawBezier(Point3D p1, Point3D p2, Point3D p3, Point3D p4, GLenum mode) {

	GLfloat controlPoints[4][3] = { {p1.x, p1.y, p1.z},
									{p2.x, p2.y, p2.z},
									{p3.x, p3.y, p3.z},
									{p4.x, p4.y, p4.z} };

	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &controlPoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);

	glBegin(mode);
		for (int i = 0; i <= 30; i++) 
			glEvalCoord1f((GLfloat) i/30.0);
	glEnd();

}