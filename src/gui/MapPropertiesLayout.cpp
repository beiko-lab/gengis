///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "MapPropertiesLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

MapPropertiesLayout::MapPropertiesLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_mapPropSizer;
	m_mapPropSizer = new wxBoxSizer( wxVERTICAL );
	
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
	m_txtLayerDescription->SetMinSize( wxSize( 200,-1 ) );
	
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
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook3 = new wxNotebook( m_pnlSymbology, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_pnlColourMap = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerColourMap;
	m_sizerColourMap = new wxStaticBoxSizer( new wxStaticBox( m_pnlColourMap, wxID_ANY, wxT("Colour Map") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerColourMapHorizontal;
	m_sizerColourMapHorizontal = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblColourMap = new wxStaticText( m_pnlColourMap, ID_LBL_COLOUR_MAP, wxT("Colour map:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblColourMap->Wrap( -1 );
	m_sizerColourMapHorizontal->Add( m_lblColourMap, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboColourMap = new wxComboBox( m_pnlColourMap, ID_CBO_COLOUR_MAP, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	m_sizerColourMapHorizontal->Add( m_cboColourMap, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerColourMap->Add( m_sizerColourMapHorizontal, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerInterpolate;
	m_sizerInterpolate = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblInterpolation = new wxStaticText( m_pnlColourMap, ID_LBL_INTERPOLATE, wxT("Interpolation:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblInterpolation->Wrap( -1 );
	m_sizerInterpolate->Add( m_lblInterpolation, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboInterpolation = new wxComboBox( m_pnlColourMap, ID_CBO_INTERPOLATE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	m_cboInterpolation->Append( wxT("Discrete") );
	m_cboInterpolation->Append( wxT("Linearly") );
	m_sizerInterpolate->Add( m_cboInterpolation, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerColourMap->Add( m_sizerInterpolate, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerEntries;
	m_sizerEntries = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblEntries = new wxStaticText( m_pnlColourMap, ID_LBL_NUM_ENTRIES, wxT("Number Entries:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblEntries->Wrap( -1 );
	m_sizerEntries->Add( m_lblEntries, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinNumEntries = new wxSpinCtrl( m_pnlColourMap, ID_SPIN_NUM_ENTRIES, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 2, 100, 2 );
	m_sizerEntries->Add( m_spinNumEntries, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerColourMap->Add( m_sizerEntries, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerScrolledWindow;
	m_sizerScrolledWindow = new wxBoxSizer( wxVERTICAL );
	
	m_scrolledWindowColourMap = new wxScrolledWindow( m_pnlColourMap, ID_SCROLLED_WINDOW_COLOUR_MAP, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxSTATIC_BORDER|wxVSCROLL );
	m_scrolledWindowColourMap->SetScrollRate( 5, 5 );
	m_scrolledWindowColourMap->SetMinSize( wxSize( -1,200 ) );
	
	m_sizerScrolledWindow->Add( m_scrolledWindowColourMap, 10, wxEXPAND | wxALL, 5 );
	
	m_sizerColourMap->Add( m_sizerScrolledWindow, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerEvenlyDistribute;
	m_sizerEvenlyDistribute = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_sizerEvenlyDistribute->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnEvenlySpace = new wxButton( m_pnlColourMap, ID_BTN_EVENLY_SPACE, wxT("Evenly space entries"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerEvenlyDistribute->Add( m_btnEvenlySpace, 0, wxALL, 5 );
	
	m_sizerColourMap->Add( m_sizerEvenlyDistribute, 0, wxEXPAND, 5 );
	
	bSizer25->Add( m_sizerColourMap, 1, wxALL|wxEXPAND, 5 );
	
	m_pnlColourMap->SetSizer( bSizer25 );
	m_pnlColourMap->Layout();
	bSizer25->Fit( m_pnlColourMap );
	m_notebook3->AddPage( m_pnlColourMap, wxT("Colour Map"), true );
	m_pnlAdvanced = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_sizerSymbology;
	m_sizerSymbology = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerDisplayProperties;
	m_sizerDisplayProperties = new wxStaticBoxSizer( new wxStaticBox( m_pnlAdvanced, wxID_ANY, wxT("Display Properties") ), wxVERTICAL );
	
	wxBoxSizer* b_sizerVerticalExaggeration;
	b_sizerVerticalExaggeration = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblVerticalExaggeration = new wxStaticText( m_pnlAdvanced, ID_LBL_LEVEL_OF_DETAIL, wxT("Vertical exaggeration:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblVerticalExaggeration->Wrap( -1 );
	b_sizerVerticalExaggeration->Add( m_lblVerticalExaggeration, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtVerticalExaggeration = new wxTextCtrl( m_pnlAdvanced, ID_TXT_VERTICAL_EXAGGERATION, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	b_sizerVerticalExaggeration->Add( m_txtVerticalExaggeration, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerDisplayProperties->Add( b_sizerVerticalExaggeration, 0, wxEXPAND, 5 );
	
	wxBoxSizer* b_sizerLevelOfDetail;
	b_sizerLevelOfDetail = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblLevelOfDetail = new wxStaticText( m_pnlAdvanced, ID_LBL_LEVEL_OF_DETAIL, wxT("Level of Detail:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblLevelOfDetail->Wrap( -1 );
	b_sizerLevelOfDetail->Add( m_lblLevelOfDetail, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_sliderLevelOfDetail = new wxSlider( m_pnlAdvanced, ID_SLIDER_LEVEL_OF_DETAIL, 0, 0, 10000, wxDefaultPosition, wxDefaultSize, wxSL_BOTH|wxSL_HORIZONTAL );
	bSizer20->Add( m_sliderLevelOfDetail, 1, wxEXPAND|wxTOP, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblMinLevelOfDetail = new wxStaticText( m_pnlAdvanced, wxID_ANY, wxT("Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMinLevelOfDetail->Wrap( -1 );
	bSizer21->Add( m_lblMinLevelOfDetail, 0, wxALL, 0 );
	
	
	bSizer21->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_lblMaxLevelOfDetail = new wxStaticText( m_pnlAdvanced, wxID_ANY, wxT("Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMaxLevelOfDetail->Wrap( -1 );
	bSizer21->Add( m_lblMaxLevelOfDetail, 0, wxALL, 0 );
	
	bSizer20->Add( bSizer21, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	b_sizerLevelOfDetail->Add( bSizer20, 1, wxEXPAND, 5 );
	
	m_sizerDisplayProperties->Add( b_sizerLevelOfDetail, 0, wxEXPAND, 5 );
	
	m_sizerSymbology->Add( m_sizerDisplayProperties, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerTransparency;
	m_sizerTransparency = new wxStaticBoxSizer( new wxStaticBox( m_pnlAdvanced, wxID_ANY, wxT("Transparency") ), wxVERTICAL );
	
	m_sliderTransparency = new wxSlider( m_pnlAdvanced, ID_SLIDER_TRANSPARENCY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_BOTH|wxSL_HORIZONTAL|wxSL_LABELS|wxSL_TOP );
	m_sizerTransparency->Add( m_sliderTransparency, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	m_sizerSymbology->Add( m_sizerTransparency, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer19->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_chkWireframe = new wxCheckBox( m_pnlAdvanced, ID_CHECK_WIREFRAME, wxT("Wireframe"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_chkWireframe, 0, wxALL, 5 );
	
	m_sizerSymbology->Add( bSizer19, 1, wxEXPAND, 5 );
	
	m_pnlAdvanced->SetSizer( m_sizerSymbology );
	m_pnlAdvanced->Layout();
	m_sizerSymbology->Fit( m_pnlAdvanced );
	m_notebook3->AddPage( m_pnlAdvanced, wxT("Advanced"), false );
	
	bSizer22->Add( m_notebook3, 1, wxEXPAND | wxALL, 5 );
	
	m_pnlSymbology->SetSizer( bSizer22 );
	m_pnlSymbology->Layout();
	bSizer22->Fit( m_pnlSymbology );
	m_notebook->AddPage( m_pnlSymbology, wxT("Symbology"), true );
	m_pnlCartogram = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( m_pnlCartogram, wxID_ANY, wxT("Location Properties") ), wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_lblAreaFudge = new wxStaticText( m_pnlCartogram, wxID_ANY, wxT("Location Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAreaFudge->Wrap( -1 );
	gSizer1->Add( m_lblAreaFudge, 0, wxALL, 5 );
	
	m_spinAreaFudge = new wxSpinCtrl( m_pnlCartogram, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, 4 );
	gSizer1->Add( m_spinAreaFudge, 0, wxALL, 5 );
	
	m_lblValueFudge = new wxStaticText( m_pnlCartogram, wxID_ANY, wxT("Variable Multiplier"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblValueFudge->Wrap( -1 );
	gSizer1->Add( m_lblValueFudge, 0, wxALL, 5 );
	
	m_spinValueFudge = new wxSpinCtrl( m_pnlCartogram, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, 10 );
	gSizer1->Add( m_spinValueFudge, 0, wxALL, 5 );
	
	sbSizer5->Add( gSizer1, 0, wxEXPAND, 5 );
	
	bSizer23->Add( sbSizer5, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_pnlCartogram, wxID_ANY, wxT("Cartogram Properties") ), wxVERTICAL );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 5, 2, 0, 0 );
	
	m_lblSelectLocation = new wxStaticText( m_pnlCartogram, wxID_ANY, wxT("Select Location Set"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblSelectLocation->Wrap( -1 );
	gSizer2->Add( m_lblSelectLocation, 0, wxALL, 5 );
	
	wxArrayString m_cboSelectLocationChoices;
	m_cboSelectLocation = new wxChoice( m_pnlCartogram, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cboSelectLocationChoices, 0 );
	m_cboSelectLocation->SetSelection( 0 );
	gSizer2->Add( m_cboSelectLocation, 0, wxALL, 5 );
	
	m_lblSelectVectorMap = new wxStaticText( m_pnlCartogram, wxID_ANY, wxT("Select Vector Map"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblSelectVectorMap->Wrap( -1 );
	gSizer2->Add( m_lblSelectVectorMap, 0, wxALL, 5 );
	
	wxArrayString m_cboSelectVectorMapChoices;
	m_cboSelectVectorMap = new wxChoice( m_pnlCartogram, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cboSelectVectorMapChoices, 0 );
	m_cboSelectVectorMap->SetSelection( 0 );
	gSizer2->Add( m_cboSelectVectorMap, 0, wxALL, 5 );
	
	m_lblSelectMethod = new wxStaticText( m_pnlCartogram, wxID_ANY, wxT("Select Distortion Method"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblSelectMethod->Wrap( -1 );
	gSizer2->Add( m_lblSelectMethod, 0, wxALL, 5 );
	
	wxArrayString m_cboSelectMethodChoices;
	m_cboSelectMethod = new wxChoice( m_pnlCartogram, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cboSelectMethodChoices, 0 );
	m_cboSelectMethod->SetSelection( 0 );
	gSizer2->Add( m_cboSelectMethod, 0, wxALL, 5 );
	
	m_radioOne = new wxRadioButton( m_pnlCartogram, wxID_VALUE_DEFAULT, wxT("Value"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_radioOne->SetValue( true ); 
	gSizer2->Add( m_radioOne, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_radioTwo = new wxRadioButton( m_pnlCartogram, wxID_VALUE_INVERTED, wxT("1-Value"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer2->Add( m_radioTwo, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer6->Add( gSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer251;
	bSizer251 = new wxBoxSizer( wxVERTICAL );
	
	m_checkResize = new wxCheckBox( m_pnlCartogram, wxID_ANY, wxT("Resize Density Map"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer251->Add( m_checkResize, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer6->Add( bSizer251, 0, wxEXPAND, 5 );
	
	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 1, 2, 0, 0 );
	
	m_lblResiizePercent = new wxStaticText( m_pnlCartogram, wxID_ANY, wxT("Resize Percent (0-60)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblResiizePercent->Wrap( -1 );
	gSizer3->Add( m_lblResiizePercent, 0, wxALL, 5 );
	
	m_spinResizePercent = new wxSpinCtrl( m_pnlCartogram, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 60, 50 );
	gSizer3->Add( m_spinResizePercent, 0, wxALL, 5 );
	
	sbSizer6->Add( gSizer3, 0, wxEXPAND, 5 );
	
	bSizer23->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnMakeCartogram = new wxButton( m_pnlCartogram, wxID_ANY, wxT("Go!"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer24->Add( m_btnMakeCartogram, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer24->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnUndoCartogram = new wxButton( m_pnlCartogram, wxID_ANY, wxT("Restore Map"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer24->Add( m_btnUndoCartogram, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer23->Add( bSizer24, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_pnlCartogram->SetSizer( bSizer23 );
	m_pnlCartogram->Layout();
	bSizer23->Fit( m_pnlCartogram );
	m_notebook->AddPage( m_pnlCartogram, wxT("Cartogram"), false );
	m_pnlMetadata = new wxPanel( m_notebook, ID_PNL_METADATA, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_sizerMetadata;
	m_sizerMetadata = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_sizerLayerSource;
	m_sizerLayerSource = new wxBoxSizer( wxVERTICAL );
	
	m_lblLayerSource = new wxStaticText( m_pnlMetadata, ID_LBL_LAYER_SOURCE, wxT("Layer Source:"), wxDefaultPosition, wxSize( 200,-1 ), 0 );
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
	
	m_mapPropSizer->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );
	
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
	
	m_mapPropSizer->Add( m_sizerButtons, 0, wxEXPAND, 5 );
	
	this->SetSizer( m_mapPropSizer );
	this->Layout();
	m_mapPropSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MapPropertiesLayout::OnClose ) );
	m_txtLayerName->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MapPropertiesLayout::findme ), NULL, this );
	m_cboColourMap->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( MapPropertiesLayout::OnColourMapChange ), NULL, this );
	m_cboInterpolation->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( MapPropertiesLayout::OnInterpolationChange ), NULL, this );
	m_spinNumEntries->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MapPropertiesLayout::OnNumEntriesChange ), NULL, this );
	m_btnEvenlySpace->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnEvenlySpace ), NULL, this );
	m_radioOne->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MapPropertiesLayout::OnCartValueToggle ), NULL, this );
	m_radioTwo->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MapPropertiesLayout::OnCartValueToggle ), NULL, this );
	m_checkResize->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnResizeToggle ), NULL, this );
	m_spinResizePercent->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MapPropertiesLayout::OnSetResizePercent ), NULL, this );
	m_spinResizePercent->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MapPropertiesLayout::OnSetResizePercent ), NULL, this );
	m_btnMakeCartogram->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnCartogram ), NULL, this );
	m_btnUndoCartogram->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnUndoCartogram ), NULL, this );
	m_btnHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnCancel ), NULL, this );
}

MapPropertiesLayout::~MapPropertiesLayout()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MapPropertiesLayout::OnClose ) );
	m_txtLayerName->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MapPropertiesLayout::findme ), NULL, this );
	m_cboColourMap->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( MapPropertiesLayout::OnColourMapChange ), NULL, this );
	m_cboInterpolation->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( MapPropertiesLayout::OnInterpolationChange ), NULL, this );
	m_spinNumEntries->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MapPropertiesLayout::OnNumEntriesChange ), NULL, this );
	m_btnEvenlySpace->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnEvenlySpace ), NULL, this );
	m_radioOne->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MapPropertiesLayout::OnCartValueToggle ), NULL, this );
	m_radioTwo->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MapPropertiesLayout::OnCartValueToggle ), NULL, this );
	m_checkResize->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnResizeToggle ), NULL, this );
	m_spinResizePercent->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MapPropertiesLayout::OnSetResizePercent ), NULL, this );
	m_spinResizePercent->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MapPropertiesLayout::OnSetResizePercent ), NULL, this );
	m_btnMakeCartogram->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnCartogram ), NULL, this );
	m_btnUndoCartogram->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnUndoCartogram ), NULL, this );
	m_btnHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapPropertiesLayout::OnCancel ), NULL, this );
	
}
