///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "AboutLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

AboutLayout::AboutLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_bitmap = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_bitmap, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_scrolledWindow1 = new wxScrolledWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_htmlReleaseNotes = new wxHtmlWindow( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxSize( 560,400 ), wxHW_SCROLLBAR_AUTO );
	bSizer2->Add( m_htmlReleaseNotes, 1, wxEXPAND, 5 );
	
	m_scrolledWindow1->SetSizer( bSizer2 );
	m_scrolledWindow1->Layout();
	bSizer2->Fit( m_scrolledWindow1 );
	m_notebook->AddPage( m_scrolledWindow1, wxT("Release Notes"), true );
	m_scrolledWindow2 = new wxScrolledWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow2->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_htmlAuthors = new wxHtmlWindow( m_scrolledWindow2, wxID_ANY, wxDefaultPosition, wxSize( 560,400 ), wxHW_SCROLLBAR_AUTO );
	bSizer3->Add( m_htmlAuthors, 1, wxEXPAND, 5 );
	
	m_scrolledWindow2->SetSizer( bSizer3 );
	m_scrolledWindow2->Layout();
	bSizer3->Fit( m_scrolledWindow2 );
	m_notebook->AddPage( m_scrolledWindow2, wxT("Authors"), false );
	m_scrolledWindow3 = new wxScrolledWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow3->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_htmlLicense = new wxHtmlWindow( m_scrolledWindow3, wxID_ANY, wxDefaultPosition, wxSize( 560,400 ), wxHW_SCROLLBAR_AUTO );
	bSizer4->Add( m_htmlLicense, 1, wxEXPAND, 5 );
	
	m_scrolledWindow3->SetSizer( bSizer4 );
	m_scrolledWindow3->Layout();
	bSizer4->Fit( m_scrolledWindow3 );
	m_notebook->AddPage( m_scrolledWindow3, wxT("License"), false );
	
	bSizer1->Add( m_notebook, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticTextGenGisVersion = new wxStaticText( this, wxID_ANY, wxT("GenGIS Version"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGenGisVersion->Wrap( -1 );
	bSizer5->Add( m_staticTextGenGisVersion, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_button = new wxButton( this, wxID_CANCEL, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer1->Add( bSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_htmlReleaseNotes->Connect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler( AboutLayout::OnLinkClicked ), NULL, this );
	m_htmlAuthors->Connect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler( AboutLayout::OnLinkClicked ), NULL, this );
	m_htmlLicense->Connect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler( AboutLayout::OnLinkClicked ), NULL, this );
	m_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutLayout::OnClose ), NULL, this );
}

AboutLayout::~AboutLayout()
{
	// Disconnect Events
	m_htmlReleaseNotes->Disconnect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler( AboutLayout::OnLinkClicked ), NULL, this );
	m_htmlAuthors->Disconnect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler( AboutLayout::OnLinkClicked ), NULL, this );
	m_htmlLicense->Disconnect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler( AboutLayout::OnLinkClicked ), NULL, this );
	m_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutLayout::OnClose ), NULL, this );
	
}
