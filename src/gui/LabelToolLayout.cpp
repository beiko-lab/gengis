///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "LabelToolLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

LabelToolLayout::LabelToolLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_sizerMain;
	m_sizerMain = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerLightPos;
	m_sizerLightPos = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Label Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerDirection;
	m_sizerDirection = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblText = new wxStaticText( this, wxID_ANY, wxT("Text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblText->Wrap( -1 );
	m_sizerDirection->Add( m_lblText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtLabel = new wxTextCtrl( this, ID_LABEL_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerDirection->Add( m_txtLabel, 1, wxALL, 5 );
	
	m_sizerLightPos->Add( m_sizerDirection, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizeColour;
	m_sizeColour = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblColour = new wxStaticText( this, wxID_ANY, wxT("Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblColour->Wrap( -1 );
	m_sizeColour->Add( m_lblColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_colourLabel = new wxColourPickerCtrl( this, wxID_ANY, wxColour( 255, 255, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizeColour->Add( m_colourLabel, 0, wxALL, 5 );
	
	m_sizerLightPos->Add( m_sizeColour, 1, wxEXPAND, 5 );
	
	wxBoxSizer* sizerSize;
	sizerSize = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblLabelSize = new wxStaticText( this, wxID_ANY, wxT("Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLabelSize->Wrap( -1 );
	sizerSize->Add( m_lblLabelSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinLabelSize = new wxSpinCtrl( this, ID_LABEL_SIZE, wxT("12"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 0 );
	sizerSize->Add( m_spinLabelSize, 1, wxALL, 5 );
	
	m_sizerLightPos->Add( sizerSize, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerStyle;
	m_sizerStyle = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblLabelStyle = new wxStaticText( this, wxID_ANY, wxT("Label style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLabelStyle->Wrap( -1 );
	m_sizerStyle->Add( m_lblLabelStyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboLabelStyle = new wxComboBox( this, wxID_ANY, wxT("\"Billboard\""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboLabelStyle->Append( wxT("Billboard") );
	m_cboLabelStyle->Append( wxT("Dynamic") );
	m_sizerStyle->Add( m_cboLabelStyle, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerLightPos->Add( m_sizerStyle, 1, wxEXPAND, 5 );
	
	m_sizerMain->Add( m_sizerLightPos, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerButtons;
	m_sizerButtons = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_sizerButtons->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnOK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_btnOK, 0, wxALL, 5 );
	
	m_btnCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_btnCancel, 0, wxALL, 5 );
	
	m_sizerMain->Add( m_sizerButtons, 0, wxEXPAND, 5 );
	
	this->SetSizer( m_sizerMain );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LabelToolLayout::OnOK ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LabelToolLayout::OnCancel ), NULL, this );
}

LabelToolLayout::~LabelToolLayout()
{
	// Disconnect Events
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LabelToolLayout::OnOK ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LabelToolLayout::OnCancel ), NULL, this );
	
}
