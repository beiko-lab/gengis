//=======================================================================
// Author: Donovan Parks
//
// Copyright 2009 Donovan Parks
//
// This file is part of GenGIS.
//
// GenGIS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GenGIS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GenGIS.  If not, see <http://www.gnu.org/licenses/>.
//=======================================================================

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"
#include "../core/GeoTreeView.hpp"
#include "../core/TreeLayer.hpp"
#include "../core/Viewport.hpp"
#include "../core/LayerTreeController.hpp"

#include "../gui/TreePropertiesDlg.hpp"

#include "../utils/StringTools.hpp"

#include "../widgets/CustomColourButton.hpp"

using namespace GenGIS;

TreePropertiesDlg::TreePropertiesDlg(wxWindow* parent, TreeLayerPtr treeLayer)
: TreePropertiesLayout(parent), m_treeLayer(treeLayer)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	// Limit the properties dialog to a single instance
	m_treeLayer->SetPropertiesDialog( this );

	Init();
	Fit();
}

TreePropertiesDlg::TreePropertiesDlg(wxWindow* parent, TreeLayerPtr treeLayer, LayoutLinePtr layoutLine)
: TreePropertiesLayout(parent), m_treeLayer(treeLayer)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	// Limit the properties dialog to a single instance
	m_treeLayer->SetPropertiesDialog( this );
	
	GeoTreeViewPtr geoTreeView = m_treeLayer->GetGeoTreeView();
	geoTreeView->SetLayoutLine(layoutLine);

	Init();
	Fit();
}

TreePropertiesDlg::~TreePropertiesDlg()
{
	// Tell the layer that the properties dialog is being closed
	m_treeLayer->SetPropertiesDialogOpenStatus( false );
	m_treeLayer->SetPropertiesDialog( NULL );
}

