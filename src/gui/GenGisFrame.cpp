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

#include "../gui/GenGisFrame.hpp"

#include "../core/App.hpp"
#include "../core/MapView.hpp"
#include "../core/MapTexture.hpp"
#include "../core/GeoTreeView.hpp"
#include "../core/Camera.hpp"
#include "../core/LayerTreeView.hpp"
#include "../core/GeoTreeIO.hpp"
#include "../core/LocationSetIO.hpp"
#include "../core/SequenceIO.hpp"
#include "../core/SequenceController.hpp"
#include "../core/LayerTreeModel.hpp"
#include "../core/LayoutLine.hpp"
#include "../core/LayoutEllipse.hpp"
#include "../core/GeoAxisPolyline.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/MapController.hpp"
#include "../core/VectorMapController.hpp"
#include "../core/PopupMenu.hpp"
#include "../core/Viewport.hpp"
#include "../core/LayerTreeItem.hpp"
#include "../core/Layer.hpp"
#include "../core/StudyLayer.hpp"
#include "../core/MapLayer.hpp"
#include "../core/VectorMapLayer.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/LocationSetController.hpp"
#include "../core/TreeLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/VisualLabel.hpp"
#include "../core/UserElements.hpp"
#include "../core/LabelController.hpp"
#include "../core/Lighting.hpp"

#include "../gui/AboutDlg.hpp"
#include "../gui/ConfigureDatabaseDlg.hpp"
#include "../gui/DbSelectSampleDlg.hpp"
#include "../gui/FontPropertiesDlg.hpp"
#include "../gui/GenGisFrame.hpp"
#include "../gui/LabelToolDlg.hpp"
#include "../gui/LayoutObjectPropertiesDlg.hpp"
#include "../gui/LinearAnalysisPlotDlg.hpp"
#include "../gui/NonlinearAxisTableDlg.hpp"
#include "../gui/LightingPropertiesDlg.hpp"
#include "../gui/LocationMergeDlg.hpp"
#include "../gui/LocationPropertiesDlg.hpp"
#include "../gui/LocationSetPropertiesDlg.hpp"
#include "../gui/MapPropertiesDlg.hpp"
#include "../gui/VectorMapPropertiesDlg.hpp"
#include "../gui/MonteCarloPlotDlg.hpp"
#include "../gui/ProgressDlg.hpp"
#include "../gui/SaveImageDlg.hpp"
#include "../gui/SeqSelectionDlg.hpp"
#include "../gui/SequencePropertiesDlg.hpp"
#include "../gui/StudyPropertiesDlg.hpp"
#include "../gui/TreePropertiesDlg.hpp"
#include "../gui/UpdateGenGISDlg.hpp"
#include "../gui/WaitingDlg.hpp"

#include "../utils/StringTools.hpp"
#include "../utils/UniqueId.hpp"
#include "../utils/ColourMap.hpp"
#include "../utils/ColourMapDiscrete.hpp"
#include "../utils/ColourMapManager.hpp"
#include "../utils/DemReader.hpp"
#include "../utils/ShpReader.hpp"
#include "../utils/Log.hpp"

#include "../widgets/CustomColourButton.hpp"
#include "../widgets/WidgetController.hpp"

// Don't use this code in Windows debug builds!
#ifndef _DEBUG
#include "../python/PythonInterface.hpp"
#include "../core/PluginManager.hpp"
#endif

using namespace GenGIS;

const int ID_MNU_LAYER_STUDY_PROPERTIES                     = 5000;
const int ID_MNU_LAYER_MAP_PROPERTIES                       = 5001;
const int ID_MNU_LAYER_VECTOR_MAP_PROPERTIES                = 5007;

const int ID_MNU_LAYER_LOCATION_SET_PROPERTIES              = 5002;
const int ID_MNU_LAYER_LOCATION_PROPERTIES                  = 5003;
const int ID_MNU_LAYER_TREE_PROPERTIES                      = 5004;
const int ID_MNU_LAYER_SEQUENCE_PROPERTIES                  = 5005;

const int ID_RIGHT_CLICK_CLADOGRAM_3D                       = 5100;
const int ID_RIGHT_CLICK_SLANTED_CLADOGRAM_3D               = 5101;
const int ID_RIGHT_CLICK_CLADOGRAM_2D                       = 5102;
const int ID_RIGHT_CLICK_SLANTED_PHYLOGRAM_3D               = 5103;
const int ID_RIGHT_CLICK_PHYLOGRAM_2D                       = 5104;
const int ID_RIGHT_CLICK_SLANTED_CLADOGRAM_2D               = 5105;
const int ID_RIGHT_CLICK_LAYOUT_ELLIPSE_QUADRANT            = 5106;
const int ID_RIGHT_CLICK_TREE_LAYOUT_LINE                   = 5107;
const int ID_RIGHT_CLICK_TREE_LAYOUT_ELLIPSE                = 5108;
const int ID_RIGHT_CLICK_TREE_LEAF_NODE_LABELS              = 5109;
const int ID_RIGHT_CLICK_TREE_SIGNIFICANCE_TEST             = 5110;
const int ID_RIGHT_CLICK_GEO_AXIS_TO_TREE                   = 5111;
const int ID_RIGHT_CLICK_GEO_AXIS_REVERSE_ORDER             = 5112;
const int ID_RIGHT_CLICK_TREE_GEO_AXIS                      = 5113;
const int ID_RIGHT_CLICK_LINEAR_AXES_ANALYSIS               = 5114;
const int ID_RIGHT_CLICK_TREE_FLIP                          = 5115;
const int ID_RIGHT_CLICK_SHOW_GEOGRAPHIC_AXIS_TABLE			= 5116;

const int ID_RIGHT_CLICK_LAYOUT_CHARTS_EVENLY_ON_LINE       = 5201;
const int ID_RIGHT_CLICK_LAYOUT_CHARTS_LINE_PROJECTED       = 5202;
const int ID_RIGHT_CLICK_LAYOUT_CHARTS_ELLIPSE_QUADRANT     = 5203;
const int ID_RIGHT_CLICK_LAYOUT_CHARTS_EVENLY_ON_ELLIPSE    = 5204;
const int ID_RIGHT_CLICK_LAYOUT_CHARTS_ELLIPSE_PROJECTED    = 5205;
const int ID_RIGHT_CLICK_CHART_SET_LAYOUT_LINE              = 5206;
const int ID_RIGHT_CLICK_CHART_SET_LAYOUT_ELLIPSE           = 5207;
const int ID_RIGHT_CLICK_LAYOUT_CHARTS_DEFAULT_POS          = 5208;
const int ID_RIGHT_CLICK_LAYOUT_CHARTS_TREE_LEAVES          = 5209;

const int ID_MNU_LAYOUT_LINE_DELETE                         = 6000;
const int ID_MNU_LAYOUT_ELLIPSE_DELETE                      = 6001;
const int ID_MNU_GEO_AXIS_POLYLINE_DELETE                   = 6002;

// Popup menu IDs are in the 7000's

const int ID_MNU_FILE_RECENT_SESSION_1                      = 8001;
const int ID_MNU_FILE_RECENT_SESSION_2                      = 8002;
const int ID_MNU_FILE_RECENT_SESSION_3                      = 8003;
const int ID_MNU_FILE_RECENT_SESSION_4                      = 8004;
const int ID_MNU_FILE_RECENT_SESSION_5                      = 8005;
const int ID_MNU_FILE_RECENT_SESSION_6                      = 8006;
const int ID_MNU_FILE_RECENT_SESSION_7                      = 8007;
const int ID_MNU_FILE_RECENT_SESSION_8                      = 8008;
const int ID_MNU_FILE_RECENT_SESSION_9                      = 8009;

GenGisFrame::GenGisFrame(wxWindow* parent): GenGisLayout(parent), m_toolbarTextStatus( true ),m_toolbarFullScreenStatus(false),
m_viewCompassStatus(true), m_viewMinimapStatus(true), m_viewNavStatus(true), m_showWelcomeDialog( true )
{
	Log::Inst().SetConsole(m_console);

	App::Inst().SetViewport( new Viewport( m_pnlViewport, ID_VIEWPORT, wxPoint(0,0), wxSize(0,0) ) );

	// setup sizers so viewport automatically expand to fit panel
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	sizer->Add(App::Inst().GetViewport(), 1, wxEXPAND);
	m_pnlViewport->SetSizer(sizer);

	// initialize layer tree model, view, and controller
	LayerTreeModelPtr layerTreeModel(new LayerTreeModel());
	LayerTreeViewPtr layerTreeView(new LayerTreeView(layerTreeModel, m_treeCtrlLayer));
	LayerTreeControllerPtr layerTreeController(new LayerTreeController(layerTreeModel, layerTreeView));
	App::Inst().SetLayerTreeController(layerTreeController);

	// select the first item (study) in the layer tree
	wxTreeItemId treeId = App::Inst().GetLayerTreeController()->GetTreeCtrl()->GetRootItem();
	wxTreeItemIdValue cookie;
	wxTreeItemId studyId = App::Inst().GetLayerTreeController()->GetTreeCtrl()->GetFirstChild(treeId, cookie);
	App::Inst().GetLayerTreeController()->GetTreeCtrl()->SelectItem(studyId);

	// set window title
	#ifndef _DEBUG
	SetTitle( wxT( "New Document - GenGIS" ) );
	#else
	SetTitle( wxT( "New Document - GenGIS [DEBUG Mode]" ) );
	#endif

	// attach viewport mouse and keyboard events to local functions
	App::Inst().GetViewport()->SignalRender(boost::bind(&GenGisFrame::SigRender, this));
	App::Inst().GetViewport()->SignalMouse(boost::bind(&GenGisFrame::SigViewportMouse, this, _1));
	App::Inst().GetViewport()->SignalMouseWheel(boost::bind(&GenGisFrame::SigViewportMouseWheel, this, _1));
	App::Inst().GetViewport()->SignalKeyboard(boost::bind(&GenGisFrame::SigViewportKeyboard, this, _1));

	// initialize OpenGL widget controller
	m_widgetController.reset(new WidgetController(App::Inst().GetViewport()));

	// seed random number generator
	srand((unsigned)time(0));

	ConnectEvents();
}

GenGisFrame::~GenGisFrame()
{
	DisconnectEvents();
}

template<class Archive>
void GenGisFrame::GenGisFrameSerialize(Archive & ar, const unsigned int file_version)
{
	//ar & m_lastMousePos;      // Point2D
	//ar & m_horSplitterPos;    // int
	//ar & m_verSplitterPos;    // int
	//ar & m_toolbarTextStatus; // bool
	//ar & m_showWelcomeDialog; // bool
	//ar & m_recentSessions;    // std::vector<std::string>
	ar & m_layoutLine;        // LayoutLinePtr
	ar & m_layoutEllipse;     // LayoutEllipsePtr
	ar & m_geoAxisPolyline;   // GeoAxisPolylinePtr
	//ar & m_widgetController;  // WidgetControllerPtr
	ar & m_studyLayer;        // StudyLayerPtr
	ar & m_mapLayer;          // MapLayerPtr
	ar & m_vectorMapLayer;          // MapLayerPtr
	ar & m_locationSetLayer;  // LocationSetLayerPtr
	ar & m_locationLayer;     // LocationLayerPtr
	ar & m_sequenceLayer;     // SequenceLayerPtr
	ar & m_treeLayer;         // TreeLayerPtr;
}
template void GenGisFrame::GenGisFrameSerialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int file_version); 
template void GenGisFrame::GenGisFrameSerialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int file_version);

void GenGisFrame::ConnectEvents()
{
	this->Connect( ID_MNU_LAYER_STUDY_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnStudyProperties ) );
	this->Connect( ID_MNU_LAYER_MAP_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnMapProperties ) );
	this->Connect( ID_MNU_LAYER_VECTOR_MAP_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnVectorMapProperties ) );
	this->Connect( ID_MNU_LAYER_LOCATION_SET_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLocationSetProperties ) );
	this->Connect( ID_MNU_LAYER_LOCATION_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLocationProperties ) );
	this->Connect( ID_MNU_LAYER_SEQUENCE_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSequenceProperties ) );
	this->Connect( ID_MNU_LAYER_TREE_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeProperties ) );

	this->Connect( ID_RIGHT_CLICK_CLADOGRAM_3D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnCladogram3D ) );
	this->Connect( ID_RIGHT_CLICK_SLANTED_CLADOGRAM_3D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSlantedCladogram3D ) );
	this->Connect( ID_RIGHT_CLICK_SLANTED_PHYLOGRAM_3D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSlantedPhylogram3D ) );
	this->Connect( ID_RIGHT_CLICK_CLADOGRAM_2D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnCladogram2D ) );
	this->Connect( ID_RIGHT_CLICK_PHYLOGRAM_2D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnPhylogram2D ) );
	this->Connect( ID_RIGHT_CLICK_SLANTED_CLADOGRAM_2D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSlantedCladogram2D ) );
	this->Connect( ID_RIGHT_CLICK_LAYOUT_ELLIPSE_QUADRANT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutTreeEllipseQuadrant) );
	this->Connect( ID_RIGHT_CLICK_TREE_LAYOUT_LINE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeLayoutLineVisibility ) );
	this->Connect( ID_RIGHT_CLICK_TREE_LAYOUT_ELLIPSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeLayoutEllipseVisibility ) );
	this->Connect( ID_RIGHT_CLICK_TREE_LEAF_NODE_LABELS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLeafNodeLabels ) );
	this->Connect( ID_RIGHT_CLICK_TREE_SIGNIFICANCE_TEST, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeSignificanceTest ) );
	this->Connect( ID_RIGHT_CLICK_GEO_AXIS_TO_TREE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnAssignGeoAxisToTree ) );
	this->Connect( ID_RIGHT_CLICK_GEO_AXIS_REVERSE_ORDER, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnReverseGeoAxisOrder ) );	
	this->Connect( ID_RIGHT_CLICK_TREE_GEO_AXIS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeGeographicAxisVisibility ) );
	this->Connect( ID_RIGHT_CLICK_LINEAR_AXES_ANALYSIS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnPerformLinearAxesAnalysis ) );
	this->Connect( ID_RIGHT_CLICK_TREE_FLIP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeFlip ) );
	this->Connect( ID_RIGHT_CLICK_SHOW_GEOGRAPHIC_AXIS_TABLE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnShowNonlinearAxisTable ) );

	

	this->Connect( ID_RIGHT_CLICK_LAYOUT_CHARTS_EVENLY_ON_LINE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutChartsEvenlyOnLine ) );
	this->Connect( ID_RIGHT_CLICK_LAYOUT_CHARTS_LINE_PROJECTED, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutChartsLineProjected ) );
	this->Connect( ID_RIGHT_CLICK_LAYOUT_CHARTS_ELLIPSE_QUADRANT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutChartsEllipseQuadrant ) );
	this->Connect( ID_RIGHT_CLICK_LAYOUT_CHARTS_EVENLY_ON_ELLIPSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutChartsEvenlyOnEllipse ) );
	this->Connect( ID_RIGHT_CLICK_LAYOUT_CHARTS_ELLIPSE_PROJECTED, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutChartsEllipseProjected) );
	this->Connect( ID_RIGHT_CLICK_LAYOUT_CHARTS_DEFAULT_POS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutChartsDefaultPosition ) );
	this->Connect( ID_RIGHT_CLICK_LAYOUT_CHARTS_TREE_LEAVES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutChartsOnTreeLeaves ) );
	this->Connect( ID_RIGHT_CLICK_CHART_SET_LAYOUT_LINE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnChartSetLayoutLine ) );
	this->Connect( ID_RIGHT_CLICK_CHART_SET_LAYOUT_ELLIPSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnChartSetLayoutEllipse ) );

	this->Connect( ID_MNU_LAYOUT_LINE_DELETE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutLineDelete ) );
	this->Connect( ID_MNU_LAYOUT_ELLIPSE_DELETE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutEllipseDelete ) );
	this->Connect( ID_MNU_GEO_AXIS_POLYLINE_DELETE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnGeoAxisPolylineDelete ) );
	this->Connect( ID_POPUP_MNU_HIDE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSelectedViewHide ) );
	this->Connect( ID_POPUP_MNU_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSelectedViewProperties ) );
	this->Connect( ID_POPUP_MNU_REMOVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSelectedViewRemove ) );

	this->Connect( ID_POPUP_MNU_SUBTREE_SIGNIFICANCE_TEST, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeSignificanceTestPopup ) );
	this->Connect( ID_POPUP_MNU_EXTEND_POLYLINE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnExtendGeographicPolyline ) );
	this->Connect( ID_POPUP_MNU_LINEAR_AXES_ANALYSIS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnPerformLinearAxesAnalysisPopup ) );
	this->Connect( ID_POPUP_MNU_SHOW_GEOGRAPHIC_AXIS_TABLE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnShowNonLinearAxisTablePopup ) );
}

void GenGisFrame::DisconnectEvents()
{
	this->Disconnect( ID_MNU_LAYER_STUDY_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnStudyProperties ) );
	this->Disconnect( ID_MNU_LAYER_MAP_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnMapProperties ) );
	this->Disconnect( ID_MNU_LAYER_VECTOR_MAP_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnVectorMapProperties ) );

	this->Disconnect( ID_MNU_LAYER_LOCATION_SET_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLocationSetProperties ) );
	this->Disconnect( ID_MNU_LAYER_LOCATION_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLocationProperties ) );
	this->Disconnect( ID_MNU_LAYER_SEQUENCE_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSequenceProperties ) );
	this->Disconnect( ID_MNU_LAYER_TREE_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeProperties ) );

	this->Disconnect( ID_RIGHT_CLICK_CLADOGRAM_3D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnCladogram3D ) );
	this->Disconnect( ID_RIGHT_CLICK_SLANTED_CLADOGRAM_3D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSlantedCladogram3D ) );
	this->Disconnect( ID_RIGHT_CLICK_SLANTED_PHYLOGRAM_3D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSlantedPhylogram3D ) );
	this->Disconnect( ID_RIGHT_CLICK_CLADOGRAM_2D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnCladogram2D ) );
	this->Disconnect( ID_RIGHT_CLICK_PHYLOGRAM_2D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnPhylogram2D ) );
	this->Disconnect( ID_RIGHT_CLICK_SLANTED_CLADOGRAM_2D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSlantedCladogram2D ) );
	this->Disconnect( ID_RIGHT_CLICK_LAYOUT_ELLIPSE_QUADRANT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutTreeEllipseQuadrant) );
	this->Disconnect( ID_RIGHT_CLICK_TREE_LAYOUT_LINE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeLayoutLineVisibility ) );
	this->Disconnect( ID_RIGHT_CLICK_TREE_LAYOUT_ELLIPSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeLayoutEllipseVisibility ) );
	this->Disconnect( ID_RIGHT_CLICK_TREE_LEAF_NODE_LABELS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLeafNodeLabels ) );
	this->Disconnect( ID_RIGHT_CLICK_TREE_SIGNIFICANCE_TEST, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeSignificanceTest ) );
	this->Disconnect( ID_RIGHT_CLICK_GEO_AXIS_TO_TREE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnAssignGeoAxisToTree ) );
	this->Disconnect( ID_RIGHT_CLICK_GEO_AXIS_REVERSE_ORDER, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnReverseGeoAxisOrder ) );	
	this->Disconnect( ID_RIGHT_CLICK_TREE_GEO_AXIS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeGeographicAxisVisibility ) );
	this->Disconnect( ID_RIGHT_CLICK_LINEAR_AXES_ANALYSIS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnPerformLinearAxesAnalysis ) );
	this->Disconnect( ID_RIGHT_CLICK_TREE_FLIP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeFlip ) );
	this->Disconnect( ID_RIGHT_CLICK_SHOW_GEOGRAPHIC_AXIS_TABLE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnShowNonlinearAxisTable ) );

	this->Disconnect( ID_RIGHT_CLICK_LAYOUT_CHARTS_EVENLY_ON_LINE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutChartsEvenlyOnLine ) );
	this->Disconnect( ID_RIGHT_CLICK_LAYOUT_CHARTS_LINE_PROJECTED, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutChartsLineProjected ) );
	this->Disconnect( ID_RIGHT_CLICK_LAYOUT_CHARTS_ELLIPSE_QUADRANT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutChartsEllipseQuadrant ) );	
	this->Disconnect( ID_RIGHT_CLICK_LAYOUT_CHARTS_EVENLY_ON_ELLIPSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutChartsEvenlyOnEllipse ) );
	this->Disconnect( ID_RIGHT_CLICK_LAYOUT_CHARTS_ELLIPSE_PROJECTED, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutChartsEllipseProjected) );
	this->Disconnect( ID_RIGHT_CLICK_CHART_SET_LAYOUT_LINE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnChartSetLayoutLine ) );
	this->Disconnect( ID_RIGHT_CLICK_CHART_SET_LAYOUT_ELLIPSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnChartSetLayoutEllipse ) );

	this->Disconnect( ID_MNU_LAYOUT_LINE_DELETE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutLineDelete ) );
	this->Disconnect( ID_MNU_LAYOUT_ELLIPSE_DELETE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnLayoutEllipseDelete ) );
	this->Disconnect( ID_MNU_GEO_AXIS_POLYLINE_DELETE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnGeoAxisPolylineDelete ) );
	this->Disconnect( ID_POPUP_MNU_HIDE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSelectedViewHide ) );
	this->Disconnect( ID_POPUP_MNU_PROPERTIES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSelectedViewProperties ) );
	this->Disconnect( ID_POPUP_MNU_REMOVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnSelectedViewRemove ) );

	this->Disconnect( ID_POPUP_MNU_SUBTREE_SIGNIFICANCE_TEST, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnTreeSignificanceTestPopup ) );
	this->Disconnect( ID_POPUP_MNU_EXTEND_POLYLINE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnExtendGeographicPolyline ) );
	this->Disconnect( ID_POPUP_MNU_LINEAR_AXES_ANALYSIS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnPerformLinearAxesAnalysisPopup ) );
	this->Disconnect( ID_POPUP_MNU_SHOW_GEOGRAPHIC_AXIS_TABLE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnShowNonLinearAxisTablePopup ) );
}

