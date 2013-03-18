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

#include "../gui/PluginManagerDlg.hpp"

#include "../utils/StringTools.hpp"
#include "../utils/Log.hpp"

using namespace GenGIS;
using namespace boost;
using namespace std;

const int ID_GRID                        = 10000;
const int ID_CHOICE_BOOK                 = 10001;
const int ID_POPUP_MNU_REMOVE_SELECTED   = 10002;
const int ID_POPUP_MNU_RETAIN_SELECTED   = 10003;
const int ID_POPUP_MNU_SORT_ASCENDING    = 10004;
const int ID_POPUP_MNU_RETAIN_DESCENDING = 10005;

PluginManagerDlg::PluginManagerDlg(wxWindow* parent): PluginManagerLayout(parent)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	m_grid->SetMargins(0,0);
	m_grid->SetSelectionMode(wxGrid::wxGridSelectRows);
	m_grid->SetRowLabelSize(0);
	m_grid->EnableScrolling(false, false);
}

PluginManagerDlg::~PluginManagerDlg()
{

}

void PluginManagerDlg::PopulateTable(const vector<string>& fields, const vector< vector<string> >& rows)
{
	m_grid->Freeze();

	// set size of table
	m_grid->DeleteCols(0, m_grid->GetNumberCols());
	m_grid->AppendCols(fields.size());
	m_grid->DeleteRows(0, m_grid->GetNumberRows());
	m_grid->AppendRows(rows.size());

	// set field labels
	for(uint i = 0; i < fields.size(); ++i)
		m_grid->SetColLabelValue(i, wxString(fields.at(i).c_str(), wxConvISO8859_1));

	// set field values
	for(uint i = 0; i < rows.size(); ++i)
		for(uint j = 0; j < fields.size(); ++j)
			m_grid->SetCellValue(i, j, wxString(rows.at(i).at(j).c_str(), wxConvISO8859_1));

	m_grid->AutoSizeColumns();
	m_grid->Thaw();

	Fit();
}
