///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "GenGisLayout.h"

#include "../../bin/images/DEM.xpm"
#include "../../bin/images/Label.xpm"
#include "../../bin/images/Location.xpm"
#include "../../bin/images/Reset.xpm"
#include "../../bin/images/SampleData.xpm"
#include "../../bin/images/checkmark.xpm"
#include "../../bin/images/close.xpm"
#include "../../bin/images/ellipse.xpm"
#include "../../bin/images/filenewsession.xpm"
#include "../../bin/images/fileopensession.xpm"
#include "../../bin/images/filesave.xpm"
#include "../../bin/images/filesaveas.xpm"
#include "../../bin/images/line.xpm"
#include "../../bin/images/manual.xpm"
#include "../../bin/images/phylogram.xpm"
#include "../../bin/images/polyline.xpm"
#include "../../bin/images/shapefile.xpm"
#include "../../bin/images/top.xpm"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

GenGisLayout::GenGisLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 600,400 ), wxDefaultSize );
	
	wxBoxSizer* m_mainVerticalSizer;
	m_mainVerticalSizer = new wxBoxSizer( wxVERTICAL );
	
	m_staticline = new wxStaticLine( this, ID_STATIC_LINE, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxCLIP_CHILDREN );
	m_staticline->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_staticline->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	m_mainVerticalSizer->Add( m_staticline, 0, wxBOTTOM|wxEXPAND, 0 );
	
	m_splitterHorizontal = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3DSASH|wxSP_NO_XP_THEME|wxCLIP_CHILDREN );
	m_splitterHorizontal->SetSashGravity( 1 );
	m_splitterHorizontal->Connect( wxEVT_IDLE, wxIdleEventHandler( GenGisLayout::m_splitterHorizontalOnIdle ), NULL, this );
	m_splitterHorizontal->SetMinimumPaneSize( 10 );
	
	m_pnlTop = new wxPanel( m_splitterHorizontal, ID_PANEL_TOP, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN|wxTAB_TRAVERSAL );
	wxBoxSizer* m_mainHorizontalSizer;
	m_mainHorizontalSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_splitterVertical = new wxSplitterWindow( m_pnlTop, ID_SPLITTER, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxCLIP_CHILDREN );
	m_splitterVertical->SetSashGravity( 0 );
	m_splitterVertical->Connect( wxEVT_IDLE, wxIdleEventHandler( GenGisLayout::m_splitterVerticalOnIdle ), NULL, this );
	m_splitterVertical->SetMinimumPaneSize( 10 );
	
	m_pnlSidebar = new wxPanel( m_splitterVertical, ID_SIDEBAR, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN|wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* m_sidebarSizer;
	m_sidebarSizer = new wxBoxSizer( wxVERTICAL );
	
	m_sidebarNotebook = new wxNotebook( m_pnlSidebar, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_treeCtrlPanel = new wxPanel( m_sidebarNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_treeCtrlSizer;
	m_treeCtrlSizer = new wxBoxSizer( wxVERTICAL );
	
	m_treeCtrlLayer = new wxTreeCtrl( m_treeCtrlPanel, ID_TREE_CTRL_LAYER, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HAS_BUTTONS|wxTR_HIDE_ROOT|wxCLIP_CHILDREN|wxSIMPLE_BORDER );
	m_treeCtrlSizer->Add( m_treeCtrlLayer, 1, wxALL|wxEXPAND|wxTOP, 3 );
	
	m_treeCtrlPanel->SetSizer( m_treeCtrlSizer );
	m_treeCtrlPanel->Layout();
	m_treeCtrlSizer->Fit( m_treeCtrlPanel );
	m_sidebarNotebook->AddPage( m_treeCtrlPanel, wxT("Layers"), true );
	m_legendLocations = new wxScrolledWindow( m_sidebarNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_legendLocations->SetScrollRate( 5, 5 );
	m_legendLocationsSizer = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_locationSetChoiceChoices;
	m_locationSetChoice = new wxChoice( m_legendLocations, wxID_LOCATION_SET_CHOICE, wxDefaultPosition, wxDefaultSize, m_locationSetChoiceChoices, 0 );
	m_locationSetChoice->SetSelection( 0 );
	m_legendLocationsSizer->Add( m_locationSetChoice, 0, wxALL, 5 );
	
	m_legendLocationsSizerColour = new wxStaticBoxSizer( new wxStaticBox( m_legendLocations, ID_LEGEND_LOCATIONS_SIZER_COLOUR, wxT("Colour") ), wxVERTICAL );
	
	m_legendLocationsSizerColour2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_legendLocationsSizerColour2->SetFlexibleDirection( wxBOTH );
	m_legendLocationsSizerColour2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_legendLocationsSizerColour->Add( m_legendLocationsSizerColour2, 0, 0, 5 );
	
	m_legendLocationsSizer->Add( m_legendLocationsSizerColour, 0, wxEXPAND, 5 );
	
	m_legendLocationsSizerShape = new wxStaticBoxSizer( new wxStaticBox( m_legendLocations, ID_LEGEND_LOCATIONS_SIZER_SHAPE, wxT("Shape") ), wxVERTICAL );
	
	m_legendLocationsSizerShape2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_legendLocationsSizerShape2->SetFlexibleDirection( wxBOTH );
	m_legendLocationsSizerShape2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_legendLocationsSizerShape->Add( m_legendLocationsSizerShape2, 0, 0, 5 );
	
	m_legendLocationsSizer->Add( m_legendLocationsSizerShape, 0, wxEXPAND, 5 );
	
	m_legendLocationsSizerSize = new wxStaticBoxSizer( new wxStaticBox( m_legendLocations, ID_LEGEND_LOCATIONS_SIZER_SIZE, wxT("Size") ), wxVERTICAL );
	
	m_legendLocationsSizerSize2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_legendLocationsSizerSize2->SetFlexibleDirection( wxBOTH );
	m_legendLocationsSizerSize2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_legendLocationsSizerSize->Add( m_legendLocationsSizerSize2, 0, 0, 5 );
	
	m_legendLocationsSizer->Add( m_legendLocationsSizerSize, 0, wxEXPAND, 5 );
	
	m_legendLocations->SetSizer( m_legendLocationsSizer );
	m_legendLocations->Layout();
	m_legendLocationsSizer->Fit( m_legendLocations );
	m_sidebarNotebook->AddPage( m_legendLocations, wxT("Locations"), false );
	m_legendSequence = new wxScrolledWindow( m_sidebarNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_legendSequence->SetScrollRate( 5, 5 );
	m_legendSequenceSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_legendSequenceSizer->SetFlexibleDirection( wxHORIZONTAL );
	m_legendSequenceSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_legendSequence->SetSizer( m_legendSequenceSizer );
	m_legendSequence->Layout();
	m_legendSequenceSizer->Fit( m_legendSequence );
	m_sidebarNotebook->AddPage( m_legendSequence, wxT("Sequences"), false );
	
	m_sidebarSizer->Add( m_sidebarNotebook, 1, wxEXPAND | wxALL, 5 );
	
	m_pnlSidebar->SetSizer( m_sidebarSizer );
	m_pnlSidebar->Layout();
	m_sidebarSizer->Fit( m_pnlSidebar );
	m_pnlViewport = new wxPanel( m_splitterVertical, ID_VIEWPORT, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN|wxSIMPLE_BORDER|wxWANTS_CHARS );
	m_splitterVertical->SplitVertically( m_pnlSidebar, m_pnlViewport, 250 );
	m_mainHorizontalSizer->Add( m_splitterVertical, 1, wxEXPAND, 2 );
	
	m_pnlTop->SetSizer( m_mainHorizontalSizer );
	m_pnlTop->Layout();
	m_mainHorizontalSizer->Fit( m_pnlTop );
	m_pnlBottom = new wxPanel( m_splitterHorizontal, ID_PANEL_BOTTOM, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_bottomSizer;
	m_bottomSizer = new wxBoxSizer( wxVERTICAL );
	
	m_bottomNotebook = new wxNotebook( m_pnlBottom, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_consolePanel = new wxPanel( m_bottomNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_consoleSizer;
	m_consoleSizer = new wxBoxSizer( wxVERTICAL );
	
	m_console = new wxRichTextCtrl( m_consolePanel, ID_CONSOLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_TAB|wxCLIP_CHILDREN|wxHSCROLL|wxVSCROLL|wxWANTS_CHARS );
	m_consoleSizer->Add( m_console, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_consolePanel->SetSizer( m_consoleSizer );
	m_consolePanel->Layout();
	m_consoleSizer->Fit( m_consolePanel );
	m_bottomNotebook->AddPage( m_consolePanel, wxT("Console"), false );
	
	m_bottomSizer->Add( m_bottomNotebook, 1, wxEXPAND | wxALL, 5 );
	
	m_pnlBottom->SetSizer( m_bottomSizer );
	m_pnlBottom->Layout();
	m_bottomSizer->Fit( m_pnlBottom );
	m_splitterHorizontal->SplitHorizontally( m_pnlTop, m_pnlBottom, 373 );
	m_mainVerticalSizer->Add( m_splitterHorizontal, 1, wxEXPAND, 2 );
	
	this->SetSizer( m_mainVerticalSizer );
	this->Layout();
	m_menubar = new wxMenuBar( 0|wxCLIP_CHILDREN );
	m_mnuFile = new wxMenu();
	wxMenuItem* m_mnuFileNewSession;
	m_mnuFileNewSession = new wxMenuItem( m_mnuFile, ID_MNU_FILE_NEW_SESSION, wxString( wxT("&New Session") ) + wxT('\t') + wxT("Ctrl+N"), wxT("Starts a new GenGIS session"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuFileNewSession->SetBitmaps( wxBitmap( filenewsession_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuFileNewSession->SetBitmap( wxBitmap( filenewsession_xpm ) );
	#endif
	m_mnuFile->Append( m_mnuFileNewSession );
	
	wxMenuItem* m_mnuFileOpenSession;
	m_mnuFileOpenSession = new wxMenuItem( m_mnuFile, ID_MNU_FILE_OPEN_SESSION, wxString( wxT("&Open Session") ) + wxT('\t') + wxT("Ctrl+O"), wxT("Opens previous configuration settings"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuFileOpenSession->SetBitmaps( wxBitmap( fileopensession_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuFileOpenSession->SetBitmap( wxBitmap( fileopensession_xpm ) );
	#endif
	m_mnuFile->Append( m_mnuFileOpenSession );
	
	m_mnuRecentSessions = new wxMenu();
	wxMenuItem* m_clearHistory;
	m_clearHistory = new wxMenuItem( m_mnuRecentSessions, ID_MNU_FILE_RECENT_SESSION_CLEAR_HISTORY, wxString( wxT("Clear History") ) , wxT("Clears history of recently opened sessions"), wxITEM_NORMAL );
	m_mnuRecentSessions->Append( m_clearHistory );
	m_clearHistory->Enable( false );
	
	wxMenuItem* m_separator71;
	m_separator71 = m_mnuRecentSessions->AppendSeparator();
	
	m_mnuFile->Append( -1, wxT("Recent Sessions"), m_mnuRecentSessions );
	
	wxMenuItem* m_mnuFileSaveSession;
	m_mnuFileSaveSession = new wxMenuItem( m_mnuFile, ID_MNU_FILE_SAVE_SESSION, wxString( wxT("&Save") ) + wxT('\t') + wxT("Ctrl+S"), wxT("Saves configuration settings to file"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuFileSaveSession->SetBitmaps( wxBitmap( filesave_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuFileSaveSession->SetBitmap( wxBitmap( filesave_xpm ) );
	#endif
	m_mnuFile->Append( m_mnuFileSaveSession );
	
	wxMenuItem* m_mnuFileSaveSessionAs;
	m_mnuFileSaveSessionAs = new wxMenuItem( m_mnuFile, ID_MNU_FILE_SAVE_SESSION_AS, wxString( wxT("Save &As...") ) + wxT('\t') + wxT("Ctrl+Shift+S"), wxT("Saves configuration settings to a new file"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuFileSaveSessionAs->SetBitmaps( wxBitmap( filesaveas_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuFileSaveSessionAs->SetBitmap( wxBitmap( filesaveas_xpm ) );
	#endif
	m_mnuFile->Append( m_mnuFileSaveSessionAs );
	
	wxMenuItem* m_mnuFileSaveImageAs;
	m_mnuFileSaveImageAs = new wxMenuItem( m_mnuFile, ID_MNU_FILE_SAVE_IMAGE_AS, wxString( wxT("&Export as Image...") ) + wxT('\t') + wxT("Ctrl+E"), wxT("Saves the viewport as an image"), wxITEM_NORMAL );
	m_mnuFile->Append( m_mnuFileSaveImageAs );
	
	wxMenuItem* m_separator3;
	m_separator3 = m_mnuFile->AppendSeparator();
	
	wxMenuItem* m_mnuFileExit;
	m_mnuFileExit = new wxMenuItem( m_mnuFile, wxID_EXIT, wxString( wxT("E&xit") ) , wxT("Closes the program"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuFileExit->SetBitmaps( wxBitmap( close_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuFileExit->SetBitmap( wxBitmap( close_xpm ) );
	#endif
	m_mnuFile->Append( m_mnuFileExit );
	
	m_menubar->Append( m_mnuFile, wxT("&File") ); 
	
	m_mnuView = new wxMenu();
	m_mnuViewPanes = new wxMenu();
	wxMenuItem* m_mnuViewSidePanel;
	m_mnuViewSidePanel = new wxMenuItem( m_mnuViewPanes, ID_MNU_VIEW_SIDE_PANEL, wxString( wxT("Side &Panel") ) + wxT('\t') + wxT("Ctrl+Alt+L"), wxEmptyString, wxITEM_CHECK );
	m_mnuViewPanes->Append( m_mnuViewSidePanel );
	m_mnuViewSidePanel->Check( true );
	
	wxMenuItem* m_mnuViewConsole;
	m_mnuViewConsole = new wxMenuItem( m_mnuViewPanes, ID_MNU_VIEW_CONSOLE, wxString( wxT("&Console") ) + wxT('\t') + wxT("Ctrl+Alt+C"), wxEmptyString, wxITEM_CHECK );
	m_mnuViewPanes->Append( m_mnuViewConsole );
	m_mnuViewConsole->Check( true );
	
	m_mnuView->Append( -1, wxT("&Panes"), m_mnuViewPanes );
	
	m_mnuViewWidgets = new wxMenu();
	wxMenuItem* m_mnuViewNav;
	m_mnuViewNav = new wxMenuItem( m_mnuViewWidgets, ID_MNU_VIEW_NAV, wxString( wxT("&Navigation Control") ) , wxT("Show/hide navigation control"), wxITEM_CHECK );
	m_mnuViewWidgets->Append( m_mnuViewNav );
	m_mnuViewNav->Check( true );
	
	wxMenuItem* m_mnuViewMiniMap;
	m_mnuViewMiniMap = new wxMenuItem( m_mnuViewWidgets, ID_MNU_VIEW_MINIMAP, wxString( wxT("&Overview Map") ) , wxT("Show/hide overview map"), wxITEM_CHECK );
	m_mnuViewWidgets->Append( m_mnuViewMiniMap );
	m_mnuViewMiniMap->Check( true );
	
	wxMenuItem* m_mnuViewCompass;
	m_mnuViewCompass = new wxMenuItem( m_mnuViewWidgets, ID_MNU_VIEW_COMPASS, wxString( wxT("&Compass") ) , wxT("Show/hide compass widget"), wxITEM_CHECK );
	m_mnuViewWidgets->Append( m_mnuViewCompass );
	m_mnuViewCompass->Check( true );
	
	wxMenuItem* m_separator9;
	m_separator9 = m_mnuViewWidgets->AppendSeparator();
	
	wxMenuItem* m_mnuViewWidgetsShowAll;
	m_mnuViewWidgetsShowAll = new wxMenuItem( m_mnuViewWidgets, wxID_ANY, wxString( wxT("&Show All") ) , wxEmptyString, wxITEM_NORMAL );
	m_mnuViewWidgets->Append( m_mnuViewWidgetsShowAll );
	
	wxMenuItem* m_mnuViewWidgetsHideAll;
	m_mnuViewWidgetsHideAll = new wxMenuItem( m_mnuViewWidgets, wxID_ANY, wxString( wxT("&Hide All") ) , wxEmptyString, wxITEM_NORMAL );
	m_mnuViewWidgets->Append( m_mnuViewWidgetsHideAll );
	
	m_mnuView->Append( -1, wxT("&Map Controls"), m_mnuViewWidgets );
	
	wxMenuItem* m_separator4;
	m_separator4 = m_mnuView->AppendSeparator();
	
	m_mnuViewCamera = new wxMenu();
	wxMenuItem* m_mnuViewDefault;
	m_mnuViewDefault = new wxMenuItem( m_mnuViewCamera, ID_MNU_VIEW_DEFAULT, wxString( wxT("&Perspective (Default)") ) + wxT('\t') + wxT("Ctrl+Alt+R"), wxT("Reset viewpoint"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuViewDefault->SetBitmaps( wxBitmap( Reset_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuViewDefault->SetBitmap( wxBitmap( Reset_xpm ) );
	#endif
	m_mnuViewCamera->Append( m_mnuViewDefault );
	
	wxMenuItem* m_mnuViewTop;
	m_mnuViewTop = new wxMenuItem( m_mnuViewCamera, ID_MNU_VIEW_TOP, wxString( wxT("&Top") ) + wxT('\t') + wxT("Ctrl+Alt+T"), wxT("View from top"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuViewTop->SetBitmaps( wxBitmap( top_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuViewTop->SetBitmap( wxBitmap( top_xpm ) );
	#endif
	m_mnuViewCamera->Append( m_mnuViewTop );
	
	m_mnuView->Append( -1, wxT("&Camera Position"), m_mnuViewCamera );
	
	wxMenuItem* m_separator5;
	m_separator5 = m_mnuView->AppendSeparator();
	
	wxMenuItem* m_mnuToolbarText;
	m_mnuToolbarText = new wxMenuItem( m_mnuView, ID_MNU_VIEW_TOOLBARTEXT, wxString( wxT("Show &Text Under Toolbar Icons") ) , wxT("Displays text below toolbar icons"), wxITEM_CHECK );
	m_mnuView->Append( m_mnuToolbarText );
	m_mnuToolbarText->Check( true );
	
	wxMenuItem* m_mnuViewFullscreen;
	m_mnuViewFullscreen = new wxMenuItem( m_mnuView, ID_MNU_FULL_SCREEN, wxString( wxT("&Full Screen") ) + wxT('\t') + wxT("Ctrl+Alt+F"), wxEmptyString, wxITEM_CHECK );
	m_mnuView->Append( m_mnuViewFullscreen );
	
	m_menubar->Append( m_mnuView, wxT("&View") ); 
	
	m_mnuLayers = new wxMenu();
	wxMenuItem* m_mnuLayerMap;
	m_mnuLayerMap = new wxMenuItem( m_mnuLayers, ID_MNU_LAYER_ADD_MAP, wxString( wxT("Add &Raster Map") ) + wxT('\t') + wxT("Ctrl+M"), wxT("Open digital elevation map"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuLayerMap->SetBitmaps( wxBitmap( DEM_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuLayerMap->SetBitmap( wxBitmap( DEM_xpm ) );
	#endif
	m_mnuLayers->Append( m_mnuLayerMap );
	
	wxMenuItem* m_mnuLayerVectorMap;
	m_mnuLayerVectorMap = new wxMenuItem( m_mnuLayers, ID_MNU_LAYER_ADD_VECTOR_MAP, wxString( wxT("Add Vector Map") ) + wxT('\t') + wxT("Ctrl+Shift+V"), wxT("Open vector map"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuLayerVectorMap->SetBitmaps( wxBitmap( shapefile_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuLayerVectorMap->SetBitmap( wxBitmap( shapefile_xpm ) );
	#endif
	m_mnuLayers->Append( m_mnuLayerVectorMap );
	
	wxMenuItem* m_mnuLayerLocations;
	m_mnuLayerLocations = new wxMenuItem( m_mnuLayers, ID_MNU_LAYER_ADD_LOCATIONS, wxString( wxT("Add &Location Set") ) + wxT('\t') + wxT("Ctrl+L"), wxT("Open georeferenced sample sites"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuLayerLocations->SetBitmaps( wxBitmap( Location_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuLayerLocations->SetBitmap( wxBitmap( Location_xpm ) );
	#endif
	m_mnuLayers->Append( m_mnuLayerLocations );
	
	wxMenuItem* m_mnuLayerSequenceData;
	m_mnuLayerSequenceData = new wxMenuItem( m_mnuLayers, ID_MNU_LAYER_SEQUENCE_DATA, wxString( wxT("Add Sequence &Data") ) + wxT('\t') + wxT("Ctrl+D"), wxT("Open sequence data"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuLayerSequenceData->SetBitmaps( wxBitmap( SampleData_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuLayerSequenceData->SetBitmap( wxBitmap( SampleData_xpm ) );
	#endif
	m_mnuLayers->Append( m_mnuLayerSequenceData );
	
	wxMenuItem* m_mnuLayerTree;
	m_mnuLayerTree = new wxMenuItem( m_mnuLayers, ID_MNU_LAYER_ADD_TREE, wxString( wxT("Add &Tree") ) + wxT('\t') + wxT("Ctrl+T"), wxT("Open tree"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuLayerTree->SetBitmaps( wxBitmap( phylogram_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuLayerTree->SetBitmap( wxBitmap( phylogram_xpm ) );
	#endif
	m_mnuLayers->Append( m_mnuLayerTree );
	
	wxMenuItem* m_menuLocationMerger;
	m_menuLocationMerger = new wxMenuItem( m_mnuLayers, wxID_ANY, wxString( wxT("Location Set &Merge") ) , wxT("Merge location set layers"), wxITEM_NORMAL );
	m_mnuLayers->Append( m_menuLocationMerger );
	
	wxMenuItem* m_separator7;
	m_separator7 = m_mnuLayers->AppendSeparator();
	
	wxMenuItem* m_mnuLayerRemove;
	m_mnuLayerRemove = new wxMenuItem( m_mnuLayers, ID_MNU_LAYER_REMOVE, wxString( wxT("&Remove Layer") ) + wxT('\t') + wxT("Ctrl+R"), wxT("Remove selected layer"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuLayerRemove->SetBitmaps( wxBitmap( close_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuLayerRemove->SetBitmap( wxBitmap( close_xpm ) );
	#endif
	m_mnuLayers->Append( m_mnuLayerRemove );
	
	wxMenuItem* m_mnuAllLayerRemove;
	m_mnuAllLayerRemove = new wxMenuItem( m_mnuLayers, ID_MNU_ALL_LAYER_REMOVE, wxString( wxT("&Remove All Layers") ) , wxT("Remove selected layer"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_mnuAllLayerRemove->SetBitmaps( wxBitmap( close_xpm ) );
	#elif defined( __WXGTK__ )
	m_mnuAllLayerRemove->SetBitmap( wxBitmap( close_xpm ) );
	#endif
	m_mnuLayers->Append( m_mnuAllLayerRemove );
	
	wxMenuItem* m_separator8;
	m_separator8 = m_mnuLayers->AppendSeparator();
	
	wxMenuItem* m_mnuLayerHideAll;
	m_mnuLayerHideAll = new wxMenuItem( m_mnuLayers, ID_MNU_LAYER_HIDE_ALL, wxString( wxT("&Hide All Layers") ) , wxT("Hide all layers"), wxITEM_NORMAL );
	m_mnuLayers->Append( m_mnuLayerHideAll );
	
	wxMenuItem* m_mnuLayerShowAll;
	m_mnuLayerShowAll = new wxMenuItem( m_mnuLayers, ID_MNU_LAYER_SHOW_ALL, wxString( wxT("&Show All Layers") ) , wxT("Show all layers"), wxITEM_NORMAL );
	m_mnuLayers->Append( m_mnuLayerShowAll );
	
	m_menubar->Append( m_mnuLayers, wxT("&Layer") ); 
	
	m_mnuSettings = new wxMenu();
	wxMenuItem* m_mnuSettingsFont;
	m_mnuSettingsFont = new wxMenuItem( m_mnuSettings, wxID_ANY, wxString( wxT("Label &Font Settings") ) , wxT("Set font properties"), wxITEM_NORMAL );
	m_mnuSettings->Append( m_mnuSettingsFont );
	
	wxMenuItem* m_mnuSettingsLighting;
	m_mnuSettingsLighting = new wxMenuItem( m_mnuSettings, ID_MNU_SETTINGS_LIGHTING, wxString( wxT("&Lighting Settings") ) , wxT("Set lighting properties"), wxITEM_NORMAL );
	m_mnuSettings->Append( m_mnuSettingsLighting );
	
	wxMenuItem* m_mnuSettingsLayoutObject;
	m_mnuSettingsLayoutObject = new wxMenuItem( m_mnuSettings, ID_MNU_LAYOUT_OBJECT_PROP, wxString( wxT("&Object Layout Settings") ) , wxT("Set layout object properties"), wxITEM_NORMAL );
	m_mnuSettings->Append( m_mnuSettingsLayoutObject );
	
	wxMenuItem* m_separator10;
	m_separator10 = m_mnuSettings->AppendSeparator();
	
	m_mnuRAutoConfigUtil = new wxMenuItem( m_mnuSettings, wxID_ANY, wxString( wxT("R Auto-Configuration Utility") ) , wxEmptyString, wxITEM_NORMAL );
	m_mnuSettings->Append( m_mnuRAutoConfigUtil );
	
	wxMenuItem* m_separator81;
	m_separator81 = m_mnuSettings->AppendSeparator();
	
	wxMenuItem* m_mnuSettingsShowWelcomeScreen;
	m_mnuSettingsShowWelcomeScreen = new wxMenuItem( m_mnuSettings, ID_MNU_SHOW_WELCOME, wxString( wxT("Show &Welcome Screen on Startup") ) , wxEmptyString, wxITEM_CHECK );
	m_mnuSettings->Append( m_mnuSettingsShowWelcomeScreen );
	
	m_menubar->Append( m_mnuSettings, wxT("&Settings") ); 
	
	m_mnuAnalysis = new wxMenu();
	wxMenuItem* m_mnuAnalysisActiveSequences;
	m_mnuAnalysisActiveSequences = new wxMenuItem( m_mnuAnalysis, wxID_ANY, wxString( wxT("&Select Sequence Dialog") ) , wxEmptyString, wxITEM_NORMAL );
	m_mnuAnalysis->Append( m_mnuAnalysisActiveSequences );
	
	m_menubar->Append( m_mnuAnalysis, wxT("&Analysis") ); 
	
	m_mnuData = new wxMenu();
	m_menubar->Append( m_mnuData, wxT("&Data") ); 
	
	m_mnuPlugins = new wxMenu();
	m_menubar->Append( m_mnuPlugins, wxT("&Plugins") ); 
	
	m_mnuHelp = new wxMenu();
	wxMenuItem* m_menuHelpManual;
	m_menuHelpManual = new wxMenuItem( m_mnuHelp, wxID_ANY, wxString( wxT("GenGIS &Manual") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuHelpManual->SetBitmaps( wxBitmap( manual_xpm ) );
	#elif defined( __WXGTK__ )
	m_menuHelpManual->SetBitmap( wxBitmap( manual_xpm ) );
	#endif
	m_mnuHelp->Append( m_menuHelpManual );
	
	wxMenuItem* m_menuCheckForUpdates;
	m_menuCheckForUpdates = new wxMenuItem( m_mnuHelp, wxID_ANY, wxString( wxT("Check for &Updates") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuCheckForUpdates->SetBitmaps( wxBitmap( checkmark_xpm ) );
	#elif defined( __WXGTK__ )
	m_menuCheckForUpdates->SetBitmap( wxBitmap( checkmark_xpm ) );
	#endif
	m_mnuHelp->Append( m_menuCheckForUpdates );
	
	wxMenuItem* m_separator91;
	m_separator91 = m_mnuHelp->AppendSeparator();
	
	wxMenuItem* m_mnuHelpAbout;
	m_mnuHelpAbout = new wxMenuItem( m_mnuHelp, wxID_ABOUT, wxString( wxT("&About...") ) , wxT("Information about program"), wxITEM_NORMAL );
	m_mnuHelp->Append( m_mnuHelpAbout );
	
	m_menubar->Append( m_mnuHelp, wxT("&Help") ); 
	
	this->SetMenuBar( m_menubar );
	
	m_toolBar = this->CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL|wxTB_TEXT|wxCLIP_CHILDREN, ID_TOOLBAR ); 
	m_toolBar->AddTool( ID_TB_OPEN_SESSION, wxT("Open Session"), wxBitmap( fileopensession_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Open Session"), wxT("Opens previous configuration settings") ); 
	m_toolBar->AddSeparator(); 
	m_toolBar->AddTool( ID_TB_LAYER_OPEN_DEM, wxT("Load Raster Map"), wxBitmap( DEM_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Open digital map"), wxT("Open digital map") ); 
	m_toolBar->AddTool( ID_TB_LAYER_OPEN_VECTOR, wxT("Load Vector Map"), wxBitmap( shapefile_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Open vector map"), wxT("Open vector map") ); 
	m_toolBar->AddTool( ID_TB_LAYER_OPEN_LOCATIONS, wxT("Load Locations"), wxBitmap( Location_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Open georeferenced sample sites"), wxT("Open georeferenced sample sites") ); 
	m_toolBar->AddTool( ID_TB_LAYER_OPEN_SAMPLE_DATA, wxT("Load Sequences"), wxBitmap( SampleData_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Open sequence data"), wxT("Open sequence data") ); 
	m_toolBar->AddTool( ID_TB_LAYER_OPEN_TREE, wxT("Load Tree"), wxBitmap( phylogram_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Open tree"), wxT("Open tree") ); 
	m_toolBar->AddSeparator(); 
	m_toolBar->AddTool( ID_VIEW_RESET, wxT("Perspective View"), wxBitmap( Reset_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Reset viewpoint"), wxT("Reset viewpoint") ); 
	m_toolBar->AddTool( ID_VIEW_TOP, wxT("Top View"), wxBitmap( top_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("View from top"), wxT("View from top") ); 
	m_toolBar->AddSeparator(); 
	m_toolBar->AddTool( ID_TB_MODE_DRAW_LAYOUT_LINE, wxT("Layout Line"), wxBitmap( line_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Draw layout line"), wxT("Draw layout line") ); 
	m_toolBar->AddTool( ID_TB_MODE_DRAW_LAYOUT_ELLIPSE, wxT("Layout Ellipse"), wxBitmap( ellipse_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Draw layout ellipse"), wxT("Draw layout ellipse") ); 
	m_toolBar->AddSeparator(); 
	m_toolBar->AddTool( ID_TB_GEO_AXIS_BY_POLYLINE, wxT("Geographic Axis"), wxBitmap( polyline_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Define geographic axis"), wxT("Define geographic axis using a polyline") ); 
	m_toolBar->AddSeparator(); 
	m_toolBar->AddTool( ID_TB_LABEL_TOOL, wxT("Label Tool"), wxBitmap( Label_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Label tool"), wxT("Label tool") ); 
	m_toolBar->Realize();
	
	m_statusBar = this->CreateStatusBar( 4, wxST_SIZEGRIP|wxCLIP_CHILDREN, ID_STATUSBAR );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CHAR, wxKeyEventHandler( GenGisLayout::OnCharFromKeyboard ) );
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GenGisLayout::OnWindowClose ) );
	m_treeCtrlLayer->Connect( wxEVT_CHAR, wxKeyEventHandler( GenGisLayout::OnCharFromKeyboard ), NULL, this );
	m_treeCtrlLayer->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( GenGisLayout::OnLayerTreeLeftDClick ), NULL, this );
	m_treeCtrlLayer->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GenGisLayout::OnLayerTreeClick ), NULL, this );
	m_treeCtrlLayer->Connect( wxEVT_COMMAND_TREE_ITEM_MENU, wxTreeEventHandler( GenGisLayout::OnLayerTreePopupMenu ), NULL, this );
	m_treeCtrlLayer->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( GenGisLayout::OnLayerTreeSelect ), NULL, this );
	m_locationSetChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GenGisLayout::OnSamplesLegendLocationSetSelect ), NULL, this );
	m_console->Connect( wxEVT_CHAR, wxKeyEventHandler( GenGisLayout::OnCharFromKeyboard ), NULL, this );
	m_console->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GenGisLayout::OnConsoleEnter ), NULL, this );
	this->Connect( m_mnuFileNewSession->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFileNewSession ) );
	this->Connect( m_mnuFileOpenSession->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFileOpenSession ) );
	this->Connect( m_clearHistory->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnClearSessionHistory ) );
	this->Connect( m_mnuFileSaveSession->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFileSaveSession ) );
	this->Connect( m_mnuFileSaveSessionAs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFileSaveSessionAs ) );
	this->Connect( m_mnuFileSaveImageAs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFileSaveImageAs ) );
	this->Connect( m_mnuFileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFileExit ) );
	this->Connect( m_mnuViewSidePanel->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewSidePanel ) );
	this->Connect( m_mnuViewConsole->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewConsole ) );
	this->Connect( m_mnuViewNav->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewNav ) );
	this->Connect( m_mnuViewMiniMap->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewMiniMap ) );
	this->Connect( m_mnuViewCompass->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewCompass ) );
	this->Connect( m_mnuViewWidgetsShowAll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewShowWidgets ) );
	this->Connect( m_mnuViewWidgetsHideAll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewHideWidgets ) );
	this->Connect( m_mnuViewDefault->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewReset ) );
	this->Connect( m_mnuViewTop->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewTop ) );
	this->Connect( m_mnuToolbarText->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewShowToolbarText ) );
	this->Connect( m_mnuViewFullscreen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewFullScreen ) );
	this->Connect( m_mnuLayerMap->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerOpenMap ) );
	this->Connect( m_mnuLayerVectorMap->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerOpenVectorMap ) );
	this->Connect( m_mnuLayerLocations->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerOpenLocations ) );
	this->Connect( m_mnuLayerSequenceData->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerOpenSequenceData ) );
	this->Connect( m_mnuLayerTree->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerOpenTree ) );
	this->Connect( m_menuLocationMerger->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLocationMerge ) );
	this->Connect( m_mnuLayerRemove->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerRemove ) );
	this->Connect( m_mnuAllLayerRemove->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnAllLayersRemove ) );
	this->Connect( m_mnuLayerHideAll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerHideAll ) );
	this->Connect( m_mnuLayerShowAll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerShowAll ) );
	this->Connect( m_mnuSettingsFont->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFontSettingsDlg ) );
	this->Connect( m_mnuSettingsLighting->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLightingSettingsDlg ) );
	this->Connect( m_mnuSettingsLayoutObject->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayoutObjectSettingsDlg ) );
	this->Connect( m_mnuRAutoConfigUtil->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnRAutoConfigurationUtility ) );
	this->Connect( m_mnuSettingsShowWelcomeScreen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnShowWelcomeScreenOnStartupDlg ) );
	this->Connect( m_mnuAnalysisActiveSequences->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewSelectSeqDlg ) );
	this->Connect( m_menuHelpManual->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnHelpManual ) );
	this->Connect( m_menuCheckForUpdates->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnCheckForUpdates ) );
	this->Connect( m_mnuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnHelpAbout ) );
	this->Connect( ID_TB_OPEN_SESSION, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnFileOpenSession ) );
	this->Connect( ID_TB_LAYER_OPEN_DEM, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnLayerOpenMap ) );
	this->Connect( ID_TB_LAYER_OPEN_VECTOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnLayerOpenVectorMap ) );
	this->Connect( ID_TB_LAYER_OPEN_LOCATIONS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnLayerOpenLocations ) );
	this->Connect( ID_TB_LAYER_OPEN_SAMPLE_DATA, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnLayerOpenSequenceData ) );
	this->Connect( ID_TB_LAYER_OPEN_TREE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnLayerOpenTree ) );
	this->Connect( ID_VIEW_RESET, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnViewReset ) );
	this->Connect( ID_VIEW_TOP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnViewTop ) );
	this->Connect( ID_TB_MODE_DRAW_LAYOUT_LINE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnModeDrawLayoutLine ) );
	this->Connect( ID_TB_MODE_DRAW_LAYOUT_ELLIPSE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnModeDrawLayoutEllipse ) );
	this->Connect( ID_TB_GEO_AXIS_BY_POLYLINE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnGeoAxisByPolyline ) );
	this->Connect( ID_TB_LABEL_TOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnLabelTool ) );
}

GenGisLayout::~GenGisLayout()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CHAR, wxKeyEventHandler( GenGisLayout::OnCharFromKeyboard ) );
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GenGisLayout::OnWindowClose ) );
	m_treeCtrlLayer->Disconnect( wxEVT_CHAR, wxKeyEventHandler( GenGisLayout::OnCharFromKeyboard ), NULL, this );
	m_treeCtrlLayer->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( GenGisLayout::OnLayerTreeLeftDClick ), NULL, this );
	m_treeCtrlLayer->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GenGisLayout::OnLayerTreeClick ), NULL, this );
	m_treeCtrlLayer->Disconnect( wxEVT_COMMAND_TREE_ITEM_MENU, wxTreeEventHandler( GenGisLayout::OnLayerTreePopupMenu ), NULL, this );
	m_treeCtrlLayer->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( GenGisLayout::OnLayerTreeSelect ), NULL, this );
	m_locationSetChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GenGisLayout::OnSamplesLegendLocationSetSelect ), NULL, this );
	m_console->Disconnect( wxEVT_CHAR, wxKeyEventHandler( GenGisLayout::OnCharFromKeyboard ), NULL, this );
	m_console->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GenGisLayout::OnConsoleEnter ), NULL, this );
	this->Disconnect( ID_MNU_FILE_NEW_SESSION, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFileNewSession ) );
	this->Disconnect( ID_MNU_FILE_OPEN_SESSION, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFileOpenSession ) );
	this->Disconnect( ID_MNU_FILE_RECENT_SESSION_CLEAR_HISTORY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnClearSessionHistory ) );
	this->Disconnect( ID_MNU_FILE_SAVE_SESSION, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFileSaveSession ) );
	this->Disconnect( ID_MNU_FILE_SAVE_SESSION_AS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFileSaveSessionAs ) );
	this->Disconnect( ID_MNU_FILE_SAVE_IMAGE_AS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFileSaveImageAs ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFileExit ) );
	this->Disconnect( ID_MNU_VIEW_SIDE_PANEL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewSidePanel ) );
	this->Disconnect( ID_MNU_VIEW_CONSOLE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewConsole ) );
	this->Disconnect( ID_MNU_VIEW_NAV, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewNav ) );
	this->Disconnect( ID_MNU_VIEW_MINIMAP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewMiniMap ) );
	this->Disconnect( ID_MNU_VIEW_COMPASS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewCompass ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewShowWidgets ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewHideWidgets ) );
	this->Disconnect( ID_MNU_VIEW_DEFAULT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewReset ) );
	this->Disconnect( ID_MNU_VIEW_TOP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewTop ) );
	this->Disconnect( ID_MNU_VIEW_TOOLBARTEXT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewShowToolbarText ) );
	this->Disconnect( ID_MNU_FULL_SCREEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewFullScreen ) );
	this->Disconnect( ID_MNU_LAYER_ADD_MAP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerOpenMap ) );
	this->Disconnect( ID_MNU_LAYER_ADD_VECTOR_MAP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerOpenVectorMap ) );
	this->Disconnect( ID_MNU_LAYER_ADD_LOCATIONS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerOpenLocations ) );
	this->Disconnect( ID_MNU_LAYER_SEQUENCE_DATA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerOpenSequenceData ) );
	this->Disconnect( ID_MNU_LAYER_ADD_TREE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerOpenTree ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLocationMerge ) );
	this->Disconnect( ID_MNU_LAYER_REMOVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerRemove ) );
	this->Disconnect( ID_MNU_ALL_LAYER_REMOVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnAllLayersRemove ) );
	this->Disconnect( ID_MNU_LAYER_HIDE_ALL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerHideAll ) );
	this->Disconnect( ID_MNU_LAYER_SHOW_ALL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayerShowAll ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnFontSettingsDlg ) );
	this->Disconnect( ID_MNU_SETTINGS_LIGHTING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLightingSettingsDlg ) );
	this->Disconnect( ID_MNU_LAYOUT_OBJECT_PROP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnLayoutObjectSettingsDlg ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnRAutoConfigurationUtility ) );
	this->Disconnect( ID_MNU_SHOW_WELCOME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnShowWelcomeScreenOnStartupDlg ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnViewSelectSeqDlg ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnHelpManual ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnCheckForUpdates ) );
	this->Disconnect( wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GenGisLayout::OnHelpAbout ) );
	this->Disconnect( ID_TB_OPEN_SESSION, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnFileOpenSession ) );
	this->Disconnect( ID_TB_LAYER_OPEN_DEM, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnLayerOpenMap ) );
	this->Disconnect( ID_TB_LAYER_OPEN_VECTOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnLayerOpenVectorMap ) );
	this->Disconnect( ID_TB_LAYER_OPEN_LOCATIONS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnLayerOpenLocations ) );
	this->Disconnect( ID_TB_LAYER_OPEN_SAMPLE_DATA, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnLayerOpenSequenceData ) );
	this->Disconnect( ID_TB_LAYER_OPEN_TREE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnLayerOpenTree ) );
	this->Disconnect( ID_VIEW_RESET, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnViewReset ) );
	this->Disconnect( ID_VIEW_TOP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnViewTop ) );
	this->Disconnect( ID_TB_MODE_DRAW_LAYOUT_LINE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnModeDrawLayoutLine ) );
	this->Disconnect( ID_TB_MODE_DRAW_LAYOUT_ELLIPSE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnModeDrawLayoutEllipse ) );
	this->Disconnect( ID_TB_GEO_AXIS_BY_POLYLINE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnGeoAxisByPolyline ) );
	this->Disconnect( ID_TB_LABEL_TOOL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GenGisLayout::OnLabelTool ) );
	
}
