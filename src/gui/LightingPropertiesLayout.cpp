///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "LightingPropertiesLayout.h"
#include "../core/App.hpp"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

LightingPropertiesLayout::LightingPropertiesLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_sizerMain;
	m_sizerMain = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerLightPos;
	m_sizerLightPos = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Light Direction") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerDirection;
	m_sizerDirection = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblLightDirection = new wxStaticText( this, ID_LBL_LIGHT_BEARING, wxT("Bearing:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLightDirection->Wrap( -1 );
	m_sizerDirection->Add( m_lblLightDirection, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sliderLightDirection = new wxSlider( this, ID_SLIDER_DIRECTION, 1, 0, 359, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_sizerDirection->Add( m_sliderLightDirection, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_bitmapCompass = new wxStaticBitmap( this, wxID_ANY, wxBitmap( App::Inst().GetExeDir() + wxT("images/compass_small.bmp"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxSize( -1,-1 ), wxSIMPLE_BORDER );
	m_sizerDirection->Add( m_bitmapCompass, 0, wxALL, 5 );
	
	m_sizerLightPos->Add( m_sizerDirection, 0, wxEXPAND, 5 );
	
	m_sizerMain->Add( m_sizerLightPos, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerLightAltitude;
	m_sizerLightAltitude = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Light Altitude") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerLightHeight;
	m_sizerLightHeight = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblLightAltitude = new wxStaticText( this, ID_LBL_LIGHT_ALTITUDE, wxT("Altitude:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLightAltitude->Wrap( -1 );
	m_sizerLightHeight->Add( m_lblLightAltitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sliderLightAltitude = new wxSlider( this, ID_SLIDER_ALTITUDE, 30, 0, 90, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_sizerLightHeight->Add( m_sliderLightAltitude, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_bitmapAltitude = new wxStaticBitmap( this, wxID_ANY, wxBitmap( App::Inst().GetExeDir() + wxT("images/altitude.bmp"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxSize( -1,-1 ), wxSIMPLE_BORDER );
	m_sizerLightHeight->Add( m_bitmapAltitude, 0, wxALL, 5 );
	
	m_sizerLightAltitude->Add( m_sizerLightHeight, 0, wxEXPAND, 5 );
	
	m_sizerMain->Add( m_sizerLightAltitude, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerAmbientLightStrength;
	m_sizerAmbientLightStrength = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Ambient Light Strength") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerStrength;
	m_sizerStrength = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblAmbientLightStrength = new wxStaticText( this, ID_LBL_AMBIENT_LIGHT_STRENGTH, wxT("Strength:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAmbientLightStrength->Wrap( -1 );
	m_sizerStrength->Add( m_lblAmbientLightStrength, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sliderAmbientLightStrength = new wxSlider( this, ID_SLIDER_AMBIENT_LIGHT_STRENGTH, 30, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_sizerStrength->Add( m_sliderAmbientLightStrength, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sizerAmbientLightStrength->Add( m_sizerStrength, 0, wxEXPAND, 5 );
	
	m_sizerMain->Add( m_sizerAmbientLightStrength, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerButtons;
	m_sizerButtons = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnDefaultValues = new wxButton( this, wxID_ANY, wxT("Restore default values"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_btnDefaultValues, 0, wxALL, 5 );
	
	
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
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_sliderLightDirection->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_btnDefaultValues->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LightingPropertiesLayout::OnDefaultValues ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LightingPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LightingPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LightingPropertiesLayout::OnCancel ), NULL, this );
}

LightingPropertiesLayout::~LightingPropertiesLayout()
{
	// Disconnect Events
	m_sliderLightDirection->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightDirection->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderLightAltitude->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_sliderAmbientLightStrength->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( LightingPropertiesLayout::OnScrollChanged ), NULL, this );
	m_btnDefaultValues->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LightingPropertiesLayout::OnDefaultValues ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LightingPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LightingPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LightingPropertiesLayout::OnCancel ), NULL, this );
	
}
