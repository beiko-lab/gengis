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
#include "../core/GeoTreeView.hpp"
#include "../core/Camera.hpp"
#include "../core/GeoTreeIO.hpp"
#include "../core/GeoTreeEllipticalLayout.hpp"
#include "../core/GeoTreeLinearLayout.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/MapLayer.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/SequenceController.hpp"
#include "../core/TreeLayer.hpp"
#include "../core/LayoutLine.hpp"
#include "../core/LayoutEllipse.hpp"
#include "../core/Viewport.hpp"
#include "../core/MapController.hpp"
#include "../core/GeoTreeOptLeafOrder.hpp"
#include "../core/GeoAxisPolyline.hpp"
#include "../core/OrthoLayers.hpp"
#include "../core/PopupMenu.hpp"
#include "../core/VisualLabel.hpp"
#include "../core/LayoutObjectProperties.hpp"

#include "../utils/TreeTools.hpp"
#include "../utils/ErrorGL.hpp"
#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/Geometry.hpp"
#include "../utils/Font.hpp"

#include "../gui/TreePropertiesDlg.hpp"
#include "../gui/ProgressDlg.hpp"

#include "../core/NodeGeoTree.hpp"
#include "../utils/Tree.hpp"

using namespace GenGIS;

const int NODE_DEPTH = TREE_LAYER-10;
const int MAX_DEGREE_FOR_OPTIMIZING_TREE = 9;


// Structure used to sort nodes.
typedef struct sNODE_SORTER
{
	sNODE_SORTER(int _index, float _distance): index(_index), distance(_distance) {}

	int index;
	float distance;
} NodeSorter;

// Predicate used to sort items in NodeSorter.
bool NodeSortPredicate(NodeSorter elem1, NodeSorter elem2)
{
	return elem1.distance < elem2.distance;
}

GeoTreeView::GeoTreeView(Tree<NodeGeoTree>::Ptr tree, boost::shared_ptr<Layer> mapLayer) :
	m_tree(tree),
	m_mapLayer(mapLayer),
	m_originalTree(tree),
	m_layout(NONE),
	m_orientation(HORIZONTAL),
	m_bShowLabels(false),
	m_markerQuadric(gluNewQuadric()),
	m_height(0.3f),
	m_colour(Colour(157, 78, 163)),
	m_lineThickness(3.0f),
	m_bOptimizeTopology(true),
	m_fontColour(Colour(1.0f, 1.0f, 1.0f, 1.0f)),
	m_fontSize(12),
	m_leafNodeSize(5),
	m_internalNodeSize(4),
	m_activeView(NO_ACTIVE_VIEW),
	m_bActiveViewMoved(false),
	m_linearLayout(new GeoTreeLinearLayout()),
	m_ellipticalLayout(new GeoTreeEllipticalLayout()),
	m_bSpreadGeoPts(true),
	m_bSelectedFirstGeoLineCP(false),
	m_geographyLineOffsetPercentage(0.10f),
	m_locationLines(Colour(230, 171, 2)),
	m_correlationLines(Colour(230, 171, 2)),
	m_phylogramDropLines(Colour(230, 171, 2), 1.0f, VisualLine::SHORT_DASH),
	m_geoPts(Colour(230, 171, 2)),
	m_bReverseOrderGeographicPts(false),
	m_leafNodeBorderSize(1.0),
	m_leafNodeBorderColour(Colour(0,0,0)),
	m_internalNodeBorderSize(1.0),
	m_internalNodeBorderColour(Colour(0,0,0)),
	m_geoPtsBorderSize(1.0), m_geoPtsBorderColour(Colour(0,0,0)),
	m_locationLineBorderSize(1.0),
	m_locationLineBorderColour(Colour(0,0,0)),
	m_correlationLineBorderSize(1.0),
	m_correlationLineBorderColour(Colour(0,0,0)),
	m_branchBorderSize(1.0),
	m_branchBorderColour(Colour(0,0,0)),
	m_treeColourStyle(COLOUR_SINGLE),
	m_selectedNode(NULL)
{
	m_geographyLine = VisualLine(App::Inst().GetLayoutObjProp()->GetLayoutLineColour(),
		App::Inst().GetLayoutObjProp()->GetLayoutLineThickness(),
		App::Inst().GetLayoutObjProp()->GetLayoutLineStyle());

	m_3dInternalDropLine.SetColour(Colour(128, 128, 128));
	m_3dInternalDropLine.SetSize(1.0f);
	m_3dInternalDropLine.SetVisibility(true);
	m_3dInternalDropLine.SetLineStyle(VisualLine::SHORT_DASH);

	m_3dLeafDropLine.SetColour(Colour(128, 128, 128));
	m_3dLeafDropLine.SetSize(1.0f);
	m_3dLeafDropLine.SetVisibility(true);
	m_3dLeafDropLine.SetLineStyle(VisualLine::SHORT_DASH);

	m_correlationLines.SetThickness(3.0f);
	m_locationLines.SetThickness(3.0f);
}

template<class Archive>
void GeoTreeView::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_originalTree;                  // Tree<NodeGeoTree>::Ptr
	ar & m_treeLayerId;                   // uint
	//ar & m_sigNodeUpdate;                 // SigNodeUpdate
	ar & m_layout;                        // LAYOUT
	ar & m_orientation;                   // ORIENTATION
	ar & m_layoutLine;                    // LayoutLinePtr
	ar & m_layoutEllipse;                 // LayoutEllipsePtr
	ar & m_geoAxisPolyline;               // GeoAxisPolylinePtr
	//ar & m_markerQuadric;                 // GLUquadricObj*
	ar & m_locationLines;                 // VisualLines
	ar & m_correlationLines;              // VisualLines
	ar & m_geographyLine;                 // VisualLine
	ar & m_phylogramDropLines;            // VisualLines
	ar & m_geoPts;                        // VisualPoints
	ar & m_modLayoutLine;                 // Line3D
	ar & m_modGeographyLine;              // VisualLine
	ar & m_font;                          // FontPtr
	ar & m_bShowLabels;                   // bool
	ar & m_fontFile;                      // std::string
	ar & m_fontSize;                      // uint
	ar & m_fontColour;                    // Colour
	ar & m_lineThickness;                 // float
	ar & m_height;                        // float
	ar & m_colour;                        // Colour
	ar & m_leafNodeSize;                  // float
	ar & m_internalNodeSize;              // float
	ar & m_bOptimizeTopology;             // bool
	ar & m_activeView;                    // ACTIVE_VIEW
	ar & m_bActiveViewMoved;              // bool
	ar & m_linearLayout;                  // GeoTreeLinearLayoutPtr
	ar & m_ellipticalLayout;              // GeoTreeEllipticalLayoutPtr
	ar & m_geographyLineOffsetPercentage; // float
	ar & m_bSpreadGeoPts;                 // bool
	ar & m_selectedNode;                  // NodeGeoTree*
	ar & m_bSelectedFirstGeoLineCP;       // bool
	ar & m_bReverseOrderGeographicPts;    // bool
	ar & m_3dInternalDropLine;            // VisualLine
	ar & m_3dLeafDropLine;                // VisualLine
	ar & m_leafNodeBorderSize;            // float
	ar & m_leafNodeBorderColour;          // Colour
	ar & m_internalNodeBorderSize;        // float
	ar & m_internalNodeBorderColour;      // Colour
	ar & m_geoPtsBorderSize;              // float
	ar & m_geoPtsBorderColour;            // Colour
	ar & m_locationLineBorderSize;        // float
	ar & m_locationLineBorderColour;      // Colour
	ar & m_correlationLineBorderSize;     // float
	ar & m_correlationLineBorderColour;   // Colour
	ar & m_branchBorderSize;              // float
	ar & m_branchBorderColour;            // Colour
	ar & m_treeColourStyle;               // TREE_COLOUR_STYLE
	ar & m_linearAxesResults;             // std::vector<GeoTreeOptLeafOrder::LinearResults>

	if ( Archive::is_loading::value )
	{
		SignalNodeUpdate(boost::bind(&GenGisFrame::SigNodeUpdate, ((GenGisFrame*)App::Inst().GetMainWindow()), _1));
	}
}
template void GeoTreeView::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void GeoTreeView::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void GeoTreeView::ProjectTree(std::vector<std::wstring>& names)
{
	m_tree = m_originalTree->Clone();

	// mark all internal node as active so we can distinguish them from true leaf nodes
	std::vector<NodeGeoTree*> nodes = m_tree->GetNodes();
	foreach(NodeGeoTree* node, nodes)
	{
		node->SetActive(!node->IsLeaf());
	}

	// Remove all leaf nodes not in projection set.

	// 1. Create dictionary mapping names to leaf nodes.
	std::vector<NodeGeoTree*> leafNodes = m_tree->GetLeaves();
	std::map<std::wstring, NodeGeoTree*> namesToNodes;
	foreach(NodeGeoTree* leaf, leafNodes)
	{
		namesToNodes.insert(std::pair<std::wstring, NodeGeoTree*>(leaf->GetName(), leaf));
	}

	// 2. Remove all nodes contained in the projection set.
	std::map<std::wstring, NodeGeoTree*>::iterator it;
	foreach(const std::wstring& name, names)
	{
		it = namesToNodes.find(name);
		if(it != namesToNodes.end())
			namesToNodes.erase(it);
	}

	// 3. Delete all remaining nodes in the dictionary.
	for(it = namesToNodes.begin(); it != namesToNodes.end(); ++it)
	{
		NodeGeoTree* deadNode = it->second;
		deadNode->GetParent()->RemoveChild(deadNode);
	}

	// collapse any internal nodes that have less than 2 children. This is 
	// done in a breadth first manner from the leaf nodes to the root node.
	std::vector<NodeGeoTree*> curNodes = m_tree->GetLeaves();
	std::set<NodeGeoTree*> nextNodes;
	while(!curNodes.empty())
	{
		nextNodes.clear();
		foreach(NodeGeoTree* node, curNodes)
		{
			if(!node->IsRoot())
				nextNodes.insert(node->GetParent());

			if(node->GetActive() && node->GetNumberOfChildren() == 0)
			{
				if(node->IsRoot())
				{
					// we have a root with no children so just leave it as the sole node in the tree
				}
				else
				{
					// remove this node from the tree
					node->GetParent()->RemoveChild(node);
					nextNodes.erase(node);
					delete node;
				}
			}
			else if(node->GetActive() && node->GetNumberOfChildren() == 1)
			{		
				if(node->IsRoot())
				{
					// the root is degenerate so we must make its sole child the new root
					node->GetChild(0)->SetParent(NULL);
					node->GetChild(0)->SetDistanceToParent(Node::NO_DISTANCE);
					m_tree->SetRootNode(node->GetChild(0));
					nextNodes.erase(node);
					delete node;
				}
				else
				{
					// remove node from tree after assigning its sole child to its parent
					node->GetParent()->AddChild(node->GetChild(0));	

					if(node->GetChild(0)->GetDistanceToParent() != Node::NO_DISTANCE)
					{
						// keep track of branch lengths
						node->GetChild(0)->SetDistanceToParent(node->GetChild(0)->GetDistanceToParent() 
							+ node->GetDistanceToParent()); 
					}

					node->GetParent()->RemoveChild(node);
					nextNodes.erase(node);
					delete node;
				}
			}	
		}

		curNodes.assign(nextNodes.begin(), nextNodes.end());
	}

	// default state of the active flag should be true
	nodes = m_tree->GetNodes();
	foreach(NodeGeoTree* node, nodes)
	{
		node->SetActive(true);
	}

	ForceTreeLayout();
}

void GeoTreeView::ProjectToActiveSet(std::vector<LocationLayerPtr>& locationLayers)
{
	std::vector<std::wstring> allLoc;
	std::vector<std::wstring> visibleLoc;
	std::vector<std::wstring> visibleSeqs;
	for(uint i = 0; i < locationLayers.size(); ++i)
	{
		allLoc.push_back(locationLayers[i]->GetName());
		if(locationLayers[i]->IsActive())
		{
			visibleLoc.push_back(locationLayers[i]->GetName());

			const std::vector<SequenceLayerPtr> seqLayerPtrs = locationLayers[i]->GetAllSequenceLayers();
			foreach(const SequenceLayerPtr seqLayerPtr, seqLayerPtrs)
			{
				if(seqLayerPtr->IsActive() && seqLayerPtr->GetSequenceController()->IsActive())
					visibleSeqs.push_back(seqLayerPtr->GetSequenceController()->GetSequenceId());
			}
		}
	}

	// check if tree is based on locations or sequences
	std::vector<std::wstring> leaves = m_tree->GetLeafNames();

	RestoreTree();
	
	// assume projecting to locations if there are no leaves
	if(leaves.size() == 0)
	{
		// this is a tree with locations as leaf nodes
		ProjectTree(visibleLoc);
	}
	else if(std::find(allLoc.begin(), allLoc.end(), leaves.at(0)) != allLoc.end())
	{
		// this is a tree with locations as leaf nodes
		ProjectTree(visibleLoc);
	}
	else
	{
		// this is a tree with sequence as leaf nodes
		ProjectTree(visibleSeqs);
	}
}

