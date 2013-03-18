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

#include "../gui/NonlinearAxisTableDlg.hpp"

#include "../core/App.hpp"
#include "../core/GeoTreeOptLeafOrder.hpp"
#include "../core/TreeLayer.hpp"
#include "../core/GeoTreeView.hpp"
#include "../core/Viewport.hpp"

#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"


using namespace GenGIS;


NonlinearAxisTableDlg::NonlinearAxisTableDlg(wxWindow* parent, TreeLayerPtr treeLayer) :
		NonlinearAxisTableLayout(parent, wxID_ANY, wxT("Linear axes plot")),	m_treeLayer(treeLayer)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	wxArrayString permutationStrs = m_treeLayer->GetGeoTreeView()->GetNonlinearPermutationResults();
	m_listbox->InsertItems(permutationStrs, 0);
}

NonlinearAxisTableDlg::~NonlinearAxisTableDlg()
{

}

void NonlinearAxisTableDlg::OnListBox(wxCommandEvent& event)
{
	wxString selectedPermutationStr = m_listbox->GetStringSelection();
	wxString permutationNumStr = selectedPermutationStr.Left(selectedPermutationStr.Find(_T(',')));
	long permutationNum;
	permutationNumStr.ToLong(&permutationNum);

	m_treeLayer->GetGeoTreeView()->SetNonlinearAxisOrdering(uint(permutationNum-1));

	m_treeLayer->GetGeoTreeView()->Render();
}
