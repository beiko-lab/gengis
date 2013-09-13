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

#ifndef _DATA_TYPES_
#define _DATA_TYPES_

// basic data types
typedef unsigned int  uint;
typedef unsigned char byte;
typedef unsigned long ulong;

#include <boost/shared_ptr.hpp>

// smart pointers to GenGIS classes
namespace GenGIS
{
	class   Camera;
	typedef boost::shared_ptr<Camera> CameraPtr;

	class   ChartSetView;
	typedef boost::shared_ptr<ChartSetView> ChartSetViewPtr;

	class   ChartView;
	typedef boost::shared_ptr<ChartView> ChartViewPtr;

	class   Colour;
	typedef boost::shared_ptr<Colour> ColourPtr;

	class   ColourMap;
	typedef boost::shared_ptr<ColourMap> ColourMapPtr;

	class   ColourMapDiscrete;
	typedef boost::shared_ptr<ColourMapDiscrete> ColourMapDiscretePtr;

	class   ColourMapContinuous;
	typedef boost::shared_ptr<ColourMapContinuous> ColourMapContinuousPtr;

	class   ColourMapManager;
	typedef boost::shared_ptr<ColourMapManager> ColourMapManagerPtr;

	class   ColourMapWidget;
	typedef boost::shared_ptr<ColourMapWidget> ColourMapWidgetPtr;

	class   CompassView2D;
	typedef boost::shared_ptr<CompassView2D> CompassView2DPtr;

	class   DropLines;
	typedef boost::shared_ptr<DropLines> DropLinesPtr;

	class   Font;
	typedef boost::shared_ptr<Font> FontPtr;

	class   GeoAxisPolyline;
	typedef boost::shared_ptr<GeoAxisPolyline> GeoAxisPolylinePtr;

	class   GeoTreeEllipticalLayout;
	typedef boost::shared_ptr<GeoTreeEllipticalLayout> GeoTreeEllipticalLayoutPtr;

	class   GeoTreeLinearLayout;
	typedef boost::shared_ptr<GeoTreeLinearLayout> GeoTreeLinearLayoutPtr;

	class   GeoTreeIO;
	typedef boost::shared_ptr<GeoTreeIO> GeoTreeIOPtr;

	class   GeoTreeView;
	typedef boost::shared_ptr<GeoTreeView> GeoTreeViewPtr;

	class   LayerTreeModel;
	typedef boost::shared_ptr<LayerTreeModel> LayerTreeModelPtr;

	class   Layer;
	typedef boost::shared_ptr<Layer> LayerPtr;

	class   LayerTreeController;
	typedef boost::shared_ptr<LayerTreeController> LayerTreeControllerPtr;

	class   LayerTreeView;
	typedef boost::shared_ptr<LayerTreeView> LayerTreeViewPtr;

	class   LayoutEllipse;
	typedef boost::shared_ptr<LayoutEllipse> LayoutEllipsePtr;

	class   LayoutLine;
	typedef boost::shared_ptr<LayoutLine> LayoutLinePtr;

	class   LayoutObjectProperties;
	typedef boost::shared_ptr<LayoutObjectProperties> LayoutObjectPropertiesPtr;

	class   Lighting;
	typedef boost::shared_ptr<Lighting> LightingPtr;

	class   LocationController;
	typedef boost::shared_ptr<LocationController> LocationControllerPtr;

	class   LocationGrid;
	typedef boost::shared_ptr<LocationGrid> LocationGridPtr;

	class	TileModel;
	typedef boost::shared_ptr<TileModel> TileModelPtr;

	class   LocationLayer;
	typedef boost::shared_ptr<LocationLayer> LocationLayerPtr;

	class   LocationModel;
	typedef boost::shared_ptr<LocationModel> LocationModelPtr;

	class   LocationSetLayer;
	typedef boost::shared_ptr<LocationSetLayer> LocationSetLayerPtr;

	class   LocationSetController;
	typedef boost::shared_ptr<LocationSetController> LocationSetControllerPtr;

	class   LocationView;
	typedef boost::shared_ptr<LocationView> LocationViewPtr;
	
	class   MainWindowWrapper;
	typedef boost::shared_ptr<MainWindowWrapper> MainWindowWrapperPtr;

	class   MapLayer;
	typedef boost::shared_ptr<MapLayer> MapLayerPtr;

	class   MiniMapView2D;
	typedef boost::shared_ptr<MiniMapView2D> MiniMapView2DPtr;

