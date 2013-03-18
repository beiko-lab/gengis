///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "DbSelectSampleLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

DbSelectSampleLayout::DbSelectSampleLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->SetSashGravity( 0 );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( DbSelectSampleLayout::m_splitter1OnIdle ), NULL, this );
	m_splitter1->SetMinimumPaneSize( 100 );
	
	m_panel2 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* m_filterSizer;
	m_filterSizer = new wxStaticBoxSizer( new wxStaticBox( m_panel2, ID_FILTER_SIZER, wxT("Filter samples") ), wxVERTICAL );
	
	m_choiceBook = new wxChoicebook( m_panel2, ID_CHOICE_BOOK, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_filterSizer->Add( m_choiceBook, 1, wxEXPAND | wxALL, 5 );
	
	m_panel2->SetSizer( m_filterSizer );
	m_panel2->Layout();
	m_filterSizer->Fit( m_panel2 );
	m_tablePanel = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_grid = new wxGrid( m_tablePanel, ID_GRID, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_grid->CreateGrid( 5, 5 );
	m_grid->EnableEditing( false );
	m_grid->EnableGridLines( true );
	m_grid->EnableDragGridSize( false );
	m_grid->SetMargins( 0, 0 );
	
	// Columns
	m_grid->AutoSizeColumns();
	m_grid->EnableDragColMove( false );
	m_grid->EnableDragColSize( true );
	m_grid->SetColLabelSize( 30 );
	m_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_grid->SetRowSize( 0, 19 );
	m_grid->SetRowSize( 1, 19 );
	m_grid->SetRowSize( 2, 19 );
	m_grid->SetRowSize( 3, 19 );
	m_grid->SetRowSize( 4, 19 );
	m_grid->AutoSizeRows();
	m_grid->EnableDragRowSize( true );
	m_grid->SetRowLabelSize( 30 );
	m_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	m_grid->SetLabelBackgroundColour( wxColour( 255, 255, 255 ) );
	
	// Cell Defaults
	m_grid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer2->Add( m_grid, 1, wxALL|wxEXPAND, 5 );
	
	m_tablePanel->SetSizer( bSizer2 );
	m_tablePanel->Layout();
	bSizer2->Fit( m_tablePanel );
	m_splitter1->SplitVertically( m_panel2, m_tablePanel, 250 );
	bSizer7->Add( m_splitter1, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnFetchFromDB = new wxButton( this, wxID_ANY, wxT("Fetch samples from database"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnFetchFromDB, 0, wxALL, 5 );
	
	m_btnLoadSampleList = new wxButton( this, wxID_ANY, wxT("Load sample list"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnLoadSampleList, 0, wxALL, 5 );
	
	m_btnSaveSamples = new wxButton( this, wxID_ANY, wxT("Save sample list"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnSaveSamples, 0, wxALL, 5 );
	
	
	bSizer9->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnRestore = new wxButton( this, ID_BTN_RESTORE, wxT("Restore all samples"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnRestore, 0, wxALL, 5 );
	
	
	bSizer9->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnContinue = new wxButton( this, ID_BTN_CONTINUE, wxT("Continue"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnContinue->SetDefault(); 
	bSizer9->Add( m_btnContinue, 0, wxALL, 5 );
	
	m_btnCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnCancel, 0, wxALL, 5 );
	
	bSizer7->Add( bSizer9, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer7 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_choiceBook->Connect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( DbSelectSampleLayout::OnFieldChanged ), NULL, this );
	m_grid->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( DbSelectSampleLayout::OnGridRightClick ), NULL, this );
	m_grid->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( DbSelectSampleLayout::OnLabelLeftClick ), NULL, this );
	m_grid->Connect( wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler( DbSelectSampleLayout::OnLabelRightClick ), NULL, this );
	m_btnFetchFromDB->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleLayout::OnFetchSamplesFromDB ), NULL, this );
	m_btnLoadSampleList->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleLayout::OnLoadSampleList ), NULL, this );
	m_btnSaveSamples->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleLayout::OnSaveSampleList ), NULL, this );
	m_btnRestore->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleLayout::OnRestoreAllSamples ), NULL, this );
	m_btnContinue->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleLayout::OnContinue ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleLayout::OnCancel ), NULL, this );
}

DbSelectSampleLayout::~DbSelectSampleLayout()
{
	// Disconnect Events
	m_choiceBook->Disconnect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( DbSelectSampleLayout::OnFieldChanged ), NULL, this );
	m_grid->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( DbSelectSampleLayout::OnGridRightClick ), NULL, this );
	m_grid->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( DbSelectSampleLayout::OnLabelLeftClick ), NULL, this );
	m_grid->Disconnect( wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler( DbSelectSampleLayout::OnLabelRightClick ), NULL, this );
	m_btnFetchFromDB->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleLayout::OnFetchSamplesFromDB ), NULL, this );
	m_btnLoadSampleList->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleLayout::OnLoadSampleList ), NULL, this );
	m_btnSaveSamples->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleLayout::OnSaveSampleList ), NULL, this );
	m_btnRestore->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleLayout::OnRestoreAllSamples ), NULL, this );
	m_btnContinue->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleLayout::OnContinue ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleLayout::OnCancel ), NULL, this );
	
}