void TreePropertiesDlg::Init()
{
	// set the title of the properties dialog
	this->SetLabel( wxString(m_treeLayer->GetName().c_str()) + wxT( " : Tree Properties" ) );

	// *** Set state of controls on General page
	m_txtLayerName->SetValue(wxString(m_treeLayer->GetName().c_str()));
	m_txtLayerDescription->SetValue(wxString(m_treeLayer->GetDescription().c_str()));
	m_txtAuthours->SetValue(wxString(m_treeLayer->GetAuthours().c_str()));

	// *** Set state of controls on Symbology page
	GeoTreeViewPtr geoTreeView = m_treeLayer->GetGeoTreeView();

	// tree layout properties
	if(geoTreeView->GetLayout() == GeoTreeView::SLANTED_CLADOGRAM_3D)
		m_cboTreeLayout->SetValue(_T("3D slanted cladogram"));
	else if(geoTreeView->GetLayout() == GeoTreeView::CLADOGRAM_3D)
		m_cboTreeLayout->SetValue(_T("3D cladogram"));
	else if(geoTreeView->GetLayout() == GeoTreeView::SLANTED_PHYLOGRAM_3D)
		m_cboTreeLayout->SetValue(_T("3D slanted phylogram"));
	else if(geoTreeView->GetLayout() == GeoTreeView::CLADOGRAM_2D)
		m_cboTreeLayout->SetValue(_T("2D cladogram"));
	else if(geoTreeView->GetLayout() == GeoTreeView::PHYLOGRAM_2D)
		m_cboTreeLayout->SetValue(_T("2D phylogram"));
	else if(geoTreeView->GetLayout() == GeoTreeView::ELLIPSE_QUADRANT)
		m_cboTreeLayout->SetValue(_T("Elliptical Layout"));

	m_spinTreeThickness->SetValue(geoTreeView->GetLineThickness());
	m_txtTreeHeight->SetValue(wxString::Format(wxT("%f"), geoTreeView->GetHeight()));

	if(geoTreeView->GetOrientation() == GeoTreeView::VERTICAL)
		m_cbo2dOrientation->SetValue(_T("Vertical"));
	else if(geoTreeView->GetOrientation() == GeoTreeView::HORIZONTAL)
		m_cbo2dOrientation->SetValue(_T("Horizontal"));

	m_chkDrawLayoutPrimative->SetValue(geoTreeView->GetLayoutPrimativeVisibility());

	// tree style properties
	Colour treeColour = geoTreeView->GetColour();
	m_colourTree->SetColour(wxColour(treeColour.GetRedInt(), treeColour.GetGreenInt(), treeColour.GetBlueInt(), treeColour.GetAlphaInt()));
	ReplaceColourPicker( m_colourTree, treeColour );

	if(geoTreeView->GetTreeColourStyle() == GeoTreeView::COLOUR_SINGLE)
		m_cboTreeColourStyle->SetValue(_T("Single colour"));
	else if(geoTreeView->GetTreeColourStyle() == GeoTreeView::COLOUR_DISCRETE)
		m_cboTreeColourStyle->SetValue(_T("Propogate discrete colours"));
	else if(geoTreeView->GetTreeColourStyle() == GeoTreeView::COLOUR_CONTINUOUS)
		m_cboTreeColourStyle->SetValue(_T("Propogate continuous colours"));

	m_spinLeafNodeSize->SetValue(geoTreeView->GetLeafNodeSize());
	m_spinInternalNodeSize->SetValue(geoTreeView->GetInternalNodeSize());

	m_spinBranchBorderSize->SetValue(geoTreeView->GetBranchBorderSize());
	m_spinLeafNodeBorderSize->SetValue(geoTreeView->GetLeafNodeBorderSize());
	m_spinInternalNodeBorderSize->SetValue(geoTreeView->GetInternalNodeBorderSize());
	m_spinGeoPointBorderSize->SetValue(geoTreeView->GetGeoPtsBorderSize());
	m_spinLocationLineBorderSize->SetValue(geoTreeView->GetLocationLineBorderSize());
	m_spinCorrelationLineBorderSize->SetValue(geoTreeView->GetCorrelationLineBorderSize());

	Colour leafNodeBorderColour = geoTreeView->GetLeafNodeBorderColour();
	m_colourLeafNodeBorder->SetColour(wxColour(leafNodeBorderColour.GetRedInt(), leafNodeBorderColour.GetGreenInt(), 
		leafNodeBorderColour.GetBlueInt(), leafNodeBorderColour.GetAlphaInt()));
	ReplaceColourPicker( m_colourLeafNodeBorder, leafNodeBorderColour );

	Colour internalNodeBorderColour = geoTreeView->GetInternalNodeBorderColour();
	m_colourInternalNodeBorder->SetColour(wxColour(internalNodeBorderColour.GetRedInt(), internalNodeBorderColour.GetGreenInt(), 
		internalNodeBorderColour.GetBlueInt(), internalNodeBorderColour.GetAlphaInt()));
	ReplaceColourPicker( m_colourInternalNodeBorder, internalNodeBorderColour );

	Colour geoPtsBorderColour = geoTreeView->GetGeoPtsBorderColour();
	m_colourGeoPointBorder->SetColour(wxColour(geoPtsBorderColour.GetRedInt(), geoPtsBorderColour.GetGreenInt(), 
		geoPtsBorderColour.GetBlueInt(), geoPtsBorderColour.GetAlphaInt()));
	ReplaceColourPicker( m_colourGeoPointBorder, geoPtsBorderColour );

	Colour branchBorderColour = geoTreeView->GetBranchBorderColour();
	m_colourBranchBorder->SetColour(wxColour(branchBorderColour.GetRedInt(), branchBorderColour.GetGreenInt(), 
		branchBorderColour.GetBlueInt(), branchBorderColour.GetAlphaInt()));
	ReplaceColourPicker( m_colourBranchBorder, branchBorderColour );

	Colour locationLineBorderColour = geoTreeView->GetLocationLineBorderColour();
	m_colourLocationLineBorder->SetColour(wxColour(locationLineBorderColour.GetRedInt(), locationLineBorderColour.GetGreenInt(), 
		locationLineBorderColour.GetBlueInt(), locationLineBorderColour.GetAlphaInt()));
	ReplaceColourPicker( m_colourLocationLineBorder, locationLineBorderColour );

	Colour correlationLineBorderColour = geoTreeView->GetCorrelationLineBorderColour();
	m_colourCorrelationLineBorder->SetColour(wxColour(correlationLineBorderColour.GetRedInt(), correlationLineBorderColour.GetGreenInt(), 
		correlationLineBorderColour.GetBlueInt(), correlationLineBorderColour.GetAlphaInt()));
	ReplaceColourPicker( m_colourCorrelationLineBorder, correlationLineBorderColour );

	// location line properties
	m_spinLocationLineThickness->SetValue(geoTreeView->LocationLines().GetThickness());

	if(geoTreeView->LocationLines().GetLineStyle() == VisualLine::SOLID)
		m_cboLocationLineStyle->SetValue(_T("Solid"));
	else if(geoTreeView->LocationLines().GetLineStyle() == VisualLine::SHORT_DASH)
		m_cboLocationLineStyle->SetValue(_T("Short dash"));
	else if(geoTreeView->LocationLines().GetLineStyle() == VisualLine::LONG_DASH)
		m_cboLocationLineStyle->SetValue(_T("Long dash"));
	else if(geoTreeView->LocationLines().GetLineStyle() == VisualLine::HIDDEN)
		m_cboLocationLineStyle->SetValue(_T("Hidden"));


	// correlation line properties
	m_spinCorrelationLineThickness->SetValue(geoTreeView->CorrelationLines().GetThickness());

	if(geoTreeView->CorrelationLines().GetLineStyle() == VisualLine::SOLID)
		m_cboCorrelationLineStyle->SetValue(_T("Solid"));
	else if(geoTreeView->CorrelationLines().GetLineStyle() == VisualLine::SHORT_DASH)
		m_cboCorrelationLineStyle->SetValue(_T("Short dash"));
	else if(geoTreeView->CorrelationLines().GetLineStyle() == VisualLine::LONG_DASH)
		m_cboCorrelationLineStyle->SetValue(_T("Long dash"));
	else if(geoTreeView->CorrelationLines().GetLineStyle() == VisualLine::HIDDEN)
		m_cboCorrelationLineStyle->SetValue(_T("Hidden"));

	// internal drop line properties
	m_spinInternalDropLineThickness->SetValue(geoTreeView->Get3dInternalDropLineStyle().GetSize());

	Colour colour = geoTreeView->Get3dInternalDropLineStyle().GetColour();
	m_colourInternalDropLine->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt()));
	ReplaceColourPicker( m_colourInternalDropLine, colour );

	if(geoTreeView->Get3dInternalDropLineStyle().GetLineStyle() == VisualLine::SOLID)
		m_cboInternalDropLineStyle->SetValue(_T("Solid"));
	else if(geoTreeView->Get3dInternalDropLineStyle().GetLineStyle() == VisualLine::SHORT_DASH)
		m_cboInternalDropLineStyle->SetValue(_T("Short dash"));
	else if(geoTreeView->Get3dInternalDropLineStyle().GetLineStyle() == VisualLine::LONG_DASH)
		m_cboInternalDropLineStyle->SetValue(_T("Long dash"));
	else if(geoTreeView->Get3dInternalDropLineStyle().GetLineStyle() == VisualLine::HIDDEN)
		m_cboInternalDropLineStyle->SetValue(_T("Hidden"));

	// leaf drop line properties
	if(geoTreeView->Get3dLeafDropLineStyle().GetLineStyle() == VisualLine::SOLID)
		m_cboLeafDropLineStyle->SetValue(_T("Solid"));
	else if(geoTreeView->Get3dLeafDropLineStyle().GetLineStyle() == VisualLine::SHORT_DASH)
		m_cboLeafDropLineStyle->SetValue(_T("Short dash"));
	else if(geoTreeView->Get3dLeafDropLineStyle().GetLineStyle() == VisualLine::LONG_DASH)
		m_cboLeafDropLineStyle->SetValue(_T("Long dash"));
	else if(geoTreeView->Get3dLeafDropLineStyle().GetLineStyle() == VisualLine::HIDDEN)
		m_cboLeafDropLineStyle->SetValue(_T("Hidden"));

	// geography line properties
	m_spinGeoLineOffset->SetValue(uint(geoTreeView->GetGeographyLineOffsetPercentage()*100 + 0.5));

	m_spinGeoLineThickness->SetValue(geoTreeView->GeographyLine().GetSize());

	if(geoTreeView->GeographyLine().GetLineStyle() == VisualLine::SOLID)
		m_cboGeoLineStyle->SetValue(_T("Solid"));
	else if(geoTreeView->GeographyLine().GetLineStyle() == VisualLine::SHORT_DASH)
		m_cboGeoLineStyle->SetValue(_T("Short dash"));
	else if(geoTreeView->GeographyLine().GetLineStyle() == VisualLine::LONG_DASH)
		m_cboGeoLineStyle->SetValue(_T("Long dash"));
	else if(geoTreeView->GeographyLine().GetLineStyle() == VisualLine::HIDDEN)
		m_cboGeoLineStyle->SetValue(_T("Hidden"));

	colour = geoTreeView->GeographyLine().GetColour();
	m_colourGeoLine->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt()));
	ReplaceColourPicker( m_colourGeoLine, colour );

	// geographic point properties
	m_spinGeoPointSize->SetValue(geoTreeView->GeographicPoints().GetSize());

	m_chkDrawGeoPoint->SetValue(geoTreeView->GeographicPoints().GetVisibility());
	m_chkEvenlySpeadGeoPts->SetValue(geoTreeView->GetSpreadGeographyPts());

	m_chkDrawGeoAxis->SetValue(geoTreeView->GetGeographicAxisVisibility());

	m_chkOptimizeLeafNodes->SetValue(geoTreeView->GetOptimizeTopology());


	// *** Set state of controls on Label page
	m_spinLabelSize->SetValue(geoTreeView->GetFontSize());

	Colour fontColour = geoTreeView->GetFontColour();
	m_colourLabel->SetColour(wxColour(fontColour.GetRedInt(), fontColour.GetGreenInt(), fontColour.GetBlueInt(), fontColour.GetAlphaInt()));
	ReplaceColourPicker( m_colourLabel, fontColour );

	m_chkShowLabels->SetValue(geoTreeView->GetLabelVisibility());

	// set state of controls on Metadata page
	m_txtLayerSource->SetValue(wxString(m_treeLayer->GetPath().c_str()) + _T("\\") + wxString(m_treeLayer->GetFilename().c_str()));
	SetMetaData();
}

