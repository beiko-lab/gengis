///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "PluginManagerLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

PluginManagerLayout::PluginManagerLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_grid = new wxGrid( this, ID_GRID, wxDefaultPosition, wxDefaultSize, 0 );
	
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
	m_grid->SetLabelBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_grid->SetLabelFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	// Cell Defaults
	m_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	bSizer7->Add( m_grid, 1, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnOK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_btnOK, 0, wxALL, 5 );
	
	bSizer7->Add( bSizer4, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer7 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_grid->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( PluginManagerLayout::OnGridRightClick ), NULL, this );
	m_grid->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( PluginManagerLayout::OnLabelLeftClick ), NULL, this );
	m_grid->Connect( wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler( PluginManagerLayout::OnLabelRightClick ), NULL, this );
}

PluginManagerLayout::~PluginManagerLayout()
{
	// Disconnect Events
	m_grid->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( PluginManagerLayout::OnGridRightClick ), NULL, this );
	m_grid->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( PluginManagerLayout::OnLabelLeftClick ), NULL, this );
	m_grid->Disconnect( wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler( PluginManagerLayout::OnLabelRightClick ), NULL, this );
	
}
