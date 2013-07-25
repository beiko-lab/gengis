//=======================================================================
// Author: Somayyeh Zangooei
//
// Copyright 2013 Somayyeh Zangooei
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
#include "../core/MapController.hpp"
#include "../core/MapView.hpp"
#include "../core/MapTexture.hpp"
#include "../core/MapLayer.hpp"
#include "../core/VectorMapLayer.hpp"
#include "../core/VectorMapController.hpp"


#include "../core/Viewport.hpp"
#include "../core/LayerTreeController.hpp"

#include "../gui/MapPropertiesDlg.hpp"
#include "../gui/VectorMapPropertiesDlg.hpp"

#include "../gui/ProgressDlg.hpp"
#include "../gui/TerrainMapWidget.hpp"

#include "../utils/StringTools.hpp"

using namespace GenGIS;

const int MAX_ELEVATION = 10000;

VectorMapPropertiesDlg::VectorMapPropertiesDlg(wxWindow* parent, VectorMapLayerPtr vectorMapLayer) :
	VectorMapPropertiesLayout(parent),
	m_vectorMapLayer(vectorMapLayer)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	// Limit the properties dialog to a single instance
	m_vectorMapLayer->SetPropertiesDialog( this );

	InitGeneral();
	InitMetaData();
	InitPointSymbology();
	InitPolyLineSymbology();
	InitPolygonSymbology();

	Fit();
}

VectorMapPropertiesDlg::~VectorMapPropertiesDlg()
{
	// Tell the layer that the properties dialog is being closed
	m_vectorMapLayer->SetPropertiesDialogOpenStatus( false );
	m_vectorMapLayer->SetPropertiesDialog( NULL );
}

void VectorMapPropertiesDlg::InitGeneral()
{
	// set the title of the properties dialog
	this->SetLabel( wxString(m_vectorMapLayer->GetName().c_str()) + wxT( " : Vector Map Properties" ) );	
	// set state of controls on General page
	m_txtLayerName->SetValue(wxString(m_vectorMapLayer->GetName().c_str()));
	m_txtLayerDescription->SetValue(wxString(m_vectorMapLayer->GetDescription().c_str()));
	m_txtAuthours->SetValue(wxString(m_vectorMapLayer->GetAuthours().c_str()));
}


void VectorMapPropertiesDlg::InitMetaData()
{	
	//VectorMapModelPtr vectorMapModel = m_vectorMapLayer->GetVectorMapController()->GetVectorMapModel();
	//VectorMapViewPtr vectorMapView = m_vectorMapLayer->GetVectorMapController()->GetVectorMapView();	
	m_txtLayerSource->SetValue(m_vectorMapLayer->GetFullPath());
	InitMeta_Data();
}

void VectorMapPropertiesDlg::InitPointSymbology()
{
	VectorMapViewPtr vectorMapView = m_vectorMapLayer->GetVectorMapController()->GetVectorMapView();

	Colour colour = vectorMapView->GetPointColour();
	m_FillPointColourPicker->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt()));
	ReplaceColourPicker( m_FillPointColourPicker, colour );
	m_TxtCtrlPointSize->SetValue(wxString(StringTools::ToStringW(vectorMapView->GetPointSize(), 2).c_str()));

	Colour bColour = vectorMapView->GetPointBorderColour();
	m_PointBorderColourPicker->SetColour(wxColour(bColour.GetRedInt(), bColour.GetGreenInt(), bColour.GetBlueInt(), bColour.GetAlphaInt()));
	ReplaceColourPicker( m_PointBorderColourPicker, bColour );
	m_TxtCtrlPointBorderThickness->SetValue(wxString(StringTools::ToStringW(vectorMapView->GetPointBorderSize(), 2).c_str()));
	
	VisualMarker::MARKER_SHAPE pointShape = vectorMapView->GetPointShape();
	if(pointShape == VisualMarker::CIRCLE)
		m_CboPointShape->SetValue(_T("Circle"));
	else if(pointShape == VisualMarker::CIRCLE_FAST)
		m_CboPointShape->SetValue(_T("Circle (fast)"));
	else if(pointShape == VisualMarker::SQUARE)
		m_CboPointShape->SetValue(_T("Square"));
	else if(pointShape == VisualMarker::TRIANGLE)
		m_CboPointShape->SetValue(_T("Triangle"));
	else if(pointShape == VisualMarker::STAR)
		m_CboPointShape->SetValue(_T("Star"));
	else if(pointShape == VisualMarker::PLUS_SIGN)
		m_CboPointShape->SetValue(_T("Plus sign"));
	else if(pointShape == VisualMarker::OCTAGON)
		m_CboPointShape->SetValue(_T("Octagon"));
	else if(pointShape== VisualMarker::DIAMOND)
		m_CboPointShape->SetValue(_T("Diamond"));
	else if(pointShape == VisualMarker::INVERTED_TRIANGLE)
		m_CboPointShape->SetValue(_T("Inverted triangle"));	
}

