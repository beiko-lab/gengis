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
#include "../core/LayoutObjectProperties.hpp"
#include "../core/Viewport.hpp"

#include "../gui/LayoutObjectPropertiesDlg.hpp"
#include "../gui/ColourPickerDlg.hpp"

#include "../widgets/CustomColourButton.hpp"

using namespace GenGIS;

LayoutObjectPropertiesDlg::LayoutObjectPropertiesDlg(wxWindow* parent): LayoutObjectPropertiesLayout(parent)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	Init();
	Fit();
}

LayoutObjectPropertiesDlg::~LayoutObjectPropertiesDlg()
{

}

void LayoutObjectPropertiesDlg::Init()
{
	// Object Layout Properties
	LayoutObjectPropertiesPtr layoutObjProp = App::Inst().GetLayoutObjProp();
	m_spinObjectLineThickness->SetValue(layoutObjProp->GetObjectLineThickness());
	
	Colour colour = layoutObjProp->GetObjectLineColour();
	m_colourObjectLine->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt()));
	#ifndef WIN32
	m_colourObjectLine->Hide();
	CustomColourButton* customColourButton = new CustomColourButton( m_colourObjectLine->GetParent(), m_colourObjectLine,
		wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt()) );

	wxSizer* colourIconSizer = m_colourObjectLine->GetContainingSizer();
	colourIconSizer->Replace( m_colourObjectLine, customColourButton );
	colourIconSizer->Layout();
	
	customColourButton->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LayoutObjectPropertiesDlg::OnCustomColourButtonClicked ), NULL, this );
	#endif

	if(layoutObjProp->GetObjectLineStyle() == VisualLine::HIDDEN)
		m_cboObjectLineStyle->SetValue(_T("Hidden"));
	else if(layoutObjProp->GetObjectLineStyle() == VisualLine::SOLID)
		m_cboObjectLineStyle->SetValue(_T("Solid"));
	else if(layoutObjProp->GetObjectLineStyle() == VisualLine::SHORT_DASH)
		m_cboObjectLineStyle->SetValue(_T("Short dash"));
	else if(layoutObjProp->GetObjectLineStyle() == VisualLine::LONG_DASH)
		m_cboObjectLineStyle->SetValue(_T("Long dash"));

	// Layout Line Properties
	m_spinLayoutLineThickness->SetValue(layoutObjProp->GetLayoutLineThickness());

	colour = layoutObjProp->GetLayoutLineColour();
	m_colourLayoutLine->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt()));
	#ifndef WIN32
	m_colourLayoutLine->Hide();
	customColourButton = new CustomColourButton( m_colourLayoutLine->GetParent(), m_colourLayoutLine,
		wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt()) );

	colourIconSizer = m_colourLayoutLine->GetContainingSizer();
	colourIconSizer->Replace( m_colourLayoutLine, customColourButton );
	colourIconSizer->Layout();
	
	customColourButton->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LayoutObjectPropertiesDlg::OnCustomColourButtonClicked ), NULL, this );
	#endif

	if(layoutObjProp->GetLayoutLineStyle() == VisualLine::HIDDEN)
		m_cboLayoutLineStyle->SetValue(_T("Hidden"));
	else if(layoutObjProp->GetLayoutLineStyle() == VisualLine::SOLID)
		m_cboLayoutLineStyle->SetValue(_T("Solid"));
	else if(layoutObjProp->GetLayoutLineStyle() == VisualLine::SHORT_DASH)
		m_cboLayoutLineStyle->SetValue(_T("Short dash"));
	else if(layoutObjProp->GetLayoutLineStyle() == VisualLine::LONG_DASH)
		m_cboLayoutLineStyle->SetValue(_T("Long dash"));

	// Control Point Properties
	m_spinControlPointSize->SetValue(layoutObjProp->GetControlPointSize());

	colour = layoutObjProp->GetControlPointColour();
	m_colourControlPoint->SetColour(wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt(), colour.GetAlphaInt()));
	#ifndef WIN32
	m_colourControlPoint->Hide();
	customColourButton = new CustomColourButton( m_colourControlPoint->GetParent(), m_colourControlPoint,
		wxColour(colour.GetRedInt(), colour.GetGreenInt(), colour.GetBlueInt()) );

	colourIconSizer = m_colourControlPoint->GetContainingSizer();
	colourIconSizer->Replace( m_colourControlPoint, customColourButton );
	colourIconSizer->Layout();
	
	customColourButton->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LayoutObjectPropertiesDlg::OnCustomColourButtonClicked ), NULL, this );
	#endif
}

void LayoutObjectPropertiesDlg::Apply()
{
	LayoutObjectPropertiesPtr layoutObjProp = App::Inst().GetLayoutObjProp();

	layoutObjProp->SetObjectLineThickness(m_spinObjectLineThickness->GetValue());
	layoutObjProp->SetObjectLineColour(Colour(m_colourObjectLine->GetColour()));
	if(m_cboObjectLineStyle->GetValue() == _T("Hidden"))
		layoutObjProp->SetObjectLineStyle(VisualLine::HIDDEN);
	else if(m_cboObjectLineStyle->GetValue() == _T("Solid"))
		layoutObjProp->SetObjectLineStyle(VisualLine::SOLID);
	else if(m_cboObjectLineStyle->GetValue() == _T("Short dash"))
		layoutObjProp->SetObjectLineStyle(VisualLine::SHORT_DASH);
	else if(m_cboObjectLineStyle->GetValue() == _T("Long dash"))
		layoutObjProp->SetObjectLineStyle(VisualLine::LONG_DASH);

	layoutObjProp->SetLayoutLineThickness(m_spinLayoutLineThickness->GetValue());
	layoutObjProp->SetLayoutLineColour(Colour(m_colourLayoutLine->GetColour()));
	if(m_cboLayoutLineStyle->GetValue() == _T("Hidden"))
		layoutObjProp->SetLayoutLineStyle(VisualLine::HIDDEN);
	else if(m_cboLayoutLineStyle->GetValue() == _T("Solid"))
		layoutObjProp->SetLayoutLineStyle(VisualLine::SOLID);
	else if(m_cboLayoutLineStyle->GetValue() == _T("Short dash"))
		layoutObjProp->SetLayoutLineStyle(VisualLine::SHORT_DASH);
	else if(m_cboLayoutLineStyle->GetValue() == _T("Long dash"))
		layoutObjProp->SetLayoutLineStyle(VisualLine::LONG_DASH);

	layoutObjProp->SetControlPointSize(m_spinControlPointSize->GetValue());
	layoutObjProp->SetControlPointColour(Colour(m_colourControlPoint->GetColour()));

	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	App::Inst().GetViewport()->Refresh( false );
}

void LayoutObjectPropertiesDlg::OnCustomColourButtonClicked( wxMouseEvent& event )
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

	// Set the wxColourPickerCtrl background (hidden, but its value will be used to change the background)
	customColourButton->GetWXColourPickerCtrl()->SetColour( colourPicker->GetColourData().GetColour() );
}