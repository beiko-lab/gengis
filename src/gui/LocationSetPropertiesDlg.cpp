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

#include "../gui/LocationSetPropertiesDlg.hpp"
#include "../gui/ProgressDlg.hpp"
#include "../gui/ColourMapWidget.hpp"
#include "../gui/ShapeMapWidget.hpp"
#include "../gui/ColourPickerDlg.hpp"

#include "../core/App.hpp"
#include "../core/DropLines.hpp"
#include "../core/ChartView.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/SequenceController.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/LocationSetController.hpp"
#include "../core/Viewport.hpp"
#include "../core/VisualLines.hpp"

#include "../utils/StringTools.hpp"
#include "../utils/ColourMapDiscrete.hpp"
#include "../utils/ColourMapManager.hpp"

#include "../widgets/CustomColourButton.hpp"

using namespace GenGIS;

LocationSetPropertiesDlg::LocationSetPropertiesDlg(wxWindow* parent, LocationSetLayerPtr locationSetLayer, int currentPageSelection) : 
	LocationSetPropertiesLayout(parent),
	m_locationSetLayer(locationSetLayer), 
	m_colourMapWidget(new ColourMapWidget(m_cboColourMap, m_scrolledWindowChart, m_scrolledWindowColour)),
	m_chartColourMapWidget(new ColourMapWidget(m_cboChartColourMap, m_scrolledWindowChart, m_scrolledWindowColour)),
	m_shapeMapWidget(new ShapeMapWidget(m_cboShapeMap))
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	// Limit the properties dialog to a single instance
	m_locationSetLayer->SetPropertiesDialog( this );

	m_locationSetController = locationSetLayer->GetLocationSetController();
	m_locationSetController->SetLocationSetLayers(locationSetLayer->GetAllLocationLayers());

	InitGeneral();
	InitMetadata();
	InitLocationSet();
	InitChart();

	m_pnlLocations->Fit();
	m_pnlChart->Fit();
	Fit();

	if ( currentPageSelection >= 0 )
		m_notebookLocationSet->SetSelection( currentPageSelection );
}

LocationSetPropertiesDlg::~LocationSetPropertiesDlg()
{
	// Tell the layer that the properties dialog is being closed
	m_locationSetLayer->SetPropertiesDialogOpenStatus( false );
	m_locationSetLayer->SetPropertiesDialog( NULL );
}

void LocationSetPropertiesDlg::InitGeneral()
{
	// set the title of the properties dialog
	this->SetLabel( wxString(m_locationSetLayer->GetName().c_str()) + wxT( " : Location Set Properties" ) );
	
	m_txtLayerName->SetValue(wxString(m_locationSetLayer->GetName().c_str()));
	m_txtLayerDescription->SetValue(wxString(m_locationSetLayer->GetDescription().c_str()));
	m_txtAuthours->SetValue(wxString(m_locationSetLayer->GetAuthours().c_str()));
}

void LocationSetPropertiesDlg::InitLocationSet()
{
	wxBusyCursor wait;

	InitLocationSetColour();
	InitLocationSetShape();
	InitLocationSetSize();
	InitLocationSetLabel();
}

void LocationSetPropertiesDlg::InitLocationSetColour()
{
	// populate combo box with all fields associated with a location
	std::vector<std::wstring> fields = m_locationSetController->GetMetadataFields();
	std::vector<std::wstring>::iterator it;
	for(it = fields.begin(); it != fields.end(); ++it)
	{
		m_cboColourField->Append(wxString((*it).c_str()));
	}

	if(!m_locationSetController->GetColourField().empty())
		m_cboColourField->SetValue(m_locationSetController->GetColourField().c_str());
	else
	{
		if(!m_cboColourField->IsEmpty())
			m_cboColourField->SetValue(m_cboColourField->GetString(0));
	}

	// Populate colour map combo box with all available colour maps
	m_colourMapWidget->SetColourMap(m_locationSetController->GetColourMap());
	m_colourMapWidget->PopulateColourMapComboBox();

	// Set field values
	wxCommandEvent dummy;
	OnColourFieldChange(dummy);

	Colour uniColour = m_locationSetController->GetUniformColour();
	m_colourUniform->SetColour(wxColour(uniColour.GetRedInt(), uniColour.GetGreenInt(), uniColour.GetBlueInt()));
	ReplaceColourPicker( m_colourUniform, uniColour );

	// Set uniform colour checkbox and colour
	if(!m_locationSetController->ModifiedColour())
	{
		m_chkUniformColour->SetValue(m_locationSetController->GetUniformColourFlag());

		wxCommandEvent dummy;
		OnUniformColour(dummy);
	}
	else
	{
		m_chkUniformColour->Set3StateValue(wxCHK_UNDETERMINED);

		#ifdef WIN32
		m_colourUniform->Enable(false);
		#else
		EnableButton( m_colourUniform, false );
		#endif
		m_scrolledWindowColour->Enable(false);
		m_cboColourField->Enable(false);
		m_cboColourMap->Enable(false);
	}

	m_spinBorderSize->SetValue(m_locationSetController->GetBorderSize());

	Colour borderColour = m_locationSetController->GetBorderColour();
	m_colourBorders->SetColour(wxColour(borderColour.GetRedInt(), borderColour.GetGreenInt(), borderColour.GetBlueInt()));
	ReplaceColourPicker( m_colourBorders, borderColour );
}

