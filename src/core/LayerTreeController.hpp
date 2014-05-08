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

#ifndef _GENGIS_LAYER_TREE_CTRL_
#define _GENGIS_LAYER_TREE_CTRL_

#include "../core/Precompiled.hpp"

#include "../core/ChartSetView.hpp"
#include "../core/GeoTreeView.hpp"

#include "../utils/LayerTable.hpp"

class GenGIS::LayerTreeController;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::LayerTreeController * t, const unsigned int file_version);
	}
}

namespace GenGIS 
{
	/**
	* @brief Controller for layer tree model. 
	*/
	class LayerTreeController: private Uncopyable
	{
	public:
		/** Constructor. */
		explicit LayerTreeController(LayerTreeModelPtr tree, LayerTreeViewPtr treeView);

		/** Destructor. */
		~LayerTreeController() {}

		/** Get currently selected layer. */
		LayerPtr GetSelectedLayer() const;

		/** Get layer tree model. */
		LayerTreeModelPtr GetLayerTreeModel() const;

		/**
		* @brief Add new map layer to currently selected node in layer tree.
		* @param Map layer to add.
		*/
		//bool AddMapLayer(MapLayerPtr map, bool display);
		bool AddMapLayer(MapLayerPtr map);

		/**
		* @brief Add new vector map layer to currently selected node in layer tree.
		* @param Vector Map layer to add.
		*/
		bool AddVectorMapLayer( VectorMapLayerPtr vectorMap, LayerPtr selectedLayer,wxTreeItemId selectionId );

		/**
		* @brief Add new tree layer to currently selected node in layer tree.
		* @param Tree layer to add.
		*/
		bool AddTreeLayer(TreeLayerPtr tree);

		/**
		* @brief Add new location set layer to currently selected node in layer tree. 
		* @param Location set layer to add.
		*/
		bool AddLocationSetLayer(LocationSetLayerPtr locationSet);

		/**
		* @brief Add new location set layer to currently selected node in layer tree at given position.
		* @param Location set layer to add, position to add it at.		
		*/
		bool AddLocationSetLayerAtPosition(LocationSetLayerPtr locationSet, int pos);

		/**
		* @brief Add new sequence layer to currently selected node in layer tree. 
		* @param Sequence layer to add.
		*/
		bool AddSequence(SequenceLayerPtr sequence, SequenceModelPtr sequenceModel, std::set<std::wstring>& missingLocations);

		/**
		* @brief Add new sequence layer to currently selected node in layer tree. 
		* @param Location Set layer at which to add the sequence.
		*/
		void AddSequence(LocationSetLayerPtr locationSetPtr);

		/** 
		* @brief Get name of specified layer.
		* @param id Id specifying layer of interest.
		* @return Name of specified layer.
		*/
		std::wstring GetName(uint id) const;

		/** 
		* @brief Set name of specified layer.
		* @param layer Pointer to the specified layer.
		* @parm name Desired name for specified layer.
		*/
		void SetName(LayerPtr layer, const std::wstring& name);

		/** 
		* @brief Get description of specified layer.
		* @param id Id specifying layer of interest.
		* @return Description of specified layer.
		*/
		std::wstring GetDescription(uint id) const;

		/** 
		* @brief Set description of specified layer.
		* @param id Id specifying layer of interest.
		* @parm description Desired description for specified layer.
		*/
		void SetDescription(uint id, const std::wstring& description);

		/** 
		* @brief Get authours of specified layer.
		* @param id Id specifying layer of interest.
		* @return Authours of specified layer.
		*/
		std::wstring GetAuthours(uint id) const;

		/** 
		* @brief Set authours of specified layer.
		* @param id Id specifying layer of interest.
		* @parm authours Desired authours for specified layer.
		*/
		void SetAuthours(uint id, const std::wstring& authours);

		/** 
		* @brief Get path of specified layer.
		* @param id Id specifying layer of interest.
		* @return Path of specified layer.
		*/
		std::wstring GetPath(uint id) const;

		/** 
		* @brief Get filename of specified layer.
		* @param id Id specifying layer of interest.
		* @return Path of specified layer.
		*/
		std::wstring GetFilename(uint id) const;

		/** 
		* @brief Check if specified layer is visible.
		* @param id Id specifying layer of interest.
		* @return Visibility of specified layer.
		*/
		const bool IsVisible(uint id) const;

		/** 
		* @brief Set visibility of specified layer.
		* @param id Id specifying layer of interest.
		* @parm visibility Desired visibility for specified layer.
		*/
		void SetVisible(uint id, bool visibility);

