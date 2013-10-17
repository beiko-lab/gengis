///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "LocationMergeLayout.h"

///////////////////////////////////////////////////////////////////////////

LocationMergeLayout::LocationMergeLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 260,160 ), wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_locationSetCheckboxChoices;
	m_locationSetCheckbox = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_locationSetCheckboxChoices, wxLB_MULTIPLE|wxLB_NEEDED_SB );
	bSizer1->Add( m_locationSetCheckbox, 0, wxEXPAND|wxALL, 5 );
	
	m_remove = new wxCheckBox( this, wxID_ANY, wxT("Remove layers after combination."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_remove, 0, wxALL, 5 );
	
	m_description = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_description->Wrap( 260 );
	bSizer1->Add( m_description, 0, wxALL, 5 );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 1, 2, 0, 0 );
	
	m_OK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_OK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_Close = new wxButton( this, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_Close, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	bSizer1->Add( gSizer1, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationMergeLayout::OnOK ), NULL, this );
	m_Close->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationMergeLayout::OnClose ), NULL, this );
}

LocationMergeLayout::~LocationMergeLayout()
{
	// Disconnect Events
	m_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationMergeLayout::OnOK ), NULL, this );
	m_Close->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationMergeLayout::OnClose ), NULL, this );
	
}