void GeoTreeView::SetOrientation(ORIENTATION orientation) 
{
	if(m_orientation != orientation)
	{
		m_orientation = orientation;

		if(m_layout == CLADOGRAM_2D || m_layout == SLANTED_CLADOGRAM_2D || m_layout == PHYLOGRAM_2D)
			LayoutTrees2D();
		else if(m_layout == ELLIPSE_QUADRANT)
			m_ellipticalLayout->LayoutEllipseQuadrant(m_tree, m_layoutEllipse, m_height);
	}
}

void GeoTreeView::SetHeight(float height) 
{ 
	if(m_height != height)
	{
		m_height = height; 
		if(m_layout == CLADOGRAM_2D || m_layout == SLANTED_CLADOGRAM_2D || m_layout == PHYLOGRAM_2D)
			LayoutTrees2D();
		else if(m_layout == CLADOGRAM_3D || m_layout == SLANTED_CLADOGRAM_3D || m_layout == SLANTED_PHYLOGRAM_3D)
			LayoutTree3D();
		else if(m_layout == ELLIPSE_QUADRANT)
			m_ellipticalLayout->LayoutEllipseQuadrant(m_tree, m_layoutEllipse, m_height);
	}
}

void GeoTreeView::SetFontSize(uint size) 
{
	m_fontSize = size; 
}

Colour GeoTreeView::GetLocationColour(const std::wstring& leafId) const
{
	NodeGeoTree* node = m_tree->GetNode(leafId);
	if(node != NULL)
	{
		return node->GetLocationView()->GetColour();
	}

	return Colour(0.0f,0.0f,0.0f,1.0f);
}

void GeoTreeView::Render()
{
	error::ErrorGL::Check();

	if(m_bVisible)
	{
		glDisable(GL_LIGHTING);

		if(m_layout == CLADOGRAM_3D || m_layout == SLANTED_CLADOGRAM_3D)
		{
			RenderTree();
			RenderLabels();
		}
		else if(m_layout == SLANTED_PHYLOGRAM_3D)
		{
			RenderTree();
			RenderPhylogramDropLines3D();
			RenderLabels();
		}
		else if(m_layout == CLADOGRAM_2D || m_layout == SLANTED_CLADOGRAM_2D || m_layout == PHYLOGRAM_2D)
		{
			RenderTree();
			RenderLabels();
			RenderCorrelationLines();
			RenderGeographyLine();
			RenderGeographicPts();
			RenderLocationLines();
			RenderPhylogramDropLines();
		}

		glEnable(GL_LIGHTING);

		error::ErrorGL::Check();
	}

	if(m_layoutLine)	
	{
		m_layoutLine->SetSelected(m_activeView == LAYOUT_LINE);
		m_layoutLine->Render();
	}

	if(m_layoutEllipse)
	{
		m_layoutEllipse->SetSelected(m_activeView == LAYOUT_ELLIPSE);
		m_layoutEllipse->Render();
	}

	if(m_geoAxisPolyline)
	{
		m_geoAxisPolyline->SetSelected(m_activeView == GEOGRAPHIC_AXIS);
		m_geoAxisPolyline->Render();
	}

}

void GeoTreeView::RenderTree()
{
	error::ErrorGL::Check();

	// set all nodes to indicate that they have not been processed
	std::vector<NodeGeoTree*> nodes = m_tree->GetNodes();
	foreach(NodeGeoTree* node, nodes)
		node->SetProcessed(false);

	// get direction of layout line and tree
	Point3D lineDir, treeDir;
	if(m_layout == CLADOGRAM_2D || m_layout == PHYLOGRAM_2D)
		CalculateTreeDirection(lineDir, treeDir);

	float treeAngle = treeDir.Angle(Point3D(1,0,0)) * RAD_TO_DEG;

	// Render each branch of tree going from the leaf nodes to the root. Note, that it is
	// simpler to render the tree from the root down, but this makes propagating colours
	// up the tree more difficult. The following algorithm is certainly not the most efficent
	// possible, but should be adequate for the size of trees we are dealing with.
	bool bRootRendered = false;
	std::vector<NodeGeoTree*> curNodes = m_tree->GetLeaves();
	while(!curNodes.empty())
	{
		std::vector<NodeGeoTree*> nextNodes;	
		foreach(NodeGeoTree* curNode, curNodes)
		{
			Point3D curCoord = curNode->GetGridCoord();

			// determine colour of branch
			Colour nodeColour = GetNodeColour(curNode);

			// render node
			if(m_layout == SLANTED_CLADOGRAM_3D || m_layout == CLADOGRAM_3D || m_layout == SLANTED_PHYLOGRAM_3D)
				RenderNode3D(curCoord, nodeColour, curNode->IsLeaf(), curNode->GetSelected());
			else
				RenderNode2D(curCoord, nodeColour, curNode->IsLeaf(), curNode->GetSelected());

			// get screen coordinates of grid point	
			NodeGeoTree* parentNode = curNode->GetParent();
			Point3D parentCoord = parentNode->GetGridCoord();

			if(m_layout == SLANTED_CLADOGRAM_3D || m_layout == SLANTED_PHYLOGRAM_3D)
			{
				RenderLine3D(Line3D(parentCoord, curCoord), nodeColour, GetLineThickness(), VisualLine::SOLID,  parentNode->GetSelected());
			}
			else if(m_layout == CLADOGRAM_3D)
			{
				Point3D cornerPt(curCoord.x, parentCoord.y, curCoord.z);
				RenderLine3D(Line3D(parentCoord, cornerPt), nodeColour, GetLineThickness(), VisualLine::SOLID,  parentNode->GetSelected());
				RenderLine3D(Line3D(cornerPt, curCoord), nodeColour, GetLineThickness(), VisualLine::SOLID,  parentNode->GetSelected());
			}
			else if(m_layout == SLANTED_CLADOGRAM_2D)
			{
				VisualLine::RenderLineWithBorder(Line3D(parentCoord, curCoord), nodeColour, GetLineThickness(), VisualLine::SOLID,
					m_branchBorderColour, m_branchBorderSize, TREE_LAYER, parentNode->GetSelected());	
			}
			else if(m_layout == CLADOGRAM_2D || m_layout == PHYLOGRAM_2D)
			{		
				Point3D cornerCoord;
				bool bReverseOrientation = false;
				if(m_orientation == VERTICAL)
					cornerCoord = Point3D(curCoord.x, parentCoord.y, curCoord.z);
				else
				{
					// It isn't exactly ideal to have to do this calculation. A better solution would be
					// to calculate the position of corner points during the layout in the canonical 
					// coordinate system.
					Point3D closestPt;
					Line3D parentLine(parentCoord, parentCoord + treeDir);
					Geometry::ClosestPointToLine(parentLine, curCoord, closestPt);
					double dist = Geometry::Distance(parentCoord, closestPt);

					cornerCoord = Point3D(curCoord.x + treeDir.x*dist, curCoord.y, curCoord.z + treeDir.z*dist);

					// It can get messy to determine which way to offset border lines when you have branches of length zero. This flag is used
					// to indicate the correct orientation. Again, it would be better if everything was just laid out in a canonical coordinate
					// system and then rotated.
					if(Geometry::Distance(cornerCoord, m_layoutLine->GetStartPt()) < Geometry::Distance(parentCoord, m_layoutLine->GetStartPt()))
						bReverseOrientation = true;
				}

				RenderCornerLines(curCoord, cornerCoord, parentCoord, 
					treeAngle, GetLineThickness(), TREE_LAYER,
					nodeColour, GetBranchBorderSize(),
					GetBranchBorderColour(), bReverseOrientation, parentNode->GetSelected());

				if(parentNode->IsRoot() && !bRootRendered)
				{
					bRootRendered = true;
					RenderRootBranch(curCoord, cornerCoord, parentCoord, GetLineThickness(), TREE_LAYER, GetNodeColour(m_tree->GetRootNode()),
						GetBranchBorderSize(), GetBranchBorderColour());
				}
			}

			// flag current, possible leaf node, as processed
			curNode->SetProcessed(true);		

			// add parent node to processing list only if there is sufficent information
			// for it to be processed				
			if(!parentNode->GetProcessed() && !parentNode->IsRoot())
			{
				// check if all child nodes have been processed
				bool bChildrenProcessed = true;
				std::vector<NodeGeoTree*> childNodes = parentNode->GetChildren();	
				foreach(NodeGeoTree* childNode, childNodes)
				{
					if(!childNode->GetProcessed())
					{
						bChildrenProcessed = false;
						break;
					}
				}

				if(bChildrenProcessed)
				{
					// we can safely set this nodes processed flag since it will be processed
					// before anything else that is later added to the list of nodes to be processed
					parentNode->SetProcessed(true);
					nextNodes.push_back(parentNode);
				}
			}
		}		

		curNodes = nextNodes;
	}

	// render root node
	Colour nodeColour = GetNodeColour(m_tree->GetRootNode());
	if(m_layout == SLANTED_CLADOGRAM_3D || m_layout == CLADOGRAM_3D || m_layout == SLANTED_PHYLOGRAM_3D)
		RenderNode3D(m_tree->GetRootNode()->GetGridCoord(), nodeColour, false, m_tree->GetRootNode()->GetSelected());
	else
		RenderNode2D(m_tree->GetRootNode()->GetGridCoord(), nodeColour, false, m_tree->GetRootNode()->GetSelected());

	error::ErrorGL::Check();
}

void GeoTreeView::RenderLocationLines()
{
	error::ErrorGL::Check();

	foreach(Line3D& locationLine, m_locationLines.Lines())
	{
		NodeGeoTree* node = m_tree->GetNode(locationLine.startId);

		Colour colour = m_locationLines.GetColour();
		if(m_treeColourStyle != COLOUR_SINGLE)
			colour = node->GetLocationView()->GetColour();

		if(m_locationLines.GetLineStyle() != VisualLine::HIDDEN)
		{
			VisualLine::RenderLineWithBorder(locationLine, colour, m_locationLines.GetThickness(), m_locationLines.GetLineStyle(),
				m_locationLineBorderColour, m_locationLineBorderSize, TREE_LAYER, node->GetSelected());		
		}
		else if(node->GetSelected())
		{
			VisualLine::RenderLineWithBorder(locationLine, colour, m_locationLines.GetThickness(), VisualLine::SOLID,
				m_locationLineBorderColour, m_locationLineBorderSize, TREE_LAYER, node->GetSelected());	
		}
	}

	error::ErrorGL::Check();
}

void GeoTreeView::RenderCorrelationLines()
{
	if(m_geographyLine.IsVisible())
	{
		error::ErrorGL::Check();

		foreach(Line3D& line, m_correlationLines.Lines())
		{
			Colour colour = m_correlationLines.GetColour();
			if(m_treeColourStyle != COLOUR_SINGLE)
			{
				// determine colour of geographic location site that line is connected to				
				colour = GetLocationColour(line.startId);
			}

			NodeGeoTree* node = m_tree->GetNode(line.startId);

			if(m_correlationLines.GetLineStyle() != VisualLine::HIDDEN)
			{
				VisualLine::RenderLineWithBorder(line, colour, m_correlationLines.GetThickness(), m_correlationLines.GetLineStyle(),
					m_correlationLineBorderColour, m_correlationLineBorderSize, TREE_LAYER, node->GetSelected());	
			}
			else if(node->GetSelected())
			{
				VisualLine::RenderLineWithBorder(line, colour, m_correlationLines.GetThickness(), VisualLine::SOLID,
					m_correlationLineBorderColour, m_correlationLineBorderSize, TREE_LAYER, node->GetSelected());	
			}
		}

		error::ErrorGL::Check();
	}
}

void GeoTreeView::RenderPhylogramDropLines3D()
{
	error::ErrorGL::Check();

	std::vector<NodeGeoTree*> leaves = m_tree->GetLeaves();
	foreach(NodeGeoTree* leaf, leaves)
	{
		Point3D childCoord = leaf->GetGridCoord();
		Point3D groundPoint(childCoord.x, 0.0, childCoord.z);

		RenderDropline3D(childCoord, leaf->GetColour(), m_3dLeafDropLine.GetLineStyle(), GetLineThickness());
	}

	error::ErrorGL::Check();
}

void GeoTreeView::RenderPhylogramDropLines()
{	
	error::ErrorGL::Check();

	if(m_layout == PHYLOGRAM_2D && m_phylogramDropLines.GetVisibility())
	{
		foreach(Line3D& line, m_phylogramDropLines.Lines())
		{
			Colour colour = m_phylogramDropLines.GetColour();
			if(m_treeColourStyle != COLOUR_SINGLE)
			{
				// determine colour of geographic location site that line is connected to				
				colour = GetLocationColour(line.startId);
			}

			NodeGeoTree* node = m_tree->GetNode(line.startId);
			VisualLine::RenderLineWithBorder(line, colour, m_phylogramDropLines.GetThickness(), m_phylogramDropLines.GetLineStyle(),
				Colour(0,0,0), 0.0f, TREE_LAYER, node->GetSelected());		
		}
	}

	error::ErrorGL::Check();
}

