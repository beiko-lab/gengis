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

#include "../gui/StudyPropertiesDlg.hpp"
#include "../gui/ProgressDlg.hpp"

#include "../utils/StringTools.hpp"

#include "../core/App.hpp"
#include "../core/Viewport.hpp"
#include "../core/StudyLayer.hpp"
#include "../core/MapController.hpp"
#include "../core/MapView.hpp"
#include "../core/LayerTreeController.hpp"

#include "../widgets/CustomColourButton.hpp"

using namespace GenGIS;

StudyPropertiesDlg::StudyPropertiesDlg(wxWindow* parent, StudyLayerPtr studyLayer)
	: StudyPropertiesLayout(parent), m_studyLayer(studyLayer)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	// Limit the properties dialog to a single instance
	m_studyLayer->SetPropertiesDialog( this );

	Init();
	Fit();
}

StudyPropertiesDlg::~StudyPropertiesDlg()
{
	// Tell the layer that the properties dialog is being closed
	m_studyLayer->SetPropertiesDialogOpenStatus( false );
	m_studyLayer->SetPropertiesDialog( NULL );
}

void StudyPropertiesDlg::OnOK( wxCommandEvent& event )
{
	Apply();
	Destroy();
}

/** Apply button event handler. */
void StudyPropertiesDlg::OnApply( wxCommandEvent& event ){ Apply(); }

/**Cancel button event handler. */
void StudyPropertiesDlg::OnCancel( wxCommandEvent& event )
{
	Destroy();
}

/** Close dialog. */
void StudyPropertiesDlg::OnClose( wxCloseEvent& event )
{
	Destroy();
}

void StudyPropertiesDlg::Init()
{
	// set the title of the properties dialog
	this->SetLabel( wxString(m_studyLayer->GetName().c_str()) + wxT( " : Study Properties" ) );

	// set state of controls on General page
	m_txtLayerName->SetValue(wxString(m_studyLayer->GetName().c_str()));
	m_txtLayerDescription->SetValue(wxString(m_studyLayer->GetDescription().c_str()));
	m_txtAuthours->SetValue(wxString(m_studyLayer->GetAuthours().c_str()));

	// set state of controls on Projection page
	StudyControllerPtr studyController = m_studyLayer->GetStudyController();
	m_cboDatum->SetValue(wxString(studyController->GetDatum().c_str()));
	m_cboProjection->SetValue(wxString(studyController->GetProjection().c_str()));

	// set state of controls on Symbology page
	Colour bgColour = App::Inst().GetViewport()->GetBackgroundColour();
	m_colourBackground->SetColour(wxColour(bgColour.GetRedInt(), bgColour.GetGreenInt(), bgColour.GetBlueInt()));

	#ifndef WIN32
	m_colourBackground->Hide();

	CustomColourButton* customColourButton = new CustomColourButton( m_colourBackground->GetParent(), m_colourBackground,
		wxColour(bgColour.GetRedInt(), bgColour.GetGreenInt(), bgColour.GetBlueInt()) );

	wxSizer* colourIconSizer = m_colourBackground->GetContainingSizer();
	colourIconSizer->Replace( m_colourBackground, customColourButton );
	colourIconSizer->Layout();

	customColourButton->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( StudyPropertiesDlg::OnCustomColourButtonClicked ), NULL, this );
	#endif

	int maxTerrainResolution = App::Inst().GetViewport()->GetMaxTerrainResolution();
	if      (maxTerrainResolution == 256)  m_cboTerrainResolution->SetValue(_T("256"));
	else if (maxTerrainResolution == 512)  m_cboTerrainResolution->SetValue(_T("512"));
	else if (maxTerrainResolution == 1024) m_cboTerrainResolution->SetValue(_T("1024"));
	else if (maxTerrainResolution == 2048) m_cboTerrainResolution->SetValue(_T("2048"));
	else if (maxTerrainResolution == 4096) m_cboTerrainResolution->SetValue(_T("4096"));
	else if (maxTerrainResolution == 8196) m_cboTerrainResolution->SetValue(_T("8196"));
}

void StudyPropertiesDlg::OnCustomColourButtonClicked( wxMouseEvent& event )
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
	m_colourBackground->SetColour( colourPicker->GetColourData().GetColour() );
}

void StudyPropertiesDlg::Apply()
{
	// set properties based on state of controls in General page
	m_studyLayer->SetName(m_txtLayerName->GetValue().c_str());
	m_studyLayer->SetDescription(m_txtLayerDescription->GetValue().c_str());
	m_studyLayer->SetAuthours(m_txtAuthours->GetValue().c_str());

	// set properties based on state of controls in Projection page
	StudyControllerPtr studyController = m_studyLayer->GetStudyController();
	studyController->SetDatum(m_cboDatum->GetValue().wc_str());
	studyController->SetProjection(m_cboProjection->GetValue().wc_str());

	ViewportPtr viewport = App::Inst().GetViewport();

	// set properties based on state of controls in Symbology page
	viewport->SetBackgroundColour( Colour( m_colourBackground->GetColour() ) );

	if(m_cboTerrainResolution->GetValue() == _T("256"))
		viewport->SetMaxTerrainResolution(256);
	else if(m_cboTerrainResolution->GetValue() == _T("512"))
		viewport->SetMaxTerrainResolution(512);
	else if(m_cboTerrainResolution->GetValue() == _T("1024"))
		viewport->SetMaxTerrainResolution(1024);
	else if(m_cboTerrainResolution->GetValue() == _T("2048"))
		viewport->SetMaxTerrainResolution(2048);
	else if(m_cboTerrainResolution->GetValue() == _T("4096"))
		viewport->SetMaxTerrainResolution(4096);
	else if(m_cboTerrainResolution->GetValue() == _T("8196"))
		viewport->SetMaxTerrainResolution(8196);

	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	viewport->Refresh( false );
	App::Inst().GetLayerTreeController()->SetName(m_studyLayer, m_studyLayer->GetName());
}