void LocationSetPropertiesDlg::InitLocationSetShape()
{
	// populate combo box with all fields associated with a location
	std::vector<std::wstring> fields = m_locationSetController->GetMetadataFields();
	std::vector<std::wstring>::iterator it;
	for(it = fields.begin(); it != fields.end(); ++it)
	{
		m_cboShapeField->Append(wxString((*it).c_str()));
	}

	if(!m_locationSetController->GetShapeField().empty())
	{
		m_cboShapeField->SetValue(m_locationSetController->GetShapeField().c_str());
	}
	else
	{
		if(!m_cboShapeField->IsEmpty())
			m_cboShapeField->SetValue(m_cboShapeField->GetString(0));
	}
		
	// Populate shape map combo box with all available shape maps
	m_shapeMapWidget->SetShapeMap(m_locationSetController->GetShapeMap());
	m_shapeMapWidget->PopulateShapeMapComboBox();
	m_shapeMapWidget->PopulateShapeComboBox(m_cboUniformShape);

	// Set field values
	wxCommandEvent dummy;
	OnShapeFieldChange(dummy);

	// Set uniform shape checkbox and shape
	VisualMarker::MARKER_SHAPE shape = m_locationSetController->GetUniformShape();
	m_cboUniformShape->SetValue(wxString(m_locationSetController->GetShapeMap()->ToStringW(shape).c_str()));

	if(!m_locationSetController->ModifiedShape())
	{
		m_chkUniformShape->SetValue(m_locationSetController->GetUniformShapeFlag());
		OnUniformShape(dummy);
	}
	else
	{
		m_chkUniformShape->Set3StateValue(wxCHK_UNDETERMINED);

		m_cboUniformShape->Enable(false);
		m_scrolledWindowShape->Enable(false);
		m_cboShapeField->Enable(false);
		m_cboShapeMap->Enable(false);
	}
}

void LocationSetPropertiesDlg::InitLocationSetSize()
{
	// populate combo box with all fields associated with a location
	std::vector<std::wstring> fields = m_locationSetController->GetNumericMetadataFields();
	std::vector<std::wstring>::iterator it;
	for(it = fields.begin(); it != fields.end(); ++it)
	{
		m_cboSizeField->Append(wxString((*it).c_str()));
	}

	if(!m_locationSetController->GetSizeField().empty())
	{
		m_cboSizeField->SetValue(m_locationSetController->GetSizeField().c_str());
	}
	else
	{
		if(!m_cboSizeField->IsEmpty())
			m_cboSizeField->SetValue(m_cboSizeField->GetString(0));
	}

	// Set min and max values
	m_txtMinLocationSize->SetValue(wxString(StringTools::ToStringW(m_locationSetController->GetMinLocationSize(),2).c_str()));
	m_txtMaxLocationSize->SetValue(wxString(StringTools::ToStringW(m_locationSetController->GetMaxLocationSize(),2).c_str()));
	
	if(m_locationSetController->ModifiedSize())
	{
		m_cboSizeField->Append(_T("<individually set>"));
		m_cboSizeField->SetValue(_T("<individually set>"));
	}
}

void LocationSetPropertiesDlg::InitLocationSetLabel()
{
	// populate combo box with all fields associated with a location
	std::vector<std::wstring> fields = m_locationSetController->GetMetadataFields();
	std::vector<std::wstring>::iterator it;
	for(it = fields.begin(); it != fields.end(); ++it)
		m_cboLabelField->Append(wxString((*it).c_str()));

	if(!m_locationSetController->GetColourField().empty())
		m_cboLabelField->SetValue(m_locationSetController->GetLabelField().c_str());
	else
	{
		if(!m_cboLabelField->IsEmpty())
			m_cboLabelField->SetValue(m_cboLabelField->GetString(0));
	}

	if(!m_locationSetController->ModifiedLabelSize())
		m_spinFontSize->SetValue(m_locationSetController->GetLabelSize());
	else
		m_spinFontSize->SetValue(-1);

	if(!m_locationSetController->ModifiedLabelColour())
	{
		Colour colour = m_locationSetController->GetLabelColour();
		m_colourLabel->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt()));
		ReplaceColourPicker( m_colourLabel, colour );
	}
	else
	{
        Colour colour(0,0,0);
		m_colourLabel->SetColour(wxColour(0,0,0,0));
		ReplaceColourPicker( m_colourLabel, colour );
	}

	if(!m_locationSetController->ModifiedLabelPosition())
	{
		m_cboLabelPosition->SetValue(wxString(m_locationSetController->GetLabelPosition().c_str()));
	}
	else
	{
		m_cboLabelPosition->Append(_T("<individually set>"));
		m_cboLabelPosition->SetValue(_T("<individually set>"));
	}

	if(!m_locationSetController->ModifiedLabelStyle())
	{
		m_cboLabelStyle->SetValue(wxString(m_locationSetController->GetLabelStyle().c_str()));
	}
	else
	{
		m_cboLabelStyle->Append(_T("<individually set>"));
		m_cboLabelStyle->SetValue(_T("<individually set>"));
	}

	if(!m_locationSetController->ModifiedLabelVisibility())
	{
		m_chkShowLabels->SetValue(m_locationSetController->GetLabelVisibility());
	}
	else
	{
		m_chkShowLabels->Set3StateValue(wxCHK_UNDETERMINED);
	}

	m_chkBindToCharts->SetValue(m_locationSetController->GetLabelBindToChart());

	wxCommandEvent dummy;
	OnShowLabels(dummy);
}

