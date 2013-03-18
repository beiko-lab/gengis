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

#include "../core/App.hpp"
#include "../core/GeoTreeOptLeafOrder.hpp"
#include "../core/GeoTreeView.hpp"
#include "../core/Viewport.hpp"
#include "../core/LocationView.hpp"
#include "../core/VisualLine.hpp"

#include "../utils/TreeTools.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/Geometry.hpp"

#include "../gui/ProgressDlg.hpp"

using namespace GenGIS;

typedef struct sSLOPE_INFO
{
	sSLOPE_INFO(uint _indexI, uint _indexJ, double rise, double run)
		: indexI(_indexI), indexJ(_indexJ)
	{
		// record slopes between [90, 270) degrees (i.e., those that are perpendicular to
		// the geographic axis as it rotates from [0, 180) degrees).
		if(run > 0)
		{
			rise = -rise;
			run = -run;
		}

		if(run > -1e-6)
			slope = -std::numeric_limits<float>::max();
		else 
			slope = rise/run;

		// calculate angle
		if(slope != -std::numeric_limits<float>::max())
		{
			angle = atan2(rise, run)*RAD_TO_DEG;
			if(angle < 0)
				angle = 360 + angle;
		}
		else
			angle = 90.0;
	}

	uint indexI;
	uint indexJ;
	double slope;
	double angle;
} SlopeInfo;

bool SlopeInfoSorter(SlopeInfo elem1, SlopeInfo elem2)
{
	return elem1.slope < elem2.slope;
}

typedef struct POSITION
{
	POSITION(float _xPos, float _zPos, uint _index): xPos(_xPos), zPos(_zPos), index(_index) {}

	float xPos;
	float zPos;
	uint index;
} Position;

bool PositionSorter(Position elem1, Position elem2)
{
	if(elem1.xPos == elem2.xPos)
		return elem1.zPos > elem2.zPos;

	return elem1.xPos < elem2.xPos;
}

typedef struct sNODE_POSITION
{
	sNODE_POSITION(NodeGeoTree* _node, uint _index): node(_node), index(_index) {}

	NodeGeoTree* node;
	uint index;
} NodePosition;

bool NodePositionSorter(NodePosition elem1, NodePosition elem2)
{
	return elem1.node->GetLayoutPos() < elem2.node->GetLayoutPos();
}

