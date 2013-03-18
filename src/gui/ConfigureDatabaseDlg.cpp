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

#include "../gui/ConfigureDatabaseDlg.hpp"

#include "../core/App.hpp"

using namespace GenGIS;

ConfigureDatabaseDlg::ConfigureDatabaseDlg(wxWindow* parent): ConfigureDatabaseLayout(parent) 
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	int index = m_choiceMOA->FindString(wxString(App::Inst().GetDatabaseConnectionString().c_str()));

	if(index == wxNOT_FOUND)
	{
		m_choiceMOA->SetStringSelection(_T("<Custom>"));
	}
	else
	{
		m_choiceMOA->SetSelection(index);
	}
}

void ConfigureDatabaseDlg::OnOK( wxCommandEvent& event )
{ 
	wxString choiceStr = m_choiceMOA->GetString(m_choiceMOA->GetSelection());
	if(choiceStr == _T("<Custom>"))
	{
		App::Inst().SetDatabaseConnectionString(m_txtCustomStr->GetValue().c_str());
	}
	else
	{
		App::Inst().SetDatabaseConnectionString(choiceStr.c_str());
	}

	Destroy();
}

void ConfigureDatabaseDlg::OnTextChanged(wxCommandEvent& event)
{
	if(m_txtCustomStr->GetValue().empty())
	{
		m_choiceMOA->SetSelection(0);
	}
	else
	{
		m_choiceMOA->SetStringSelection(_T("<Custom>"));
	}
}