		/** Get number of study layers. */
		uint GetNumStudyLayers() const;

		/** Get study layer. */
		StudyLayerPtr GetStudyLayer(uint index) const;

		/** Get number of map layers. */
		uint GetNumMapLayers() const;

		/** Get map layer. */
		MapLayerPtr GetMapLayer(uint index) const;

		/** Get number of map layers. */
		uint GetNumVectorMapLayers() const;

		/** Get vector map layer. */
		VectorMapLayerPtr GetVectorMapLayer(uint index) const;

		/** Get number of location set layers. */
		uint GetNumLocationSetLayers() const;

		/** Get location set layer. */
		LocationSetLayerPtr GetLocationSetLayer(uint index) const;

		/** Get number of location layers. */
		uint GetNumLocationLayers() const;

		/** Get location set layer. */
		LocationLayerPtr GetLocationLayer(uint index) const;

		/** Get location set layer by id. */
		LocationLayerPtr GetLocationLayerById( uint id ) const;

		/** Get location layers. */
		std::vector<LocationLayerPtr> GetLocationLayers() const { return m_locations; }

		/** Get location controller with the given id. */
		LocationControllerPtr GetLocationController(const std::wstring& id) const;

		/** Get number of sequence layers. */
		uint GetNumSequenceLayers() const;

		/** Get sequence layer. */
		SequenceLayerPtr GetSequenceLayer(uint index) const;

		/** Get sequence layers. */
		std::vector<SequenceLayerPtr> GetSequenceLayers() const { return m_sequences; }

		/** Get number of sequence layers. */
		uint GetNumTreeLayers() const;

		/** Get tree layer. */
		TreeLayerPtr GetTreeLayer(uint index) const;

		/** Get tree layer by id. */
		TreeLayerPtr GetTreeLayerById( uint id ) const;

		/** Get layer tree control. */
		wxTreeCtrl* GetTreeCtrl() const;

		/** Get layer tree view. */
		LayerTreeViewPtr GetTreeView() const;

		/** 
		* @brief Handle mouse left button down event. 
		* @param x,y Position of mouse cursor in OpenGL window. 
		* @return True if view in layer tree is processing this event.
		*/
		bool MouseLeftDown(const Point2D& mousePt);

		/** 
		* @brief Process mouse right button down events. 
		* @param mousePt Screen position of mouse.
		* @param popupMenu Popup menu to populate.
		* @param layerPtr Layer associated with view processing the right click event.
		* @return True if view is processing the event, else false.
		*/
		bool MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu, LayerPtr& layerPtr);


		/** Handler the layer tree render event. */
		void OnRender();

		/** Handle the layer tree select event. */
		void OnSelect( wxTreeEvent& event );

		/** Handle the layer tree left-click event. */
		void OnClick( wxMouseEvent& event );

		/** Handle the layer tree left float click event. */
		void OnLeftDClick( wxMouseEvent& event );

		/** Remove layer from layer tree. */
		void OnLayerRemove( wxCommandEvent& event );
		void OnAllLayersRemove( wxCommandEvent& event );

		/** Hide all layers. */
		void OnHideAll( wxCommandEvent& event );

		/** Show all layers. */
		void OnShowAll( wxCommandEvent& event );

		/** 
		* @brief Change tree layout style for the selected tree. 
		* @param layout Desired layout for tree.
		* @param layoutLine Line to use for 2D layouts.
		* @return True if layout line is acquired by tree, otherwise false. 
		*/
		bool OnLayoutTree(GeoTreeView::LAYOUT layout, LayoutLinePtr layoutLine);

		/** 
		* @brief Change tree layout style for the selected tree. 
		* @param layout Desired layout for tree.
		* @param layoutEllipse Ellipse to use for 2D layouts.
		* @return True if layout ellipse is acquired by tree, otherwise false. 
		*/
		bool OnLayoutTree(GeoTreeView::LAYOUT layout, LayoutEllipsePtr layoutEllipse);


		/** 
		* @brief Assign a geographical axis to the selected tree
		* @param geoAxisPolyline Desired geographical axis.
		* @return True if geographical axis is acquired by tree, otherwise false. 
		*/
		bool OnAssignGeoAxisToTree(GeoAxisPolylinePtr geoAxisPolyline);

		/** 
		* @brief Specify if order of geographic points specified by a geographic axis should be reversed
		* @param state Flag indicating if order should be reversed.
		* @return True if geographical axis is acquired by tree, otherwise false. 
		*/
		void OnReverseGeoAxisOrder(bool state);

