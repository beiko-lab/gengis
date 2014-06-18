#include "../core/Precompiled.hpp"

#include "../core/App.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/LocationPolygons.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/LocationGrid.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/LocationModel.hpp"

#include "../utils/Colour.hpp"

using namespace GenGIS;

LocationPolygons::LocationPolygons() : 

	//General
	m_bVisible ( false ),
	m_polygonsChanged ( true ),
	m_autoAdjustElevation ( false ),
	m_increasingElevation ( false ),
	m_smoothPolygons ( true ),
	m_elevationOffset ( 0.01f ),
	m_inputElevation ( 0.001f ),
	m_fillOpacity ( 0.7f ),
	m_polygonInflation ( 0.00f ),
	m_polygonScaling ( 1.1f ),
	m_sortBy ( CONVEX_HULL ),

	//Border
	m_showBorders ( true ),
	m_borderThickness ( 3 ),
	m_borderOpacity ( 1.0f )

{
	
}

template<class Archive>
void LocationPolygons::serialize(Archive & ar, const unsigned int file_version)
{
	//General
	ar & m_bVisible;			//bool
	ar & m_polygonsChanged;		//bool
	ar & m_autoAdjustElevation;	//bool
	ar & m_increasingElevation;	//bool
	ar & m_smoothPolygons;		//bool
	ar & m_elevationOffset;		//float
	ar & m_inputElevation;		//float
	ar & m_elevationUsed;		//float
	ar & m_fillOpacity;			//float
	ar & m_sortBy;				//ALGORITHM
	ar & m_currentLocationSet;	//std::vector<LocationLayerPtr>
	ar & m_polygons;			//std::vector<Polygon>

	//Sizing
	ar & m_polygonInflation;	//float
	ar & m_polygonScaling;		//float

	//Border
	ar & m_showBorders;			//bool
	ar & m_borderThickness;	//uint
	ar & m_borderOpacity;		//float
}
template void LocationPolygons::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void LocationPolygons::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

LocationPolygons::~LocationPolygons()
{	
}

void LocationPolygons::InitPolygons()
{
	//Clears existing polygons
	m_polygons.clear();

	Colour currentColour;
	GeoCoord currentCoord;
	Point3D mapPoint;
	std::wstring currentField;

	Polygon emptyPoly;
	m_polygons.push_back(emptyPoly);
	int currentPolygonIndex = 0;

	//Get elevation to be used
	MapControllerPtr mapController = App::Inst().GetLayerTreeController()->GetMapLayer(0)->GetMapController();

	if ( m_autoAdjustElevation )
	{
		m_elevationUsed = mapController->GetMaxElevationGridSpace()
		                * mapController->GetVerticalExaggeration()
		                + 0.005; // Add a minimal amount to avoid clashing
		                         // between colours of polygons and map
	}
	else
	{
		m_elevationUsed = m_inputElevation;
	}

	//Sort location set by colour
	std::sort (m_currentLocationSet.begin(), 
			   m_currentLocationSet.end(), 
			   LocationPolygons::SortByColour);

	//Goes through each location in the given set
	for (uint i = 0; i < m_currentLocationSet.size(); i++) {

		//If the current location is active
		if (m_currentLocationSet[i]->GetLocationController()->GetLocationModel()->IsActive()) {

			//Sets the current properties of the location
			currentColour = m_currentLocationSet[i]->GetLocationController()->GetColour();
			currentColour.SetAlpha(m_fillOpacity);

			currentCoord.easting = m_currentLocationSet[i]->GetLocationController()->GetLocationModel()->GetEasting();
			currentCoord.northing = m_currentLocationSet[i]->GetLocationController()->GetLocationModel()->GetNorthing();

			//Converts from lat/long point to a point on the map
			App::Inst().GetMapController()->GetMapModel()->LatLongToGrid(currentCoord, mapPoint);

			//If there's a new colour, a new polygon is created
			if (currentColour != m_polygons[currentPolygonIndex].polygonColour) {
				m_polygons.push_back(emptyPoly);
				currentPolygonIndex++;
				m_polygons[currentPolygonIndex].polygonColour = currentColour;
			}

			//Sets elevation
			if (m_increasingElevation)
				mapPoint.y = m_elevationUsed + m_elevationOffset*currentPolygonIndex;
			else 
				mapPoint.y = m_elevationUsed;

			//If the point is the same point (within EPSILON) then it should not be added to the polygon
			//**If the same points are added, then the convex hull algorithm doesn't work**
			bool shouldAdd = true;

			for (uint j = 0; j < m_polygons[currentPolygonIndex].originalVertices.size(); j++) {
				
				double deltaX = abs( m_polygons[currentPolygonIndex].originalVertices[j].x - mapPoint.x );
				double deltaY = abs( m_polygons[currentPolygonIndex].originalVertices[j].z - mapPoint.z );

				if (deltaX <= EPSILON && deltaY <= EPSILON)
					shouldAdd = false;

			}

			if (shouldAdd) {

				m_polygons[currentPolygonIndex].originalVertices.push_back(mapPoint);
									
			}
		}
	}

	//Sorts the polygons according to the given algorithm
	if (m_sortBy == CONVEX_HULL) {

		for (uint i = 0; i < m_polygons.size(); i++) {
			
			LocationPolygons::ConvexHull(m_polygons[i]);

		}
	} else if (m_sortBy == BLANK) {}
}

