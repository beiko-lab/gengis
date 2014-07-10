#include "../core/Precompiled.hpp"

#include "../utils/Polygon.hpp"

using namespace GenGIS;

template<class Archive>
void Polygon::serialize(Archive & ar, const unsigned int file_version)
{
	//General
	ar & originalVertices;	//std::vector<Point3D>
	ar & modifiedVertices;	//std::vector<Point3D>
	ar & controlPoints;		//std::vector<Point3D>
	ar & polygonColour;		//colour
	ar & lastVertexIndex;	//int
	ar & bVisible;			//bool
}
template void Polygon::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void Polygon::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void Polygon::InitPolygon(float inflation, bool smooth) {

	/* Calculate stopping point */
	int lastPoint = lastVertexIndex;

	if (lastVertexIndex == -1)
		lastPoint = originalVertices.size();

	//Fills modifiedVertices with points to be modified
	modifiedVertices.clear();
	for (int i = 0; i < lastPoint; i++) {
		modifiedVertices.push_back(originalVertices[i]);
	}

	if (lastPoint == 1) {

		int num_segments = 8;

		if (smooth)
			num_segments = 60;

		InitCircle(modifiedVertices[0], 0.015 + inflation, num_segments);

	} else if (lastPoint == 2) {

		OffsetPolygon(inflation, lastPoint, smooth);

		std::vector<Point3D> hexPoints = CalculateHexPoints(smooth);

		if (smooth) {
			
			controlPoints = hexPoints;

		} else {

			modifiedVertices.insert(modifiedVertices.begin() + 1, hexPoints[1]);
			modifiedVertices.insert(modifiedVertices.begin() + 1, hexPoints[0]);
			modifiedVertices.push_back(hexPoints[2]);
			modifiedVertices.push_back(hexPoints[3]);

			lastPoint += 4;

		}

	} else {

		OffsetPolygon(inflation, lastPoint, smooth);

		if (smooth)
			controlPoints = CalculateClockwiseControlPoints(lastPoint, smooth);

	}


}

void Polygon::RenderBorder(float thicknessOfBorder, bool smooth, float scale) {

	if (!bVisible)
		return;
	
	glLineWidth(thicknessOfBorder);
	if (smooth && originalVertices.size() != 1)
		RenderVertices(GL_LINE_STRIP, smooth, scale);
	else 
		RenderVertices(GL_LINE_LOOP, smooth, scale);

}

void Polygon::RenderVertices(GLenum mode, bool smooth, float scale) {

	/* Calculate stopping point */
	int lastPoint = lastVertexIndex;

	if (lastVertexIndex == -1)
		lastPoint = originalVertices.size();

	//Used for scaling the polygon
	Point3D averagePoint = AveragePoint();

	glPushMatrix();
	glTranslatef(averagePoint.x, averagePoint.y, averagePoint.z);
	glScalef(scale, scale, scale);
	glTranslatef(-averagePoint.x, -averagePoint.y, -averagePoint.z);

	if (smooth && originalVertices.size() != 1) {
		DrawSmoothVertices(mode);
	} else {
		DrawVertices(mode);
	}

	glPopMatrix();

}

void Polygon::DrawVertices(GLenum mode) {

	glColor4f(polygonColour.GetRed(), polygonColour.GetGreen(), polygonColour.GetBlue(), polygonColour.GetAlpha());

	/* Draws the polygon */
	glBegin(mode);

	for (uint i = 0; i < modifiedVertices.size(); i++) {
		glVertex3f(modifiedVertices[i].x, modifiedVertices[i].y, modifiedVertices[i].z);
	}

	glEnd();

}

void Polygon::DrawSmoothVertices(GLenum mode) {

	glColor4f(polygonColour.GetRed(), polygonColour.GetGreen(), polygonColour.GetBlue(), polygonColour.GetAlpha());

	//Draws the curved outside parts
	for (uint i = 0; i < modifiedVertices.size(); i++) {

		DrawBezier(modifiedVertices[i], 
				   controlPoints[(i*2)], 
				   controlPoints[(i*2+1)], 
				   modifiedVertices[(i+1)%modifiedVertices.size()], 
				   mode);

	}

	//Draws the inside
	if (mode == GL_TRIANGLE_FAN) {
		
		glBegin(mode);
			for (uint i = 0; i < modifiedVertices.size(); i++) {

				glVertex3f(modifiedVertices[i].x, modifiedVertices[i].y, modifiedVertices[i].z);

			}
		glEnd();

	}

	//Draws control points for testing

	//glBegin(GL_LINES);
	//	
	//uint x = controlPoints.size();
	//for (uint i = 1; i < x+1; i++) {

	//	glColor3f(0,1,0);
	//	glVertex3f(controlPoints[i%x].x, controlPoints[i%x].y, controlPoints[i%x].z);

	//}

	//glEnd();

}

void Polygon::OffsetPolygon(float offset, int lastPoint, bool smooth) {

	if (lastPoint < 2)
		return;

	std::vector<Point3D> innerPoints = GetInnerPoints();

	std::vector<Point3D> offsetVertices;

	//Calculate the offset points
	uint size = modifiedVertices.size();

	for (uint i = 1; i < size+1; i++){

		Point3D *vertex = &modifiedVertices.at(i%size);
		Point3D outer = innerPoints.at(i-1);

		float slope = (outer.z - (*vertex).z) / (outer.x - (*vertex).x);


		if (innerPoints.at(i-1).x < (*vertex).x) {
			OffsetAlongSlope(*vertex, slope, offset);
		} else {
			OffsetAlongSlope(*vertex, slope, -offset);
		}
		
	}

}