void GeoTreeView::RenderGeographyLine()
{
	error::ErrorGL::Check();

	if(m_geographyLine.IsVisible())
	{
		error::ErrorGL::Check();

		VisualLine geographyLine;
		if(m_activeView == GEO_LINE)
		{
			m_modGeographyLine.SetSize(m_geographyLine.GetSize());
			m_modGeographyLine.SetColour(m_geographyLine.GetColour());
			m_modGeographyLine.SetLineStyle(m_geographyLine.GetLineStyle());
			geographyLine = m_modGeographyLine;
		}
		else
			geographyLine = m_geographyLine;

		geographyLine.SetSelected(m_activeView == GEO_LINE);
		geographyLine.Render();

		if(geographyLine.GetLineStyle() != VisualLine::HIDDEN)
		{
			VisualMarker::MARKER_SHAPE startShape = VisualMarker::TRIANGLE;
			VisualMarker::MARKER_SHAPE endShape = VisualMarker::SQUARE;
			if(m_bReverseOrderGeographicPts)
			{
				startShape = VisualMarker::SQUARE;
				endShape = VisualMarker::TRIANGLE;
			}

			if(m_activeView == GEO_LINE)
			{
				float sizeCP = App::Inst().GetLayoutObjProp()->GetControlPointSize() + App::Inst().GetSelectionThickness();
				VisualMarker::RenderAlwaysVisible(geographyLine.GetStartPt(), App::Inst().GetSelectionColour(), sizeCP, 
					startShape, 0.0f, Colour(0,0,0), 0, TREE_LAYER-1);

				VisualMarker::RenderAlwaysVisible(geographyLine.GetEndPt(), App::Inst().GetSelectionColour(), sizeCP, 
					endShape, 0.0f, Colour(0,0,0), 0, TREE_LAYER-1);
			}


			VisualMarker::RenderAlwaysVisible(geographyLine.GetStartPt(), geographyLine.GetColour(), App::Inst().GetLayoutObjProp()->GetControlPointSize(),
				startShape, 0.0f, Colour(0,0,0), 0, TREE_LAYER-1);

			VisualMarker::RenderAlwaysVisible(geographyLine.GetEndPt(), geographyLine.GetColour(), App::Inst().GetLayoutObjProp()->GetControlPointSize(),
				endShape, 0.0f, Colour(0,0,0), 0, TREE_LAYER-1);
		}
	}

	error::ErrorGL::Check();
}

void GeoTreeView::RenderGeographicPts()
{
	if(m_geoPts.GetVisibility())
	{
		error::ErrorGL::Check();

		float depthCP = TREE_LAYER + m_geoPts.GetSize()*2 + 1;

		for(uint i = 0; i < m_geoPts.Points().size(); ++i)
		{
			Colour colour = m_geoPts.GetColour();
			if(m_treeColourStyle != COLOUR_SINGLE)
			{
				// determine colour of location site that dropline is connected to				
				colour = GetLocationColour(m_geoPts.Ids().at(i));
			}

			NodeGeoTree* node = m_tree->GetNode(m_geoPts.Ids().at(i));
			RenderPoint(m_geoPts.Points().at(i), colour, m_geoPts.GetSize(), VisualMarker::CIRCLE_FAST,
				0.0, depthCP, node->GetSelected(), m_geoPtsBorderColour, m_geoPtsBorderSize);
		}

		error::ErrorGL::Check();
	}
}

void GeoTreeView::RenderLabels()
{
	error::ErrorGL::Check();

	if(m_bShowLabels && !App::Inst().GetMouseDragging())
	{
		float sizeCP = App::Inst().GetLayoutObjProp()->GetControlPointSize();
		float depthLabel = TREE_LAYER+sizeCP*2+10;

		Point3D lineDir, treeDir;
		float textAngle;
		int textOffsetX, textOffsetY;
		if(m_layout == CLADOGRAM_2D || m_layout == SLANTED_CLADOGRAM_2D || m_layout == PHYLOGRAM_2D)
		{
			// text should be written in the same direction as the tree is drawn
			// (perpendicular to the layout line)
			CalculateTreeDirection(lineDir, treeDir);
			textAngle = treeDir.Angle(Point3D(1,0,0)) * RAD_TO_DEG;
			if(treeDir.z > 0)	// adjust text angle as it is in the 3rd or 4th quadrant
				textAngle = -textAngle;

			if(treeDir.x >= 0)
				textOffsetX = GetLineThickness()/2.0 + 2;
			else if(treeDir.x < 0)
				textOffsetX = GetLineThickness()/2.0 - 2;

			if(treeDir.z >= 0)
				textOffsetY = GetLineThickness()/2.0 + 2;
			else if(treeDir.z < 0)
				textOffsetY = GetLineThickness()/2.0 - 2;
		}
		else if(m_layout == CLADOGRAM_3D || m_layout == SLANTED_CLADOGRAM_3D || m_layout == SLANTED_PHYLOGRAM_3D)
		{
			// text should be drawn next to each leaf node
			textAngle = 0;
			lineDir = Point3D(1, 0, 0);

			textOffsetX = 3;	// this should really be set to be a little greater than
			// the radius of a location marker
			textOffsetY = 0;
		}

		// render leaf node labels
		std::vector<NodeGeoTree*> leafNodes = m_tree->GetLeaves();
		foreach(NodeGeoTree* leafNode, leafNodes)
		{
			Point3D gridCoord = leafNode->GetGridCoord();
			Point3D curWin = App::Inst().GetMapController()->ProjectToScreen(gridCoord);

			VisualLabel label;
			label.SetText(leafNode->GetName());
			label.SetColour(m_fontColour);
			label.SetSize(m_fontSize);
			label.SetScreenPosition(Point3D(curWin.x+textOffsetX, curWin.y+textOffsetY, depthLabel));
			label.SetRotateZ(true);
			label.SetRotationZ(textAngle);
			label.SetLabelStyle(_T("Dynamic"));
			label.SetVisibility(true);
			label.Render();
		}
	}

	error::ErrorGL::Check();
}

void GeoTreeView::RenderPoint(const Point3D& pt, const Colour& colour, float size, VisualMarker::MARKER_SHAPE shape, 
							  float rotate, float depth, bool bSelected, const Colour& borderColour, float borderSize)
{
	if(bSelected && size > 0.0f)
	{
		VisualMarker::RenderAlwaysVisible(pt, App::Inst().GetSelectionColour(), size+App::Inst().GetSelectionThickness(), 
			shape, rotate, Colour(0,0,0), 0, NODE_DEPTH+1);
	}

	VisualMarker::RenderAlwaysVisible(pt, colour, size, shape, rotate, borderColour, borderSize, NODE_DEPTH);
}

void GeoTreeView::RenderNode2D(const Point3D& pt, const Colour& colour, bool bLeafNode, bool bSelected)
{
	float nodeSize;
	if(bLeafNode)
		nodeSize = GetLeafNodeSize();
	else
		nodeSize = GetInternalNodeSize();

	float rotation = 0;
	if(m_orientation == VERTICAL)
		rotation = -90;

	if(bLeafNode)
		RenderPoint(pt, colour, nodeSize, VisualMarker::CIRCLE_FAST, rotation, NODE_DEPTH, bSelected, m_leafNodeBorderColour, m_leafNodeBorderSize);
	else
		RenderPoint(pt, colour, nodeSize, VisualMarker::CIRCLE_FAST, rotation, NODE_DEPTH, bSelected, m_internalNodeBorderColour, m_internalNodeBorderSize);
}

void GeoTreeView::RenderNode3D(const Point3D& pt, const Colour& colour, bool bLeafNode, bool bSelected)
{
	// render node and dropline
	if(!bLeafNode)
	{
		VisualMarker::Render3D(pt, colour, GetInternalNodeSize(), VisualMarker::CIRCLE_FAST, 0, bSelected);

		if(m_3dInternalDropLine.IsVisible())
			RenderDropline3D(pt, m_3dInternalDropLine.GetColour(), m_3dInternalDropLine.GetLineStyle(), m_3dInternalDropLine.GetSize());
	}
}

void GeoTreeView::RenderDropline3D(const Point3D& pt, const Colour& colour, VisualLine::LINE_STYLE style, float size)
{
	// calculate ground point
	float elev = App::Inst().GetMapController()->GetExaggeratedElevation(pt.x, pt.z);
	Point3D groundPt(pt.x, elev, pt.z);

	// render dropline
	const int REPEAT_FACTOR = 5 * App::Inst().GetResolutionFactor();

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(REPEAT_FACTOR, style);

	glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());

	glLineWidth(size * App::Inst().GetResolutionFactor());		
	glBegin(GL_LINES);
	glVertex3f(pt.x, pt.y, pt.z);
	glVertex3f(groundPt.x, groundPt.y, groundPt.z);
	glEnd();

	glDisable(GL_LINE_STIPPLE);
}

void GeoTreeView::RenderLine3D(const Line3D& line, const Colour& colour, float thickness,
							   VisualLine::LINE_STYLE lineStyle,	bool bSelected)
{
	if(lineStyle == VisualLine::HIDDEN)
		return;

	float approxPixelThickness = thickness / (2000.0f * App::Inst().GetResolutionFactor());

	//if(lineStyle == VisualLine::SOLID)
	{
		VisualLine::RenderCylinder(line.start, line.end, approxPixelThickness, 16, colour, bSelected);
	}
	/*
	else if(lineStyle == VisualLine::SHORT_DASH || lineStyle == VisualLine::LONG_DASH)
	{
	const float SHORT_DASH_LEN = 15 / 2000.0f;
	const float LONG_DASH_LEN = 30 / 2000.0f;
	const float DASH_SPACING = 15 / 2000.0f;

	Point3D startToEnd = line.end - line.start;

	float offset = SHORT_DASH_LEN;
	if(lineStyle == VisualLine::LONG_DASH)
	offset = LONG_DASH_LEN;

	Point3D dashStart = line.start;
	Point3D dashEnd = line.start + startToEnd*offset;

	while(true)
	{
	Point3D endDashToEndPt = line.end - dashEnd;
	if(endDashToEndPt.x*startToEnd.x > 0 || endDashToEndPt.y*startToEnd.y > 0 || endDashToEndPt.z*startToEnd.z > 0)
	{
	// end of dash is before the end point
	VisualLine::RenderCylinder(dashStart, dashEnd, approxPixelThickness, 16, colour, bSelected);

	dashStart += startToEnd*(offset+DASH_SPACING);
	dashEnd += startToEnd*(offset+DASH_SPACING);
	}
	else
	{
	// end of dask is after the end point

	// adjust the end of the dask to be at the end point
	dashEnd = line.end;

	// make sure start of the dask isn't also past the end point
	Point3D startDashToEndPt = line.end - dashStart;
	if(startDashToEndPt.x*startToEnd.x >=0 && startDashToEndPt.y*startToEnd.y >= 0 && endDashToEndPt.z*startToEnd.z > 0)
	{
	// render final dash
	VisualLine::RenderCylinder(dashStart, dashEnd, approxPixelThickness, 16, colour, bSelected);
	}

	break;
	}
	}
	}
	*/
}

Colour GeoTreeView::GetNodeColour(NodeGeoTree* node) const
{
	Colour nodeColour = m_colour;
	if(m_treeColourStyle == COLOUR_DISCRETE || m_treeColourStyle == COLOUR_CONTINUOUS)
	{
		if(node->IsLeaf())
		{
			// determine colour of geographic location associated with leaf node
			if(node->GetActive())
			{
				// if leaf node is currently not being associated with a geographical location
				// (i.e., because a geographic axis has been defined), than leave it at the default colour
				nodeColour = GetLocationColour(node->GetName());
			}
		}
		else if(m_treeColourStyle == COLOUR_DISCRETE) 
		{
			// Set internal nodes to colour of their children. If the children do not have the same
			// colour than leave the branch at the default colour.
			std::vector<NodeGeoTree*> childNodes = node->GetChildren();
			bool bConsistentColour = true;
			Colour childColour = m_colour;
			foreach(NodeGeoTree* child, childNodes)
			{
				// for this child to be considered, it needs to have at least on leaf node that
				// is being associated with a geographical location
				bool bProcess = false;
				std::vector<NodeGeoTree*> leafNodes = TreeTools<NodeGeoTree>::GetLeaves(child);
				foreach(NodeGeoTree* leaf, leafNodes)
				{
					if(leaf->GetActive())
					{
						bProcess = true;
						break;
					}
				}

				if(bProcess)
				{
					if(child->GetColour() == m_colour)
					{
						bConsistentColour = false;
						break;
					}

					if(childColour == m_colour)
					{
						childColour = child->GetColour();
					}
					else if(childColour != child->GetColour())
					{
						bConsistentColour = false;
						break;
					}
				}		
			}

			if(bConsistentColour)
				nodeColour = childColour;
		}
		else if(m_treeColourStyle == COLOUR_CONTINUOUS)
		{
			// Set internal nodes to colour of their children. If the children do not have the same
			// colour than leave the branch at the default colour.
			std::vector<NodeGeoTree*> childNodes = node->GetChildren();
			Colour childColour = m_colour;
			uint leafCount = 0;
			foreach(NodeGeoTree* child, childNodes)
			{
				// for this child to be considered, it needs to have at least on leaf node that
				// is being associated with a geographical location
				bool bProcess = false;
				std::vector<NodeGeoTree*> leafNodes = TreeTools<NodeGeoTree>::GetLeaves(child);
				foreach(NodeGeoTree* leaf, leafNodes)
				{
					if(leaf->GetActive())
					{
						bProcess = true;
						break;
					}
				}

				if(bProcess)
				{
					if(child->GetColour() == m_colour)
						continue;


					if(nodeColour == m_colour)
					{
						nodeColour = child->GetColour();
						leafCount++;
					}
					else 
					{
						nodeColour = Colour( (leafCount*nodeColour.GetRed() + child->GetColour().GetRed()) / (leafCount + 1),
							(leafCount*nodeColour.GetGreen() + child->GetColour().GetGreen()) / (leafCount + 1),
							(leafCount*nodeColour.GetBlue() + child->GetColour().GetBlue()) / (leafCount + 1),
							(leafCount*nodeColour.GetAlpha() + child->GetColour().GetAlpha()) / (leafCount + 1));
						leafCount++;
					}
				}	
			}
		}
	}

	node->SetColour(nodeColour);
	return nodeColour;
}

