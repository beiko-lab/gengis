///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "LocationPropertiesLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

LocationPropertiesLayout::LocationPropertiesLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
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
	
	m_lblLayerName = new wxStaticText( m_pnlGeneral, ID_LBL_LAYER_NAME, wxT("Layer name:"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
	m_lblLayerName->Wrap( -1 );
	m_sizerLayerName->Add( m_lblLayerName, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtLayerName = new wxTextCtrl( m_pnlGeneral, ID_TXT_LAYER_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerLayerName->Add( m_txtLayerName, 100, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_sizerGeneral->Add( m_sizerLayerName, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerDescription;
	m_sizerDescription = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblLayerName1 = new wxStaticText( m_pnlGeneral, ID_LBL_LAYER_NAME, wxT("Description:"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
	m_lblLayerName1->Wrap( -1 );
	m_sizerDescription->Add( m_lblLayerName1, 0, wxALL, 5 );
	
	m_txtLayerDescription = new wxTextCtrl( m_pnlGeneral, ID_TXT_LAYER_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_txtLayerDescription->SetMinSize( wxSize( 200,200 ) );
	
	m_sizerDescription->Add( m_txtLayerDescription, 100, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_sizerGeneral->Add( m_sizerDescription, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerAuthours;
	m_sizerAuthours = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblAuthours = new wxStaticText( m_pnlGeneral, ID_LBL_AUTHORS, wxT("Authors:"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
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
	m_notebook->AddPage( m_pnlGeneral, wxT("General"), false );
	m_pnlSymbology = new wxPanel( m_notebook, ID_PNL_SYMBOLOGY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_sizerSymbology;
	m_sizerSymbology = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerSymbolProperties;
	m_sizerSymbolProperties = new wxStaticBoxSizer( new wxStaticBox( m_pnlSymbology, wxID_ANY, wxT("Symbol Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerSymbolShape;
	m_sizerSymbolShape = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblSymbolShape = new wxStaticText( m_pnlSymbology, ID_LBL_SYMBOL_SHAPE, wxT("Shape:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblSymbolShape->Wrap( -1 );
	m_sizerSymbolShape->Add( m_lblSymbolShape, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboSymbolShape = new wxComboBox( m_pnlSymbology, wxID_ANY, wxT("Circle (fast)"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboSymbolShape->Append( wxT("Circle") );
	m_cboSymbolShape->Append( wxT("Circle (fast)") );
	m_cboSymbolShape->Append( wxT("Square") );
	m_cboSymbolShape->Append( wxT("Triangle") );
	m_cboSymbolShape->Append( wxT("Star") );
	m_cboSymbolShape->Append( wxT("Plus sign") );
	m_cboSymbolShape->Append( wxT("Octagon") );
	m_cboSymbolShape->Append( wxT("Diamond") );
	m_cboSymbolShape->Append( wxT("Inverted triangle") );
	m_sizerSymbolShape->Add( m_cboSymbolShape, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerSymbolProperties->Add( m_sizerSymbolShape, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerSymbolSize;
	m_sizerSymbolSize = new wxBoxSizer( wxHORIZONTAL );
	
	m_lbSymbolSize = new wxStaticText( m_pnlSymbology, ID_LBL_FONT_SIZE, wxT("Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbSymbolSize->Wrap( -1 );
	m_sizerSymbolSize->Add( m_lbSymbolSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtSymbolSize = new wxTextCtrl( m_pnlSymbology, ID_TXT_SYMBOL_SIZE, wxT("5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerSymbolSize->Add( m_txtSymbolSize, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerSymbolProperties->Add( m_sizerSymbolSize, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerSymbolColour;
	m_sizerSymbolColour = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblSymbolColour = new wxStaticText( m_pnlSymbology, ID_LBL_SYMBOL_COLOUR, wxT("Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblSymbolColour->Wrap( -1 );
	m_sizerSymbolColour->Add( m_lblSymbolColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_colourSymbol = new wxColourPickerCtrl( m_pnlSymbology, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerSymbolColour->Add( m_colourSymbol, 0, wxALL, 5 );
	
	m_sizerSymbolProperties->Add( m_sizerSymbolColour, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer15->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_chkLocationVisible = new wxCheckBox( m_pnlSymbology, wxID_ANY, wxT("Show Location"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( m_chkLocationVisible, 0, wxALL, 5 );
	
	m_sizerSymbolProperties->Add( bSizer15, 1, wxEXPAND, 5 );
	
	m_sizerSymbology->Add( m_sizerSymbolProperties, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerLabel;
	m_sizerLabel = new wxStaticBoxSizer( new wxStaticBox( m_pnlSymbology, wxID_ANY, wxT("Label Properties") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblFontSize = new wxStaticText( m_pnlSymbology, wxID_ANY, wxT("Label size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFontSize->Wrap( -1 );
	fgSizer1->Add( m_lblFontSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinLabelSize = new wxSpinCtrl( m_pnlSymbology, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 2, 100, 2 );
	fgSizer1->Add( m_spinLabelSize, 0, wxALL, 5 );
	
	m_lblFontPosition = new wxStaticText( m_pnlSymbology, wxID_ANY, wxT("Label position:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFontPosition->Wrap( -1 );
	fgSizer1->Add( m_lblFontPosition, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboLabelPosition = new wxComboBox( m_pnlSymbology, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboLabelPosition->Append( wxT("Right") );
	m_cboLabelPosition->Append( wxT("Left") );
	m_cboLabelPosition->Append( wxT("Top") );
	m_cboLabelPosition->Append( wxT("Bottom") );
	fgSizer1->Add( m_cboLabelPosition, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText11 = new wxStaticText( m_pnlSymbology, wxID_ANY, wxT("Label style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer1->Add( m_staticText11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_cboLabelStyle = new wxComboBox( m_pnlSymbology, wxID_ANY, wxT("Billboard"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboLabelStyle->Append( wxT("Billboard") );
	m_cboLabelStyle->Append( wxT("Dynamic") );
	fgSizer1->Add( m_cboLabelStyle, 0, wxALL, 5 );
	
	m_lblFontColour = new wxStaticText( m_pnlSymbology, wxID_ANY, wxT("Label colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFontColour->Wrap( -1 );
	fgSizer1->Add( m_lblFontColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_colourLabel = new wxColourPickerCtrl( m_pnlSymbology, wxID_ANY, wxColour( 255, 255, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer1->Add( m_colourLabel, 0, wxALL, 5 );
	
	m_sizerLabel->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerLabelColour;
	m_sizerLabelColour = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_sizerLabelColour->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_chkLabelVisible = new wxCheckBox( m_pnlSymbology, wxID_ANY, wxT("Show Label"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerLabelColour->Add( m_chkLabelVisible, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sizerLabel->Add( m_sizerLabelColour, 0, wxEXPAND, 5 );
	
	m_sizerSymbology->Add( m_sizerLabel, 0, wxALL|wxEXPAND, 5 );
	
	m_pnlSymbology->SetSizer( m_sizerSymbology );
	m_pnlSymbology->Layout();
	m_sizerSymbology->Fit( m_pnlSymbology );
	m_notebook->AddPage( m_pnlSymbology, wxT("Symbology"), true );
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
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( LocationPropertiesLayout::OnClose ) );
	m_btnHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationPropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationPropertiesLayout::OnCancel ), NULL, this );
}

LocationPropertiesLayout::~LocationPropertiesLayout()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( LocationPropertiesLayout::OnClose ) );
	m_btnHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationPropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationPropertiesLayout::OnCancel ), NULL, this );
	
}
