///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "NonlinearAxisTableLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

NonlinearAxisTableLayout::NonlinearAxisTableLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Permutation #, Number of Crossings, Optimal"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText3->Wrap( -1 );
	bSizer3->Add( m_staticText3, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_listbox = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB|wxLB_SINGLE ); 
	bSizer3->Add( m_listbox, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnOK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_btnOK, 0, wxALL, 5 );
	
	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_listbox->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( NonlinearAxisTableLayout::OnListBox ), NULL, this );
}

NonlinearAxisTableLayout::~NonlinearAxisTableLayout()
{
	// Disconnect Events
	m_listbox->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( NonlinearAxisTableLayout::OnListBox ), NULL, this );
	
}
