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

#include "../core/GeoTreeLinearLayout.hpp"
#include "../utils/Geometry.hpp"
#include "../core/LayoutLine.hpp"

using namespace GenGIS;

template<class Archive>
void GeoTreeLinearLayout::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_implicitRoot; // Point3D
}
template void GeoTreeLinearLayout::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void GeoTreeLinearLayout::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


void GeoTreeLinearLayout::LayoutCladogram2D(Tree<NodeGeoTree>::Ptr tree, LayoutLinePtr layoutLine, float height, bool bHorizontal)
{
	// The tree is initially laid out within a unit square in the 
	// x,y plane with leaf nodes along the x-axis. It is later scaled, 
	// rotated, and translate into the desired position.

	// Note: The algorithm used here to layout the tree is rather inefficent as it will visit nodes
	// before there is sufficent information to determine where they should be placed. This can certainly
	// be improved!

	// clear grid position of all nodes
	std::vector<NodeGeoTree*> nodes = tree->GetNodes();
	foreach(NodeGeoTree* node, nodes)
	{
		node->SetGridCoord(Point3D(Node::NO_DISTANCE, Node::NO_DISTANCE, Node::NO_DISTANCE));
	}

	// distribute leaf nodes along the layout line
	std::vector<NodeGeoTree*> leafNodes = tree->GetLeaves();
	DistributeLeafNodes(leafNodes);

	// each internal node should be set to the mean of its immediate children
	std::vector<NodeGeoTree*> curNodes = tree->GetLeaves();

	// determine distance between tree levels
	float depthStep = (1.0/tree->GetDepth(tree->GetRootNode()));

	while(!curNodes.empty())
	{
		std::vector<NodeGeoTree*> parentNodes; 
		foreach(NodeGeoTree* curNode, curNodes)
		{
			NodeGeoTree* parent = curNode->GetParent();
			if(parent->GetGridCoord().x == Node::NO_DISTANCE)
			{
				// set x-position of parent to the mean x-position of its children

				std::vector<NodeGeoTree*> children = parent->GetChildren();
				std::vector<Point3D> points;
				bool bProcessParent = true;

				foreach(NodeGeoTree* child, children)
				{
					Point3D leafCoord = child->GetGridCoord();
					if(leafCoord.x == Node::NO_DISTANCE)
					{
						bProcessParent = false;
						break;
					}

					points.push_back(leafCoord);
				}

				if(!bProcessParent)
				{
					// at least one child has not had its position determined yet
					continue;
				}

				// calculate mean position of leaf nodes
				Point3D mean;
				CalculateMean(points, mean);

				// calculate position of parent node
				parent->SetGridCoord(Point3D(mean.x, tree->GetDepth(parent)*depthStep, 0.0));

				if(!parent->IsRoot())
					parentNodes.push_back(parent);
			}
		}

		curNodes = parentNodes;
	}

	TransformTreeLayout(tree, layoutLine, height, bHorizontal);
}

void GeoTreeLinearLayout::LayoutSlantedCladogram2D(Tree<NodeGeoTree>::Ptr tree, LayoutLinePtr layoutLine, float height, bool bHorizontal)
{
	// The tree is initially laid out within a unit square in the 
	// x,y plane with leaf nodes along the x-axis. It is than scaled, 
	// rotated, and translate into the desired position.

	// distribute leaf nodes along the layout line
	std::vector<NodeGeoTree*> leafNodes = tree->GetLeaves();
	DistributeLeafNodes(leafNodes);

	// position root node 
	tree->GetRootNode()->SetGridCoord(Point3D(0.5, 1.0, 0.0));

	// determine distance between tree levels
	float depthStep = (1.0/tree->GetDepth(tree->GetRootNode()));

	// position all internal nodes
	std::vector<NodeGeoTree*> curNodes;
	std::vector<NodeGeoTree*> childNodes = tree->GetRootNode()->GetChildren();

	while(childNodes.size() != 0)
	{
		curNodes = childNodes;
		childNodes.clear();

		foreach(NodeGeoTree* curNode, curNodes)
		{
			std::vector<Point3D> endPoints;

			if(curNode->IsLeaf())
				continue;

			// add children to child node list
			for(unsigned int childIndex = 0; childIndex < curNode->GetNumberOfChildren(); ++childIndex)
			{
				childNodes.push_back(curNode->GetChild(childIndex));
			}

			// get position of left-most descendant
			NodeGeoTree* leftDescendant = curNode->GetChild(0);
			while(!leftDescendant->IsLeaf())
			{
				leftDescendant = leftDescendant->GetChild(0);
			}
			endPoints.push_back(leftDescendant->GetGridCoord());

			// get position of right-most descendant
			NodeGeoTree* rightDescendant = curNode->GetChild(curNode->GetNumberOfChildren()-1);
			while(!rightDescendant->IsLeaf())
			{
				rightDescendant = rightDescendant->GetChild(rightDescendant->GetNumberOfChildren()-1);
			}
			endPoints.push_back(rightDescendant->GetGridCoord());

			// calculate mid-point of descendants
			Point3D mean;
			CalculateMean(endPoints, mean);

			// determine if current node is a left, right, or middle child
			NodeGeoTree* parent = curNode->GetParent();

			Point2D leafPos;
			if(mean.x < curNode->GetParent()->GetGridCoord().x)
			{
				// left: place node along line from parent to left-most leaf node
				leafPos = Point2D(leftDescendant->GetGridCoord().x, leftDescendant->GetGridCoord().y);
			}
			else
			{
				// right: place node along line from parent to right-most leaf node
				leafPos = Point2D(rightDescendant->GetGridCoord().x, rightDescendant->GetGridCoord().y);
			}

			if(parent->GetGridCoord().x != mean.x)
			{
				Point2D parentPos(parent->GetGridCoord().x, parent->GetGridCoord().y);
				Point2D pt = Geometry::VerticalIntersect(Line2D(parentPos, leafPos), mean.x);
				curNode->SetGridCoord(Point3D(pt.x, pt.y, 0.0));	
			}
			else
			{
				// the line between the parent and leaf node is vertical so place the node
				// along this line at the desired depth
				curNode->SetGridCoord(Point3D(mean.x, tree->GetDepth(curNode)*depthStep, 0.0));
			}
		}
	}

	TransformTreeLayout(tree, layoutLine, height, bHorizontal);
}