void GeoTreeView::LayoutTree3D()
{
	// Note: The algorithm used here to layout the tree is rather inefficent as it will visit nodes
	// before there is sufficent information to determine where they should be placed. This can certainly
	// be improved!

	// convert projected coordinates of all leaf nodes to grid coordinates
	std::vector<NodeGeoTree*> leafNodes = m_tree->GetLeaves();
	foreach(NodeGeoTree* leafNode, leafNodes)
	{
		leafNode->SetGridCoord(leafNode->GetLocationView()->GetPosition());
	}

	// set processed flags of leaf nodes as there position is set by the GTM model,
	// but clear processed flag of all internal nodes as these still need to be set
	std::vector<NodeGeoTree*> nodes = m_tree->GetNodes();
	foreach(NodeGeoTree* node, nodes)
		node->SetProcessed(node->IsLeaf());

	// Base height of tree on max distance between any two leaves
	float maxDist = 0;
	float maxElevation = 0;
	foreach(NodeGeoTree* leafNode, leafNodes)
	{
		Point3D iGridCoord = leafNode->GetGridCoord();

		foreach(NodeGeoTree* leafNodeInner, leafNodes)
		{
			Point3D jGridCoord = leafNodeInner->GetGridCoord();

			float x = fabs(iGridCoord.x-jGridCoord.x);
			float z = fabs(iGridCoord.z-jGridCoord.z);

			if(x > maxDist)
				maxDist = x;

			if(z > maxDist)
				maxDist = z;
		}

		if(iGridCoord.y < maxElevation)	// elevations are negative
			maxElevation = iGridCoord.y;
	}


	// each internal node should be set to the mean of its immediate children
	if(m_layout == CLADOGRAM_3D || m_layout == SLANTED_CLADOGRAM_3D)
		LayoutCladogram3D(maxDist, maxElevation);
	else
		LayoutPhylogram3D(maxDist, maxElevation);
}

void GeoTreeView::LayoutCladogram3D(double maxDistBetweenLeaves, double maxElevationOfLeaves)
{
	float depthStep = (m_height*maxDistBetweenLeaves) / m_tree->GetDepth(m_tree->GetRootNode());

	std::vector<NodeGeoTree*> curNodes = m_tree->GetLeaves();

	while(!curNodes.empty())
	{
		std::vector<NodeGeoTree*> parentNodes; 
		foreach(NodeGeoTree* curNode, curNodes)
		{
			NodeGeoTree* parent = curNode->GetParent();

			if(!parent->GetProcessed())
			{
				std::vector<NodeGeoTree*> children = parent->GetChildren();
				Point3D gridCoord;
				if(!CalculateMean(children, gridCoord))
				{
					// at least one child has not had its position determined
					continue;
				}

				gridCoord.y = depthStep * m_tree->GetDepth(parent) + maxElevationOfLeaves;
				parent->SetGridCoord(gridCoord);

				parent->SetProcessed(true);

				if(!parent->IsRoot())
					parentNodes.push_back(parent);
			}
		}

		curNodes = parentNodes;
	}
}

void GeoTreeView::LayoutPhylogram3D(double maxDistBetweenLeaves, double maxElevationOfLeaves)
{
	float treeHeight = m_height*maxDistBetweenLeaves + maxElevationOfLeaves;

	std::vector<NodeGeoTree*> curNodes = m_tree->GetLeaves();

	// layout x,z position of each internal node
	while(!curNodes.empty())
	{
		std::vector<NodeGeoTree*> parentNodes; 
		foreach(NodeGeoTree* curNode, curNodes)
		{
			NodeGeoTree* parent = curNode->GetParent();

			if(!parent->GetProcessed())
			{
				std::vector<NodeGeoTree*> children = parent->GetChildren();
				Point3D gridCoord;
				if(!CalculateMean(children, gridCoord))
				{
					// at least one child has not had its position determined
					continue;
				}

				parent->SetGridCoord(gridCoord);
				parent->SetProcessed(true);

				if(!parent->IsRoot())
					parentNodes.push_back(parent);
			}
		}

		curNodes = parentNodes;
	}

	// layout height of each internal node
	float furthestLeafNode = m_tree->GetDistToFurthestLeafNode(m_tree->GetRootNode());
	float heightPerUnitBranchLength = (m_height*maxDistBetweenLeaves) / furthestLeafNode;
	std::queue<NodeGeoTree*> queue;
	queue.push(m_tree->GetRootNode());
	while(!queue.empty())
	{
		NodeGeoTree* curNode = queue.front();

		// set height of node
		Point3D gridCoord = curNode->GetGridCoord();
		if(curNode->IsRoot())
			gridCoord.y = treeHeight;
		else
			gridCoord.y = curNode->GetParent()->GetGridCoord().y - curNode->GetDistanceToParent() * heightPerUnitBranchLength;
		curNode->SetGridCoord(gridCoord);

		// push children onto queue
		std::vector<NodeGeoTree*> children = curNode->GetChildren();
		foreach(NodeGeoTree* child, children)
			queue.push(child);

		queue.pop();
	}
}

void GeoTreeView::LayoutTrees2D()
{
	// The order of function calls here is critical. Do not
	// change unless you know what you are doing.
	if(m_layoutLine || m_layoutEllipse)
	{
		OptimizeLeafNodeOrdering();

		if(m_layout == CLADOGRAM_2D)
			m_linearLayout->LayoutCladogram2D(m_tree, m_layoutLine, m_height, m_orientation == HORIZONTAL);
		else if(m_layout == SLANTED_CLADOGRAM_2D)
			m_linearLayout->LayoutSlantedCladogram2D(m_tree, m_layoutLine, m_height, m_orientation == HORIZONTAL);
		else if(m_layout == PHYLOGRAM_2D)
			m_linearLayout->LayoutPhylogram2D(m_tree, m_layoutLine, m_height, m_orientation == HORIZONTAL);
	}
}

void GeoTreeView::LayoutConnectionLines()
{
	LayoutPhylogramDropLines();

	if(m_layoutLine)
	{
		if(!m_geographyLine.IsVisible())
		{			
			LayoutGeoLocationToLayoutPt();			
		}
		else
		{
			LayoutGeoLocationToGeoPt();
			LayoutGeoPtToLayoutPt();
		}
	}
	else if(m_layoutEllipse)
	{
		LayoutGeoLocationToLayoutPt();
	}
}

void GeoTreeView::LayoutGeoLocationToLayoutPt()
{
	// directly connect each geographic location to its associated
	// layout point (leaf node)
	std::vector<NodeGeoTree*> leafNodes = m_tree->GetLeaves();
	m_locationLines.Lines().clear();
	m_geoPts.Points().clear();
	for(uint i = 0; i < leafNodes.size(); ++i)
	{
		if(leafNodes.at(i)->GetActive())
		{
			Line3D locationLine;
			locationLine.start = leafNodes.at(i)->GetLocationView()->GetPosition();		

			// connect to phylogram dropline (if the tree is being laid our as a cladogram this will simply indicate the position of the leaf node)
			locationLine.end = m_phylogramDropLines.Lines().at(i).end;

			locationLine.startId = locationLine.endId = leafNodes.at(i)->GetName();

			m_locationLines.Lines().push_back(locationLine);
		}
	}
}

void GeoTreeView::LayoutGeoLocationToGeoPt()
{
	// connect each geographic location to its associated
	// geographic point on the geography line
	std::vector<NodeGeoTree*> leafNodes = m_tree->GetLeaves();
	m_locationLines.Lines().clear();
	m_geoPts.Points().clear();
	m_geoPts.Ids().clear();

	float stepSize; 
	if(m_geoAxisPolyline)
		stepSize = m_layoutLine->GetLength() / (m_geoAxisPolyline->GetNumActiveLeafNodes()-1);
	else
		stepSize = m_layoutLine->GetLength() / (leafNodes.size()-1);

	foreach(NodeGeoTree* leafNode, leafNodes)
	{
		if(!leafNode->GetActive())
			continue;

		Line3D locationLine;
		locationLine.start = leafNode->GetLocationView()->GetPosition();	

		if(m_bSpreadGeoPts)
		{
			// move to correct position along line
			locationLine.end = m_layoutLine->GetStartPt() + leafNode->GetLayoutPos()*stepSize*m_layoutLine->GetDirection();	
		}
		else
		{
			if(m_geoAxisPolyline)
			{
				locationLine.end = m_layoutLine->GetStartPt() 
					+ (leafNode->GetLayoutDistance()/m_geoAxisPolyline->GetLength())*m_layoutLine->GetLength()*m_layoutLine->GetDirection();
			}
			else
			{
				// project geographic location onto layout line
				Point3D closestPoint;
				Geometry::ClosestPointToLine(m_layoutLine->GetLine(), leafNode->GetLocationView()->GetPosition(), closestPoint);

				locationLine.end = closestPoint;
			}
		}

		// translate to geography line
		locationLine.end += GetGeographyLineOffset()*m_layoutLine->GetPerpDirection();

		locationLine.startId = locationLine.endId  = leafNode->GetName();

		m_geoPts.Points().push_back(locationLine.end);
		m_geoPts.Ids().push_back(locationLine.endId);

		m_locationLines.Lines().push_back(locationLine);
	}
}

void GeoTreeView::LayoutGeoPtToLayoutPt()
{
	// connect each geographic point on the geography line to its associated
	// position on the layout line (as indicated by m_phylogramDropLines)
	std::vector<NodeGeoTree*> leafNodes = m_tree->GetLeaves();
	m_correlationLines.Lines().clear();

	float stepSize; 
	if(m_geoAxisPolyline)
		stepSize = m_layoutLine->GetLength() / (m_geoAxisPolyline->GetNumActiveLeafNodes()-1);
	else
		stepSize = m_layoutLine->GetLength() / (leafNodes.size()-1);

	double nearPt = std::numeric_limits<double>::max();
	double farPt = -std::numeric_limits<double>::max();;
	for(uint i = 0; i < leafNodes.size(); ++i)
	{
		if(!leafNodes.at(i)->GetActive())
			continue;

		uint pos = leafNodes.at(i)->GetLayoutPos();

		Line3D correlationLine;

		if(m_bSpreadGeoPts)
		{
			// move to correct position along line
			correlationLine.start = m_layoutLine->GetStartPt() + leafNodes.at(i)->GetLayoutPos()*stepSize*m_layoutLine->GetDirection();	
		}
		else
		{
			if(m_geoAxisPolyline)
			{
				correlationLine.start = m_layoutLine->GetStartPt() 
					+ (leafNodes.at(i)->GetLayoutDistance()/m_geoAxisPolyline->GetLength())*m_layoutLine->GetLength()*m_layoutLine->GetDirection();
			}
			else
			{
				// project geographic location onto layout line
				Point3D closestPoint;
				Geometry::ClosestPointToLine(m_layoutLine->GetLine(), leafNodes.at(i)->GetLocationView()->GetPosition(), closestPoint);	

				// calculate signed distance from start point of line to projected point
				double dist = Geometry::Distance(m_layoutLine->GetLine().start, closestPoint);
				float angle = m_layoutLine->GetDirection().Angle(closestPoint - m_layoutLine->GetLine().start);
				if(angle > PI_BY_2)
					dist = -dist;

				if(dist < nearPt)
					nearPt = dist;

				if(dist > farPt)
					farPt = dist;

				correlationLine.start = closestPoint;
			}
		}

		// translate to geography line
		correlationLine.start += GetGeographyLineOffset()*m_layoutLine->GetPerpDirection();

		// connect to phylogram dropline (if the tree is being laid our as a cladogram this will simply indicate the position of the leaf node)
		correlationLine.end = m_phylogramDropLines.Lines().at(i).end;

		correlationLine.startId = correlationLine.endId = leafNodes.at(i)->GetName();

		m_correlationLines.Lines().push_back(correlationLine);
	}

	if(!m_bSpreadGeoPts && m_geoAxisPolyline == GeoAxisPolylinePtr())
	{
		// have geographic layout line be the same length as the span covered by the projected geographic locations
		m_geographyLine.SetStartPt(m_layoutLine->GetStartControlPoint() + (nearPt-m_layoutLine->GetLayoutOffset())*m_layoutLine->GetDirection() + GetGeographyLineOffset()*m_layoutLine->GetPerpDirection());
		m_geographyLine.SetEndPt(m_layoutLine->GetStartControlPoint() + (farPt+m_layoutLine->GetLayoutOffset())*m_layoutLine->GetDirection() + GetGeographyLineOffset()*m_layoutLine->GetPerpDirection());
	}
	else
	{
		// have geographic layout line be the same length as the layout line
		m_geographyLine.SetStartPt(m_layoutLine->GetStartControlPoint() + GetGeographyLineOffset()*m_layoutLine->GetPerpDirection());
		m_geographyLine.SetEndPt(m_layoutLine->GetEndControlPoint() + GetGeographyLineOffset()*m_layoutLine->GetPerpDirection());
	}
}

