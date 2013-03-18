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

#ifndef _GENGIS_GEO_TREE_VIEW
#define _GENGIS_GEO_TREE_VIEW

#include "../core/Precompiled.hpp"

#include "../core/NodeGeoTree.hpp"
#include "../core/Layer.hpp"
#include "../core/View.hpp"
#include "../core/VisualLines.hpp"
#include "../core/VisualPoints.hpp"
#include "../core/VisualMarker.hpp"
#include "../core/GeoTreeOptLeafOrder.hpp"

#include "../utils/Colour.hpp"
#include "../utils/Tree.hpp"

class GenGIS::GeoTreeView;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::GeoTreeView * t, const unsigned int file_version);
	}
}

namespace GenGIS 
{

	/** Node info. */
	typedef struct sNODE_INFO
	{
		/** Constructor. */
		sNODE_INFO(std::wstring _name, uint _crossings): name(_name), crossings(_crossings) {}

		/** Name of node. */
		std::wstring name;

		/** Number of crossings within subtree. */
		uint crossings;
	} NodeInfo;

	class GeoTreeView : public View
	{
	public:
		/** Function signature for node update signal. */
		typedef boost::signal<void (NodeInfo nodeInfo)> SigNodeUpdate;

	public:
		enum LAYOUT { NONE, CLADOGRAM_3D, SLANTED_CLADOGRAM_3D, SLANTED_PHYLOGRAM_3D, CLADOGRAM_2D, SLANTED_CLADOGRAM_2D, PHYLOGRAM_2D, ELLIPSE_QUADRANT };
		enum ORIENTATION { HORIZONTAL, VERTICAL };	
		enum ACTIVE_VIEW { NO_ACTIVE_VIEW, LAYOUT_LINE, LAYOUT_ELLIPSE, GEOGRAPHIC_AXIS, GEO_POINT, GEO_LINE, TREE };
		enum TREE_COLOUR_STYLE { COLOUR_SINGLE, COLOUR_DISCRETE, COLOUR_CONTINUOUS };

	public:
		/** 
		* @brief Constructor. 
		* @param tree Tree to visualize.
		* @param mapLayer Map layer associated with tree.
		*/
		explicit GeoTreeView(Tree<NodeGeoTree>::Ptr tree, boost::shared_ptr<Layer> mapLayer);

		/** Destructor. */
		~GeoTreeView() { gluDeleteQuadric(m_markerQuadric); }

		/** Render tree. */
		void Render();

		/**
		* @brief Set a slot (callback) to be signalled when the selected node changes.
		* @param slot Slot (function) to call.
		*/
		void SignalNodeUpdate(const SigNodeUpdate::slot_type& slot) { m_sigNodeUpdate.connect(slot); }

		/**
		* @brief Set geographic axis to the specified angle.
		* @param angle Angle to set geographic axis to (in compass coordinates).
		* @param node Node analysis is being applied to.
		*/
		uint SetGeographicAxisAngle(float angle, NodeGeoTree* node);

		/**
		* @brief Set geographic axis to the specified angle.
		* @param angle Angle to set geographic axis to (in compass coordinates).
		*/
		uint SetGeographicAxisAngleOnRoot(float angle);

		/** 
		* @brief Project tree onto a set of leaf nodes.
		* @param names Names of leaf nodes to project tree onto.
		* Note: names will contain a list of all the names not found in the tree after function returns.
		*/
		void ProjectTree(std::vector<std::wstring>& names);

		/**
		* @brief Project tree to the set of active locations or sequences.
		* @param locationLayers Set of all location layers.
		*/
		void ProjectToActiveSet(std::vector<LocationLayerPtr>& locationLayers);

		/** 
		* @brief Restore original tree.
		*/
		void RestoreTree() { m_tree = m_originalTree->Clone(); ForceTreeLayout(); }

		/**
		* @brief Get current tree.
		*/
		Tree<NodeGeoTree>::Ptr GetTreeLayer() const { return m_tree; }

		/** 
		* @brief Calculate grid positions of all nodes in tree for a given layout style. 
		* @param layout Desired layout style.
		*/
		void SetLayout(LAYOUT layout);