void VectorMapPropertiesDlg::InitPolyLineSymbology()
{
	VectorMapViewPtr vectorMapView = m_vectorMapLayer->GetVectorMapController()->GetVectorMapView();

	Colour colour = vectorMapView->GetLineColour();
	m_PolylineFillcolourPicker->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt()));
	ReplaceColourPicker( m_PolylineFillcolourPicker, colour );
	m_TxtCtrlPolylineThickness->SetValue(wxString(StringTools::ToStringW(vectorMapView->GetLineSize(), 2).c_str()));

	Colour bColour = vectorMapView->GetLineBorderColour();
	m_PolylineBordercolourPicker->SetColour(wxColour(bColour.GetRedInt(), bColour.GetGreenInt(), bColour.GetBlueInt(), bColour.GetAlphaInt()));
	ReplaceColourPicker( m_PolylineBordercolourPicker, bColour );
	m_TxtCtrlPolylineBorderThickness->SetValue(wxString(StringTools::ToStringW(vectorMapView->GetLineBorderSize(), 2).c_str()));
	
	VisualLine::LINE_STYLE lineStyle = vectorMapView->GetLineStyle();
	if(lineStyle == VisualLine::SOLID)
		m_CboPolylineStyle->SetValue(_T("Solid"));
	else if(lineStyle == VisualLine::SHORT_DASH)
		m_CboPolylineStyle->SetValue(_T("Short dash"));
	else if(lineStyle == VisualLine::LONG_DASH)
		m_CboPolylineStyle->SetValue(_T("Long dash"));
	else if(lineStyle == VisualLine::HIDDEN)
		m_CboPolylineStyle->SetValue(_T("Hidden"));	
}

void VectorMapPropertiesDlg::InitPolygonSymbology()
{
	VectorMapViewPtr vectorMapView = m_vectorMapLayer->GetVectorMapController()->GetVectorMapView();

	Colour colour = vectorMapView->GetPolygonBorderColour();
	m_PolygonBordercolourPicker->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt()));
	ReplaceColourPicker( m_PolygonBordercolourPicker, colour );
	m_TxtCtrlPolygonThickness->SetValue(wxString(StringTools::ToStringW(vectorMapView->GetPolygonBorderSize(), 2).c_str()));	
	
	VisualLine::LINE_STYLE polygonBStyle = vectorMapView->GetPolygonBorderStyle();
	if(polygonBStyle == VisualLine::SOLID)
		m_CboPolygonStyle->SetValue(_T("Solid"));
	else if(polygonBStyle == VisualLine::SHORT_DASH)
		m_CboPolygonStyle->SetValue(_T("Short dash"));
	else if(polygonBStyle == VisualLine::LONG_DASH)
		m_CboPolygonStyle->SetValue(_T("Long dash"));
	else if(polygonBStyle == VisualLine::HIDDEN)
		m_CboPolygonStyle->SetValue(_T("Hidden"));	
}

void VectorMapPropertiesDlg::ReplaceColourPicker( wxColourPickerCtrl* wxColourPicker, Colour& colour )
{
	#ifndef WIN32
	wxColourPicker->Hide();
	CustomColourButton* customColourButton = new CustomColourButton( wxColourPicker->GetParent(), wxColourPicker,
		wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt()) );

	wxSizer* colourIconSizer = wxColourPicker->GetContainingSizer();
	colourIconSizer->Replace( wxColourPicker, customColourButton );
	colourIconSizer->Layout();

	customColourButton->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( VectorMapPropertiesDlg::OnCustomColourButtonClicked ), NULL, this );
	#endif
}

