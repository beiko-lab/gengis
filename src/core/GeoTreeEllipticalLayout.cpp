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

#include "../core/GeoTreeEllipticalLayout.hpp"
#include "../core/LayoutEllipse.hpp"
#include "../core/MapController.hpp"
#include "../core/LocationView.hpp"

#include "../utils/Geometry.hpp"

using namespace GenGIS;

template<class Archive>
void GeoTreeEllipticalLayout::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_ellipticalNodes; // std::vector<EllipticalNode>
}
template void GeoTreeEllipticalLayout::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void GeoTreeEllipticalLayout::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void GeoTreeEllipticalLayout::LayoutEllipseQuadrant(Tree<NodeGeoTree>::Ptr tree, LayoutEllipsePtr layoutEllipse, float height)
{
	m_ellipticalNodes.clear();

	// find the optimal ordering of leaf nodes
	// OptimalLeafNodeOrderingElliptical();

	// clear grid position of all nodes
	std::vector<NodeGeoTree*> nodes = tree->GetNodes();
	foreach(NodeGeoTree* node, nodes)
	{
		node->SetGridCoord(Point3D(Node::NO_DISTANCE, Node::NO_DISTANCE, Node::NO_DISTANCE));
	}

	// determine distance between tree levels
	double depthStep = (1.0/tree->GetDepth(tree->GetRootNode()))*layoutEllipse->GetRadiusX()*height;
	if(layoutEllipse->GetRadiusX() < layoutEllipse->GetRadiusZ())
		depthStep = (1.0/tree->GetDepth(tree->GetRootNode()))*layoutEllipse->GetRadiusZ()*height;

	// layout leaf nodes evenly within each quadrant
	std::vector<NodeGeoTree*> leafNodes = tree->GetLeaves();
	std::vector<Point3D> locationPts;
	foreach(NodeGeoTree* leafNode, leafNodes)
	{
		locationPts.push_back(leafNode->GetLocationView()->GetPosition());
	}

	std::vector<Point3D> layoutPts;
	layoutEllipse->LayoutPtsByQuadrant(locationPts, layoutPts, true);

	for(uint i = 0; i < layoutPts.size(); ++i)
	{
		NodeGeoTree* node = leafNodes.at(i);
		node->SetGridCoord(layoutPts.at(i));	
		Point3D offsetPt = GetEllipseLayoutPt(layoutEllipse, 
			GetAngleOnStandardEllipse(layoutEllipse, layoutPts.at(i)), 
			tree->GetDepth(node)*depthStep,
			tree->GetDepth(node->GetParent())*depthStep);
		m_ellipticalNodes.push_back(EllipticalNode(node, Line3D(layoutPts.at(i), offsetPt)));
	}

	// define commonly used variables
	Point3D ellipseCentre = layoutEllipse->GetCentrePt();

	// each internal node should be set to the mean of its immediate children
	std::vector<NodeGeoTree*> curNodes = tree->GetLeaves();
	while(!curNodes.empty())
	{
		std::vector<NodeGeoTree*> parentNodes; 
		foreach(NodeGeoTree* curNode, curNodes)
		{
			NodeGeoTree* parent = curNode->GetParent();
			if(parent->GetGridCoord().x == Node::NO_DISTANCE)
			{
				// make sure the position of all children nodes has already been set
				std::vector<NodeGeoTree*> children = parent->GetChildren();
				bool bChildrenSet = true;
				std::vector<double> childAngles;
				foreach(NodeGeoTree* child, children)
				{
					if(child->GetGridCoord().x == Node::NO_DISTANCE)
					{
						bChildrenSet = false;
						break;
					}

					// get angle between 0 and 2PI in standard ellipse coordinate frame
					childAngles.push_back(GetAngleOnStandardEllipse(layoutEllipse, child->GetGridCoord()));
				}

				if(!bChildrenSet)
					continue;

				if(!parent->IsRoot())
					parentNodes.push_back(parent);

				// determine CCW ordering of angles
				std::vector<uint> ccwIndices;
				Geometry::AngleBisector(childAngles, ccwIndices);

				// determine start and end point of line the connects a child node to
				// its parent's arc
				std::vector<double> offsetChildAngles;
				EllipticalNode ellipticalNode(parent);
				for(uint i = 0; i < ccwIndices.size(); ++i)
				{
					uint index = ccwIndices.at(i);

					Point3D offsetPt = GetEllipseLayoutPt(layoutEllipse, 
						childAngles.at(index), 
						tree->GetDepth(children.at(index))*depthStep,
						tree->GetDepth(parent)*depthStep);

					// get angle of offset child node between 0 and 2PI in standard ellipse coordinate frame					
					offsetChildAngles.push_back(GetAngleOnStandardEllipse(layoutEllipse, offsetPt));
				}

				// get points comprising the polyline arc that connects a parent to its children
				Point3D dummyPt;
				PolyLine3D arc;
				double rX = layoutEllipse->GetRadiusX()+tree->GetDepth(parent)*depthStep;
				double rZ = layoutEllipse->GetRadiusZ()+tree->GetDepth(parent)*depthStep;
				for(uint i = 0; i < offsetChildAngles.size(); ++i)
				{					
					if(i < offsetChildAngles.size() - 1)
					{
						double dAngle = offsetChildAngles.at(i+1) - offsetChildAngles.at(i);
						if(dAngle < 0)
							dAngle = PI2 + dAngle;

						uint steps = int(dAngle / DEG_TO_RAD);
						for(uint j = 0; j < steps; ++j)
						{
							Point3D canonicalPt = Geometry::GetEllipsePointAtAngleXZ(rX, rZ, offsetChildAngles.at(i)+j*DEG_TO_RAD);
							arc.pts.push_back(layoutEllipse->EllipseRefFramePos(canonicalPt));
						}
					}
					else
					{		
						// make sure arc extends completely to the last point
						Point3D canonicalPt = Geometry::GetEllipsePointAtAngleXZ(rX, rZ, offsetChildAngles.at(i));
						arc.pts.push_back(layoutEllipse->EllipseRefFramePos(canonicalPt));
					}
				}

				// position parent node in the "middle" of where its children connect with the arc
				double parentAngle = Geometry::AngleBisector(offsetChildAngles, ccwIndices);				
				Point3D canonicalPt = Geometry::GetEllipsePointAtAngleXZ(rX, rZ, parentAngle);
				Point3D parentPt = layoutEllipse->EllipseRefFramePos(canonicalPt);
				parent->SetGridCoord(parentPt);

				// determine line that connect node to its parent arc
				Point3D offsetPt;
				if(!parent->IsRoot())
				{
					offsetPt = GetEllipseLayoutPt(layoutEllipse, 
						GetAngleOnStandardEllipse(layoutEllipse, parentPt), 
						tree->GetDepth(parent)*depthStep,
						tree->GetDepth(parent->GetParent())*depthStep);
				}
				else
				{
					offsetPt = GetEllipseLayoutPt(layoutEllipse, 
						GetAngleOnStandardEllipse(layoutEllipse, parentPt), 
						tree->GetDepth(parent)*depthStep,
						(tree->GetDepth(parent)+1)*depthStep);	
				}

				m_ellipticalNodes.push_back(EllipticalNode(parent, Line3D(parentPt, offsetPt), arc));
			}
		}

		curNodes = parentNodes;
	}
}