void LocationSetPropertiesDlg::InitChart()
{
	wxBusyCursor wait;

	ChartViewPtr chartView = m_locationSetLayer->GetLocationLayer(0)->GetLocationController()->GetChartView();
	
	// populate field combo box with all possible fields
	if(App::Inst().GetLayerTreeController()->GetNumSequenceLayers() > 0)
	{
		// populate combo box with all fields associated with a sequence
		std::map<std::wstring,std::wstring> data = m_locationSetController->GetSequenceMetadata();
		std::map<std::wstring,std::wstring>::iterator it;
		for(it = data.begin(); it != data.end(); ++it)
		{
			if(StringTools::ToLower((*it).first.c_str()) != _T("site id")
					&& StringTools::ToLower((*it).first.c_str()) != _T("sequence id"))
			{
				m_cboChartField->Append(wxString((*it).first.c_str()));
			}
		}

		foreach(const std::wstring& field,	App::Inst().GetLayerTreeController()->GetSequenceLayer(0)->GetSequenceController()->GetNumericMetadataFields())
			m_cboQuantitativeField->Append(wxString(field.c_str()));

		if(!chartView->GetField().empty())
			m_cboChartField->SetValue(wxString(chartView->GetField().c_str()));
		else
		{
			if(!m_cboChartField->IsEmpty())
				m_cboChartField->SetValue(m_cboChartField->GetString(0));
		}

		if(!chartView->GetQuantitativeField().empty())
			m_cboQuantitativeField->SetValue(wxString(chartView->GetQuantitativeField().c_str()));
		else
		{
			if(!m_cboQuantitativeField->IsEmpty())
				m_cboQuantitativeField->SetValue(m_cboQuantitativeField->GetString(0));
		}

		// Populate colour map combo box with all available colour maps
		m_chartColourMapWidget->SetColourMap(chartView->GetColourMap());
		m_chartColourMapWidget->PopulateColourMapComboBox();

		// Set all controls
		if(chartView->GetChartType() == ChartView::PIE_CHART_2D)
		{
			m_chkChart2D->SetValue(true);
			m_cboChartType->SetValue(wxT("Pie Chart"));
		}
		else if(chartView->GetChartType() == ChartView::PIE_CHART_3D)
		{
			m_chkChart2D->SetValue(false);
			m_cboChartType->SetValue(wxT("Pie Chart"));
		}
		else if(chartView->GetChartType() == ChartView::BAR_CHART_2D)
		{
			m_chkChart2D->SetValue(true);
			m_cboChartType->SetValue(wxT("Bar Chart"));
		}
		else if(chartView->GetChartType() == ChartView::BAR_CHART_3D)
		{
			m_chkChart2D->SetValue(false);
			m_cboChartType->SetValue(wxT("Bar Chart"));
		}

		m_spinChartWidth->SetValue((int)chartView->GetWidth());
		m_spinChartHeight->SetValue((int)chartView->GetHeight());

		m_spinChartBorderSize->SetValue((int)chartView->GetBorderSize());

		m_chkShowElementBorders->SetValue(chartView->GetWedgeBorders());

		m_chkBottomBorder->SetValue(chartView->IsBottomBorder());
		m_chkLeftBorder->SetValue(chartView->IsLeftBorder());
		m_chkTopBorder->SetValue(chartView->IsTopBorder());
		m_chkRightBorder->SetValue(chartView->IsRightBorder());

		Colour bgColour = chartView->GetBackgroundColour();
		m_colourChartBackground->SetColour(wxColour(bgColour.GetRedInt(), bgColour.GetGreenInt(), bgColour.GetBlueInt()));
		ReplaceColourPicker( m_colourChartBackground, bgColour );
		if(bgColour.GetAlphaInt() == 0)
			m_chkTransparentBackground->SetValue(true);
		else
			m_chkTransparentBackground->SetValue(false);

		Colour borderColour = chartView->GetBorderColour();
		m_colourChartBorder->SetColour(wxColour(borderColour.GetRedInt(), borderColour.GetGreenInt(), borderColour.GetBlueInt()));
		ReplaceColourPicker( m_colourChartBorder, borderColour );

		m_chkAssignLocationColour->SetValue(chartView->IsAssignBorderLocationColour());

		m_chkShowCharts->SetValue(chartView->IsVisible());

		m_chkChartSizeBySeqCount->SetValue(chartView->GetSizeProportionalToSeq());
		m_txtChartMinSize->SetValue(wxString(StringTools::ToStringW(chartView->GetMinSize()).c_str()));
		m_txtChartMaxSize->SetValue(wxString(StringTools::ToStringW(chartView->GetMaxSize()).c_str()));

		m_spinChartFilterTaxaPercentage->SetValue((int)chartView->GetAssignToOther());
		m_spinChartDropLineThickness->SetValue(chartView->GetDropLines()->GetThickness());

		Colour colour = chartView->GetDropLines()->GetColour();
		m_colourChartDropLine->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt()));
		ReplaceColourPicker( m_colourChartDropLine, colour );
		
		if(chartView->GetDropLines()->GetStyle() == VisualLine::SOLID)
			m_cboChartDropLineStyle->SetValue(_T("Solid"));
		else if(chartView->GetDropLines()->GetStyle() == VisualLine::SHORT_DASH)
			m_cboChartDropLineStyle->SetValue(_T("Short dash"));
		else if(chartView->GetDropLines()->GetStyle() == VisualLine::LONG_DASH)
			m_cboChartDropLineStyle->SetValue(_T("Long dash"));
		else if(chartView->GetDropLines()->GetStyle() == VisualLine::HIDDEN)
			m_cboChartDropLineStyle->SetValue(_T("Hidden"));

		m_chkQuantitative->SetValue(chartView->GetQuantitative());
		
		// Set field values
		wxCommandEvent dummy;
		OnChartFieldChange(dummy);
		OnQuantitative(dummy);

		// enable all controls since there is sequence data
		m_cboChartField->Enable();
		m_cboChartColourMap->Enable();
		m_cboChartType->Enable();
		m_spinChartWidth->Enable();
		m_spinChartHeight->Enable();
		m_spinChartBorderSize->Enable();
		m_colourChartBorder->Enable();
		m_chkAssignLocationColour->Enable();
		m_chkShowElementBorders->Enable();
		m_chkShowCharts->Enable();
		m_cboChartDropLineStyle->Enable();
		m_spinChartDropLineThickness->Enable();
		#ifdef WIN32
		m_colourChartDropLine->Enable();
		#else
		EnableButton( m_colourChartDropLine, true );
		#endif
		m_spinChartFilterTaxaPercentage->Enable();
		m_chkChartSizeBySeqCount->Enable();
	}
	else
	{
		// disable all controls since there is no sequence data
		m_cboChartField->Enable(false);
		m_cboChartColourMap->Enable(false);
		m_cboChartType->Enable(false);
		m_spinChartWidth->Enable(false);
		m_spinChartHeight->Enable(false);
		m_spinChartBorderSize->Enable(false);
		m_colourChartBorder->Enable(false);
		m_chkAssignLocationColour->Enable(false);
		m_chkShowElementBorders->Enable(false);
		m_chkShowCharts->Enable(false);
		m_cboChartDropLineStyle->Enable(false);
		m_spinChartDropLineThickness->Enable(false);
		#ifdef WIN32
		m_colourChartDropLine->Enable( false );
		#else
		EnableButton( m_colourChartDropLine, false );
		#endif
		m_spinChartFilterTaxaPercentage->Enable(false);
		m_chkChartSizeBySeqCount->Enable(false);

		m_chkChart2D->SetValue(true);
		m_cboChartType->SetValue(wxT("Pie Chart"));
	}
  
	wxCommandEvent dummy;
	OnProportionalChartSize(dummy);
	OnChartTypeChanged(dummy);
}