		/** Force a recalculation of the position of the tree and all auxiliary visual components. */
		void ForceTreeLayout();

		/** Get current layout of geograph tree model. */
		LAYOUT GetLayout() const { return m_layout; }

		/** Get visibility of tree layout line. */
		bool GetLayoutLineVisibility() const;

		/** 
		* @brief Set visibility of tree layout line. 
		* @param state Desired visibility state for layout line.
		*/
		void SetLayoutLineVisibility(bool state);

		/** Get visibility of tree layout ellipse. */
		bool GetLayoutEllipseVisibility() const;

		/** 
		* @brief Set visibility of tree layout ellipse. 
		* @param state Desired visibility state for layout ellipse.
		*/
		void SetLayoutEllipseVisibility(bool state);

		/** Get visibility of layout primative. */
		bool GetLayoutPrimativeVisibility() const;

		/** 
		* @brief Set visibility of tree layout primative. 
		* @param state Desired visibility state for layout primative.
		*/
		void SetLayoutPrimativeVisibility(bool state);

		/** Get 2D layout line. */
		LayoutLinePtr GetLayoutLine() const { return m_layoutLine; }

		/** Get 2D layout ellipse. */
		LayoutEllipsePtr GetLayoutEllipse() const { return m_layoutEllipse; }

		/**
		* @brief Set 2D layout line. 
		* @param line Layout line.
		*/
		void SetLayoutLine(LayoutLinePtr line);

		/**
		* @brief Set 2D layout ellipse. 
		* @param ellipse Layout ellipse.
		*/
		void SetLayoutEllipse(LayoutEllipsePtr ellipse);

		/** Check if a layout line has been assigned to the tree. */
		bool IsLayoutLine() const;

		/** Check if a layout ellipse has been assigned to the tree. */
		bool IsLayoutEllipse() const;

		/**
		* @brief Set geographical axis.
		* @param geoAxisPolyline Geographical axis.
		*/
		void SetGeographicAxis(GeoAxisPolylinePtr geoAxisPolyline);

		/** Check if a geographical axis has been assigned to the tree. */
		bool IsGeographicAxis() const;

		/** Get visibility of geographical axis. */
		bool GetGeographicAxisVisibility() const;

		/** 
		* @brief Set visibility of geographical axis. 
		* @param state Desired visibility state for geographical axis.
		*/
		void SetGeographicAxisVisibility(bool state);

		/** 
		* @brief Extend geographic axis.
		*/
		void ExtendGeographicAxis();

		/** Flip tree about the layout line. */
		void OnTreeFlip();

		/** Get leaf node size. */
		float GetLeafNodeSize() const;

		/** Set leaf node size. */
		void SetLeafNodeSize(float size) { m_leafNodeSize = size; }

		/** Get internal node size. */
		float GetInternalNodeSize() const;

		/** Set internal node size. */
		void SetInternalNodeSize(float size) { m_internalNodeSize = size; }

		/** Get tree line thickness. */
		float GetLineThickness() const;

		/** Set tree line thickness. */
		void SetLineThickness(float thickness) { m_lineThickness = thickness; }

		/** Get 2D tree orientation. */
		float GetOrientation() const { return m_orientation; }

		/** Set 2D tree orientation. */
		void SetOrientation(ORIENTATION orientation);

		/** Get tree height. */
		float GetHeight() const { return m_height; }

		/** Set tree height. */
		void SetHeight(float height);

		/** Get tree colour. */
		Colour GetColour() const { return m_colour; }

		/** Set tree colour. */
		void SetColour(const Colour& colour) { m_colour = colour; }

		/** Get state of optimize topology flag. */
		bool GetOptimizeTopology() const { return m_bOptimizeTopology; }

		/** Set state of optimize topology flag. */
		void SetOptimizeTopology(bool state) { m_bOptimizeTopology = state; }

		/** Get visibility of leaf labels. */
		bool GetLabelVisibility() const { return m_bShowLabels; }

