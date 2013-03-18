///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "SaveImageLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

SaveImageLayout::SaveImageLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextFilename = new wxStaticText( this, wxID_ANY, wxT("Filename:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFilename->Wrap( -1 );
	fgSizer1->Add( m_staticTextFilename, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_txtFilename = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_txtFilename, 1, wxALL|wxEXPAND, 5 );
	
	m_btnSelectFilename = new wxButton( this, wxID_ANY, wxT("Select a location..."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_btnSelectFilename, 0, wxALL, 5 );
	
	m_staticTextResolution = new wxStaticText( this, wxID_ANY, wxT("Resolution:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextResolution->Wrap( -1 );
	fgSizer1->Add( m_staticTextResolution, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_cboResolution = new wxComboBox( this, wxID_ANY, wxT("Native screen resolution (smallest file size)"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboResolution->Append( wxT("Native screen resolution (smallest file size)") );
	m_cboResolution->Append( wxT("2 x screen resolution (recommended for review papers and websites)") );
	m_cboResolution->Append( wxT("3 x screen resolution (recommended for papers)") );
	m_cboResolution->Append( wxT("4 x screen resolution (recommended for posters)") );
	m_cboResolution->Append( wxT("5 x screen resolution") );
	m_cboResolution->Append( wxT("10 x screen resolution") );
	fgSizer1->Add( m_cboResolution, 0, wxALL, 5 );
	
	bSizer4->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	m_staticTextErrorMessage = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextErrorMessage->Wrap( -1 );
	bSizer4->Add( m_staticTextErrorMessage, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_openFileLocation = new wxCheckBox( this, wxID_ANY, wxT("Open file location after saving"), wxDefaultPosition, wxDefaultSize, 0 );
	m_openFileLocation->SetValue(true); 
	bSizer5->Add( m_openFileLocation, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnOK = new wxButton( this, wxID_OK, wxT("Export"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnOK->SetDefault(); 
	bSizer5->Add( m_btnOK, 0, wxALL, 5 );
	
	m_btnCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_btnCancel, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer5, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer4 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_btnSelectFilename->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageLayout::OnSelectFilename ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageLayout::OnOK ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageLayout::OnCancel ), NULL, this );
}

SaveImageLayout::~SaveImageLayout()
{
	// Disconnect Events
	m_btnSelectFilename->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageLayout::OnSelectFilename ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageLayout::OnOK ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SaveImageLayout::OnCancel ), NULL, this );
	
}