void VectorMapPropertiesDlg::OnCustomColourButtonClicked( wxMouseEvent& event )
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


void VectorMapPropertiesDlg::InitMeta_Data()
{
	vFileMetaData* vectorMetaData = m_vectorMapLayer->GetVectorMapController()->GetVectorMetaData();
	
	m_txtMetaData->BeginBold();
	m_txtMetaData->AppendText(_T("\nOriginal Projection:\n"));
	m_txtMetaData->EndBold();
	m_txtMetaData->AppendText(wxString(StringTools::ToStringW(vectorMetaData->originalProjection).c_str()) + _T("\n"));

	m_txtMetaData->BeginBold();
	m_txtMetaData->AppendText(_T("\nCurrent Projection:\n"));
	m_txtMetaData->EndBold();
	m_txtMetaData->AppendText(wxString(StringTools::ToStringW(vectorMetaData->destinationProjection).c_str())+_T("\n"));

	m_txtMetaData->BeginBold();
	m_txtMetaData->AppendText(_T("\nGeometry Type of the Features:\n"));
	m_txtMetaData->EndBold();
	m_txtMetaData->AppendText(wxString(StringTools::ToStringW(vectorMetaData->geometryType).c_str())+_T("\n"));

	m_txtMetaData->BeginBold();
	m_txtMetaData->AppendText(_T("\nNumber of Features:\n"));
	m_txtMetaData->EndBold();
	m_txtMetaData->AppendText(wxString(StringTools::ToStringW(vectorMetaData->numFeatures).c_str())+_T("\n"));

	m_txtMetaData->BeginBold();
	m_txtMetaData->AppendText(_T("\nOriginal Extents:\n"));
	m_txtMetaData->EndBold();
	m_txtMetaData->AppendText(_T("Lower, left corner: ") + wxString(StringTools::ToStringW(vectorMetaData->originalExtents.x, 2).c_str()) + _T(", ") 
		+ wxString(StringTools::ToStringW(vectorMetaData->originalExtents.y, 2).c_str()) + _T("\n"));
	m_txtMetaData->AppendText(_T("Upper, right corner: ") + wxString(StringTools::ToStringW(vectorMetaData->originalExtents.dx, 2).c_str()) + _T(", ") 
		+ wxString(StringTools::ToStringW(vectorMetaData->originalExtents.dy, 2).c_str()) + _T("\n"));

	m_txtMetaData->BeginBold();
	m_txtMetaData->AppendText(_T("\nCurrent Extents:\n"));
	m_txtMetaData->EndBold();
	m_txtMetaData->AppendText(_T("Lower, left corner: ") + wxString(StringTools::ToStringW(vectorMetaData->currentExtents.x, 2).c_str()) + _T(", ") 
		+ wxString(StringTools::ToStringW(vectorMetaData->currentExtents.y, 2).c_str()) + _T("\n"));
	m_txtMetaData->AppendText(_T("Upper, right corner: ") + wxString(StringTools::ToStringW(vectorMetaData->currentExtents.dx, 2).c_str()) + _T(", ") 
		+ wxString(StringTools::ToStringW(vectorMetaData->currentExtents.dy, 2).c_str()) + _T("\n"));

}