std::vector<LinearResults> GeoTreeOptLeafOrder::PerformLinearAxesAnalysis(Tree<NodeGeoTree>::Ptr tree, NodeGeoTree* rootNode)
{
	std::vector<LinearResults> results;

	// 1. Remove any inactive leaf nodes, O(n)
	std::vector<NodeGeoTree*> leafNodes = tree->GetLeaves();
	std::vector<NodeGeoTree*> remainingLeafNodes;
	foreach(NodeGeoTree* leafNode, leafNodes)
	{
		leafNode->SetNumCrossings(0);
		if(leafNode->GetActive())
			remainingLeafNodes.push_back(leafNode);
	}

	// 2. Get nodes that are at identical geographic positions, O(n^2)
	std::vector< std::vector<NodeGeoTree*> > identicalSets;
	do
	{
		std::vector<NodeGeoTree*> tempLeafNodes;
		std::vector<NodeGeoTree*> identicalSet;

		GeoCoord coord1 = remainingLeafNodes.at(0)->GetLocationView()->GetProjection();

		for(uint i = 0; i < remainingLeafNodes.size(); ++i)
		{
			GeoCoord coord2 = remainingLeafNodes.at(i)->GetLocationView()->GetProjection();

			if(coord1.easting == coord2.easting && coord1.northing == coord2.northing)
				identicalSet.push_back(remainingLeafNodes.at(i));
			else
				tempLeafNodes.push_back(remainingLeafNodes.at(i));
		}

		identicalSets.push_back(identicalSet);

		remainingLeafNodes = tempLeafNodes;
	}while(!remainingLeafNodes.empty());

	// 2. Calculate slope between all pairs of unique points, O(n^2)
	std::vector<SlopeInfo> slopeInfo;
	for(uint i = 0; i < identicalSets.size(); ++i)
	{
		Point3D posI = identicalSets.at(i).at(0)->GetLocationView()->GetPosition();
		for(uint j = i+1; j < identicalSets.size(); ++j)
		{
			Point3D posJ = identicalSets.at(j).at(0)->GetLocationView()->GetPosition();

			double run = (posI.x - posJ.x);
			double rise = -(posI.z - posJ.z);	// correct for positive z being south

			slopeInfo.push_back(SlopeInfo(i, j, rise, run));
		}
	}

	// 2. Sort calculated slopes in ascending order, O(n^2 log(n^2))
	std::sort(slopeInfo.begin(), slopeInfo.end(), SlopeInfoSorter);

	// 3. Set first permutation of projected locations according to their x-coordinate,
	// with locations having identical x-coordinates sorted by their z-coordinate value, O(n log(n))
	std::vector<Position> xPositions;
	for(uint i = 0; i < identicalSets.size(); ++i)
	{
		Point3D pos = identicalSets.at(i).at(0)->GetLocationView()->GetPosition();	
		xPositions.push_back(Position(pos.x, pos.z, i));
	}

	std::sort(xPositions.begin(), xPositions.end(), PositionSorter);

	for(uint i = 0; i < xPositions.size(); ++i)
	{
		std::vector<NodeGeoTree*> nodes = identicalSets.at(xPositions.at(i).index);

		for(uint j = 0; j < nodes.size(); ++j)
			nodes.at(j)->SetLayoutPos(i);
	}

	uint numCrossings = 0;
	BranchAndBoundTreeOpt(rootNode, numCrossings, true);
	results.push_back(LinearResults(270, numCrossings));

	// 4. Calculate number of crossings for each new permutation of locations, O(?)
	ProgressDlgPtr progressDlg(new ProgressDlg(_T("Linear axes analysis"), _T("Calculating crossings for all linear axes..."), slopeInfo.size(), App::Inst().GetMainWindow()));
	for(uint i = 0; i < slopeInfo.size(); ++i)
	{
		if(progressDlg && i % 25 == 0)
			progressDlg->Update(i);

		SlopeInfo si = slopeInfo.at(i);
		if(si.angle == 90.0)	// handled by initial ordering of points
			continue;

		// check for co-linear points
		std::set<uint> colinearPtsIndices;
		uint colinearPairs = 0;
		for(uint j = i+1; j < slopeInfo.size(); ++j)
		{
			if(fabs(si.slope - slopeInfo.at(j).slope) < EPSILON)
			{
				// points *might* be colinear
				std::vector<Point2D> pts;
				Point3D pt1 = identicalSets.at(si.indexI).at(0)->GetLocationView()->GetPosition();
				Point3D pt2 = identicalSets.at(si.indexJ).at(0)->GetLocationView()->GetPosition();
				Point3D pt3 = identicalSets.at(slopeInfo.at(j).indexI).at(0)->GetLocationView()->GetPosition();

				if(Geometry::IsColinear(pt1, pt2, pt3))
				{
					// 3 of the 4 points are colinear AND have the same slope, so all
					// 4 points must be colinear
					colinearPtsIndices.insert(si.indexI);
					colinearPtsIndices.insert(si.indexJ);
					colinearPtsIndices.insert(slopeInfo.at(j).indexI);
					colinearPtsIndices.insert(slopeInfo.at(j).indexJ);
					colinearPairs++;
				}
			}
			else
				break;
		}

		if(colinearPtsIndices.size() == 0)
		{
			// swap points that are tangential to layout line
			uint tempPosI = identicalSets.at(si.indexI).at(0)->GetLayoutPos();
			uint tempPosJ = identicalSets.at(si.indexJ).at(0)->GetLayoutPos();

			std::vector<NodeGeoTree*> nodesI = identicalSets.at(si.indexI);
			for(uint j = 0; j < nodesI.size(); ++j)
				nodesI.at(j)->SetLayoutPos(tempPosJ);

			std::vector<NodeGeoTree*> nodesJ = identicalSets.at(si.indexJ);
			for(uint j = 0; j < nodesJ.size(); ++j)
				nodesJ.at(j)->SetLayoutPos(tempPosI);

			// only record results if next slope is different
			if(i == slopeInfo.size()-1 || si.slope != slopeInfo.at(i+1).slope)
			{
				numCrossings = 0;
				BranchAndBoundTreeOpt(rootNode, numCrossings, true);
				double angle = si.angle+180;
				if(angle > 360)
					angle -= 360;
				results.push_back(LinearResults(360 - si.angle, numCrossings));
			}
		}
		else
		{
			// Special case: we have colinear points. The order of points along the 
			// layout line must be reversed.

			// get current ordering
			std::vector<NodePosition> nodePositions;
			std::set<uint>::iterator it;
			for(it = colinearPtsIndices.begin(); it != colinearPtsIndices.end(); ++it)
				nodePositions.push_back(NodePosition(identicalSets.at(*it).at(0), *it));

			std::sort(nodePositions.begin(), nodePositions.end(), NodePositionSorter);			

			// reverse ordering
			for(uint nodePos = 0; nodePos < nodePositions.size()/2; ++nodePos)
			{
				uint indexI = nodePositions.at(nodePos).index;
				uint indexJ = nodePositions.at(nodePositions.size()-nodePos-1).index;
				uint tempPosI = identicalSets.at(indexI).at(0)->GetLayoutPos();
				uint tempPosJ = identicalSets.at(indexJ).at(0)->GetLayoutPos();

				std::vector<NodeGeoTree*> nodesI = identicalSets.at(indexI);
				for(uint j = 0; j < nodesI.size(); ++j)
					nodesI.at(j)->SetLayoutPos(tempPosJ);

				std::vector<NodeGeoTree*> nodesJ = identicalSets.at(indexJ);
				for(uint j = 0; j < nodesJ.size(); ++j)
					nodesJ.at(j)->SetLayoutPos(tempPosI);
			}

			numCrossings = 0;
			BranchAndBoundTreeOpt(rootNode, numCrossings, true);
			results.push_back(LinearResults(360 - si.angle, numCrossings));

			// advance past colinear points
			i += colinearPairs;
		}
	}

	LinearResults finalResults = results.at(0);
	finalResults.angle = 90;
	results.push_back(finalResults);

	// 5. Print results
	Log::Inst().Write("Results of optimal linear axis analysis:");
	Log::Inst().Write("Slope, Crossings");
	Log::Inst().Write("---------------");
	uint minCrossings = std::numeric_limits<int>::max();
	for(int i = (int)results.size()-1; i >= 0; --i)
	{
		std::string resultStr = StringTools::ToString(results.at(i).angle, 5);
		resultStr += ", " + StringTools::ToString(results.at(i).numCrossings);
		Log::Inst().Write(resultStr);

		if(results.at(i).numCrossings < minCrossings)
			minCrossings = results.at(i).numCrossings;
	}
	Log::Inst().Write("");
	std::string resultStr = "Number of crossings for optimal linear axis = " + StringTools::ToString(minCrossings);
	Log::Inst().Write(resultStr);
	Log::Inst().Write("---------------\n");

	return results;
}