void LocationSetPropertiesDlg::InitMetadata()
{
	m_txtLayerSource->SetValue(wxString(m_locationSetLayer->GetPath().c_str()) + _T("\\") + wxString(m_locationSetLayer->GetFilename().c_str()));

	uint numLocationLayers = m_locationSetController->GetNumLocationLayers();
	m_txtMetaData->BeginBold();
	m_txtMetaData->AppendText(_T("Number of Sites:\n"));
	m_txtMetaData->EndBold();
	m_txtMetaData->AppendText(wxString((StringTools::ToStringW(numLocationLayers) + _T("\n\n")).c_str()));
}

void LocationSetPropertiesDlg::OnColourFieldChange( wxCommandEvent& event ) 
{ 
	std::vector<std::wstring> fieldValues;
	GetSortedFieldValues(m_cboColourField->GetValue().c_str(), fieldValues);

	m_colourMapWidget->SetFieldValues(m_scrolledWindowColour, fieldValues);
}

void LocationSetPropertiesDlg::OnColourMapChange( wxCommandEvent& event ) 
{ 
	m_colourMapWidget->SetColourMap(); 
}

void LocationSetPropertiesDlg::OnUniformColour( wxCommandEvent& event )
{
	#ifdef WIN32
	m_colourUniform->Enable(m_chkUniformColour->GetValue());
	#else
	EnableButton( m_colourUniform, m_chkUniformColour->GetValue() );
	#endif
	m_scrolledWindowColour->Enable(!m_chkUniformColour->GetValue());
	m_cboColourField->Enable(!m_chkUniformColour->GetValue());
	m_cboColourMap->Enable(!m_chkUniformColour->GetValue());
}