void GeoTreeView::LayoutPhylogramDropLines()
{
	// connect each leaf node to the layout line
	std::vector<NodeGeoTree*> leafNodes = m_tree->GetLeaves();
	m_phylogramDropLines.Lines().clear();
	foreach(NodeGeoTree* leafNode, leafNodes)
	{
		Line3D dropLine;
		dropLine.start = leafNode->GetGridCoord();		

		Point3D closestPt;
		if(m_layoutLine)
		{
			Geometry::ClosestPointToLine(m_layoutLine->GetLine(), dropLine.start, closestPt);
			dropLine.end = closestPt;
		}
		else
		{
			dropLine.end = dropLine.start;
		}

		dropLine.startId = dropLine.endId = leafNode->GetName();

		m_phylogramDropLines.Lines().push_back(dropLine);
	}
}

void GeoTreeView::OptimizeLeafNodeOrdering()
{
	wxBusyCursor wait;

	// check if it is reasonable to optimize the leaf node ordering for this tree
	std::vector<NodeGeoTree*> nodes = m_tree->GetNodes();
	m_bOptimizeTopology = true;
	foreach(NodeGeoTree* leaf, nodes)
	{
		uint numChildren = leaf->GetNumberOfChildren();
		if(leaf->GetNumberOfChildren() > MAX_DEGREE_FOR_OPTIMIZING_TREE)
		{
			wxString msg = wxT("Optimal leaf node orderings are not calculated for trees which contain nodes with a degree > 9");
			wxMessageBox(msg, wxT("Optimal leaf node orderings not calculated."), wxOK | wxICON_INFORMATION);
			m_bOptimizeTopology = false;
			break;
		}
	}

	// find ordering of locations along layout line and optimal leaf node ordering
	if(!m_geoAxisPolyline)
	{
		FindProjectedOrdering();

		uint numCrossings;
		GeoTreeOptLeafOrder::OptimizeLeafNodeOrdering(m_tree, m_tree->GetRootNode(), numCrossings, m_bOptimizeTopology);
	}
	else
	{
		// try all possible orderings of geographic polylines and select the one resulting in the fewest crossings
		uint minCrossings = INT_MAX;
		double minLengthForMinCrossings = INT_MAX;
		uint bestOrdering = INT_MAX;
		std::vector<uint> numCrossingReport;
		for(uint i = 0; i < m_geoAxisPolyline->GetNumPolyLineOrderings(); ++i)
		{
			m_geoAxisPolyline->SetPolyLineOrdering(i);
			SetGeographicAxisOrdering();		

			uint numCrossings;
			GeoTreeOptLeafOrder::OptimizeLeafNodeOrdering(m_tree, m_tree->GetRootNode(), numCrossings, m_bOptimizeTopology);
			numCrossingReport.push_back(numCrossings);

			double length = GeoTreeOptLeafOrder::CalculateLengthOfLocationLines(m_tree, m_geographyLine);

			if(numCrossings < minCrossings || (numCrossings == minCrossings && length < minLengthForMinCrossings))
			{
				minCrossings = numCrossings;
				minLengthForMinCrossings = length;
				bestOrdering = i;
			}
		}

		// set to best ordering
		if(bestOrdering != m_geoAxisPolyline->GetNumPolyLineOrderings()-1)
		{
			m_geoAxisPolyline->SetPolyLineOrdering(bestOrdering);
			SetGeographicAxisOrdering();		

			uint numCrossings;
			GeoTreeOptLeafOrder::OptimizeLeafNodeOrdering(m_tree, m_tree->GetRootNode(), numCrossings, m_bOptimizeTopology);
		}

		// Print results
		Log::Inst().Write("Results of nonlinear geographic axis analysis:");
		Log::Inst().Write("Permutation, Crossings, Optimal");
		Log::Inst().Write("-----------------------------");
		uint numOptimalPermutations = 0;
		for(uint i = 0; i < numCrossingReport.size(); ++i)
		{
			//std::wstring resultStr = StringTools::ToStringW(i+1);
			//resultStr += _T(", ") + StringTools::ToStringW(numCrossingReport.at(i));
			///std::string resultStr = StringTools::ToString(i+1);
			///resultStr += ", " + StringTools::ToString(numCrossingReport.at(i));
			wxString resultStr = wxString(StringTools::ToStringW(i+1).c_str());
			resultStr += _T(", ") + wxString(StringTools::ToStringW(numCrossingReport.at(i)).c_str());
			if(numCrossingReport.at(i) == minCrossings)
			{
				//resultStr += _T(", Yes");
				///resultStr += ", Yes";
				resultStr += _T(", Yes");
				numOptimalPermutations++;
			}
			else
				//resultStr += _T(", No");
				///resultStr += ", No";
				resultStr += _T(", No");
			Log::Inst().Write(resultStr);

			m_nonlinearPermutationStrs.Add(resultStr);
		}
		Log::Inst().Write("");
		std::string resultStr = "Number of optimal permutations = " + StringTools::ToString(numOptimalPermutations) + "\n";
		resultStr += "Minimum number of crossings = " + StringTools::ToString(minCrossings);
		Log::Inst().Write(resultStr);
		Log::Inst().Write("---------------\n");
	}
}

void GeoTreeView::SetNonlinearAxisOrdering(uint bestOrdering)
{
	m_geoAxisPolyline->SetPolyLineOrdering(bestOrdering);
	SetGeographicAxisOrdering();		

	uint numCrossings;
	GeoTreeOptLeafOrder::OptimizeLeafNodeOrdering(m_tree, m_tree->GetRootNode(), numCrossings, m_bOptimizeTopology);

	if(m_layout == CLADOGRAM_2D)
		m_linearLayout->LayoutCladogram2D(m_tree, m_layoutLine, m_height, m_orientation == HORIZONTAL);
	else if(m_layout == SLANTED_CLADOGRAM_2D)
		m_linearLayout->LayoutSlantedCladogram2D(m_tree, m_layoutLine, m_height, m_orientation == HORIZONTAL);
	else if(m_layout == PHYLOGRAM_2D)
		m_linearLayout->LayoutPhylogram2D(m_tree, m_layoutLine, m_height, m_orientation == HORIZONTAL);

	LayoutConnectionLines();

	App::Inst().GetViewport()->Refresh(false);
}

void GeoTreeView::SetGeographicAxisOrdering()
{
	std::vector<NodeGeoTree*> leafNodes = m_tree->GetLeaves();
	uint numPtsOnGeographicAxis = 0;
	foreach(NodeGeoTree* leaf, leafNodes)
	{
		uint pos;
		float dist;
		if(m_geoAxisPolyline->GeographicLocationPos(leaf->GetLocationView(), pos, dist))
		{
			leaf->SetActive(true);
			leaf->SetLayoutPos(pos);
			leaf->SetLayoutDistance(dist);
			numPtsOnGeographicAxis++;
		}
		else
		{	
			leaf->SetActive(false);
			leaf->SetLayoutPos(NOT_SET);
		}
	}

	if(m_bReverseOrderGeographicPts)
	{
		foreach(NodeGeoTree* leaf, leafNodes)
		{
			if(leaf->GetActive())
			{
				leaf->SetLayoutPos(numPtsOnGeographicAxis - leaf->GetLayoutPos() - 1);
			}
		}
	}
}

void GeoTreeView::FindProjectedOrdering()
{
	// find position of each leaf node on the layout line
	std::vector<NodeGeoTree*> leafNodes = m_tree->GetLeaves();
	foreach(NodeGeoTree* leafNode, leafNodes)
	{
		// project grid coordinates onto layout line
		Point3D closestPoint;
		Geometry::ClosestPointToLine(m_layoutLine->GetLine(), leafNode->GetLocationView()->GetPosition(), closestPoint);

		leafNode->SetGridCoord(closestPoint);
	}

	// get direction in which to draw tree (RHS of line when walking from start to end point)
	Point3D lineDir, treeDir;
	CalculateTreeDirection(lineDir, treeDir);

	// find order of leaf nodes on the layout line going from the
	// line "origin" towards the end point
	Point3D lineOrigin = m_layoutLine->GetStartPt() - 100*lineDir;
	lineOrigin.y = 0;

	std::vector<NodeSorter> nodeSorter;
	for(uint i = 0; i < leafNodes.size(); ++i)
	{
		// find distance between leaf node and line origin
		Point3D leafPos = leafNodes.at(i)->GetGridCoord();

		float x = lineOrigin.x-leafPos.x;
		float z = lineOrigin.z-leafPos.z;
		float dist = x*x + z*z;

		nodeSorter.push_back(NodeSorter(i, dist));
	}

	std::sort(nodeSorter.begin(), nodeSorter.end(), NodeSortPredicate);

	// specify position of each leaf node along the layout line
	uint pos = 0;
	foreach(NodeSorter& nodeSort, nodeSorter)
	{
		leafNodes.at(nodeSort.index)->SetActive(true);
		leafNodes.at(nodeSort.index)->SetLayoutPos(pos);
		pos++;
	}
}


void GeoTreeView::SetLayout(LAYOUT layout)
{
	if(layout != m_layout)
	{
		if(layout == PHYLOGRAM_2D)
		{
			// check that tree has distance information. If not, perform a cladogram layout.
			std::vector<NodeGeoTree*> leafNodes = m_tree->GetLeaves();
			if(leafNodes.size() > 0 && leafNodes.at(0)->GetDistanceToParent() == Node::NO_DISTANCE)
				layout = CLADOGRAM_2D;
		}

		m_layout = layout;

		ForceTreeLayout();
	}
}

void GeoTreeView::ForceTreeLayout()
{
	if(m_layout == CLADOGRAM_3D || m_layout == SLANTED_CLADOGRAM_3D || m_layout == SLANTED_PHYLOGRAM_3D)
	{
		LayoutTree3D();
	}
	else if(m_layout == CLADOGRAM_2D || m_layout == SLANTED_CLADOGRAM_2D || m_layout == PHYLOGRAM_2D)
	{
		LayoutTrees2D();
		LayoutConnectionLines();
	}
	else if(m_layout == ELLIPSE_QUADRANT)
	{
		m_ellipticalLayout->LayoutEllipseQuadrant(m_tree, m_layoutEllipse, m_height);
		LayoutConnectionLines();
	}
}

bool GeoTreeView::CalculateMean(const std::vector<NodeGeoTree*>&  nodes, Point3D& mean)
{
	mean.x = 0;
	mean.y = 0;
	mean.z = 0;

	foreach(NodeGeoTree* node, nodes)
	{
		if(!node->GetProcessed())
			return false;

		Point3D gridCoord = node->GetGridCoord();

		mean.x += gridCoord.x;
		mean.y += gridCoord.y;
		mean.z += gridCoord.z;
	}

	mean.x = mean.x / nodes.size();
	mean.y = mean.y / nodes.size();
	mean.z = mean.z / nodes.size();

	return true;
}

const void GeoTreeView::CalculateTreeDirection(Point3D& layoutLineDir, Point3D& treeDir) const
{
	layoutLineDir = m_layoutLine->GetDirection();
	layoutLineDir.y = 0;
	treeDir = layoutLineDir.Cross(Point3D(0,1,0));
}

void GeoTreeView::ShowPropertiesDlg()
{
	if(m_activeView == LAYOUT_LINE)
	{
		m_layoutLine->ShowPropertiesDlg();
	}
	else if(m_activeView == LAYOUT_ELLIPSE)
	{
		m_layoutEllipse->ShowPropertiesDlg();
	}
	else if(m_activeView == GEOGRAPHIC_AXIS)
	{
		m_geoAxisPolyline->ShowPropertiesDlg();
	}
	else if(m_activeView == TREE || m_activeView == GEO_POINT || m_activeView == GEO_LINE)
	{
		TreeLayerPtr treeLayer;
		for (uint i = 0; i < App::Inst().GetLayerTreeController()->GetNumTreeLayers(); i++)
		{
			treeLayer = App::Inst().GetLayerTreeController()->GetTreeLayer(i);
			if (treeLayer->GetId() == m_treeLayerId)
				break;
		}

		if ( treeLayer->HasPropertiesDialogOpen() )
		{
			treeLayer->GetPropertiesDialog()->SetFocus();
			treeLayer->GetPropertiesDialog()->Raise();
		}
		else
		{
			TreePropertiesDlg* dlg = new TreePropertiesDlg(App::Inst().GetMainWindow(), treeLayer);
			dlg->SetPage(TreePropertiesDlg::SYMBOLOGY);
			dlg->Show();
			treeLayer->SetPropertiesDialogOpenStatus( true );
		}
	}
}

