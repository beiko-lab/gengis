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

#ifndef _GENGIS_GEOPHYLOGENY_FRAME_
#define _GENGIS_GEOPHYLOGENY_FRAME_

#include "../core/Precompiled.hpp"

#include "../core/GeoTreeView.hpp"

#include "../gui/GenGisLayout.h"

namespace GenGIS
{

	/**
	* @brief Handles all events and GUI processing for PluginLayout form.
	*/
	class GenGisFrame : public GenGisLayout
	{
	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		*/
		explicit GenGisFrame( wxWindow* parent = (wxFrame *)NULL );

		/** Destructor */
		~GenGisFrame();

		void SetSidePanelVisibility(bool bVisible);
		void SetConsoleVisibility(bool bVisible);

		void AddConsolePanel(wxPanel* panel, wxString name);
		wxNotebook* GetBottomNotebook();

		void FillSequenceLegend();

		void FillSamplesLegend();
		void OnSamplesLegendClick( wxMouseEvent& event );
		void OnSamplesLegendPaint(wxPaintEvent& WXUNUSED(event));

		void SafeYield() { ::wxSafeYield(); }

		LayoutLinePtr GetLayoutLineController() const
		{
			return m_layoutLine;
		}

		void SetLayoutLineController(LayoutLinePtr layoutLine)
		{
			m_layoutLine = layoutLine;
		}

		LayoutEllipsePtr GetLayoutEllipseController() const
		{
			return m_layoutEllipse;
		}

		void SetLayoutEllipseController(LayoutEllipsePtr layoutEllipse)
		{
			m_layoutEllipse = layoutEllipse;
		}

		/** Bring up a dialog to specify a map file. */
		wxString OpenLayerMapDialog();

		/** Bring up a dialog to specify a vector map file. */
		wxString OpenLayerVectorMapDialog();

		/** Open map from command line. */
		void LayerOpenMap( std::string file);

		/** Handle actual loading of map from file. */
		void LayerOpenMap( wxFileName fullPath, uint mapIndex);

		/** Open vector map from command line. */
		void LayerOpenVectorMap( std::string file );

		/** Handle actual loading of vector map from file. */
		void LayerOpenVectorMap( wxFileName fullPath, uint mapIndex );

		/** Open location set from command line. */
		void LayerOpenLocations(std::string file);

		/** Handle actual loading of location set data from file. */
		void LayerOpenLocations( wxFileName fullPath );

		/** Handle loading of location set CSV files */
		void LayerOpenLocations( std::vector<std::wstring>& csvTableRows, std::wstring locationNames );

		/** Open sequence data from command line. */
		void LayerOpenSequenceData( std::string file );

		/** Handle actual loading of sequence data from file. */
		void LayerOpenSequenceData( wxFileName fullPath );

		/** Handle actual loading of sequence CSV files. */
		void LayerOpenSequenceData( std::vector<std::wstring>& csvTableRows, std::wstring locationNames );

		/** Open tree from command line. */
		void LayerOpenTree( std::string file );

		/** Handle actual loading of tree data from file. */
		void LayerOpenTree( wxFileName fullPath );

		/** Get the Tree Control Layer (for serialization purposes). */
		wxTreeCtrl* GetTreeCtrlLayer() const { return m_treeCtrlLayer; }

		/** Get the Panel Viewport (for serialization purposes). */
		wxPanel* GetPanelViewport() const { return m_pnlViewport; }

		wxWindowID GetViewPortID() const { return ID_VIEWPORT; }

		/** Write the user preferences to file. */
		void SaveUserPreferences();

		/** Open user preferences from file. */
		void LoadUserPreferences();

		/** Show/Hide text below the toolbar icons. */
		void SetToolbarTextStatus( bool newStatus );

		/* Show/Hide the welcome dialog */
		void SetWelcomeDialogStatus( bool newStatus );

		bool GetWelcomeDialogStatus() { return m_showWelcomeDialog; }

		/** Add session path to the list of recently opened sessions. */
		void AddToRecentSessionList( wxString sessionFilePath );