void GenGisFrame::AddConsolePanel(wxPanel* panel, wxString name)
{
	m_bottomNotebook->AddPage(panel, name, false);
}

wxNotebook* GenGisFrame::GetBottomNotebook()
{
	return m_bottomNotebook;
}

void GenGisFrame::FillSequenceLegend()
{
	LayerTreeControllerPtr layerTreeController = App::Inst().GetLayerTreeController();

	std::map<std::wstring, Colour> seenNames;
	// for all LocationSetLayers...
	for(uint eachLayer = 0; eachLayer < layerTreeController->GetNumLocationSetLayers(); eachLayer++)
	{
		LocationSetLayerPtr locationSetLayer = layerTreeController->GetLocationSetLayer(eachLayer);
		// for all LocationLayers...
		for(uint eachLocation = 0; eachLocation < locationSetLayer->GetNumLocationLayers(); eachLocation++)
		{
			ChartViewPtr chartView = locationSetLayer->GetLocationLayer(eachLocation)->GetLocationController()->GetChartView();
			ColourMapDiscretePtr colourMap = chartView->GetColourMap();
			std::vector<std::wstring> chartViewNames = chartView->GetNames();
			std::vector<std::wstring>::iterator it;
			// for each ChartView's name...
			for(it = chartViewNames.begin(); it != chartViewNames.end(); ++it)
			{
				std::wstring wstr = *it;
				if(seenNames.count(wstr) > 0)
				{
					continue;
				}

				Colour colour;
				colourMap->GetColour(wstr, colour);
				float assignToOther = chartView->GetAssignToOther();
				if(chartView->GetPercentage(wstr) > chartView->GetAssignToOther()/100)
					seenNames.insert(make_pair(wstr, colour));
			}

			if(chartViewNames.size() != 0)
			{
				std::wstring otherStr = _T("Other");
				Colour colour;
				colourMap->GetColour(otherStr, colour);			
				seenNames.insert(make_pair(otherStr, colour));	
			}
		}
	}

	m_legendSequenceSizer->GetContainingWindow()->Freeze();
	m_legendSequenceSizer->Clear(true);
	std::map<std::wstring,Colour>::iterator it;
	for(it = seenNames.begin(); it != seenNames.end(); ++it)
	{
		CustomColourButton* customColourButton = new CustomColourButton(
			m_legendSequence, NULL, wxColour( (it->second).GetRedInt(), (it->second).GetGreenInt(), (it->second).GetBlueInt()) );
		customColourButton->SetSequenceText( (it->first).c_str() );
		customColourButton->Connect( wxEVT_LEFT_DOWN,
			wxMouseEventHandler( GenGisFrame::OnCustomColourButtonClicked ), NULL, this );

		m_legendSequenceSizer->Add( customColourButton, 0, wxALL, 5 );

		wxStaticText* tempStaticText = new wxStaticText(m_legendSequence, wxID_ANY, wxString((it->first).c_str()), wxDefaultPosition, wxDefaultSize, 0);
		tempStaticText->Wrap(-1);
		m_legendSequenceSizer->Add(tempStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	}

	m_legendSequenceSizer->GetContainingWindow()->Thaw();
	// Redraw the flexGridSizer
	m_legendSequenceSizer->Layout();
	// Force the scrollbars to reset
	m_legendSequenceSizer->FitInside(m_legendSequenceSizer->GetContainingWindow());
}

void GenGisFrame::OnCustomColourButtonClicked( wxMouseEvent& event )
{
	// Retrieve the CustomColourButton object that was clicked
	CustomColourButton* customColourButton = ( CustomColourButton* )event.GetEventObject();

	// Create and open the colour picker dialog
	wxColourData *colourData = new wxColourData();
	colourData->SetColour( customColourButton->GetBackgroundColour() );
	wxColourDialog *colourPicker = new wxColourDialog( this, colourData );
	if ( colourPicker->ShowModal() == wxID_CANCEL ) return;

	// Set the CustomColourButton background colour (used for display under Mac/Unix releases)
	customColourButton->SetBackgroundColour( colourPicker->GetColourData().GetColour() );
	customColourButton->Refresh();

	// Fetch the colour map 
	ColourMapDiscretePtr colourMap = App::Inst().GetLayerTreeController()->GetLocationSetLayer(0)
                                     ->GetLocationLayer(0)->GetLocationController()->GetChartView()->GetColourMap();

	// Set the new colour within the colour map
	colourMap->SetColour( customColourButton->GetSequenceText(), Colour( customColourButton->GetBackgroundColour() ) );
	
	// Refresh the Viewport
	App::Inst().GetViewport()->Refresh( false );
}

void GenGisFrame::OnSamplesLegendPaint(wxPaintEvent& event)
{
	wxGLCanvas* shape = (wxGLCanvas*)event.GetEventObject();
	int id = event.GetId();

	shape->SetCurrent();
	wxPaintDC dc(shape);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, shape->GetSize().x, 0.0, shape->GetSize().y, 1, 100 );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uint x = shape->GetSize().x;
	uint y = shape->GetSize().y;
	glViewport(0, 0, (GLint)shape->GetSize().x, (GLint)shape->GetSize().y);

	bool bDone = false;
	for(uint eachLocationSet = 0; eachLocationSet < App::Inst().GetLayerTreeController()->GetNumLocationSetLayers(); eachLocationSet++)
	{
		LocationSetLayerPtr locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer(eachLocationSet);

		for(uint eachLocation = 0; eachLocation < locationSetLayer->GetNumLocationLayers(); eachLocation++)
		{
			LocationViewPtr locationView = locationSetLayer->GetLocationLayer(eachLocation)->GetLocationController()->GetLocationView();
			if(locationView->GetId() == id)
			{
				Colour colour = locationView->GetColour();

				glTranslated(x/2, y/2, -10);
				VisualMarker::RenderMarker(locationView->GetSize(), colour, locationView->GetShape());

				bDone = true;
				break;
			}
		}

		if(bDone)
			break;
	}

	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );

	shape->SwapBuffers();
}

void GenGisFrame::OnSamplesLegendClick( wxMouseEvent& event )
{
	if ( m_locationSetLayer == NULL ) return;

	if ( m_locationSetLayer->HasPropertiesDialogOpen() )
	{
		m_locationSetLayer->GetPropertiesDialog()->SetFocus();
		m_locationSetLayer->GetPropertiesDialog()->Raise();
	}
	else
	{
		LocationSetPropertiesDlg* dlg = new LocationSetPropertiesDlg( App::Inst().GetMainWindow(), m_locationSetLayer, event.GetId() );
		m_locationSetLayer->SetPropertiesDialogOpenStatus( true );
		dlg->Show();
	}
}

void GenGisFrame::OnSamplesLegendLocationSetSelect( wxCommandEvent& event )
{
	// Get currently selected location set from the choice box
	m_locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer( event.GetSelection() );

	// Return if invalid selection
	if ( m_locationSetLayer == NULL ) return;

	// Generate the samples legend
	GenerateSamplesLegend();
}

// Fills the Samples legend (on the sidebar) with data.
void GenGisFrame::FillSamplesLegend()
{
	// Clear the choice box and other controls
	m_locationSetChoice->Clear();
	m_legendLocationsSizerColour2->Clear(true);
	m_legendLocationsSizerShape2->Clear(true);
	m_legendLocationsSizerSize2->Clear(true);

	// Refresh interface
	m_legendLocations->FitInside();
	m_legendLocations->Refresh( false );

	if ( m_locationSetLayer == NULL )
	{
		// Set location set pointer to first location set (if available)
		if ( App::Inst().GetLayerTreeController()->GetNumLocationSetLayers() > 0 )
		{
			m_locationSetLayer = App::Inst().GetLayerTreeController()->GetLocationSetLayer( 0 );
		}
		else
		{
			m_locationSetChoice->Append( wxT( "<No Location Set Loaded>" ) );
			m_locationSetChoice->SetStringSelection( wxT( "<No Location Set Loaded>" ) );
			return;
		}
	}

	LayerTreeControllerPtr layerTree = App::Inst().GetLayerTreeController();

	// Generate new choices
	for (uint locSet = 0; locSet < layerTree->GetNumLocationSetLayers(); locSet++)
	{
		m_locationSetChoice->Append( wxString( layerTree->GetLocationSetLayer( locSet )->GetName().c_str() ) );
	}

	// Select currently active location set within choice box
	m_locationSetChoice->SetStringSelection( wxString(m_locationSetLayer->GetName().c_str() ) );

	// Generate the samples legend
	GenerateSamplesLegend();
}