void GeoTreeOptLeafOrder::PrintLayoutOrderOfLocations(const std::vector<NodeGeoTree*>& leaves)
{
	wxString layoutPos;
	for(uint pos = 0; pos < leaves.size(); ++pos)
	{
		for(uint i = 0; i < leaves.size(); ++i)
		{
			if(leaves.at(i)->GetLayoutPos() == pos)
			{
				layoutPos += wxT(" ") + wxString(leaves.at(i)->GetName().c_str());
			}
		}
	}

	Log::Inst().Write(layoutPos);
	Log::Inst().Write("----------------------");
}

typedef struct sGEO_LOCATION_SET
{
	sGEO_LOCATION_SET(uint _index, uint _layoutPos): index(_index), layoutPos(_layoutPos) {}

	uint index;
	uint layoutPos;
} GeoLocationSet;

bool GeoLocationSetPredicate(GeoLocationSet elem1, GeoLocationSet elem2)
{
	return elem1.layoutPos < elem2.layoutPos;
}

bool IdenticalSetPredicate(std::vector<GeoLocationSet> elem1, std::vector<GeoLocationSet> elem2)
{
	return elem1.at(0).layoutPos < elem2.at(0).layoutPos;
}

void GeoTreeOptLeafOrder::OptimizeLeafNodeOrdering(Tree<NodeGeoTree>::Ptr tree, NodeGeoTree* node, uint& numCrossings, bool bOptimize)
{
	numCrossings = 0;

	// remove any inactive leaf nodes
	std::vector<NodeGeoTree*> leafNodes = tree->GetLeaves();
	std::vector<NodeGeoTree*> remainingLeafNodes;
	foreach(NodeGeoTree* leafNode, leafNodes)
	{
		leafNode->SetNumCrossings(0);
		if(leafNode->GetActive())
			remainingLeafNodes.push_back(leafNode);
	}

	// get sets of geographic points that are from the same geographic location
	std::vector<std::vector<GeoLocationSet> > identicalSets;
	do
	{
		std::vector<GeoLocationSet> indenticalSet;
		std::vector<NodeGeoTree*> tempLeafNodes;

		GeoCoord coord1 = remainingLeafNodes.at(0)->GetLocationView()->GetProjection();

		for(uint i = 0; i < remainingLeafNodes.size(); ++i)
		{
			GeoCoord coord2 = remainingLeafNodes.at(i)->GetLocationView()->GetProjection();

			if(coord1.easting == coord2.easting && coord1.northing == coord2.northing)
				indenticalSet.push_back(GeoLocationSet(remainingLeafNodes.at(i)->GetLeafOrderIndex(), remainingLeafNodes.at(i)->GetLayoutPos()));
			else
				tempLeafNodes.push_back(remainingLeafNodes.at(i));
		}

		identicalSets.push_back(indenticalSet);

		remainingLeafNodes = tempLeafNodes;
	}while(!remainingLeafNodes.empty());

	// sort all GeoLocationSet sets so they are in asending order of layout pos
	for(uint i = 0; i < identicalSets.size(); ++i)
		std::sort(identicalSets.at(i).begin(), identicalSets.at(i).end(), GeoLocationSetPredicate);

	// sort identical sets so they are in asending order of layout pos
	std::sort(identicalSets.begin(), identicalSets.end(), IdenticalSetPredicate);

	// relabel position of geographic points along layout line to reflect those points from the same geographic location
	leafNodes = tree->GetLeaves();
	for(uint i = 0; i < identicalSets.size(); ++i)
	{
		for(uint j = 0; j < identicalSets.at(i).size(); ++j)
			leafNodes.at(identicalSets.at(i).at(j).index)->SetLayoutPos(i);
	}

	// find optimal ordering of leaf nodes in tree
	BranchAndBoundTreeOpt(node, numCrossings, bOptimize);

	// Set layout position of any geographic pts that have identical geographic locations
	// so they reflect the order of the leaf nodes (i.e., so they have no crossings, but
	// are assigned to seperate points along the geographic layout line)

	// 1. Get leaf node with identical layout pos
	std::vector<std::vector<GeoLocationSet> > identicalPosSets;
	remainingLeafNodes = tree->GetLeaves();
	do
	{
		std::vector<GeoLocationSet> indenticalSet;
		std::vector<NodeGeoTree*> tempLeafNodes;
		for(uint i = 0; i < remainingLeafNodes.size(); ++i)
		{
			if(remainingLeafNodes.at(0)->GetLayoutPos() == remainingLeafNodes.at(i)->GetLayoutPos())
			{
				indenticalSet.push_back(GeoLocationSet(remainingLeafNodes.at(i)->GetLeafOrderIndex(), remainingLeafNodes.at(i)->GetLayoutPos()));
			}
			else
			{
				tempLeafNodes.push_back(remainingLeafNodes.at(i));
			}
		}

		identicalPosSets.push_back(indenticalSet);

		remainingLeafNodes = tempLeafNodes;
	}while(!remainingLeafNodes.empty());

	// 2. Sort identical position sets in ascending order
	std::sort(identicalPosSets.begin(), identicalPosSets.end(), IdenticalSetPredicate);

	// 3. Set layout position of each node 
	std::vector<NodeGeoTree*> sortedLeafNodes = tree->GetLeaves();
	uint index = 0;
	for(uint i = 0; i < identicalPosSets.size(); ++i)
	{
		for(uint j = 0; j < identicalPosSets.at(i).size(); ++j)
		{
			sortedLeafNodes.at(identicalPosSets.at(i).at(j).index)->SetLayoutPos(index);
			index++;
		}
	}
}

