///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GenGisLayout__
#define __GenGisLayout__

#include <wx/statline.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/choice.h>
#include <wx/statbox.h>
#include <wx/scrolwin.h>
#include <wx/notebook.h>
#include <wx/splitter.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class GenGisLayout
	///////////////////////////////////////////////////////////////////////////////
	class GenGisLayout : public wxFrame 
	{
		private:
		
		protected:
			enum
			{
				ID_MAIN_FRAME = 1000,
				ID_STATIC_LINE,
				ID_PANEL_TOP,
				ID_SPLITTER,
				ID_SIDEBAR,
				ID_TREE_CTRL_LAYER,
				wxID_LOCATION_SET_CHOICE,
				ID_LEGEND_LOCATIONS_SIZER_COLOUR,
				ID_LEGEND_LOCATIONS_SIZER_SHAPE,
				ID_LEGEND_LOCATIONS_SIZER_SIZE,
				ID_VIEWPORT,
				ID_PANEL_BOTTOM,
				ID_CONSOLE,
				ID_MENUBAR,
				ID_MNU_FILE_NEW_SESSION,
				ID_MNU_FILE_OPEN_SESSION,
				ID_MNU_FILE_RECENT_SESSION_CLEAR_HISTORY,
				ID_MNU_FILE_SAVE_SESSION,
				ID_MNU_FILE_SAVE_SESSION_AS,
				ID_MNU_FILE_SAVE_IMAGE_AS,
				ID_MNU_VIEW_SIDE_PANEL,
				ID_MNU_VIEW_CONSOLE,
				ID_MNU_VIEW_NAV,
				ID_MNU_VIEW_MINIMAP,
				ID_MNU_VIEW_COMPASS,
				ID_MNU_VIEW_DEFAULT,
				ID_MNU_VIEW_TOP,
				ID_MNU_VIEW_TOOLBARTEXT,
				ID_MNU_FULL_SCREEN,
				ID_MNU_LAYER_ADD_MAP,
				ID_MNU_LAYER_ADD_VECTOR_MAP,
				ID_MNU_LAYER_ADD_LOCATIONS,
				ID_MNU_LAYER_SEQUENCE_DATA,
				ID_MNU_LAYER_ADD_TREE,
				ID_MNU_LAYER_REMOVE,
				ID_MNU_ALL_LAYER_REMOVE,
				ID_MNU_LAYER_HIDE_ALL,
				ID_MNU_LAYER_SHOW_ALL,
				ID_MNU_SETTINGS_LIGHTING,
				ID_MNU_LAYOUT_OBJECT_PROP,
				ID_MNU_SHOW_WELCOME,
				ID_TOOLBAR,
				ID_TB_OPEN_SESSION,
				ID_TB_LAYER_OPEN_DEM,
				ID_TB_LAYER_OPEN_VECTOR,
				ID_TB_LAYER_OPEN_LOCATIONS,
				ID_TB_LAYER_OPEN_SAMPLE_DATA,
				ID_TB_LAYER_OPEN_TREE,
				ID_VIEW_RESET,
				ID_VIEW_TOP,
				ID_TB_MODE_DRAW_LAYOUT_LINE,
				ID_TB_MODE_DRAW_LAYOUT_ELLIPSE,
				ID_TB_GEO_AXIS_BY_POLYLINE,
				ID_TB_LABEL_TOOL,
				ID_STATUSBAR,
			};
			
			wxStaticLine* m_staticline;
			wxSplitterWindow* m_splitterHorizontal;
			wxPanel* m_pnlTop;
			wxSplitterWindow* m_splitterVertical;
			wxPanel* m_pnlSidebar;
			wxNotebook* m_sidebarNotebook;
			wxPanel* m_treeCtrlPanel;
			wxTreeCtrl* m_treeCtrlLayer;
			wxScrolledWindow* m_legendLocations;
			wxBoxSizer* m_legendLocationsSizer;
			wxChoice* m_locationSetChoice;
			wxStaticBoxSizer* m_legendLocationsSizerColour;
			wxFlexGridSizer* m_legendLocationsSizerColour2;
			wxStaticBoxSizer* m_legendLocationsSizerShape;
			wxFlexGridSizer* m_legendLocationsSizerShape2;
			wxStaticBoxSizer* m_legendLocationsSizerSize;
			wxFlexGridSizer* m_legendLocationsSizerSize2;
			wxScrolledWindow* m_legendSequence;
			wxFlexGridSizer* m_legendSequenceSizer;
			wxPanel* m_pnlViewport;
			wxPanel* m_pnlBottom;
			wxNotebook* m_bottomNotebook;
			wxPanel* m_consolePanel;
			wxRichTextCtrl* m_console;
			wxMenuBar* m_menubar;
			wxMenu* m_mnuFile;
			wxMenu* m_mnuRecentSessions;
			wxMenu* m_mnuView;
			wxMenu* m_mnuViewPanes;
			wxMenu* m_mnuViewWidgets;
			wxMenu* m_mnuViewCamera;
			wxMenu* m_mnuLayers;
			wxMenu* m_mnuSettings;
			wxMenuItem* m_mnuRAutoConfigUtil;
			wxMenu* m_mnuAnalysis;
			wxMenu* m_mnuData;
			wxMenu* m_mnuPlugins;
			wxMenu* m_mnuHelp;
			wxToolBar* m_toolBar;
			wxStatusBar* m_statusBar;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnCharFromKeyboard( wxKeyEvent& event ) { event.Skip(); }
			virtual void OnWindowClose( wxCloseEvent& event ) { event.Skip(); }
			virtual void OnLayerTreeLeftDClick( wxMouseEvent& event ) { event.Skip(); }
			virtual void OnLayerTreeClick( wxMouseEvent& event ) { event.Skip(); }
			virtual void OnLayerTreePopupMenu( wxTreeEvent& event ) { event.Skip(); }
			virtual void OnLayerTreeSelect( wxTreeEvent& event ) { event.Skip(); }
			virtual void OnSamplesLegendLocationSetSelect( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnConsoleEnter( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnFileNewSession( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnFileOpenSession( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnClearSessionHistory( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnFileSaveSession( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnFileSaveSessionAs( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnFileSaveImageAs( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnFileExit( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnViewSidePanel( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnViewConsole( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnViewNav( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnViewMiniMap( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnViewCompass( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnViewShowWidgets( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnViewHideWidgets( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnViewReset( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnViewTop( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnViewShowToolbarText( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnViewFullScreen( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnLayerOpenMap( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnLayerOpenVectorMap( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnLayerOpenLocations( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnLayerOpenSequenceData( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnLayerOpenTree( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnLocationMerge( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnLayerRemove( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnAllLayersRemove( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnLayerHideAll( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnLayerShowAll( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnFontSettingsDlg( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnLightingSettingsDlg( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnLayoutObjectSettingsDlg( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRAutoConfigurationUtility( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnShowWelcomeScreenOnStartupDlg( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnViewSelectSeqDlg( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnHelpManual( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCheckForUpdates( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnHelpAbout( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnModeDrawLayoutLine( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnModeDrawLayoutEllipse( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnGeoAxisByPolyline( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnLabelTool( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			GenGisLayout( wxWindow* parent, wxWindowID id = ID_MAIN_FRAME, const wxString& title = wxT("GenGIS"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1120,653 ), long style = wxDEFAULT_FRAME_STYLE|wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
			~GenGisLayout();
			
			void m_splitterHorizontalOnIdle( wxIdleEvent& )
			{
				m_splitterHorizontal->SetSashPosition( 373 );
				m_splitterHorizontal->Disconnect( wxEVT_IDLE, wxIdleEventHandler( GenGisLayout::m_splitterHorizontalOnIdle ), NULL, this );
			}
			
			void m_splitterVerticalOnIdle( wxIdleEvent& )
			{
				m_splitterVertical->SetSashPosition( 250 );
				m_splitterVertical->Disconnect( wxEVT_IDLE, wxIdleEventHandler( GenGisLayout::m_splitterVerticalOnIdle ), NULL, this );
			}
		
	};
	
} // namespace GenGIS

#endif //__GenGisLayout__