void GeoTreeView::SetSelectionVisiblity(bool visible)
{
	if(m_activeView == LAYOUT_LINE)
	{
		m_layoutLine->SetVisibility(visible);
	}
	else if(m_activeView == LAYOUT_ELLIPSE)
	{
		m_layoutEllipse->SetVisibility(visible);
	}
	else if(m_activeView == GEOGRAPHIC_AXIS)
	{
		m_geoAxisPolyline->SetVisibility(visible);
	}
	else if(m_activeView == TREE)
	{
		SetVisibility(visible);
	}
	else if(m_activeView == GEO_POINT)
	{
		m_geoPts.SetVisibility(visible);
	}
}

bool GeoTreeView::MouseLeftDown(const Point2D& mousePt) 
{ 
	// Note: the order of checks is important. It defines which visual object
	// will be selected if multiple visual objects happen to overlap.
	m_selectedNode = NULL;
	m_sigNodeUpdate(NodeInfo(_T(""), NOT_SET));

	if(m_layoutLine && m_layoutLine->MouseLeftDown(mousePt))
	{
		m_activeView = LAYOUT_LINE;
		return true;
	}
	else if(m_layoutEllipse && m_layoutEllipse->MouseLeftDown(mousePt))
	{
		m_activeView = LAYOUT_ELLIPSE;
		return true;
	}
	else if(m_geoAxisPolyline && m_geoAxisPolyline->MouseLeftDown(mousePt))
	{
		m_activeView = GEOGRAPHIC_AXIS;
		return true;
	}
	else if(IsTreeClicked(mousePt, true))
	{
		m_activeView = TREE;

		if(m_layout == CLADOGRAM_2D || m_layout == SLANTED_CLADOGRAM_2D  || m_layout == PHYLOGRAM_2D)
		{
			m_modLayoutLine.start = m_layoutLine->GetStartControlPoint();
			m_modLayoutLine.end = m_layoutLine->GetEndControlPoint();
		}

		return true;
	}
	else if(IsGeoPointClicked(mousePt))
	{
		m_activeView = GEO_POINT;
		return true;
	}
	else if(IsGeoLineControlPointClicked(mousePt))
	{
		m_modGeographyLine = m_geographyLine;

		m_activeView = GEO_LINE;
		return true;
	}

	m_activeView = NO_ACTIVE_VIEW;
	return false;
}


void GeoTreeView::MouseLeftUp(const Point2D& mousePt)
{
	if(m_activeView == LAYOUT_LINE)
	{
		m_layoutLine->MouseLeftUp(mousePt);

		// need to layout the tree if the layout line have moved
		if(m_bActiveViewMoved)
		{		
			m_bActiveViewMoved = false;
			ForceTreeLayout();
		}
	}
	else if(m_activeView == LAYOUT_ELLIPSE)
	{
		m_layoutEllipse->MouseLeftUp(mousePt);

		// need to layout the tree if the layout line have moved
		if(m_bActiveViewMoved)
		{		
			m_bActiveViewMoved = false;
			if(m_layout == ELLIPSE_QUADRANT)
			{
				m_ellipticalLayout->LayoutEllipseQuadrant(m_tree, m_layoutEllipse, m_height);
				LayoutConnectionLines();
			}
		}
	}
	else if(m_activeView == GEOGRAPHIC_AXIS)
	{
		m_geoAxisPolyline->MouseLeftUp(mousePt);

		// if the polyline has changed, the order of leaf nodes may be different so we need
		// to layout the tree
		if(m_bActiveViewMoved)
		{		
			m_bActiveViewMoved = false;
			if(m_layout == CLADOGRAM_2D || m_layout == SLANTED_CLADOGRAM_2D || m_layout == PHYLOGRAM_2D)
			{		
				LayoutTrees2D();	
			}
			else if(m_layout == ELLIPSE_QUADRANT)
			{
				m_ellipticalLayout->LayoutEllipseQuadrant(m_tree, m_layoutEllipse, m_height);
			}

			LayoutConnectionLines();
		}
	}
	else if(m_activeView == TREE && (m_layout == CLADOGRAM_2D || m_layout == SLANTED_CLADOGRAM_2D || m_layout == PHYLOGRAM_2D))
	{	
		if(m_bActiveViewMoved)
		{		
			m_bActiveViewMoved = false;

			m_layoutLine->SetStartControlPoint(m_modLayoutLine.start);
			m_layoutLine->SetEndControlPoint(m_modLayoutLine.end);
			ForceTreeLayout();
		}
	}
	else if(m_activeView == GEO_POINT)
	{
		m_bActiveViewMoved = false;
	}
	else if(m_activeView == GEO_LINE)
	{
		if(m_bActiveViewMoved)
		{		
			m_bActiveViewMoved = false;

			// momentarily set layout line to position of modified geography line
			m_layoutLine->SetStartControlPoint(m_modGeographyLine.GetStartPt());
			m_layoutLine->SetEndControlPoint(m_modGeographyLine.GetEndPt());

			// now calculate offset of layout line from geography line and reposition the layout line
			float offset = GetGeographyLineOffset();
			m_layoutLine->SetStartControlPoint(m_modGeographyLine.GetStartPt() - offset*Geometry::NormalToLine(m_modGeographyLine.GetLine()));
			m_layoutLine->SetEndControlPoint(m_modGeographyLine.GetEndPt() - offset*Geometry::NormalToLine(m_modGeographyLine.GetLine()));

			ForceTreeLayout();
		}
	}
}

void GeoTreeView::MouseLeftDblClick(const Point2D& mousePt) 
{ 
	if(m_activeView == LAYOUT_LINE)
		m_layoutLine->MouseLeftDblClick(mousePt);
	else if(m_activeView == LAYOUT_ELLIPSE)
		m_layoutEllipse->MouseLeftDblClick(mousePt);
	else if(m_activeView == GEOGRAPHIC_AXIS)
		m_geoAxisPolyline->MouseLeftDblClick(mousePt);
	else if(m_activeView == TREE || m_activeView == GEO_POINT || m_activeView == GEO_LINE)
		ShowPropertiesDlg();
}

bool GeoTreeView::MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu) 
{ 
	// Note: the order of checks is important. It defines which visual object
	// will be selected if multiple visual objects happen to overlap.
	m_selectedNode = NULL;
	m_sigNodeUpdate(NodeInfo(_T(""), NOT_SET));

	if(m_layoutLine && m_layoutLine->MouseRightDown(mousePt, popupMenu))
	{
		m_activeView = LAYOUT_LINE;
		return true;
	}
	else if(m_layoutEllipse && m_layoutEllipse->MouseRightDown(mousePt, popupMenu))
	{
		m_activeView = LAYOUT_ELLIPSE;
		return true;
	}
	else if(m_geoAxisPolyline && m_geoAxisPolyline->MouseRightDown(mousePt, popupMenu))
	{
		m_activeView = GEOGRAPHIC_AXIS;
		return true;
	}
	else if(IsTreeClicked(mousePt, false))
	{
		if(popupMenu.GetMenuItems().size() == 0)
		{
			popupMenu.Append(ID_POPUP_MNU_PROPERTIES, wxT("Properties"));

			if ( m_layout == CLADOGRAM_2D || m_layout == SLANTED_CLADOGRAM_2D || m_layout == PHYLOGRAM_2D )
			{
				popupMenu.AppendSeparator();
				popupMenu.Append(ID_POPUP_MNU_SUBTREE_SIGNIFICANCE_TEST, wxT("Perform significance test on subtree"));
				popupMenu.AppendSeparator();
				popupMenu.Append(ID_POPUP_MNU_LINEAR_AXES_ANALYSIS, wxT("Perform linear axes analysis on subtree"));

				if(m_geoAxisPolyline)
				{
					popupMenu.AppendSeparator();
					popupMenu.Append(ID_POPUP_MNU_SHOW_GEOGRAPHIC_AXIS_TABLE, wxT("Show nonlinear axis table"));
				}
			}
		}

		m_activeView = TREE;
		return true;
	}
	else if(IsGeoPointClicked(mousePt))
	{
		popupMenu.Append(ID_POPUP_MNU_PROPERTIES, wxT("Properties"));
		popupMenu.Append(ID_POPUP_MNU_EXTEND_POLYLINE, wxT("Extend geographic axis"));

		m_activeView = GEO_POINT;
		return true;
	}
	else if(IsGeoLineControlPointClicked(mousePt))
	{
		popupMenu.Append(ID_POPUP_MNU_PROPERTIES, wxT("Properties"));

		m_activeView = GEO_LINE;
		return true;
	}

	m_activeView = NO_ACTIVE_VIEW;
	return false;
}

void GeoTreeView::MouseDragging(const Point2D& mousePt) 
{ 
	if(m_activeView == LAYOUT_LINE)
	{
		m_bActiveViewMoved = true;
		m_layoutLine->MouseDragging(mousePt);
	}
	else if(m_activeView == LAYOUT_ELLIPSE)
	{
		m_bActiveViewMoved = true;
		m_layoutEllipse->MouseDragging(mousePt);
	}
	else if(m_activeView == GEOGRAPHIC_AXIS)
	{
		m_bActiveViewMoved = true;
		m_geoAxisPolyline->MouseDragging(mousePt);
	}
	else if(m_activeView == TREE)
	{
		if(m_layout == CLADOGRAM_2D || m_layout == SLANTED_CLADOGRAM_2D || m_layout == PHYLOGRAM_2D)
		{
			// translate tree
			m_bActiveViewMoved = true;
			TranslateTree();
		}
	}
	else if(m_activeView == GEO_POINT)
	{
		// what should happen when the user tries to move a geographic point
	}
	else if(m_activeView == GEO_LINE)
	{
		m_bActiveViewMoved = true;
		TranslateGeoLine();
	}

}

void GeoTreeView::TranslateGeoLine()
{
	Point3D mousePos = App::Inst().GetMouseTerrainPlanePos();

	Point3D startPos = m_geographyLine.GetLine().start;
	if(!m_bSelectedFirstGeoLineCP)
		startPos = m_geographyLine.GetLine().end;

	float dx = mousePos.x - startPos.x;
	float dy = mousePos.y - startPos.y;
	float dz = mousePos.z - startPos.z;
	Point3D offsetPt(dx, dy, dz);

	if(m_bSelectedFirstGeoLineCP)
		m_modGeographyLine.SetStartPt(m_geographyLine.GetStartPt().Offset(offsetPt));
	else
		m_modGeographyLine.SetEndPt(m_geographyLine.GetEndPt().Offset(offsetPt));
}

void GeoTreeView::TranslateTree()
{	
	Point3D mousePos = App::Inst().GetMouseTerrainPlanePos();
	Point3D startPos = m_selectedNode->GetGridCoord();

	float dx = mousePos.x - startPos.x;
	float dy = mousePos.y - startPos.y;
	float dz = mousePos.z - startPos.z;
	Point3D offsetPt(dx, dy, dz);

	std::vector<NodeGeoTree*> nodes = m_tree->GetNodes();
	for(uint i = 0; i < nodes.size(); ++i)
	{
		nodes.at(i)->SetGridCoord(nodes.at(i)->GetGridCoord().Offset(offsetPt));
	}

	m_modLayoutLine.start = m_modLayoutLine.start.Offset(offsetPt);
	m_modLayoutLine.end = m_modLayoutLine.end.Offset(offsetPt);
}

bool GeoTreeView::MouseMove(const Point2D& mousePt)
{
	if(m_activeView == LAYOUT_LINE)
	{
		return m_layoutLine->MouseMove(mousePt);
	}
	else if(m_activeView == LAYOUT_ELLIPSE)
	{
		return m_layoutEllipse->MouseMove(mousePt);
	}
	else if(m_activeView == GEOGRAPHIC_AXIS)
	{
		return m_geoAxisPolyline->MouseMove(mousePt);
	}
	else if(m_activeView == TREE)
	{
		// indicate that tree is being moved
		return true;
	}
	else if(m_activeView == GEO_POINT)
	{
		// what should happen when the user tries to drag a geographic point
		return false;
	}
	else if(m_activeView == GEO_LINE)
	{
		// indicate geographic line is being moved
		return true;
	}

	return false;
}

bool GeoTreeView::IsTreeClicked(const Point2D& mousePt, bool bLeftButton)
{    
	if(!IsVisible())
		return false;

	std::vector<NodeGeoTree*> nodes = m_tree->GetNodes();

	if(bLeftButton)
		m_tree->GetRootNode()->SetSelectedSubtree(false);

	for(uint i = 0; i < nodes.size(); ++i)
	{
		Point3D pt = App::Inst().GetMapController()->ProjectToScreen(nodes.at(i)->GetGridCoord());

		float nodeSize;
		if(nodes.at(i)->IsLeaf())
			nodeSize = GetLeafNodeSize();
		else
			nodeSize = std::max(GetInternalNodeSize(), 0.5f*GetLineThickness());

		if(nodeSize < 3.0)	// ensure one can still click even on small (possibly zero) sized nodes
			nodeSize = 3.0;

		if(abs(pt.x - mousePt.x) < nodeSize && abs(pt.y - mousePt.y) < nodeSize)
		{
			m_selectedNode = nodes.at(i);
			m_sigNodeUpdate(NodeInfo(nodes.at(i)->GetName(), nodes.at(i)->GetNumCrossings()));

			if(bLeftButton)
			{
				// mark all nodes in this subtree as selected
				nodes.at(i)->SetSelectedSubtree(true);
			}

			return true;
		}
	}

	// select any leaf nodes that are associated with a selected location
	std::vector< NodeGeoTree* > leafNodes = m_tree->GetLeaves();
	foreach(NodeGeoTree* leafNode, leafNodes)
	{
		leafNode->SetSelected(leafNode->GetLocationView()->IsSelected() || leafNode->GetSelected());
	}

	return false;
}