void GeoTreeOptLeafOrder::BranchAndBoundTreeOpt(NodeGeoTree* node, uint& numCrossings, bool bOptimize)
{
	if(node->IsLeaf())
	{
		node->SetNumCrossings(0);
		return;
	}

	uint numChildren = node->GetNumberOfChildren();

	// optimize ordering of subtrees first
	std::vector<uint> permutationVec;
	for(uint i = 0; i < numChildren; ++i)
	{
		BranchAndBoundTreeOpt(node->GetChild(i), numCrossings, bOptimize);
		permutationVec.push_back(i);
	}

	// Get number of crossings in subtrees below this node
	std::vector<NodeGeoTree*> childNode = node->GetChildren();
	uint childrenCrossings = 0;
	foreach(NodeGeoTree* child, childNode)
		childrenCrossings += child->GetNumCrossings();

	// Get ordering of leaf nodes determine by barycenter heuristic
	// Complexity barycenter heuristic: O(|E|)
	std::vector<uint> baryCenterOrdering;
	BaryCenterOrdering(node, baryCenterOrdering);				

	// Get crossing count for the barycenter heuristic		
	// Complexity: O(|E|log(|Vsmall|))
	for(uint i = 0; i < numChildren; ++i)
		node->SetChild(i, childNode.at(baryCenterOrdering.at(i)));
	uint baryCenterBound = CountCrossingsByInversionNumber(node);

	if(!bOptimize)
	{
		numCrossings = baryCenterBound;
		node->SetNumCrossings(childrenCrossings);
		return;
	}

	// It has been shown that if there is an ordering of points that results in zero crossings,
	// that both the median and barycenter heuristics will find this ordering.
	// see: Graph Drawing: Algorithms for the Visualization of Graphs
	//				by Battista, Eades, Tamassia, Tollis
	uint bound = baryCenterBound;
	if(baryCenterBound == 0)
	{
		node->SetNumCrossings(childrenCrossings);
		return;
	}
	else
	{
		// Create count matrix
		// Complexity: O(|Vsmall|^2) 
		std::vector< std::vector<uint> > countMatrix;
		CreateCountMatrix(node, countMatrix);

		// Get order of children now that they are in the order specified by the barycenter heuristic.
		// It is likely that lower bound will be found more quickly starting from this ordering than
		// the default one.
		childNode = node->GetChildren();	

		// Determine best order of leaf nodes by performing a branch and bound search
		// over the entire permutation tree as represented by a permutation vector.
		std::vector<uint> optOrder = permutationVec;

		// perform branch and bound on the permutation tree in a depth first search manner
		// Complexity: O(?)
		do
		{		
			uint crossingsForPermutation = 0;

			// evaluate current permutation
			// Complexity (worst case): O(|Vsmall|^2)
			for(uint i = 1; i < numChildren; ++i)
			{
				// determine number of crossings for the potentially partial permutation						
				for(uint j = 0; j < i; ++j)
				{
					crossingsForPermutation += countMatrix[permutationVec.at(j)][permutationVec.at(i)];
					if(crossingsForPermutation >= bound)
						break;
				}

				if(i < numChildren-1 && crossingsForPermutation >= bound)
				{
					// No need to explore the rest of the permutation below the current node
					// in the permutation tree. This portion of the tree can be skipped by
					// setting the elements in the permutation vector beyond this node in
					// descending order. This works because std::next_permutation() goes through
					// permutations in a specific order. Namely, it always generates the
					// "lexicographically next greater permutation of the elements".
					//
					// Note: this could be done in O(|Vsmall|) time using a radix sort,
					// however in practice for the size of problem considered here
					// the std::sort will be faster. Notice, that this step only every
					// occurs once per permutation.
					std::sort(permutationVec.begin()+i+1, permutationVec.end(), std::greater<int>());
					break;
				}
			}

			if(crossingsForPermutation < bound)
			{
				bound = crossingsForPermutation;
				optOrder = permutationVec;
			}
		} while(next_permutation(permutationVec.begin(), permutationVec.end()));

		// Set node to the optimal order
		if(bound < baryCenterBound)
		{
			// if the bary center bound is optimal than the children 
			// have already been put in the correct order
			node->RemoveChildren();
			for(uint i = 0; i < numChildren; ++i)
			{
				node->AddChild(childNode.at(optOrder.at(i)));
			}
		}
	}

	// tabulate total number of crossings for tree
	numCrossings += bound;

	node->SetNumCrossings(childrenCrossings + bound);
}

