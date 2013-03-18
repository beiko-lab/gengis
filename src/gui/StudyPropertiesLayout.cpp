///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "StudyPropertiesLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

StudyPropertiesLayout::StudyPropertiesLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
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
	m_pnlProjection = new wxPanel( m_notebook, ID_PNL_PROJECTION, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_sizerProjectionProperties;
	m_sizerProjectionProperties = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_sizerDatum;
	m_sizerDatum = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblDatum = new wxStaticText( m_pnlProjection, ID_LBL_DATUM, wxT("Datum:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblDatum->Wrap( -1 );
	m_sizerDatum->Add( m_lblDatum, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_cboDatum = new wxComboBox( m_pnlProjection, ID_CBO_DATUM, wxT("WGS84"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboDatum->Append( wxT("WGS84") );
	m_cboDatum->Append( wxT("WGS72") );
	m_cboDatum->Append( wxT("NAD83") );
	m_cboDatum->Append( wxT("NAD27") );
	m_sizerDatum->Add( m_cboDatum, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerProjectionProperties->Add( m_sizerDatum, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerProjection;
	m_sizerProjection = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblProjection = new wxStaticText( m_pnlProjection, ID_LBL_PROJECTION, wxT("Projection:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblProjection->Wrap( -1 );
	m_sizerProjection->Add( m_lblProjection, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_cboProjection = new wxComboBox( m_pnlProjection, ID_CBO_PROJECTION, wxT("Plate Carree (Equirectangular)"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboProjection->Append( wxT("Albers Conic Equal Area") );
	m_cboProjection->Append( wxT("Azimuthal Equidistant") );
	m_cboProjection->Append( wxT("Eckert IV") );
	m_cboProjection->Append( wxT("Equidistant Conic") );
	m_cboProjection->Append( wxT("Gall Stereograpic") );
	m_cboProjection->Append( wxT("Lambert Azimuthal Equal-Area") );
	m_cboProjection->Append( wxT("Lambert Conformal Conic") );
	m_cboProjection->Append( wxT("Mercator") );
	m_cboProjection->Append( wxT("Mollweide") );
	m_cboProjection->Append( wxT("Orthographic") );
	m_cboProjection->Append( wxT("Plate Carree (Equirectangular)") );
	m_cboProjection->Append( wxT("Robinson") );
	m_cboProjection->Append( wxT("Stereographic") );
	m_cboProjection->Append( wxT("Transverse Mercator") );
	m_cboProjection->Append( wxT("Universal Transverse Mercator (UTM)") );
	m_sizerProjection->Add( m_cboProjection, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerProjectionProperties->Add( m_sizerProjection, 0, wxEXPAND, 5 );
	
	m_txtProjectionNote = new wxTextCtrl( m_pnlProjection, wxID_ANY, wxT("Notes:\n\n1. Geographical coordinates given in location sets and geographic tree models are assumed to be given as latitudinal and longitudinal values with the specified datum.\n\n2. Projections information must be set before loading a map. On-the-fly projections will be implemented in the future based on user demand.\n\n3. If you require a projection not listed here please contact the GenGIS development team. "), wxDefaultPosition, wxDefaultSize, wxTE_AUTO_URL|wxTE_MULTILINE|wxTE_READONLY );
	m_txtProjectionNote->SetMinSize( wxSize( 300,200 ) );
	
	m_sizerProjectionProperties->Add( m_txtProjectionNote, 1, wxALL|wxEXPAND, 5 );
	
	m_pnlProjection->SetSizer( m_sizerProjectionProperties );
	m_pnlProjection->Layout();
	m_sizerProjectionProperties->Fit( m_pnlProjection );
	m_notebook->AddPage( m_pnlProjection, wxT("Projection"), true );
	m_pnlSymbology = new wxPanel( m_notebook, ID_PNL_SYMBOLOGY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_mainSymbologySizer;
	m_mainSymbologySizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizeSymbology;
	m_sizeSymbology = new wxStaticBoxSizer( new wxStaticBox( m_pnlSymbology, wxID_ANY, wxT("Viewport Properties") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblColourBackground = new wxStaticText( m_pnlSymbology, wxID_ANY, wxT("Background colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblColourBackground->Wrap( -1 );
	fgSizer1->Add( m_lblColourBackground, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_colourBackground = new wxColourPickerCtrl( m_pnlSymbology, ID_COLOUR_PICKER_BG, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer1->Add( m_colourBackground, 0, wxALL, 5 );
	
	m_staticText9 = new wxStaticText( m_pnlSymbology, wxID_ANY, wxT("Terrain resolution:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer1->Add( m_staticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_cboTerrainResolution = new wxComboBox( m_pnlSymbology, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboTerrainResolution->Append( wxT("256") );
	m_cboTerrainResolution->Append( wxT("512") );
	m_cboTerrainResolution->Append( wxT("1024") );
	m_cboTerrainResolution->Append( wxT("2048") );
	m_cboTerrainResolution->Append( wxT("4096") );
	m_cboTerrainResolution->Append( wxT("8196") );
	fgSizer1->Add( m_cboTerrainResolution, 0, wxALL, 5 );
	
	m_sizeSymbology->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	m_mainSymbologySizer->Add( m_sizeSymbology, 0, wxALL|wxEXPAND, 5 );
	
	m_textCtrl6 = new wxTextCtrl( m_pnlSymbology, wxID_ANY, wxT("Notes:\n\nIf GenGIS indicates there is insufficent memory to build the terrain, you need to reduce the terrain resolution."), wxDefaultPosition, wxDefaultSize, wxTE_AUTO_URL|wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH );
	m_textCtrl6->SetMinSize( wxSize( -1,75 ) );
	
	m_mainSymbologySizer->Add( m_textCtrl6, 0, wxALL|wxEXPAND, 5 );
	
	m_pnlSymbology->SetSizer( m_mainSymbologySizer );
	m_pnlSymbology->Layout();
	m_mainSymbologySizer->Fit( m_pnlSymbology );
	m_notebook->AddPage( m_pnlSymbology, wxT("Symbology"), false );
	
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
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( StudyPropertiesLayout::OnClose ) );
	m_btnHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StudyPropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StudyPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StudyPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StudyPropertiesLayout::OnCancel ), NULL, this );
}

StudyPropertiesLayout::~StudyPropertiesLayout()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( StudyPropertiesLayout::OnClose ) );
	m_btnHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StudyPropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StudyPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StudyPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StudyPropertiesLayout::OnCancel ), NULL, this );
	
}