void LocationSetPropertiesDlg::OnUniformShape( wxCommandEvent& event )
{
	m_cboUniformShape->Enable(m_chkUniformShape->GetValue());
	m_scrolledWindowShape->Enable(!m_chkUniformShape->GetValue());
	m_cboShapeField->Enable(!m_chkUniformShape->GetValue());
	m_cboShapeMap->Enable(!m_chkUniformShape->GetValue());
}

void LocationSetPropertiesDlg::OnShapeFieldChange( wxCommandEvent& event ) 
{ 
	std::vector<std::wstring> fieldValues;
	GetSortedFieldValues(m_cboShapeField->GetValue().c_str(), fieldValues);

	m_shapeMapWidget->SetFieldValues(m_scrolledWindowShape, fieldValues);
}

void LocationSetPropertiesDlg::OnShapeMapChange( wxCommandEvent& event ) 
{ 
	m_shapeMapWidget->SetShapeMap(); 
}

void LocationSetPropertiesDlg::OnChartFieldChange( wxCommandEvent& event ) 
{ 
	// get all unique field values for the given field
	bool bAccept = true;
	std::set<std::wstring> uniqueFieldValues;
	for(unsigned int i = 0; i < m_locationSetLayer->GetNumLocationLayers(); ++i)
	{
		ChartViewPtr chartView = m_locationSetLayer->GetLocationLayer(i)->GetLocationController()->GetChartView();

		std::set<std::wstring> fieldValues;
		chartView->GetValues(m_cboChartField->GetValue().c_str(), fieldValues);

		if ( !m_cboQuantitativeField->IsEmpty() )
			chartView->SetQuantitativeField(m_cboQuantitativeField->GetValue().c_str());
		chartView->SetQuantitative(m_chkQuantitative->IsChecked());

		bAccept = bAccept && chartView->UpdateChart(m_cboChartField->GetValue().c_str(), fieldValues);

		foreach(const std::wstring& value, fieldValues)
		{
			if(chartView->GetPercentage(value) > float(m_spinChartFilterTaxaPercentage->GetValue())/100)
				uniqueFieldValues.insert(value);
		}
	}

	if(!bAccept)
		wxMessageBox(wxT("None numeric type found in specific quantitative field."), wxT("Abundance count error"), wxOK | wxICON_INFORMATION);

	std::vector<std::wstring> fieldValues(uniqueFieldValues.begin(), uniqueFieldValues.end());

	SortFieldValues(fieldValues);

	fieldValues.push_back(_T("Other"));

	m_chartColourMapWidget->SetFieldValues(m_scrolledWindowChart, fieldValues);
}

void LocationSetPropertiesDlg::OnChartColourMapChange( wxCommandEvent& event ) 
{ 
	m_chartColourMapWidget->SetColourMap(); 
}

void LocationSetPropertiesDlg::OnProportionalChartSize( wxCommandEvent& event )
{
	m_spinChartWidth->Enable(!m_chkChartSizeBySeqCount->GetValue());

	m_txtChartMinSize->Enable(m_chkChartSizeBySeqCount->GetValue());
	m_txtChartMaxSize->Enable(m_chkChartSizeBySeqCount->GetValue());
}

void LocationSetPropertiesDlg::GetSortedFieldValues(const std::wstring& field, std::vector<std::wstring>& fieldValues)
{
	// get all unique field values within the given field
	std::set<std::wstring> uniqueFieldValues;
	for(unsigned int i = 0; i < m_locationSetLayer->GetNumLocationLayers(); ++i)
	{
		std::map<std::wstring,std::wstring> data = m_locationSetLayer->GetLocationLayer(i)->GetLocationController()->GetData();

		std::map<std::wstring,std::wstring>::const_iterator it = data.find(field);
		uniqueFieldValues.insert(it->second);
	}

	fieldValues.clear();
	fieldValues = std::vector<std::wstring>(uniqueFieldValues.begin(), uniqueFieldValues.end());

	SortFieldValues(fieldValues);
}

