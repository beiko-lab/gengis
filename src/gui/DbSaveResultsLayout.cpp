///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "DbSaveResultsLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

DbSaveResultsLayout::DbSaveResultsLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Sample filename:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_txtSampleFilename = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_txtSampleFilename, 1, wxALL|wxEXPAND, 5 );
	
	m_btnSampleFilename = new wxButton( this, wxID_ANY, wxT("Select"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_btnSampleFilename, 0, wxALL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Sequence filename:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_txtSequenceFilename = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_txtSequenceFilename, 1, wxALL|wxEXPAND, 5 );
	
	m_btnSequenceFilename = new wxButton( this, wxID_ANY, wxT("Select"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_btnSequenceFilename, 0, wxALL, 5 );
	
	bSizer4->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnOK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_btnOK, 0, wxALL, 5 );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer4->Add( bSizer5, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer4 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_btnSampleFilename->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSaveResultsLayout::OnSampleFilename ), NULL, this );
	m_btnSequenceFilename->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSaveResultsLayout::OnSequenceFilename ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSaveResultsLayout::OnOK ), NULL, this );
}

DbSaveResultsLayout::~DbSaveResultsLayout()
{
	// Disconnect Events
	m_btnSampleFilename->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSaveResultsLayout::OnSampleFilename ), NULL, this );
	m_btnSequenceFilename->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSaveResultsLayout::OnSequenceFilename ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSaveResultsLayout::OnOK ), NULL, this );
	
}