void VectorMapPropertiesDlg::Apply(){
	
	// set properties based on state of controls in General page
	m_vectorMapLayer->SetName(m_txtLayerName->GetValue().c_str());
	m_vectorMapLayer->SetDescription(m_txtLayerDescription->GetValue().c_str());
	m_vectorMapLayer->SetAuthours(m_txtAuthours->GetValue().c_str());
	
	ApplyPointSymbology();
	ApplyPolyLineSymbology();	

	//VectorMapViewPtr vectorMapView = m_vectorMapLayer->GetVectorMapController()->GetVectorMapView();
	//double polygonThickness= StringTools::ToDouble(m_TxtCtrlPolygonThickness->GetValue().c_str());
	/*
	if((float)vectorMapView->GetPolygonBorderSize()!= (float)polygonThickness)
	{
		
		if( polygonThickness < vectorMapView->GetPolygonMinWidthSize() || polygonThickness > vectorMapView->GetPolygonMaxWidthSize() )
		{
			int answer = wxMessageBox( wxT( "Thicknesses less than 1 and greater than 10 take longer time to be rendered. Would you still like to proceed?"),
						wxT( "Confirmation" ), wxYES_NO );
			if ( answer == wxYES)
				ApplyPolygonSymbology();
		}
		else
			ApplyPolygonSymbology();
	}
	else
		ApplyPolygonSymbology();
		*/
	ApplyPolygonSymbology();
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );	
	App::Inst().GetLayerTreeController()->SetName(m_vectorMapLayer, m_vectorMapLayer->GetName());
	App::Inst().GetViewport()->Refresh( false );
}


void VectorMapPropertiesDlg::ApplyPointSymbology()
{
	VectorMapViewPtr vectorMapView = m_vectorMapLayer->GetVectorMapController()->GetVectorMapView();

	// set properties based on state of controls in Symbology page
	Colour colour = Colour(m_FillPointColourPicker->GetColour());
	Colour bColour = Colour(m_PointBorderColourPicker->GetColour());

	if(colour != vectorMapView->GetPointColour())
	{
		vectorMapView->SetPointColour(colour);
		
	}
	if(bColour != vectorMapView->GetPointBorderColour())
	{
		vectorMapView->SetPointBorderColour(bColour);
	}

	if(vectorMapView->GetPointSize() != m_TxtCtrlPointSize->GetValue())
	{
		//vectorMapView->SetPointSize(m_TxtCtrlPointSize->GetValue());
		vectorMapView->SetPointSize(StringTools::ToDouble(m_TxtCtrlPointSize->GetValue().c_str()));

	}
	if(vectorMapView->GetPointBorderSize() != m_TxtCtrlPointBorderThickness->GetValue())
	{
		//vectorMapView->SetPointBorderSize(m_CtrlPointBorderThickness->GetValue());
		vectorMapView->SetPointBorderSize(StringTools::ToDouble(m_TxtCtrlPointBorderThickness->GetValue().c_str()));
		
	}
	
	VisualMarker::MARKER_SHAPE pShape;
	if(m_CboPointShape->GetValue() == _T("Circle"))
		pShape = VisualMarker::CIRCLE;
	else if(m_CboPointShape->GetValue() == _T("Circle (fast)"))
		pShape = VisualMarker::CIRCLE_FAST;
	else if(m_CboPointShape->GetValue() == _T("Square"))
		pShape = VisualMarker::SQUARE;
	else if(m_CboPointShape->GetValue() == _T("Triangle"))
		pShape = VisualMarker::TRIANGLE;
	else if(m_CboPointShape->GetValue() == _T("Star"))
		pShape = VisualMarker::STAR;
	else if(m_CboPointShape->GetValue() == _T("Plus sign"))
		pShape = VisualMarker::PLUS_SIGN;
	else if(m_CboPointShape->GetValue() == _T("Octagon"))
		pShape = VisualMarker::OCTAGON;
	else if(m_CboPointShape->GetValue() == _T("Diamond"))
		pShape = VisualMarker::DIAMOND;
	else if(m_CboPointShape->GetValue() == _T("Inverted triangle"))
		pShape = VisualMarker::INVERTED_TRIANGLE;

	if(vectorMapView->GetPointShape() != pShape)
	{
		vectorMapView->SetPointShape(pShape);
		//vectorMapView->SetShapeModified(true);
	}

	App::Inst().GetViewport()->Refresh(false);
}