std::vector<Point3D> Polygon::CalculateHexPoints(bool smooth) {

	std::vector<Point3D> hexPoints;

	float slope = ( (modifiedVertices[0].z - modifiedVertices[1].z) /
				    (modifiedVertices[0].x - modifiedVertices[1].x) );

	//OffsetAmount determines how wide the hexagons will be
	float offsetAmount = 0.0125f;
	if (smooth)
		offsetAmount = 0.02f;

	while (Distance(modifiedVertices[0], modifiedVertices[1]) < offsetAmount/2) 
		offsetAmount /= 2;			

	//Calculate slope perpendicular to the 2 points
	slope = -1.0f / slope;

	Point3D control1, control2, control3, control4;

	//Not smooth polygons have the points offset from 1/4 and 3/4 between the 2 points
	if (!smooth) {
		control1 = control4 = Midpoint( modifiedVertices[0], modifiedVertices[1], 0.25f );
		control2 = control3 = Midpoint( modifiedVertices[0], modifiedVertices[1], 0.75f );
	//Smooth polygons have the points offset directly from the 2 points
	} else {
		control1 = control4 = modifiedVertices[0];
		control2 = control3 = modifiedVertices[1];
	}

	OffsetAlongSlope( control1, slope, offsetAmount );
	OffsetAlongSlope( control2, slope, offsetAmount );
	OffsetAlongSlope( control3, slope, -offsetAmount );
	OffsetAlongSlope( control4, slope, -offsetAmount );

	hexPoints.push_back(control1);
	hexPoints.push_back(control2);
	hexPoints.push_back(control3);
	hexPoints.push_back(control4);

	return hexPoints;

}

std::vector<Point3D> Polygon::CalculateControls(bool smooth) {

	int lastPoint = modifiedVertices.size();
	std::vector<Point3D> innerPoints = GetInnerPoints();
	std::vector<Point3D> controls;

	controls.push_back( Point3D(0,0,0) );

	for (int i = 0; i < lastPoint; i++) {

		Point3D mid1 = Midpoint(modifiedVertices[i], modifiedVertices[(i+1)%lastPoint], 0.5f);
		Point3D mid2 = Midpoint(modifiedVertices[(i+1)%lastPoint], modifiedVertices[(i+2)%lastPoint], 0.5f);

		Point3D control1 ( modifiedVertices[(i+1)%lastPoint].x + (mid1.x - innerPoints[i].x),
						   modifiedVertices[(i+1)%lastPoint].y,
						   modifiedVertices[(i+1)%lastPoint].z + (mid1.z - innerPoints[i].z) );

		Point3D control2 ( modifiedVertices[(i+1)%lastPoint].x + (mid2.x - innerPoints[i].x),
						   modifiedVertices[(i+1)%lastPoint].y,
						   modifiedVertices[(i+1)%lastPoint].z + (mid2.z - innerPoints[i].z) );

		if (i != lastPoint-1) {
			controls.push_back(control1);
			controls.push_back(control2);
		} else {
			controls.push_back(control1);
			controls[0] = control2;
		}
	}

	return controls;

}

std::vector<Point3D> Polygon::CalculateClockwiseControlPoints(int &lastPoint, bool smooth) {

	if (lastPoint < 3)
		return CalculateControls(smooth);

	//Initializes control points
	std::vector<Point3D> controlPoints = CalculateControls(smooth);
	int j = 0;
	bool reset = false;

	//Removes points for which both sets of adjacent control points make a counter clockwise turn
	for (std::vector<Point3D>::iterator it = modifiedVertices.begin(); it != modifiedVertices.end(); ++it) {

		int previous = (j-1)%lastPoint; if (previous < 0) previous += lastPoint; //modulus operator

		//If a point was removed in the last iteration, the iterator starts from the beginning
		if (reset) {
			it = modifiedVertices.begin();
			reset = false;
		}

		//Points on the right side of the polygon are missed using the same cutoff point as the left side
		Point3D averagePoint = AveragePoint();
		double cutOff;

		if (it->x < averagePoint.x || it->z > averagePoint.z) {
			cutOff = 0;
		} else {
			cutOff = -EPSILON*25; //chosen experimentally
		}

		//Removes points for which both sets of adjacent control points are counter-clockwise,
		// with less precision for lower cutOff values
		if ( CCW(controlPoints[previous*2], controlPoints[previous*2+1], *it) <= cutOff 
		  && CCW(*it, controlPoints[j*2], controlPoints[j*2+1]) <= cutOff ) {
		
			if (lastPoint > 1) {
				modifiedVertices.erase(it);
				lastPoint--;
				controlPoints = CalculateControls(smooth);
				it = modifiedVertices.begin();
				reset = true;
				j = -1;
			}
		
		}
		j = (j+1)%lastPoint;
	}

	return controlPoints;

}

std::vector<Point3D> Polygon::GetInnerPoints() {

	std::vector<Point3D> innerPoints;
	int lastPoint = modifiedVertices.size();

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

		innerPoints.push_back(betweenMids);

	}

	return innerPoints;

}

void Polygon::InitCircle(Point3D center, float radius, int num_segments) {

	//Angle of rotation
	float theta = PI2 / num_segments;

	float cosTheta = cos(theta);
	float sinTheta = sin(theta);
	float temp;

	float dx = radius;
	float dy = 0;

	modifiedVertices.clear();

	for (int i = 0; i < num_segments; i++) {

		modifiedVertices.push_back( Point3D(center.x + dx, center.y, center.z + dy) );

		//Rotate point by theta
		temp = dx;
		dx = cosTheta * dx - sinTheta * dy;
		dy = sinTheta * temp + cosTheta * dy;

	}

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

Point3D Polygon::AveragePoint() {

	Point3D average (0,0,0);

	for (uint i = 0; i < modifiedVertices.size(); i++) 
		average += modifiedVertices[i];

	average = average / modifiedVertices.size();
	return average;

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