void TreePropertiesDlg::SetMetaData()
{
	GeoTreeViewPtr geoTreeView = m_treeLayer->GetGeoTreeView();

	m_txtMetaData->BeginBold();
	m_txtMetaData->AppendText(_T("Number of nodes:\n"));
	m_txtMetaData->EndBold();
	m_txtMetaData->AppendText(wxString(StringTools::ToStringW(geoTreeView->GetNumberOfNodes()).c_str()) + _T("\n"));

	m_txtMetaData->BeginBold();
	m_txtMetaData->AppendText(_T("\nNumber of leaf nodes:\n"));
	m_txtMetaData->EndBold();
	m_txtMetaData->AppendText(wxString(StringTools::ToStringW(geoTreeView->GetNumberOfLeaves()).c_str()) + _T("\n"));

	m_txtMetaData->BeginBold();
	m_txtMetaData->AppendText(_T("\nDepth of tree:\n"));
	m_txtMetaData->EndBold();
	m_txtMetaData->AppendText(wxString(StringTools::ToStringW(geoTreeView->GetDepthOfTree()).c_str()) + _T("\n"));

	m_txtMetaData->BeginBold();
	m_txtMetaData->AppendText(_T("\nMultifurcating tree:\n"));
	m_txtMetaData->EndBold();
	if(geoTreeView->IsMultifurcating())
		m_txtMetaData->AppendText(_T("True\n"));
	else
		m_txtMetaData->AppendText(_T("False\n"));

	m_txtMetaData->BeginBold();
	m_txtMetaData->AppendText(_T("\nTopology optimized to leaf node geography:\n"));
	m_txtMetaData->EndBold();
	if(geoTreeView->GetOptimizeTopology())
		m_txtMetaData->AppendText(_T("True"));
	else
		m_txtMetaData->AppendText(_T("False"));
}

