///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "TreePropertiesLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

TreePropertiesLayout::TreePropertiesLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
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
	
	m_symbologyNotebook = new wxNotebook( m_pnlSymbology, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelTreeProp = new wxPanel( m_symbologyNotebook, ID_PANEL_TREE_PROP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_sizerTreeProp;
	m_sizerTreeProp = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerLayoutProperties;
	m_sizerLayoutProperties = new wxStaticBoxSizer( new wxStaticBox( m_panelTreeProp, wxID_ANY, wxT("Tree Layout Properties") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblTreeLayout = new wxStaticText( m_panelTreeProp, ID_LBL_SYMBOL_SHAPE, wxT("Layout style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblTreeLayout->Wrap( -1 );
	fgSizer4->Add( m_lblTreeLayout, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboTreeLayout = new wxComboBox( m_panelTreeProp, ID_CBO_TREE_LAYOUT, wxT("3D slanted cladogram"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_cboTreeLayout->Append( wxT("3D slanted cladogram") );
	m_cboTreeLayout->Append( wxT("3D cladogram") );
	m_cboTreeLayout->Append( wxT("3D slanted phylogram") );
	m_cboTreeLayout->Append( wxT("2D cladogram") );
	m_cboTreeLayout->Append( wxT("2D phylogram") );
	fgSizer4->Add( m_cboTreeLayout, 0, wxALL|wxEXPAND, 5 );
	
	m_lblTreeThickness = new wxStaticText( m_panelTreeProp, ID_LBL_TREE_THICKNESS, wxT("Line thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblTreeThickness->Wrap( -1 );
	fgSizer4->Add( m_lblTreeThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinTreeThickness = new wxSpinCtrl( m_panelTreeProp, ID_SPIN_TREE_THICKNESS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 1 );
	fgSizer4->Add( m_spinTreeThickness, 0, wxALL, 5 );
	
	m_lblTreeHeight = new wxStaticText( m_panelTreeProp, ID_LBL_TREE_THICKNESS, wxT("Relative height :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblTreeHeight->Wrap( -1 );
	fgSizer4->Add( m_lblTreeHeight, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_txtTreeHeight = new wxTextCtrl( m_panelTreeProp, ID_TXT_TREE_HEIGHT, wxT("0.5"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_txtTreeHeight, 0, wxALL, 5 );
	
	m_lbl2dOrientation = new wxStaticText( m_panelTreeProp, ID_LBL_2D_ORIENTATION, wxT("2D orientation:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbl2dOrientation->Wrap( -1 );
	fgSizer4->Add( m_lbl2dOrientation, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cbo2dOrientation = new wxComboBox( m_panelTreeProp, ID_CBO_TREE_LAYOUT, wxT("Horizontal"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_cbo2dOrientation->Append( wxT("Horizontal") );
	m_cbo2dOrientation->Append( wxT("Vertical") );
	fgSizer4->Add( m_cbo2dOrientation, 0, wxALL, 5 );
	
	m_sizerLayoutProperties->Add( fgSizer4, 0, wxEXPAND, 5 );
	
	m_chkOptimizeLeafNodes = new wxCheckBox( m_panelTreeProp, wxID_ANY, wxT("Optimize leaf node order"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkOptimizeLeafNodes->SetValue(true); 
	m_sizerLayoutProperties->Add( m_chkOptimizeLeafNodes, 0, wxALL, 5 );
	
	m_sizerTreeProp->Add( m_sizerLayoutProperties, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerStyleProperties;
	m_sizerStyleProperties = new wxStaticBoxSizer( new wxStaticBox( m_panelTreeProp, wxID_ANY, wxT("Tree Style Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerTreeStyle;
	m_sizerTreeStyle = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText331 = new wxStaticText( m_panelTreeProp, wxID_ANY, wxT("Style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText331->Wrap( -1 );
	m_sizerTreeStyle->Add( m_staticText331, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_cboTreeColourStyle = new wxComboBox( m_panelTreeProp, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	m_cboTreeColourStyle->Append( wxT("Single colour") );
	m_cboTreeColourStyle->Append( wxT("Propogate discrete colours") );
	m_cboTreeColourStyle->Append( wxT("Propogate continuous colours") );
	m_sizerTreeStyle->Add( m_cboTreeColourStyle, 0, wxALL, 5 );
	
	m_sizerStyleProperties->Add( m_sizerTreeStyle, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerSymbolColour;
	m_sizerSymbolColour = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblTreeColour = new wxStaticText( m_panelTreeProp, ID_LBL_TREE_COLOUR, wxT("Default Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblTreeColour->Wrap( -1 );
	m_sizerSymbolColour->Add( m_lblTreeColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_colourTree = new wxColourPickerCtrl( m_panelTreeProp, wxID_ANY, wxColour( 255, 255, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerSymbolColour->Add( m_colourTree, 0, wxALL, 5 );
	
	
	m_sizerSymbolColour->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sizerStyleProperties->Add( m_sizerSymbolColour, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* m_sizerNodeSizes;
	m_sizerNodeSizes = new wxFlexGridSizer( 2, 2, 0, 0 );
	m_sizerNodeSizes->SetFlexibleDirection( wxBOTH );
	m_sizerNodeSizes->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblBranchBorderSize = new wxStaticText( m_panelTreeProp, wxID_ANY, wxT("Branch border thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblBranchBorderSize->Wrap( -1 );
	m_sizerNodeSizes->Add( m_lblBranchBorderSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer431;
	bSizer431 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spinBranchBorderSize = new wxSpinCtrl( m_panelTreeProp, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 6, 0 );
	bSizer431->Add( m_spinBranchBorderSize, 0, wxALL, 5 );
	
	m_colourBranchBorder = new wxColourPickerCtrl( m_panelTreeProp, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer431->Add( m_colourBranchBorder, 0, wxALL, 5 );
	
	m_sizerNodeSizes->Add( bSizer431, 1, wxEXPAND, 5 );
	
	m_lblLeafNodeSize = new wxStaticText( m_panelTreeProp, wxID_ANY, wxT("Leaf node radius:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLeafNodeSize->Wrap( -1 );
	m_sizerNodeSizes->Add( m_lblLeafNodeSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinLeafNodeSize = new wxSpinCtrl( m_panelTreeProp, wxID_ANY, wxT("5"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	m_sizerNodeSizes->Add( m_spinLeafNodeSize, 0, wxALL, 5 );
	
	m_staticText30 = new wxStaticText( m_panelTreeProp, wxID_ANY, wxT("Leaf node border thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	m_sizerNodeSizes->Add( m_staticText30, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spinLeafNodeBorderSize = new wxSpinCtrl( m_panelTreeProp, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 6, 0 );
	bSizer41->Add( m_spinLeafNodeBorderSize, 0, wxALL, 5 );
	
	m_colourLeafNodeBorder = new wxColourPickerCtrl( m_panelTreeProp, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer41->Add( m_colourLeafNodeBorder, 0, wxALL, 5 );
	
	m_sizerNodeSizes->Add( bSizer41, 0, wxEXPAND, 5 );
	
	m_lblInternalNodeSize = new wxStaticText( m_panelTreeProp, wxID_ANY, wxT("Internal node radius:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblInternalNodeSize->Wrap( -1 );
	m_sizerNodeSizes->Add( m_lblInternalNodeSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_spinInternalNodeSize = new wxSpinCtrl( m_panelTreeProp, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	m_sizerNodeSizes->Add( m_spinInternalNodeSize, 0, wxALL, 5 );
	
	m_staticText31 = new wxStaticText( m_panelTreeProp, wxID_ANY, wxT("Internal node border thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	m_sizerNodeSizes->Add( m_staticText31, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* m_sizerNodeStyle;
	m_sizerNodeStyle = new wxBoxSizer( wxHORIZONTAL );
	
	m_spinInternalNodeBorderSize = new wxSpinCtrl( m_panelTreeProp, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 6, 0 );
	m_sizerNodeStyle->Add( m_spinInternalNodeBorderSize, 0, wxALL, 5 );
	
	m_colourInternalNodeBorder = new wxColourPickerCtrl( m_panelTreeProp, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerNodeStyle->Add( m_colourInternalNodeBorder, 0, wxALL, 5 );
	
	m_sizerNodeSizes->Add( m_sizerNodeStyle, 0, wxEXPAND, 5 );
	
	m_chkDrawLayoutPrimative = new wxCheckBox( m_panelTreeProp, ID_CHECK_DRAW_LAYOUT_PRIMATIVE, wxT("Draw layout primitive"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerNodeSizes->Add( m_chkDrawLayoutPrimative, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_chkDrawGeoAxis = new wxCheckBox( m_panelTreeProp, ID_CHECK_DRAW_GEO_AXIS, wxT("Draw geographic axis"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerNodeSizes->Add( m_chkDrawGeoAxis, 0, wxALL, 5 );
	
	m_sizerStyleProperties->Add( m_sizerNodeSizes, 0, wxEXPAND, 5 );
	
	m_sizerTreeProp->Add( m_sizerStyleProperties, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerShowTree;
	m_sizerShowTree = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_sizerShowTree->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sizerTreeProp->Add( m_sizerShowTree, 1, wxEXPAND, 5 );
	
	m_panelTreeProp->SetSizer( m_sizerTreeProp );
	m_panelTreeProp->Layout();
	m_sizerTreeProp->Fit( m_panelTreeProp );
	m_symbologyNotebook->AddPage( m_panelTreeProp, wxT("Tree"), true );
	m_pnlConnectingLineProp = new wxPanel( m_symbologyNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_sizerLineProp;
	m_sizerLineProp = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerLocationLineProperties;
	m_sizerLocationLineProperties = new wxStaticBoxSizer( new wxStaticBox( m_pnlConnectingLineProp, wxID_ANY, wxT("Location Line Properties") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblLocationLineStyle = new wxStaticText( m_pnlConnectingLineProp, ID_LBL_LOCATION_LINE_STYLE, wxT("Line style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLocationLineStyle->Wrap( -1 );
	fgSizer5->Add( m_lblLocationLineStyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboLocationLineStyle = new wxComboBox( m_pnlConnectingLineProp, ID_CBO_LOCATION_LINE_STYLE, wxT("Solid"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	m_cboLocationLineStyle->Append( wxT("Solid") );
	m_cboLocationLineStyle->Append( wxT("Short dash") );
	m_cboLocationLineStyle->Append( wxT("Long dash") );
	m_cboLocationLineStyle->Append( wxT("Hidden") );
	fgSizer5->Add( m_cboLocationLineStyle, 0, wxALL|wxEXPAND, 5 );
	
	m_lblLocationLineThickness = new wxStaticText( m_pnlConnectingLineProp, ID_LBL_LOCATION_LINE_THICKNESS, wxT("Line thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLocationLineThickness->Wrap( -1 );
	fgSizer5->Add( m_lblLocationLineThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer441;
	bSizer441 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spinLocationLineThickness = new wxSpinCtrl( m_pnlConnectingLineProp, ID_SPIN_LOCATION_LINE_THICKNESS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 10 );
	bSizer441->Add( m_spinLocationLineThickness, 0, wxALL, 5 );
	
	fgSizer5->Add( bSizer441, 1, wxEXPAND, 5 );
	
	m_staticText35 = new wxStaticText( m_pnlConnectingLineProp, wxID_ANY, wxT("Border size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	fgSizer5->Add( m_staticText35, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer451;
	bSizer451 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spinLocationLineBorderSize = new wxSpinCtrl( m_pnlConnectingLineProp, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	bSizer451->Add( m_spinLocationLineBorderSize, 0, wxALL, 5 );
	
	m_colourLocationLineBorder = new wxColourPickerCtrl( m_pnlConnectingLineProp, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer451->Add( m_colourLocationLineBorder, 0, wxALL, 5 );
	
	fgSizer5->Add( bSizer451, 1, wxEXPAND, 5 );
	
	m_sizerLocationLineProperties->Add( fgSizer5, 0, wxEXPAND, 5 );
	
	m_sizerLineProp->Add( m_sizerLocationLineProperties, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerCorrelationLineProperties;
	m_sizerCorrelationLineProperties = new wxStaticBoxSizer( new wxStaticBox( m_pnlConnectingLineProp, wxID_ANY, wxT("Correlation Line Properties") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblCorrelationLineStyle = new wxStaticText( m_pnlConnectingLineProp, ID_LBL_CORRELATION_LINE_STYLE, wxT("Line style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblCorrelationLineStyle->Wrap( -1 );
	fgSizer6->Add( m_lblCorrelationLineStyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboCorrelationLineStyle = new wxComboBox( m_pnlConnectingLineProp, ID_CBO_CORRELATION_LINE_STYLE, wxT("Solid"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	m_cboCorrelationLineStyle->Append( wxT("Solid") );
	m_cboCorrelationLineStyle->Append( wxT("Short dash") );
	m_cboCorrelationLineStyle->Append( wxT("Long dash") );
	m_cboCorrelationLineStyle->Append( wxT("Hidden") );
	fgSizer6->Add( m_cboCorrelationLineStyle, 0, wxALL|wxEXPAND, 5 );
	
	m_lblCorrelationLineThickness = new wxStaticText( m_pnlConnectingLineProp, ID_LBL_CORRELATIN_LINE_THICKNESS, wxT("Line thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblCorrelationLineThickness->Wrap( -1 );
	fgSizer6->Add( m_lblCorrelationLineThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer46;
	bSizer46 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spinCorrelationLineThickness = new wxSpinCtrl( m_pnlConnectingLineProp, ID_SPIN_CORRELATION_LINE_THICKNESS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 10 );
	bSizer46->Add( m_spinCorrelationLineThickness, 0, wxALL, 5 );
	
	fgSizer6->Add( bSizer46, 1, wxEXPAND, 5 );
	
	m_lblCorrelationLineBorderSize = new wxStaticText( m_pnlConnectingLineProp, wxID_ANY, wxT("Border size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblCorrelationLineBorderSize->Wrap( -1 );
	fgSizer6->Add( m_lblCorrelationLineBorderSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer47;
	bSizer47 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spinCorrelationLineBorderSize = new wxSpinCtrl( m_pnlConnectingLineProp, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	bSizer47->Add( m_spinCorrelationLineBorderSize, 0, wxALL, 5 );
	
	m_colourCorrelationLineBorder = new wxColourPickerCtrl( m_pnlConnectingLineProp, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer47->Add( m_colourCorrelationLineBorder, 0, wxALL, 5 );
	
	fgSizer6->Add( bSizer47, 1, wxEXPAND, 5 );
	
	m_sizerCorrelationLineProperties->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	m_sizerLineProp->Add( m_sizerCorrelationLineProperties, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerDroplines;
	m_sizerDroplines = new wxStaticBoxSizer( new wxStaticBox( m_pnlConnectingLineProp, wxID_ANY, wxT("3D Drop Line Properties") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblDropLineThickness = new wxStaticText( m_pnlConnectingLineProp, wxID_ANY, wxT("Internal drop line thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDropLineThickness->Wrap( -1 );
	fgSizer7->Add( m_lblDropLineThickness, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_spinInternalDropLineThickness = new wxSpinCtrl( m_pnlConnectingLineProp, ID_SPIN_DROP_LINE_THICKNESS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer7->Add( m_spinInternalDropLineThickness, 0, wxALL, 5 );
	
	m_lblDropLineStyle = new wxStaticText( m_pnlConnectingLineProp, wxID_ANY, wxT("Internal drop line style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDropLineStyle->Wrap( -1 );
	fgSizer7->Add( m_lblDropLineStyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer48;
	bSizer48 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cboInternalDropLineStyle = new wxComboBox( m_pnlConnectingLineProp, ID_CBO_DROP_LINE_STYLE, wxT("Short dash"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	m_cboInternalDropLineStyle->Append( wxT("Solid") );
	m_cboInternalDropLineStyle->Append( wxT("Short dash") );
	m_cboInternalDropLineStyle->Append( wxT("Long dash") );
	m_cboInternalDropLineStyle->Append( wxT("Hidden") );
	bSizer48->Add( m_cboInternalDropLineStyle, 0, wxALL|wxEXPAND, 5 );
	
	m_colourInternalDropLine = new wxColourPickerCtrl( m_pnlConnectingLineProp, ID_COLOUR_PICKER_CORRELATION_LINE, wxColour( 128, 128, 128 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer48->Add( m_colourInternalDropLine, 0, wxALL|wxEXPAND, 5 );
	
	fgSizer7->Add( bSizer48, 1, wxEXPAND, 5 );
	
	m_staticText32 = new wxStaticText( m_pnlConnectingLineProp, wxID_ANY, wxT("Leaf drop line style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	fgSizer7->Add( m_staticText32, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cboLeafDropLineStyle = new wxComboBox( m_pnlConnectingLineProp, wxID_ANY, wxT("Short dash"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	m_cboLeafDropLineStyle->Append( wxT("Solid") );
	m_cboLeafDropLineStyle->Append( wxT("Short dash") );
	m_cboLeafDropLineStyle->Append( wxT("Long dash") );
	m_cboLeafDropLineStyle->Append( wxT("Hidden") );
	bSizer35->Add( m_cboLeafDropLineStyle, 0, wxALL, 5 );
	
	fgSizer7->Add( bSizer35, 1, wxEXPAND, 5 );
	
	m_sizerDroplines->Add( fgSizer7, 1, wxEXPAND, 5 );
	
	m_sizerLineProp->Add( m_sizerDroplines, 0, wxALL|wxEXPAND, 5 );
	
	m_pnlConnectingLineProp->SetSizer( m_sizerLineProp );
	m_pnlConnectingLineProp->Layout();
	m_sizerLineProp->Fit( m_pnlConnectingLineProp );
	m_symbologyNotebook->AddPage( m_pnlConnectingLineProp, wxT("Connecting Lines"), false );
	m_pnlGeographyLineProp = new wxPanel( m_symbologyNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_sizerGeoLinePtProp;
	m_sizerGeoLinePtProp = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerGeoLineProp;
	m_sizerGeoLineProp = new wxStaticBoxSizer( new wxStaticBox( m_pnlGeographyLineProp, wxID_ANY, wxT("Geography Line Properties") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblGeoLineOffset = new wxStaticText( m_pnlGeographyLineProp, ID_LBL_GEOGRAPHY_LINE_OFFSET, wxT("Line offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblGeoLineOffset->Wrap( -1 );
	fgSizer2->Add( m_lblGeoLineOffset, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinGeoLineOffset = new wxSpinCtrl( m_pnlGeographyLineProp, ID_SPIN_CORRELATION_LINE_THICKNESS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 200, 50 );
	fgSizer2->Add( m_spinGeoLineOffset, 0, wxALL, 5 );
	
	m_lblGeoLineStyle = new wxStaticText( m_pnlGeographyLineProp, ID_LBL_GEOGRAPHY_LINE_STYLE, wxT("Line style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblGeoLineStyle->Wrap( -1 );
	fgSizer2->Add( m_lblGeoLineStyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboGeoLineStyle = new wxComboBox( m_pnlGeographyLineProp, ID_CBO_GEOGRAPHY_LINE_STYLE, wxT("Solid"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT );
	m_cboGeoLineStyle->Append( wxT("Solid") );
	m_cboGeoLineStyle->Append( wxT("Short dash") );
	m_cboGeoLineStyle->Append( wxT("Long dash") );
	m_cboGeoLineStyle->Append( wxT("Hidden") );
	fgSizer2->Add( m_cboGeoLineStyle, 0, wxALL|wxEXPAND, 5 );
	
	m_lblGeoLineThickness = new wxStaticText( m_pnlGeographyLineProp, ID_LBL_CORRELATIN_LINE_THICKNESS, wxT("Line thickness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblGeoLineThickness->Wrap( -1 );
	fgSizer2->Add( m_lblGeoLineThickness, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer49;
	bSizer49 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spinGeoLineThickness = new wxSpinCtrl( m_pnlGeographyLineProp, ID_SPIN_CORRELATION_LINE_THICKNESS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 10 );
	bSizer49->Add( m_spinGeoLineThickness, 0, wxALL, 5 );
	
	m_colourGeoLine = new wxColourPickerCtrl( m_pnlGeographyLineProp, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer49->Add( m_colourGeoLine, 0, wxALL, 5 );
	
	fgSizer2->Add( bSizer49, 1, wxEXPAND, 5 );
	
	m_sizerGeoLineProp->Add( fgSizer2, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerGeoLineColour;
	m_sizerGeoLineColour = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_sizerGeoLineColour->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sizerGeoLineProp->Add( m_sizerGeoLineColour, 1, wxEXPAND, 5 );
	
	m_sizerGeoLinePtProp->Add( m_sizerGeoLineProp, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* m_sizerGeoPointProperties;
	m_sizerGeoPointProperties = new wxStaticBoxSizer( new wxStaticBox( m_pnlGeographyLineProp, wxID_ANY, wxT("Geographic Point Properties") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lbGeoPointSize = new wxStaticText( m_pnlGeographyLineProp, ID_LBL_CORRELATIN_LINE_THICKNESS, wxT("Point size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbGeoPointSize->Wrap( -1 );
	fgSizer3->Add( m_lbGeoPointSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer50;
	bSizer50 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spinGeoPointSize = new wxSpinCtrl( m_pnlGeographyLineProp, ID_SPIN_GEO_POINT_SIZE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 12 );
	bSizer50->Add( m_spinGeoPointSize, 0, wxALL, 5 );
	
	fgSizer3->Add( bSizer50, 1, wxEXPAND, 5 );
	
	m_staticText33 = new wxStaticText( m_pnlGeographyLineProp, wxID_ANY, wxT("Border size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33->Wrap( -1 );
	fgSizer3->Add( m_staticText33, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spinGeoPointBorderSize = new wxSpinCtrl( m_pnlGeographyLineProp, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	bSizer43->Add( m_spinGeoPointBorderSize, 0, wxALL, 5 );
	
	m_colourGeoPointBorder = new wxColourPickerCtrl( m_pnlGeographyLineProp, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer43->Add( m_colourGeoPointBorder, 0, wxALL, 5 );
	
	fgSizer3->Add( bSizer43, 1, wxEXPAND, 5 );
	
	m_sizerGeoPointProperties->Add( fgSizer3, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerGeoPointDraw;
	m_sizerGeoPointDraw = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_sizerGeoPointDraw->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_chkEvenlySpeadGeoPts = new wxCheckBox( m_pnlGeographyLineProp, ID_CHECK_DRAW_CORRELATION_LINES, wxT("Evenly spread geographic points"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerGeoPointDraw->Add( m_chkEvenlySpeadGeoPts, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sizerGeoPointProperties->Add( m_sizerGeoPointDraw, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer45->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_chkDrawGeoPoint = new wxCheckBox( m_pnlGeographyLineProp, ID_CHECK_DRAW_CORRELATION_LINES, wxT("Draw geographic points"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer45->Add( m_chkDrawGeoPoint, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sizerGeoPointProperties->Add( bSizer45, 1, wxEXPAND, 5 );
	
	m_sizerGeoLinePtProp->Add( m_sizerGeoPointProperties, 0, wxALL|wxEXPAND, 5 );
	
	m_pnlGeographyLineProp->SetSizer( m_sizerGeoLinePtProp );
	m_pnlGeographyLineProp->Layout();
	m_sizerGeoLinePtProp->Fit( m_pnlGeographyLineProp );
	m_symbologyNotebook->AddPage( m_pnlGeographyLineProp, wxT("Geography Line"), false );
	
	m_sizerSymbology->Add( m_symbologyNotebook, 1, wxEXPAND | wxALL, 5 );
	
	m_pnlSymbology->SetSizer( m_sizerSymbology );
	m_pnlSymbology->Layout();
	m_sizerSymbology->Fit( m_pnlSymbology );
	m_notebook->AddPage( m_pnlSymbology, wxT("Symbology"), false );
	m_pnlLabel = new wxPanel( m_notebook, ID_PNL_LABEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_sizerLabelProperties;
	m_sizerLabelProperties = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* m_sizerLabel;
	m_sizerLabel = new wxStaticBoxSizer( new wxStaticBox( m_pnlLabel, wxID_ANY, wxT("Label Properties") ), wxVERTICAL );
	
	wxBoxSizer* m_sizerLabelSize;
	m_sizerLabelSize = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblLabelSize = new wxStaticText( m_pnlLabel, ID_LBL_LABEL_SIZE, wxT("Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLabelSize->Wrap( -1 );
	m_sizerLabelSize->Add( m_lblLabelSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinLabelSize = new wxSpinCtrl( m_pnlLabel, ID_SPIN_LABEL_SIZE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 10 );
	m_sizerLabelSize->Add( m_spinLabelSize, 0, wxALL, 5 );
	
	m_sizerLabel->Add( m_sizerLabelSize, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerLabelColour;
	m_sizerLabelColour = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblLabelColour = new wxStaticText( m_pnlLabel, ID_LBL_LABEL_COLOUR, wxT("Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLabelColour->Wrap( -1 );
	m_sizerLabelColour->Add( m_lblLabelColour, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_colourLabel = new wxColourPickerCtrl( m_pnlLabel, ID_COLOUR_LABEL, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_sizerLabelColour->Add( m_colourLabel, 0, wxALL, 5 );
	
	
	m_sizerLabelColour->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_chkShowLabels = new wxCheckBox( m_pnlLabel, ID_CHECK_SHOW_LABELS, wxT("Show labels"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerLabelColour->Add( m_chkShowLabels, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sizerLabel->Add( m_sizerLabelColour, 1, wxEXPAND, 5 );
	
	m_sizerLabelProperties->Add( m_sizerLabel, 0, wxALL|wxEXPAND, 5 );
	
	m_pnlLabel->SetSizer( m_sizerLabelProperties );
	m_pnlLabel->Layout();
	m_sizerLabelProperties->Fit( m_pnlLabel );
	m_notebook->AddPage( m_pnlLabel, wxT("Label"), false );
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
	m_notebook->AddPage( m_pnlMetadata, wxT("Metadata"), true );
	
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
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( TreePropertiesLayout::OnClose ) );
	m_cboTreeLayout->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( TreePropertiesLayout::OnLayoutStyleChange ), NULL, this );
	m_btnHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TreePropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TreePropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TreePropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TreePropertiesLayout::OnCancel ), NULL, this );
}

TreePropertiesLayout::~TreePropertiesLayout()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( TreePropertiesLayout::OnClose ) );
	m_cboTreeLayout->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( TreePropertiesLayout::OnLayoutStyleChange ), NULL, this );
	m_btnHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TreePropertiesLayout::OnHelp ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TreePropertiesLayout::OnOK ), NULL, this );
	m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TreePropertiesLayout::OnApply ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TreePropertiesLayout::OnCancel ), NULL, this );
	
}