		/** 
		* @brief Set visibility of leaf labels. 
		* @param state Desired visibility state of leaf labels.
		*/
		void SetLabelVisibility(bool state) { m_bShowLabels = state; }

		/** Get font size. */
		uint GetFontSize() const { return m_fontSize; }

		/** Set font size. */
		void SetFontSize(uint size);

		/** Get font colour. */
		Colour GetFontColour() const { return m_fontColour; }

		/** Set font colour. */
		void SetFontColour(const Colour& colour) { m_fontColour = colour; }

		/** Show properties dialog for this view. */
		void ShowPropertiesDlg();

		/** Set the visibility of the currently select component of a view. */
		void SetSelectionVisiblity( bool visible );

		/** Set visual properties of internal 3D drop lines. */
		void Set3dInternalDropLineStyle(const VisualLine& line) { m_3dInternalDropLine = line; }

		/** Get visiual properties of internal 3D drop lines. */
		VisualLine Get3dInternalDropLineStyle() const { return m_3dInternalDropLine; }

		/** Set visual properties of leaf 3D drop lines. */
		void Set3dLeafDropLineStyle(const VisualLine& line) { m_3dLeafDropLine = line; }

		/** Get visiual properties of leaf 3D drop lines. */
		VisualLine Get3dLeafDropLineStyle() const { return m_3dLeafDropLine; }

		/** Get/set location lines. */
		VisualLines& LocationLines() { return m_locationLines; }

		/** Get/set correlation lines. */
		VisualLines& CorrelationLines() { return m_correlationLines; }

		/** Get/set geography line. */
		VisualLine& GeographyLine() { return m_geographyLine; }

		/** Get/set geographic points. */
		VisualPoints& GeographicPoints() { return m_geoPts; }

		/* Set offset of geography line percentage (in percentage of layout line length). */
		void SetGeographyLineOffsetPercentage(float offset) { m_geographyLineOffsetPercentage = offset; }

		/* Get offset of geography line percentage (in percentage of layout line length). */
		float GetGeographyLineOffsetPercentage() const { return m_geographyLineOffsetPercentage; }

		/** Get geography line offset. */
		float GetGeographyLineOffset() const;

		/* Set offset of geography line. */
		void SetSpreadGeographyPts(bool bSpread) { m_bSpreadGeoPts = bSpread; }

		/* Get offset of geography line. */
		bool GetSpreadGeographyPts() const { return m_bSpreadGeoPts; }

		/** Get node with the given id. */
		NodeGeoTree* GetNode(const std::wstring& id) const { return m_tree->GetNode(id); }

		void SetLeafNodeBorderSize(float size) { m_leafNodeBorderSize = size; }
		float GetLeafNodeBorderSize() const { return m_leafNodeBorderSize; }

		void SetInternalNodeBorderSize(float size) { m_internalNodeBorderSize = size; }
		float GetInternalNodeBorderSize() const { return m_internalNodeBorderSize; }

		void SetGeoPtsBorderSize(float size) { m_geoPtsBorderSize = size; }
		float GetGeoPtsBorderSize() const { return m_geoPtsBorderSize; }

		void SetBranchBorderSize(float size) { m_branchBorderSize = size; }
		float GetBranchBorderSize() const { return m_branchBorderSize; }

		void SetLocationLineBorderSize(float size) { m_locationLineBorderSize = size; }
		float GetLocationLineBorderSize() const { return m_locationLineBorderSize; }

		void SetCorrelationLineBorderSize(float size) { m_correlationLineBorderSize = size; }
		float GetCorrelationLineBorderSize() const { return m_correlationLineBorderSize; }

		void SetLeafNodeBorderColour(const Colour& colour) { m_leafNodeBorderColour = colour; }
		Colour GetLeafNodeBorderColour() const { return m_leafNodeBorderColour; }

		void SetInternalNodeBorderColour(const Colour& colour) { m_internalNodeBorderColour = colour; }
		Colour GetInternalNodeBorderColour() const { return m_internalNodeBorderColour; }

