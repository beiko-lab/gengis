///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "UpdateGenGISLayout.h"

///////////////////////////////////////////////////////////////////////////

UpdateGenGISLayout::UpdateGenGISLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_htmlUpdateMessage = new wxHtmlWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO );
	m_htmlUpdateMessage->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	bSizer1->Add( m_htmlUpdateMessage, 1, wxALL|wxEXPAND, 5 );
	
	m_checkBoxSendOSInformation = new wxCheckBox( this, wxID_ANY, wxT("Tell the GenGIS team what OS you are using (Mac or PC)?"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxSendOSInformation->SetValue(true); 
	bSizer1->Add( m_checkBoxSendOSInformation, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOpenGenGISDownloadPage = new wxButton( this, wxID_ANY, wxT("Take me to the download page (&Yes)"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonOpenGenGISDownloadPage, 0, wxALL, 5 );
	
	m_buttonCloseDialog = new wxButton( this, wxID_ANY, wxT("I will update later (&No)"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonCloseDialog, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 0, wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonOpenGenGISDownloadPage->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UpdateGenGISLayout::OpenDownloadPage ), NULL, this );
	m_buttonCloseDialog->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UpdateGenGISLayout::OnClose ), NULL, this );
}

UpdateGenGISLayout::~UpdateGenGISLayout()
{
	// Disconnect Events
	m_buttonOpenGenGISDownloadPage->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UpdateGenGISLayout::OpenDownloadPage ), NULL, this );
	m_buttonCloseDialog->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UpdateGenGISLayout::OnClose ), NULL, this );
	
}