void TreePropertiesDlg::Apply()
{
	// *** Set properties based on state of controls in General page
	m_treeLayer->SetName(m_txtLayerName->GetValue().c_str());
	m_treeLayer->SetDescription(m_txtLayerDescription->GetValue().c_str());
	m_treeLayer->SetAuthours(m_txtAuthours->GetValue().c_str());

	// *** Set properties based on state of control in Symbology page
	GeoTreeViewPtr geoTreeView = m_treeLayer->GetGeoTreeView();

	if(m_cboTreeLayout->GetValue() == _T("3D slanted cladogram"))
		geoTreeView->SetLayout(GeoTreeView::SLANTED_CLADOGRAM_3D);
	else if(m_cboTreeLayout->GetValue() == _T("3D cladogram"))
		geoTreeView->SetLayout(GeoTreeView::CLADOGRAM_3D);
	else if(m_cboTreeLayout->GetValue() == _T("3D slanted phylogram"))
		geoTreeView->SetLayout(GeoTreeView::SLANTED_PHYLOGRAM_3D);
	else if(m_cboTreeLayout->GetValue() == _T("2D slanted cladogram"))
		geoTreeView->SetLayout(GeoTreeView::SLANTED_CLADOGRAM_2D);
	else if(m_cboTreeLayout->GetValue() == _T("2D cladogram"))
		geoTreeView->SetLayout(GeoTreeView::CLADOGRAM_2D);
	else if(m_cboTreeLayout->GetValue() == _T("2D phylogram"))
		geoTreeView->SetLayout(GeoTreeView::PHYLOGRAM_2D);
	else if(m_cboTreeLayout->GetValue() == _T("Elliptical layout"))
		geoTreeView->SetLayout(GeoTreeView::ELLIPSE_QUADRANT);

	geoTreeView->SetLineThickness(m_spinTreeThickness->GetValue());

	geoTreeView->SetHeight(StringTools::ToDouble(std::wstring(m_txtTreeHeight->GetValue())));

	if(m_cbo2dOrientation->GetValue() == _T("Vertical"))
		geoTreeView->SetOrientation(GeoTreeView::VERTICAL);
	else if(m_cbo2dOrientation->GetValue() == _T("Horizontal"))
		geoTreeView->SetOrientation(GeoTreeView::HORIZONTAL);

	geoTreeView->SetLayoutPrimativeVisibility(m_chkDrawLayoutPrimative->GetValue());

	geoTreeView->SetColour(Colour(m_colourTree->GetColour()));

	if(m_cboTreeColourStyle->GetValue() == _T("Single colour"))
		geoTreeView->SetTreeColourStyle(GeoTreeView::COLOUR_SINGLE);
	else if(m_cboTreeColourStyle->GetValue() == _T("Propogate discrete colours"))
		geoTreeView->SetTreeColourStyle(GeoTreeView::COLOUR_DISCRETE);
	else if(m_cboTreeColourStyle->GetValue() == _T("Propogate continuous colours"))
		geoTreeView->SetTreeColourStyle(GeoTreeView::COLOUR_CONTINUOUS);

	geoTreeView->SetLeafNodeSize(m_spinLeafNodeSize->GetValue());
	geoTreeView->SetInternalNodeSize(m_spinInternalNodeSize->GetValue());

	geoTreeView->SetLeafNodeBorderSize(m_spinLeafNodeBorderSize->GetValue());
	geoTreeView->SetInternalNodeBorderSize(m_spinInternalNodeBorderSize->GetValue());
	geoTreeView->SetGeoPtsBorderSize(m_spinGeoPointBorderSize->GetValue());
	geoTreeView->SetBranchBorderSize(m_spinBranchBorderSize->GetValue());
	geoTreeView->SetLocationLineBorderSize(m_spinLocationLineBorderSize->GetValue());
	geoTreeView->SetCorrelationLineBorderSize(m_spinCorrelationLineBorderSize->GetValue());

	geoTreeView->SetLeafNodeBorderColour(Colour(m_colourLeafNodeBorder->GetColour()));
	geoTreeView->SetInternalNodeBorderColour(Colour(m_colourInternalNodeBorder->GetColour()));
	geoTreeView->SetGeoPtsBorderColour(Colour(m_colourGeoPointBorder->GetColour()));
	geoTreeView->SetBranchBorderColour(Colour(m_colourBranchBorder->GetColour()));
	geoTreeView->SetLocationLineBorderColour(Colour(m_colourLocationLineBorder->GetColour()));
	geoTreeView->SetCorrelationLineBorderColour(Colour(m_colourCorrelationLineBorder->GetColour()));

	// set location line properties
	geoTreeView->LocationLines().SetThickness(m_spinLocationLineThickness->GetValue());

	geoTreeView->LocationLines().SetVisibility(true);
	if(m_cboLocationLineStyle->GetValue() == _T("Solid"))
		geoTreeView->LocationLines().SetLineStyle(VisualLine::SOLID);
	else if(m_cboLocationLineStyle->GetValue() == _T("Short dash"))
		geoTreeView->LocationLines().SetLineStyle(VisualLine::SHORT_DASH);
	else if(m_cboLocationLineStyle->GetValue() == _T("Long dash"))
		geoTreeView->LocationLines().SetLineStyle(VisualLine::LONG_DASH);
	else if(m_cboLocationLineStyle->GetValue() == _T("Hidden"))
	{
		geoTreeView->LocationLines().SetLineStyle(VisualLine::HIDDEN);
		geoTreeView->LocationLines().SetVisibility(false);
	}

	// set correlation line properties
	geoTreeView->CorrelationLines().SetThickness(m_spinCorrelationLineThickness->GetValue());

	geoTreeView->CorrelationLines().SetVisibility(true);
	if(m_cboCorrelationLineStyle->GetValue() == _T("Solid"))
		geoTreeView->CorrelationLines().SetLineStyle(VisualLine::SOLID);
	else if(m_cboCorrelationLineStyle->GetValue() == _T("Short dash"))
		geoTreeView->CorrelationLines().SetLineStyle(VisualLine::SHORT_DASH);
	else if(m_cboCorrelationLineStyle->GetValue() == _T("Long dash"))
		geoTreeView->CorrelationLines().SetLineStyle(VisualLine::LONG_DASH);
	else if(m_cboCorrelationLineStyle->GetValue() == _T("Hidden"))
	{
		geoTreeView->CorrelationLines().SetLineStyle(VisualLine::HIDDEN);
		geoTreeView->CorrelationLines().SetVisibility(false);
	}

	// set internal 3d drop line properties
	VisualLine internalDropLineStyle = geoTreeView->Get3dInternalDropLineStyle();
	internalDropLineStyle.SetSize(m_spinInternalDropLineThickness->GetValue());
	internalDropLineStyle.SetColour(Colour(m_colourInternalDropLine->GetColour()));

	internalDropLineStyle.SetVisibility(true);
	if(m_cboInternalDropLineStyle->GetValue() == _T("Solid"))
		internalDropLineStyle.SetLineStyle(VisualLine::SOLID);
	else if(m_cboInternalDropLineStyle->GetValue() == _T("Short dash"))
		internalDropLineStyle.SetLineStyle(VisualLine::SHORT_DASH);
	else if(m_cboInternalDropLineStyle->GetValue() == _T("Long dash"))
		internalDropLineStyle.SetLineStyle(VisualLine::LONG_DASH);
	else if(m_cboInternalDropLineStyle->GetValue() == _T("Hidden"))
	{
		internalDropLineStyle.SetLineStyle(VisualLine::HIDDEN);
		internalDropLineStyle.SetVisibility(true);
	}

	geoTreeView->Set3dInternalDropLineStyle(internalDropLineStyle);

	// set leaf 3d drop line properties
	VisualLine leafDropLineStyle = geoTreeView->Get3dLeafDropLineStyle();

	leafDropLineStyle.SetVisibility(true);
	if(m_cboLeafDropLineStyle->GetValue() == _T("Solid"))
		leafDropLineStyle.SetLineStyle(VisualLine::SOLID);
	else if(m_cboLeafDropLineStyle->GetValue() == _T("Short dash"))
		leafDropLineStyle.SetLineStyle(VisualLine::SHORT_DASH);
	else if(m_cboLeafDropLineStyle->GetValue() == _T("Long dash"))
		leafDropLineStyle.SetLineStyle(VisualLine::LONG_DASH);
	else if(m_cboLeafDropLineStyle->GetValue() == _T("Hidden"))
	{
		leafDropLineStyle.SetLineStyle(VisualLine::HIDDEN);
		leafDropLineStyle.SetVisibility(true);
	}

	geoTreeView->Set3dLeafDropLineStyle(leafDropLineStyle);

	// set geography line properties
	geoTreeView->SetGeographyLineOffsetPercentage(m_spinGeoLineOffset->GetValue() / 100.0);
	geoTreeView->GeographyLine().SetSize(m_spinGeoLineThickness->GetValue());
	geoTreeView->GeographyLine().SetColour(Colour(m_colourGeoLine->GetColour()));

	geoTreeView->GeographyLine().SetVisibility(true);
	if(m_cboGeoLineStyle->GetValue() == _T("Solid"))
		geoTreeView->GeographyLine().SetLineStyle(VisualLine::SOLID);
	else if(m_cboGeoLineStyle->GetValue() == _T("Short dash"))
		geoTreeView->GeographyLine().SetLineStyle(VisualLine::SHORT_DASH);
	else if(m_cboGeoLineStyle->GetValue() == _T("Long dash"))
		geoTreeView->GeographyLine().SetLineStyle(VisualLine::LONG_DASH);
	else if(m_cboGeoLineStyle->GetValue() == _T("Hidden"))
	{
		geoTreeView->GeographyLine().SetLineStyle(VisualLine::HIDDEN);
		geoTreeView->GeographyLine().SetVisibility(true);
	}

	geoTreeView->SetOptimizeTopology(m_chkOptimizeLeafNodes->GetValue());

	// geographic point properties
	geoTreeView->GeographicPoints().SetSize(m_spinGeoPointSize->GetValue());

	geoTreeView->GeographicPoints().SetVisibility(m_chkDrawGeoPoint->GetValue());
	geoTreeView->SetSpreadGeographyPts(m_chkEvenlySpeadGeoPts->GetValue());

	geoTreeView->SetGeographicAxisVisibility(m_chkDrawGeoAxis->GetValue());

	// *** Set properties based on state of control in Label page
	geoTreeView->SetFontSize(m_spinLabelSize->GetValue());

	geoTreeView->SetFontColour(Colour(m_colourLabel->GetColour()));

	geoTreeView->SetLabelVisibility(m_chkShowLabels->GetValue());

	geoTreeView->ForceTreeLayout();

	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	App::Inst().GetViewport()->Refresh( false );
	App::Inst().GetLayerTreeController()->SetName( m_treeLayer, m_treeLayer->GetName() );
}

