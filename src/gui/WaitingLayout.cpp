///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "WaitingLayout.h"

#include "../../bin/images/filesave.xpm"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

WaitingLayout::WaitingLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_bitmap = new wxStaticBitmap( this, wxID_ANY, wxBitmap( filesave_xpm ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_bitmap, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_messageText = new wxStaticText( this, wxID_ANY, wxT("Saving to session file..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_messageText->Wrap( -1 );
	m_messageText->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer1->Add( m_messageText, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
}

WaitingLayout::~WaitingLayout()
{
}