typedef struct sCROSS_SORTER
{
	sCROSS_SORTER(uint _southIndex, uint _northIndex): southIndex(_southIndex), northIndex(_northIndex) {}

	uint southIndex;
	uint northIndex;
} CrossSorter;

// Predicate used to sort items in NodeSorter.
bool CrossSorterPredicate(CrossSorter elem1, CrossSorter elem2)
{
	return elem1.southIndex < elem2.southIndex;
}

uint GeoTreeOptLeafOrder::CountCrossingsByInversionNumber(NodeGeoTree* node, uint maxCrossings)
{
	// Determine lexicographical order of nodes on the north line as read from left to right
	// across the south line. The implementation here takes O(|E|log(|E|)) where E is the 
	// number of edges. It can be done in O(|E|) time by using radix sort. Although radix
	// sort is better theoretically (and in practice for large |E|) I have decided to use
	// std::sort for simplicity and since |E| is fairly small for the cases we are interested in.
	std::vector<NodeGeoTree*> children = node->GetChildren();
	std::vector<CrossSorter> crossSeq;
	for(uint i = 0; i < children.size(); ++i)
	{
		if(children.at(i)->IsLeaf())
		{
			uint pos = children.at(i)->GetLayoutPos();
			if(pos != NOT_SET)
			{
				CrossSorter crossSorter(pos, i);
				crossSeq.push_back(crossSorter);
			}
		}
		else
		{
			std::vector<NodeGeoTree*> grandChildren = TreeTools<NodeGeoTree>::GetLeaves(children.at(i));

			foreach(NodeGeoTree* node, grandChildren)
			{			
				uint pos = node->GetLayoutPos();
				if(pos != NOT_SET)
				{
					CrossSorter crossSorter(pos, i);
					crossSeq.push_back(crossSorter);
				}
			}
		}
	}

	std::sort(crossSeq.begin(), crossSeq.end(), CrossSorterPredicate);

	// Count crossings using accumulator tree
	// See: "Simple and efficient bilayer cross counting" by Barth, W., Junger, M., and Mutzel, P.
	// This takes O(|E|log(|V_small|), where E is the number of edges and V_small is the smaller
	// cardinality node set. In our case, E is the number of leaf nodes in the subtree rooted at 
	// the specified node and V_small is the number of children of this node. Intuitively, the 
	// runtime is O(|E|log(|V_small|)) since for each edge all positions in a balance search tree
	// from the leaf node of this edge to the root must be updated. The height of the balanced
	// search tree is log(|V_small|) so there is log(|V_small|) work for each edge.
	uint firstIndex = 1;
	while(firstIndex < children.size())
		firstIndex *= 2;

	uint treeSize = 2*firstIndex - 1;		// number of nodes in tree	
	firstIndex -= 1;										// index of leftmost leaf
	uint* tree = new uint[treeSize];
	for(uint t = 0; t < treeSize; ++t)
		tree[t] = 0;

	uint crossCount = 0;
	for(uint k = 0; k < crossSeq.size(); ++k)
	{
		uint index = crossSeq.at(k).northIndex + firstIndex;
		tree[index]++;
		while(index > 0)
		{
			if(index%2)
			{
				crossCount += tree[index+1];
				if(crossCount > maxCrossings)
					return crossCount;
			}

			index = (index-1) / 2;
			tree[index]++;
		}
	}

	delete[] tree;

	return crossCount;
}

