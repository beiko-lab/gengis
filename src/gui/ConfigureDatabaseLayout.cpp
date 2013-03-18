///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "ConfigureDatabaseLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

ConfigureDatabaseLayout::ConfigureDatabaseLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("MOA:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer2->Add( m_staticText1, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString m_choiceMOAChoices[] = { wxT("http://ratite.cs.dal.ca/mg_web/fetch_metagenome_samples.cgi?type=metagenome"), wxT("http://ratite.cs.dal.ca/mg_web/fetch_metagenome_samples.cgi?type=genome"), wxT("<Custom>") };
	int m_choiceMOANChoices = sizeof( m_choiceMOAChoices ) / sizeof( wxString );
	m_choiceMOA = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceMOANChoices, m_choiceMOAChoices, 0 );
	m_choiceMOA->SetSelection( 0 );
	bSizer2->Add( m_choiceMOA, 1, wxALL|wxEXPAND, 5 );
	
	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Custom:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer3->Add( m_staticText2, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_txtCustomStr = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer3->Add( m_txtCustomStr, 1, wxALL|wxEXPAND, 5 );
	
	bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnOK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_btnOK, 0, wxALL, 5 );
	
	m_btnCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_btnCancel, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer4, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_txtCustomStr->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ConfigureDatabaseLayout::OnTextChanged ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigureDatabaseLayout::OnOK ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigureDatabaseLayout::OnCancel ), NULL, this );
}

ConfigureDatabaseLayout::~ConfigureDatabaseLayout()
{
	// Disconnect Events
	m_txtCustomStr->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ConfigureDatabaseLayout::OnTextChanged ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigureDatabaseLayout::OnOK ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigureDatabaseLayout::OnCancel ), NULL, this );
	
}