		wxArrayString GetRecentSessionList();

		/** Populate (first time) the 'Recent Sessions' submenu. */
		void PopulateRecentSessionsMenu();

		/** Update the 'Recent Sessions' submenu. */
		void UpdateRecentSessionsMenu();

		/** File->New Session event handler. */
		void OnFileNewSession( wxCommandEvent& event );

		/** File->Open Session event handler. */
		void OnFileOpenSession( wxCommandEvent& event );
        
        void OpenSession( wxString sessionFullPath );

		/** Help->GenGIS Manual event handler. */
		void OnHelpManual( wxCommandEvent& event );

		/** Enable/Disable the R Auto-Configuration Utility in the Settings menu. */
		void SetRAutoConfigUtilStatus( bool status ) { m_mnuRAutoConfigUtil->Enable( status ); }

		/** Functions to clear layer vectors. */
		void ClearMapLayer()         { m_mapLayer.reset();         }
		void ClearVectorMapLayer()         { m_vectorMapLayer.reset();         }
		void ClearLocationSetLayer() { m_locationSetLayer.reset(); }
		void ClearLocationLayer()    { m_locationLayer.reset();    }
		void ClearSequenceLayer()    { m_sequenceLayer.reset();    }
		void ClearTreeLayer()        { m_treeLayer.reset();        }

	private:
		/** Custom serialization function for GenGisFrame */
		template<class Archive>
		void GenGisFrameSerialize(Archive & ar, const unsigned int file_version);

		/** Connect events handled by this frame. */
		virtual void ConnectEvents();

		/** Disconnect events handled by this frame. */
		virtual void DisconnectEvents();

		/** File->Save (session) event handler. */
		void OnFileSaveSession( wxCommandEvent& event );

		/** File->Save As (session) event handler. */
		void OnFileSaveSessionAs( wxCommandEvent& event );

		/** Perform actual saving operations. */
		bool SaveSession( bool openSaveDialog );

		void OnConfigureDB( wxCommandEvent& event );
		void OnObtainDataFromDB( wxCommandEvent& event );

		/** File->Save image as event handler.  Menu renamed to "Export As Image..." */
		void OnFileSaveImageAs( wxCommandEvent& event );

		/** Handler for clearing the list of recently opened sessions. */
		void OnClearSessionHistory( wxCommandEvent& event );

		/** File->Exit event handler. */
		void OnFileExit( wxCommandEvent& event );

		/** Close button handler for main window. */
		void OnWindowClose( wxCloseEvent& event );

		/** Layer->Open map layer event handler. */
		void OnLayerOpenMap( wxCommandEvent& event );

		/** Layer->Open vector map layer event handler. */
		void OnLayerOpenVectorMap( wxCommandEvent& event );

		/** Layer->Open location layer event handler. */
		void OnLayerOpenLocations( wxCommandEvent& event );

		/** Layer->Open tree layer event handler. */
		void OnLayerOpenTree( wxCommandEvent& event );

		/** Layer->Open sequence data event handler. */
		void OnLayerOpenSequenceData( wxCommandEvent& event );

		/** Layer->Remove event handler. */
		void OnLayerRemove( wxCommandEvent& event );
		void OnAllLayersRemove( wxCommandEvent& event );


		/** Layer->Hide All event handler. */
		void OnLayerHideAll( wxCommandEvent& event );

		/** Layer->Show All handler. */
		void OnLayerShowAll( wxCommandEvent& event );

		/** Handle the layer tree select event. */
		void OnLayerTreeSelect( wxTreeEvent& event );

		/** Handle the layer tree left-click event. */
		void OnLayerTreeClick( wxMouseEvent& event );

		/** Handle the layer tree left float click event. */
		void OnLayerTreeLeftDClick( wxMouseEvent& event );

		/** Handle the layer tree right-click event. */
		void OnLayerTreePopupMenu( wxTreeEvent& event );

		/** View->Camera Position->Reset event handler. */
		void OnViewReset( wxCommandEvent& event );
		void ViewReset();