void GeoTreeOptLeafOrder::CreateCountMatrix(NodeGeoTree* node, std::vector< std::vector<uint> >& countMatrix)
{
	std::vector<NodeGeoTree*> childNodes = node->GetChildren();
	countMatrix.resize(childNodes.size());
	for(uint i = 0; i < childNodes.size(); ++i)
		countMatrix.at(i).resize(childNodes.size());

	// count crossings between all pairs of children 
	for(uint i = 0; i < childNodes.size(); ++i)
	{
		for(uint j = 0; j < childNodes.size(); ++j)
		{
			if(i != j)
			{
				NodeGeoTree tempNode(0);
				tempNode.AddChild(childNodes[i]);
				tempNode.AddChild(childNodes[j]);
				countMatrix[i][j] = CountCrossingsByInversionNumber(&tempNode);
			}
			else 
				countMatrix[i][j] = 0;
		}
	}

	// reset all children to point to the parent node since they will be modified above
	for(uint i = 0; i < childNodes.size(); ++i)
	{
		childNodes.at(i)->SetParent(node);
	}
}

uint GeoTreeOptLeafOrder::CanonicalLowerBound(const std::vector< std::vector<uint> >& countMatrix)
{
	uint lowerBound = 0;
	for(uint i = 0; i < countMatrix.size()-1; ++i)
	{
		for(uint j = i+1; j < countMatrix.size(); ++j)
		{
			uint c_ij = countMatrix[i][j];
			uint c_ji = countMatrix[j][i];

			if(c_ij <= c_ji)
				lowerBound += c_ij;
			else
				lowerBound += c_ji;
		}
	}

	return lowerBound;
}

typedef struct sHEURISTIC_SORTER
{
	sHEURISTIC_SORTER(double _heuristicValue, uint _index, uint _degree): heuristicValue(_heuristicValue), index(_index), degree(_degree) {}

	double heuristicValue;
	uint index;
	uint degree;
} HeuristicSorter;