		void SetGeoPtsBorderColour(const Colour& colour) { m_geoPtsBorderColour = colour; }
		Colour GetGeoPtsBorderColour() const { return m_geoPtsBorderColour; }

		void SetLocationLineBorderColour(const Colour& colour) { m_locationLineBorderColour = colour; }
		Colour GetLocationLineBorderColour() const { return m_locationLineBorderColour; }

		void SetCorrelationLineBorderColour(const Colour& colour) { m_correlationLineBorderColour = colour; }
		Colour GetCorrelationLineBorderColour() const { return m_correlationLineBorderColour; }

		void SetBranchBorderColour(const Colour& colour) { m_branchBorderColour = colour; }
		Colour GetBranchBorderColour() const { return m_branchBorderColour; }

		void SetTreeColourStyle(TREE_COLOUR_STYLE style) { m_treeColourStyle = style; }
		TREE_COLOUR_STYLE GetTreeColourStyle() { return m_treeColourStyle; }

		/** Set flag indicating if order of geographic points specified by a geographic axis should be reversed. */
		void SetReverseOrderGeographicPts(bool state) { m_bReverseOrderGeographicPts = state; ForceTreeLayout(); }

		/** Get flag indicating if order of geographic points specified by a geographic axis should be reversed. */
		bool GetReverseOrderGeographicPts() const { return m_bReverseOrderGeographicPts; }

		/** Get number of nodes in tree. */
		uint GetNumberOfNodes() const; 

		/** Get number of leaves in tree. */
		uint GetNumberOfLeaves() const; 

		/** Get name of leaves in tree. */
		std::vector<std::wstring> GetLeafNames() const;

		/** Get depth of tree. */
		uint GetDepthOfTree() const; 

		/** Check if tree is multifurcating. */
		bool IsMultifurcating() const;

		/**
		* @brief Perform Monte Carlo significance test between tree topology and position of points on layout primative.
		* @param iterations Number of iterations to perform when creating probability density function.
		* @param pdf Probability density function generated by significance test.
		* @param originalNumberCrossings Number of crossings for actual data.
		* @param node Node to apply test to.
		* @param bSuccess Flag indicating if test was run to completion or terminated early.
		* @return P-value from significance test.
		*/
		double SignificanceTest(uint iterations, std::map<uint, uint>& pdf, uint& originalNumberCrossings, NodeGeoTree* node, bool& bSuccess) const;

		/**
		* Calculate goodness-of-fit for all linear axes for the specified subtree.
		*/
		std::vector<LinearResults> PerformLinearAxesAnalysis(NodeGeoTree* node);

		/**
		* Calculate goodness-of-fit for all linear axes.
		*/
		std::vector<LinearResults> PerformLinearAxesAnalysisOnRootNode();

		/**
		* Get results of last linear axes analysis.
		*/
		std::vector<LinearResults> GetLinearAxesResults() const { return m_linearAxesResults; }

		/** 
		* @brief Process mouse left button down events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		bool MouseLeftDown(const Point2D& mousePt);

		/** 
		* @brief Process mouse left button up events. 
		* @param mousePt Screen position of mouse.
		*/
		void MouseLeftUp(const Point2D& mousePt);

		/** 
		* @brief Process mouse left button float click events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		void MouseLeftDblClick(const Point2D& mousePt);

		/** 
		* @brief Process mouse right button down events. 
		* @param mousePt Screen position of mouse.
		* @param popupMenu Popup menu to populate.
		* @return True if view is processing the event, else false.
		*/
		bool MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu);

		/** 
		* @brief Process mouse dragging events. 
		* @param mousePt Screen position of mouse.
		*/
		void MouseDragging(const Point2D& mousePt);

		/** 
		* @brief Process mouse moving events. 
		* @param mousePt Screen position of mouse.
		* @return True if the OpenGL viewport should be refreshed.
		*/
		bool MouseMove(const Point2D& mousePt);

		/** 
		* @brief Process keyboard events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		void KeyboardEvent(wxKeyEvent& event);

		/** Get the Map layer. */
		boost::shared_ptr<Layer> GetMapLayer() const { return m_mapLayer; }

