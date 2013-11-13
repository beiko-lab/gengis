///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "LocationSetPropertiesLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

LocationSetPropertiesLayout::LocationSetPropertiesLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_sizerMain;
	m_sizerMain = new wxBoxSizer( wxVERTICAL );
	
	m_notebook = new wxNotebook( this, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_TOP, wxT("Location_Set") );
	m_pnlGeneral = new wxPanel( m_notebook, ID_PNL_GENERAL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("General") );
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
	m_pnlLocations = new wxPanel( m_notebook, ID_PNL_LOCATIONS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("Location_Set") );
	wxBoxSizer* m_sizerLocations;
	m_sizerLocations = new wxBoxSizer( wxVERTICAL );
	
	m_notebookLocationSet = new wxNotebook( m_pnlLocations, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_pnlLocationSetColour = new wxPanel( m_notebookLocationSet, ID_PANEL_LOCATION_SET_COLOUR, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("Colour") );
	wxBoxSizer* m_sizerColourVert;
	m_sizerColourVert = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerLocationSetColour;
	m_sizerLocationSetColour = new wxStaticBoxSizer( new wxStaticBox( m_pnlLocationSetColour, ID_SIZER_LOCATION_SET_COLOUR, wxT("Colour Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerColourUniform;
	m_sizerColourUniform = new wxBoxSizer( wxHORIZONTAL );
	
	m_chkUniformColour = new wxCheckBox( m_pnlLocationSetColour, ID_CHK_UNIFORM_COLOUR, wxT("Uniform colour"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE );
	m_chkUniformColour->SetValue(true); 
	m_sizerColourUniform->Add( m_chkUniformColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_colourUniform = new wxColourPickerCtrl( m_pnlLocationSetColour, ID_COLOUR_PICKER_UNIFORM_COLOUR, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerColourUniform->Add( m_colourUniform, 0, wxALL, 5 );
	
	
	m_sizerColourUniform->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sizerLocationSetColour->Add( m_sizerColourUniform, 0, wxEXPAND, 5 );
	
	m_staticline5 = new wxStaticLine( m_pnlLocationSetColour, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_sizerLocationSetColour->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* m_sizerLocationSetColourProperties;
	m_sizerLocationSetColourProperties = new wxFlexGridSizer( 2, 2, 0, 0 );
	m_sizerLocationSetColourProperties->SetFlexibleDirection( wxBOTH );
	m_sizerLocationSetColourProperties->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblColourField = new wxStaticText( m_pnlLocationSetColour, ID_LBL_LABEL_COLOUR_FIELD, wxT("Field to chart:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblColourField->Wrap( -1 );
	m_lblColourField->SetMinSize( wxSize( 70,-1 ) );
	
	m_sizerLocationSetColourProperties->Add( m_lblColourField, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboColourField = new wxComboBox( m_pnlLocationSetColour, ID_CBO_COLOUR_FIELD, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	m_cboColourField->Enable( false );
	
	m_sizerLocationSetColourProperties->Add( m_cboColourField, 0, wxALL|wxEXPAND, 5 );
	
	m_lblColourMap = new wxStaticText( m_pnlLocationSetColour, ID_LBL_COLOUR_MAP, wxT("Colour map:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblColourMap->Wrap( -1 );
	m_sizerLocationSetColourProperties->Add( m_lblColourMap, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboColourMap = new wxComboBox( m_pnlLocationSetColour, ID_CBO_COLOUR_MAP, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	m_cboColourMap->Enable( false );
	
	m_sizerLocationSetColourProperties->Add( m_cboColourMap, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerLocationSetColour->Add( m_sizerLocationSetColourProperties, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerWindowColour;
	m_sizerWindowColour = new wxBoxSizer( wxHORIZONTAL );
	
	m_scrolledWindowColour = new wxScrolledWindow( m_pnlLocationSetColour, ID_SCROLLED_WINDOW_COLOUR, wxDefaultPosition, wxSize( -1,-1 ), wxHSCROLL|wxSTATIC_BORDER|wxVSCROLL );
	m_scrolledWindowColour->SetScrollRate( 5, 5 );
	m_scrolledWindowColour->Enable( false );
	m_scrolledWindowColour->SetMinSize( wxSize( -1,250 ) );
	
	m_sizerWindowColour->Add( m_scrolledWindowColour, 10, wxALL|wxEXPAND, 5 );
	
	m_sizerLocationSetColour->Add( m_sizerWindowColour, 1, wxEXPAND, 5 );
	
	m_sizerColourVert->Add( m_sizerLocationSetColour, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( m_pnlLocationSetColour, wxID_ANY, wxT("Borders") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerBorders;
	m_sizerBorders = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText28 = new wxStaticText( m_pnlLocationSetColour, wxID_ANY, wxT("Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	m_sizerBorders->Add( m_staticText28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_spinBorderSize = new wxSpinCtrl( m_pnlLocationSetColour, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	m_sizerBorders->Add( m_spinBorderSize, 0, wxALL, 5 );
	
	m_colourBorders = new wxColourPickerCtrl( m_pnlLocationSetColour, ID_COLOUR_PICKER_BORDERS, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerBorders->Add( m_colourBorders, 0, wxALL, 5 );
	
	sbSizer9->Add( m_sizerBorders, 0, wxEXPAND, 5 );
	
	m_sizerColourVert->Add( sbSizer9, 0, wxALL|wxEXPAND, 5 );
	
	m_pnlLocationSetColour->SetSizer( m_sizerColourVert );
	m_pnlLocationSetColour->Layout();
	m_sizerColourVert->Fit( m_pnlLocationSetColour );
	m_notebookLocationSet->AddPage( m_pnlLocationSetColour, wxT("Colour"), false );
	m_pnlLocationSetShape = new wxPanel( m_notebookLocationSet, ID_PANEL_LOCATION_SET_SHAPE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("Shape") );
	wxBoxSizer* m_sizerShapeVert;
	m_sizerShapeVert = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerLocationSetShape;
	m_sizerLocationSetShape = new wxStaticBoxSizer( new wxStaticBox( m_pnlLocationSetShape, ID_SIZER_LOCATION_SET_SHAPE, wxT("Shape Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerShapeUniform;
	m_sizerShapeUniform = new wxBoxSizer( wxHORIZONTAL );
	
	m_chkUniformShape = new wxCheckBox( m_pnlLocationSetShape, ID_CHK_UNIFORM_SHAPE, wxT("Uniform shape"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE );
	m_chkUniformShape->SetValue(true); 
	m_sizerShapeUniform->Add( m_chkUniformShape, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboUniformShape = new wxComboBox( m_pnlLocationSetShape, ID_CBO_UNIFORM_SHAPE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	m_sizerShapeUniform->Add( m_cboUniformShape, 0, wxALL, 5 );
	
	
	m_sizerShapeUniform->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sizerLocationSetShape->Add( m_sizerShapeUniform, 0, wxEXPAND, 5 );
	
	m_staticline4 = new wxStaticLine( m_pnlLocationSetShape, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_sizerLocationSetShape->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* m_sizerLocationSetShapeProperties;
	m_sizerLocationSetShapeProperties = new wxFlexGridSizer( 2, 2, 0, 0 );
	m_sizerLocationSetShapeProperties->SetFlexibleDirection( wxBOTH );
	m_sizerLocationSetShapeProperties->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblShapeField = new wxStaticText( m_pnlLocationSetShape, ID_LBL_LABEL_SHAPE_FIELD, wxT("Field to chart:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblShapeField->Wrap( -1 );
	m_sizerLocationSetShapeProperties->Add( m_lblShapeField, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboShapeField = new wxComboBox( m_pnlLocationSetShape, ID_CBO_SHAPE_FIELD, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	m_cboShapeField->Enable( false );
	
	m_sizerLocationSetShapeProperties->Add( m_cboShapeField, 0, wxALL|wxEXPAND, 5 );
	
	m_lblShapeMap = new wxStaticText( m_pnlLocationSetShape, ID_LBL_SHAPE_MAP, wxT("Shape map:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblShapeMap->Wrap( -1 );
	m_sizerLocationSetShapeProperties->Add( m_lblShapeMap, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboShapeMap = new wxComboBox( m_pnlLocationSetShape, ID_CBO_SHAPE_MAP, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	m_cboShapeMap->Enable( false );
	
	m_sizerLocationSetShapeProperties->Add( m_cboShapeMap, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerLocationSetShape->Add( m_sizerLocationSetShapeProperties, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerShapeWindow;
	m_sizerShapeWindow = new wxBoxSizer( wxHORIZONTAL );
	
	m_scrolledWindowShape = new wxScrolledWindow( m_pnlLocationSetShape, ID_SCROLLED_WINDOW_SHAPE, wxDefaultPosition, wxSize( -1,-1 ), wxHSCROLL|wxSTATIC_BORDER|wxVSCROLL );
	m_scrolledWindowShape->SetScrollRate( 5, 5 );
	m_scrolledWindowShape->Enable( false );
	
	m_sizerShapeWindow->Add( m_scrolledWindowShape, 10, wxALL|wxEXPAND, 5 );
	
	m_sizerLocationSetShape->Add( m_sizerShapeWindow, 1, wxEXPAND, 5 );
	
	m_sizerShapeVert->Add( m_sizerLocationSetShape, 1, wxALL|wxEXPAND, 5 );
	
	m_pnlLocationSetShape->SetSizer( m_sizerShapeVert );
	m_pnlLocationSetShape->Layout();
	m_sizerShapeVert->Fit( m_pnlLocationSetShape );
	m_notebookLocationSet->AddPage( m_pnlLocationSetShape, wxT("Shape"), false );
	m_pnlLocationSetSize = new wxPanel( m_notebookLocationSet, ID_PANEL_LOCATION_SET_SIZE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("Size") );
	wxBoxSizer* m_sizerSizeVert;
	m_sizerSizeVert = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerLocationSetSize;
	m_sizerLocationSetSize = new wxStaticBoxSizer( new wxStaticBox( m_pnlLocationSetSize, ID_SIZER_LOCATION_SET_SIZE, wxT("Size Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerSizetLabelField;
	m_sizerSizetLabelField = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblSizeField = new wxStaticText( m_pnlLocationSetSize, ID_LBL_LABEL_SIZE_FIELD, wxT("Field to chart:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblSizeField->Wrap( -1 );
	m_sizerSizetLabelField->Add( m_lblSizeField, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboSizeField = new wxComboBox( m_pnlLocationSetSize, ID_CBO_SIZE_FIELD, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	m_sizerSizetLabelField->Add( m_cboSizeField, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerLocationSetSize->Add( m_sizerSizetLabelField, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerLocationSetColourMap11;
	m_sizerLocationSetColourMap11 = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_sizerLocationSetColourMap11->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_lblMinLocationSize = new wxStaticText( m_pnlLocationSetSize, ID_LBL_MIN_SIZE, wxT("Min:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMinLocationSize->Wrap( -1 );
	m_sizerLocationSetColourMap11->Add( m_lblMinLocationSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtMinLocationSize = new wxTextCtrl( m_pnlLocationSetSize, wxID_ANY, wxT("5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerLocationSetColourMap11->Add( m_txtMinLocationSize, 0, wxALL, 5 );
	
	m_lnlMaxLocationSize = new wxStaticText( m_pnlLocationSetSize, wxID_ANY, wxT("Max:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lnlMaxLocationSize->Wrap( -1 );
	m_sizerLocationSetColourMap11->Add( m_lnlMaxLocationSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtMaxLocationSize = new wxTextCtrl( m_pnlLocationSetSize, wxID_ANY, wxT("5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerLocationSetColourMap11->Add( m_txtMaxLocationSize, 0, wxALL, 5 );
	
	m_sizerLocationSetSize->Add( m_sizerLocationSetColourMap11, 0, wxEXPAND, 5 );
	
	m_sizerSizeVert->Add( m_sizerLocationSetSize, 0, wxALL|wxEXPAND, 5 );
	
	m_pnlLocationSetSize->SetSizer( m_sizerSizeVert );
	m_pnlLocationSetSize->Layout();
	m_sizerSizeVert->Fit( m_pnlLocationSetSize );
	m_notebookLocationSet->AddPage( m_pnlLocationSetSize, wxT("Size"), false );
	m_pnlLocationSetLabel = new wxPanel( m_notebookLocationSet, ID_PANEL_LOCATION_SET_LABEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("Label") );
	wxBoxSizer* m_sizerLabelVert;
	m_sizerLabelVert = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerLabelProp;
	m_sizerLabelProp = new wxStaticBoxSizer( new wxStaticBox( m_pnlLocationSetLabel, wxID_ANY, wxT("Label Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerLabelColour;
	m_sizerLabelColour = new wxBoxSizer( wxVERTICAL );
	
	m_chkShowLabels = new wxCheckBox( m_pnlLocationSetLabel, ID_CHECK_SHOW_LABELS, wxT("Show labels"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE );
	m_sizerLabelColour->Add( m_chkShowLabels, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_chkBindToCharts = new wxCheckBox( m_pnlLocationSetLabel, ID_CHECK_SHOW_LABELS, wxT("Bind labels to charts"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE );
	m_sizerLabelColour->Add( m_chkBindToCharts, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sizerLabelProp->Add( m_sizerLabelColour, 0, wxALIGN_RIGHT|wxEXPAND, 5 );
	
	m_staticline6 = new wxStaticLine( m_pnlLocationSetLabel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_sizerLabelProp->Add( m_staticline6, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 5, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblLabelField = new wxStaticText( m_pnlLocationSetLabel, ID_LBL_LABEL_FIELD, wxT("Label field:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLabelField->Wrap( -1 );
	fgSizer1->Add( m_lblLabelField, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboLabelField = new wxComboBox( m_pnlLocationSetLabel, ID_CBO_LABEL_FIELD, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	fgSizer1->Add( m_cboLabelField, 0, wxALL|wxEXPAND, 5 );
	
	m_lblFontSize = new wxStaticText( m_pnlLocationSetLabel, ID_LBL_FONT_SIZE, wxT("Label size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFontSize->Wrap( -1 );
	fgSizer1->Add( m_lblFontSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinFontSize = new wxSpinCtrl( m_pnlLocationSetLabel, ID_SPIN_FONT_SIZE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -1, 100, 10 );
	fgSizer1->Add( m_spinFontSize, 0, wxALL, 5 );
	
	m_lblFontPosition = new wxStaticText( m_pnlLocationSetLabel, ID_LBL_FONT_SIZE, wxT("Label position:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFontPosition->Wrap( -1 );
	fgSizer1->Add( m_lblFontPosition, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboLabelPosition = new wxComboBox( m_pnlLocationSetLabel, wxID_ANY, wxT("Right"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	m_cboLabelPosition->Append( wxT("Right") );
	m_cboLabelPosition->Append( wxT("Left") );
	m_cboLabelPosition->Append( wxT("Top") );
	m_cboLabelPosition->Append( wxT("Bottom") );
	fgSizer1->Add( m_cboLabelPosition, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText27 = new wxStaticText( m_pnlLocationSetLabel, wxID_ANY, wxT("Label style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	fgSizer1->Add( m_staticText27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_cboLabelStyle = new wxComboBox( m_pnlLocationSetLabel, wxID_ANY, wxT("\"Billboard\""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboLabelStyle->Append( wxT("Billboard") );
	m_cboLabelStyle->Append( wxT("Dynamic") );
	fgSizer1->Add( m_cboLabelStyle, 0, wxALL, 5 );
	
	m_lblLabelColour = new wxStaticText( m_pnlLocationSetLabel, ID_LBL_LABEL_COLOUR, wxT("Label colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLabelColour->Wrap( -1 );
	fgSizer1->Add( m_lblLabelColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_colourLabel = new wxColourPickerCtrl( m_pnlLocationSetLabel, ID_COLOUR_PICKER_LABEL_COLOUR, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer1->Add( m_colourLabel, 0, wxALL, 5 );
	
	m_sizerLabelProp->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	m_sizerLabelVert->Add( m_sizerLabelProp, 0, wxALL|wxEXPAND, 5 );
	
	m_pnlLocationSetLabel->SetSizer( m_sizerLabelVert );
	m_pnlLocationSetLabel->Layout();
	m_sizerLabelVert->Fit( m_pnlLocationSetLabel );
	m_notebookLocationSet->AddPage( m_pnlLocationSetLabel, wxT("Label"), true );
	
	m_sizerLocations->Add( m_notebookLocationSet, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* m_sizerShowLocations;
	m_sizerShowLocations = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_sizerShowLocations->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sizerLocations->Add( m_sizerShowLocations, 0, wxEXPAND, 5 );
	
	m_pnlLocations->SetSizer( m_sizerLocations );
	m_pnlLocations->Layout();
	m_sizerLocations->Fit( m_pnlLocations );
	m_notebook->AddPage( m_pnlLocations, wxT("Location Set"), false );
	m_pnlChart = new wxPanel( m_notebook, ID_PNL_CHART, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("Charts") );
	wxBoxSizer* m_sizerLabels1;
	m_sizerLabels1 = new wxBoxSizer( wxVERTICAL );
	
	m_notebookChart = new wxNotebook( m_pnlChart, ID_NOTEBOOK_CHART, wxDefaultPosition, wxDefaultSize, 0 );
	m_pnlChartColourMap = new wxPanel( m_notebookChart, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("Colour_Map") );
	wxBoxSizer* m_sizerChartGeneral;
	m_sizerChartGeneral = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerChartField;
	m_sizerChartField = new wxStaticBoxSizer( new wxStaticBox( m_pnlChartColourMap, wxID_ANY, wxT("Field Properties") ), wxVERTICAL );
	
	wxFlexGridSizer* m_sizerColourMapFields;
	m_sizerColourMapFields = new wxFlexGridSizer( 2, 2, 0, 0 );
	m_sizerColourMapFields->SetFlexibleDirection( wxBOTH );
	m_sizerColourMapFields->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblChartField = new wxStaticText( m_pnlChartColourMap, ID_LBL_LABEL_CHART_FIELD, wxT("Field to chart:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChartField->Wrap( -1 );
	m_sizerColourMapFields->Add( m_lblChartField, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboChartField = new wxComboBox( m_pnlChartColourMap, ID_CBO_CHART_FIELD, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	m_sizerColourMapFields->Add( m_cboChartField, 0, wxALL|wxEXPAND, 5 );
	
	m_lblChartColourMap = new wxStaticText( m_pnlChartColourMap, ID_LBL_COLOUR_MAP, wxT("Colour map:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChartColourMap->Wrap( -1 );
	m_sizerColourMapFields->Add( m_lblChartColourMap, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboChartColourMap = new wxComboBox( m_pnlChartColourMap, ID_CBO_CHART_COLOUR_MAP, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	m_sizerColourMapFields->Add( m_cboChartColourMap, 0, wxALL|wxEXPAND, 5 );
	
	m_sizerChartField->Add( m_sizerColourMapFields, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerWedgeColour;
	m_sizerWedgeColour = new wxBoxSizer( wxHORIZONTAL );
	
	m_scrolledWindowChart = new wxScrolledWindow( m_pnlChartColourMap, ID_SCROLLED_WINDOW_CHART, wxDefaultPosition, wxSize( -1,-1 ), wxHSCROLL|wxSTATIC_BORDER|wxVSCROLL );
	m_scrolledWindowChart->SetScrollRate( 5, 5 );
	m_scrolledWindowChart->SetMinSize( wxSize( -1,250 ) );
	
	m_sizerWedgeColour->Add( m_scrolledWindowChart, 10, wxALL|wxEXPAND, 5 );
	
	m_sizerChartField->Add( m_sizerWedgeColour, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer( wxHORIZONTAL );
	
	m_chkQuantitative = new wxCheckBox( m_pnlChartColourMap, wxID_ANY, wxT("Create quantitative charts using count data in field:"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer45->Add( m_chkQuantitative, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_cboQuantitativeField = new wxComboBox( m_pnlChartColourMap, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	bSizer45->Add( m_cboQuantitativeField, 0, wxALL, 5 );
	
	m_sizerChartField->Add( bSizer45, 0, wxEXPAND, 5 );
	
	m_sizerChartGeneral->Add( m_sizerChartField, 1, wxALL|wxEXPAND, 5 );
	
	m_pnlChartColourMap->SetSizer( m_sizerChartGeneral );
	m_pnlChartColourMap->Layout();
	m_sizerChartGeneral->Fit( m_pnlChartColourMap );
	m_notebookChart->AddPage( m_pnlChartColourMap, wxT("Colour Map"), true );
	m_pnlChartSymbology = new wxPanel( m_notebookChart, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("Symbology") );
	wxBoxSizer* m_sizerChartSymbology;
	m_sizerChartSymbology = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerChartProperties;
	m_sizerChartProperties = new wxStaticBoxSizer( new wxStaticBox( m_pnlChartSymbology, wxID_ANY, wxT("Chart Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerChartType;
	m_sizerChartType = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblChartType = new wxStaticText( m_pnlChartSymbology, ID_LBL_LABEL_COLOUR, wxT("Chart type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChartType->Wrap( -1 );
	m_sizerChartType->Add( m_lblChartType, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboChartType = new wxComboBox( m_pnlChartSymbology, ID_CBO_CHART_TYPE, wxT("3D Pie Chart"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	m_cboChartType->Append( wxT("Bar Chart") );
	m_cboChartType->Append( wxT("Pie Chart") );
	m_sizerChartType->Add( m_cboChartType, 0, wxALL, 4 );
	
	m_chkChart2D = new wxCheckBox( m_pnlChartSymbology, wxID_ANY, wxT("Display in 2D"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerChartType->Add( m_chkChart2D, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sizerChartProperties->Add( m_sizerChartType, 0, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( m_pnlChartSymbology, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_sizerChartProperties->Add( m_staticline2, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* m_sizerChartSize;
	m_sizerChartSize = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblChartWidth = new wxStaticText( m_pnlChartSymbology, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChartWidth->Wrap( -1 );
	m_sizerChartSize->Add( m_lblChartWidth, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinChartWidth = new wxSpinCtrl( m_pnlChartSymbology, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 20 );
	m_sizerChartSize->Add( m_spinChartWidth, 0, wxALL, 4 );
	
	m_lblChartHeight = new wxStaticText( m_pnlChartSymbology, wxID_ANY, wxT("Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChartHeight->Wrap( -1 );
	m_sizerChartSize->Add( m_lblChartHeight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_spinChartHeight = new wxSpinCtrl( m_pnlChartSymbology, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 20 );
	m_sizerChartSize->Add( m_spinChartHeight, 0, wxALL, 5 );
	
	m_sizerChartProperties->Add( m_sizerChartSize, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerChartSizeBySeqCount;
	m_sizerChartSizeBySeqCount = new wxBoxSizer( wxVERTICAL );
	
	m_chkChartSizeBySeqCount = new wxCheckBox( m_pnlChartSymbology, ID_CHK_CHART_SIZE_BY_SEQ_COUNT, wxT("Set chart size proportional to number of sequences"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE );
	m_sizerChartSizeBySeqCount->Add( m_chkChartSizeBySeqCount, 0, wxALL, 4 );
	
	m_sizerChartProperties->Add( m_sizerChartSizeBySeqCount, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerChartSizeBounds;
	m_sizerChartSizeBounds = new wxBoxSizer( wxHORIZONTAL );
	
	m_chartMinSize = new wxStaticText( m_pnlChartSymbology, IDLBL_CHART_MIN_SIZE, wxT("Min Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chartMinSize->Wrap( -1 );
	m_sizerChartSizeBounds->Add( m_chartMinSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 4 );
	
	m_txtChartMinSize = new wxTextCtrl( m_pnlChartSymbology, ID_TXT_CHART_MIN_SIZE, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtChartMinSize->Enable( false );
	
	m_sizerChartSizeBounds->Add( m_txtChartMinSize, 0, wxALL, 4 );
	
	m_lblChartMaxSize = new wxStaticText( m_pnlChartSymbology, wxID_ANY, wxT("Max Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChartMaxSize->Wrap( -1 );
	m_sizerChartSizeBounds->Add( m_lblChartMaxSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 4 );
	
	m_txtChartMaxSize = new wxTextCtrl( m_pnlChartSymbology, ID_TXT_CHART_MAX_SIZE, wxT("30"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtChartMaxSize->Enable( false );
	
	m_sizerChartSizeBounds->Add( m_txtChartMaxSize, 0, wxALL, 4 );
	
	m_sizerChartProperties->Add( m_sizerChartSizeBounds, 0, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( m_pnlChartSymbology, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_sizerChartProperties->Add( m_staticline1, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* m_sizerChartBorder;
	m_sizerChartBorder = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblChartBorderSize = new wxStaticText( m_pnlChartSymbology, ID_LBL_CHART_BORDER_SIZE, wxT("Chart border size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChartBorderSize->Wrap( -1 );
	m_sizerChartBorder->Add( m_lblChartBorderSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 4 );
	
	m_spinChartBorderSize = new wxSpinCtrl( m_pnlChartSymbology, ID_SPIN_CHART_BORDER_SIZE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 1 );
	m_sizerChartBorder->Add( m_spinChartBorderSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 4 );
	
	m_colourChartBorder = new wxColourPickerCtrl( m_pnlChartSymbology, ID_COLOUR_PICKER_CHART_BORDER, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerChartBorder->Add( m_colourChartBorder, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_chkAssignLocationColour = new wxCheckBox( m_pnlChartSymbology, wxID_ANY, wxT("Assign location colour to border"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerChartBorder->Add( m_chkAssignLocationColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sizerChartProperties->Add( m_sizerChartBorder, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxHORIZONTAL );
	
	m_chkBottomBorder = new wxCheckBox( m_pnlChartSymbology, wxID_ANY, wxT("Bottom border"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( m_chkBottomBorder, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_chkLeftBorder = new wxCheckBox( m_pnlChartSymbology, wxID_ANY, wxT("Left border"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( m_chkLeftBorder, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_chkTopBorder = new wxCheckBox( m_pnlChartSymbology, wxID_ANY, wxT("Top border"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( m_chkTopBorder, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_chkRightBorder = new wxCheckBox( m_pnlChartSymbology, wxID_ANY, wxT("Right border"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( m_chkRightBorder, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sizerChartProperties->Add( bSizer43, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerShowWedgeBorders;
	m_sizerShowWedgeBorders = new wxBoxSizer( wxHORIZONTAL );
	
	m_chkShowElementBorders = new wxCheckBox( m_pnlChartSymbology, wxID_ANY, wxT("Show individual element borders"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE );
	m_sizerShowWedgeBorders->Add( m_chkShowElementBorders, 0, wxALIGN_CENTER_VERTICAL|wxALL, 4 );
	
	m_sizerChartProperties->Add( m_sizerShowWedgeBorders, 1, wxEXPAND, 5 );
	
	m_lineAssignTaxaToOther = new wxStaticLine( m_pnlChartSymbology, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_sizerChartProperties->Add( m_lineAssignTaxaToOther, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* m_sizerChartBorder1;
	m_sizerChartBorder1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblChartAssignToOther = new wxStaticText( m_pnlChartSymbology, ID_LBL_CHART_ASSIGN_TO_OTHER, wxT("Assign taxa to other (percentage):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChartAssignToOther->Wrap( -1 );
	m_sizerChartBorder1->Add( m_lblChartAssignToOther, 0, wxALL|wxALIGN_CENTER_VERTICAL, 4 );
	
	m_spinChartFilterTaxaPercentage = new wxSpinCtrl( m_pnlChartSymbology, ID_SPIN_CHART_BORDER_SIZE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 5 );
	m_sizerChartBorder1->Add( m_spinChartFilterTaxaPercentage, 0, wxALL, 4 );
	
	m_sizerChartProperties->Add( m_sizerChartBorder1, 1, wxEXPAND, 5 );
	
	m_lineColourChartBackground = new wxStaticLine( m_pnlChartSymbology, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_sizerChartProperties->Add( m_lineColourChartBackground, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblChartBgColour = new wxStaticText( m_pnlChartSymbology, wxID_ANY, wxT("Background colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChartBgColour->Wrap( -1 );
	bSizer42->Add( m_lblChartBgColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourChartBackground = new wxColourPickerCtrl( m_pnlChartSymbology, ID_COLOUR_PICKER_CHART_BACKGROUND, wxColour( 255, 255, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer42->Add( m_colourChartBackground, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_chkTransparentBackground = new wxCheckBox( m_pnlChartSymbology, wxID_ANY, wxT("Transparent"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkTransparentBackground->SetValue(true); 
	bSizer42->Add( m_chkTransparentBackground, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sizerChartProperties->Add( bSizer42, 1, wxEXPAND, 5 );
	
	m_sizerChartSymbology->Add( m_sizerChartProperties, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerChartDroplineProperties;
	m_sizerChartDroplineProperties = new wxStaticBoxSizer( new wxStaticBox( m_pnlChartSymbology, wxID_ANY, wxT("Dropline Properties") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblChartDroplineStyle = new wxStaticText( m_pnlChartSymbology, ID_LBL_LABEL_CHART_FIELD, wxT("Line style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChartDroplineStyle->Wrap( -1 );
	fgSizer5->Add( m_lblChartDroplineStyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 4 );
	
	m_cboChartDropLineStyle = new wxComboBox( m_pnlChartSymbology, ID_CBO_DROPLINE_STYLE, wxT("Solid"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	m_cboChartDropLineStyle->Append( wxT("Solid") );
	m_cboChartDropLineStyle->Append( wxT("Short dash") );
	m_cboChartDropLineStyle->Append( wxT("Long dash") );
	m_cboChartDropLineStyle->Append( wxT("Hidden") );
	fgSizer5->Add( m_cboChartDropLineStyle, 0, wxALL|wxEXPAND, 4 );
	
	m_lblChartDroplineThickness = new wxStaticText( m_pnlChartSymbology, ID_LBL_DROPLINE_THICKNESS, wxT("Line thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChartDroplineThickness->Wrap( -1 );
	fgSizer5->Add( m_lblChartDroplineThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL, 4 );
	
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spinChartDropLineThickness = new wxSpinCtrl( m_pnlChartSymbology, ID_SPIN_DROPLINE_THICKNESS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 1 );
	bSizer41->Add( m_spinChartDropLineThickness, 0, wxALL, 4 );
	
	m_colourChartDropLine = new wxColourPickerCtrl( m_pnlChartSymbology, ID_COLOUR_PICKER_DROPLINE, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer41->Add( m_colourChartDropLine, 0, wxALL, 5 );
	
	fgSizer5->Add( bSizer41, 1, wxEXPAND, 5 );
	
	m_sizerChartDroplineProperties->Add( fgSizer5, 1, wxEXPAND, 5 );
	
	m_sizerChartSymbology->Add( m_sizerChartDroplineProperties, 0, wxALL|wxEXPAND, 5 );
	
	m_pnlChartSymbology->SetSizer( m_sizerChartSymbology );
	m_pnlChartSymbology->Layout();
	m_sizerChartSymbology->Fit( m_pnlChartSymbology );
	m_notebookChart->AddPage( m_pnlChartSymbology, wxT("Symbology"), false );
	
	m_sizerLabels1->Add( m_notebookChart, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* m_sizerShowCharts;
	m_sizerShowCharts = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_sizerShowCharts->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_chkShowCharts = new wxCheckBox( m_pnlChart, ID_CHECK_SHOW_CHART, wxT("Show charts"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE );
	m_sizerShowCharts->Add( m_chkShowCharts, 0, wxALL, 5 );
	
	m_sizerLabels1->Add( m_sizerShowCharts, 0, wxEXPAND, 5 );
	
	m_pnlChart->SetSizer( m_sizerLabels1 );
	m_pnlChart->Layout();
	m_sizerLabels1->Fit( m_pnlChart );
	m_notebook->AddPage( m_pnlChart, wxT("Charts"), false );
	m_pnlGrid = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("Grid") );
	wxBoxSizer* m_sizerGrid;
	m_sizerGrid = new wxBoxSizer( wxVERTICAL );
	
	m_notebook6 = new wxNotebook( m_pnlGrid, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel13 = new wxPanel( m_notebook6, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer431;
	bSizer431 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer12;
	sbSizer12 = new wxStaticBoxSizer( new wxStaticBox( m_panel13, wxID_ANY, wxT("Colour Fill Properties") ), wxVERTICAL );
	
	wxBoxSizer* bSizer452;
	bSizer452 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_radioGridNoFill = new wxRadioButton( m_panel13, wxID_RADIO_TILE_COLOUR_NO_FILL, wxT("No fill"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	fgSizer10->Add( m_radioGridNoFill, 0, wxALL, 5 );
	
	
	fgSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_radioGridUniformColour = new wxRadioButton( m_panel13, wxID_RADIO_TILE_COLOUR_UNIFORM_FILL, wxT("Uniform colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_radioGridUniformColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_gridTileColour = new wxColourPickerCtrl( m_panel13, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer10->Add( m_gridTileColour, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_radioGridColourMap = new wxRadioButton( m_panel13, wxID_RADIO_TILE_COLOUR_MAP_FILL, wxT("Colour Map"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_radioGridColourMap, 1, wxALL, 5 );
	
	bSizer452->Add( fgSizer10, 0, 0, 5 );
	
	
	bSizer452->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_txtTileAlpha = new wxStaticText( m_panel13, wxID_ANY, wxT("Opacity:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtTileAlpha->Wrap( -1 );
	fgSizer14->Add( m_txtTileAlpha, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sliderTileAlpha = new wxSlider( m_panel13, wxID_ANY, 8, 0, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer14->Add( m_sliderTileAlpha, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer452->Add( fgSizer14, 1, wxALIGN_RIGHT|wxALIGN_BOTTOM, 5 );
	
	sbSizer12->Add( bSizer452, 0, wxEXPAND, 5 );
	
	m_staticline13 = new wxStaticLine( m_panel13, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLI_HORIZONTAL );
	sbSizer12->Add( m_staticline13, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 2, 4, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	m_txtGridFieldToChart = new wxStaticText( m_panel13, wxID_ANY, wxT("Field to chart:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtGridFieldToChart->Wrap( -1 );
	fgSizer7->Add( m_txtGridFieldToChart, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_choiceGridFieldToChart = new wxComboBox( m_panel13, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	fgSizer7->Add( m_choiceGridFieldToChart, 0, wxALL, 5 );
	
	m_txtGridDefaultColour = new wxStaticText( m_panel13, wxID_ANY, wxT("Default Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtGridDefaultColour->Wrap( -1 );
	fgSizer7->Add( m_txtGridDefaultColour, 0, wxALL, 5 );
	
	m_colourGridDefaultColour = new wxColourPickerCtrl( m_panel13, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer7->Add( m_colourGridDefaultColour, 1, wxALIGN_CENTER|wxALL, 5 );
	
	m_txtGridColourMap = new wxStaticText( m_panel13, wxID_ANY, wxT("Colour map:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtGridColourMap->Wrap( -1 );
	fgSizer7->Add( m_txtGridColourMap, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_choiceGridColourMap = new wxComboBox( m_panel13, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	fgSizer7->Add( m_choiceGridColourMap, 0, wxALL, 5 );
	
	m_txtDefaultTileOpacity = new wxStaticText( m_panel13, wxID_ANY, wxT("Default Opacity:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtDefaultTileOpacity->Wrap( -1 );
	fgSizer7->Add( m_txtDefaultTileOpacity, 0, wxALL, 5 );
	
	m_sliderDefaultTileAlpha = new wxSlider( m_panel13, wxID_ANY, 8, 0, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer7->Add( m_sliderDefaultTileAlpha, 0, wxALL, 5 );
	
	sbSizer12->Add( fgSizer7, 0, wxEXPAND, 5 );
	
	m_scrolledWindowGridColour = new wxScrolledWindow( m_panel13, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindowGridColour->SetScrollRate( 5, 5 );
	sbSizer12->Add( m_scrolledWindowGridColour, 1, wxALL|wxEXPAND, 5 );
	
	bSizer431->Add( sbSizer12, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer13;
	sbSizer13 = new wxStaticBoxSizer( new wxStaticBox( m_panel13, wxID_ANY, wxT("Grid Borders") ), wxVERTICAL );
	
	m_chkShowGridBorders = new wxCheckBox( m_panel13, wxID_ANY, wxT("Show borders"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer13->Add( m_chkShowGridBorders, 0, wxALL, 5 );
	
	m_staticline101 = new wxStaticLine( m_panel13, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sbSizer13->Add( m_staticline101, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 2, 4, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_txtGridBorderColour = new wxStaticText( m_panel13, wxID_ANY, wxT("Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtGridBorderColour->Wrap( -1 );
	fgSizer9->Add( m_txtGridBorderColour, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_txtGridBorderAlpha = new wxStaticText( m_panel13, wxID_ANY, wxT("Opacity:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtGridBorderAlpha->Wrap( -1 );
	fgSizer9->Add( m_txtGridBorderAlpha, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtGridBorderThickness = new wxStaticText( m_panel13, wxID_ANY, wxT("Thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtGridBorderThickness->Wrap( -1 );
	fgSizer9->Add( m_txtGridBorderThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_txtGridBorderStyle = new wxStaticText( m_panel13, wxID_ANY, wxT("Style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtGridBorderStyle->Wrap( -1 );
	fgSizer9->Add( m_txtGridBorderStyle, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_gridBorderColour = new wxColourPickerCtrl( m_panel13, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer9->Add( m_gridBorderColour, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_sliderBorderAlpha = new wxSlider( m_panel13, wxID_ANY, 10, 0, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer9->Add( m_sliderBorderAlpha, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinGridBorderThickness = new wxSpinCtrl( m_panel13, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10, 1 );
	fgSizer9->Add( m_spinGridBorderThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_cboGridBorderStyle = new wxComboBox( m_panel13, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboGridBorderStyle->Append( wxT("Hidden") );
	m_cboGridBorderStyle->Append( wxT("Solid") );
	m_cboGridBorderStyle->Append( wxT("Short dash") );
	m_cboGridBorderStyle->Append( wxT("Long dash") );
	fgSizer9->Add( m_cboGridBorderStyle, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer13->Add( fgSizer9, 0, wxEXPAND, 5 );
	
	bSizer431->Add( sbSizer13, 0, wxEXPAND, 5 );
	
	m_panel13->SetSizer( bSizer431 );
	m_panel13->Layout();
	bSizer431->Fit( m_panel13 );
	m_notebook6->AddPage( m_panel13, wxT("Colour"), false );
	m_panel12 = new wxPanel( m_notebook6, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer451;
	bSizer451 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer16;
	sbSizer16 = new wxStaticBoxSizer( new wxStaticBox( m_panel12, wxID_ANY, wxT("Tile Size") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 2, 4, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_radioBox = new wxRadioButton( m_panel12, wxID_DIVIDE_BY_BOX, wxT("Tile Size"), wxDefaultPosition, wxDefaultSize, wxRB_SINGLE );
	fgSizer12->Add( m_radioBox, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinBoxDivisions = new wxSpinCtrl( m_panel12, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 90, 1 );
	m_spinBoxDivisions->Enable( false );
	
	fgSizer12->Add( m_spinBoxDivisions, 0, wxALL, 5 );
	
	m_radioBtnDegrees = new wxRadioButton( m_panel12, wxID_DIVIDE_INTO_DEGREES, wxT("Degrees"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	fgSizer12->Add( m_radioBtnDegrees, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_radioBtnPixels = new wxRadioButton( m_panel12, wxID_DIVIDE_INTO_PIXELS, wxT("Pixels"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12->Add( m_radioBtnPixels, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_radioAxis = new wxRadioButton( m_panel12, wxID_DIVIDE_BY_AXIS, wxT("Divisions"), wxDefaultPosition, wxDefaultSize, wxRB_SINGLE );
	m_radioAxis->SetValue( true ); 
	fgSizer12->Add( m_radioAxis, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_spinGridDivisions = new wxSpinCtrl( m_panel12, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 255, 1 );
	fgSizer12->Add( m_spinGridDivisions, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_radioBtnLatitude = new wxRadioButton( m_panel12, wxID_RADIO_GRID_DIVIDE_ALONG_LATITUDE, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	fgSizer12->Add( m_radioBtnLatitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_radioBtnLongitude = new wxRadioButton( m_panel12, wxID_RADIO_GRID_DIVIDE_ALONG_LONGITUDE, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12->Add( m_radioBtnLongitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	sbSizer16->Add( fgSizer12, 0, wxEXPAND, 5 );
	
	bSizer451->Add( sbSizer16, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer17;
	sbSizer17 = new wxStaticBoxSizer( new wxStaticBox( m_panel12, wxID_ANY, wxT("Grid Alignment") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_radioAlignToOrigin = new wxRadioButton( m_panel12, wxID_RADIO_GRID_ALIGN_TO_ORIGIN, wxT("Align to origin"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	fgSizer11->Add( m_radioAlignToOrigin, 0, wxALL, 5 );
	
	
	fgSizer11->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_radioAlignToLocation = new wxRadioButton( m_panel12, wxID_RADIO_GRID_ALIGN_TO_LOCATION, wxT("Align to location:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_radioAlignToLocation->SetValue( true ); 
	fgSizer11->Add( m_radioAlignToLocation, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_choiceAlignToLocation = new wxComboBox( m_panel12, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	fgSizer11->Add( m_choiceAlignToLocation, 0, wxALL, 5 );
	
	m_radioAlignToCoordinates = new wxRadioButton( m_panel12, wxID_RADIO_GRID_ALIGN_TO_COORDINATES, wxT("Align to coordinates:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_radioAlignToCoordinates, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	sbSizer17->Add( fgSizer11, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer121;
	fgSizer121 = new wxFlexGridSizer( 2, 6, 0, 0 );
	fgSizer121->SetFlexibleDirection( wxBOTH );
	fgSizer121->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_txtLatitude = new wxStaticText( m_panel12, wxID_ANY, wxT("Latitude:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtLatitude->Wrap( -1 );
	m_txtLatitude->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer121->Add( m_txtLatitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtMinLatitude = new wxStaticText( m_panel12, wxID_ANY, wxT("-90"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtMinLatitude->Wrap( -1 );
	fgSizer121->Add( m_txtMinLatitude, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtLatitudeLessEqualThan1 = new wxStaticText( m_panel12, wxID_ANY, wxT("<="), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtLatitudeLessEqualThan1->Wrap( -1 );
	fgSizer121->Add( m_txtLatitudeLessEqualThan1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrlLatitude = new wxTextCtrl( m_panel12, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer121->Add( m_textCtrlLatitude, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_txtLatitudeLessEqualThan2 = new wxStaticText( m_panel12, wxID_ANY, wxT("<="), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtLatitudeLessEqualThan2->Wrap( -1 );
	fgSizer121->Add( m_txtLatitudeLessEqualThan2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtMaxLatitude = new wxStaticText( m_panel12, wxID_ANY, wxT("90"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtMaxLatitude->Wrap( -1 );
	fgSizer121->Add( m_txtMaxLatitude, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtLongitude = new wxStaticText( m_panel12, wxID_ANY, wxT("Longitude:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtLongitude->Wrap( -1 );
	m_txtLongitude->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	fgSizer121->Add( m_txtLongitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtMinLongitude = new wxStaticText( m_panel12, wxID_ANY, wxT("-180"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtMinLongitude->Wrap( -1 );
	fgSizer121->Add( m_txtMinLongitude, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtLongitudeLessEqualThan1 = new wxStaticText( m_panel12, wxID_ANY, wxT("<="), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtLongitudeLessEqualThan1->Wrap( -1 );
	fgSizer121->Add( m_txtLongitudeLessEqualThan1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrlLongitude = new wxTextCtrl( m_panel12, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer121->Add( m_textCtrlLongitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtLongitudeLessEqualThan2 = new wxStaticText( m_panel12, wxID_ANY, wxT("<="), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtLongitudeLessEqualThan2->Wrap( -1 );
	fgSizer121->Add( m_txtLongitudeLessEqualThan2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtMaxLongitude = new wxStaticText( m_panel12, wxID_ANY, wxT("180"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtMaxLongitude->Wrap( -1 );
	fgSizer121->Add( m_txtMaxLongitude, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	sbSizer17->Add( fgSizer121, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer13->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_buttonGridPositionReset = new wxButton( m_panel12, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer13->Add( m_buttonGridPositionReset, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_buttonClickMapToAlign = new wxButton( m_panel12, wxID_ANY, wxT("Click map to align"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer13->Add( m_buttonClickMapToAlign, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	sbSizer17->Add( fgSizer13, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	bSizer451->Add( sbSizer17, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerGridPositionProperties;
	m_sizerGridPositionProperties = new wxStaticBoxSizer( new wxStaticBox( m_panel12, wxID_ANY, wxT("Grid Elevation (Altitude)") ), wxVERTICAL );
	
	m_chkAutoAdjustElevation = new wxCheckBox( m_panel12, wxID_ANY, wxT("Auto adjust to map elevation"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkAutoAdjustElevation->SetValue(true); 
	m_sizerGridPositionProperties->Add( m_chkAutoAdjustElevation, 0, wxALL, 5 );
	
	m_staticline8 = new wxStaticLine( m_panel12, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_sizerGridPositionProperties->Add( m_staticline8, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 1, 4, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblVerticalElevation = new wxStaticText( m_panel12, wxID_ANY, wxT("Vertical elevation:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblVerticalElevation->Wrap( -1 );
	fgSizer8->Add( m_lblVerticalElevation, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrlGridElevation = new wxTextCtrl( m_panel12, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( m_textCtrlGridElevation, 0, wxALL, 5 );
	
	m_sizerGridPositionProperties->Add( fgSizer8, 0, wxEXPAND, 5 );
	
	bSizer451->Add( m_sizerGridPositionProperties, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerTileFields;
	m_sizerTileFields = new wxStaticBoxSizer( new wxStaticBox( m_panel12, wxID_ANY, wxT("Tile Fields") ), wxVERTICAL );
	
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_txtTileFieldChoice = new wxStaticText( m_panel12, wxID_ANY, wxT("Tile field combination method:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtTileFieldChoice->Wrap( -1 );
	gbSizer1->Add( m_txtTileFieldChoice, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_tileFieldChoice = new wxComboBox( m_panel12, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_tileFieldChoice->Append( wxT("Average") );
	m_tileFieldChoice->Append( wxT("Standard Deviation") );
	m_tileFieldChoice->Append( wxT("Gini Index") );
	m_tileFieldChoice->Append( wxT("Sum") );
	gbSizer1->Add( m_tileFieldChoice, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_sizerTileFields->Add( gbSizer1, 1, wxEXPAND, 5 );
	
	bSizer451->Add( m_sizerTileFields, 0, wxEXPAND, 5 );
	
	m_panel12->SetSizer( bSizer451 );
	m_panel12->Layout();
	bSizer451->Fit( m_panel12 );
	m_notebook6->AddPage( m_panel12, wxT("Custom"), true );
	
	m_sizerGrid->Add( m_notebook6, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* m_sizerLayerSource1;
	m_sizerLayerSource1 = new wxBoxSizer( wxVERTICAL );
	
	m_chkShowGrid = new wxCheckBox( m_pnlGrid, wxID_ANY, wxT("Show Grid"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerLayerSource1->Add( m_chkShowGrid, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	m_sizerGrid->Add( m_sizerLayerSource1, 0, wxEXPAND, 5 );
	
	m_pnlGrid->SetSizer( m_sizerGrid );
	m_pnlGrid->Layout();
	m_sizerGrid->Fit( m_pnlGrid );
	m_notebook->AddPage( m_pnlGrid, wxT("Grid"), true );
	m_pnlMetadata = new wxPanel( m_notebook, ID_PNL_METADATA, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("Metadata") );
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
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( LocationSetPropertiesLayout::OnClose ) );
	m_notebook->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( LocationSetPropertiesLayout::OnPageChange ), NULL, this );
	m_chkUniformColour->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnUniformColour ), NULL, this );
	m_cboColourField->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnColourFieldChange ), NULL, this );
	m_cboColourMap->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnColourMapChange ), NULL, this );
	m_chkUniformShape->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnUniformShape ), NULL, this );
	m_cboShapeField->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnShapeFieldChange ), NULL, this );
	m_cboShapeMap->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnShapeMapChange ), NULL, this );
	m_cboSizeField->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnSizeFieldChange ), NULL, this );
	m_chkShowLabels->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnShowLabels ), NULL, this );
	m_cboChartField->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnChartFieldChange ), NULL, this );
	m_cboChartColourMap->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnChartColourMapChange ), NULL, this );
	m_chkQuantitative->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnQuantitative ), NULL, this );
	m_cboQuantitativeField->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnQuantitativeFieldChanged ), NULL, this );
	m_cboChartType->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationSetPropertiesLayout::OnChartTypeChanged ), NULL, this );
	m_spinChartWidth->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocationSetPropertiesLayout::OnSpinChartSize ), NULL, this );
	m_chkChartSizeBySeqCount->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnProportionalChartSize ), NULL, this );
	m_spinChartBorderSize->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocationSetPropertiesLayout::OnSpinBorderSize ), NULL, this );
	m_spinChartFilterTaxaPercentage->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocationSetPropertiesLayout::OnSpinAssignTaxa ), NULL, this );
	m_cboChartDropLineStyle->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnCboFieldChange ), NULL, this );
	m_spinChartDropLineThickness->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocationSetPropertiesLayout::OnSpinLineThickness ), NULL, this );
	m_radioGridNoFill->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioColourFill ), NULL, this );
	m_radioGridUniformColour->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioColourFill ), NULL, this );
	m_radioGridColourMap->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioColourFill ), NULL, this );
	m_choiceGridFieldToChart->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnChoiceGridFieldToChartChange ), NULL, this );
	m_choiceGridColourMap->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnGridColourMapChange ), NULL, this );
	m_chkShowGridBorders->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnShowGridBorders ), NULL, this );
	m_radioBox->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioDivideBy ), NULL, this );
	m_spinBoxDivisions->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocationSetPropertiesLayout::OnNumberOfDivisions ), NULL, this );
	m_spinBoxDivisions->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationSetPropertiesLayout::OnNumberOfDivisions ), NULL, this );
	m_radioBtnDegrees->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioDivideType ), NULL, this );
	m_radioBtnPixels->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioDivideType ), NULL, this );
	m_radioAxis->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioDivideBy ), NULL, this );
	m_spinGridDivisions->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocationSetPropertiesLayout::OnNumberOfDivisions ), NULL, this );
	m_spinGridDivisions->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationSetPropertiesLayout::OnNumberOfDivisions ), NULL, this );
	m_radioBtnLatitude->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioLatitudeLongitude ), NULL, this );
	m_radioBtnLongitude->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioLatitudeLongitude ), NULL, this );
	m_radioAlignToOrigin->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioAlignTo ), NULL, this );
	m_radioAlignToLocation->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioAlignTo ), NULL, this );
	m_choiceAlignToLocation->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnGridChanged ), NULL, this );
	m_radioAlignToCoordinates->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioAlignTo ), NULL, this );
	m_textCtrlLatitude->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationSetPropertiesLayout::OnAlignToLocationChange ), NULL, this );
	m_textCtrlLongitude->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationSetPropertiesLayout::OnAlignToLocationChange ), NULL, this );
	m_buttonGridPositionReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnCoordinateReset ), NULL, this );
	m_buttonClickMapToAlign->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnAlignCoordinateToMouse ), NULL, this );
	m_chkAutoAdjustElevation->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnAutoAdjustElevation ), NULL, this );
	m_textCtrlGridElevation->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationSetPropertiesLayout::OnGridChanged ), NULL, this );
	m_tileFieldChoice->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnTileFieldChoiceChange ), NULL, this );
	m_btnHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnCancel ), NULL, this );
}

LocationSetPropertiesLayout::~LocationSetPropertiesLayout()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( LocationSetPropertiesLayout::OnClose ) );
	m_notebook->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( LocationSetPropertiesLayout::OnPageChange ), NULL, this );
	m_chkUniformColour->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnUniformColour ), NULL, this );
	m_cboColourField->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnColourFieldChange ), NULL, this );
	m_cboColourMap->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnColourMapChange ), NULL, this );
	m_chkUniformShape->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnUniformShape ), NULL, this );
	m_cboShapeField->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnShapeFieldChange ), NULL, this );
	m_cboShapeMap->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnShapeMapChange ), NULL, this );
	m_cboSizeField->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnSizeFieldChange ), NULL, this );
	m_chkShowLabels->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnShowLabels ), NULL, this );
	m_cboChartField->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnChartFieldChange ), NULL, this );
	m_cboChartColourMap->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnChartColourMapChange ), NULL, this );
	m_chkQuantitative->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnQuantitative ), NULL, this );
	m_cboQuantitativeField->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnQuantitativeFieldChanged ), NULL, this );
	m_cboChartType->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationSetPropertiesLayout::OnChartTypeChanged ), NULL, this );
	m_spinChartWidth->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocationSetPropertiesLayout::OnSpinChartSize ), NULL, this );
	m_chkChartSizeBySeqCount->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnProportionalChartSize ), NULL, this );
	m_spinChartBorderSize->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocationSetPropertiesLayout::OnSpinBorderSize ), NULL, this );
	m_spinChartFilterTaxaPercentage->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocationSetPropertiesLayout::OnSpinAssignTaxa ), NULL, this );
	m_cboChartDropLineStyle->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnCboFieldChange ), NULL, this );
	m_spinChartDropLineThickness->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocationSetPropertiesLayout::OnSpinLineThickness ), NULL, this );
	m_radioGridNoFill->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioColourFill ), NULL, this );
	m_radioGridUniformColour->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioColourFill ), NULL, this );
	m_radioGridColourMap->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioColourFill ), NULL, this );
	m_choiceGridFieldToChart->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnChoiceGridFieldToChartChange ), NULL, this );
	m_choiceGridColourMap->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnGridColourMapChange ), NULL, this );
	m_chkShowGridBorders->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnShowGridBorders ), NULL, this );
	m_radioBox->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioDivideBy ), NULL, this );
	m_spinBoxDivisions->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocationSetPropertiesLayout::OnNumberOfDivisions ), NULL, this );
	m_spinBoxDivisions->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationSetPropertiesLayout::OnNumberOfDivisions ), NULL, this );
	m_radioBtnDegrees->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioDivideType ), NULL, this );
	m_radioBtnPixels->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioDivideType ), NULL, this );
	m_radioAxis->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioDivideBy ), NULL, this );
	m_spinGridDivisions->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( LocationSetPropertiesLayout::OnNumberOfDivisions ), NULL, this );
	m_spinGridDivisions->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationSetPropertiesLayout::OnNumberOfDivisions ), NULL, this );
	m_radioBtnLatitude->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioLatitudeLongitude ), NULL, this );
	m_radioBtnLongitude->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioLatitudeLongitude ), NULL, this );
	m_radioAlignToOrigin->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioAlignTo ), NULL, this );
	m_radioAlignToLocation->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioAlignTo ), NULL, this );
	m_choiceAlignToLocation->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnGridChanged ), NULL, this );
	m_radioAlignToCoordinates->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnRadioAlignTo ), NULL, this );
	m_textCtrlLatitude->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationSetPropertiesLayout::OnAlignToLocationChange ), NULL, this );
	m_textCtrlLongitude->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationSetPropertiesLayout::OnAlignToLocationChange ), NULL, this );
	m_buttonGridPositionReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnCoordinateReset ), NULL, this );
	m_buttonClickMapToAlign->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnAlignCoordinateToMouse ), NULL, this );
	m_chkAutoAdjustElevation->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnAutoAdjustElevation ), NULL, this );
	m_textCtrlGridElevation->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationSetPropertiesLayout::OnGridChanged ), NULL, this );
	m_tileFieldChoice->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( LocationSetPropertiesLayout::OnTileFieldChoiceChange ), NULL, this );
	m_btnHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationSetPropertiesLayout::OnCancel ), NULL, this );
	
}