// Predicate used to sort items in NodeSorter.
int HeuristicSorterPredicate(const void* _elem1, const void* _elem2)
{
	HeuristicSorter* elem1 = (HeuristicSorter*) _elem1;
	HeuristicSorter* elem2 = (HeuristicSorter*) _elem2;

	if(elem1->heuristicValue < elem2->heuristicValue)
		return -1;
	else if(elem1->heuristicValue > elem2->heuristicValue)
		return 1;
	else
	{
		// if one of the elements has odd degree and the other even, than the
		// odd degree vertex should be placed on the left. If both have the
		// same degree than the order is arbitrary.
		// see: Graph Drawing: Algorithms for the Visualization of Graphs
		//				by Battista, Eades, Tamassia, Tollis
		if( ( elem1->degree % 2 ) ) 
			return -1; 

		return 1;
		

		// Note: Weird way to write the above relationship so the it is a strictly weak ordering
		// as required by STL
		//if( ( elem1.degree % 2 ) >= ( elem2.degree % 2 ) ) 
		//    return true; 
	}

	return -1;
}

void GeoTreeOptLeafOrder::BaryCenterOrdering(NodeGeoTree* node, std::vector<uint>& baryCenterOrdering)
{
	// Calculate the barycenter (average) for each subtree. This heuristic finds an O(sqrt(n))
	// -approximation solution or a (d−1)-approximation solution, where d is the maximum degree of 
	// nodes in the free side.

	// This algorithm assumes the barycenter of all children node (except leaf nodes) has already been
	// calculated. In this way, the barycenter can more efficently be calculated.
	std::vector<NodeGeoTree*> childNodes = node->GetChildren();
	std::vector<HeuristicSorter> baryCenters;
	for(uint i = 0; i < childNodes.size(); ++i)
	{
		NodeGeoTree* curNode = childNodes.at(i);

		if(curNode->IsLeaf())
			baryCenters.push_back(HeuristicSorter(curNode->GetLayoutPos(), i, 0));
		else
		{
			std::vector<NodeGeoTree*> childChildNodes = curNode->GetChildren();

			uint baryCenter = 0;
			uint divisor = 0;
			foreach(NodeGeoTree* node, childChildNodes)
			{
				if(node->IsLeaf())
				{
					uint pos = node->GetLayoutPos();
					if(pos != NOT_SET)
					{
						baryCenter += pos;
						divisor++;
					}
				}
				else
				{
					uint pos = node->GetBaryCenter();
					if(pos != NOT_SET)
					{
						baryCenter += node->GetBaryCenter()*node->GetNumberOfChildren();
						divisor += node->GetNumberOfChildren();
					}
				}
			}

			if(divisor != 0)
			{
				baryCenter /= divisor;
			}
			else
			{
				// the placement of nodes where none of the leaves are attached to geographic locations does not matter
				// so just place it at the end and mark the internal node as being unset
				baryCenter = NOT_SET;
			}

			baryCenters.push_back(HeuristicSorter(baryCenter, i, childNodes.at(i)->GetNumberOfChildren()));
			curNode->SetBaryCenter(baryCenter);
		}		
	}

	qsort(&baryCenters.front(), baryCenters.size(), sizeof(HeuristicSorter), HeuristicSorterPredicate);

	for(uint i = 0; i < baryCenters.size(); ++i)
		baryCenterOrdering.push_back(baryCenters.at(i).index);
}

void GeoTreeOptLeafOrder::MedianOrdering(NodeGeoTree* node, std::vector<uint>& medianOrdering)
{
	// Calculate the median for each subtree. This heuristic finds an 3-approximation solution,
	// but in NodeGeoTree is often worse than the bartcenter heuristic.
	std::vector<NodeGeoTree*> childNodes = node->GetChildren();
	std::vector<HeuristicSorter> medians;
	for(uint i = 0; i < childNodes.size(); ++i)
	{
		std::vector<NodeGeoTree*> leafNodes = TreeTools<NodeGeoTree>::GetLeaves(node->GetChild(i));
		std::vector<uint> leafPos;
		for(uint j = 0; j < leafNodes.size(); ++j)
		{
			uint pos = leafNodes.at(j)->GetLayoutPos();
			if(pos != NOT_SET)
			{
				leafPos.push_back(pos);
			}
		}

		std::sort(leafPos.begin(), leafPos.end());
		uint median = leafPos.at(leafPos.size()/2);
		medians.push_back(HeuristicSorter(median, i, childNodes.at(i)->GetNumberOfChildren()));
	}

	qsort(&medians.front(), medians.size(), sizeof(HeuristicSorter), HeuristicSorterPredicate);

	for(uint i = 0; i < medians.size(); ++i)
	{
		medianOrdering.push_back(medians.at(i).index);
	}
}