double GeoTreeEllipticalLayout::GetAngleOnStandardEllipse(LayoutEllipsePtr layoutEllipse, const Point3D& pt) const
{
	double angle = Geometry::AngleToX(Line3D(layoutEllipse->GetCentrePt(), pt));
	angle += layoutEllipse->GetRotation();
	if(angle < 0) 
		angle = PI2 + angle;

	return angle;
}

Point3D GeoTreeEllipticalLayout::GetEllipseLayoutPt(LayoutEllipsePtr layoutEllipse, double angle, double childOffset, double parentOffset) const
{	
	// get radius of ellipse on which the child node resides
	double rX = layoutEllipse->GetRadiusX()+childOffset;
	double rZ = layoutEllipse->GetRadiusZ()+childOffset;

	// find intersection between line from (0,0) to (xx,zz) and the ellipse 
	Point3D canonicalPt = Geometry::GetEllipsePointAtAngleXZ(rX, rZ, angle);

	// find position of child node on the arc (ellipse) which connects it to it's parent node
	Point3D tangentVec = Geometry::EllipseNormalXZ(rX, rZ, canonicalPt);
	Line3D line(canonicalPt, canonicalPt + tangentVec);
	std::vector<Point3D> intersectPt;
	Geometry::EllipseLineIntersectXZ(layoutEllipse->GetRadiusX()+parentOffset,
		layoutEllipse->GetRadiusZ()+parentOffset,
		line, intersectPt);

	// find intersection point is same quadrant as line segment
	if(canonicalPt.x >= 0 && canonicalPt.z >= 0)
	{
		foreach(Point3D pt, intersectPt)
		{
			if(pt.x >= 0 && pt.z >= 0)
				return layoutEllipse->EllipseRefFramePos(pt);
		}
	}
	else if(canonicalPt.x < 0 && canonicalPt.z >= 0)
	{
		foreach(Point3D pt, intersectPt)
		{
			if(pt.x < 0 && pt.z >= 0)
				return layoutEllipse->EllipseRefFramePos(pt);
		}
	}
	else if(canonicalPt.x < 0 && canonicalPt.z < 0)
	{
		foreach(Point3D pt, intersectPt)
		{
			if(pt.x < 0 && pt.z < 0)
				return layoutEllipse->EllipseRefFramePos(pt);
		}
	}
	else if(canonicalPt.x >= 0 && canonicalPt.z < 0)
	{
		foreach(Point3D pt, intersectPt)
		{
			if(pt.x >= 0 && pt.z < 0)
				return layoutEllipse->EllipseRefFramePos(pt);
		}
	}

	// this should never happen
	Log::Inst().Error("GetEllipseLayoutPt(): Unhandle case encountered.");
	return Point3D(0,0,0);
}