void GeoTreeLinearLayout::LayoutPhylogram2D(Tree<NodeGeoTree>::Ptr tree, LayoutLinePtr layoutLine, float height, bool bHorizontal)
{
	// The tree is initially laid out within a unit square in the 
	// x,y plane with leaf nodes along the x-axis. It is later scaled, 
	// rotated, and translate into the desired position.

	// Note: The algorithm used here to layout the tree is rather inefficent as it will visit nodes
	// before there is sufficent information to determine where they should be placed. This can certainly
	// be improved!

	// clear grid position of all nodes
	std::vector<NodeGeoTree*> nodes = tree->GetNodes();
	foreach(NodeGeoTree* node, nodes)
	{
		node->SetGridCoord(Point3D(Node::NO_DISTANCE, Node::NO_DISTANCE, Node::NO_DISTANCE));
	}

	// evenly distribute leaf nodes along the layout line
	std::vector<NodeGeoTree*> leafNodes = tree->GetLeaves();
	float maxDistToLeaf = tree->GetDistToFurthestLeafNode(tree->GetRootNode());	
	float dx = 1.0 / (leafNodes.size()-1);	
	for(uint i = 0; i < leafNodes.size(); ++i)
	{
		float distToRoot = tree->GetDistToRoot(leafNodes.at(i));
		leafNodes.at(i)->SetGridCoord(Point3D(i*dx, 1.0f - distToRoot/maxDistToLeaf, 0.0));
	}

	// calculate position of internal nodes
	std::vector<NodeGeoTree*> curNodes = tree->GetLeaves();
	while(!curNodes.empty())
	{
		std::vector<NodeGeoTree*> parentNodes; 
		foreach(NodeGeoTree* curNode, curNodes)
		{
			NodeGeoTree* parent = curNode->GetParent();
			if(parent->GetGridCoord().x == Node::NO_DISTANCE)
			{
				// set x-position of parent to the mean x-position of its children
				std::vector<NodeGeoTree*> children = parent->GetChildren();
				std::vector<Point3D> points;
				bool bProcessParent = true;

				foreach(NodeGeoTree* child, children)
				{
					Point3D leafCoord = child->GetGridCoord();
					if(leafCoord.x == Node::NO_DISTANCE)
					{
						bProcessParent = false;
						break;
					}

					points.push_back(leafCoord);
				}

				if(!bProcessParent)
				{
					// at least one child has not had its position determined yet
					continue;
				}

				// calculate mean position of leaf nodes
				Point3D mean;
				CalculateMean(points, mean);

				// calculate position of parent node
				float dist = children.at(0)->GetGridCoord().y + children.at(0)->GetDistanceToParent()/maxDistToLeaf;
				parent->SetGridCoord(Point3D(mean.x, dist, 0.0));

				if(!parent->IsRoot())
					parentNodes.push_back(parent);
			}
		}

		curNodes = parentNodes;
	}

	TransformTreeLayout(tree, layoutLine, height, bHorizontal);
}

