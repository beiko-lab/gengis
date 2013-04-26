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

#include "../gui/LocationPropertiesDlg.hpp"
#include "../gui/ProgressDlg.hpp"
#include "../gui/ColourPickerDlg.hpp"
#include "../gui/GenGisFrame.hpp"

#include "../core/App.hpp"
#include "../core/ChartView.hpp"
#include "../core/DropLines.hpp"
#include "../core/SequenceController.hpp"
#include "../core/VisualLabel.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/Viewport.hpp"
#include "../core/LayerTreeController.hpp"

#include "../utils/StringTools.hpp"
#include "../utils/Colour.hpp"
#include "../utils/ColourMap.hpp"
#include "../utils/ColourMapManager.hpp"

#include "../widgets/CustomColourButton.hpp"

using namespace GenGIS;

LocationPropertiesDlg::LocationPropertiesDlg(wxWindow* parent, LocationLayerPtr locationLayer)
	: LocationPropertiesLayout(parent), m_locationLayer(locationLayer)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	// Limit the properties dialog to a single instance
	m_locationLayer->SetPropertiesDialog( this );

	InitGeneral();
	InitSymbology();
	InitMetadata();

	Fit();
}

LocationPropertiesDlg::~LocationPropertiesDlg()
{
	// Tell the layer that the properties dialog is being closed
	m_locationLayer->SetPropertiesDialogOpenStatus( false );
	m_locationLayer->SetPropertiesDialog( NULL );
}

void LocationPropertiesDlg::InitGeneral()
{
	// set the title of the properties dialog
	this->SetLabel( wxString(m_locationLayer->GetName().c_str()) + wxT( " : Location Properties" ) );

	m_txtLayerName->SetValue(wxString(m_locationLayer->GetName().c_str()));
	m_txtLayerDescription->SetValue(wxString(m_locationLayer->GetDescription().c_str()));
	m_txtAuthours->SetValue(wxString(m_locationLayer->GetAuthours().c_str()));
}

void LocationPropertiesDlg::InitSymbology()
{
	LocationViewPtr locationView = m_locationLayer->GetLocationController()->GetLocationView();

	Colour colour = locationView->GetColour();
	m_colourSymbol->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt()));
	ReplaceColourPicker( m_colourSymbol, colour );
	m_txtSymbolSize->SetValue(wxString(StringTools::ToStringW(locationView->GetSize(), 2).c_str()));
	
	VisualMarker::MARKER_SHAPE shape = locationView->GetShape();
	if(shape == VisualMarker::CIRCLE)
		m_cboSymbolShape->SetValue(_T("Circle"));
	else if(shape == VisualMarker::SQUARE)
		m_cboSymbolShape->SetValue(_T("Square"));
	else if(shape == VisualMarker::TRIANGLE)
		m_cboSymbolShape->SetValue(_T("Triangle"));
	else if(shape == VisualMarker::STAR)
		m_cboSymbolShape->SetValue(_T("Star"));
	else if(shape == VisualMarker::PLUS_SIGN)
		m_cboSymbolShape->SetValue(_T("Plus sign"));
	else if(shape == VisualMarker::OCTAGON)
		m_cboSymbolShape->SetValue(_T("octagon"));
	else if(shape == VisualMarker::DIAMOND)
		m_cboSymbolShape->SetValue(_T("Diamond"));
	else if(shape == VisualMarker::INVERTED_TRIANGLE)
		m_cboSymbolShape->SetValue(_T("Inverted triangle"));

	VisualLabelPtr label = locationView->GetLabel();
	m_spinLabelSize->SetValue(label->GetSize());
	m_cboLabelPosition->SetValue(wxString(label->GetLabelPosition().c_str()));
	m_cboLabelStyle->SetValue(wxString(label->GetLabelStyle().c_str()));

	colour = label->GetColour();
	m_colourLabel->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt()));
	ReplaceColourPicker( m_colourLabel, colour );

	m_chkLabelVisible->SetValue(label->IsVisible());
}

void LocationPropertiesDlg::InitMetadata()
{
	m_txtLayerSource->SetValue(wxString(m_locationLayer->GetPath().c_str()) + _T("\\") + wxString(m_locationLayer->GetFilename().c_str()));

	std::map<std::wstring,std::wstring> data = m_locationLayer->GetLocationController()->GetData();
	for(std::map<std::wstring,std::wstring>::iterator it=data.begin(); it != data.end(); it++)
	{
		m_txtMetaData->BeginBold();
		m_txtMetaData->AppendText(wxString((*it).first.c_str()) + _T(":\n"));
		m_txtMetaData->EndBold();
		m_txtMetaData->AppendText(wxString((*it).second.c_str()) + _T("\n\n"));
	}
}