void LocationSetPropertiesDlg::SortFieldValues(std::vector<std::wstring>& fieldValues)
{
	// if all data is numerical, then sort in numerical as opposed to lexigraphical order
	bool bNumeric = true;
	foreach(std::wstring str, fieldValues)
	{
		if(!StringTools::IsDecimalNumber(str))
		{
			bNumeric = false;
			break;
		}
	}

	// sort field values eith numerically or lexigraphically
	if(bNumeric)
	{
		// convert all data to its numeric equivalent, sort, and put back into string vector
		// Note: this somewhate convoluted way of doing a numerical sort of strings is done to
		// ensure that at the end the field values strings are exactly as they originally appeared.
		// Any added training zeros or rounding may cause problems later on when exact string matching
		// is attempted.
		std::vector<NumericStringSorter> fieldNumeric;
		for(uint i = 0; i < fieldValues.size(); ++i)
		{
			fieldNumeric.push_back(NumericStringSorter(StringTools::ToDouble(fieldValues.at(i)), i));
		}

		std::sort(fieldNumeric.begin(), fieldNumeric.end(), NumericStringSorter::NumericStringPredicate); 

		std::vector<std::wstring> sortedFieldValues;
		foreach(NumericStringSorter numeric, fieldNumeric)
		{
			sortedFieldValues.push_back(fieldValues.at(numeric.index));
		}

		fieldValues = sortedFieldValues;
	}
	else
	{
		// sort in lexigraphical order
		std::sort(fieldValues.begin(), fieldValues.end());
	}
}

void LocationSetPropertiesDlg::Apply() 
{ 
	ApplyGeneral(); 

	ApplyLocationSetColour(); 
	ApplyLocationSetShape(); 
	ApplyLocationSetSize(); 
	ApplyLocationLabel();

	ApplyChart();

	((GenGisFrame*)App::Inst().GetMainWindow())->FillSamplesLegend(); 
	((GenGisFrame*)App::Inst().GetMainWindow())->FillSequenceLegend();

	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	App::Inst().GetViewport()->Refresh( false );
}

void LocationSetPropertiesDlg::ApplyGeneral()
{
	m_locationSetLayer->SetName(m_txtLayerName->GetValue().c_str());
	m_locationSetLayer->SetDescription(m_txtLayerDescription->GetValue().c_str());
	m_locationSetLayer->SetAuthours(m_txtAuthours->GetValue().c_str());

	App::Inst().GetLayerTreeController()->SetName(m_locationSetLayer, m_locationSetLayer->GetName());
}

void LocationSetPropertiesDlg::ApplyLocationSetColour()
{
	// set location set properties
	if(m_chkUniformColour->Get3StateValue() != wxCHK_UNDETERMINED)
	{
		m_locationSetController->SetColourField(m_cboColourField->GetValue().c_str());
		m_locationSetController->SetColourMap(m_colourMapWidget->GetColourMap());
		m_locationSetController->SetUniformColourFlag(m_chkUniformColour->GetValue());
		m_locationSetController->SetUniformColour(Colour(m_colourUniform->GetColour()));

		m_locationSetController->SetLocationColours();
	}

	m_locationSetController->SetBorderSize(m_spinBorderSize->GetValue());
	m_locationSetController->SetBorderColour(Colour(m_colourBorders->GetColour()));

	m_locationSetController->SetLocationBorders();
}

void LocationSetPropertiesDlg::ApplyLocationSetShape()
{
	if(m_chkUniformShape->Get3StateValue() != wxCHK_UNDETERMINED)
	{
		ShapeMapPtr shapeMap = m_shapeMapWidget->GetShapeMap();

		m_locationSetController->SetShapeField(m_cboShapeField->GetValue().c_str());
		m_locationSetController->SetShapeMap(shapeMap);
		m_locationSetController->SetUniformShapeFlag(m_chkUniformShape->GetValue());
		m_locationSetController->SetUniformShape(shapeMap->ToShape(m_cboUniformShape->GetValue().c_str()));

		m_locationSetController->SetLocationShapes();
	}
}

void LocationSetPropertiesDlg::ApplyLocationSetSize()
{
	std::wstring str = m_cboSizeField->GetValue().c_str();
	if(str != _T("<individually set>"))
	{
		m_locationSetController->SetSizeField(m_cboSizeField->GetValue().c_str());
		m_locationSetController->SetMinLocationSize(StringTools::ToDouble(m_txtMinLocationSize->GetValue().c_str()));
		m_locationSetController->SetMaxLocationSize(StringTools::ToDouble(m_txtMaxLocationSize->GetValue().c_str()));

		m_locationSetController->SetLocationSizes();
	}
}

void LocationSetPropertiesDlg::ApplyLocationLabel()
{
	m_locationSetController->SetLabelField(m_cboLabelField->GetValue().c_str());
	m_locationSetController->SetLabelBindToChart(m_chkBindToCharts->GetValue());

	bool bUpdateLabelVisibility = false;
	if(m_chkShowLabels->Get3StateValue() != wxCHK_UNDETERMINED)
	{
		m_locationSetController->SetLabelVisibility(m_chkShowLabels->GetValue());
		bUpdateLabelVisibility = true;
	}

	bool bUpdateLabelSize = false;
	if(m_spinFontSize->GetValue() != -1)
	{
		m_locationSetController->SetLabelSize(m_spinFontSize->GetValue());
		bUpdateLabelSize = true;
	}

	bool bUpdateLabelPosition = false;
	std::wstring str = m_cboLabelPosition->GetValue().c_str();
	if(str != _T("<individually set>"))
	{
		m_locationSetController->SetLabelPosition(m_cboLabelPosition->GetValue().c_str());		
		bUpdateLabelPosition = true;
	}

	bool bUpdateLabelStyle = false;
	str = m_cboLabelStyle->GetValue().c_str();
	if(str != _T("<individually set>"))
	{
		m_locationSetController->SetLabelStyle(m_cboLabelStyle->GetValue().c_str());		
		bUpdateLabelStyle = true;
	}

	bool bUpdateLabelColour = false;
	if(m_colourLabel->GetColour() != wxColour(0, 0, 0, 0))
	{
		m_locationSetController->SetLabelColour(Colour(m_colourLabel->GetColour()));
		bUpdateLabelColour = true;
	}
		
	m_locationSetController->SetLocationLabels(bUpdateLabelSize, bUpdateLabelPosition, bUpdateLabelStyle, bUpdateLabelColour, bUpdateLabelVisibility);
}

