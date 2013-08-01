///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "VectorMapPropertiesLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

VectorMapPropertiesLayout::VectorMapPropertiesLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	wxBoxSizer* m_vecmapPropSizer;
	m_vecmapPropSizer = new wxBoxSizer( wxVERTICAL );
	
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
	
	m_lblDescription = new wxStaticText( m_pnlGeneral, ID_LBL_DESCRIPTION, wxT("Description:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblDescription->Wrap( -1 );
	m_sizerDescription->Add( m_lblDescription, 0, wxALL, 5 );
	
	m_txtLayerDescription = new wxTextCtrl( m_pnlGeneral, ID_TXT_LAYER_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_txtLayerDescription->SetMinSize( wxSize( 200,-1 ) );
	
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
	m_notebook->AddPage( m_pnlGeneral, wxT("General"), false );
	m_pnlSymbology = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_sizerSetting;
	m_sizerSetting = new wxBoxSizer( wxVERTICAL );
	
	m_symbology_Notebook = new wxNotebook( m_pnlSymbology, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_pnlPoint = new wxPanel( m_symbology_Notebook, ID_PNL_POINT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_SizerSymbolPointProperties1;
	m_SizerSymbolPointProperties1 = new wxStaticBoxSizer( new wxStaticBox( m_pnlPoint, ID_Symbol_POINT_PROPERTIES, wxT("Symbol Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerColour11;
	m_sizerColour11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPointShape = new wxStaticText( m_pnlPoint, wxID_ANY, wxT("Shape"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_lblPointShape->Wrap( -1 );
	m_sizerColour11->Add( m_lblPointShape, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_CboPointShape = new wxComboBox( m_pnlPoint, ID_CBO_POINT_SHAPE, wxT("Circle (fast)"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_CboPointShape->Append( wxT("Circle") );
	m_CboPointShape->Append( wxT("Circle (fast)") );
	m_CboPointShape->Append( wxT("Square") );
	m_CboPointShape->Append( wxT("Triangle") );
	m_CboPointShape->Append( wxT("Star") );
	m_CboPointShape->Append( wxT("Plus sign") );
	m_CboPointShape->Append( wxT("Octagon") );
	m_CboPointShape->Append( wxT("Diamond") );
	m_CboPointShape->Append( wxT("Inverted triangle") );
	m_sizerColour11->Add( m_CboPointShape, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_SizerSymbolPointProperties1->Add( m_sizerColour11, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerPointSize1;
	m_sizerPointSize1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPointSize = new wxStaticText( m_pnlPoint, ID_LBL_POINT_SIZE, wxT("Size"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_lblPointSize->Wrap( -1 );
	m_sizerPointSize1->Add( m_lblPointSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_TxtCtrlPointSize = new wxTextCtrl( m_pnlPoint, ID_TXT_CTRL_POINT_SIZE, wxT("1.0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerPointSize1->Add( m_TxtCtrlPointSize, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_SizerSymbolPointProperties1->Add( m_sizerPointSize1, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerColour1;
	m_sizerColour1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPointColour = new wxStaticText( m_pnlPoint, ID_LBL_POINT_COLOUR, wxT("Fill Colour"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_lblPointColour->Wrap( -1 );
	m_sizerColour1->Add( m_lblPointColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_FillPointColourPicker = new wxColourPickerCtrl( m_pnlPoint, ID_POINT_FILL_COLOUR_PICKER, wxColour( 0, 128, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerColour1->Add( m_FillPointColourPicker, 0, wxALL|wxEXPAND, 5 );
	
	m_SizerSymbolPointProperties1->Add( m_sizerColour1, 0, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( m_pnlPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_SizerSymbolPointProperties1->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* m_sizerColour12;
	m_sizerColour12 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPointBorderColour = new wxStaticText( m_pnlPoint, ID_LBL_POINT_BORDER_COLOUR, wxT("Border Colour"), wxDefaultPosition, wxSize( 80,-1 ), 0 );
	m_lblPointBorderColour->Wrap( -1 );
	m_sizerColour12->Add( m_lblPointBorderColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_PointBorderColourPicker = new wxColourPickerCtrl( m_pnlPoint, ID_POINT_BORDER_COLOUR_PICKER, wxColour( 0, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerColour12->Add( m_PointBorderColourPicker, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_SizerSymbolPointProperties1->Add( m_sizerColour12, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerPointSize11;
	m_sizerPointSize11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPointBorderThickness = new wxStaticText( m_pnlPoint, ID_LBL_POINT_BORDER_THICKNESS, wxT("Border Thickness"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblPointBorderThickness->Wrap( -1 );
	m_sizerPointSize11->Add( m_lblPointBorderThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_TxtCtrlPointBorderThickness = new wxTextCtrl( m_pnlPoint, ID_TXT_CTRL_POINT_BORDER_THICKNESS, wxT("0.0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerPointSize11->Add( m_TxtCtrlPointBorderThickness, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_SizerSymbolPointProperties1->Add( m_sizerPointSize11, 1, wxEXPAND, 5 );
	
	bSizer15->Add( m_SizerSymbolPointProperties1, 1, wxALL|wxEXPAND, 5 );
	
	m_pnlPoint->SetSizer( bSizer15 );
	m_pnlPoint->Layout();
	bSizer15->Fit( m_pnlPoint );
	m_symbology_Notebook->AddPage( m_pnlPoint, wxT("Point"), true );
	m_pnlLine = new wxPanel( m_symbology_Notebook, ID_PNL_LINE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_SizerSymbolPolylineProperties;
	m_SizerSymbolPolylineProperties = new wxStaticBoxSizer( new wxStaticBox( m_pnlLine, ID_Symbol_POLYLINE_PROPERTIES, wxT("Symbol Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerPointSize21;
	m_sizerPointSize21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPolylineStyle = new wxStaticText( m_pnlLine, ID_LBL_POLYLINE_STYLE, wxT("Line Style"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblPolylineStyle->Wrap( -1 );
	m_sizerPointSize21->Add( m_lblPolylineStyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_CboPolylineStyle = new wxComboBox( m_pnlLine, ID_CBO_POLYLINE_STYLE, wxT("Solid"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_CboPolylineStyle->Append( wxT("Hidden") );
	m_CboPolylineStyle->Append( wxT("Solid") );
	m_CboPolylineStyle->Append( wxT("Short dash") );
	m_CboPolylineStyle->Append( wxT("Long dash") );
	m_sizerPointSize21->Add( m_CboPolylineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_SizerSymbolPolylineProperties->Add( m_sizerPointSize21, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerPointSize2;
	m_sizerPointSize2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPolylineThickness = new wxStaticText( m_pnlLine, ID_LBL_POLYLINE_THICKNESS, wxT("Thickness"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblPolylineThickness->Wrap( -1 );
	m_sizerPointSize2->Add( m_lblPolylineThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_TxtCtrlPolylineThickness = new wxTextCtrl( m_pnlLine, ID_TXT_CTRL_POLYLINE_THICKNESS, wxT("1.0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerPointSize2->Add( m_TxtCtrlPolylineThickness, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_SizerSymbolPolylineProperties->Add( m_sizerPointSize2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerColour2;
	m_sizerColour2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPolylineColour = new wxStaticText( m_pnlLine, ID_LBL_POLYLINE_COLOUR, wxT("Colour"), wxDefaultPosition, wxSize( 45,-1 ), 0 );
	m_lblPolylineColour->Wrap( -1 );
	m_sizerColour2->Add( m_lblPolylineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_PolylineFillcolourPicker = new wxColourPickerCtrl( m_pnlLine, ID_POLYLINE_FILL_COLOUR_PICKER, wxColour( 0, 0, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerColour2->Add( m_PolylineFillcolourPicker, 0, wxALL|wxEXPAND, 5 );
	
	m_SizerSymbolPolylineProperties->Add( m_sizerColour2, 0, wxEXPAND, 5 );
	
	m_staticline21 = new wxStaticLine( m_pnlLine, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_SizerSymbolPolylineProperties->Add( m_staticline21, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* m_sizerColour21;
	m_sizerColour21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPolylineBorderColour = new wxStaticText( m_pnlLine, ID_LBL_POLYLINE_BORDER_COLOUR, wxT("Border Colour"), wxDefaultPosition, wxSize( 80,-1 ), 0 );
	m_lblPolylineBorderColour->Wrap( -1 );
	m_sizerColour21->Add( m_lblPolylineBorderColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_PolylineBordercolourPicker = new wxColourPickerCtrl( m_pnlLine, ID_POLYLINE_BORDER_COLOUR_PICKER, wxColour( 0, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerColour21->Add( m_PolylineBordercolourPicker, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_SizerSymbolPolylineProperties->Add( m_sizerColour21, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerPointSize22;
	m_sizerPointSize22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPolylineBorderThickness = new wxStaticText( m_pnlLine, ID_LBL_POLYLINE_BORDER_THICKNESS, wxT("Border Thickness"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblPolylineBorderThickness->Wrap( -1 );
	m_sizerPointSize22->Add( m_lblPolylineBorderThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_TxtCtrlPolylineBorderThickness = new wxTextCtrl( m_pnlLine, ID_TXT_CTRL_POLYLINE_BORDER_THICKNESS, wxT("0.0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerPointSize22->Add( m_TxtCtrlPolylineBorderThickness, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_SizerSymbolPolylineProperties->Add( m_sizerPointSize22, 1, wxEXPAND, 5 );
	
	bSizer16->Add( m_SizerSymbolPolylineProperties, 1, wxALL|wxEXPAND, 5 );
	
	m_pnlLine->SetSizer( bSizer16 );
	m_pnlLine->Layout();
	bSizer16->Fit( m_pnlLine );
	m_symbology_Notebook->AddPage( m_pnlLine, wxT("PolyLine"), false );
	m_pnlPolygon = new wxPanel( m_symbology_Notebook, ID_PNL_POLYGON, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_SizerSymbolPolygonProperties3;
	m_SizerSymbolPolygonProperties3 = new wxStaticBoxSizer( new wxStaticBox( m_pnlPolygon, ID_Symbol_POLYGON_PROPERTIES, wxT("Symbol Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerPointSize211;
	m_sizerPointSize211 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPolygonStyle = new wxStaticText( m_pnlPolygon, ID_LBL_POLYGON_STYLE, wxT("Line Style"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblPolygonStyle->Wrap( -1 );
	m_sizerPointSize211->Add( m_lblPolygonStyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_CboPolygonStyle = new wxComboBox( m_pnlPolygon, ID_CBO_POLYGON_STYLE, wxT("Solid"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_CboPolygonStyle->Append( wxT("Hidden") );
	m_CboPolygonStyle->Append( wxT("Solid") );
	m_CboPolygonStyle->Append( wxT("Short dash") );
	m_CboPolygonStyle->Append( wxT("Long dash") );
	m_sizerPointSize211->Add( m_CboPolygonStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_SizerSymbolPolygonProperties3->Add( m_sizerPointSize211, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerPointSize3;
	m_sizerPointSize3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPolygonThickness = new wxStaticText( m_pnlPolygon, ID_LBL_POLYGON_Thickness, wxT("Thickness"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lblPolygonThickness->Wrap( -1 );
	m_sizerPointSize3->Add( m_lblPolygonThickness, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_TxtCtrlPolygonThickness = new wxTextCtrl( m_pnlPolygon, ID_TXT_CTRL_POLYGON_Thickness, wxT("1.0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerPointSize3->Add( m_TxtCtrlPolygonThickness, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_SizerSymbolPolygonProperties3->Add( m_sizerPointSize3, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerBorderColour3;
	m_sizerBorderColour3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblPolygonColour = new wxStaticText( m_pnlPolygon, ID_LBL_POLYGON_COLOUR, wxT("Colour"), wxDefaultPosition, wxSize( 45,-1 ), 0 );
	m_lblPolygonColour->Wrap( -1 );
	m_sizerBorderColour3->Add( m_lblPolygonColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_PolygonBordercolourPicker = new wxColourPickerCtrl( m_pnlPolygon, ID_POLYGON_BORDER_COLOUR_PICKER, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerBorderColour3->Add( m_PolygonBordercolourPicker, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_SizerSymbolPolygonProperties3->Add( m_sizerBorderColour3, 1, wxEXPAND, 5 );
	
	bSizer17->Add( m_SizerSymbolPolygonProperties3, 1, wxALL|wxEXPAND, 5 );
	
	m_pnlPolygon->SetSizer( bSizer17 );
	m_pnlPolygon->Layout();
	bSizer17->Fit( m_pnlPolygon );
	m_symbology_Notebook->AddPage( m_pnlPolygon, wxT("Polygon"), false );
	
	m_sizerSetting->Add( m_symbology_Notebook, 1, wxEXPAND | wxALL, 5 );
	
	m_pnlSymbology->SetSizer( m_sizerSetting );
	m_pnlSymbology->Layout();
	m_sizerSetting->Fit( m_pnlSymbology );
	m_notebook->AddPage( m_pnlSymbology, wxT("Symbology"), true );
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
	m_txtMetaData->SetMinSize( wxSize( -1,200 ) );
	
	m_sizerSourceData->Add( m_txtMetaData, 1, wxEXPAND | wxALL, 5 );
	
	m_sizerMetadata->Add( m_sizerSourceData, 1, wxALL|wxEXPAND, 5 );
	
	m_pnlMetadata->SetSizer( m_sizerMetadata );
	m_pnlMetadata->Layout();
	m_sizerMetadata->Fit( m_pnlMetadata );
	m_notebook->AddPage( m_pnlMetadata, wxT("Metadata"), false );
	
	m_vecmapPropSizer->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );
	
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
	
	m_vecmapPropSizer->Add( m_sizerButtons, 0, wxEXPAND, 5 );
	
	this->SetSizer( m_vecmapPropSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( VectorMapPropertiesLayout::OnClose ) );
	m_txtLayerName->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( VectorMapPropertiesLayout::findme ), NULL, this );
	m_btnHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VectorMapPropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VectorMapPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VectorMapPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VectorMapPropertiesLayout::OnCancel ), NULL, this );
}

VectorMapPropertiesLayout::~VectorMapPropertiesLayout()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( VectorMapPropertiesLayout::OnClose ) );
	m_txtLayerName->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( VectorMapPropertiesLayout::findme ), NULL, this );
	m_btnHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VectorMapPropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VectorMapPropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VectorMapPropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VectorMapPropertiesLayout::OnCancel ), NULL, this );
	
}