		/** Set the id of the tree layer associated with this view. */
		virtual void SetTreeLayerId(uint treeLayerId) { m_treeLayerId = treeLayerId; }

		/** Get the id of the tree layer associated with this view. */
		virtual uint GetTreeLayerId() const { return m_treeLayerId; }

		/** Get id of root. */
		NodeGeoTree* GetRootNode() const { return m_tree->GetRootNode(); }

		/** Get id of currently selected node. */
		NodeGeoTree* GetSelectedNode() const { return m_selectedNode; }

		/** Get Newick representation of tree. */
		std::wstring GetNewickString() const;

		/** Check if tree has a nonlinear axis assigned to it. */
		bool HasNonlinearAxis() { return m_geoAxisPolyline; }

		/** Get results for all permutation of nonlinear polylines. */
		wxArrayString GetNonlinearPermutationResults() { return m_nonlinearPermutationStrs; }

		/** Set desired ordering of nonlinear polylines. */
		void SetNonlinearAxisOrdering(uint bestOrdering);

	protected:
		/** Render 3D tree or 2D tree along layout line. */
		void RenderTree();

		/** Render droplines for 3D phylogram. */
		void RenderPhylogramDropLines3D();

		/** Render 2D tree node. */
		void RenderNode2D(const Point3D& pt, const Colour& colour, bool bLeafNode, bool bSelected);

		/** Render 3D tree node. */
		void RenderNode3D(const Point3D& pt, const Colour& colour, bool bLeafNode, bool bSelected);

		/** Render 3D dropline. */
		void RenderDropline3D(const Point3D& pt, const Colour& colour, VisualLine::LINE_STYLE style, float size);

		/** 
		* @brief Render point.
		* @param pt Point to render
		* @param colour Colour of point.
		* @param size Size of point.
		* @param shape Shape of point.
		* @param rotation Rotation of point about x-axis.
		* @param depth Depth of point in orthographic projection.
		* @param bSelected Flag indicating if point is currently selected.
		*/
		void RenderPoint(const Point3D& pt, const Colour& colour, float size, VisualMarker::MARKER_SHAPE shape, 
			float rotation, float depth, bool bSelected, const Colour& borderColour, float borderSize);

		/** 
		* @brief Render a line which is subject to occlusion from the terrain.
		* @param line Line to render
		* @param colour Colour of line.
		* @param thickness Thickness of line.
		* @param lineStyle Line style of line.
		* @param bSelected Flag indicating if line is currently selected.
		*/
		void RenderLine3D(const Line3D& line, const Colour& colour, float thickness, VisualLine::LINE_STYLE lineStyle, bool bSelected);

		/** Render location lines which connect geographic locations to either geographic poitns or layout points. */
		void RenderLocationLines();

		/** Render correlation lines which connect geographic point to layout points. */
		void RenderCorrelationLines();

		/** Render phylogram drop lines which connect leaf nodes to the layout line. */
		void RenderPhylogramDropLines();

		/** Render geography line where geographic points are laid out. */
		void RenderGeographyLine();

		/** Render geographic points.*/
		void RenderGeographicPts();

		/** Render leaf node labels. */
		void RenderLabels();

		/** Render two lines meeting at a right angle at pt2. */
		void RenderCornerLines(const Point3D& pt1, const Point3D& pt2, const Point3D& pt3, 
			float angle, float thickness, float depth, const Colour& colour, 
			float borderThickness, const Colour& borderColour, bool bReverseOrientation, bool bSelected);

		void RenderCorner(const Point3D& screenPt1, const Point3D& screenPt2, const Point3D& screenPt3, 
			float angle, float thickness, float depth);

		void RenderCornerOutline(const Point3D& screenPt1, const Point3D& screenPt2, const Point3D& screenPt3, 
			float lineThickness, const Colour& borderColour, float borderThickness, bool bReverseOrientation, float depth);

		void RenderRootBranch(const Point3D& pt1, const Point3D& pt2, const Point3D& pt3, float thickness, 
			float depth, const Colour& colour, 
			float borderThickness, const Colour& borderColour);

