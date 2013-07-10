///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "WelcomeLayout.h"

#include "../../bin/images/close.xpm"
#include "../../bin/images/filenewsession.xpm"
#include "../../bin/images/fileopensession.xpm"
#include "../../bin/images/manual.xpm"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

WelcomeLayout::WelcomeLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_bitmap = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_bitmap, 0, wxEXPAND, 50 );
	
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Previous Sessions") ), wxVERTICAL );
	
	m_listBoxRecentSessions = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( 545,500 ), 0, NULL, wxLB_HSCROLL ); 
	m_listBoxRecentSessions->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	sbSizer1->Add( m_listBoxRecentSessions, 1, wxALL|wxEXPAND, 5 );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_button1, 0, wxALIGN_RIGHT|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	bSizer81->Add( sbSizer1, 1, wxEXPAND|wxALL, 5 );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 2, 2, 0, 0 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_newSession = new wxBitmapButton( this, wxID_ANY, wxBitmap( filenewsession_xpm ), wxDefaultPosition, wxSize( 30,30 ), 0 );
	m_newSession->SetDefault(); 
	bSizer9->Add( m_newSession, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	m_buttonNewSession = new wxButton( this, wxID_ANY, wxT("New Session"), wxDefaultPosition, wxSize( 150,25 ), 0 );
	m_buttonNewSession->SetFont( wxFont( 11, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer9->Add( m_buttonNewSession, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer9->Add( 10, 0, 0, 0, 5 );
	
	gSizer2->Add( bSizer9, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer8->Add( 10, 0, 0, 0, 5 );
	
	m_openSession = new wxBitmapButton( this, wxID_ANY, wxBitmap( fileopensession_xpm ), wxDefaultPosition, wxSize( 30,30 ), 0 );
	bSizer8->Add( m_openSession, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	m_buttonOpenSession = new wxButton( this, wxID_ANY, wxT("Open Session"), wxDefaultPosition, wxSize( 150,25 ), 0 );
	m_buttonOpenSession->SetFont( wxFont( 11, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer8->Add( m_buttonOpenSession, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer8->Add( 10, 0, 0, wxEXPAND, 5 );
	
	gSizer2->Add( bSizer8, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_bpButton4 = new wxBitmapButton( this, wxID_ANY, wxBitmap( manual_xpm ), wxDefaultPosition, wxSize( 30,30 ), 0 );
	bSizer7->Add( m_bpButton4, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	m_button5 = new wxButton( this, wxID_ANY, wxT("GenGIS Manual"), wxDefaultPosition, wxSize( 150,25 ), 0 );
	m_button5->SetFont( wxFont( 11, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer7->Add( m_button5, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer7->Add( 10, 0, 0, 0, 5 );
	
	gSizer2->Add( bSizer7, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer10->Add( 10, 0, 0, 0, 5 );
	
	m_exit = new wxBitmapButton( this, wxID_ANY, wxBitmap( close_xpm ), wxDefaultPosition, wxSize( 30,30 ), 0 );
	bSizer10->Add( m_exit, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	m_buttonExit = new wxButton( this, wxID_ANY, wxT("Exit GenGIS"), wxDefaultPosition, wxSize( 150,25 ), 0 );
	m_buttonExit->SetFont( wxFont( 11, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer10->Add( m_buttonExit, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer10->Add( 10, 0, 0, wxEXPAND, 5 );
	
	gSizer2->Add( bSizer10, 1, wxEXPAND, 5 );
	
	bSizer81->Add( gSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 0 );
	
	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBoxShowOnStartup = new wxCheckBox( this, wxID_ANY, wxT("Show this window on startup"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxShowOnStartup->SetValue(true); 
	bSizer141->Add( m_checkBoxShowOnStartup, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	bSizer141->Add( 10, 0, 0, wxALIGN_RIGHT, 5 );
	
	bSizer81->Add( bSizer141, 0, wxALIGN_RIGHT, 5 );
	
	bSizer1->Add( bSizer81, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_listBoxRecentSessions->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( WelcomeLayout::OnRecentSession ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnOpenRecentSession ), NULL, this );
	m_newSession->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnNewSession ), NULL, this );
	m_buttonNewSession->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnNewSession ), NULL, this );
	m_openSession->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnOpenSession ), NULL, this );
	m_buttonOpenSession->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnOpenSession ), NULL, this );
	m_bpButton4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnGenGISManual ), NULL, this );
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnGenGISManual ), NULL, this );
	m_exit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnExitGenGIS ), NULL, this );
	m_buttonExit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnExitGenGIS ), NULL, this );
	m_checkBoxShowOnStartup->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WelcomeLayout::OnCheckShowOnStartup ), NULL, this );
}

WelcomeLayout::~WelcomeLayout()
{
	// Disconnect Events
	m_listBoxRecentSessions->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( WelcomeLayout::OnRecentSession ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnOpenRecentSession ), NULL, this );
	m_newSession->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnNewSession ), NULL, this );
	m_buttonNewSession->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnNewSession ), NULL, this );
	m_openSession->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnOpenSession ), NULL, this );
	m_buttonOpenSession->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnOpenSession ), NULL, this );
	m_bpButton4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnGenGISManual ), NULL, this );
	m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnGenGISManual ), NULL, this );
	m_exit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnExitGenGIS ), NULL, this );
	m_buttonExit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WelcomeLayout::OnExitGenGIS ), NULL, this );
	m_checkBoxShowOnStartup->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WelcomeLayout::OnCheckShowOnStartup ), NULL, this );
	
}