bool GeoTreeView::IsGeoPointClicked(const Point2D& mousePt)
{
	if(!m_geoPts.GetVisibility() || m_layout == ELLIPSE_QUADRANT)
		return false;

	for(uint i = 0; i < m_geoPts.Points().size(); ++i)
	{
		Point3D pt = App::Inst().GetMapController()->ProjectToScreen(m_geoPts.Points().at(i));
		if(abs(pt.x - mousePt.x) < m_geoPts.GetSize() && abs(pt.y - mousePt.y) < m_geoPts.GetSize())
		{
			NodeGeoTree* node = m_tree->GetNode(m_geoPts.Ids().at(i));
			node->SetSelected(true);

			return true;
		}
	}

	return false;
}

bool GeoTreeView::IsGeoLineControlPointClicked(const Point2D& mousePt)
{
	if(!m_geographyLine.IsVisible() || m_layout == ELLIPSE_QUADRANT)
		return false;

	Point3D pt = App::Inst().GetMapController()->ProjectToScreen(m_geographyLine.GetLine().start);
	if(abs(pt.x - mousePt.x) < m_geoPts.GetSize() && abs(pt.y - mousePt.y) < m_geoPts.GetSize())
	{
		m_bSelectedFirstGeoLineCP = true;
		return true;
	}

	pt = App::Inst().GetMapController()->ProjectToScreen(m_geographyLine.GetLine().end);
	if(abs(pt.x - mousePt.x) < m_geoPts.GetSize() && abs(pt.y - mousePt.y) < m_geoPts.GetSize())
	{
		m_bSelectedFirstGeoLineCP = false;
		return true;
	}

	return false;
}

void GeoTreeView::KeyboardEvent(wxKeyEvent& event)
{
	if(m_activeView == LAYOUT_LINE)
	{
		m_layoutLine->KeyboardEvent(event);

		if(event.GetKeyCode() == WXK_ESCAPE)
		{
			m_bActiveViewMoved = false;
			m_activeView = NO_ACTIVE_VIEW;
		}
		else if(event.GetKeyCode() == WXK_DELETE)
		{
			m_activeView = NO_ACTIVE_VIEW;
			m_layoutLine.reset();
			m_layout = SLANTED_CLADOGRAM_3D;
			ForceTreeLayout();
		}
	}
	else if(m_activeView == LAYOUT_ELLIPSE)
	{
		m_layoutEllipse->KeyboardEvent(event);

		if(event.GetKeyCode() == WXK_ESCAPE)
		{
			m_bActiveViewMoved = false;
			m_activeView = NO_ACTIVE_VIEW;
		}
		else if(event.GetKeyCode() == WXK_DELETE)
		{
			m_activeView = NO_ACTIVE_VIEW;
			m_layoutEllipse.reset();
			m_layout = SLANTED_CLADOGRAM_3D;
			ForceTreeLayout();
		}
	}
	else if(m_activeView == GEOGRAPHIC_AXIS)
	{
		m_geoAxisPolyline->KeyboardEvent(event);

		if(event.GetKeyCode() == WXK_ESCAPE)
		{
			m_bActiveViewMoved = false;
			m_activeView = NO_ACTIVE_VIEW;
		}
		else if(event.GetKeyCode() == WXK_DELETE)
		{
			m_activeView = NO_ACTIVE_VIEW;
			m_geoAxisPolyline.reset();
			SetGeographicAxisVisibility(false);
			ForceTreeLayout();
		}
		else if(event.GetKeyCode() == WXK_RETURN)
		{
			m_geoAxisPolyline->PolylineFinished();
			ForceTreeLayout();
		}
	}
	else if(m_activeView == TREE)
	{
		if(event.GetKeyCode() == WXK_ESCAPE)
		{
			m_bActiveViewMoved = false;
			m_activeView = NO_ACTIVE_VIEW;
			m_selectedNode = NULL;
			m_sigNodeUpdate(NodeInfo(_T(""), NOT_SET));
			m_tree->GetRootNode()->SetSelectedSubtree(false);
			ForceTreeLayout();
		}
	}
	else if(m_activeView == GEO_LINE)
	{
		if(event.GetKeyCode() == WXK_ESCAPE || event.GetKeyCode() == WXK_DELETE)
		{
			m_bActiveViewMoved = false;
			m_activeView = NO_ACTIVE_VIEW;

			if(event.GetKeyCode() == WXK_DELETE)
				SetGeographicAxisVisibility(false);

			ForceTreeLayout();	
		}
	}
}

bool GeoTreeView::GetLayoutLineVisibility()  const
{ 
	if(m_layoutLine)
		return m_layoutLine->IsVisible(); 

	return false;
}

void GeoTreeView::SetLayoutLineVisibility(bool state) 
{ 
	if(m_layoutLine)
		m_layoutLine->SetVisibility(state); 
}

bool GeoTreeView::GetLayoutEllipseVisibility() const
{ 
	if(m_layoutEllipse)
		return m_layoutEllipse->IsVisible(); 

	return false;
}

void GeoTreeView::SetLayoutEllipseVisibility(bool state) 
{ 
	if(m_layoutEllipse)
		m_layoutEllipse->SetVisibility(state); 
}

bool GeoTreeView::GetLayoutPrimativeVisibility() const
{
	if(m_layoutLine)
		return m_layoutLine->IsVisible(); 

	if(m_layoutEllipse)
		return m_layoutEllipse->IsVisible(); 

	return false;
}

void GeoTreeView::SetLayoutPrimativeVisibility(bool state)
{
	if(m_layoutLine)
		return m_layoutLine->SetVisibility(state); 

	if(m_layoutEllipse)
		return m_layoutEllipse->SetVisibility(state); 
}

void GeoTreeView::SetLayoutLine(LayoutLinePtr layoutLine) 
{ 
	m_layoutLine = layoutLine; 

	// a chart set can only have either a layout line or ellipse associated with it
	if(m_layoutEllipse)
		m_layoutEllipse.reset();
}

void GeoTreeView::SetLayoutEllipse(LayoutEllipsePtr ellipse) 
{ 
	m_layoutEllipse = ellipse; 

	// a chart set can only have either a layout line or ellipse associated with it
	if(m_layoutLine)
		m_layoutLine.reset();
}

bool GeoTreeView::IsLayoutLine() const
{ 
	return m_layoutLine; 
}

bool GeoTreeView::IsLayoutEllipse() const
{ 
	return m_layoutEllipse; 
}

void GeoTreeView::SetGeographicAxis(GeoAxisPolylinePtr geoAxisPolyline)
{ 
	m_geoAxisPolyline = geoAxisPolyline; 
	m_geoAxisPolyline->SetTree(m_tree);
	ForceTreeLayout();
}

bool GeoTreeView::IsGeographicAxis() const
{ 
	return m_geoAxisPolyline; 
}

bool GeoTreeView::GetGeographicAxisVisibility() const 
{ 
	if(m_geoAxisPolyline)
		return m_geoAxisPolyline->IsVisible(); 

	return m_geographyLine.IsVisible();
}

void GeoTreeView::SetGeographicAxisVisibility(bool state) 
{ 
	if(m_geoAxisPolyline)
		m_geoAxisPolyline->SetVisibility(state); 
	m_geographyLine.SetVisibility(state);
}

double GeoTreeView::SignificanceTest(uint iterations, std::map<uint, uint>& pdf, uint& originalNumberCrossings, NodeGeoTree* node, bool& bSuccess) const
{
	if(m_bOptimizeTopology)
	{
		return GeoTreeOptLeafOrder::SignificanceTest(m_tree, node, iterations, pdf, originalNumberCrossings, bSuccess);
	}
	else
	{
		wxMessageBox(_T("An optimal leaf node orderings is not being calculated for this tree.\nAs a result, a significance test cannot be performed."),
			_T("Significance test failed."), wxOK | wxICON_INFORMATION);
		return -1.0;
	}
}

std::vector<LinearResults> GeoTreeView::PerformLinearAxesAnalysis(NodeGeoTree* node)
{
	m_linearAxesResults.clear();

	if(m_bOptimizeTopology)
	{
		m_linearAxesResults = GeoTreeOptLeafOrder::PerformLinearAxesAnalysis(m_tree, node);
	}
	else
	{
		wxMessageBox(_T("Optimal leaf node orderings cannot be calculated for this tree.\nAs a result, the linear axes analysis cannot be performed."),
			_T("Linear axes analysis failed."), wxOK | wxICON_INFORMATION);
	}

	return m_linearAxesResults;
}

std::vector<LinearResults> GeoTreeView::PerformLinearAxesAnalysisOnRootNode()
{
	m_linearAxesResults.clear();

	// check if it is reasonable to optimize the leaf node ordering for this tree
	std::vector<NodeGeoTree*> nodes = m_tree->GetNodes();
	m_bOptimizeTopology = true;
	foreach(NodeGeoTree* leaf, nodes)
	{
		uint numChildren = leaf->GetNumberOfChildren();
		if(leaf->GetNumberOfChildren() > MAX_DEGREE_FOR_OPTIMIZING_TREE)
		{
			m_bOptimizeTopology = false;
			break;
		}
	}


	if(m_bOptimizeTopology)
	{
		m_linearAxesResults = GeoTreeOptLeafOrder::PerformLinearAxesAnalysis(m_tree, m_tree->GetRootNode());
	}
	else
	{
		wxString msg = wxT("The linear axes analysis cannot be performed on trees which contain nodes with a degree > 9");
		wxMessageBox(msg, wxT("Linear axes analysis cannot be performed."), wxOK | wxICON_INFORMATION);
	}

	return m_linearAxesResults;
}

void GeoTreeView::ExtendGeographicAxis() 
{ 
	m_geoAxisPolyline->ExtendGeographicAxis(); 
}

void GeoTreeView::OnTreeFlip()
{
	Point3D endPt = m_layoutLine->GetEndPt();
	m_layoutLine->SetEndPt(m_layoutLine->GetStartPt());
	m_layoutLine->SetStartPt(endPt);

	ForceTreeLayout();
}

float GeoTreeView::GetGeographyLineOffset() const 
{ 
	if(m_layoutLine)
		return m_geographyLineOffsetPercentage*m_layoutLine->GetLength(); 
	else
		return 0.0;
}

float GeoTreeView::GetLeafNodeSize() const
{ 
	return m_leafNodeSize * App::Inst().GetResolutionFactor(); 
}

float GeoTreeView::GetInternalNodeSize() const
{ 
	float size = m_internalNodeSize;
	if(m_layout == SLANTED_CLADOGRAM_3D || m_layout == CLADOGRAM_3D || m_layout == SLANTED_PHYLOGRAM_3D)
		size = std::max(m_internalNodeSize, 0.5f*m_lineThickness);

	return size * App::Inst().GetResolutionFactor(); 
}

float GeoTreeView::GetLineThickness() const
{ 
	return m_lineThickness * App::Inst().GetResolutionFactor(); 
}

void GeoTreeView::RenderCornerLines(const Point3D& pt1, const Point3D& pt2, const Point3D& pt3, 
									float angle, float thickness, float depth, const Colour& colour, 
									float borderThickness, const Colour& borderColour, bool bReverseOrientation, bool bSelected)
{
	error::ErrorGL::Check();
	glDepthRange(0.0, TERRAIN_START_DEPTH);

	App::Inst().GetViewport()->GetCamera()->SetOrthoCamera();

	glPushMatrix();	
	Point3D screenPt1 = App::Inst().GetMapController()->ProjectToScreen(pt1);
	Point3D screenPt2 = App::Inst().GetMapController()->ProjectToScreen(pt2);
	Point3D screenPt3 = App::Inst().GetMapController()->ProjectToScreen(pt3);

	glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());

	RenderCorner(screenPt1, screenPt2, screenPt3, angle, thickness + borderThickness, depth);

	if(borderThickness > 0.0f)
		RenderCornerOutline(screenPt1, screenPt2, screenPt3, thickness, borderColour, borderThickness, bReverseOrientation, depth-1);

	if(bSelected)
	{
		Colour selectionColour = App::Inst().GetSelectionColour();
		RenderCornerOutline(screenPt1, screenPt2, screenPt3, thickness+borderThickness, selectionColour, App::Inst().GetSelectionThickness(), bReverseOrientation, depth-2);
	}
	glPopMatrix();

	App::Inst().GetViewport()->GetCamera()->UnsetOrthoCamera();

	error::ErrorGL::Check();
}