		/** Layout nodes for 3D tree. */
		void LayoutTree3D();

		/** Layout nodes for 3D cladogram. */
		void LayoutCladogram3D(double maxDistBetweenLeaves, double maxElevationOfLeaves);

		/** Layout nodes for 3D phylogram. */
		void LayoutPhylogram3D(double maxDistBetweenLeaves, double maxElevationOfLeaves);

		/** Layout nodes for 2D tree. */
		void LayoutTrees2D();

		/** Layout location and correlation lines. */
		void LayoutConnectionLines();

		/** Layout line from geographic location to layout point. */
		void LayoutGeoLocationToLayoutPt();

		/** Layout line from geographic location to geographic point. */
		void LayoutGeoLocationToGeoPt();

		/** Layout line from geographic point to layout point. */
		void LayoutGeoPtToLayoutPt();

		/** Layout line from leaf nodes to layout line for phylogram. */
		void LayoutPhylogramDropLines();

		/**
		* @brief Calculate mean of a set of nodes.
		* @param nodes Nodes to calculate mean of.
		* @param mean Mean coordinate of nodes.
		* @return True if mean was calculated, false if one or more nodes was not set.
		*/
		bool CalculateMean(const std::vector<NodeGeoTree*>&  nodes, Point3D& mean);

		/** Optimize the ordering of leaf nodes along a line so they match, as closely as possible, the geographical ordering. */
		void OptimizeLeafNodeOrdering();

		/** Find order of geographic locations when projected onto the geographic line. */
		void FindProjectedOrdering();

		/** Set desired order of leaf nodes to the position of geographic locations along the geographic axis. */
		void SetGeographicAxisOrdering();

		/** 
		* @brief Get direction of layout line and direction in which to draw 2D tree. 
		* @param layoutLineDir Set to a unit vector in the direction of the layout line.
		* @param treeDir Set to a unit vector in the direction the tree should be drawn.
		*/
		const void CalculateTreeDirection(Point3D& layoutLineDir, Point3D& treeDir) const;

		/** 
		* @brief Get colour of the location site that has the same id or contains a sequence with the same id as the leaf node.
		* @param leafId Id of leaf node to get colour information from.
		* @return Colour of location site with the given id.
		*/
		Colour GetLocationColour(const std::wstring& leafId) const;

		/**
		* @brief Get colour of given node. 
		* @param node Node to get colour of.
		* @return Colour of node.
		*/
		Colour GetNodeColour(NodeGeoTree* node) const;

		/** Check if current position of mouse overlays with a node of the tree. */
		bool IsTreeClicked(const Point2D& mousePt, bool bLeftButton);

		/** Check if current position of mouse overlays with a geographic point. */
		bool IsGeoPointClicked(const Point2D& mousePt);

		/** Check if current position of mouse overlays with a control point of the geography line. */
		bool IsGeoLineControlPointClicked(const Point2D& mousePt);

		/** Translate entire tree. */
		void TranslateTree();

		/** Translate a control point of the geography line. */
		void TranslateGeoLine();

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const GeoTreeView * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Active geographic tree model. May be subject to modification (e.g., projection onto a set of leaf nodes). */
		Tree<NodeGeoTree>::Ptr m_tree;

		/** Map layer associated with tree. */
		boost::shared_ptr<Layer> m_mapLayer;

		/** Original tree model. */
		Tree<NodeGeoTree>::Ptr m_originalTree;

		/** Id of tree layer associated with this view. */
		uint m_treeLayerId;

		/** Signal that the selected node has changed. */
		SigNodeUpdate m_sigNodeUpdate;

		/** Current layout style of tree. */
		LAYOUT m_layout;

		/** Current orientation of 2D tree. */
		ORIENTATION m_orientation;

		/** Layout line for 2D tree. */
		LayoutLinePtr m_layoutLine;

		/** Layout ellipse for 2D tree. */
		LayoutEllipsePtr m_layoutEllipse;

		/** Geographical axis for tree. */
		GeoAxisPolylinePtr m_geoAxisPolyline;

		/** Used to render line markers. */
		GLUquadricObj* m_markerQuadric;

