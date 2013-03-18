///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "FontPropertiesLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

FontPropertiesLayout::FontPropertiesLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Label font:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer8->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_cboFont = new wxComboBox( this, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_cboFont->Append( wxT("Arial") );
	m_cboFont->Append( wxT("Courier") );
	m_cboFont->Append( wxT("Times New Roman") );
	m_cboFont->Append( wxT("Times New Roman (bold)") );
	m_cboFont->Append( wxT("Verdana") );
	bSizer8->Add( m_cboFont, 0, wxALL|wxEXPAND, 5 );
	
	bSizer7->Add( bSizer8, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer9->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnOK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnOK, 0, wxALL, 5 );
	
	m_btnApply = new wxButton( this, ID_BTN_APPLY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnApply, 0, wxALL, 5 );
	
	m_btnCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnCancel->SetDefault(); 
	bSizer9->Add( m_btnCancel, 0, wxALL, 5 );
	
	bSizer7->Add( bSizer9, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer7 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPropertiesLayout::OnCancel ), NULL, this );
}

FontPropertiesLayout::~FontPropertiesLayout()
{
	// Disconnect Events
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPropertiesLayout::OnCancel ), NULL, this );
	
}