		/** Set visibility of tree layout line. */
		void OnTreeLayoutLineVisibility();

		/** Set visibility of tree layout line. */
		void OnTreeFlip();

		/** Set visibility of tree layout ellipse. */
		void OnTreeLayoutEllipseVisibility();

		/** Set visibility of tree geographic axis. */
		void OnTreeGeographicAxisVisibility();

		/** 
		* @brief Layout charts in default position
		* @param layout Desired layout for charts.
		* @return True if layout line is acquired by chart set, otherwise false. 
		*/
		bool OnLayoutCharts(ChartSetView::LAYOUT_CHART layout);

		/** 
		* @brief Layout charts on a layout line.
		* @param layout Desired layout for charts.
		* @param layoutLine Line to use for layouts.
		* @return True if layout line is acquired by chart set, otherwise false. 
		*/
		bool OnLayoutCharts(ChartSetView::LAYOUT_CHART layout, LayoutLinePtr layoutLine);

		/** 
		* @brief Layout charts on a layout ellipse.
		* @param layout Desired layout for charts.
		* @param layoutEllipse Ellipse to use for layouts.
		* @return True if layout ellipse is acquired by chart set, otherwise false. 
		*/
		bool OnLayoutCharts(ChartSetView::LAYOUT_CHART layout, LayoutEllipsePtr layoutEllipse);

		/** Set visibility of chart set layout line. */
		void OnChartSetLayoutLine();

		/** Set visibility of chart set layout ellipse. */
		void OnChartSetLayoutEllipse();

		/** Set visibility of leaf node labels. */
		void OnLeafNodeLabels();

		/** Select a layer within the layer tree. */
		void SetSelection( LayerPtr layer );

		/** Update active state of all layers. */
		void UpdatePythonState();
		void UpdateActiveState();
		void UpdateActiveState(wxTreeItemId subtreeId, wxTreeItemIdValue& cookie);

		void SetIsBlankRaster(bool isBlank);
		bool GetIsBlankRaster();
		bool EraseVectorMap(uint layerId);

	protected:
		/**
		* @brief Traverse all children and set their visibility.
		* @param id Id of top node to traverse from.
		* @param cookie Required to make function reentrant (see wxTreeCtrl documentation).
		* @param visibility Desired state of visibility.
		*/
		void SetVisibilityAllChildren(wxTreeItemId id, wxTreeItemIdValue cookie, bool visibility);

		bool EraseMap(uint layerId);
		
		void EraseTrees(MapLayerPtr mapLayer);
		bool EraseTree(uint layerId);
		void EraseLocationSets(MapLayerPtr mapLayer);
		bool EraseLocationSet(uint layerId);
		void EraseLocations(LocationSetLayerPtr locationSetLayer);
		bool EraseLocation(uint layerId);
		void EraseSequences(LocationLayerPtr locationLayer);
		bool EraseSequence(uint layerId);

	protected:
		/** Layer tree model. */
		LayerTreeModelPtr m_layerTreeModel;

		/** Tree view of layer tree model. */
		LayerTreeViewPtr m_treeView;

		/** Allow access to map layers without having to traverse tree. */
		std::vector<MapLayerPtr> m_maps;

		/** Allow access to vector map layers without having to traverse tree. */
		std::vector<VectorMapLayerPtr> m_vectorMaps;

		/** Allow access to location set layers without having to traverse tree. */
		std::vector<LocationSetLayerPtr> m_locationSets;

		/** Allow access to location layers without having to traverse tree. */
		std::vector<LocationLayerPtr> m_locations;

		/** Allow access to sequence layers without having to traverse tree. */
		std::vector<SequenceLayerPtr> m_sequences;

		/** Allow access to location sets without having to traverse tree. */
		std::vector<TreeLayerPtr> m_trees;

		bool m_isBlankRaster;

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const LayerTreeController* t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int file_version);
	};
}

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const LayerTreeController * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//ar << t->GetLayerTreeModel(); // m_layerTreeModel (LayerTreeModelPtr)
			//ar << t->GetTreeView();       // m_treeView (LayerTreeViewPtr)

			ar << t->m_layerTreeModel; // LayerTreeModelPtr
			ar << t->m_treeView;       // LayerTreeViewPtr
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, LayerTreeController * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			LayerTreeModelPtr _layerTreeModel;
			ar >> _layerTreeModel;

			LayerTreeViewPtr _treeView;
			ar >> _treeView;

			::new(t)LayerTreeController(_layerTreeModel, _treeView);
		}
	}
}

#endif