		/** Lines connecting geographic points to their geographical locations. */
		VisualLines m_locationLines;

		/** Lines connecting layout points to geographic points. */
		VisualLines m_correlationLines;

		/** Geography line for laying out geographic points. */
		VisualLine m_geographyLine;

		/** Phylogram droplines which connect leaf nodes to the layout line. */
		VisualLines m_phylogramDropLines;

		/** Geographic points. */
		VisualPoints m_geoPts;

		/** Modified layout line for translating tree. */
		Line3D m_modLayoutLine;

		/** Geography line under modification. */
		VisualLine m_modGeographyLine;

		/** Font class used to render TrueType fonts on an OpenGL canvas. */
		FontPtr m_font;

		/** Flag indicating if the leaf labels are visible. */
		bool m_bShowLabels;

		/** Font file. */
		std::string m_fontFile;

		/** Size of font. */
		uint m_fontSize;

		/** Colour of font. */
		Colour m_fontColour;

		/** Thickness of tree lines. */
		float m_lineThickness;

		/** Height of tree. */
		float m_height;

		/** Colour of tree. */
		Colour m_colour;

		/** Size of tree leaf nodes. */
		float m_leafNodeSize;

		/** Size of tree internal nodes. */
		float m_internalNodeSize;

		/** Flag indicating if the tree topology should be optimized to the geography. */
		bool m_bOptimizeTopology;

		/** Currently active view. */
		ACTIVE_VIEW m_activeView;

		/** Flag indicating if the currently active view has moved. */
		bool m_bActiveViewMoved;

		/** Perform linear layout. */
		GeoTreeLinearLayoutPtr m_linearLayout;

		/** Perform elliptical layout. */
		GeoTreeEllipticalLayoutPtr m_ellipticalLayout;

		/** Geography line offset as a percentage of the layout line length. */
		float m_geographyLineOffsetPercentage;

		/** Spread out geographic points along layout line. */
		bool m_bSpreadGeoPts;

		/** Index of last selected node. */
		NodeGeoTree* m_selectedNode;

		/** Indicate if the first or last control point on the geography line is selected. */
		bool m_bSelectedFirstGeoLineCP;

		/** Flag indicating if the order of geographic points specified by a geographic axis should be reversed. */
		bool m_bReverseOrderGeographicPts;

		/** Indicates visual properties of internal 3D drop lines. */
		VisualLine m_3dInternalDropLine;

		/** Indicates visual properties of leaf 3D drop lines. */
		VisualLine m_3dLeafDropLine;

		float m_leafNodeBorderSize;
		Colour m_leafNodeBorderColour;

		float m_internalNodeBorderSize;
		Colour m_internalNodeBorderColour;

		float m_geoPtsBorderSize;
		Colour m_geoPtsBorderColour;

		float m_locationLineBorderSize;
		Colour m_locationLineBorderColour;

		float m_correlationLineBorderSize;
		Colour m_correlationLineBorderColour;

		float m_branchBorderSize;
		Colour m_branchBorderColour;

		TREE_COLOUR_STYLE m_treeColourStyle; 

		/** Save results of linear axes analysis. **/
		std::vector<LinearResults> m_linearAxesResults;

		wxArrayString m_nonlinearPermutationStrs;
	};

}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const GeoTreeView * t, const unsigned int file_version)
		{
			//// Save data required to construct instance
			//ar << t->GetTreeLayer();         // m_tree (Tree<NodeGeoTree>::Ptr)
			//ar << t->GetMapLayer();          // m_mapLayer (boost::shared_ptr<Layer>)

			ar << t->m_tree;              // Tree<NodeGeoTree>::Ptr
			ar << t->m_mapLayer;          // boost::shared_ptr<Layer>
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, GeoTreeView * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			Tree<NodeGeoTree>::Ptr _tree;
			ar >> _tree;

			boost::shared_ptr<GenGIS::Layer> _mapLayer;
			ar >> _mapLayer;

			::new(t)GeoTreeView(_tree, _mapLayer);
		}
	}
}

#endif