double GeoTreeOptLeafOrder::SignificanceTest(Tree<NodeGeoTree>::Ptr tree, NodeGeoTree* node, uint iterations, std::map<uint, uint>& pdf, uint& origNumCrossings, bool& bSuccess)
{
	bSuccess = true;

	ProgressDlgPtr progressDlg(new ProgressDlg(_T("Significance Test"), _T("Iteration: 0"), iterations, App::Inst().GetMainWindow()));

	// get order of leaf nodes
	std::vector<uint> leafOrder;
	std::vector<NodeGeoTree*> leafNodes = TreeTools<NodeGeoTree>::GetLeaves(node);
	std::vector<NodeGeoTree*> activeLeafNodes;
	foreach(NodeGeoTree* leaf, leafNodes)
	{
		if(leaf->GetActive())
		{
			leafOrder.push_back(leaf->GetLayoutPos());
			activeLeafNodes.push_back(leaf);
		}
	}

	// get number of crossings for original association of leaf nodes to geographical locations
	origNumCrossings = 0;
	OptimizeLeafNodeOrdering(tree, node, origNumCrossings, true);

	// Create probability mass function by holding the tree topology, geographic axis,
	// and association between leaf nodes and geographic locations constant. Only the 
	// order of geographic locations along the geographic layout line are randomized. 
	std::vector<uint> rndPermutation = leafOrder;
	for(uint i = 0; i < iterations; ++i)
	{
		// randomly permute association between leaf node and geographical location
		RandomPermutation(rndPermutation);
		for(uint j = 0; j < activeLeafNodes.size(); ++j)
			activeLeafNodes.at(j)->SetLayoutPos(rndPermutation.at(j));

		uint numCrossings = 0;
		OptimizeLeafNodeOrdering(tree, node, numCrossings, true);

		std::map<uint, uint>::iterator iter;
		iter = pdf.find(numCrossings);
		if(iter == pdf.end())
			pdf[numCrossings] = 1;
		else
			iter->second++;

		if(!progressDlg->Update(i, (_T("Iteration: ") + StringTools::ToStringW(i)).c_str()))
		{
			Log::Inst().Warning("(Warning) Significance test terminated.");
			bSuccess = false;
			return 1.0f;
		}
	}

	// determine p-value for original association of leaf nodes to geographical locations
	std::map<uint, uint>::iterator iter;
	uint pValueCount = 0;
	for(iter = pdf.begin(); iter != pdf.end(); ++iter)
	{
		if(iter->first <= origNumCrossings)
			pValueCount += iter->second;
	}
	double pValue = float(pValueCount) / iterations;	

	// restore original order and statistics for tree
	for(uint i = 0; i < activeLeafNodes.size(); ++i)
		activeLeafNodes.at(i)->SetLayoutPos(leafOrder.at(i));

	uint numCrossings = 0;
	OptimizeLeafNodeOrdering(tree, node, numCrossings, true);

	return pValue;
}

void GeoTreeOptLeafOrder::RandomPermutation(std::vector<uint>& leafOrder)
{
	// randomly permute order of leaf nodes
	// see: http://www.algoblog.com/2007/06/04/permutation/
	for(uint i = 1; i < leafOrder.size(); ++i)
	{
		uint j = rand() % (i+1);
		uint temp = leafOrder.at(i);
		leafOrder.at(i) = leafOrder.at(j);
		leafOrder.at(j) = temp;
	}
}

double GeoTreeOptLeafOrder::CalculateLengthOfLocationLines(Tree<NodeGeoTree>::Ptr tree, const VisualLine& geographyLine)
{
	// Remove inactive leaf nodes
	std::vector<NodeGeoTree*> leafNodes = tree->GetLeaves();
	std::vector<NodeGeoTree*> activeLeafNodes;
	foreach(NodeGeoTree* leafNode, leafNodes)
	{
		if(leafNode->GetActive())
			activeLeafNodes.push_back(leafNode);
	}

	// Calculate length of all location lines
	double length = 0;
	Point3D startPt = geographyLine.GetLine().start;
	Point3D endPt = geographyLine.GetLine().end;
	double spacing = Geometry::Distance(startPt, endPt) / (activeLeafNodes.size()-1);
	Point3D geographyLineVector = endPt - startPt;
	geographyLineVector = geographyLineVector.Normalize();
	foreach(NodeGeoTree* leafNode, leafNodes)
	{
		Point3D geographyPt = startPt + (leafNode->GetLayoutPos()*spacing)*geographyLineVector;
		length += Geometry::Distance(leafNode->GetGridCoord(), geographyPt);
	}

	return length;
}