void GeoTreeView::RenderCorner(const Point3D& screenPt1, const Point3D& screenPt2, const Point3D& screenPt3, 
							   float angle, float thickness, float depth)
{
	glPushMatrix();	
		
	float halfThickness = 0.5*thickness;

	Point3D childToCorner = screenPt2 - screenPt1;
	childToCorner.Normalize();
	Point3D perpChildToCorner(-childToCorner.y, childToCorner.x, childToCorner.z);

	Point3D cornerToParent = screenPt3 - screenPt2;
	cornerToParent.Normalize();

	glBegin(GL_QUADS);
		glVertex3f(screenPt1.x + halfThickness*perpChildToCorner.x, screenPt1.y + halfThickness*perpChildToCorner.y, -depth);
		glVertex3f(screenPt2.x + halfThickness*perpChildToCorner.x + halfThickness*childToCorner.x, screenPt2.y + halfThickness*perpChildToCorner.y + halfThickness*childToCorner.y, -depth);
		glVertex3f(screenPt2.x - halfThickness*perpChildToCorner.x + halfThickness*childToCorner.x, screenPt2.y - halfThickness*perpChildToCorner.y + halfThickness*childToCorner.y, -depth);
		glVertex3f(screenPt1.x - halfThickness*perpChildToCorner.x, screenPt1.y - halfThickness*perpChildToCorner.y, -depth);			

		if(screenPt2 != screenPt3)
		{
			Point3D perpChildToParent(-cornerToParent.y, cornerToParent.x, cornerToParent.z);

			glVertex3f(screenPt2.x + halfThickness*perpChildToParent.x - halfThickness*cornerToParent.x, screenPt2.y + halfThickness*perpChildToParent.y - halfThickness*cornerToParent.y, -depth);
			glVertex3f(screenPt3.x + halfThickness*perpChildToParent.x, screenPt3.y + halfThickness*perpChildToParent.y, -depth);
			glVertex3f(screenPt3.x - halfThickness*perpChildToParent.x, screenPt3.y - halfThickness*perpChildToParent.y, -depth);
			glVertex3f(screenPt2.x - halfThickness*perpChildToParent.x - halfThickness*cornerToParent.x, screenPt2.y - halfThickness*perpChildToParent.y - halfThickness*cornerToParent.y, -depth);	
		}
	glEnd();

	glPopMatrix();
}

void GeoTreeView::RenderCornerOutline(const Point3D& screenPt1, const Point3D& screenPt2, const Point3D& screenPt3, 
									  float lineThickness, const Colour& borderColour, float borderThickness, bool bReverseOrientation, float depth)
{
	glPushMatrix();	
	
	float halfThickness = 0.5*lineThickness;

	Point3D childToCorner = screenPt2 - screenPt1;
	childToCorner.Normalize();
	
	Point3D cornerToParent = screenPt3 - screenPt2;
	cornerToParent.Normalize();
	if(cornerToParent.x == 0 && cornerToParent.y == 0)
		cornerToParent = Point3D(-childToCorner.y, childToCorner.x, childToCorner.z);

	Point3D pt1(screenPt1.x + halfThickness*cornerToParent.x + halfThickness*childToCorner.x, screenPt1.y + halfThickness*cornerToParent.y + halfThickness*childToCorner.y, 0);
	Point3D pt2(screenPt2.x + halfThickness*cornerToParent.x - (halfThickness-0.5*borderThickness)*childToCorner.x, screenPt2.y + halfThickness*cornerToParent.y - (halfThickness-0.5*borderThickness)*childToCorner.y, 0);
	VisualLine::RenderAntialiasedLine(pt1, pt2, borderColour, borderThickness, VisualLine::SOLID, depth);

	Point3D pt3(screenPt2.x - halfThickness*cornerToParent.x + (halfThickness+0.5*borderThickness)*childToCorner.x, screenPt2.y - halfThickness*cornerToParent.y + (halfThickness+0.5*borderThickness)*childToCorner.y, 0);
	Point3D pt4(screenPt1.x - halfThickness*cornerToParent.x + halfThickness*childToCorner.x, screenPt1.y - halfThickness*cornerToParent.y + halfThickness*childToCorner.y, 0);
	VisualLine::RenderAntialiasedLine(pt3, pt4, borderColour, borderThickness, VisualLine::SOLID, depth);

	if(screenPt2 != screenPt3)
	{
		Point3D perpChildToParent(-cornerToParent.y, cornerToParent.x, cornerToParent.z);
		if(bReverseOrientation)
			perpChildToParent = Point3D(cornerToParent.y, -cornerToParent.x, cornerToParent.z);

		Point3D pt5(screenPt2.x + halfThickness*cornerToParent.x - halfThickness*perpChildToParent.x, screenPt2.y + halfThickness*cornerToParent.y - halfThickness*perpChildToParent.y, 0);
		Point3D pt6(screenPt3.x - halfThickness*perpChildToParent.x, screenPt3.y - halfThickness*perpChildToParent.y, 0);
		VisualLine::RenderAntialiasedLine(pt5, pt6, borderColour, borderThickness, VisualLine::SOLID, depth);

		Point3D pt7(screenPt3.x + halfThickness*perpChildToParent.x - (halfThickness-0.5*borderThickness)*cornerToParent.x, screenPt3.y + halfThickness*perpChildToParent.y - (halfThickness-0.5*borderThickness)*cornerToParent.y, 0);
		Point3D pt8(screenPt2.x - halfThickness*cornerToParent.x + halfThickness*perpChildToParent.x, screenPt2.y - halfThickness*cornerToParent.y + halfThickness*perpChildToParent.y, 0);
		VisualLine::RenderAntialiasedLine(pt7, pt8, borderColour, borderThickness, VisualLine::SOLID, depth);
	}

	glPopMatrix();
}

void GeoTreeView::RenderRootBranch(const Point3D& pt1, const Point3D& pt2, const Point3D& pt3, float thickness, float depth, const Colour& colour, 
								   float borderThickness, const Colour& borderColour)
{
	const float ROOT_BRANCH_LEGTH = 6.0f * App::Inst().GetResolutionFactor();

	error::ErrorGL::Check();
	glDepthRange(0.0, TERRAIN_START_DEPTH);

	App::Inst().GetViewport()->GetCamera()->SetOrthoCamera();

	glPushMatrix();	
	Point3D implicitRoot = App::Inst().GetMapController()->ProjectToScreen(m_linearLayout->GetImplicitRoot());
	Point3D screenPt2 = App::Inst().GetMapController()->ProjectToScreen(pt2);
	Point3D screenPt3 = App::Inst().GetMapController()->ProjectToScreen(pt3);

	Point3D cornerToParent = screenPt3 - screenPt2;
	cornerToParent.Normalize();

	Point3D childToCorner = implicitRoot - screenPt3;
	childToCorner.Normalize();

	float halfThickness = 0.5*thickness;

	glColor4f(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), colour.GetAlpha());
	glBegin(GL_QUADS);
	glVertex3f(screenPt3.x + halfThickness*cornerToParent.x, screenPt3.y + halfThickness*cornerToParent.y, -depth);
	glVertex3f(screenPt3.x + halfThickness*cornerToParent.x + (ROOT_BRANCH_LEGTH+halfThickness)*childToCorner.x, screenPt3.y + halfThickness*cornerToParent.y + (ROOT_BRANCH_LEGTH+halfThickness)*childToCorner.y, -depth);
	glVertex3f(screenPt3.x - halfThickness*cornerToParent.x + (ROOT_BRANCH_LEGTH+halfThickness)*childToCorner.x, screenPt3.y - halfThickness*cornerToParent.y + (ROOT_BRANCH_LEGTH+halfThickness)*childToCorner.y, -depth);
	glVertex3f(screenPt3.x - halfThickness*cornerToParent.x, screenPt3.y - halfThickness*cornerToParent.y, -depth);		
	glEnd();


	if(borderThickness > 0.0f)
	{
		Point3D pt1(screenPt3.x + halfThickness*cornerToParent.x + halfThickness*childToCorner.x, screenPt3.y + halfThickness*cornerToParent.y + halfThickness*childToCorner.y, 0);
		Point3D pt2(screenPt3.x + halfThickness*cornerToParent.x + (ROOT_BRANCH_LEGTH+halfThickness)*childToCorner.x, screenPt3.y + halfThickness*cornerToParent.y + (ROOT_BRANCH_LEGTH+halfThickness)*childToCorner.y, 0);
		VisualLine::RenderAntialiasedLine(pt1, pt2, borderColour, borderThickness, VisualLine::SOLID, depth);

		Point3D pt3(screenPt3.x - halfThickness*cornerToParent.x + halfThickness*childToCorner.x, screenPt3.y - halfThickness*cornerToParent.y + halfThickness*childToCorner.y, 0);
		Point3D pt4(screenPt3.x - halfThickness*cornerToParent.x + (ROOT_BRANCH_LEGTH+halfThickness)*childToCorner.x, screenPt3.y - halfThickness*cornerToParent.y + (ROOT_BRANCH_LEGTH+halfThickness)*childToCorner.y, 0);
		VisualLine::RenderAntialiasedLine(pt3, pt4, borderColour, borderThickness, VisualLine::SOLID, depth);
	}
	glPopMatrix();

	App::Inst().GetViewport()->GetCamera()->UnsetOrthoCamera();

	error::ErrorGL::Check();
}

uint GeoTreeView::SetGeographicAxisAngleOnRoot(float angle)
{
	return SetGeographicAxisAngle(angle, m_tree->GetRootNode());
}

uint GeoTreeView::SetGeographicAxisAngle(float angle, NodeGeoTree* node)
{
	// change to standard Cartesian angles
	angle = 90 - angle;

	// find extents of locations
	std::vector<NodeGeoTree*> leafNodes = m_tree->GetLeaves();
	float xMin = std::numeric_limits<float>::max();
	float xMax = -std::numeric_limits<float>::max();
	float zMin = std::numeric_limits<float>::max();
	float zMax = -std::numeric_limits<float>::max();
	foreach(NodeGeoTree* leaf, leafNodes)
	{
		Point3D pos = leaf->GetLocationView()->GetPosition();

		if(pos.x < xMin)
			xMin = pos.x;

		if(pos.x > xMax)
			xMax = pos.x;

		if(pos.z < zMin)
			zMin = pos.z;

		if(pos.z > zMax)
			zMax = pos.z;
	}

	// calculate mid-point of locations
	float xMid = xMin + (xMax - xMin)/2;
	float zMid = zMin + (zMax - zMin)/2;
	Point3D midLoc(xMid, 0.0, zMid);

	// calculate unit vector perpendicular to desired geographic axis
	float perpAngle = (angle-90)*DEG_TO_RAD;
	Point3D perpVec(cos(perpAngle), 0.0, -sin(perpAngle));	// negative sign accounts for positive z-axis being to the south

	// calculate unit vector along desired geographic axis
	float geoAxisAngle = angle*DEG_TO_RAD;
	Point3D geoAxisVec(cos(geoAxisAngle), 0.0, -sin(geoAxisAngle));  // negative sign accounts for positive z-axis being to the south

	// calculate length of layout line
	double layoutLineLen = Geometry::Distance(m_layoutLine->GetStartControlPoint(), m_layoutLine->GetEndControlPoint());

	// calculate desired offset from mid-point of locations
	//  -> base radius is based on the distance from mid-point to extreme points
	//  -> twice the offset of the geography line is added to this to give a pleasing offset from all geographic locations
	float offset = 1.1 * sqrt((xMid - xMin) * (xMid - xMin) + (zMid - zMin) * (zMid - zMin));
	offset += 2*GetGeographyLineOffset();

	// find start and end point of new layout line
	Point3D startPt = midLoc + perpVec*offset - geoAxisVec*(0.5*layoutLineLen);
	Point3D endPt = midLoc + perpVec*offset + geoAxisVec*(0.5*layoutLineLen);

	m_layoutLine->SetStartControlPoint(startPt);
	m_layoutLine->SetEndControlPoint(endPt);
	ForceTreeLayout();

	// update displayed node info
	uint numCrossings;
	numCrossings = node->GetNumCrossings();
	m_sigNodeUpdate(NodeInfo(node->GetName(), numCrossings));

	App::Inst().GetViewport()->Refresh(false);

	return numCrossings;
}

uint GeoTreeView::GetNumberOfNodes() const
{ 
	return m_tree->GetNumberOfNodes(); 
}

uint GeoTreeView::GetNumberOfLeaves() const 
{ 
	return m_tree->GetNumberOfLeaves(); 
}

uint GeoTreeView::GetDepthOfTree() const 
{ 
	return m_tree->GetDepth(m_tree->GetRootNode()); 
}

bool GeoTreeView::IsMultifurcating() const
{ 
	return m_tree->IsMultifurcating(); 
}

std::vector<std::wstring> GeoTreeView::GetLeafNames() const
{
	return m_tree->GetLeafNames();
}

std::wstring GeoTreeView::GetNewickString() const
{
	GeoTreeIO geoTreeIO;
	std::wstring newickStr = geoTreeIO.GetNewickString(m_tree);
	newickStr = newickStr.substr(0, newickStr.find(';')+1);
	return newickStr;
}