void LocationSetPropertiesDlg::ApplyChart()
{
	// set properties based on state of controls in Chart page
	if(m_locationSetLayer->IsSequencesData())
	{
		for(unsigned int i = 0; i < m_locationSetLayer->GetNumLocationLayers(); ++i)
		{
			LocationControllerPtr locationController = m_locationSetLayer->GetLocationLayer(i)->GetLocationController();
			ChartViewPtr chartView = locationController->GetChartView();

			if ( !m_cboQuantitativeField->IsEmpty() )
				chartView->SetQuantitativeField(m_cboQuantitativeField->GetValue().c_str());
			chartView->SetQuantitative(m_chkQuantitative->IsChecked());

			chartView->SetColourMap(m_chartColourMapWidget->GetColourMap());

			LocationViewPtr locationView = locationController->GetLocationView();
			chartView->SetLabel(locationView->GetLabel());
			chartView->SetLabelBindToChart(locationView->GetLabelBindToChart());

			if(m_cboChartType->GetValue() == wxT("Pie Chart") && m_chkChart2D->IsChecked())
				chartView->SetChartType(ChartView::PIE_CHART_2D);
			else if(m_cboChartType->GetValue() == wxT("Pie Chart") && !m_chkChart2D->IsChecked())
				chartView->SetChartType(ChartView::PIE_CHART_3D);
			else if(m_cboChartType->GetValue() == wxT("Bar Chart") && m_chkChart2D->IsChecked())
				chartView->SetChartType(ChartView::BAR_CHART_2D);
			else if(m_cboChartType->GetValue() == wxT("Bar Chart") && !m_chkChart2D->IsChecked())
				chartView->SetChartType(ChartView::BAR_CHART_3D);

			chartView->SetWidth(m_spinChartWidth->GetValue());
			chartView->SetHeight(m_spinChartHeight->GetValue());
		
			chartView->SetBorderSize(m_spinChartBorderSize->GetValue());

			chartView->SetWedgeBorders(m_chkShowElementBorders->GetValue());

			chartView->SetVisibility(m_chkShowCharts->GetValue());

			chartView->SetAssignToOther(m_spinChartFilterTaxaPercentage->GetValue());
			chartView->SetSizeProportionalToSeq(m_chkChartSizeBySeqCount->GetValue());

			chartView->SetMinSize(StringTools::ToInt(m_txtChartMinSize->GetValue().c_str()));
			chartView->SetMaxSize(StringTools::ToInt(m_txtChartMaxSize->GetValue().c_str()));

			chartView->SetBottomBorder(m_chkBottomBorder->IsChecked());
			chartView->SetLeftBorder(m_chkLeftBorder->IsChecked());
			chartView->SetTopBorder(m_chkTopBorder->IsChecked());
			chartView->SetRightBorder(m_chkRightBorder->IsChecked());
			
			wxColour colour = m_colourChartBackground->GetColour();
			if(m_chkTransparentBackground->IsChecked())
				colour.Set(colour.Red(), colour.Green(), colour.Blue(), wxALPHA_TRANSPARENT);
			chartView->SetBackgroundColour(Colour(colour));

			chartView->SetAssignBorderLocationColour(m_chkAssignLocationColour->IsChecked());
			chartView->SetBorderColour(Colour(m_colourChartBorder->GetColour()));

			chartView->GetDropLines()->SetThickness(m_spinChartDropLineThickness->GetValue());
			chartView->GetDropLines()->SetColour(Colour(m_colourChartDropLine->GetColour()));
		
			if(m_cboChartDropLineStyle->GetValue() == _T("Solid"))
				chartView->GetDropLines()->SetStyle(VisualLine::SOLID);
			else if(m_cboChartDropLineStyle->GetValue() == _T("Short dash"))
				chartView->GetDropLines()->SetStyle(VisualLine::SHORT_DASH);
			else if(m_cboChartDropLineStyle->GetValue() == _T("Long dash"))
				chartView->GetDropLines()->SetStyle(VisualLine::LONG_DASH);
			else if(m_cboChartDropLineStyle->GetValue() == _T("Hidden"))
				chartView->GetDropLines()->SetStyle(VisualLine::HIDDEN);
		}

		m_locationSetLayer->GetChartSetView()->UpdateCharts(m_cboChartField->GetValue().c_str());
	}
}