void GeoTreeLinearLayout::TransformTreeLayout(Tree<NodeGeoTree>::Ptr tree, LayoutLinePtr layoutLine, float height, bool bHorizontal)
{
	// scale tree to length of layout line
	float treeWidth = layoutLine->GetLength();
	float treeHeight = height*treeWidth;

	// rotate tree to desired angle
	Point3D layoutVec = layoutLine->GetDirection();
	layoutVec.y = 0;
	float layoutAngle = layoutVec.Angle(Point3D(1, 0, 0));
	if(layoutVec.z > 0)
	{
		// Angle() returns the smallest angle between two vectors. If the layout line goes in 
		// the positive z direction than it is a CW angle so should be negative.
		layoutAngle = -layoutAngle;
	}

	float cosAngle = cos(layoutAngle);
	float sinAngle = sin(layoutAngle);

	Point3D rootPos = tree->GetRootNode()->GetGridCoord();
	m_implicitRoot = Point3D(rootPos.x, rootPos.y + 0.01, 0.0);
	if(bHorizontal)
	{
		m_implicitRoot.x = treeWidth*m_implicitRoot.x;
		m_implicitRoot.y = -treeHeight*m_implicitRoot.y;
		m_implicitRoot.x  += layoutLine->GetLayoutOffset();

		float originalX = m_implicitRoot.x;
		m_implicitRoot.x = m_implicitRoot.x*cosAngle - m_implicitRoot.y*sinAngle;
		m_implicitRoot.z = -(originalX*sinAngle + m_implicitRoot.y*cosAngle);
		m_implicitRoot.y = 0;
	}
	else	// vertical
	{
		m_implicitRoot.x = treeWidth*m_implicitRoot.x;
		m_implicitRoot.y = treeHeight*m_implicitRoot.y;
		m_implicitRoot.x += layoutLine->GetLayoutOffset();
		m_implicitRoot.z = -m_implicitRoot.x*sinAngle;
		m_implicitRoot.x = m_implicitRoot.x*cosAngle;
	}

	// translate
	m_implicitRoot.x += layoutLine->GetStartControlPoint().x;
	m_implicitRoot.z += layoutLine->GetStartControlPoint().z;

	// scale, rotate, and translate each node in the tree
	std::vector<NodeGeoTree*> nodes = tree->GetNodes();
	foreach(NodeGeoTree* node, nodes)
	{
		Point3D pos = node->GetGridCoord();

		if(bHorizontal)
		{
			// scale
			pos.x = treeWidth*pos.x;
			pos.y = -treeHeight*pos.y; // the negative is because the tree should be drawn on the RHS of the layout line
			// as one walks from the start to the end of the line

			// offset position of nodes so tree start to the 'right' of the control point of the layout line
			pos.x += layoutLine->GetLayoutOffset();

			// rotate tree to desired angle in x,z plane
			float originalX = pos.x;
			pos.x = pos.x*cosAngle - pos.y*sinAngle;
			pos.z = -(originalX*sinAngle + pos.y*cosAngle);
			pos.y = 0;
		}
		else // vertical
		{
			// scale
			pos.x = treeWidth*pos.x;
			pos.y = treeHeight*pos.y;

			// offset position of nodes so tree start to the 'right' of the control point of the layout line
			pos.x += layoutLine->GetLayoutOffset();

			// rotate tree to desired angle in x,y plane	
			pos.z = -pos.x*sinAngle;
			pos.x = pos.x*cosAngle;
		}

		// translate
		pos.x += layoutLine->GetStartControlPoint().x;
		pos.z += layoutLine->GetStartControlPoint().z;

		node->SetGridCoord(pos);
	}
}

bool GeoTreeLinearLayout::CalculateMean(const std::vector<Point3D>&  points, Point3D& mean)
{
	float x = 0;
	float y = 0;
	float z = 0;
	for(uint i = 0; i < points.size(); ++i)
	{
		if(points.at(i).x == Node::NO_DISTANCE)
		{
			mean = Point3D(Node::NO_DISTANCE, Node::NO_DISTANCE, Node::NO_DISTANCE);
			return false;
		}

		x += points.at(i).x;
		y += points.at(i).y;
		z += points.at(i).z;
	}

	x = x / points.size();
	y = y / points.size();
	z = z / points.size();

	mean.x = x;
	mean.y = y;
	mean.z = z;

	return true;
}

void GeoTreeLinearLayout::DistributeLeafNodes( std::vector<NodeGeoTree*> &leafNodes ) 
{
	//collapsedScaling determines how much space collapsed subtrees will get on the layout line
	float collapsedScalingFactor = 0.2f;

	//find how many spaces will be adjusted to the collapsed size
	uint numSmallSpaces = 0;
	for (uint i = 0; i < leafNodes.size(); ++i)
	{
		bool nextIsCollapsed = i < leafNodes.size()-1 && leafNodes.at(i+1)->IsCollapsed();
		if ( leafNodes.at(i)->IsCollapsed() && nextIsCollapsed )
		{
			numSmallSpaces++;
		}
	}

	//Set the grid coordinates of the leaf nodes
	float dx = 1.0 / ( ((leafNodes.size() - numSmallSpaces) + numSmallSpaces * collapsedScalingFactor) - 1);
	float currentPoint = 0.0f;
	for(uint i = 0; i < leafNodes.size(); ++i)
	{
		leafNodes.at(i)->SetGridCoord(Point3D(currentPoint, 0.0, 0.0));

		bool nextIsCollapsed = i < leafNodes.size()-1 && leafNodes.at(i+1)->IsCollapsed();
		if ( leafNodes.at(i)->IsCollapsed() && nextIsCollapsed )
		{
			currentPoint += dx*collapsedScalingFactor;
		}
		else
		{
			currentPoint += dx;
		}
	}
}