void VectorMapPropertiesDlg::ApplyPolyLineSymbology()
{
	VectorMapViewPtr vectorMapView = m_vectorMapLayer->GetVectorMapController()->GetVectorMapView();

	// set properties based on state of controls in Symbology page
	Colour colour = Colour(m_PolylineFillcolourPicker->GetColour());
	Colour bColour = Colour(m_PolylineBordercolourPicker->GetColour());

	if(colour != vectorMapView->GetLineColour())
	{
		vectorMapView->SetLineColour(colour);		
	}
	if(bColour != vectorMapView->GetLineBorderColour())
	{
		vectorMapView->SetLineBorderColour(bColour);		
	}

	if(vectorMapView->GetLineSize() != m_TxtCtrlPolylineThickness->GetValue())
	{
		//vectorMapView->SetLineSize(m_TxtCtrlPolylineThickness->GetValue());
		vectorMapView->SetLineSize(StringTools::ToDouble(m_TxtCtrlPolylineThickness->GetValue().c_str()));		
		
	}
	if(vectorMapView->GetLineBorderSize() != m_TxtCtrlPolylineBorderThickness->GetValue())
	{
		vectorMapView->SetLineBorderSize(StringTools::ToDouble(m_TxtCtrlPolylineBorderThickness->GetValue().c_str()));		
	}
	
	VisualLine::LINE_STYLE lnStyle;
	if(m_CboPolylineStyle->GetValue() == _T("Hidden"))
		lnStyle = VisualLine::HIDDEN;
	else if(m_CboPolylineStyle->GetValue() == _T("Short dash"))
		lnStyle = VisualLine::SHORT_DASH;
	else if(m_CboPolylineStyle->GetValue() == _T("Long dash"))
		lnStyle = VisualLine::LONG_DASH;
	else if(m_CboPolylineStyle->GetValue() == _T("Solid"))
		lnStyle = VisualLine::SOLID;
	if(vectorMapView->GetLineStyle() != lnStyle)
	{
		vectorMapView->SetLineStyle(lnStyle);
		//vectorMapView->SetShapeModified(true);
	}

	App::Inst().GetViewport()->Refresh(false);
}


void VectorMapPropertiesDlg::ApplyPolygonSymbology()
{
	
	VectorMapViewPtr vectorMapView = m_vectorMapLayer->GetVectorMapController()->GetVectorMapView();
	// set properties based on state of controls in Symbology page
	Colour colour = Colour(m_PolygonBordercolourPicker->GetColour());

	if(colour != vectorMapView->GetPolygonBorderColour())
	{		
		vectorMapView->SetPolygonBorderColour(colour);
		//vectorMapView->SetPointColourModified(true);
	}
	if(vectorMapView->GetPolygonBorderSize() != m_TxtCtrlPolygonThickness->GetValue())
	{
		//vectorMapView->SetPolygonBorderSize(m_TxtCtrlPolygonThickness->GetValue());
		vectorMapView->SetPolygonBorderSize(StringTools::ToDouble(m_TxtCtrlPolygonThickness->GetValue().c_str()));
	}	

	VisualLine::LINE_STYLE polygonBStyle;
	if(m_CboPolygonStyle->GetValue() == _T("Hidden"))
		polygonBStyle = VisualLine::HIDDEN;
	else if(m_CboPolygonStyle->GetValue() == _T("Short dash"))
		polygonBStyle = VisualLine::SHORT_DASH;
	else if(m_CboPolygonStyle->GetValue() == _T("Long dash"))
		polygonBStyle = VisualLine::LONG_DASH;
	else if(m_CboPolygonStyle->GetValue() == _T("Solid"))
		polygonBStyle = VisualLine::SOLID;
	if(vectorMapView->GetPolygonBorderStyle() != polygonBStyle)
	{
		vectorMapView->SetPolygonBorderStyle(polygonBStyle);
		//vectorMapView->SetShapeModified(true);
	}

	App::Inst().GetViewport()->Refresh(false);
}


/** OK button event handler. */
void VectorMapPropertiesDlg::OnOK( wxCommandEvent& event )
{
	Apply();
	Destroy();
}

/** Apply button event handler. */
void VectorMapPropertiesDlg::OnApply( wxCommandEvent& event )
{
	Apply();
}

/** Cancel button event handler. */
void VectorMapPropertiesDlg::OnCancel( wxCommandEvent& event )
{
	Destroy();
}

/** Close dialog. */
void VectorMapPropertiesDlg::OnClose( wxCloseEvent& event )
{
	Destroy();
}
