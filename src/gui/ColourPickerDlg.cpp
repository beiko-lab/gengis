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
#include "../gui/ColourPickerDlg.hpp"

using namespace GenGIS;

void ColourPickerDlg::ShowModal(wxWindow* parent, wxButton* colourPickerBtn)
{
	static bool bInit = false;

	if(bInit == false)
	{
		bInit = true;

		m_colourData.SetChooseFull(true);
		for (int i = 0; i < 16; i++)
		{
			wxColour colour(i*16, i*16, i*16);
			m_colourData.SetCustomColour(i, colour);
		}
	}

	m_colourData.SetColour(colourPickerBtn->GetBackgroundColour());

	wxColourDialog colourDlg(parent, &m_colourData);
	if(colourDlg.ShowModal() != wxID_OK)
	{
		return;
	}

	m_colourData = colourDlg.GetColourData();
	m_colour = m_colourData.GetColour();
	colourPickerBtn->SetBackgroundColour(m_colour);
	colourPickerBtn->SetForegroundColour(m_colour);
}