void GenGisFrame::GenerateSamplesLegend()
{
	// Set properties for the Colour box

	if ( m_legendLocationsSizerColour2 != NULL)
		m_legendLocationsSizerColour2->Clear(true);

	LocationSetControllerPtr locationSetController = m_locationSetLayer->GetLocationSetController();
	
	if ( locationSetController->ModifiedColour() == true )
	{
		// Set label for static box
		m_legendLocationsSizerColour->GetStaticBox()->SetLabel( wxT( "Colour" ) );

		// Insert text into legend
		wxStaticText* id = new wxStaticText( m_legendLocations, 0,
			wxT( "<Individually Set>" ), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
		m_legendLocationsSizerColour2->Add(id, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

		id->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GenGisFrame::OnSamplesLegendClick ), NULL, this );
	}
	// If using a uniform colour for all locations
	else if ( locationSetController->GetUniformColourFlag() == true )
	{
		// Set label for static box
		m_legendLocationsSizerColour->GetStaticBox()->SetLabel( wxT( "Colour (Uniform)" ) );

		// Load uniform colour and insert into legend
		Colour uniformColour = locationSetController->GetUniformColour();
		CustomColourButton* customColourButton = new CustomColourButton(
				m_legendLocations, NULL, wxColour( uniformColour.GetRedInt(), uniformColour.GetGreenInt(), uniformColour.GetBlueInt()) );
		customColourButton->SetId(0);
		m_legendLocationsSizerColour2->Add( customColourButton, 0, wxALL, 5 );

		customColourButton->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GenGisFrame::OnSamplesLegendClick ), NULL, this );
		
		// Insert text into legend
		wxStaticText* id = new wxStaticText( m_legendLocations, wxID_ANY,
			wxT( "All locations" ), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
		m_legendLocationsSizerColour2->Add(id, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	}
	else
	{
		// Set label for static box
		wxString staticBoxTitle = wxT( "Colour (" ) + wxString( locationSetController->GetColourField().c_str(), wxConvISO8859_1 ) + wxT( ")" );
		m_legendLocationsSizerColour->GetStaticBox()->SetLabel( staticBoxTitle );

		// Retrieve the current set of field values
		std::vector<std::wstring> fieldValues;
		locationSetController->GetSortedFieldValues( locationSetController->GetColourField(), fieldValues );

		// Retrieve the current set of colour map values
		std::map<std::wstring, Colour> colourMapValues = locationSetController->GetColourMap()->GetNameToColourMap();

		// Iterate through the field values
		std::vector<std::wstring>::iterator it;
		for ( it = fieldValues.begin(); it < fieldValues.end(); it++ )
		{
			// Load colour and insert into legend
			CustomColourButton* customColourButton = new CustomColourButton(
				m_legendLocations, NULL, wxColour( colourMapValues[*it].GetRedInt(), colourMapValues[*it].GetGreenInt(), colourMapValues[*it].GetBlueInt()) );
			m_legendLocationsSizerColour2->Add( customColourButton, 0, wxALL, 5 );

			customColourButton->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GenGisFrame::OnSamplesLegendClick ), NULL, this );

			// Load colour description and insert into legend
			wxStaticText* id = new wxStaticText( m_legendLocations, 0,
				wxString( (*it).c_str(), wxConvISO8859_1 ), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
			m_legendLocationsSizerColour2->Add(id, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
		}
	}

	// Set properties for the Shape box

	if ( m_legendLocationsSizerShape2 != NULL)
		m_legendLocationsSizerShape2->Clear(true);

	std::string filenames[] = { "circle.xpm", "circle_fast.xpm", "square.xpm", "triangle.xpm", "star.xpm", 
				"plus_sign.xpm", "octagon.xpm", "diamond.xpm", "inverted_triangle.xpm" };

	if ( locationSetController->ModifiedShape() == true )
	{
		// Set label for static box
		m_legendLocationsSizerShape->GetStaticBox()->SetLabel( wxT( "Shape" ) );

		// Insert text into legend
		wxStaticText* id = new wxStaticText( m_legendLocations, 1,
			wxT( "<Individually Set>" ), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
		m_legendLocationsSizerShape2->Add(id, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

		id->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GenGisFrame::OnSamplesLegendClick ), NULL, this );
	}
	else if ( locationSetController->GetUniformShapeFlag() == true )
	{
		// Set label for static box
		m_legendLocationsSizerShape->GetStaticBox()->SetLabel( wxT( "Shape (Uniform)" ) );

		VisualMarker::MARKER_SHAPE unformShape = locationSetController->GetUniformShape();
		wxString filename = wxString( filenames[unformShape].c_str(), wxConvUTF8 );

		// Load bitmap shape and insert into shape legend
		wxBitmap bitmap;
		bitmap.LoadFile( GenGIS::App::Inst().GetExeDir() + wxT( "images/shapes/" ) + filename, wxBITMAP_TYPE_XPM );
		wxStaticBitmap *bitmap2 = new wxStaticBitmap( m_legendLocations, 1, bitmap, wxDefaultPosition, wxDefaultSize, 0 );
		bitmap2->SetBitmap( bitmap );
		m_legendLocationsSizerShape2->Add( bitmap2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

		bitmap2->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GenGisFrame::OnSamplesLegendClick ), NULL, this );

		// Insert text into legend
		wxStaticText* id = new wxStaticText( m_legendLocations, wxID_ANY,
			wxT( "All locations" ), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
		m_legendLocationsSizerShape2->Add(id, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	}
	else
	{
		// Set label for static box
		wxString staticBoxTitle = wxT( "Shape (" ) + wxString( locationSetController->GetShapeField().c_str(), wxConvISO8859_1 ) + wxT( ")" );
		m_legendLocationsSizerShape->GetStaticBox()->SetLabel( staticBoxTitle );

		// Retrieve the current set of field values
		std::vector<std::wstring> fieldValues;
		locationSetController->GetSortedFieldValues( locationSetController->GetShapeField(), fieldValues );

		// Retrieve the current set of shape map values
		std::map<std::wstring, VisualMarker::MARKER_SHAPE> shapeMapValues =	locationSetController->GetShapeMap()->GetNameToShapeMap();
		
		// Iterate through the field values
		std::vector<std::wstring>::iterator it;
		for ( it = fieldValues.begin(); it < fieldValues.end(); it++ )
		{
			wxString filename = wxString( filenames[shapeMapValues[(*it)]].c_str(), wxConvUTF8 );

			// Load bitmap shape and insert into shape legend
			wxBitmap bitmap;
			bitmap.LoadFile( GenGIS::App::Inst().GetExeDir() + wxT( "images/shapes/" ) + filename, wxBITMAP_TYPE_XPM );
			wxStaticBitmap *bitmap2 = new wxStaticBitmap( m_legendLocations, 1, bitmap, wxDefaultPosition, wxDefaultSize, 0 );
			bitmap2->SetBitmap( bitmap );
			m_legendLocationsSizerShape2->Add( bitmap2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

			bitmap2->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GenGisFrame::OnSamplesLegendClick ), NULL, this );

			// Load shape description and insert into legend
			wxStaticText* id = new wxStaticText( m_legendLocations, wxID_ANY,
				wxString( (*it).c_str(), wxConvISO8859_1 ), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
			m_legendLocationsSizerShape2->Add(id, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
		}
	}

	// Set properties for the Size box

	if ( m_legendLocationsSizerSize2 != NULL)
		m_legendLocationsSizerSize2->Clear(true);

	if ( locationSetController->ModifiedSize() == true )
	{
		// Set label for static box
		m_legendLocationsSizerSize->GetStaticBox()->SetLabel( wxT( "Size" ) );

		// Insert text into legend
		wxStaticText* id = new wxStaticText( m_legendLocations, 2,
			wxT( "<Individually Set>" ), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
		m_legendLocationsSizerSize2->Add(id, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

		id->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GenGisFrame::OnSamplesLegendClick ), NULL, this );
	}
	else
	{
		double min = locationSetController->GetMinLocationSize();
		double max = locationSetController->GetMaxLocationSize();

		wxString staticBoxTitle;

		// If all location sizes are equal
		if ( min == max )
		{
			// Set title for static box label
			staticBoxTitle = wxT( "Size (Uniform)" );

			// Set bitmap and label for uniform box
			wxStaticBitmap *minBitmap = new wxStaticBitmap( m_legendLocations, 2, wxNullBitmap,
				wxDefaultPosition, wxSize( (int)min*2, (int)min*2 ), 0, wxT( "Uniform Size" ) );
			minBitmap->SetBackgroundColour( wxColour( 96, 96, 96 ) );
			m_legendLocationsSizerSize2->Add( minBitmap, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

			minBitmap->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GenGisFrame::OnSamplesLegendClick ), NULL, this );

			wxStaticText* minText = new wxStaticText( m_legendLocations, wxID_ANY,
				wxT("All locations"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
			m_legendLocationsSizerSize2->Add( minText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
		}
		else
		{
			// Set title for static box label
			staticBoxTitle = wxT( "Size (" ) + wxString( locationSetController->GetSizeField().c_str(), wxConvISO8859_1 ) + wxT( ")" );

			// Get min and max field to chart values
			double minSize = INT_MAX;
			double maxSize = INT_MIN;
			locationSetController->FindMinMaxLocationSizes( minSize, maxSize );

			// Set bitmap and label for minimum box
			wxString minString = wxString::Format( wxT( "%.2f" ), minSize );

			wxStaticBitmap *minBitmap = new wxStaticBitmap( m_legendLocations, 2, wxNullBitmap,
				wxDefaultPosition, wxSize( (int)min*2, (int)min*2 ), 0, wxT( "Minimum Size" ) );
			minBitmap->SetBackgroundColour( wxColour( 96, 96, 96 ) );
			m_legendLocationsSizerSize2->Add( minBitmap, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

			minBitmap->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GenGisFrame::OnSamplesLegendClick ), NULL, this );

			wxStaticText* minText = new wxStaticText( m_legendLocations, wxID_ANY,
				wxString( wxT("Minimum ("), wxConvISO8859_1 ) + minString + wxT(")"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
			m_legendLocationsSizerSize2->Add( minText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
			
			// Set bitmap and label for maximum box
			wxString maxString = wxString::Format( wxT( "%.2f" ), maxSize );

			wxStaticBitmap *maxBitmap = new wxStaticBitmap( m_legendLocations, 2, wxNullBitmap,
				wxDefaultPosition, wxSize( (int)max*2, (int)max*2 ), 0, wxT( "Maximum Size" ) );
			maxBitmap->SetBackgroundColour( wxColour( 96, 96, 96 ) );
			m_legendLocationsSizerSize2->Add( maxBitmap, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

			maxBitmap->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GenGisFrame::OnSamplesLegendClick ), NULL, this );

			wxStaticText* maxText = new wxStaticText( m_legendLocations, wxID_ANY,
				wxString( wxT("Maximum ("), wxConvISO8859_1 ) + maxString + wxT(")"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
			m_legendLocationsSizerSize2->Add( maxText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
		}

		// Set label for static box
		m_legendLocationsSizerSize->GetStaticBox()->SetLabel( staticBoxTitle );
	}

	m_legendLocations->FitInside();
	m_legendLocations->Refresh( false );
}

/** File->New Session event handler. */
void GenGisFrame::OnFileNewSession( wxCommandEvent& event )
{
#ifdef WIN32
	// Windows version
	ShellExecute (NULL, wxT( "open" ), wxT( "GenGIS.exe" ), NULL, App::Inst().GetExeDir(), SW_SHOW);

#else
	// Mac version
    std::string newSessionCmd = "open -n GenGIS.app";
    system( newSessionCmd.c_str() );

#endif
}

/** File->Open Session event handler. */
void GenGisFrame::OnFileOpenSession( wxCommandEvent& event )
{
	wxString sessionFullPath;

	// Handle sessions opened through the 'File->Recent Sessions' menu
	int selectedSession = event.GetId() % 10 - 1;
	if ( ( event.GetId() >= ID_MNU_FILE_RECENT_SESSION_1 ) &&
		 ( event.GetId() <= ID_MNU_FILE_RECENT_SESSION_9 ) )
	{
		wxString tempPath( m_recentSessions[selectedSession].c_str(), wxConvUTF8 );
		sessionFullPath = tempPath;
	}
	else
	{
		// Create a new OPEN session dialog
		wxFileDialog* openDlg = new wxFileDialog( this, wxT( "Open GenGIS session..." ),
			wxEmptyString, wxEmptyString, wxT( "*.*" ), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR );
		openDlg->SetWildcard( wxT( "GenGIS Session (*.ggis)|*.ggis" ) );

		// Return to the current session if the user hits 'cancel' within the OPEN session dialog
		if ( openDlg->ShowModal() != wxID_OK )
		{
			openDlg->Destroy();
			return;
		}

		sessionFullPath = openDlg->GetPath().wc_str();
		openDlg->Destroy();
	}

	if ( App::Inst().IsNewSession() )
	{
		OpenSession( sessionFullPath );
	}
	else
	{
		#ifdef WIN32
		ShellExecute (NULL, wxT( "open" ), wxT( "GenGIS.exe" ), sessionFullPath, App::Inst().GetExeDir(), SW_SHOW);
		#else
		std::string _fileName = std::string( sessionFullPath.mb_str() );
		std::string newSessionCmd = "open -n GenGIS.app " + _fileName;
		system( newSessionCmd.c_str() );
		#endif
	}	
}

void GenGisFrame::OpenSession( wxString sessionFullPath )
{
	// Get the full path of session file and parse it
	wxFileName tempFilename( sessionFullPath );
	wxString sessionFilename = tempFilename.GetName();
	
	// Open input file for loading
	std::wifstream ifs( sessionFullPath.mb_str( wxConvUTF8 ) );
	std::locale loc( ifs.getloc(), new boost::archive::codecvt_null<std::ostream::char_type>() );
	ifs.imbue( loc );

	if ( ifs.is_open() )
	{
		try
		{
			boost::archive::text_wiarchive ia( ifs, boost::archive::no_codecvt );

			// Enable serialization mode
			App::Inst().ToggleSerializationStatus();			

			// Read in data from session file
			ia >> App::Inst();			

			// Read in Viewport separately
			App::Inst().GetViewport()->ViewportSerialize( ia, App::SAVE_VERSION );

			// Read in GenGisFrame separately
			GenGisFrameSerialize( ia, App::SAVE_VERSION );

			// Save UniqueId instance separately
			UniqueId::Inst().UniqueIdSerialize( ia, App::SAVE_VERSION );
		}
		catch ( ... )
		{
			wxMessageBox( wxT( "GenGIS is unable to load the requested session file." ),
				wxT( "Error" ), wxOK | wxICON_ERROR );
			Log::Inst().Error("(Error) Could not open session file.");
			return;
		}

		App::Inst().NoLongerANewSession();
		App::Inst().SetSessionFullPath( sessionFullPath );
		AddToRecentSessionList( sessionFullPath );
		App::Inst().SetSaveStatus( SESSION_SAVED );

		ifs.close();
	}
	else
	{
		int answer = wxMessageBox( wxT( "Session does not exist or has been moved, remove it from history?" ),
		wxT( "GenGIS" ), wxYES_NO );

		if( answer == wxYES )
		{
			// Erase session from history list
			std::string _fileName = std::string( sessionFullPath.mb_str() );
			std::vector<std::string>::iterator position = std::find( m_recentSessions.begin(), m_recentSessions.end(), _fileName );
			
			if (position != m_recentSessions.end())
				m_recentSessions.erase( position );
			
			// Update the menu
			UpdateRecentSessionsMenu();
		}
		return;
	}

	// Start loading layers from restored data
	LayerTreeControllerPtr layerTreeController = App::Inst().GetLayerTreeController();
	StudyLayerPtr studyLayer = layerTreeController->GetStudyLayer( 0 );

	m_pnlViewport->Freeze();

	if ( studyLayer->GetNumMapLayers() > 0 )
	{
		// Load the current Map layer from file
		// Add this Map layer to the tree controller (wxTreeCtrl)
		MapLayerPtr mapLayer = studyLayer->GetMapLayer( 0 );		
			
		LayerOpenMap( mapLayer->GetFullPath(), 0);

		// At this stage, if the map file is not found cancel the loading process and output an error message
			if( !App::Inst().GetMapController()->IsLoaded() )
			{
				Log::Inst().Error("(Error) Could not locate map file associated with this session.");
				wxMessageBox( wxT( "GenGIS could not locate the map file associated with this session." ),
				wxT( "Map file not found" ), wxOK | wxICON_ERROR );

				m_pnlViewport->Thaw();
				App::Inst().ToggleSerializationStatus();
				return;
			}	
		

		for ( uint currLocSetId = 0; currLocSetId < mapLayer->GetNumLocationSetLayers(); ++currLocSetId )
		{	
			// Add the Location Set Layer to the tree controller (wxTreeCtrl)
			LocationSetLayerPtr locationSetLayer = mapLayer->GetLocationSetLayer( currLocSetId );
			layerTreeController->AddLocationSetLayer( locationSetLayer );
			m_locationSetLayer = locationSetLayer;

			// Select the current Location Set layer 
			layerTreeController->SetSelection( locationSetLayer );

			// Add the Sequence Layer to the tree controller (wxTreeCtrl)
			layerTreeController->AddSequence( locationSetLayer );
		}

		for ( uint currTreeId = 0; currTreeId < mapLayer->GetNumTreeLayers(); ++currTreeId )
		{
			// Select the current Map layer
			layerTreeController->SetSelection( mapLayer );

			// Add the Tree Layer to the tree controller (wxTreeCtrl)
			TreeLayerPtr treeLayer = mapLayer->GetTreeLayer( currTreeId );
			layerTreeController->AddTreeLayer( treeLayer );
		}	

		FillSequenceLegend();
		FillSamplesLegend();

	}
	if( studyLayer->GetNumVectorMapLayers() > 0)
		{		
			for( uint currVectorId = 0; currVectorId < studyLayer->GetNumVectorMapLayers() ; ++currVectorId)
			{
				App::Inst().GetLayerTreeController()->SetSelection( App::Inst().GetLayerTreeController()->GetStudyLayer(0) );
				VectorMapLayerPtr vecorMapLayer = studyLayer->GetVectorMapLayer( currVectorId );
				LayerOpenVectorMap( vecorMapLayer->GetFullPath(), currVectorId);

				if( !App::Inst().GetVectorMapController(currVectorId)->IsLoaded() )
				{
					Log::Inst().Error("(Error) Could not locate vector map file associated with this session.");

					wxMessageBox( wxT( "GenGIS could not locate the vector map file associated with this session." ),
					wxT( "Vector Map file not found" ), wxOK | wxICON_ERROR );

					m_pnlViewport->Thaw();
					App::Inst().ToggleSerializationStatus();
					return;
				}

			}
		}

	// Update compass and minimap widgets
	m_widgetController->SetCompassAngle( App::Inst().GetViewport()->GetCamera()->GetYaw() );

	m_pnlViewport->Thaw();

	#ifndef _DEBUG
		PythonInterface::Inst().UpdatePythonInterface();
	#endif

	#ifndef _DEBUG
	SetTitle( sessionFilename + wxT( " - GenGIS" ) );
	#else
	SetTitle( sessionFilename + wxT( " - GenGIS [DEBUG Mode]" ) );
	#endif

	// Disable serialization mode
	App::Inst().ToggleSerializationStatus();

	App::Inst().SetSelectedView(ViewPtr());

	// Redraw screen
	App::Inst().GetViewport()->Refresh(false); 

	return;
}

void GenGisFrame::SaveUserPreferences()
{
	wxString configFile = GenGIS::App::Inst().GetUserDataDir() + wxT( "/GenGIS_Conf.txt" );
	std::string configFileStr( std::string( configFile.begin(), configFile.end() ) );
	std::ofstream out;
	out.open( configFileStr.c_str() );

	if ( out.is_open() )
	{
		out << m_toolbarTextStatus << std::endl;

		out << m_showWelcomeDialog << std::endl;

		out << m_recentSessions.size() << std::endl;

		foreach(const std::string& str, m_recentSessions)
		{
			out << str << std::endl;
		}
	}
}

void GenGisFrame::LoadUserPreferences()
{
	wxString configFile = GenGIS::App::Inst().GetUserDataDir() + wxT( "/GenGIS_Conf.txt" );
	std::string configFileStr( std::string( configFile.begin(), configFile.end() ));
	std::ifstream in;
	in.open( configFileStr.c_str() );

	if ( in.is_open() )
	{
		bool toolbarTextStatus;
		in >> toolbarTextStatus; in.ignore( 80, '\n');
		SetToolbarTextStatus( toolbarTextStatus );

		bool showWelcomeDialog;
		in >> showWelcomeDialog; in.ignore( 80, '\n');
		SetWelcomeDialogStatus( showWelcomeDialog );

		int numOfRecentSessions;
		in >> numOfRecentSessions; in.ignore( 80, '\n');

		std::string session;
		m_recentSessions.clear();
		for (int a = 0; a < numOfRecentSessions; a++)
		{
			getline( in, session, '\n' );
			m_recentSessions.push_back( session );
		}
		PopulateRecentSessionsMenu();
	}
}

void GenGisFrame::AddToRecentSessionList( wxString sessionFilePath )
{
	std::string sessionFilePathStr( std::string( sessionFilePath.begin(), sessionFilePath.end() ));

	std::vector<std::string>::iterator currentFilename;

	bool found = false;
	for ( currentFilename = m_recentSessions.begin();
		currentFilename != m_recentSessions.end(); ++currentFilename )
	{
		if ( sessionFilePathStr == *currentFilename )
		{
			found = true;
			m_recentSessions.erase( currentFilename );
			m_recentSessions.insert( m_recentSessions.begin(), sessionFilePathStr );
			break;
		}
	}

	if ( !found )
	{
		if ( m_recentSessions.size() < 9 )
			m_recentSessions.insert( m_recentSessions.begin(), sessionFilePathStr );

		else // m_recentSessions.size() == 9
		{
			m_recentSessions.insert( m_recentSessions.begin(), sessionFilePathStr );
			m_recentSessions.pop_back();
		}
	}
	UpdateRecentSessionsMenu();
}

void GenGisFrame::PopulateRecentSessionsMenu()
{
	// Return if there are no recent sessions
	if ( m_recentSessions.size() == 0 )
		return;

	// Enable the 'Clear History' button
	m_mnuRecentSessions->FindChildItem( ID_MNU_FILE_RECENT_SESSION_CLEAR_HISTORY )->Enable( true );

	int id = ID_MNU_FILE_RECENT_SESSION_1;

	// Add recent sessions to the menu
	foreach ( const std::string& sessionPath, m_recentSessions )
	{
		wxString sessionPathWX = wxString( sessionPath.c_str(), wxConvUTF8 );

		wxString order = wxString::Format( wxT( "%i" ), id % 10 );

		wxMenuItem* mnuSession = new wxMenuItem( m_mnuRecentSessions, id, order + wxT( ": " ) + sessionPathWX );
		m_mnuRecentSessions->Append( mnuSession );
		id++;

		Connect( mnuSession->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisFrame::OnFileOpenSession ) );
	}
}

void GenGisFrame::UpdateRecentSessionsMenu()
{
	if( m_mnuRecentSessions->GetMenuItemCount() == 2 )
	{
		PopulateRecentSessionsMenu();
		SaveUserPreferences();
		return;
	}

	uint firstMenuID = ID_MNU_FILE_RECENT_SESSION_1;
	uint lastMenuID  = ID_MNU_FILE_RECENT_SESSION_1 + m_mnuRecentSessions->GetMenuItemCount() - 2;

	// Clear the menu all existing entries from the menu
	for( uint currentMenuID = firstMenuID; currentMenuID < lastMenuID; ++currentMenuID )
		m_mnuRecentSessions->Delete( currentMenuID );

	// Disable the 'Clear History' button
	m_mnuRecentSessions->FindChildItem( ID_MNU_FILE_RECENT_SESSION_CLEAR_HISTORY )->Enable( false );

	// Add recent sessions to the menu
	PopulateRecentSessionsMenu();

	SaveUserPreferences();
}

void GenGisFrame::OnClearSessionHistory( wxCommandEvent& event )
{
	int answer = wxMessageBox( wxT( "Clear session history?" ),
		wxT( "GenGIS" ), wxYES_NO );

	if( answer == wxYES )
	{
		// Clear list of recent sessions
		m_recentSessions.clear();
		
		// Update the menu
		UpdateRecentSessionsMenu();
	}
}

/** File->Save (session) event handler. */
void GenGisFrame::OnFileSaveSession( wxCommandEvent& event )
{
	// If the session has not yet been saved (empty file path),
	// open the save dialog to specify a filename and location.
	// Otherwise, save to current file.
	SaveSession( App::Inst().GetSessionFullPath().IsEmpty() );
}

/** File->Save As (session) event handler. */
void GenGisFrame::OnFileSaveSessionAs( wxCommandEvent& event )
{
	// Always open the save dialog to specify a filename and location.
	SaveSession( OPEN_DIALOG );
}

bool GenGisFrame::SaveSession( bool openSaveDialog )
{
	wxString sessionFullPath; // Filename + Directory

	if ( openSaveDialog )
	{
		// Open save dialog to specify a filename and location
		wxFileDialog* saveDlg = new wxFileDialog( this, wxT( "Save GenGIS session..." ),
			wxEmptyString, wxEmptyString, wxT( "GGIS Session (*.ggis)|*.ggis" ), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR );

		if ( saveDlg->ShowModal() != wxID_OK )
		{
			saveDlg->Destroy();
			return false;
		}

		sessionFullPath = saveDlg->GetPath();

		// Check whether a file extension has been automatically appended (problematic on the Mac)
		if ( !sessionFullPath.EndsWith( wxT( ".ggis" ) ) )
			sessionFullPath.append( wxT( ".ggis" ) );


		#ifndef _DEBUG
		SetTitle( saveDlg->GetFilename() + wxT( " - GenGIS" ) );
		#else
		SetTitle( saveDlg->GetFilename() + wxT( " - GenGIS [DEBUG Mode]" ) );
		#endif

		saveDlg->Destroy();

		App::Inst().SetSessionFullPath( sessionFullPath );
	}
	else
	{
		// Otherwise save using existing filename and location
		sessionFullPath = App::Inst().GetSessionFullPath();
	}

	// Display a friendly box to notify the user that the session is being saved
	WaitingDlg saveSessionDlg( this );
	saveSessionDlg.Show();
	saveSessionDlg.Update();

	// Make the directory path of each map file relative to the session file
	// so that sessions opened on different machines can automatically locate
	// the appropriate map files.
	StudyLayerPtr studyLayer = App::Inst().GetLayerTreeController()->GetStudyLayer( 0 );
	for ( uint currMapId = 0; currMapId < studyLayer->GetNumMapLayers(); ++currMapId )
	{
		
		MapLayerPtr mapLayer = studyLayer->GetMapLayer( currMapId );
		wxFileName mapFullPath( mapLayer->GetFullPath() );

		if ( mapFullPath.IsAbsolute() )
		{
			mapFullPath.MakeRelativeTo( App::Inst().GetSessionFullPath() );
			mapLayer->SetFullPath( mapFullPath.GetFullPath() );
		}
	}

	for ( uint currVectorMapId = 0; currVectorMapId < studyLayer->GetNumVectorMapLayers(); ++currVectorMapId )
	{
		VectorMapLayerPtr vectorMapLayer = studyLayer->GetVectorMapLayer( currVectorMapId );
		wxFileName vectorMapFullPath( vectorMapLayer->GetFullPath() );

		if ( vectorMapFullPath.IsAbsolute() )
		{
			vectorMapFullPath.MakeRelativeTo( App::Inst().GetSessionFullPath() );
			vectorMapLayer->SetFullPath( vectorMapFullPath.GetFullPath() );
		}
	}

	std::wofstream ofs( sessionFullPath.mb_str() );
	std::locale loc( ofs.getloc(), new boost::archive::codecvt_null<std::ostream::char_type>() );
	ofs.imbue( loc );

	boost::archive::text_woarchive oa( ofs, boost::archive::no_codecvt );

	App::Inst().ToggleSerializationStatus();

	// Save data to session file
	oa << App::Inst();

	// Save Viewport separately
	App::Inst().GetViewport()->ViewportSerialize( oa, App::SAVE_VERSION );

	// Save GenGisFrame separately
	GenGisFrameSerialize( oa, App::SAVE_VERSION );

	// Save UniqueId instance separately
	UniqueId::Inst().UniqueIdSerialize( oa, App::SAVE_VERSION );

	App::Inst().ToggleSerializationStatus();
	AddToRecentSessionList( sessionFullPath );
	App::Inst().SetSaveStatus( SESSION_SAVED );

	ofs.close();

	saveSessionDlg.SaveOK();

#ifdef WIN32
	Sleep( 1000 );
#else
	sleep( 1 );
#endif

	return true;
}

void GenGisFrame::OnFileSaveImageAs( wxCommandEvent& event )
{
	SaveImageDlg* saveDlg = new SaveImageDlg(this);
	saveDlg->ShowModal();

	wxString filename = saveDlg->GetFilename();
	
	if ( !filename.empty() )
	{
		// save (possibly) high resolution image
		App::Inst().SetResolutionFactor(saveDlg->GetResolutionScalingFactor());
		App::Inst().SetSavingHiResImage(true);

		// If no file extension has been provided, append ".png", the default
		if ( !filename.EndsWith( wxT( ".png" ) ) && !filename.EndsWith( wxT( ".ppm") ) )
			filename.append( wxT( ".png" ) );

		App::Inst().GetViewport()->SaveImage(std::string(filename.utf8_str()));

		App::Inst().SetResolutionFactor(1);
		App::Inst().SetSavingHiResImage(false);

		// Redraw screen
		App::Inst().GetViewport()->Refresh(false);

		if ( saveDlg->OpenFileLocationAfterSave() )
		{
			#ifdef WIN32
			wxExecute( wxT("explorer /select,") + filename, wxEXEC_ASYNC, NULL ) ;
			#else
			wxExecute( wxT("open \"") + filename + wxT( "\"" ), wxEXEC_ASYNC, NULL ) ;
			#endif
		}
	}

	saveDlg->Destroy();
}

// Thin wrapper method for opening maps from File menu
void GenGisFrame::OnLayerOpenMap(wxCommandEvent& event)
{
	if(App::Inst().GetLayerTreeController()->GetNumMapLayers() > 0)
	{
		wxMessageBox(wxT("GenGIS currently supports only a single map."),
			wxT("Multiple maps not supported"), wxOK | wxICON_INFORMATION);
		return;
	}

	wxString fullPath = OpenLayerMapDialog();
	App::Inst().GetLayerTreeController()->SetIsBlankRaster(false);

	if ( !fullPath.empty() )
	{
		LayerOpenMap( fullPath, 0);
		App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	}
}

wxString GenGisFrame::OpenLayerMapDialog()
{
	wxFileDialog* openDlg = new wxFileDialog(this, wxT("Open map file..."),
		wxEmptyString, wxEmptyString, wxT("."), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	wxString wildcards = wxT("Common Raster Data Sources (*.dem;*.asc;*.ascii;*.tif;*.tiff;*.jpg;*.jpeg)|*.dem;*.asc;*.ascii;*.tif;*.tiff;*.jpg;*.jpeg|");
	wildcards.append( wxT("Arc/Info ASCII Grid (*.asc;*.ascii)|*.asc;*.ascii|") );
	wildcards.append( wxT("GeoTIFF (*.tif;*.tiff)|*.tif;*.tiff|") );
	wildcards.append( wxT("JPEG (*.jpg;*.jpeg)|*.jpg;*.jpeg|") );
	wildcards.append( wxT("USGS Optional ASCII DEM (*.dem)|*.dem|") );
	wildcards.append( wxT("All files (*.*)|*.*") );
	openDlg->SetWildcard(wildcards);

	if( openDlg->ShowModal() != wxID_OK )
	{
		openDlg->Destroy();
		return wxEmptyString;
	}

	openDlg->Destroy();

	return openDlg->GetPath();
}

// Thin wrapper method to convert std::string to two std::wstrings
void GenGisFrame::LayerOpenMap(std::string file)
{
	if(App::Inst().GetLayerTreeController()->GetNumMapLayers() > 0)
	{
		wxMessageBox(wxT("GenGIS currently supports only a single map."),
			wxT("Multiple maps not upported"), wxOK | wxICON_INFORMATION);
		return;
	}

	LayerOpenMap( wxString( file.c_str(), wxConvUTF8 ), 0);
}

/** Handle actual loading of map from file. */
void GenGisFrame::LayerOpenMap( wxFileName fullPath, uint mapIndex)
{
	// Get the currently selected layer from the tree controller in the 'Layers' tab
	LayerPtr selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();

	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::STUDY)
	{
		StudyControllerPtr studyController = (boost::dynamic_pointer_cast<StudyLayer>(selectedLayer))->GetStudyController();
		ProgressDlgPtr progressDlg;
		//App::Inst().GetLayerTreeController()->SetIsBlankRaster(isBlank);

		if(!App::Inst().GetLayerTreeController()->GetIsBlankRaster()){

		// loading a DEM may take some time so it is nice to have a progress dialog
			wxString str = wxT( "Loading DEM file." );
			str.resize( 75, wxT(' ') );
			progressDlg.reset( new ProgressDlg( wxT( "Loading file" ), str, 100, this ) );
		}
		else{
		// loading a SHP may take some time so it is nice to have a progress dialog
			wxString str = wxT( "Loading SHP file." );
			str.resize( 75, wxT(' ') );
			progressDlg.reset( new ProgressDlg( wxT( "Loading file" ), str, 100, this ) );
		}

		if ( fullPath.IsRelative() )
		{
			fullPath.Normalize( wxPATH_NORM_ALL, App::Inst().GetSessionFullPath() );
		}

		MapControllerPtr mapController;
		if ( App::Inst().GetMapController() == NULL )
			mapController.reset( new MapController() );
		else
			mapController = App::Inst().GetMapController();
		
		DemReader demReader;
		bool bReader = demReader.Open(fullPath.GetFullPath().wc_str(*wxConvCurrent), studyController, mapController, progressDlg);
		if(bReader)
		{
			mapController->SetLoaded(true);

			MapLayerPtr mapLayer;
			if ( !App::Inst().IsCurrentlySerializing() )
			{
				mapLayer.reset(
					new MapLayer(
						UniqueId::Inst().GenerateId(),
						App::Inst().GetLayerTreeController()->GetSelectedLayer(),
						mapController
					)
				);
				mapLayer->SetName( fullPath.GetName() );
			}
			else
				mapLayer = App::Inst().GetLayerTreeController()->GetLayerTreeModel()->GetStudy(0)->GetMapLayer(mapIndex);

			mapLayer->SetFullPath( fullPath.GetFullPath() );
			bool bBuilt=true;
			if (!App::Inst().GetLayerTreeController()->GetIsBlankRaster())
			{
				progressDlg->Update(0, wxT("Building terrain."));
				bBuilt = mapController->GetMapView()->BuildTerrain(mapController, progressDlg);
			}
			else
			{
				progressDlg->Update(0, wxT("Visualizing the Vector Map."));
				bBuilt=true;
				
			}			
					
			if(!bBuilt)
			{
				Log::Inst().Warning("(Warning) Failed to build terrain.");
				return;
			}		
			bool bAddMap = App::Inst().GetLayerTreeController()->AddMapLayer(mapLayer);

			// set map extents
			float width = mapController->GetWidth();
			float height = mapController->GetHeight();
			App::Inst().GetViewport()->GetCamera()->SetModelSpaceExtents(-width*0.5, width*0.5, -height*0.5, height*0.5);

			if (!App::Inst().IsCurrentlySerializing())
			{
				// position camera and lights in a nice default position
				LightingPtr lighting = App::Inst().GetViewport()->GetLighting();
				lighting->SetDefaultValues();
				
				if(mapController->GetMapModel()->GetElevationMap())
					ViewReset();			
				else
					ViewTop();
					
			}
			else // App::Inst().IsCurrentlySerializing()
			{
				mapController->GetMapView()->RefreshQuadtree();
			}
		}
		else
		{
			if (!App::Inst().IsCurrentlySerializing())
			{
				wxMessageBox( wxT( "Failed to load map file. Check console window for warning messages." ),
					wxT( "Failed to read file" ), wxOK | wxICON_INFORMATION );
			}
			else
			{
				//
				int answer = wxMessageBox( wxT( "The map file is missing, would you like to select it?"),
					wxT( "Cannot find map file" ), wxYES_NO );

				if ( answer == wxYES)
				{
					wxString newFullPath = OpenLayerMapDialog();
					if ( !newFullPath.empty() )
					{
						LayerOpenMap( newFullPath, mapIndex);
						App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
					}
				}
				return;
			}
		}
	}
	else
	{
		wxMessageBox(wxT("Please select a study node before adding a new map."),
			wxT("Select study node"), wxOK | wxICON_INFORMATION);
	}
}

void GenGisFrame::OnLayerOpenVectorMap( wxCommandEvent& event )
{
	//if(App::Inst().GetLayerTreeController()->GetNumMapLayers() > 0)
	//{
	//	wxMessageBox(wxT("GenGIS currently supports only a single map."),
	//		wxT("Multiple maps not supported"), wxOK | wxICON_INFORMATION);
	//	return;
	//}

	wxString fullPath = OpenLayerVectorMapDialog();

	if ( !fullPath.empty() )
	{
		LayerOpenVectorMap( fullPath, 0 );
		App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	}
}

wxString GenGisFrame::OpenLayerVectorMapDialog()
{
	wxFileDialog* openDlg = new wxFileDialog(this, wxT("Open vector map file..."),
		wxEmptyString, wxEmptyString, wxT("."), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	wxString wildcards = wxT("Common Vector Data Sources (*.shp)|*.shp|");
	wildcards.append( wxT("ESRI Shapefile (*.shp)|*.shp|") );
	wildcards.append( wxT("All files (*.*)|*.*") );
	openDlg->SetWildcard(wildcards);

	if( openDlg->ShowModal() != wxID_OK )
	{
		openDlg->Destroy();
		return wxEmptyString;
	}

	openDlg->Destroy();

	return openDlg->GetPath();
}

void GenGisFrame::LayerOpenVectorMap(std::string file)
{
	//if(App::Inst().GetLayerTreeController()->GetNumMapLayers() > 0)
	//{
	//	wxMessageBox(wxT("GenGIS currently supports only a single map."),
	//		wxT("Multiple maps not upported"), wxOK | wxICON_INFORMATION);
	//	return;
	//}

	LayerOpenVectorMap( wxString( file.c_str(), wxConvUTF8 ), 0 );
}

void GenGisFrame::LayerOpenVectorMap( wxFileName fullPath, uint mapIndex )
{
	LayerTreeControllerPtr ltc = App::Inst().GetLayerTreeController();
	ltc->SetSelection( ltc->GetStudyLayer(0) );
	LayerPtr selectedLayer = ltc->GetSelectedLayer();
	wxTreeItemId selectionId = ltc->GetTreeCtrl()->GetSelection();
	
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::STUDY)
	{
		StudyControllerPtr studyController = (boost::dynamic_pointer_cast<StudyLayer>(selectedLayer))->GetStudyController();	
		

		// loading a SHP may take some time so it is nice to have a progress dialog
		wxString str = wxT( "Loading SHP file." );
		str.resize( 75, wxT(' ') );
		ProgressDlgPtr progressDlg( new ProgressDlg( wxT( "Loading file" ), str, 100, this ) );

		if ( fullPath.IsRelative() )
		{
			fullPath.Normalize( wxPATH_NORM_ALL, App::Inst().GetSessionFullPath() );
		}
		VectorMapControllerPtr vectorMapController;
		if(!App::Inst().IsCurrentlySerializing())
				vectorMapController.reset( new VectorMapController() );
		else
				vectorMapController = App::Inst().GetVectorMapController(mapIndex);
		
		ShpReader shpReader;
		bool bReader = shpReader.Open(
		fullPath.GetFullPath().wc_str(*wxConvCurrent),
		ltc->GetStudyLayer(0)->GetStudyController(),
		vectorMapController,
		progressDlg
		);
		if (bReader)
		{
			vectorMapController->SetLoaded(true);

			VectorMapLayerPtr vectorMapLayer;
			if(!App::Inst().IsCurrentlySerializing())
				vectorMapLayer.reset(new VectorMapLayer( UniqueId::Inst().GenerateId(), ltc->GetStudyLayer(0), vectorMapController ));
			else			
				vectorMapLayer = App::Inst().GetLayerTreeController()->GetLayerTreeModel()->GetStudy(0)->GetVectorMapLayer(mapIndex);
			vectorMapLayer->SetName( fullPath.GetName() );
			vectorMapLayer->SetFullPath( fullPath.GetFullPath() );
			ltc->AddVectorMapLayer(vectorMapLayer,selectedLayer,selectionId);
			wxTreeItemId vectorSelectionId = ltc->GetTreeCtrl()->GetSelection();
			
			if ( App::Inst().GetLayerTreeController()->GetNumMapLayers() ==0 )
			{
				App::Inst().GetLayerTreeController()->SetSelection( App::Inst().GetLayerTreeController()->GetStudyLayer(0) );
				App::Inst().GetLayerTreeController()->SetIsBlankRaster(true);
				//LayerOpenMap(wxString(App::Inst().GetExeDir() + wxT("images/blankRaster.tif")),0);
				LayerOpenMap(wxString(App::Inst().GetExeDir() + wxT("")),0);
			//remove the empty raster layer from the study layer
			//StudyLayerPtr studyLayer = App::Inst().GetLayerTreeController()->GetStudyLayer(0);
			//studyLayer->RemoveAllMapLayers(); 
			}

			// Render vector map if the blank raster is loaded
			if ( App::Inst().GetLayerTreeController()->GetNumMapLayers() >0 )
			{				
				ltc->GetVectorMapLayer(0)->GetVectorMapController()->GetVectorMapView()->Render( true );
				App::Inst().GetViewport()->Refresh(false);
			}
			else
			{
				//if the user presses the cancel button during the loading process of the vector file, then the vector file will be removed from the layer tree 
				LayerTreeItem* selectionItem = (LayerTreeItem*)(ltc->GetTreeView())->GetTreeCtrl()->GetItemData(vectorSelectionId);
				LayerPtr layer = selectionItem->GetLayer();
				ltc->EraseVectorMap(layer->GetId());	
				ltc->GetLayerTreeModel()->RemoveLayer(layer->GetId());
				
				ltc->GetTreeView()->GetTreeCtrl()->Delete(vectorSelectionId);
				App::Inst().GetViewport()->Refresh();
			}
		}
		else
		{
			if (!App::Inst().IsCurrentlySerializing())
			{
				wxMessageBox( wxT( "Failed to load vector map file ")+ fullPath.GetName()+ wxT(".shp")+ wxT(" .Check console window for warning messages." ),
					wxT( "Failed to read file" ), wxOK | wxICON_INFORMATION );
			}
			else
			{
				//
				int answer = wxMessageBox( wxT( "The vector map file ")+ fullPath.GetName()+ wxT(".shp")+ wxT(" is missing, would you like to select it?"),
					wxT( "Cannot find vector map file" ), wxYES_NO );				

				if ( answer == wxYES)
				{
					wxString newFullPath = OpenLayerVectorMapDialog();
					if ( !newFullPath.empty() )
					{
						LayerOpenVectorMap( newFullPath, mapIndex);
						App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
					}
				}
				return;
			}
		}
	}

	else
	{
		wxMessageBox(wxT("Please select a study node before adding a new vector map."),
			wxT("Select study node"), wxOK | wxICON_INFORMATION);
	}
}

void GenGisFrame::OnLocationMerge( wxCommandEvent& event)
{
	if ( App::Inst().GetLayerTreeController()->GetNumMapLayers() != 1 )
	{
		wxMessageBox(wxT("Please load a map file before loading a location file."),
					wxT("Load map file first"), wxOK | wxICON_INFORMATION);
		return;
	}
	if ( App::Inst().GetLayerTreeController()->GetNumLocationSetLayers() <= 1 )
	{
		wxMessageBox(wxT("Please load more than one location set."),
			wxT("Needs multiple location sets"), wxOK | wxICON_INFORMATION);
		return;
	}
	//passed checks now need:
	//launch check box
	//populate box with N number of location layers
	//Go and Close buttons
	//take those selected and merge into one location layer
	//add location layer to GenGIS as location[0]
	LocationMergeDlg* dlg = new LocationMergeDlg(this);
	dlg->Show();
}
void GenGisFrame::OnLayerOpenLocations( wxCommandEvent& event )
{
	if ( App::Inst().GetLayerTreeController()->GetNumMapLayers() != 1 )
	{
		wxMessageBox(wxT("Please load a map file before loading a location file."),
					wxT("Load map file first"), wxOK | wxICON_INFORMATION);
		return;
	}

	//if ( App::Inst().GetLayerTreeController()->GetNumLocationSetLayers() > 0 )
	//{
	//	wxMessageBox( wxT( "GenGIS currently supports only a single location set." ),
	//		wxT( "Multiple location sets not upported" ), wxOK | wxICON_INFORMATION );
	//	return;
	//}

	wxFileDialog* openDlg = new wxFileDialog(this, wxT("Open location set file..."), wxEmptyString,wxEmptyString, wxT("*.*"), 
		wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	wxString wildcards = _T("Location set file (*.csv;*.txt)|*.csv;*.txt| All files (*.*)|*.*");
	openDlg->SetWildcard(wildcards);

	if(openDlg->ShowModal() != wxID_OK)
	{
		openDlg->Destroy();
		return;
	}

	openDlg->Destroy();
	LayerOpenLocations( openDlg->GetPath() );
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::LayerOpenLocations( std::string file )
{
	LayerOpenLocations( wxString( file.c_str(), wxConvUTF8 ) );
}

void GenGisFrame::LayerOpenLocations( std::vector<std::wstring>& csvTableRows, std::wstring locationNames )
{
	//if ( App::Inst().GetLayerTreeController()->GetNumLocationSetLayers() > 0 )
	//{
	//	wxMessageBox( wxT( "GenGIS currently supports only a single location set." ),
	//		wxT( "Multiple location sets not supported" ), wxOK | wxICON_INFORMATION );
	//	return;
	//}

	// get selected layer
	LayerPtr selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer == LayerPtr() || selectedLayer->GetType() != Layer::MAP)
	{
		if(App::Inst().GetLayerTreeController()->GetNumMapLayers() == 1)
		{
			// select the first map by default
			App::Inst().GetLayerTreeController()->SetSelection( App::Inst().GetLayerTreeController()->GetMapLayer(0) );
		}
	}

	selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::MAP)
	{
		std::vector<LocationModelPtr> locationModels;
		StudyControllerPtr studyController = App::Inst().GetLayerTreeController()->GetStudyLayer(0)->GetStudyController();

		if( LocationSetIO::ParseCSVFile( csvTableRows, studyController, locationModels ) )
		{	
			// stop refreshing the tree until all sequences are loaded (this is purely for efficency)
			App::Inst().GetLayerTreeController()->GetTreeCtrl()->Freeze();

			ChartSetViewPtr chartSetCtrl(new ChartSetView());
			LocationSetLayerPtr locationSet(new LocationSetLayer(UniqueId::Inst().GenerateId(), 
				App::Inst().GetLayerTreeController()->GetSelectedLayer(),
				chartSetCtrl));

			locationSet->SetName( wxString( locationNames.c_str(), wxConvISO8859_1 ) );
			locationSet->SetFullPath( wxString( locationNames.c_str(), wxConvISO8859_1 ) );
			m_locationSetLayer = locationSet;

			// assign default colour map to location
			ColourMapManagerPtr colourMapManager = App::Inst().GetColourMapManager();
			ColourMapPtr defaultColourMap = colourMapManager->GetDefaultDiscreteColourMap();
			ColourMapDiscretePtr newColourMap(new ColourMapDiscrete(defaultColourMap));

			std::vector<LocationLayerPtr> locationLayers;
			foreach(LocationModelPtr locationModel, locationModels)
			{
				LocationViewPtr locationView(new LocationView(locationModel, App::Inst().GetViewport()->GetCamera(), UniqueId::Inst().GenerateId()));
				ChartViewPtr pieChartView(new ChartView(locationModel, locationView, newColourMap));
				chartSetCtrl->AddChart(pieChartView);
				LocationControllerPtr locationController(new LocationController(locationModel, locationView, pieChartView));

				LocationLayerPtr locationLayer(new LocationLayer(UniqueId::Inst().GenerateId(), locationSet, locationController));			
				locationLayer->SetName(wxString(locationController->GetId().c_str()));
				locationLayer->SetFullPath( wxString( locationNames.c_str(), wxConvISO8859_1 ) );
				locationLayers.push_back(locationLayer);

				locationView->SetLocationLayerId(locationLayer->GetId());
				pieChartView->SetLocationLayerId(locationLayer->GetId());

				locationSet->AddLocationLayer(locationLayer);
			}
			locationSet->GetLocationSetController()->SetLocationSetLayers(locationLayers);

			App::Inst().GetLayerTreeController()->AddLocationSetLayer(locationSet);

			App::Inst().GetLayerTreeController()->GetTreeCtrl()->Thaw();

			// Update Samples legend on sidebar
			FillSamplesLegend();

			App::Inst().GetLayerTreeController()->GetLocationSetLayer(0)->GetLocationSetController()->GetNumericMetadataFields();
		}
		else
		{
			wxMessageBox(wxT("Failed to read location sites. Check console window for warning messages."), 
				wxT("Failed to read location sites"), wxOK | wxICON_INFORMATION);
		}
	}
	else
	{
		wxMessageBox(wxT("Please select a map node before adding a location sites layer."),
			wxT("Select map node"), wxOK | wxICON_INFORMATION);
	}
}

void GenGisFrame::LayerOpenLocations( wxFileName fullPath )
{
	if ( App::Inst().GetLayerTreeController()->GetNumLocationSetLayers() > 0 )
	{
		//wxMessageBox( wxT( "GenGIS currently supports only a single location set." ),
		//	wxT( "Multiple location sets not supported" ), wxOK | wxICON_INFORMATION );
		//return;
	}

	// get selected layer
	LayerPtr selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer == LayerPtr() || selectedLayer->GetType() != Layer::MAP)
	{
		if(App::Inst().GetLayerTreeController()->GetNumMapLayers() == 1)
		{
			// select the first map by default
			App::Inst().GetLayerTreeController()->SetSelection( App::Inst().GetLayerTreeController()->GetMapLayer(0) );
		}
	}

	selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::MAP)
	{
		std::vector<LocationModelPtr> locationModels;
		StudyControllerPtr studyController = App::Inst().GetLayerTreeController()->GetStudyLayer(0)->GetStudyController();

		if( LocationSetIO::Read( fullPath.GetFullPath(), studyController, locationModels ) )
		{	
			// stop refreshing the tree until all sequences are loaded (this is purely for efficency)
			App::Inst().GetLayerTreeController()->GetTreeCtrl()->Freeze();

			ChartSetViewPtr chartSetCtrl(new ChartSetView());
			LocationSetLayerPtr locationSet(new LocationSetLayer(UniqueId::Inst().GenerateId(), 
				App::Inst().GetLayerTreeController()->GetSelectedLayer(),
				chartSetCtrl));
			locationSet->SetName( fullPath.GetName() );
			locationSet->SetFullPath( fullPath.GetFullPath() );
			m_locationSetLayer = locationSet;

			LocationSetIO::ReadSourceFile( fullPath.GetFullPath(), locationSet );

			// assign default colour map to location
			ColourMapManagerPtr colourMapManager = App::Inst().GetColourMapManager();
			ColourMapPtr defaultColourMap = colourMapManager->GetDefaultDiscreteColourMap();
			ColourMapDiscretePtr newColourMap(new ColourMapDiscrete(defaultColourMap));

			std::vector<LocationLayerPtr> locationLayers;
			foreach(LocationModelPtr locationModel, locationModels)
			{
				LocationViewPtr locationView(new LocationView(locationModel, App::Inst().GetViewport()->GetCamera(), UniqueId::Inst().GenerateId()));
				ChartViewPtr pieChartView(new ChartView(locationModel, locationView, newColourMap));
				chartSetCtrl->AddChart(pieChartView);
				LocationControllerPtr locationController(new LocationController(locationModel, locationView, pieChartView));

				LocationLayerPtr locationLayer(new LocationLayer(UniqueId::Inst().GenerateId(), locationSet, locationController));			
				locationLayer->SetName(wxString(locationController->GetId().c_str()));
				locationLayer->SetFullPath( fullPath.GetFullPath() );
				locationLayers.push_back(locationLayer);

				locationView->SetLocationLayerId(locationLayer->GetId());
				pieChartView->SetLocationLayerId(locationLayer->GetId());

				locationSet->AddLocationLayer(locationLayer);
			}

			locationSet->GetLocationSetController()->SetLocationSetLayers(locationLayers);

			App::Inst().GetLayerTreeController()->AddLocationSetLayer(locationSet);

			App::Inst().GetLayerTreeController()->GetTreeCtrl()->Thaw();

			// Update Samples legend on sidebar
			FillSamplesLegend();

			App::Inst().GetLayerTreeController()->GetLocationSetLayer(0)->GetLocationSetController()->GetNumericMetadataFields();
		}
		else
		{
			wxMessageBox(wxT("Failed to read location sites from file. Check console window for warning messages."), 
				wxT("Failed to read file"), wxOK | wxICON_INFORMATION);
		}
	}
	else
	{
		wxMessageBox(wxT("Please select a map node before adding a location sites layer."),
			wxT("Select map node"), wxOK | wxICON_INFORMATION);
	}
}

void GenGisFrame::OnLayerOpenSequenceData( wxCommandEvent& event )
{
	if ( App::Inst().GetLayerTreeController()->GetNumLocationSetLayers() != 1 )
	{
		wxMessageBox(wxT("Please load a location file before loading a sequence file."),
					wxT("Load location file first"), wxOK | wxICON_INFORMATION);
		return;
	}

	if ( App::Inst().GetLayerTreeController()->GetNumSequenceLayers() > 0 )
	{
		wxMessageBox( wxT( "GenGIS currently supports only a single set of sequences." ),
			wxT( "Multiple sets of sequences not supported" ), wxOK | wxICON_INFORMATION );
		return;
	}

	wxFileDialog* openDlg = new wxFileDialog(this, wxT("Open sequence data file..."), wxEmptyString, wxEmptyString, wxT("*.*"), 
		wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	wxString wildcards = _T("Sequence data file (*.csv;*.txt)|*.csv;*.txt| All files (*.*)|*.*");
	openDlg->SetWildcard(wildcards);

	if(openDlg->ShowModal() != wxID_OK)
	{
		openDlg->Destroy();
		return;
	}

	openDlg->Destroy();
	LayerOpenSequenceData( openDlg->GetPath() );
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::LayerOpenSequenceData( std::string file )
{
	LayerOpenSequenceData( wxString( file.c_str(), wxConvUTF8 ) );
}

void GenGisFrame::LayerOpenSequenceData( wxFileName fullPath )
{
	// determine the selected layer
	LayerPtr selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer == LayerPtr() || selectedLayer->GetType() != Layer::LOCATION_SET)
	{
		if(App::Inst().GetLayerTreeController()->GetNumLocationSetLayers() == 1)
		{
			// select the first location set layer by default
			App::Inst().GetLayerTreeController()->SetSelection( App::Inst().GetLayerTreeController()->GetLocationSetLayer(0) );
		}
	}

	selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::LOCATION_SET)
	{
		std::vector<SequenceModelPtr> sequenceModels;
		bool bCancel;
		if( SequenceIO::Read( fullPath.GetFullPath(), sequenceModels, bCancel ) )
		{	
			wxBusyCursor wait;

			// stop refreshing the tree until all sequences are loaded (this is purely for efficency)
			App::Inst().GetLayerTreeController()->GetTreeCtrl()->Freeze();

			std::set<std::wstring> missingLocations;
			foreach(SequenceModelPtr sequenceModel, sequenceModels)
			{
				SequenceControllerPtr sequenceController(new SequenceController(sequenceModel));

				SequenceLayerPtr sequenceLayer(new SequenceLayer(UniqueId::Inst().GenerateId(), selectedLayer, sequenceController));
				sequenceLayer->SetName(sequenceController->GetSequenceId().c_str());
				sequenceLayer->SetFullPath( fullPath.GetFullPath() );

				if(!App::Inst().GetLayerTreeController()->AddSequence(sequenceLayer, sequenceModel, missingLocations))
				{			
					// The above function should never return false!
					wxMessageBox(wxT("Critial error: Failed to load sequence data. Please report this error to the GenGIS team."), wxT("Failed to load sequence data."), wxOK | wxICON_ERROR);
					return;
				}
			}

			// report any locations that are specified in the sequence file, but do not exist
			foreach(std::wstring locationStr, missingLocations)
			{
				Log::Inst().Warning(wxT("(Warning) Site id '") + wxString(locationStr.c_str()) + wxT("' was not found in the location set. Sequences from this location will be ignored."));
			}

			// all tree to refresh
			App::Inst().GetLayerTreeController()->GetTreeCtrl()->Thaw();
		}
		else
		{
			if(!bCancel)
				wxMessageBox(wxT("Failed to read sequence data from file. Check console window for warning messages."), wxT("Failed to read file"), wxOK | wxICON_INFORMATION);
		}
	}
	else
	{
		wxMessageBox(wxT("Please select a location set node before adding sequence data."), wxT("Select location set node"), wxOK | wxICON_INFORMATION);
	}

	App::Inst().GetViewport()->Refresh(false);
}


void GenGisFrame::LayerOpenSequenceData( std::vector<std::wstring>& csvTableRows, std::wstring locationNames )
{
	// determine the selected layer
	LayerPtr selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer == LayerPtr() || selectedLayer->GetType() != Layer::LOCATION_SET)
	{
		if(App::Inst().GetLayerTreeController()->GetNumLocationSetLayers() == 1)
		{
			// select the first location set layer by default
			App::Inst().GetLayerTreeController()->SetSelection( App::Inst().GetLayerTreeController()->GetLocationSetLayer(0) );
		}
	}

	selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::LOCATION_SET)
	{
		std::vector<SequenceModelPtr> sequenceModels;
		bool bCancel;
		if( SequenceIO::ParseCSVFile( csvTableRows, sequenceModels, bCancel ) )
		{	
			wxBusyCursor wait;

			// stop refreshing the tree until all sequences are loaded (this is purely for efficency)
			App::Inst().GetLayerTreeController()->GetTreeCtrl()->Freeze();

			std::set<std::wstring> missingLocations;
			foreach(SequenceModelPtr sequenceModel, sequenceModels)
			{
				SequenceControllerPtr sequenceController(new SequenceController(sequenceModel));

				SequenceLayerPtr sequenceLayer(new SequenceLayer(UniqueId::Inst().GenerateId(), selectedLayer, sequenceController));
				sequenceLayer->SetName(sequenceController->GetSequenceId().c_str());
				sequenceLayer->SetFullPath( wxString( locationNames.c_str(), wxConvISO8859_1 ) );

				if(!App::Inst().GetLayerTreeController()->AddSequence(sequenceLayer, sequenceModel, missingLocations))
				{			
					// The above function should never return false!
					wxMessageBox(wxT("Critial error: Failed to load sequence data. Please report this error to the GenGIS team."), wxT("Failed to load sequence data."), wxOK | wxICON_ERROR);
					return;
				}
			}

			// report any locations that are specified in the sequence file, but do not exist
			foreach(std::wstring locationStr, missingLocations)
			{
				Log::Inst().Warning(wxT("(Warning) Site id '") + wxString(locationStr.c_str()) + wxT("' was not found in the location set. Sequences from this location will be ignored."));
			}

			// all tree to refresh
			App::Inst().GetLayerTreeController()->GetTreeCtrl()->Thaw();
		}
		else
		{
			if(!bCancel)
				wxMessageBox(wxT("Failed to read sequence data from file. Check console window for warning messages."), wxT("Failed to read file"), wxOK | wxICON_INFORMATION);
		}
	}
	else
	{
		wxMessageBox(wxT("Please select a location set node before adding sequence data."), wxT("Select location set node"), wxOK | wxICON_INFORMATION);
	}

	App::Inst().GetViewport()->Refresh(false);
}

void GenGisFrame::OnLayerOpenTree( wxCommandEvent& event )
{
	if ( App::Inst().GetLayerTreeController()->GetNumLocationSetLayers() < 1 )
	{
		wxMessageBox(wxT("Please load a locations file before loading a tree file."),
					wxT("Load locations file first"), wxOK | wxICON_INFORMATION);
		return;
	}

	wxFileDialog* openDlg = new wxFileDialog(this, wxT("Open tree file..."), wxEmptyString, wxEmptyString, wxT("*.*"), 
		wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	wxString wildcards = _T("Geographic tree model (*.tre;*.gtm;*.nwk)|*.tre;*.gtm;*.nwk|All files (*.*)|*.*");
	openDlg->SetWildcard(wildcards);

	if(openDlg->ShowModal() != wxID_OK)
	{
		openDlg->Destroy();
		return;
	}

	wxArrayString filePaths;
	openDlg->GetPaths( filePaths );
	openDlg->Destroy();

	for ( uint index = 0; index < filePaths.GetCount(); index++ )
	{
		LayerOpenTree( filePaths[index] );
	}

	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::LayerOpenTree(std::string file)
{
	LayerOpenTree( wxString( file.c_str(), wxConvUTF8 ) );
}

void GenGisFrame::LayerOpenTree( wxFileName fullPath )
{
	// determine the selected layer
	LayerPtr selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer == LayerPtr() || selectedLayer->GetType() != Layer::MAP)
	{
		if(App::Inst().GetLayerTreeController()->GetNumMapLayers() == 1)
		{
			// select the first map by default
			App::Inst().GetLayerTreeController()->SetSelection( App::Inst().GetLayerTreeController()->GetMapLayer(0) );
		}
	}

	selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();
	if(selectedLayer != LayerPtr() && selectedLayer->GetType() == Layer::MAP)
	{
		GeoTreeIO geoTreeIO;
		Tree<NodeGeoTree>::Ptr geoTree(new Tree<NodeGeoTree>());

		bool bSuccess = geoTreeIO.Read(geoTree, std::string( fullPath.GetFullPath().mb_str() ) );

		if(bSuccess)
		{
			GeoTreeViewPtr geoTreeView(new GeoTreeView(geoTree, selectedLayer));
			geoTreeView->SetLayout(GeoTreeView::SLANTED_CLADOGRAM_3D);

			TreeLayerPtr treeLayer(new TreeLayer(UniqueId::Inst().GenerateId(), selectedLayer, geoTreeView));
			treeLayer->SetName( fullPath.GetName() );
			treeLayer->SetFullPath( fullPath.GetFullPath() );

			geoTreeView->SetTreeLayerId(treeLayer->GetId());

			App::Inst().GetLayerTreeController()->AddTreeLayer(treeLayer);
			//~mikep Talk to donovan about fixing this
			std::vector<GenGIS::LocationLayerPtr, std::allocator<GenGIS::LocationLayerPtr> > temp = App::Inst().GetLayerTreeController()->GetLocationLayers();
			geoTreeView->ProjectToActiveSet(temp);

			geoTreeView->SignalNodeUpdate(boost::bind(&GenGisFrame::SigNodeUpdate, this, _1));
		}
		else
		{
			wxMessageBox(wxT("Failed to read geographic tree model from file."),
				wxT("Failed to read file"), wxOK | wxICON_INFORMATION);
		}
	}
	else
	{
		wxMessageBox(wxT("Please select a map node before adding a tree layer."),
			wxT("Select map node"), wxOK | wxICON_INFORMATION);
	}
}

void GenGisFrame::OnWindowClose( wxCloseEvent& event )
{
	if ( event.CanVeto() && !App::Inst().IsSaveUpToDate() )
	{
		int answer = wxMessageBox( wxT( "Would you like to save your changes?" ),
			wxT( "GenGIS" ), wxYES_NO | wxCANCEL );

		if ( answer == wxYES )
		{
			if ( !SaveSession( App::Inst().GetSessionFullPath().IsEmpty() ) )
			{
				event.Veto();
				return; // Return to the current session if the user hits
				//'no' or 'cancel' within the SAVE session dialog
			}
		}

		else if ( answer == wxCANCEL )
		{
			event.Veto();
			return;
		}
	}
	event.Skip(); // or call Destroy() here instead
}

void GenGisFrame::OnFileExit( wxCommandEvent& event )
{
	if ( !App::Inst().IsSaveUpToDate() )
	{
		int answer = wxMessageBox( wxT( "Would you like to save your changes?" ),
			wxT( "GenGIS" ), wxYES_NO | wxCANCEL );

		if ( answer == wxYES )
		{
			if ( !SaveSession( App::Inst().GetSessionFullPath().IsEmpty() ) )
			{
				return; // Return to the current session if the user hits
				//'no' or 'cancel' within the SAVE session dialog
			}
		}
		else if ( answer == wxCANCEL )
		{
			return;
		}
	}

	Close( true );
}

void GenGisFrame::OnViewReset( wxCommandEvent& event )
{
	ViewReset();
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::ViewReset()
{ 
	if(App::Inst().GetMapController()->IsLoaded())
	{
		App::Inst().GetViewport()->GetCamera()->ResetView(); 
		m_widgetController->SetCompassAngle(App::Inst().GetViewport()->GetCamera()->GetYaw());	
		App::Inst().GetMapController()->GetMapView()->RefreshQuadtree();
		App::Inst().GetViewport()->Refresh(false);
	}
}

void GenGisFrame::ViewTop()
{ 
	if(App::Inst().GetMapController()->IsLoaded())
	{
		App::Inst().GetViewport()->GetCamera()->TopView(); 
		m_widgetController->SetCompassAngle(App::Inst().GetViewport()->GetCamera()->GetYaw());
		App::Inst().GetMapController()->GetMapView()->RefreshQuadtree();
		App::Inst().GetViewport()->Refresh(false); 
	}
}

void GenGisFrame::OnViewTop( wxCommandEvent& event )
{
	ViewTop();
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::OnViewShowToolbarText( wxCommandEvent& event )
{
	m_toolBar->ToggleWindowStyle( wxTB_TEXT );
	m_toolbarTextStatus = !m_toolbarTextStatus;
	App::Inst().SaveUserPreferences();

	// Make the frame "resize" itself to the current size, to force a Fit() call to all sizers using the current size.
	this->SetSize(wxDefaultCoord, wxDefaultCoord, wxDefaultCoord, wxDefaultCoord, wxSIZE_USE_EXISTING);
	if( m_toolbarTextStatus)
	{
		m_mnuView->Check(ID_MNU_FULL_SCREEN, false);
		m_toolbarFullScreenStatus=false;
	}
	else if(!m_splitterHorizontal->IsSplit() && !m_splitterVertical->IsSplit() && !m_viewNavStatus && !m_viewMinimapStatus && !m_viewCompassStatus)
	{
		m_toolbarFullScreenStatus=true;
		m_mnuView->Check(ID_MNU_FULL_SCREEN, true);
		
	}
	App::Inst().GetViewport()->Refresh(false);
}

void GenGisFrame::SetToolbarTextStatus( bool newStatus )
{
	m_toolbarTextStatus = newStatus;

	if (!m_toolbarTextStatus)
	{
		m_toolBar->ToggleWindowStyle( wxTB_TEXT );
		m_menubar->Check( ID_MNU_VIEW_TOOLBARTEXT, false );		
		this->UpdateWindowUI();
	}
	else
	{
		m_mnuView->Check(ID_MNU_FULL_SCREEN, false);
		m_toolbarFullScreenStatus=false;
	}
}

void GenGisFrame::OnShowWelcomeScreenOnStartupDlg( wxCommandEvent& event )
{
	m_showWelcomeDialog = !m_showWelcomeDialog;
	App::Inst().SaveUserPreferences();
}

void GenGisFrame::SetWelcomeDialogStatus( bool newStatus )
{
	m_showWelcomeDialog = newStatus;
	m_mnuSettings->Check( ID_MNU_SHOW_WELCOME, newStatus );
}

wxArrayString GenGisFrame::GetRecentSessionList()
{
	wxArrayString recentSessionList;

	foreach ( const std::string& sessionPath, m_recentSessions )
	{
		wxString sessionPathWX( sessionPath.c_str(), wxConvUTF8 );
		recentSessionList.Add( sessionPathWX );
	}

	return recentSessionList;
}

void GenGisFrame::SetSidePanelVisibility(bool bVisible)
{
	if(!bVisible && m_splitterVertical->IsSplit())
	{
		m_splitterVertical->Unsplit(m_pnlSidebar);
		m_verSplitterPos = m_splitterVertical->GetSashPosition();
		m_mnuView->Check(ID_MNU_VIEW_SIDE_PANEL, false);
		if(!m_splitterHorizontal->IsSplit() && !m_toolbarTextStatus && !m_viewNavStatus && !m_viewMinimapStatus && !m_viewCompassStatus)
		{
			m_toolbarFullScreenStatus=true;
			m_mnuView->Check(ID_MNU_FULL_SCREEN, true);
		}
	}
	else if(bVisible && !m_splitterVertical->IsSplit())
	{
		// m_splitterHorizontal->GetSashPosition() doesn't work in v2.9 of wxWidgets
		// so just set the side panel to a default size
		m_splitterVertical->SplitVertically(m_pnlSidebar, m_pnlViewport, 200);	//m_verSplitterPos);
		m_mnuView->Check(ID_MNU_VIEW_SIDE_PANEL, true);
		m_toolbarFullScreenStatus=false;
	}
}


void GenGisFrame::OnViewFullScreen(wxCommandEvent& event)
{
	if (!m_toolbarFullScreenStatus)
	{
		if(m_toolbarTextStatus)
		{
			m_toolBar->ToggleWindowStyle( wxTB_TEXT );
			m_toolbarTextStatus = false;
			m_mnuView->Check(ID_MNU_VIEW_TOOLBARTEXT, false);
			//App::Inst().SaveUserPreferences();		
			// Make the frame "resize" itself to the current size, to force a Fit() call to all sizers using the current size.
			this->SetSize(wxDefaultCoord, wxDefaultCoord, wxDefaultCoord, wxDefaultCoord, wxSIZE_USE_EXISTING);
		}	
	
		if(m_splitterHorizontal->IsSplit()) 
		{
			m_splitterHorizontal->Unsplit(m_pnlBottom);
			m_horSplitterPos = m_splitterHorizontal->GetSashPosition();
			m_mnuView->Check(ID_MNU_VIEW_CONSOLE, false);
		}

		if(m_splitterVertical->IsSplit()) 
		{		
			m_splitterVertical->Unsplit(m_pnlSidebar);
			m_verSplitterPos = m_splitterVertical->GetSashPosition();
			m_mnuView->Check(ID_MNU_VIEW_SIDE_PANEL, false);
		}
			
		if(m_viewCompassStatus)
		{
			m_widgetController->SetCompassVisibility(false);
			m_viewCompassStatus=false;
			m_menubar->FindItem(ID_MNU_VIEW_COMPASS)->Check(false);
		}
		if(m_viewMinimapStatus)
		{
			m_widgetController->SetMiniMapVisibility(false);
			m_viewMinimapStatus=false;
			m_menubar->FindItem(ID_MNU_VIEW_MINIMAP)->Check(false);
			
		}
		if(m_viewNavStatus)
		{
			m_widgetController->SetNavVisibility(false);
			m_viewNavStatus=false;
			m_menubar->FindItem(ID_MNU_VIEW_NAV)->Check(false);
		}
		m_toolbarFullScreenStatus=true;
		m_mnuView->Check(ID_MNU_FULL_SCREEN, true);
		
	}
	else
	{
		m_toolBar->ToggleWindowStyle( wxTB_TEXT );
		m_toolbarTextStatus = true;
		m_mnuView->Check(ID_MNU_VIEW_TOOLBARTEXT, true);
		//App::Inst().SaveUserPreferences();
		m_splitterVertical->SplitVertically(m_pnlSidebar, m_pnlViewport, 200);	//m_verSplitterPos);
		m_mnuView->Check(ID_MNU_VIEW_SIDE_PANEL, true);
		m_splitterHorizontal->SplitHorizontally(m_pnlTop, m_pnlBottom, App::Inst().GetViewport()->GetCamera()->GetHeight() - 200); //m_horSplitterPos);
		m_mnuView->Check(ID_MNU_VIEW_CONSOLE, true);
		m_mnuView->Check(ID_MNU_FULL_SCREEN, false);
		m_toolbarFullScreenStatus=false;
		// Make the frame "resize" itself to the current size, to force a Fit() call to all sizers using the current size.
		this->SetSize(wxDefaultCoord, wxDefaultCoord, wxDefaultCoord, wxDefaultCoord, wxSIZE_USE_EXISTING);	

		m_widgetController->SetCompassVisibility(true);
		m_widgetController->SetMiniMapVisibility(true);	
		m_widgetController->SetNavVisibility(true);
		m_menubar->FindItem(ID_MNU_VIEW_COMPASS)->Check(true);
		m_menubar->FindItem(ID_MNU_VIEW_MINIMAP)->Check(true);
		m_menubar->FindItem(ID_MNU_VIEW_NAV)->Check(true);

		m_viewCompassStatus=true;
		m_viewNavStatus=true;
		m_viewMinimapStatus=true;
	}
		
	App::Inst().GetViewport()->Refresh(true);
}


void GenGisFrame::OnViewSidePanel( wxCommandEvent& event )
{
	if(m_splitterVertical->IsSplit())
	{
		m_splitterVertical->Unsplit(m_pnlSidebar);
		m_verSplitterPos = m_splitterVertical->GetSashPosition();
		m_mnuView->Check(ID_MNU_VIEW_SIDE_PANEL, false);
		if(!m_splitterHorizontal->IsSplit() && !m_toolbarTextStatus && !m_viewNavStatus && !m_viewMinimapStatus && !m_viewCompassStatus)
		{
			m_toolbarFullScreenStatus=true;
			m_mnuView->Check(ID_MNU_FULL_SCREEN, true);
		}
	}
	else
	{
		// m_splitterHorizontal->GetSashPosition() doesn't work in v2.9 of wxWidgets
		// so just set the side panel to a default size
		m_splitterVertical->SplitVertically(m_pnlSidebar, m_pnlViewport, 200);	//m_verSplitterPos);
		m_mnuView->Check(ID_MNU_VIEW_SIDE_PANEL, true);
		m_mnuView->Check(ID_MNU_FULL_SCREEN, false);
		m_toolbarFullScreenStatus=false;
	}

}

void GenGisFrame::SetConsoleVisibility(bool bVisible)
{
	if(!bVisible && m_splitterHorizontal->IsSplit())
	{
		m_splitterHorizontal->Unsplit(m_pnlBottom);
		m_horSplitterPos = m_splitterHorizontal->GetSashPosition();
		m_mnuView->Check(ID_MNU_VIEW_CONSOLE, false);
		if(!m_splitterVertical->IsSplit() && !m_toolbarTextStatus && !m_viewNavStatus && !m_viewMinimapStatus && !m_viewCompassStatus)
		{
			m_toolbarFullScreenStatus=true;
			m_mnuView->Check(ID_MNU_FULL_SCREEN, true);
		}
	}
	else if(bVisible && !m_splitterHorizontal->IsSplit())
	{
		// m_splitterHorizontal->GetSashPosition() doesn't work in v2.9 of wxWidgets
		// so just set the console to a default size
		m_splitterHorizontal->SplitHorizontally(m_pnlTop, m_pnlBottom, App::Inst().GetViewport()->GetCamera()->GetHeight() - 200); //m_horSplitterPos);
		m_mnuView->Check(ID_MNU_VIEW_CONSOLE, true);
		m_toolbarFullScreenStatus=false;
	}
}

void GenGisFrame::OnViewConsole(wxCommandEvent& event)
{
	if(m_splitterHorizontal->IsSplit())
	{
		m_splitterHorizontal->Unsplit(m_pnlBottom);
		m_horSplitterPos = m_splitterHorizontal->GetSashPosition();
		m_mnuView->Check(ID_MNU_VIEW_CONSOLE, false);
		if(!m_splitterVertical->IsSplit() && !m_toolbarTextStatus && !m_viewNavStatus && !m_viewMinimapStatus && !m_viewCompassStatus)
		{
			m_toolbarFullScreenStatus=true;
			m_mnuView->Check(ID_MNU_FULL_SCREEN, true);
		}
	
	}		
	else
	{
		// m_splitterHorizontal->GetSashPosition() doesn't work in v2.9 of wxWidgets
		// so just set the console to a default size
		m_splitterHorizontal->SplitHorizontally(m_pnlTop, m_pnlBottom, App::Inst().GetViewport()->GetCamera()->GetHeight() - 200); //m_horSplitterPos);
		m_mnuView->Check(ID_MNU_VIEW_CONSOLE, true);
		m_mnuView->Check(ID_MNU_FULL_SCREEN, false);
		m_toolbarFullScreenStatus=false;
		
	}
}

void GenGisFrame::OnViewCompass( wxCommandEvent& event )
{
	m_viewCompassStatus=!m_viewCompassStatus;
	if(m_viewCompassStatus)
	{
		m_toolbarFullScreenStatus=false;
		m_mnuView->Check(ID_MNU_FULL_SCREEN, false);
	}
	else if(!m_splitterHorizontal->IsSplit() && !m_splitterVertical->IsSplit() && !m_toolbarTextStatus && !m_viewNavStatus && !m_viewMinimapStatus)
	{
		m_toolbarFullScreenStatus=true;
		m_mnuView->Check(ID_MNU_FULL_SCREEN, true);
	}
	m_widgetController->ToggleCompassVisibility();
}

void GenGisFrame::OnViewMiniMap( wxCommandEvent& event )
{
	m_viewMinimapStatus=!m_viewMinimapStatus;
	if(m_viewMinimapStatus)
	{
		m_toolbarFullScreenStatus=false;
		m_mnuView->Check(ID_MNU_FULL_SCREEN, false);
	}
	else if(!m_splitterHorizontal->IsSplit() && !m_splitterVertical->IsSplit() && !m_toolbarTextStatus && !m_viewNavStatus && !m_viewCompassStatus)
	{
		m_toolbarFullScreenStatus=true;
		m_mnuView->Check(ID_MNU_FULL_SCREEN, true);
	}
	m_widgetController->ToggleMiniMapVisibility();
}


void GenGisFrame::OnViewNav( wxCommandEvent& event )
{
	m_viewNavStatus=!m_viewNavStatus;
	if(m_viewNavStatus)
	{
		m_toolbarFullScreenStatus=false;
		m_mnuView->Check(ID_MNU_FULL_SCREEN, false);
	}
	else if(!m_splitterHorizontal->IsSplit() && !m_splitterVertical->IsSplit() && !m_toolbarTextStatus && !m_viewMinimapStatus && !m_viewCompassStatus)
	{
		m_toolbarFullScreenStatus=true;
		m_mnuView->Check(ID_MNU_FULL_SCREEN, true);
	}
	m_widgetController->ToggleNavVisibility();
}

void GenGisFrame::OnViewHideWidgets( wxCommandEvent& event )
{
	m_widgetController->SetCompassVisibility(false);
	m_widgetController->SetMiniMapVisibility(false);
	m_widgetController->SetNavVisibility(false);

	m_menubar->FindItem(ID_MNU_VIEW_COMPASS)->Check(false);
	m_menubar->FindItem(ID_MNU_VIEW_MINIMAP)->Check(false);
	m_menubar->FindItem(ID_MNU_VIEW_NAV)->Check(false);

	m_viewCompassStatus=false;
	m_viewNavStatus=false;
	m_viewMinimapStatus=false;

	if (!m_splitterHorizontal->IsSplit() && !m_splitterVertical->IsSplit() && !m_toolbarTextStatus)
	{
		m_toolbarFullScreenStatus=true;
		m_mnuView->Check(ID_MNU_FULL_SCREEN, true);
	}
}

void GenGisFrame::OnViewShowWidgets( wxCommandEvent& event )
{	
	m_widgetController->SetCompassVisibility(true);
	m_widgetController->SetMiniMapVisibility(true);
	m_widgetController->SetNavVisibility(true);

	m_menubar->FindItem(ID_MNU_VIEW_COMPASS)->Check(true);
	m_menubar->FindItem(ID_MNU_VIEW_MINIMAP)->Check(true);
	m_menubar->FindItem(ID_MNU_VIEW_NAV)->Check(true);

	m_viewCompassStatus=true;
	m_viewNavStatus=true;
	m_viewMinimapStatus=true;
	m_toolbarFullScreenStatus=false;
	m_mnuView->Check(ID_MNU_FULL_SCREEN, false);
}

void GenGisFrame::OnViewSelectSeqDlg( wxCommandEvent& event )
{
	ViewSelectSegDlg();
}

void GenGisFrame::ViewSelectSegDlg()
{
	SeqSelectionDlg* dlg = new SeqSelectionDlg(App::Inst().GetMainWindow(), m_locationSetLayer);
	dlg->Show();	
}

void GenGisFrame::OnLightingSettingsDlg( wxCommandEvent& event )
{
	LightingPropertiesDlg* dlg = new LightingPropertiesDlg(this);
	dlg->Show();
}

void GenGisFrame::OnFontSettingsDlg( wxCommandEvent& event )
{
	FontPropertiesDlg* dlg = new FontPropertiesDlg(this);
	dlg->Show();
}

void GenGisFrame::OnLayoutObjectSettingsDlg( wxCommandEvent& event )
{
	LayoutObjectPropertiesDlg* dlg = new LayoutObjectPropertiesDlg(this);
	dlg->Show();
}

void GenGisFrame::OnHelpManual( wxCommandEvent& event )
{
	wxLaunchDefaultBrowser( wxT( "http://kiwi.cs.dal.ca/GenGIS/The_GenGIS_2.1_Manual" ));
}

void GenGisFrame::OnHelpAbout( wxCommandEvent& event )
{
	AboutDlg* about = new AboutDlg( this );
	about->Show();
}

void GenGisFrame::OnCheckForUpdates( wxCommandEvent& event )
{
	wxString serverPath = wxT( "http://kiwi.research.cs.dal.ca/~GenGIS/gengis_dev/version.txt" );

	// Retrieve the file from the server containing the latest GenGIS release number
	wxFileSystem fsVersion;
	wxFSFile* versionFile = fsVersion.OpenFile( serverPath );
	if( versionFile == NULL )
	{
		wxMessageBox( wxT("Could not connect to server to check for updates."),
			wxT("Connection error"), wxOK | wxICON_INFORMATION );
		return;
	}
	
	// Store the version number in a string
	char releaseNumber[256];
	wxInputStream* versionStream = versionFile->GetStream();
	versionStream->Read( releaseNumber, 256 );
	versionFile->DetachStream();
	
	// If GenGIS is up-to-date, post a message and return
	if ( atoi( releaseNumber ) == App::GENGIS_VERSION )
	{
		wxMessageBox( wxT( "GenGIS is up-to-date"), wxT( "GenGIS is up-to-date!"), wxOK | wxICON_INFORMATION );
		return;
	}
	
	// Otherwise, bring up a window containing information about the latest release
	UpdateGenGISDlg* updateDialog = new UpdateGenGISDlg( this );
	updateDialog->Show();
}

void GenGisFrame::OnModeDrawLayoutLine( wxCommandEvent& event ) 
{ 
	if(!m_layoutLine)
	{
		App::Inst().SetMode(App::LAYOUT_LINE); 
		m_layoutLine.reset(new LayoutLine());
		App::Inst().SetSelectedView(m_layoutLine);
	}
	else
	{
		wxMessageBox(wxT("Can only have a single unassigned layout line active at a time."),
			wxT("Multiple layout lines"), wxOK | wxICON_INFORMATION);	
		m_toolBar->ToggleTool(ID_TB_MODE_DRAW_LAYOUT_LINE, false);	
	}
}

void GenGisFrame::OnModeDrawLayoutEllipse( wxCommandEvent& event ) 
{ 
	if(!m_layoutEllipse)
	{
		App::Inst().SetMode(App::LAYOUT_ELLIPSE); 
		m_layoutEllipse.reset(new LayoutEllipse());
		App::Inst().SetSelectedView(m_layoutEllipse);
	}
	else
	{
		wxMessageBox(wxT("Can only have a single unassigned layout ellipse active at a time."),
			wxT("Multiple layout ellipses"), wxOK | wxICON_INFORMATION);	
		m_toolBar->ToggleTool(ID_TB_MODE_DRAW_LAYOUT_ELLIPSE, false);	
	}
}

void GenGisFrame::OnGeoAxisByPolyline( wxCommandEvent& event ) 
{ 
	if(!m_geoAxisPolyline)
	{
		App::Inst().SetMode(App::GEO_AXIS_POLYLINE); 
		m_geoAxisPolyline.reset(new GeoAxisPolyline());
		App::Inst().SetSelectedView(m_geoAxisPolyline);

		m_statusBar->SetStatusText(_T("Press ENTER to finalize geographic axis"), 0);
	}
	else
	{
		wxMessageBox(wxT("Can only have a single unassigned geographic axis polyline active at a time."),
			wxT("Multiple geographic polylines"), wxOK | wxICON_INFORMATION);	
		m_toolBar->ToggleTool(ID_TB_GEO_AXIS_BY_POLYLINE, false);	
	}
}

void GenGisFrame::SigRender()
{	
	if ( !App::Inst().GetMapController() )
		return;

	App::Inst().GetLayerTreeController()->OnRender();

	UserElements::Inst().Render();
	LabelController::Inst().Render();

	if(m_layoutEllipse)
		m_layoutEllipse->Render();

	if(m_layoutLine)
		m_layoutLine->Render();

	if(m_geoAxisPolyline)
		m_geoAxisPolyline->Render();
}

void GenGisFrame::SigViewportKeyboard( wxKeyEvent& event )
{
	if(m_layoutLine && App::Inst().GetSelectedView() == m_layoutLine)
	{
		if(event.GetKeyCode() == WXK_ESCAPE)
		{
			if(App::Inst().GetMode() == App::LAYOUT_LINE)
			{
				wxCommandEvent dummy;
				OnLayoutLineDelete(dummy);
			}
		}
		if(event.GetKeyCode() == WXK_DELETE)
		{
			wxCommandEvent dummy;
			OnLayoutLineDelete(dummy);
		}
	}
	else if(m_layoutEllipse && App::Inst().GetSelectedView() == m_layoutEllipse)
	{
		m_layoutEllipse->KeyboardEvent(event);
		if(event.GetKeyCode() == WXK_ESCAPE)
		{
			if(App::Inst().GetMode() == App::LAYOUT_ELLIPSE)
			{
				wxCommandEvent dummy;
				OnLayoutEllipseDelete(dummy);
			}
		}
		if(event.GetKeyCode() == WXK_DELETE)
		{
			wxCommandEvent dummy;
			OnLayoutEllipseDelete(dummy);
		}
	}
	else if(m_geoAxisPolyline && App::Inst().GetSelectedView() == m_geoAxisPolyline)
	{
		if(event.GetKeyCode() == WXK_ESCAPE)
		{
			if(App::Inst().GetMode() == App::GEO_AXIS_POLYLINE)
			{
				wxCommandEvent dummy;
				OnGeoAxisPolylineDelete(dummy);
			}
		}
		if(event.GetKeyCode() == WXK_DELETE)
		{
			wxCommandEvent dummy;
			OnGeoAxisPolylineDelete(dummy);
		}
		if(event.GetKeyCode() == WXK_RETURN)
		{
			m_statusBar->SetStatusText(_T(""), 0);
			App::Inst().SetMode(App::STANDARD);
			m_toolBar->ToggleTool(ID_TB_GEO_AXIS_BY_POLYLINE, false);
			m_geoAxisPolyline->PolylineFinished();
		}
	}
	else if(App::Inst().GetMode() == App::USER_LABEL)
	{
		if(event.GetKeyCode() == WXK_ESCAPE)
		{
			App::Inst().SetMode(App::STANDARD);
			m_toolBar->ToggleTool(ID_TB_LABEL_TOOL, false);
		}
	}
	else if(App::Inst().GetSelectedView())
	{
		App::Inst().GetSelectedView()->KeyboardEvent(event);
	}
	else    // If there is no selected view, then the ViewPtr must be the MapView ViewPtr.
	{
		if(event.ShiftDown())    // Holding Shift down when using arrow keys zooms the map
		{
			if(event.GetKeyCode() == WXK_UP)    // Zoom in
			{
				App::Inst().GetViewport()->GetCamera()->ZoomDelta(-0.025f);
				App::Inst().GetViewport()->Refresh(false);
			}
			else if(event.GetKeyCode() == WXK_DOWN)
			{
				App::Inst().GetViewport()->GetCamera()->ZoomDelta(0.025f);
				App::Inst().GetViewport()->Refresh(false);
			}
		}
		else if(event.ControlDown())    // Control key rotates the map
		{
			if(event.GetKeyCode() == WXK_UP)
			{
				event.Skip();
				App::Inst().GetViewport()->GetCamera()->PitchDelta(-1.0f);
				App::Inst().GetViewport()->Refresh(false);
			}
			else if(event.GetKeyCode() == WXK_DOWN)
			{
				event.Skip();
				App::Inst().GetViewport()->GetCamera()->PitchDelta(1.0f);
				App::Inst().GetViewport()->Refresh(false);
			}
			else if(event.GetKeyCode() == WXK_LEFT)
			{
				event.Skip();
				App::Inst().GetViewport()->GetCamera()->YawDelta(1.0f);
				m_widgetController->SetCompassAngle(App::Inst().GetViewport()->GetCamera()->GetYaw());
				App::Inst().GetViewport()->Refresh(false);
			}
			else if(event.GetKeyCode() == WXK_RIGHT)
			{
				event.Skip();
				App::Inst().GetViewport()->GetCamera()->YawDelta(-1.0f);
				m_widgetController->SetCompassAngle(App::Inst().GetViewport()->GetCamera()->GetYaw());
				App::Inst().GetViewport()->Refresh(false);
			}
		}
		else    // No modifier key is held down, so the arrow keys pan the map
		{
			if(event.GetKeyCode() == WXK_UP)
			{
				event.Skip();
				App::Inst().GetViewport()->GetCamera()->TranslateDelta(0, -0.003f);
				App::Inst().GetViewport()->Refresh(false);
			}
			else if(event.GetKeyCode() == WXK_DOWN)
			{
				event.Skip();
				App::Inst().GetViewport()->GetCamera()->TranslateDelta(0, 0.003f);
				App::Inst().GetViewport()->Refresh(false);
			}
			else if(event.GetKeyCode() == WXK_LEFT)
			{
				event.Skip();
				App::Inst().GetViewport()->GetCamera()->TranslateDelta(-0.003f, 0);
				App::Inst().GetViewport()->Refresh(false);
			}
			else if(event.GetKeyCode() == WXK_RIGHT)
			{
				event.Skip();
				App::Inst().GetViewport()->GetCamera()->TranslateDelta(0.003f, 0);
				App::Inst().GetViewport()->Refresh(false);
			}
		}
		App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	}

	App::Inst().GetViewport()->Refresh(false);

	event.Skip();
}

void GenGisFrame::SigNodeUpdate(NodeInfo nodeInfo)
{
	if(nodeInfo.crossings == NOT_SET)
	{
		m_statusBar->SetStatusText(_T(""), 1);
	}
	else if(StringTools::IsEmpty(nodeInfo.name))
	{
		m_statusBar->SetStatusText(_T("Crossings below subtree: ") + wxString(StringTools::ToStringW(nodeInfo.crossings).c_str()), 1);
	}
	else
	{
		m_statusBar->SetStatusText(_T("Crossings below ") + wxString(nodeInfo.name.c_str()) + _T(": ") + wxString(StringTools::ToStringW(nodeInfo.crossings).c_str()), 1);
	}
}

void GenGisFrame::OnLayoutLineDelete(wxCommandEvent& event)
{
	if(App::Inst().GetSelectedView() == m_layoutLine)
		App::Inst().SetSelectedView(ViewPtr());

	App::Inst().SetMode(App::STANDARD);

	m_layoutLine.reset();
	m_toolBar->ToggleTool(ID_TB_MODE_DRAW_LAYOUT_LINE, false);	

	App::Inst().GetViewport()->Refresh(false);
}


void GenGisFrame::OnLayoutEllipseDelete(wxCommandEvent& event)
{
	if(App::Inst().GetSelectedView() == m_layoutEllipse)
		App::Inst().SetSelectedView(ViewPtr());

	App::Inst().SetMode(App::STANDARD);

	m_layoutEllipse.reset();
	m_toolBar->ToggleTool(ID_TB_MODE_DRAW_LAYOUT_ELLIPSE, false);

	App::Inst().GetViewport()->Refresh(false);
}

void GenGisFrame::OnGeoAxisPolylineDelete(wxCommandEvent& event)
{
	if(App::Inst().GetSelectedView() == m_geoAxisPolyline)
		App::Inst().SetSelectedView(ViewPtr());

	App::Inst().SetMode(App::STANDARD);

	m_geoAxisPolyline.reset();
	m_toolBar->ToggleTool(ID_TB_GEO_AXIS_BY_POLYLINE, false);	

	App::Inst().GetViewport()->Refresh(false);
}

void GenGisFrame::OnSelectedViewHide(wxCommandEvent& event)
{
	App::Inst().GetRightClickView()->SetVisibility(false);
	App::Inst().GetViewport()->Refresh();
}

void GenGisFrame::OnSelectedViewProperties(wxCommandEvent& event)
{
	App::Inst().GetRightClickView()->ShowPropertiesDlg();
	//App::Inst().GetViewport()->Refresh();
}

void GenGisFrame::OnSelectedViewRemove(wxCommandEvent& event)
{
	ViewPtr view = App::Inst().GetRightClickView();
	VisualLabelPtr visualLabel = boost::dynamic_pointer_cast<VisualLabel>(view);
	if(visualLabel)
	{		
		LabelController::Inst().RemoveLabel(visualLabel->GetId());
		App::Inst().GetViewport()->Refresh();
	}
}

void GenGisFrame::OnLayerRemove( wxCommandEvent& event ) 
{ 
	int answer;
	LayerPtr selectedLayer = App::Inst().GetLayerTreeController()->GetSelectedLayer();

	if(selectedLayer->GetType() == Layer::MAP && !App::Inst().GetLayerTreeController()->GetIsBlankRaster() && App::Inst().GetLayerTreeController()->GetNumVectorMapLayers() >0 )
		answer = wxMessageBox( wxT( "(Warning) By removing the raster file all other layers will be removed. Are you sure you want to remove this layer?" ), wxT( "Remove layer" ), wxYES_NO | wxCANCEL );
	else if(selectedLayer->GetType() == Layer::VECTOR_MAP && App::Inst().GetLayerTreeController()->GetIsBlankRaster() && App::Inst().GetLayerTreeController()->GetNumVectorMapLayers()==1 && App::Inst().GetLayerTreeController()->GetNumLocationSetLayers()>0  )
		answer = wxMessageBox( wxT( "(Warning) By removing the vector file all other layers will be removed. Are you sure you want to remove this layer?" ), wxT( "Remove layer" ), wxYES_NO | wxCANCEL );
	else
		answer = wxMessageBox( wxT( "Are you sure you want to remove this layer?" ), wxT( "Remove layer" ), wxYES_NO | wxCANCEL );

	if(answer == wxYES)
	{
		App::Inst().GetLayerTreeController()->OnLayerRemove(event);
		FillSamplesLegend();
		App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	}
}

void GenGisFrame::OnAllLayersRemove( wxCommandEvent& event ) 
{ 
	int answer = wxMessageBox( wxT( "Are you sure you want to remove All layers?" ), wxT( "Remove All layers" ), wxYES_NO | wxCANCEL );

	if(answer == wxYES)
	{
		App::Inst().GetLayerTreeController()->OnAllLayersRemove(event);

		App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	}
}


void GenGisFrame::OnLayerHideAll( wxCommandEvent& event ) 
{ 
	App::Inst().GetLayerTreeController()->OnHideAll(event); 
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::OnLayerShowAll( wxCommandEvent& event ) 
{ 
	App::Inst().GetLayerTreeController()->OnShowAll(event);
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::OnLayerTreeSelect( wxTreeEvent& event ) 
{ 
	App::Inst().GetLayerTreeController()->OnSelect(event); 
}

void GenGisFrame::OnLayerTreeClick( wxMouseEvent& event ) 
{ 
	App::Inst().GetLayerTreeController()->OnClick(event); 
}

void GenGisFrame::OnLayerTreeLeftDClick( wxMouseEvent& event )
{  
	App::Inst().GetLayerTreeController()->OnLeftDClick(event); 
}

void GenGisFrame::OnModeStandard( wxCommandEvent& event ) 
{ 
	App::Inst().SetMode(App::STANDARD);
}

void GenGisFrame::OnStudyProperties(wxCommandEvent& event)
{
	if ( m_studyLayer->HasPropertiesDialogOpen() )
	{
		m_studyLayer->GetPropertiesDialog()->SetFocus();
		m_studyLayer->GetPropertiesDialog()->Raise();
	}
	else
	{
		StudyPropertiesDlg* dlg = new StudyPropertiesDlg(App::Inst().GetMainWindow(), m_studyLayer);
		dlg->Show();

		App::Inst().GetLayerTreeController()->SetName( m_studyLayer, m_studyLayer->GetName() );
		m_studyLayer->SetPropertiesDialogOpenStatus( true );
	}
}

void GenGisFrame::OnMapProperties(wxCommandEvent& event)
{
	if ( m_mapLayer->HasPropertiesDialogOpen() )
	{
		m_mapLayer->GetPropertiesDialog()->SetFocus();
		m_mapLayer->GetPropertiesDialog()->Raise();
	}
	else
	{
		MapPropertiesDlg* dlg = new MapPropertiesDlg(this, m_mapLayer);
		dlg->Show( true );

		m_mapLayer->SetPropertiesDialogOpenStatus( true );
		dlg->Raise();
		dlg->SetFocus();
	}
}

void GenGisFrame::OnVectorMapProperties(wxCommandEvent& event)
{
	if ( m_vectorMapLayer->HasPropertiesDialogOpen() )
	{
		m_vectorMapLayer->GetPropertiesDialog()->SetFocus();
		m_vectorMapLayer->GetPropertiesDialog()->Raise();
	}
	else
	{
		VectorMapPropertiesDlg* dlg = new VectorMapPropertiesDlg(this, m_vectorMapLayer);
		VectorMapControllerPtr vectorMapController= m_vectorMapLayer->GetVectorMapController();
		vFileMetaData* vHeader = vectorMapController->GetVectorMetaData();
		std::string type = vHeader->geometryType;
		if(type=="Point")
		{
			dlg->SetSubPage(VectorMapPropertiesDlg::POINT);
			dlg->DisablePolyLinePanel();
			dlg->DisablePolygonPanel();
		}
		else if (type=="Polyline")
		{
			dlg->SetSubPage(VectorMapPropertiesDlg::POLYLINE);
			dlg->DisablePointPanel();
			dlg->DisablePolygonPanel();
		}
		else if(type=="Polygon")
		{
			dlg->SetSubPage(VectorMapPropertiesDlg::POLYGON);
			dlg->DisablePolyLinePanel();
			dlg->DisablePointPanel();
		}
		else
			dlg->SetPage(VectorMapPropertiesDlg::GENERAL);
		dlg->Show( true );

		m_vectorMapLayer->SetPropertiesDialogOpenStatus( true );
		dlg->Raise();
		dlg->SetFocus();
	}
}

void GenGisFrame::OnLocationSetProperties(wxCommandEvent& event)
{
	if ( m_locationSetLayer->HasPropertiesDialogOpen() )
	{
		m_locationSetLayer->GetPropertiesDialog()->SetFocus();
		m_locationSetLayer->GetPropertiesDialog()->Raise();
	}
	else
	{
		LocationSetPropertiesDlg* dlg = new LocationSetPropertiesDlg(App::Inst().GetMainWindow(), m_locationSetLayer);
		dlg->Show();

		App::Inst().GetLayerTreeController()->SetName( m_locationSetLayer, m_locationSetLayer->GetName() );
		m_locationSetLayer->SetPropertiesDialogOpenStatus( true );
	}
}

void GenGisFrame::OnLocationProperties(wxCommandEvent& event)
{
	if ( m_locationLayer->HasPropertiesDialogOpen() )
	{
		m_locationLayer->GetPropertiesDialog()->SetFocus();
		m_locationLayer->GetPropertiesDialog()->Raise();
	}
	else
	{
		LocationPropertiesDlg* dlg = new LocationPropertiesDlg(App::Inst().GetMainWindow(), m_locationLayer);
		dlg->Show();

		App::Inst().GetLayerTreeController()->SetName( m_locationLayer, m_locationLayer->GetName() );
		m_locationLayer->SetPropertiesDialogOpenStatus( true );
	}
}

void GenGisFrame::OnSequenceProperties(wxCommandEvent& event)
{
	if ( m_sequenceLayer->HasPropertiesDialogOpen() )
	{
		m_sequenceLayer->GetPropertiesDialog()->SetFocus();
		m_sequenceLayer->GetPropertiesDialog()->Raise();
	}
	else
	{
		SequencePropertiesDlg* dlg = new SequencePropertiesDlg(App::Inst().GetMainWindow(), m_sequenceLayer);
		dlg->Show();

		App::Inst().GetLayerTreeController()->SetName( m_sequenceLayer, m_sequenceLayer->GetName() );
		m_sequenceLayer->SetPropertiesDialogOpenStatus( true );
	}
}

void GenGisFrame::OnTreeProperties(wxCommandEvent& event)
{
	if ( m_treeLayer->HasPropertiesDialogOpen() )
	{
		m_treeLayer->GetPropertiesDialog()->SetFocus();
		m_treeLayer->GetPropertiesDialog()->Raise();
	}
	else
	{
		TreePropertiesDlg* dlg = new TreePropertiesDlg(App::Inst().GetMainWindow(), m_treeLayer);
		dlg->Show();

		App::Inst().GetLayerTreeController()->SetName( m_treeLayer, m_treeLayer->GetName() );
		m_treeLayer->SetPropertiesDialogOpenStatus( true );
	}
}

void GenGisFrame::OnCladogram3D( wxCommandEvent& event ) 
{ 
	App::Inst().GetLayerTreeController()->OnLayoutTree(GeoTreeView::CLADOGRAM_3D, LayoutLinePtr());
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::OnSlantedCladogram3D( wxCommandEvent& event )  
{ 
	App::Inst().GetLayerTreeController()->OnLayoutTree(GeoTreeView::SLANTED_CLADOGRAM_3D, LayoutLinePtr());
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::OnSlantedPhylogram3D( wxCommandEvent& event )  
{ 
	App::Inst().GetLayerTreeController()->OnLayoutTree(GeoTreeView::SLANTED_PHYLOGRAM_3D, LayoutLinePtr());
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::OnCladogram2D( wxCommandEvent& event ) 
{ 
	if(App::Inst().GetLayerTreeController()->OnLayoutTree(GeoTreeView::CLADOGRAM_2D, m_layoutLine))
	{
		m_layoutLine.reset();
	}
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::OnPhylogram2D( wxCommandEvent& event ) 
{ 
	if(App::Inst().GetLayerTreeController()->OnLayoutTree(GeoTreeView::PHYLOGRAM_2D, m_layoutLine))
	{
		m_layoutLine.reset();
	}
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::OnSlantedCladogram2D( wxCommandEvent& event )  
{ 
	if(App::Inst().GetLayerTreeController()->OnLayoutTree(GeoTreeView::SLANTED_CLADOGRAM_2D, m_layoutLine))
	{
		m_layoutLine.reset();
	}
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::OnLayoutTreeEllipseQuadrant( wxCommandEvent& event )
{
	if(App::Inst().GetLayerTreeController()->OnLayoutTree(GeoTreeView::ELLIPSE_QUADRANT, m_layoutEllipse))
	{
		m_layoutEllipse.reset();
	}
}

void GenGisFrame::OnTreeLayoutLineVisibility( wxCommandEvent& event )  
{ 
	App::Inst().GetLayerTreeController()->OnTreeLayoutLineVisibility(); 
}

void GenGisFrame::OnTreeFlip( wxCommandEvent& event )  
{ 
	App::Inst().GetLayerTreeController()->OnTreeFlip(); 
}

void GenGisFrame::OnTreeLayoutEllipseVisibility( wxCommandEvent& event )  
{ 
	App::Inst().GetLayerTreeController()->OnTreeLayoutEllipseVisibility(); 
}

void GenGisFrame::OnTreeGeographicAxisVisibility( wxCommandEvent& event )  
{ 
	App::Inst().GetLayerTreeController()->OnTreeGeographicAxisVisibility(); 
}

void GenGisFrame::OnAssignGeoAxisToTree( wxCommandEvent& event )  
{
	if(App::Inst().GetLayerTreeController()->OnAssignGeoAxisToTree(m_geoAxisPolyline))
	{
		m_geoAxisPolyline.reset();
	}
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void GenGisFrame::OnReverseGeoAxisOrder( wxCommandEvent& event )  
{
	App::Inst().GetLayerTreeController()->OnReverseGeoAxisOrder( event.IsChecked() );
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	App::Inst().GetViewport()->Refresh( false );
}

void GenGisFrame::OnPerformLinearAxesAnalysis( wxCommandEvent& event )  
{
	NodeGeoTree* rootNode = m_treeLayer->GetGeoTreeView()->GetRootNode();
	std::vector<LinearResults> results = m_treeLayer->GetGeoTreeView()->PerformLinearAxesAnalysis(rootNode);

	// show results of linear analysis
	if(results.size() > 0)
	{
		LinearAnalysisPlotDlg* dlg = new LinearAnalysisPlotDlg(App::Inst().GetMainWindow(), 
																															m_treeLayer, results, rootNode);
		dlg->Show();
	}
}

void GenGisFrame::OnPerformLinearAxesAnalysisPopup( wxCommandEvent& event )  
{
	TreeLayerPtr treeLayer = GetSelectedTree();

	NodeGeoTree* selectedNode = treeLayer->GetGeoTreeView()->GetSelectedNode();
	std::vector<LinearResults> results = treeLayer->GetGeoTreeView()->PerformLinearAxesAnalysis(selectedNode);

	// show results of linear analysis
	if(results.size() > 0)
	{
		LinearAnalysisPlotDlg* dlg = new LinearAnalysisPlotDlg(App::Inst().GetMainWindow(), 
																														treeLayer, results, selectedNode);
		dlg->Show();
	}
}

void GenGisFrame::OnShowNonlinearAxisTable( wxCommandEvent& event )
{
	NonlinearAxisTableDlg* dlg = new NonlinearAxisTableDlg(App::Inst().GetMainWindow(), m_treeLayer);
	dlg->Show();
}

void GenGisFrame::OnShowNonLinearAxisTablePopup( wxCommandEvent& event )
{
	TreeLayerPtr treeLayer = GetSelectedTree();

	NonlinearAxisTableDlg* dlg = new NonlinearAxisTableDlg(App::Inst().GetMainWindow(), treeLayer);
	dlg->Show();

}

void GenGisFrame::OnLeafNodeLabels( wxCommandEvent& event )  
{ 
	App::Inst().GetLayerTreeController()->OnLeafNodeLabels(); 
}

void GenGisFrame::OnLayoutChartsEvenlyOnLine(wxCommandEvent& event)
{
	if(App::Inst().GetLayerTreeController()->OnLayoutCharts(ChartSetView::LAYOUT_LINE_EVEN_SPACING, m_layoutLine))
	{
		m_layoutLine.reset();
	}
}

void GenGisFrame::OnLayoutChartsLineProjected(wxCommandEvent& event)
{
	if(App::Inst().GetLayerTreeController()->OnLayoutCharts(ChartSetView::LAYOUT_LINE_PROJECTED, m_layoutLine))
	{
		m_layoutLine.reset();
	}
}

void GenGisFrame::OnLayoutChartsEllipseQuadrant(wxCommandEvent& event)
{
	if(App::Inst().GetLayerTreeController()->OnLayoutCharts(ChartSetView::ELLIPSE_QUADRANT, m_layoutEllipse))
	{
		m_layoutEllipse.reset();
	}
}

void GenGisFrame::OnLayoutChartsEvenlyOnEllipse(wxCommandEvent& event)
{
	if(App::Inst().GetLayerTreeController()->OnLayoutCharts(ChartSetView::ELLIPSE_EVEN_SPACING, m_layoutEllipse))
	{
		m_layoutEllipse.reset();
	}
}

void GenGisFrame::OnLayoutChartsEllipseProjected(wxCommandEvent& event)
{
	if(App::Inst().GetLayerTreeController()->OnLayoutCharts(ChartSetView::ELLIPSE_CLOSEST_PT, m_layoutEllipse))
	{
		m_layoutEllipse.reset();
	}
}

void GenGisFrame::OnLayoutChartsDefaultPosition(wxCommandEvent& event)
{
	App::Inst().GetLayerTreeController()->OnLayoutCharts(ChartSetView::DEFAULT);
}

void GenGisFrame::OnLayoutChartsOnTreeLeaves(wxCommandEvent& event)
{
	if (App::Inst().GetLayerTreeController()->GetNumTreeLayers() > 0)
	{
		App::Inst().GetLayerTreeController()->OnLayoutCharts(ChartSetView::LAYOUT_TREE_LEAVES);
	}
}

void GenGisFrame::OnChartSetLayoutLine( wxCommandEvent& event )  
{ 
	App::Inst().GetLayerTreeController()->OnChartSetLayoutLine(); 
}

void GenGisFrame::OnChartSetLayoutEllipse( wxCommandEvent& event )  
{ 
	App::Inst().GetLayerTreeController()->OnChartSetLayoutEllipse(); 
}

void GenGisFrame::OnRAutoConfigurationUtility( wxCommandEvent& event )
{
	#ifndef _DEBUG
	bool successConfiguringR = GenGIS::PluginManager::ConfigureRInstallation();
	if ( successConfiguringR )
		wxMessageBox( wxT( "R is properly configured." ) );
	else
		wxMessageBox( wxT( "R is not configured." ) );
	#endif
}

void GenGisFrame::OnTreeSignificanceTest( wxCommandEvent& event )
{
	std::map<uint, uint> pdf;
	uint originalNumberCrossings;
	bool bSuccess;
	NodeGeoTree* rootNode = m_treeLayer->GetGeoTreeView()->GetRootNode();
	double pValue = m_treeLayer->GetGeoTreeView()->SignificanceTest(1000, pdf, originalNumberCrossings, rootNode, bSuccess);

	if(bSuccess && pdf.size() > 0)
	{
		Log::Inst().Write("Random permutation test (1000 iterations), p-value = " + StringTools::ToString(pValue));
		MonteCarloPlotDlg* dlg = new MonteCarloPlotDlg(App::Inst().GetMainWindow(), m_treeLayer, pdf, originalNumberCrossings, pValue);
		dlg->Show();
	}
}

void GenGisFrame::OnTreeSignificanceTestPopup( wxCommandEvent& event )
{
	TreeLayerPtr treeLayer = GetSelectedTree();

	std::map<uint, uint> pdf;
	uint originalNumberCrossings;
	bool bSuccess;
	NodeGeoTree* selectedNode = treeLayer->GetGeoTreeView()->GetSelectedNode();
	double pValue = treeLayer->GetGeoTreeView()->SignificanceTest(1000, pdf, originalNumberCrossings, selectedNode, bSuccess);

	if(bSuccess && pdf.size() > 0)
	{
		Log::Inst().Write("Random permutation test (1000 iterations), p-value for subtree = " + StringTools::ToString(pValue));
		MonteCarloPlotDlg* dlg = new MonteCarloPlotDlg(App::Inst().GetMainWindow(), treeLayer, pdf, originalNumberCrossings, pValue);
		dlg->Show();
	}
}

void GenGisFrame::OnExtendGeographicPolyline( wxCommandEvent& event )
{
	if(App::Inst().GetRightClickView() == m_geoAxisPolyline)
	{
		App::Inst().SetMode(App::GEO_AXIS_POLYLINE); 
		m_geoAxisPolyline->ExtendGeographicAxis();
		App::Inst().SetSelectedView(m_geoAxisPolyline);
	}
	else
	{
		GetSelectedTree()->GetGeoTreeView()->ExtendGeographicAxis();
		App::Inst().SetSelectedView(GetSelectedTree()->GetGeoTreeView());
	}
}

void GenGisFrame::OnLayerTreePopupMenu( wxTreeEvent& event )
{
	event.Skip();

	// cause the tree item under the cursor to become selected
	wxTreeItemId id = event.GetItem();

	if(id.IsOk())
	{
		App::Inst().GetLayerTreeController()->GetTreeCtrl()->SelectItem(id);

		LayerTreeItem* layerItem = (LayerTreeItem*)App::Inst().GetLayerTreeController()->GetTreeCtrl()->GetItemData(id);
		LayerPtr layer = layerItem->GetLayer();

		wxMenu menu;
		if(layer->GetType() == Layer::STUDY)
		{
			m_studyLayer = boost::dynamic_pointer_cast<StudyLayer>(layer);
			menu.Append(ID_MNU_LAYER_STUDY_PROPERTIES, wxT("Properties"));
		}
		else if(layer->GetType() == Layer::MAP && !App::Inst().GetLayerTreeController()->GetIsBlankRaster())
		{
			m_mapLayer = boost::dynamic_pointer_cast<MapLayer>(layer);

			menu.Append(ID_MNU_LAYER_MAP_PROPERTIES, wxT("Properties"));	
		}
		else if(layer->GetType() == Layer::VECTOR_MAP)
		{
			m_vectorMapLayer = boost::dynamic_pointer_cast<VectorMapLayer>(layer);

			menu.Append(ID_MNU_LAYER_VECTOR_MAP_PROPERTIES, wxT("Properties"));	

		}
		else if(layer->GetType() == Layer::LOCATION_SET)
		{
			m_locationSetLayer = boost::dynamic_pointer_cast<LocationSetLayer>(layer);
			menu.Append(ID_MNU_LAYER_LOCATION_SET_PROPERTIES, wxT("Properties"));

			bool bHasSampleData = false;
			for(unsigned int i = 0; i < m_locationSetLayer->GetNumLocationLayers(); ++i)
			{
				if(m_locationSetLayer->GetLocationLayer(i)->GetNumSequenceLayers() > 0)
				{
					bHasSampleData = true;
					break;
				}
			}

			if(bHasSampleData)
			{
				menu.AppendSeparator();
				menu.Append(ID_RIGHT_CLICK_LAYOUT_CHARTS_DEFAULT_POS, wxT("Layout charts over locations (default)"), wxEmptyString, wxITEM_CHECK);
				menu.AppendSeparator();
				menu.Append(ID_RIGHT_CLICK_LAYOUT_CHARTS_TREE_LEAVES, wxT("Layout pie charts to tree leaves"), wxEmptyString, wxITEM_CHECK);
				menu.AppendSeparator();

				menu.Append(ID_RIGHT_CLICK_LAYOUT_CHARTS_EVENLY_ON_LINE, wxT("Layout charts evenly on line"), wxEmptyString, wxITEM_CHECK);
				menu.Append(ID_RIGHT_CLICK_LAYOUT_CHARTS_LINE_PROJECTED, wxT("Project charts onto layout line"), wxEmptyString, wxITEM_CHECK);
				menu.AppendSeparator();
				menu.Append(ID_RIGHT_CLICK_LAYOUT_CHARTS_ELLIPSE_QUADRANT, wxT("Per ellipse quadrant layout (recommended)"), wxEmptyString, wxITEM_CHECK);
				menu.Append(ID_RIGHT_CLICK_LAYOUT_CHARTS_EVENLY_ON_ELLIPSE, wxT("Layout charts evenly on ellipse"), wxEmptyString, wxITEM_CHECK);
				menu.Append(ID_RIGHT_CLICK_LAYOUT_CHARTS_ELLIPSE_PROJECTED, wxT("Project charts onto layout ellipse"), wxEmptyString, wxITEM_CHECK);

				if(m_locationSetLayer->GetChartSetView()->IsLayoutLine())
				{
					menu.AppendSeparator();
					menu.Append(ID_RIGHT_CLICK_CHART_SET_LAYOUT_LINE, wxT("Show layout line"), wxEmptyString, wxITEM_CHECK);
					menu.Check(ID_RIGHT_CLICK_CHART_SET_LAYOUT_LINE, m_locationSetLayer->GetChartSetView()->GetLayoutLineVisibility());
				}
				else if(m_locationSetLayer->GetChartSetView()->IsLayoutEllipse())
				{
					menu.AppendSeparator();
					menu.Append(ID_RIGHT_CLICK_CHART_SET_LAYOUT_ELLIPSE, wxT("Show layout ellipse"), wxEmptyString, wxITEM_CHECK);
					menu.Check(ID_RIGHT_CLICK_CHART_SET_LAYOUT_ELLIPSE, m_locationSetLayer->GetChartSetView()->GetLayoutEllipseVisibility());
				}

				// mark current layout style in menu
				if(m_locationSetLayer->GetChartSetView()->GetLayout() == ChartSetView::LAYOUT_LINE_EVEN_SPACING)
				{
					menu.Check(ID_RIGHT_CLICK_LAYOUT_CHARTS_EVENLY_ON_LINE, true);
				}
				else if(m_locationSetLayer->GetChartSetView()->GetLayout() == ChartSetView::LAYOUT_LINE_PROJECTED)
				{
					menu.Check(ID_RIGHT_CLICK_LAYOUT_CHARTS_LINE_PROJECTED, true);
				}
				else if(m_locationSetLayer->GetChartSetView()->GetLayout() == ChartSetView::ELLIPSE_CLOSEST_PT)
				{
					menu.Check(ID_RIGHT_CLICK_LAYOUT_CHARTS_ELLIPSE_PROJECTED, true);
				}
				else if(m_locationSetLayer->GetChartSetView()->GetLayout() == ChartSetView::ELLIPSE_EVEN_SPACING)
				{
					menu.Check(ID_RIGHT_CLICK_LAYOUT_CHARTS_EVENLY_ON_ELLIPSE, true);
				}
				else if(m_locationSetLayer->GetChartSetView()->GetLayout() == ChartSetView::ELLIPSE_QUADRANT)
				{
					menu.Check(ID_RIGHT_CLICK_LAYOUT_CHARTS_ELLIPSE_QUADRANT, true);
				}
			}
		}
		else if(layer->GetType() == Layer::LOCATION)
		{
			m_locationLayer = boost::dynamic_pointer_cast<LocationLayer>(layer);
			menu.Append(ID_MNU_LAYER_LOCATION_PROPERTIES, wxT("Properties"));
		}
		else if(layer->GetType() == Layer::SEQUENCE)
		{
			m_sequenceLayer = boost::dynamic_pointer_cast<SequenceLayer>(layer);
			menu.Append(ID_MNU_LAYER_SEQUENCE_PROPERTIES, wxT("Properties"));
		}
		else if(layer->GetType() == Layer::TREE)
		{
			m_treeLayer = boost::dynamic_pointer_cast<TreeLayer>(layer);

			menu.Append(ID_MNU_LAYER_TREE_PROPERTIES, wxT("Properties"));
			menu.AppendSeparator();

			menu.Append(ID_RIGHT_CLICK_SLANTED_CLADOGRAM_3D, wxT("3D slanted cladogram"), wxEmptyString, wxITEM_CHECK);
			menu.Append(ID_RIGHT_CLICK_CLADOGRAM_3D, wxT("3D cladogram"), wxEmptyString, wxITEM_CHECK);
			menu.Append(ID_RIGHT_CLICK_SLANTED_PHYLOGRAM_3D, wxT("3D slanted phylogram"), wxEmptyString, wxITEM_CHECK);

			menu.AppendSeparator();
			menu.Append(ID_RIGHT_CLICK_SLANTED_CLADOGRAM_2D, wxT("2D slanted cladogram"), wxEmptyString, wxITEM_CHECK);
			menu.Append(ID_RIGHT_CLICK_CLADOGRAM_2D, wxT("2D cladogram"), wxEmptyString, wxITEM_CHECK);
			menu.Append(ID_RIGHT_CLICK_PHYLOGRAM_2D, wxT("2D phylogram"), wxEmptyString, wxITEM_CHECK);

			// DHP: Being removed until functionality is complete
			//menu.AppendSeparator();
			//menu.Append(ID_RIGHT_CLICK_LAYOUT_ELLIPSE_QUADRANT, wxT("Elliptical layout"), wxEmptyString, wxITEM_CHECK);

			menu.AppendSeparator();
			menu.Append(ID_RIGHT_CLICK_GEO_AXIS_TO_TREE, wxT("Assign geographic axis to tree"), wxEmptyString, wxITEM_CHECK);
			menu.Append(ID_RIGHT_CLICK_GEO_AXIS_REVERSE_ORDER, wxT("Reverse order of geographic points"), wxEmptyString, wxITEM_CHECK);
			menu.Check(ID_RIGHT_CLICK_GEO_AXIS_REVERSE_ORDER, m_treeLayer->GetGeoTreeView()->GetReverseOrderGeographicPts());

			menu.AppendSeparator();
			menu.Append(ID_RIGHT_CLICK_TREE_LEAF_NODE_LABELS, wxT("Show leaf node labels"), wxEmptyString, wxITEM_CHECK);
			menu.Check(ID_RIGHT_CLICK_TREE_LEAF_NODE_LABELS, m_treeLayer->GetGeoTreeView()->GetLabelVisibility());

			// check visibility of layout primatives
			if(m_treeLayer->GetGeoTreeView()->IsLayoutLine())
			{
				menu.AppendSeparator();
				menu.Append(ID_RIGHT_CLICK_TREE_FLIP, wxT("Flip tree"), wxEmptyString, wxITEM_CHECK);
				menu.Append(ID_RIGHT_CLICK_TREE_LAYOUT_LINE, wxT("Show layout line"), wxEmptyString, wxITEM_CHECK);
				menu.Check(ID_RIGHT_CLICK_TREE_LAYOUT_LINE, m_treeLayer->GetGeoTreeView()->GetLayoutLineVisibility());
			}
			else if(m_treeLayer->GetGeoTreeView()->IsLayoutEllipse())
			{
				menu.AppendSeparator();
				menu.Append(ID_RIGHT_CLICK_TREE_LAYOUT_ELLIPSE, wxT("Show layout ellipse"), wxEmptyString, wxITEM_CHECK);
				menu.Check(ID_RIGHT_CLICK_TREE_LAYOUT_ELLIPSE, m_treeLayer->GetGeoTreeView()->GetLayoutEllipseVisibility());
			}

			// check visibility of geographic axis
			if(m_treeLayer->GetGeoTreeView()->IsGeographicAxis())
			{
				menu.AppendSeparator();
				menu.Append(ID_RIGHT_CLICK_TREE_GEO_AXIS, wxT("Show geographical axis"), wxEmptyString, wxITEM_CHECK);
				menu.Check(ID_RIGHT_CLICK_TREE_GEO_AXIS, m_treeLayer->GetGeoTreeView()->GetGeographicAxisVisibility());
			}

			// mark current layout style in menu
			GeoTreeView::LAYOUT layout = m_treeLayer->GetGeoTreeView()->GetLayout();
			if(layout == GeoTreeView::SLANTED_CLADOGRAM_3D)
				menu.Check(ID_RIGHT_CLICK_SLANTED_CLADOGRAM_3D, true);
			else if(layout == GeoTreeView::CLADOGRAM_3D)
				menu.Check(ID_RIGHT_CLICK_CLADOGRAM_3D, true);
			else if(layout == GeoTreeView::SLANTED_PHYLOGRAM_3D)
				menu.Check(ID_RIGHT_CLICK_SLANTED_PHYLOGRAM_3D, true);
			else if(layout == GeoTreeView::SLANTED_CLADOGRAM_2D)
			{
				menu.Check(ID_RIGHT_CLICK_SLANTED_CLADOGRAM_2D, true);
			}
			else if(layout == GeoTreeView::CLADOGRAM_2D)
			{
				menu.Check(ID_RIGHT_CLICK_CLADOGRAM_2D, true);
			}
			else if(layout == GeoTreeView::PHYLOGRAM_2D)
			{
				menu.Check(ID_RIGHT_CLICK_PHYLOGRAM_2D, true);	
			}
			else if(layout == GeoTreeView::ELLIPSE_QUADRANT)
			{
				menu.Check(ID_RIGHT_CLICK_LAYOUT_ELLIPSE_QUADRANT, true);
			}

			if(layout == GeoTreeView::SLANTED_CLADOGRAM_2D || layout == GeoTreeView::CLADOGRAM_2D || layout == GeoTreeView::PHYLOGRAM_2D)
			{
				menu.AppendSeparator();
				menu.Append(ID_RIGHT_CLICK_TREE_SIGNIFICANCE_TEST, wxT("Perform significance test (on full tree)"), wxEmptyString, wxITEM_CHECK);
				menu.AppendSeparator();
				menu.Append(ID_RIGHT_CLICK_LINEAR_AXES_ANALYSIS, wxT("Perform linear axes analysis (on full tree)"), wxEmptyString, wxITEM_CHECK);


				if(m_treeLayer->GetGeoTreeView()->HasNonlinearAxis())
				{
					menu.AppendSeparator();
					menu.Append(ID_RIGHT_CLICK_SHOW_GEOGRAPHIC_AXIS_TABLE, wxT("Show nonlinear axis table"), wxEmptyString, wxITEM_CHECK);
				}
			}
		}

		App::Inst().GetMainWindow()->PopupMenu(&menu);
	}
}

void GenGisFrame::OnLabelTool( wxCommandEvent& event )
{
	App::Inst().SetMode(App::USER_LABEL); 
}

void GenGisFrame::OnConfigureDB( wxCommandEvent& event )
{
	ConfigureDatabaseDlg* dlg = new ConfigureDatabaseDlg(App::Inst().GetMainWindow());
	dlg->Show();	
}

void GenGisFrame::OnObtainDataFromDB( wxCommandEvent& event )
{
	DbSelectSampleDlg* dlg = new DbSelectSampleDlg(App::Inst().GetMainWindow());
	dlg->Show();	
}

bool GenGisFrame::MouseLeftDown(const Point2D& mousePt)
{
	m_statusBar->SetStatusText(_T(""), 0);

	if(App::Inst().GetMode() == App::STANDARD)
	{
		if(m_layoutLine && m_layoutLine->MouseLeftDown(mousePt))
		{
			App::Inst().SetSelectedView(m_layoutLine);
			return true;
		}

		if(m_layoutEllipse && m_layoutEllipse->MouseLeftDown(mousePt))
		{
			App::Inst().SetSelectedView(m_layoutEllipse);
			return true;
		}

		if(m_geoAxisPolyline && m_geoAxisPolyline->MouseLeftDown(mousePt))
		{
			App::Inst().SetSelectedView(m_geoAxisPolyline);
			return true;
		}

		if(m_widgetController->MouseLeftDown(mousePt))
			return true;

		if(App::Inst().GetLayerTreeController()->MouseLeftDown(mousePt))
			return true;

		if(LabelController::Inst().MouseLeftDown(mousePt))
			return true;
	}
	else if(App::Inst().GetMode() == App::LAYOUT_LINE)
	{
		App::Inst().SetSelectedView(m_layoutLine);
		m_layoutLine->MouseLeftDown(mousePt);
		if(m_layoutLine->GetStage() == LayoutLine::COMPLETE)
		{
			App::Inst().SetMode(App::STANDARD);
			m_toolBar->ToggleTool(ID_TB_MODE_DRAW_LAYOUT_LINE, false);
			return false;
		}

		return true;
	}
	else if(App::Inst().GetMode() == App::LAYOUT_ELLIPSE)
	{
		App::Inst().SetSelectedView(m_layoutEllipse);
		m_layoutEllipse->MouseLeftDown(mousePt);
		if(m_layoutEllipse->GetStage() == LayoutEllipse::COMPLETE)
		{
			App::Inst().SetMode(App::STANDARD);
			m_toolBar->ToggleTool(ID_TB_MODE_DRAW_LAYOUT_ELLIPSE, false);
			return false;
		}

		return true;
	}
	else if(App::Inst().GetMode() == App::GEO_AXIS_POLYLINE)
	{
		m_statusBar->SetStatusText(_T("Press ENTER to terminate geographic axis"), 0);

		App::Inst().SetSelectedView(m_geoAxisPolyline);
		m_geoAxisPolyline->MouseLeftDown(mousePt);
		if(m_geoAxisPolyline->GetStage() == GeoAxisPolyline::COMPLETE)
		{
			App::Inst().SetMode(App::STANDARD);
			m_toolBar->ToggleTool(ID_TB_GEO_AXIS_BY_POLYLINE, false);
			return false;
		}

		return true;
	}
	else if(App::Inst().GetMode() == App::USER_LABEL)
	{
		LabelToolDlg* dlg = new LabelToolDlg(this);
		dlg->SetCustomColourButton();
		Point3D labelPos = App::Inst().GetMapController()->ProjectToTerrain(App::Inst().GetMouseScreenPos(), false);
		if(dlg->ShowModal())
		{
			if(!dlg->GetText().IsEmpty() && dlg->SaveChanges())
			{
				VisualLabelPtr label(new VisualLabel());

				label->SetText(dlg->GetText().c_str());
				label->SetSize(dlg->GetSize());
				label->SetColour(Colour(dlg->GetColour()));
				label->SetGridPosition(labelPos);
				label->SetVisibility(true);
				label->SetLabelPosition(_T("<Custom>"));

				if(dlg->GetStyle() == _T("Billboard"))
					label->SetRotateLabel(false);
				else if(dlg->GetStyle() == _T("Dynamic"))
					label->SetRotateLabel(true);

				LabelController::Inst().AddLabel(label);

				App::Inst().GetViewport()->Refresh(false);
			}
		}

		App::Inst().SetMode(App::STANDARD);
		m_toolBar->ToggleTool(ID_TB_LABEL_TOOL, false);

		return false;
	}

	// nothing has been selected
	App::Inst().SetSelectedView(ViewPtr());

	return false;
}

void GenGisFrame::MouseLeftUp(const Point2D& mousePt)
{
	if(App::Inst().GetMode() == App::STANDARD && App::Inst().GetSelectedView())
		App::Inst().GetSelectedView()->MouseLeftUp(mousePt);
}

void GenGisFrame::MouseDragging(const Point2D& mousePt)
{
	if(App::Inst().GetMode() == App::STANDARD && App::Inst().GetSelectedView())
		App::Inst().GetSelectedView()->MouseDragging(mousePt);
}

void GenGisFrame::MouseLeftDoubleClick(const Point2D& mousePt)
{
	if(App::Inst().GetMode() == App::STANDARD && App::Inst().GetSelectedView())
		App::Inst().GetSelectedView()->MouseLeftDblClick(mousePt);
}

bool GenGisFrame::MouseRightDown(const Point2D& mousePt)
{
	wxMenu menu;
	LayerPtr layerPtr;

	if(App::Inst().GetMode() == App::STANDARD)
	{
		if(m_layoutLine && m_layoutLine->MouseRightDown(mousePt, menu))
		{
			App::Inst().SetRightClickView(m_layoutLine);

			// This is a bit of a hack. Basically, the LayoutLine has a popup menu
			// that makes sense once it has been associated with another object (like a tree).
			// Here, we want a popup menu for when the LayoutLine is still unassociated with 
			// any other object. As such, we define a different popup menu here than what is returned
			// by MouseRightDown.
			wxMenu menu;

			menu.Append(ID_MNU_LAYOUT_LINE_DELETE, wxT("Delete"));
			menu.AppendSeparator();
			menu.Append(ID_POPUP_MNU_PROPERTIES, wxT("Properties"));
			PopupMenu(&menu);

			return true;
		}
		else if(m_layoutEllipse && m_layoutEllipse->MouseRightDown(mousePt, menu))
		{
			App::Inst().SetRightClickView(m_layoutEllipse);

			// This is a bit of a hack. Basically, the LayoutEllipse has a popup menu
			// that makes sense once it has been associated with another object (like a tree).
			// Here, we want a popup menu for when the LayoutEllipse is still unassociated with 
			// any other object. As such, we define a different popup menu here than what is returned
			// by MouseRightDown.
			wxMenu menu;

			menu.Append(ID_MNU_LAYOUT_ELLIPSE_DELETE, wxT("Delete"));
			menu.AppendSeparator();
			menu.Append(ID_POPUP_MNU_PROPERTIES, wxT("Properties"));
			PopupMenu(&menu);

			return true;
		}
		else if(m_geoAxisPolyline && m_geoAxisPolyline->MouseRightDown(mousePt, menu))
		{
			App::Inst().SetRightClickView(m_geoAxisPolyline);
			PopupMenu(&menu);
			return true;
		}
		else if(LabelController::Inst().MouseRightDown(mousePt, menu))
		{
			PopupMenu(&menu);
			return true;
		}
		else if(App::Inst().GetLayerTreeController()->MouseRightDown(mousePt, menu, layerPtr))
		{
			PopupMenu(&menu);

			if(layerPtr->GetType() == Layer::STUDY)
				m_studyLayer = boost::dynamic_pointer_cast<StudyLayer>(layerPtr);
			else if(layerPtr->GetType() == Layer::MAP)
				m_mapLayer = boost::dynamic_pointer_cast<MapLayer>(layerPtr);
			else if(layerPtr->GetType() == Layer::VECTOR_MAP)
				m_vectorMapLayer = boost::dynamic_pointer_cast<VectorMapLayer>(layerPtr);
			else if(layerPtr->GetType() == Layer::LOCATION_SET)
				m_locationSetLayer = boost::dynamic_pointer_cast<LocationSetLayer>(layerPtr);
			else if(layerPtr->GetType() == Layer::LOCATION)
				m_locationLayer = boost::dynamic_pointer_cast<LocationLayer>(layerPtr);
			else if(layerPtr->GetType() == Layer::SEQUENCE)
				m_sequenceLayer = boost::dynamic_pointer_cast<SequenceLayer>(layerPtr);
			else if(layerPtr->GetType() == Layer::TREE)
				m_treeLayer = boost::dynamic_pointer_cast<TreeLayer>(layerPtr);

			return true;
		}
	}

	return false;
}

void GenGisFrame::SigViewportMouse(wxMouseEvent& event)
{
	if (!App::Inst().GetMapController())
		return;

	static bool bDragging = false;
	static bool bActiveElement = false;

	event.Skip();	// note: needs to be called so window will get focus (see wxWidgets documentation)

	// get mouse point in OpenGL screen coordinates
	Point2D mousePos(event.GetPosition().x, App::Inst().GetViewport()->GetHeight()-event.GetPosition().y);
	App::Inst().SetMouseScreenPos(mousePos);	

	// Check if viewport needs to be refreshed on a mouse move
	bool bUpdatedMousePos = false;
	if(event.Moving())
	{
		// save mouse position in world and screen space
		if(App::Inst().GetLayerTreeController()->GetNumMapLayers() > 0)
		{
			Point3D worldPos = App::Inst().GetMapController()->ProjectToTerrain(mousePos, false);
			App::Inst().SetMouseWorldPos(worldPos);

			GeoCoord geoCoord;
			App::Inst().GetMapController()->GridToGeo(worldPos, geoCoord);

			wxString worldPosStr;
			worldPosStr.Printf(wxT("Latitude: %0.2f, Longitude: %0.2f"), geoCoord.northing, geoCoord.easting);
			m_statusBar->SetStatusText(worldPosStr, 3);
		}
		
		if(App::Inst().GetMapController()->IsLoaded() && App::Inst().GetSelectedView() && App::Inst().GetSelectedView()->MouseMove(mousePos))
		{
			bUpdatedMousePos = true;
			App::Inst().GetViewport()->Render();
		}
	}

	// *** Check if the user is clicking a mouse button or dragging the mouse ***
	if(event.LeftDClick())
	{
		// process float click events and quick button presses
		MouseLeftDoubleClick(mousePos);
		App::Inst().GetViewport()->Refresh(false);
	}
	else if(event.LeftDown())
	{
		// a dragging operation is starting
		bDragging = true;

		bActiveElement = MouseLeftDown(mousePos);
		if(bActiveElement)
			App::Inst().GetViewport()->Refresh(false);
	}
	else if(event.RightDown())
	{
		App::Inst().GetViewport()->SetFocus();	// give window focus even on a right-click

		// a dragging operation is starting
		bDragging = true;

		MouseRightDown(mousePos);
	}
	else if(event.LeftUp())
	{
		// done dragging
		bDragging = false;
		if(bActiveElement)
		{			
			MouseLeftUp(mousePos);
			bActiveElement = false;
		}

		App::Inst().SetMouseDragging(false);
		App::Inst().GetViewport()->Refresh(false);
	}
	else if(event.RightUp())
	{
		// done dragging
		bDragging = false;

		App::Inst().SetMouseDragging(false);
		App::Inst().GetViewport()->Refresh(false);
	}
	else if(event.Dragging() && bDragging)	// using event.Dragging() fails when the File->Open dialog is used???
	{
		App::Inst().SetMouseDragging(true);

		// we are in the middle of a dragging operation
		int dx = mousePos.x - m_lastMousePos.x;
		int dy = mousePos.y - m_lastMousePos.y;

		if(event.LeftIsDown())
		{
			bool bRendered = false;
			if(bActiveElement)
			{				
				if(!bUpdatedMousePos && App::Inst().GetSelectedView() && App::Inst().GetSelectedView()->MouseMove(mousePos))
				{
					// save mouse position in world and screen space
					Point3D worldPos = App::Inst().GetMapController()->ProjectToTerrain(mousePos, false);
					App::Inst().SetMouseWorldPos(worldPos);

					App::Inst().GetViewport()->Render();
					bRendered = true;
				}

				MouseDragging(mousePos);
			}
			else
			{	
				const float TRANSLATION_SENSITIVITY = 0.001f;
				App::Inst().GetViewport()->GetCamera()->TranslateDelta(dx*TRANSLATION_SENSITIVITY, -dy*TRANSLATION_SENSITIVITY);
			}		

			App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
			if(!bRendered)
				App::Inst().GetViewport()->Refresh(false);
		}

		if(event.RightIsDown())
		{
			const float ANGLE_Y_SENSITIVITY = 0.5f;
			const float PITCH_SENSITIVITY = 0.5f;

			App::Inst().GetViewport()->GetCamera()->YawDelta(-dx*ANGLE_Y_SENSITIVITY);
			App::Inst().GetViewport()->GetCamera()->PitchDelta(-dy*PITCH_SENSITIVITY);

			m_widgetController->SetCompassAngle(App::Inst().GetViewport()->GetCamera()->GetYaw());

			App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
			App::Inst().GetViewport()->Refresh(false);
		}
	}

	m_lastMousePos = mousePos;
}

void GenGisFrame::SigViewportMouseWheel(wxMouseEvent& event)
{
	const float WHEEL_ZOOM_SENSITIVITY = 0.025f;
	const float REDRAW_ZOOM_SENSITIVITY = 0.25f;
	static float prevZoom = std::numeric_limits<float>::max();

	int dWheel = event.GetWheelRotation()/event.GetWheelDelta();
	App::Inst().GetViewport()->GetCamera()->ZoomDelta((float)(-dWheel*WHEEL_ZOOM_SENSITIVITY));

	float curZoom = App::Inst().GetViewport()->GetCamera()->GetHeight();
	float deltaZoom = (curZoom - prevZoom) / prevZoom;
	if(prevZoom == std::numeric_limits<float>::max() || (deltaZoom < -REDRAW_ZOOM_SENSITIVITY || deltaZoom > REDRAW_ZOOM_SENSITIVITY))
	{
		prevZoom = curZoom;
		App::Inst().GetMapController()->GetMapView()->RefreshQuadtree();
	}

	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	Refresh(false);
}

TreeLayerPtr GenGisFrame::GetSelectedTree()
{
	ViewPtr view = App::Inst().GetRightClickView();
	if (view)
	{
		GeoTreeViewPtr geoTreeView = boost::dynamic_pointer_cast<GeoTreeView>(view);
		if ( geoTreeView )
		{
			uint treeId = geoTreeView->GetTreeLayerId();
			return App::Inst().GetLayerTreeController()->GetTreeLayerById( treeId );
		}
	}

	return TreeLayerPtr();
}