	class   NavView2D;
	typedef boost::shared_ptr<NavView2D> NavView2DPtr;

	class   Node;
	typedef boost::shared_ptr<Node> NodePtr;

	class   NodeGeoTree;
	typedef boost::shared_ptr<NodeGeoTree> NodeGeoTreePtr;

	class   NodePhylo;
	typedef boost::shared_ptr<NodePhylo> NodePhyloPtr;

	class   Point3D;
	typedef boost::shared_ptr<Point3D> Point3DPtr;
	typedef boost::shared_array<Point3D> Point3DArrayPtr;

	class   ProgressDlg;
	typedef boost::shared_ptr<ProgressDlg> ProgressDlgPtr;

	class   WaitingDlg;
	typedef boost::shared_ptr<WaitingDlg> WaitingDlgPtr;

	class   SequenceController;
	typedef boost::shared_ptr<SequenceController> SequenceControllerPtr;
	
	class   SequenceLayer;
	typedef boost::shared_ptr<SequenceLayer> SequenceLayerPtr;

	class   SequenceModel;
	typedef boost::shared_ptr<SequenceModel> SequenceModelPtr;

	class   ShapeMap;
	typedef boost::shared_ptr<ShapeMap> ShapeMapPtr;

	class   ShapeMapManager;
	typedef boost::shared_ptr<ShapeMapManager> ShapeMapManagerPtr;

	class   ShapeMapWidget;
	typedef boost::shared_ptr<ShapeMapWidget> ShapeMapWidgetPtr;

	class   StudyController;
	typedef boost::shared_ptr<StudyController> StudyControllerPtr;

	class   StudyLayer;
	typedef boost::shared_ptr<StudyLayer> StudyLayerPtr;

	class   StudyModel;
	typedef boost::shared_ptr<StudyModel> StudyModelPtr;

	typedef boost::shared_ptr<OGRCoordinateTransformation> ProjectionToolPtr;

	class   MapController;
	typedef boost::shared_ptr<MapController> MapControllerPtr;

	class   TerrainMapWidget;
	typedef boost::shared_ptr<TerrainMapWidget> TerrainMapWidgetPtr;

	class   MapModel;
	typedef boost::shared_ptr<MapModel> MapModelPtr;

	class   MapTexture;
	typedef boost::shared_ptr<MapTexture> MapTexturePtr;

	class   MapView;
	typedef boost::shared_ptr<MapView> MapViewPtr;

	class   Texture;
	typedef boost::shared_ptr<Texture> TexturePtr;

	class   TreeLayer;
	typedef boost::shared_ptr<TreeLayer> TreeLayerPtr;

	class   UserElementsWrapper;
	typedef boost::shared_ptr<UserElementsWrapper> UserElementsWrapperPtr;

	class   VectorMapController;
	typedef boost::shared_ptr<VectorMapController> VectorMapControllerPtr;

	class   VectorMapLayer;
	typedef boost::shared_ptr<VectorMapLayer> VectorMapLayerPtr;

	class   VectorMapModel;
	typedef boost::shared_ptr<VectorMapModel> VectorMapModelPtr;

	class   VectorMapView;
	typedef boost::shared_ptr<VectorMapView> VectorMapViewPtr;

	class   View;
	typedef boost::shared_ptr<View> ViewPtr;

	class   Viewport;
	typedef Viewport* ViewportPtr; // smart pointer are not used here since wxWidgets objects
                                   // handle deallocation internally

	class   ViewportWrapper;
	typedef boost::shared_ptr<ViewportWrapper> ViewportWrapperPtr;
	
	class   ViewWidget;
	typedef boost::shared_ptr<ViewWidget> ViewWidgetPtr;

	class   VisualLabel;
	typedef boost::shared_ptr<VisualLabel> VisualLabelPtr;

	class   VisualLine;
	typedef boost::shared_ptr<VisualLine> VisualLinePtr;

	class   VisualLines;
	typedef boost::shared_ptr<VisualLines> VisualLinesPtr;

	class   VisualPoints;
	typedef boost::shared_ptr<VisualPoints> VisualPointsPtr;

	class   VisualMarker;
	typedef boost::shared_ptr<VisualMarker> VisualMarkerPtr;

	class   VisualPolyLine;
	typedef boost::shared_ptr<VisualPolyLine> VisualPolyLinePtr;

	class   WidgetController;
	typedef boost::shared_ptr<WidgetController> WidgetControllerPtr;
}

#endif