		/** View->Camera Position->Top event handler. */
		void OnViewTop( wxCommandEvent& event );
		void ViewTop();

		/** View->Show Text Under Toolbar Icons event handler. */
		void OnViewShowToolbarText( wxCommandEvent& event );

		/** View->Panes->Side Panel event handler. */
		void OnViewSidePanel( wxCommandEvent& event );
		void OnViewFullScreen( wxCommandEvent& event );

		/** View->Panes->Console event handler. */
		void OnViewConsole( wxCommandEvent& event );

		/** View->Widgets->Compass event handler. */
		void OnViewCompass( wxCommandEvent& event );

		/** View->Widgets->Overview Map event handler. */
		void OnViewMiniMap( wxCommandEvent& event );

		/** View->Widgets->Navigation Control event handler. */
		void OnViewNav( wxCommandEvent& event );

		/** Hide all widgets. */
		void OnViewHideWidgets( wxCommandEvent& event );

		/** Show all widgets. */
		void OnViewShowWidgets( wxCommandEvent& event );

		/** View->Select sequence dialog event handler. */
		void OnViewSelectSeqDlg( wxCommandEvent& event );
		void ViewSelectSegDlg();

		/** Settings->Font event handler. */
		void OnFontSettingsDlg( wxCommandEvent& event );

		/** Settings->Lighting event handler. */
		void OnLightingSettingsDlg( wxCommandEvent& event );

		/** Settings->Layout objects event handler. */
		void OnLayoutObjectSettingsDlg( wxCommandEvent& event );

		/** Settings->Show Welcome Screen on Startup event handler. */
		void OnShowWelcomeScreenOnStartupDlg( wxCommandEvent& event );

		/** Help->About event handler. */
		void OnHelpAbout( wxCommandEvent& event );

		/** Help->Check for Updates event handler. */
		void OnCheckForUpdates( wxCommandEvent& event );

		/** Standard mode toolbar button event handler. */
		void OnModeStandard( wxCommandEvent& event );

		/** Draw layout line toolbar button event handler. */
		void OnModeDrawLayoutLine( wxCommandEvent& event );

		/** Draw layout ellipse toolbar button event handler. */
		void OnModeDrawLayoutEllipse( wxCommandEvent& event );

		/** Label tool toolbar button event handler */
		void OnLabelTool( wxCommandEvent& event );

		/** Define geographic axis using a polyline event handler. */
		void OnGeoAxisByPolyline( wxCommandEvent& event );

		/** Erase layout line popup menu event. */
		void OnLayoutLineDelete(wxCommandEvent& event);

		/** Erase layout ellipse popup menu event. */
		void OnLayoutEllipseDelete(wxCommandEvent& event);

		/** Erase geographic axis polyline popup menu event. */
		void OnGeoAxisPolylineDelete(wxCommandEvent& event);

		/** OpenGL viewport hide popup menu event. */
		void OnSelectedViewHide(wxCommandEvent& event);

		/** OpenGL viewport properties popup menu event. */
		void OnSelectedViewProperties(wxCommandEvent& event);

		/** Remove selected view from viewport popup menu event. */
		void OnSelectedViewRemove(wxCommandEvent& event);

		/** Handle the erase event for the main form which is generated
		whenever a window's background needs to be repainted. */
		void OnEraseBackground( wxEraseEvent& event ){ /* prevent flicker */ }

		/** Handle the erase event for the viewport panel which is generated
		whenever a window's background needs to be repainted. */
		void OnViewportEraseBackground( wxEraseEvent& event ){ /* prevent flicker */ }

		/** Handle the study properties popup menu event. */
		void OnStudyProperties(wxCommandEvent& event);

		/** Handle the map properties popup menu event. */
		void OnMapProperties(wxCommandEvent& event);
		void OnVectorMapProperties(wxCommandEvent& event);


		/** Handle the location set properties popup menu event. */
		void OnLocationSetProperties(wxCommandEvent& event);

