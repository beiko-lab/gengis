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

#include "../gui/SaveImageDlg.hpp"

using namespace GenGIS;

SaveImageDlg::SaveImageDlg(wxWindow* parent): SaveImageLayout(parent), m_resolutionScalingFactor(1)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	#ifndef WIN32
	// Different behaviour on the Mac
	m_openFileLocation->SetLabel( wxT( "Open file after saving" ) );
	#endif

	m_cboResolution->SetValue(wxT("Native screen resolution (smallest file size)"));
	Fit();
}

SaveImageDlg::~SaveImageDlg()
{

}

void SaveImageDlg::OnOK( wxCommandEvent& event )
{
	wxString filename = m_txtFilename->GetValue();
	wxFileName fileTest( filename );

	// Check whether the filename variable is empty or
	// if a file name is provided (not just the directory path)
	if ( filename.IsEmpty() || !fileTest.HasName() )
	{
		m_staticTextErrorMessage->SetLabel( wxT( "Please specify a filename." ) );
		m_staticTextErrorMessage->SetForegroundColour( *wxRED );
		Refresh();
		return;
	}

	// Check for illegal characters <>"/|?*
	#ifdef WIN32
	wxString illegals( wxT( "<>\"/|?*" ) );
	#else
	wxString illegals( wxT( ":" ) );
	#endif
	for ( uint a = 0; a < illegals.length(); a++ )
	{
		if ( filename.Find( illegals[a] ) != wxNOT_FOUND )
		{
			m_staticTextErrorMessage->SetLabel( wxT( "The filename cannot contain <>\"/|?*" ) );
			m_staticTextErrorMessage->SetForegroundColour( *wxRED );
			Refresh();
			return;
		}
	}

	// Check if the file path is correct
	if ( !fileTest.DirExists( fileTest.GetPath() ) )
	{
		m_staticTextErrorMessage->SetLabel( wxT( "The specified file path does not exist." ) );
		m_staticTextErrorMessage->SetForegroundColour( *wxRED );
		Refresh();
		return;
	}

	// Check if the directory is writable
	if ( !fileTest.IsDirWritable( fileTest.GetPath() ) )
	{
		m_staticTextErrorMessage->SetLabel( wxT( "Cannot save to specified directory." ) );
		m_staticTextErrorMessage->SetForegroundColour( *wxRED );
		Refresh();
		return;
	}

	Apply();
	EndModal(wxID_OK);
}

void SaveImageDlg::Apply()
{
	if(m_cboResolution->GetValue() == _T("Native screen resolution (smallest file size)"))
		m_resolutionScalingFactor = 1;
	else if(m_cboResolution->GetValue() == _T("2 x screen resolution (recommended for review papers and websites)"))
		m_resolutionScalingFactor = 2;
	else if(m_cboResolution->GetValue() == _T("3 x screen resolution (recommended for papers)"))
		m_resolutionScalingFactor = 3;
	else if(m_cboResolution->GetValue() == _T("4 x screen resolution (recommended for posters)"))
		m_resolutionScalingFactor = 4;
	else if(m_cboResolution->GetValue() == _T("5 x screen resolution"))
		m_resolutionScalingFactor = 5;
	else if(m_cboResolution->GetValue() == _T("10 x screen resolution"))
		m_resolutionScalingFactor = 10;
}

void SaveImageDlg::OnSelectFilename(wxCommandEvent& event)
{
	wxFileDialog* saveDlg = new wxFileDialog(this, wxT("Save image..."), wxEmptyString, wxEmptyString, wxT("*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);

	wxString wildcards = wxT("Portable Network Graphics (*.png)|*.png|Portable Pixel Map (*.ppm)|*.ppm");
	saveDlg->SetWildcard(wildcards);

	if(saveDlg->ShowModal() != wxID_OK)
	{
		saveDlg->Destroy();
		return;
	}

	m_txtFilename->SetValue(saveDlg->GetPath());

	// Hide the error label (if changed) that reports incorrect filenames
	m_staticTextErrorMessage->SetLabel( wxEmptyString );
	m_staticTextErrorMessage->SetForegroundColour( *wxBLACK );
	Refresh();

	return;
}


wxString SaveImageDlg::GetDirectory() const
{
	wxFileName path( m_txtFilename->GetValue() );
	return path.GetPath();
}

wxString SaveImageDlg::GetFilename() const
{
	return m_txtFilename->GetValue();
}