void LocationSetPropertiesDlg::OnSpinAssignTaxa( wxSpinEvent& event )
{
	wxCommandEvent dummy;
	OnChartFieldChange(dummy);
}

void LocationSetPropertiesDlg::OnShowLabels( wxCommandEvent& event )
{
	bool bEnable = (m_chkShowLabels->Get3StateValue() == wxCHK_CHECKED || m_chkShowLabels->Get3StateValue() == wxCHK_UNDETERMINED);
	m_cboLabelField->Enable(bEnable);
	m_spinFontSize->Enable(bEnable);
	m_cboLabelPosition->Enable(bEnable);
	m_cboLabelStyle->Enable(bEnable);
	#ifdef WIN32
	m_colourLabel->Enable(bEnable);
	#else
	EnableButton( m_colourLabel, bEnable );
	#endif
}

void LocationSetPropertiesDlg::OnQuantitative( wxCommandEvent& event )
{
	LocationSetPropertiesDlg::m_cboQuantitativeField->Enable(m_chkQuantitative->IsChecked());

	wxCommandEvent dummy;
	OnChartFieldChange(dummy);
}

void LocationSetPropertiesDlg::OnQuantitativeFieldChanged( wxCommandEvent& event )
{
	wxCommandEvent dummy;
	OnChartFieldChange(dummy);
}

void LocationSetPropertiesDlg::OnChartTypeChanged( wxCommandEvent& event ) 
{ 
	if(m_cboChartType->GetValue() == _T("Pie Chart"))
	{
		m_lblChartWidth->SetLabel(_T("Diameter:"));
		m_lblChartHeight->Show(false);
		m_spinChartHeight->Show(false);

		m_lineColourChartBackground->Show(false);
		m_lblChartBgColour->Show(false);
		
		#ifdef WIN32
		m_colourChartBackground->Show( false );
		#else
		ShowButton( m_colourChartBackground, false );
		#endif
		m_chkTransparentBackground->Show(false);

		m_chkBottomBorder->Show(false);
		m_chkLeftBorder->Show(false);
		m_chkTopBorder->Show(false);
		m_chkRightBorder->Show(false);

		m_lineAssignTaxaToOther->Show(true);
		m_lblChartAssignToOther->Show(true);
		m_spinChartFilterTaxaPercentage->Show(true);
	}
	else if(m_cboChartType->GetValue() == _T("Bar Chart"))
	{
		m_lblChartWidth->SetLabel(_T("Width:"));
		m_lblChartHeight->Show(true);
		m_spinChartHeight->Show(true);

		m_lineColourChartBackground->Show(true);
		m_lblChartBgColour->Show(true);
		#ifdef WIN32
		m_colourChartBackground->Show( true );
		#else
		ShowButton( m_colourChartBackground, true );
		#endif
		m_chkTransparentBackground->Show(true);

		m_chkBottomBorder->Show(true);
		m_chkLeftBorder->Show(true);
		m_chkTopBorder->Show(true);
		m_chkRightBorder->Show(true);

		m_lineAssignTaxaToOther->Show(false);
		m_lblChartAssignToOther->Show(false);
		m_spinChartFilterTaxaPercentage->Show(false);
	}

	m_pnlChartSymbology->Fit();
	
	event.Skip(); 
}

void LocationSetPropertiesDlg::ShowButton( wxColourPickerCtrl* wxColourPicker, bool showStatus )
{
	if ( m_colourButton.find( wxColourPicker ) == m_colourButton.end() )
		ReplaceColourPicker( wxColourPicker, Colour( wxColourPicker->GetColour() ) );

	m_colourButton.find( wxColourPicker )->second->Show( showStatus );
}

void LocationSetPropertiesDlg::EnableButton( wxColourPickerCtrl* wxColourPicker, bool enableStatus )
{
	if ( m_colourButton.find( wxColourPicker ) == m_colourButton.end() )
		ReplaceColourPicker( wxColourPicker, Colour( wxColourPicker->GetColour() ) );

	m_colourButton.find( wxColourPicker )->second->Enable( enableStatus );
}

void LocationSetPropertiesDlg::ReplaceColourPicker( wxColourPickerCtrl* wxColourPicker, Colour colour )
{
	#ifndef WIN32
	wxColourPicker->Hide();
	CustomColourButton* customColourButton = new CustomColourButton( wxColourPicker->GetParent(), wxColourPicker,
		wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt()) );

	wxSizer* colourIconSizer = wxColourPicker->GetContainingSizer();
	colourIconSizer->Replace( wxColourPicker, customColourButton );
	colourIconSizer->Layout();

	customColourButton->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LocationSetPropertiesDlg::OnCustomColourButtonClicked ), NULL, this );
	m_colourButton[wxColourPicker] = customColourButton;
	#endif
}

void LocationSetPropertiesDlg::OnCustomColourButtonClicked( wxMouseEvent& event )
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