		/** Handle the location properties popup menu event. */
		void OnLocationProperties(wxCommandEvent& event);

		/** Handle the sequence properties popup menu event. */
		void OnSequenceProperties(wxCommandEvent& event);

		/** Handle the tree properties popup menu event. */
		void OnTreeProperties(wxCommandEvent& event);

		/** Handle the popup menu 3D cladogram event. */
		void OnCladogram3D( wxCommandEvent& event );

		/** Handle the popup menu 3D slanted cladogram event. */
		void OnSlantedCladogram3D( wxCommandEvent& event );

		/** Handle the popup menu 3D slanted phylogram event. */
		void OnSlantedPhylogram3D( wxCommandEvent& event );

		/** Handle the popup menu 2D cladogram event. */
		void OnCladogram2D( wxCommandEvent& event );

		/** Handle the popup menu 2D phylogram event. */
		void OnPhylogram2D( wxCommandEvent& event );

		/** Handle the popup menu 2D slanted cladogram event. */
		void OnSlantedCladogram2D( wxCommandEvent& event );

		/** Handle the popup menu ellipse quadrant layout event. */
		void OnLayoutTreeEllipseQuadrant( wxCommandEvent& event );

		/** Handle the popup menu tree layout line visibility event. */
		void OnTreeLayoutLineVisibility( wxCommandEvent& event );

		/** Handle the popup menu tree flip event. */
		void OnTreeFlip( wxCommandEvent& event );
		
		/** Handle the popup menu tree layout ellipse visibility event. */
		void OnTreeLayoutEllipseVisibility( wxCommandEvent& event );

		/** Handle the popup menu tree geographic axis visibility event. */
		void OnTreeGeographicAxisVisibility( wxCommandEvent& event );

		/** Handle the popup menu "Assign Geographical Axis to Tree" event. */
		void OnAssignGeoAxisToTree( wxCommandEvent& event );

		/** Reverse the order of geographic points on the geographic layout line. */
		void OnReverseGeoAxisOrder( wxCommandEvent& event );

		/** Perform analysis of all possible linear axes for a 2D tree. */
		void OnPerformLinearAxesAnalysis( wxCommandEvent& event );

		/** Perform analysis of all possible linear axes for a 2D tree. */
		void OnPerformLinearAxesAnalysisPopup( wxCommandEvent& event );

		/** Show table indicating results for different orderings of nonlinear axis polylines. */
		void OnShowNonlinearAxisTable( wxCommandEvent& event );

		/** Show table indicating results for different orderings of nonlinear axis polylines. */
		void OnShowNonLinearAxisTablePopup( wxCommandEvent& event );

		/** Handle the popup menu leaf node labels event. */
		void OnLeafNodeLabels( wxCommandEvent& event );

		/** Handle the layer tree menu significance test event. */
		void OnTreeSignificanceTest( wxCommandEvent& event );

		/** Handle the viewport popup menu tree significance test event. */
		void OnTreeSignificanceTestPopup( wxCommandEvent& event );

		/** Handle the popup menu to extend a geographic polyline. */
		void OnExtendGeographicPolyline( wxCommandEvent& event );

		/** Layout charts in default arrangement (directly above locations). */
		void OnLayoutChartsDefaultPosition(wxCommandEvent& event);

		/** Layout charts on the leaf nodes of a tree. */
		void OnLayoutChartsOnTreeLeaves(wxCommandEvent& event);

		/** Layout charts evenly along a layout line. */
		void OnLayoutChartsEvenlyOnLine(wxCommandEvent& event);

		/** Project charts onto a layout line. */
		void OnLayoutChartsLineProjected(wxCommandEvent& event);

		/** Layout each quadrant of an ellipse independently. */
		void OnLayoutChartsEllipseQuadrant(wxCommandEvent& event);

		/** Layout charts evenly along a layout ellipse. */
		void OnLayoutChartsEvenlyOnEllipse(wxCommandEvent& event);

		/** Project charts onto a layout ellipse. */
		void OnLayoutChartsEllipseProjected(wxCommandEvent& event);

