///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "SequencePropertiesLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

SequencePropertiesLayout::SequencePropertiesLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_sizerMain;
	m_sizerMain = new wxBoxSizer( wxVERTICAL );
	
	m_notebook = new wxNotebook( this, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_TOP );
	m_pnlGeneral = new wxPanel( m_notebook, ID_PNL_GENERAL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_sizerGeneral;
	m_sizerGeneral = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_sizerLayerName;
	m_sizerLayerName = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblLayerName = new wxStaticText( m_pnlGeneral, ID_LBL_LAYER_NAME, wxT("Layer name:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblLayerName->Wrap( -1 );
	m_sizerLayerName->Add( m_lblLayerName, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtLayerName = new wxTextCtrl( m_pnlGeneral, ID_TXT_LAYER_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerLayerName->Add( m_txtLayerName, 100, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_sizerGeneral->Add( m_sizerLayerName, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerDescription;
	m_sizerDescription = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblLayerName1 = new wxStaticText( m_pnlGeneral, ID_LBL_LAYER_NAME, wxT("Description:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblLayerName1->Wrap( -1 );
	m_sizerDescription->Add( m_lblLayerName1, 0, wxALL, 5 );
	
	m_txtLayerDescription = new wxTextCtrl( m_pnlGeneral, ID_TXT_LAYER_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_txtLayerDescription->SetMinSize( wxSize( 200,200 ) );
	
	m_sizerDescription->Add( m_txtLayerDescription, 100, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_sizerGeneral->Add( m_sizerDescription, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerAuthours;
	m_sizerAuthours = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblAuthours = new wxStaticText( m_pnlGeneral, ID_LBL_AUTHORS, wxT("Authors:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblAuthours->Wrap( -1 );
	m_sizerAuthours->Add( m_lblAuthours, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtAuthours = new wxTextCtrl( m_pnlGeneral, ID_TXT_AUTHOURS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerAuthours->Add( m_txtAuthours, 100, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_sizerGeneral->Add( m_sizerAuthours, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerVisible;
	m_sizerVisible = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_sizerVisible->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sizerGeneral->Add( m_sizerVisible, 0, wxEXPAND, 5 );
	
	m_pnlGeneral->SetSizer( m_sizerGeneral );
	m_pnlGeneral->Layout();
	m_sizerGeneral->Fit( m_pnlGeneral );
	m_notebook->AddPage( m_pnlGeneral, wxT("General"), true );
	m_pnlMetadata = new wxPanel( m_notebook, ID_PNL_METADATA, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_sizerMetadata;
	m_sizerMetadata = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_sizerLayerSource;
	m_sizerLayerSource = new wxBoxSizer( wxVERTICAL );
	
	m_lblLayerSource = new wxStaticText( m_pnlMetadata, ID_LBL_LAYER_SOURCE, wxT("Layer source:"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
	m_lblLayerSource->Wrap( -1 );
	m_sizerLayerSource->Add( m_lblLayerSource, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_txtLayerSource = new wxTextCtrl( m_pnlMetadata, ID_TXT_LAYER_SOURCE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_sizerLayerSource->Add( m_txtLayerSource, 100, wxALL|wxEXPAND, 5 );
	
	m_sizerMetadata->Add( m_sizerLayerSource, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerSourceData;
	m_sizerSourceData = new wxStaticBoxSizer( new wxStaticBox( m_pnlMetadata, wxID_ANY, wxT("Source Data") ), wxVERTICAL );
	
	m_txtMetaData = new wxRichTextCtrl( m_pnlMetadata, ID_TXT_METADATA, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxHSCROLL|wxVSCROLL|wxWANTS_CHARS );
	m_sizerSourceData->Add( m_txtMetaData, 1, wxEXPAND | wxALL, 5 );
	
	m_sizerMetadata->Add( m_sizerSourceData, 1, wxALL|wxEXPAND, 5 );
	
	m_pnlMetadata->SetSizer( m_sizerMetadata );
	m_pnlMetadata->Layout();
	m_sizerMetadata->Fit( m_pnlMetadata );
	m_notebook->AddPage( m_pnlMetadata, wxT("Metadata"), false );
	
	m_sizerMain->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* m_sizerButtons;
	m_sizerButtons = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnHelp = new wxButton( this, wxID_ANY, wxT("?"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_btnHelp->SetToolTip( wxT("Help") );
	
	m_sizerButtons->Add( m_btnHelp, 0, wxALL, 5 );
	
	
	m_sizerButtons->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnOK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_btnOK, 0, wxALL, 5 );
	
	m_btnApply = new wxButton( this, ID_BTN_APPLY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_btnApply, 0, wxALL, 5 );
	
	m_btnCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_btnCancel, 0, wxALL, 5 );
	
	m_sizerMain->Add( m_sizerButtons, 0, wxEXPAND, 5 );
	
	this->SetSizer( m_sizerMain );
	this->Layout();
	m_sizerMain->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( SequencePropertiesLayout::OnClose ) );
	m_btnHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SequencePropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SequencePropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SequencePropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SequencePropertiesLayout::OnCancel ), NULL, this );
}

SequencePropertiesLayout::~SequencePropertiesLayout()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( SequencePropertiesLayout::OnClose ) );
	m_btnHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SequencePropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SequencePropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SequencePropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SequencePropertiesLayout::OnCancel ), NULL, this );
	
}
