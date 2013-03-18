///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "SelectorWindowLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

SelectorWindowLayout::SelectorWindowLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText->Wrap( -1 );
	bSizer1->Add( m_staticText, 0, wxALL, 5 );
	
	m_listBoxSelect = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_NEEDED_SB|wxLB_SINGLE ); 
	bSizer1->Add( m_listBoxSelect, 1, wxEXPAND|wxALL, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonSelect = new wxButton( this, wxID_ANY, wxT("Select"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonSelect, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonCancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bSizer1->Add( bSizer2, 0, wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_listBoxSelect->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( SelectorWindowLayout::OnSelectionDoubleClick ), NULL, this );
	m_buttonSelect->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SelectorWindowLayout::OnSelectButton ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SelectorWindowLayout::OnCancel ), NULL, this );
}

SelectorWindowLayout::~SelectorWindowLayout()
{
	// Disconnect Events
	m_listBoxSelect->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( SelectorWindowLayout::OnSelectionDoubleClick ), NULL, this );
	m_buttonSelect->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SelectorWindowLayout::OnSelectButton ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SelectorWindowLayout::OnCancel ), NULL, this );
	
}