		/** Handle the popup menu chart set layout line event. */
		void OnChartSetLayoutLine( wxCommandEvent& event );

		/** Handle the popup menu chart set layout ellipse event. */
		void OnChartSetLayoutEllipse( wxCommandEvent& event );

		/** Handle the R Auto-Configuration Utility menu event. */
		void OnRAutoConfigurationUtility( wxCommandEvent& event );

		/** OpenGL render event handler. */
		void SigRender();

		/** Handle mouse move and button down events from viewport. */
		virtual void SigViewportMouse(wxMouseEvent& event);

		/** Handle mouse wheel events from viewport. */
		virtual void SigViewportMouseWheel(wxMouseEvent& event);

		/** Handle keyboard events from viewport. */
		void SigViewportKeyboard( wxKeyEvent& event );

		/** Handle node update events for trees. */
		void SigNodeUpdate(NodeInfo nodeInfo);

		/**
		* @brief Determine if mouse is over an interactive element when mouse left down event occurs.
		* @param mousePt Position of mouse.
		*/
		bool MouseLeftDown(const Point2D& mousePt);

		/**
		* @brief Determine if mouse is over an interactive element when mouse left up event occurs.
		* @param mousePt Position of mouse.
		*/
		void MouseLeftUp(const Point2D& mousePt);

		/**
		* @brief Determine if mouse is over an interactive element when mouse right down event occurs.
		* @param mousePt Position of mouse.
		*/
		bool MouseRightDown(const Point2D& mousePt);

		/**
		* @brief Determine if mouse is being dragged while connected to an interactive element.
		* @param mousePt Position of mouse.
		*/
		void MouseDragging(const Point2D& mousePt);

		/**
		* @brief Determine if mouse is over an interactive element when float-clicked.
		* @param mousePt Position of mouse.
		*/
		void MouseLeftDoubleClick(const Point2D& mousePt);

		/** Get widget controller. */
		WidgetControllerPtr GetWidgetController() { return m_widgetController; }

		/** Event handler for selecting colours under Mac/Unix. */
		void OnCustomColourButtonClicked( wxMouseEvent& event );

		/** Get a pointer to the currently selected tree. */
		TreeLayerPtr GetSelectedTree();

	protected:

		/** Last position of mouse. */
		Point2D m_lastMousePos;

		/** Track position of main window horizontal splitter. */
		int m_horSplitterPos;

		/** Track position of main window vertical splitter. */
		int m_verSplitterPos;

		/** Track whether or not toolbar text should be displayed. */
		bool m_toolbarTextStatus;
		bool m_toolbarFullScreenStatus;
		bool m_viewCompassStatus;
		bool m_viewMinimapStatus;
		bool m_viewNavStatus;


		/** Track whether or not the welcome screen should be displayed on startup. */
		bool m_showWelcomeDialog;

		/** Keep track of recent sessions. */
		std::vector<std::string> m_recentSessions;

		/** Unassigned layout line. */
		LayoutLinePtr m_layoutLine;

		/** Unassigned layout ellipse. */
		LayoutEllipsePtr m_layoutEllipse;

		/** Unassigned geographic axis defined by a polyline. */
		GeoAxisPolylinePtr m_geoAxisPolyline;

		/** Controller to manage OpenGL widgets. */
		WidgetControllerPtr m_widgetController;

		/** Currently active study layer. */
		StudyLayerPtr m_studyLayer;

		/** Currently active map layer. */
		MapLayerPtr m_mapLayer;

		/** Currently active vector map layer. */
		VectorMapLayerPtr m_vectorMapLayer;

		/** Currently active location set layer. */
		LocationSetLayerPtr m_locationSetLayer;

		/** Currently active location layer. */
		LocationLayerPtr m_locationLayer;

		/** Currently active sequence layer. */
		SequenceLayerPtr m_sequenceLayer;

		/** Currently active tree layer. */
		TreeLayerPtr m_treeLayer;
	};

}

#endif 