void TreePropertiesDlg::OnLayoutStyleChange( wxCommandEvent& event )
{
	GeoTreeViewPtr geoTreeView = m_treeLayer->GetGeoTreeView();

	// only allow 2D views if a layout line exists
	if(!geoTreeView->IsLayoutLine())
	{
		if(m_cboTreeLayout->GetValue() == _T("2D slanted cladogram")
			|| m_cboTreeLayout->GetValue() == _T("2D cladogram")
			|| m_cboTreeLayout->GetValue() == _T("2D phylogram"))
		{
			wxMessageBox(wxT("Please draw a layout line before switching to a 2D tree layout."),
				wxT("Missing layout line"), wxOK | wxICON_INFORMATION);

			// change it back to what it previously was
			if(geoTreeView->GetLayout() == GeoTreeView::SLANTED_CLADOGRAM_3D)
				m_cboTreeLayout->SetValue(_T("3D slanted cladogram"));
			else if(geoTreeView->GetLayout() == GeoTreeView::CLADOGRAM_3D)
				m_cboTreeLayout->SetValue(_T("3D cladogram"));
			else if(geoTreeView->GetLayout() == GeoTreeView::SLANTED_PHYLOGRAM_3D)
				m_cboTreeLayout->SetValue(_T("3D phylogram cladogram"));
			else if(geoTreeView->GetLayout() == GeoTreeView::ELLIPSE_QUADRANT)
				m_cboTreeLayout->SetValue(_T("Elliptical layout"));
		}
	}
}

