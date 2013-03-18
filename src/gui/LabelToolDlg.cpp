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

#include "../gui/ColourPickerDlg.hpp"
#include "../gui/LabelToolDlg.hpp"

#include "../core/App.hpp"
#include "../core/LabelController.hpp"
#include "../core/Viewport.hpp"
#include "../core/VisualLabel.hpp"

#include "../widgets/CustomColourButton.hpp"

using namespace GenGIS;

LabelToolDlg::LabelToolDlg( wxWindow* parent ): LabelToolLayout( parent ), m_saveChanges( false )
{
	SetIcon( wxIcon( App::Inst().GetExeDir() + wxT( "images/CrazyEye.ico" ), wxBITMAP_TYPE_ICO ) );

	m_cboLabelStyle->SetValue( wxT( "Billboard" ) );
	Fit();
}

LabelToolDlg::~LabelToolDlg()
{

}

/** OK button event handler. */
void LabelToolDlg::OnOK( wxCommandEvent& event )
{
	m_saveChanges = true;
	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	App::Inst().GetViewport()->Refresh( false );
	EndModal( wxID_OK );
}

void LabelToolDlg::SetCustomColourButton()
{
	#ifndef WIN32
	m_colourLabel->Hide();
	CustomColourButton* customColourButton = new CustomColourButton( m_colourLabel->GetParent(), m_colourLabel,
		m_colourLabel->GetColour() );

	wxSizer* colourIconSizer = m_colourLabel->GetContainingSizer();
	colourIconSizer->Replace( m_colourLabel, customColourButton );
	colourIconSizer->Layout();
	
	customColourButton->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( LabelToolDlg::OnCustomColourButtonClicked ), NULL, this );
	#endif
}

void LabelToolDlg::OnCustomColourButtonClicked( wxMouseEvent& event )
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
