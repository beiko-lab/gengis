///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "LayoutObjectPropertiesLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

LayoutObjectPropertiesLayout::LayoutObjectPropertiesLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_sizerMain;
	m_sizerMain = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_sizerHor;
	m_sizerHor = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* m_sizerObjectProperties;
	m_sizerObjectProperties = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Object Layout Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerObjectThickness;
	m_sizerObjectThickness = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblObjectLineThickness = new wxStaticText( this, wxID_ANY, wxT("Thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblObjectLineThickness->Wrap( -1 );
	m_sizerObjectThickness->Add( m_lblObjectLineThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinObjectLineThickness = new wxSpinCtrl( this, ID_SPIN_OBJECT_LINE_THICKNESS, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_sizerObjectThickness->Add( m_spinObjectLineThickness, 0, wxALL, 5 );
	
	m_sizerObjectProperties->Add( m_sizerObjectThickness, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerObjectColour;
	m_sizerObjectColour = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblObjectLineColour = new wxStaticText( this, wxID_ANY, wxT("Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblObjectLineColour->Wrap( -1 );
	m_sizerObjectColour->Add( m_lblObjectLineColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_colourObjectLine = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerObjectColour->Add( m_colourObjectLine, 0, wxALL, 5 );
	
	m_sizerObjectProperties->Add( m_sizerObjectColour, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerObjectLineStyle;
	m_sizerObjectLineStyle = new wxBoxSizer( wxHORIZONTAL );
	
	m_objectLineStyleLabel = new wxStaticText( this, wxID_ANY, wxT("Line style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_objectLineStyleLabel->Wrap( -1 );
	m_sizerObjectLineStyle->Add( m_objectLineStyleLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboObjectLineStyle = new wxComboBox( this, ID_CBO_OBJECT_LINE_STYLE, wxT("Solid"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboObjectLineStyle->Append( wxT("Hidden") );
	m_cboObjectLineStyle->Append( wxT("Solid") );
	m_cboObjectLineStyle->Append( wxT("Short dash") );
	m_cboObjectLineStyle->Append( wxT("Long dash") );
	m_sizerObjectLineStyle->Add( m_cboObjectLineStyle, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_sizerObjectProperties->Add( m_sizerObjectLineStyle, 0, wxEXPAND, 5 );
	
	m_sizerHor->Add( m_sizerObjectProperties, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerLayoutLineProperties;
	m_sizerLayoutLineProperties = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Layout Line Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerLayoutLineThickness;
	m_sizerLayoutLineThickness = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblLayoutLineThickness = new wxStaticText( this, wxID_ANY, wxT("Thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLayoutLineThickness->Wrap( -1 );
	m_sizerLayoutLineThickness->Add( m_lblLayoutLineThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinLayoutLineThickness = new wxSpinCtrl( this, ID_SPIN_LAYOUT_LINE_THICKNESS, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_sizerLayoutLineThickness->Add( m_spinLayoutLineThickness, 0, wxALL, 5 );
	
	m_sizerLayoutLineProperties->Add( m_sizerLayoutLineThickness, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerObjectColour1;
	m_sizerObjectColour1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblLayoutLineColour = new wxStaticText( this, wxID_ANY, wxT("Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLayoutLineColour->Wrap( -1 );
	m_sizerObjectColour1->Add( m_lblLayoutLineColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_colourLayoutLine = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerObjectColour1->Add( m_colourLayoutLine, 0, wxALL, 5 );
	
	m_sizerLayoutLineProperties->Add( m_sizerObjectColour1, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerLayoutLineStyle;
	m_sizerLayoutLineStyle = new wxBoxSizer( wxHORIZONTAL );
	
	m_layoutLineStyleLabel = new wxStaticText( this, wxID_ANY, wxT("Line style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_layoutLineStyleLabel->Wrap( -1 );
	m_sizerLayoutLineStyle->Add( m_layoutLineStyleLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboLayoutLineStyle = new wxComboBox( this, ID_CBO_LAYOUT_LINE_STYLE, wxT("Solid"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboLayoutLineStyle->Append( wxT("Hidden") );
	m_cboLayoutLineStyle->Append( wxT("Solid") );
	m_cboLayoutLineStyle->Append( wxT("Short dash") );
	m_cboLayoutLineStyle->Append( wxT("Long dash") );
	m_sizerLayoutLineStyle->Add( m_cboLayoutLineStyle, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_sizerLayoutLineProperties->Add( m_sizerLayoutLineStyle, 1, wxEXPAND, 5 );
	
	m_sizerHor->Add( m_sizerLayoutLineProperties, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerControlPointProperties;
	m_sizerControlPointProperties = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Control Point Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerControlPointSize;
	m_sizerControlPointSize = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblControlPointSize = new wxStaticText( this, wxID_ANY, wxT("Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblControlPointSize->Wrap( -1 );
	m_sizerControlPointSize->Add( m_lblControlPointSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinControlPointSize = new wxSpinCtrl( this, ID_SPIN_CONTROL_POINT_SIZE, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 50, 5 );
	m_sizerControlPointSize->Add( m_spinControlPointSize, 0, wxALL, 5 );
	
	m_sizerControlPointProperties->Add( m_sizerControlPointSize, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerControlPointColour;
	m_sizerControlPointColour = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblControlPointColour = new wxStaticText( this, ID_LBL_CONTROL_POINT_COLOUR, wxT("Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblControlPointColour->Wrap( -1 );
	m_sizerControlPointColour->Add( m_lblControlPointColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_colourControlPoint = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerControlPointColour->Add( m_colourControlPoint, 0, wxALL, 5 );
	
	m_sizerControlPointProperties->Add( m_sizerControlPointColour, 0, wxEXPAND, 5 );
	
	m_sizerHor->Add( m_sizerControlPointProperties, 1, wxALL|wxEXPAND, 5 );
	
	m_sizerMain->Add( m_sizerHor, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerButtons;
	m_sizerButtons = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_sizerButtons->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnOK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_btnOK, 0, wxALL, 5 );
	
	m_btnApply = new wxButton( this, ID_BTN_APPLY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_btnApply, 0, wxALL, 5 );
	
	m_btnCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnCancel->SetDefault(); 
	m_sizerButtons->Add( m_btnCancel, 0, wxALL, 5 );
	
	m_sizerMain->Add( m_sizerButtons, 0, wxEXPAND, 5 );
	
	this->SetSizer( m_sizerMain );
	this->Layout();
	m_sizerMain->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LayoutObjectPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LayoutObjectPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LayoutObjectPropertiesLayout::OnCancel ), NULL, this );
}

LayoutObjectPropertiesLayout::~LayoutObjectPropertiesLayout()
{
	// Disconnect Events
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LayoutObjectPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LayoutObjectPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LayoutObjectPropertiesLayout::OnCancel ), NULL, this );
	
}