void LocationPolygons::Render() {

	if (!m_bVisible)
		return;

	if (m_polygonsChanged) {
		LocationPolygons::InitPolygons();
		m_polygonsChanged = false;
	}

	//Adjust openGL settings
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glEnable ( GL_BLEND );
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	for (uint i = 0; i < m_polygons.size(); i++) {
			
		//Render Polygons
		m_polygons[i].polygonColour.SetAlpha(m_fillOpacity);
		m_polygons[i].RenderConvex(m_smoothPolygons, m_polygonInflation, m_polygonScaling);

		//Render Borders
		if (m_showBorders) {

			m_polygons[i].polygonColour.SetAlpha(m_borderOpacity);
			m_polygons[i].RenderBorder(m_borderThickness, m_smoothPolygons, m_polygonInflation, m_polygonScaling);

		}

	}

	//Re-enable disabled settings
	glEnable( GL_LIGHTING );
	glEnable( GL_DEPTH_TEST );

}

/*Structure used in ConvexHull to use std::sort to sort the vertices by angle with the lowest northing coordinate
  Note: must be declared before ConvexHull(Polygon &poly)	*/
struct sortByAngle {

	sortByAngle(Point3D low) : lowestN (low) {}

	bool operator() (Point3D s, Point3D t) {
		
		//If s is lowY it should be treated as the lowest value
		if (s == lowestN)
			return true;

		//Finds the -cotangent of s and t with respect to lowY
		float sCot = (-((s.x-lowestN.x)/(s.z-lowestN.z)));
		float tCot = (-((t.x-lowestN.x)/(t.z-lowestN.z)));
				

		//If the angles are the same, the closer one should be treated as less		
		if (sCot == tCot){ 
			
			double d2 = (s.x-lowestN.x)*(s.x-lowestN.x) + (s.y-lowestN.z) * (s.y-lowestN.z);
			double d1 = (t.x-lowestN.x)*(t.x-lowestN.x) + (t.y-lowestN.z) * (t.y-lowestN.z);
				
			return d1 < d2;		

		} 

		//Otherwise the lower angle should be treated as less
		return sCot < tCot;				
	}

private:
	Point3D lowestN;
};

void LocationPolygons::ConvexHull(GenGIS::Polygon &poly) {

	if (poly.originalVertices.size() < 3)
		return;

	//Finds point with lowest northing coordinate
	Point3D lowestNorthing = poly.originalVertices[0];
	for (uint i = 1; i < poly.originalVertices.size(); i++) {

		if (poly.originalVertices[i].z < lowestNorthing.z) {
			lowestNorthing = poly.originalVertices[i];
		} else if (poly.originalVertices[i].z == lowestNorthing.z) {
			if (poly.originalVertices[i].x < lowestNorthing.x)
				lowestNorthing = poly.originalVertices[i];
		}
	
	}

	//Sorts by angle with lowest northing point
	sortByAngle sortByAngle(lowestNorthing);
	std::sort (poly.originalVertices.begin(), poly.originalVertices.end(), sortByAngle);


	//Orders the polygon so that its first entries are the convex hull
	//the last point on the convex hull will be at the index of the polygon's lastVertexIndex member variable
	
	int M = 1;	//M represents the number of points on the convex hull

	for (uint i = 2; i < poly.originalVertices.size(); i++) {

		while (CCW(poly.originalVertices[M-1], poly.originalVertices[M], poly.originalVertices[i]) <= 0) {	

			if (M > 1) {						
				M -= 1;						
			} else if (i == poly.originalVertices.size()) {				
				break;							
			} else {							
				i++;							
			}

		}

		M++;
		if (i < poly.originalVertices.size())
			SwapIndices(poly.originalVertices,M,i);
	}

	//Sets the index of the last point to draw
	poly.lastVertexIndex = M+1;
}

bool LocationPolygons::SortByColour(LocationLayerPtr p1, LocationLayerPtr p2) {

	Colour p1Colour = p1->GetLocationController()->GetColour();
	Colour p2Colour = p2->GetLocationController()->GetColour();

	//Arbitrary weighted average to make sure the same colours are grouped together
	double p1Average = (2027*p1Colour.GetRed() + 2063*p1Colour.GetGreen() + 2067*p1Colour.GetBlue())/3;
	double p2Average = (2027*p2Colour.GetRed() + 2063*p2Colour.GetGreen() + 2067*p2Colour.GetBlue())/3;

	return (p1Average < p2Average);

}

double LocationPolygons::CCW(Point3D p1, Point3D p2, Point3D p3) {

	return (p2.x-p1.x) * (p3.z-p1.z) - (p2.z-p1.z) * (p3.x-p1.x);

}

void LocationPolygons::SwapIndices(std::vector<Point3D> &v, int a, int b) {

	Point3D temp = v.at(a);
	v.at(a) = v.at(b);
	v.at(b) = temp;

}