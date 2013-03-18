///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "DbSelectAttributesLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

DbSelectAttributesLayout::DbSelectAttributesLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter->SetSashGravity( 0 );
	m_splitter->Connect( wxEVT_IDLE, wxIdleEventHandler( DbSelectAttributesLayout::m_splitterOnIdle ), NULL, this );
	m_splitter->SetMinimumPaneSize( 100 );
	
	m_panel2 = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* m_dataSizer;
	m_dataSizer = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, wxT("Filter attribute") ), wxVERTICAL );
	
	m_choiceBook = new wxChoicebook( m_panel2, ID_CHOICE_BOOK, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_dataSizer->Add( m_choiceBook, 1, wxEXPAND | wxALL, 5 );
	
	m_panel2->SetSizer( m_dataSizer );
	m_panel2->Layout();
	m_dataSizer->Fit( m_panel2 );
	m_tablePanel = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
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
	m_splitter->SplitVertically( m_panel2, m_tablePanel, 250 );
	bSizer7->Add( m_splitter, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnRestore = new wxButton( this, ID_BTN_RESTORE, wxT("Restore original table"), wxDefaultPosition, wxDefaultSize, 0 );
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
	m_choiceBook->Connect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( DbSelectAttributesLayout::OnFieldChanged ), NULL, this );
	m_choiceBook->Connect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGING, wxChoicebookEventHandler( DbSelectAttributesLayout::OnFieldChanging ), NULL, this );
	m_grid->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( DbSelectAttributesLayout::OnGridRightClick ), NULL, this );
	m_grid->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( DbSelectAttributesLayout::OnLabelLeftClick ), NULL, this );
	m_grid->Connect( wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler( DbSelectAttributesLayout::OnLabelRightClick ), NULL, this );
	m_btnRestore->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectAttributesLayout::OnRestoreOriginalTable ), NULL, this );
	m_btnContinue->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectAttributesLayout::OnContinue ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectAttributesLayout::OnCancel ), NULL, this );
}

DbSelectAttributesLayout::~DbSelectAttributesLayout()
{
	// Disconnect Events
	m_choiceBook->Disconnect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxChoicebookEventHandler( DbSelectAttributesLayout::OnFieldChanged ), NULL, this );
	m_choiceBook->Disconnect( wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGING, wxChoicebookEventHandler( DbSelectAttributesLayout::OnFieldChanging ), NULL, this );
	m_grid->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( DbSelectAttributesLayout::OnGridRightClick ), NULL, this );
	m_grid->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( DbSelectAttributesLayout::OnLabelLeftClick ), NULL, this );
	m_grid->Disconnect( wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler( DbSelectAttributesLayout::OnLabelRightClick ), NULL, this );
	m_btnRestore->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectAttributesLayout::OnRestoreOriginalTable ), NULL, this );
	m_btnContinue->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectAttributesLayout::OnContinue ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectAttributesLayout::OnCancel ), NULL, this );
	
}