void TreePropertiesDlg::ReplaceColourPicker( wxColourPickerCtrl* wxColourPicker, Colour& colour )
{
	#ifndef WIN32
	wxColourPicker->Hide();
	CustomColourButton* customColourButton = new CustomColourButton( wxColourPicker->GetParent(), wxColourPicker,
		wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt()) );

	wxSizer* colourIconSizer = wxColourPicker->GetContainingSizer();
	colourIconSizer->Replace( wxColourPicker, customColourButton );
	colourIconSizer->Layout();

	customColourButton->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( TreePropertiesDlg::OnCustomColourButtonClicked ), NULL, this );
	#endif
}

void TreePropertiesDlg::OnCustomColourButtonClicked( wxMouseEvent& event )
{
	// Retrieve the CustomColourButton object that was clicked
	CustomColourButton* customColourButton = ( CustomColourButton* )event.GetEventObject();

	// Create and open the colour picker dialog
	wxColourData *colourData = new wxColourData();
	colourData->SetColour( customColourButton->GetBackgroundColour() );
	wxColourDialog *colourPicker = new wxColourDialog( this, colourData );
	if ( colourPicker->ShowModal() == wxID_CANCEL ) return;

	// Set the CustomColourButton background colour (used for display under Mac/Unix releases)
	customColourButton->SetBackgroundColour( colourPicker->GetColourData().GetColour() );
	customColourButton->Refresh();

	// Set the wxColourPickerCtrl colour picker (hidden, but its value will be used to change the location colour)
	customColourButton->GetWXColourPickerCtrl()->SetColour( colourPicker->GetColourData().GetColour() );
}