void LocationPropertiesDlg::Apply()
{
	ApplyGeneral();
	ApplySymbology();
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}

void LocationPropertiesDlg::ApplyGeneral()
{
	// set properties based on state of controls in General page
	App::Inst().GetLayerTreeController()->SetName(m_locationLayer, m_txtLayerName->GetValue().c_str());

	m_locationLayer->SetName(m_txtLayerName->GetValue().c_str());
	m_locationLayer->SetDescription(m_txtLayerDescription->GetValue().c_str());
	m_locationLayer->SetAuthours(m_txtAuthours->GetValue().c_str());
}

void LocationPropertiesDlg::ApplySymbology()
{
	LocationViewPtr locationView = m_locationLayer->GetLocationController()->GetLocationView();

	// set properties based on state of controls in Symbology page
	Colour colour = Colour(m_colourSymbol->GetColour());
	if(colour != locationView->GetColour())
	{
		locationView->SetColour(colour);
		locationView->SetColourModified(true);
	}

	if(locationView->GetSize() != StringTools::ToDouble(m_txtSymbolSize->GetValue().c_str()))
	{
		locationView->SetSize(StringTools::ToDouble(m_txtSymbolSize->GetValue().c_str()));
		locationView->SetSizeModified(true);
	}
	
	VisualMarker::MARKER_SHAPE shape;
	if(m_cboSymbolShape->GetValue() == _T("Circle"))
		shape = VisualMarker::CIRCLE;
	else if(m_cboSymbolShape->GetValue() == _T("Square"))
		shape = VisualMarker::SQUARE;
	else if(m_cboSymbolShape->GetValue() == _T("Triangle"))
		shape = VisualMarker::TRIANGLE;
	else if(m_cboSymbolShape->GetValue() == _T("Star"))
		shape = VisualMarker::STAR;
	else if(m_cboSymbolShape->GetValue() == _T("Plus sign"))
		shape = VisualMarker::PLUS_SIGN;
	else if(m_cboSymbolShape->GetValue() == _T("octagon"))
		shape = VisualMarker::OCTAGON;
	else if(m_cboSymbolShape->GetValue() == _T("Diamond"))
		shape = VisualMarker::DIAMOND;
	else if(m_cboSymbolShape->GetValue() == _T("Inverted triangle"))
		shape = VisualMarker::INVERTED_TRIANGLE;

	if(locationView->GetShape() != shape)
	{
		locationView->SetShape(shape);
		locationView->SetShapeModified(true);
	}

	// set label properties
	VisualLabelPtr label = locationView->GetLabel();
	if(label->GetSize() != m_spinLabelSize->GetValue())
	{
		label->SetSize(m_spinLabelSize->GetValue());
		locationView->SetLabelSizeModified(true);
	}

	if(label->GetLabelPosition() != m_cboLabelPosition->GetValue().c_str())
	{
		label->SetLabelPosition(m_cboLabelPosition->GetValue().c_str());
		locationView->SetLabelPositionModified(true);
	}

	if(label->GetLabelStyle() != m_cboLabelStyle->GetValue().c_str())
	{
		label->SetLabelStyle(m_cboLabelStyle->GetValue().c_str());
		locationView->SetLabelStyleModified(true);
	}

	colour = Colour(m_colourLabel->GetColour());
	if(label->GetColour() != colour)
	{
		label->SetColour(colour);
		locationView->SetLabelColourModified(true);
	}

	if(label->IsVisible() != m_chkLabelVisible->GetValue())
	{
		label->SetVisibility(m_chkLabelVisible->GetValue());
		locationView->SetLabelVisibilityModified(true);
	}

	locationView->SetLabel(label);

	((GenGisFrame*)App::Inst().GetMainWindow())->FillSamplesLegend(); 

	App::Inst().GetViewport()->Refresh(false);
}

void LocationPropertiesDlg::ReplaceColourPicker( wxColourPickerCtrl* wxColourPicker, Colour& colour )
{
	#ifndef WIN32
	wxColourPicker->Hide();
	CustomColourButton* customColourButton = new CustomColourButton( wxColourPicker->GetParent(), wxColourPicker,
		wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt()) );

	wxSizer* colourIconSizer = wxColourPicker->GetContainingSizer();
	colourIconSizer->Replace( wxColourPicker, customColourButton );
	colourIconSizer->Layout();

	customColourButton->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LocationPropertiesDlg::OnCustomColourButtonClicked ), NULL, this );
	#endif
}

void LocationPropertiesDlg::OnCustomColourButtonClicked( wxMouseEvent& event )
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
