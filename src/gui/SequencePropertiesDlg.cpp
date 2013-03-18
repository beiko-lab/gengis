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
#include "../core/LayerTreeController.hpp"

#include "../core/SequenceLayer.hpp"
#include "../core/SequenceController.hpp"

#include "../gui/SequencePropertiesDlg.hpp"

#include "../utils/StringTools.hpp"

using namespace GenGIS;

SequencePropertiesDlg::SequencePropertiesDlg(wxWindow* parent, SequenceLayerPtr sequenceLayer)
	: SequencePropertiesLayout(parent), m_sequenceLayer(sequenceLayer)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	// Limit the properties dialog to a single instance
	m_sequenceLayer->SetPropertiesDialog( this );

	Init();
	Fit();
}

SequencePropertiesDlg::~SequencePropertiesDlg()
{
	// Tell the layer that the properties dialog is being closed
	m_sequenceLayer->SetPropertiesDialogOpenStatus( false );
	m_sequenceLayer->SetPropertiesDialog( NULL );
}

void SequencePropertiesDlg::Init()
{
	// set the title of the properties dialog
	this->SetLabel( wxString(m_sequenceLayer->GetName().c_str()) + wxT( " : Sequence Properties" ) );

	// set state of controls on General page
	m_txtLayerName->SetValue(wxString(m_sequenceLayer->GetName().c_str()));
	m_txtLayerDescription->SetValue(wxString(m_sequenceLayer->GetDescription().c_str()));
	m_txtAuthours->SetValue(wxString(m_sequenceLayer->GetAuthours().c_str()));

	// set state of controls on Metadata page
	m_txtLayerSource->SetValue(wxString(m_sequenceLayer->GetPath().c_str()) + _T("\\") + wxString(m_sequenceLayer->GetFilename().c_str()));

	std::map<std::wstring,std::wstring> data = m_sequenceLayer->GetSequenceController()->GetData();
	for(std::map<std::wstring,std::wstring>::iterator it=data.begin(); it != data.end(); it++)
	{
		m_txtMetaData->BeginBold();
		m_txtMetaData->AppendText(wxString((*it).first.c_str()) + _T("\n"));
		m_txtMetaData->EndBold();
		m_txtMetaData->AppendText(wxString((*it).second.c_str()) + _T("\n\n"));
	}
}

void SequencePropertiesDlg::Apply()
{
	// set properties based on state of controls in General page
	m_sequenceLayer->SetName(m_txtLayerName->GetValue().c_str());
	m_sequenceLayer->SetDescription(m_txtLayerDescription->GetValue().c_str());
	m_sequenceLayer->SetAuthours(m_txtAuthours->GetValue().c_str());

	App::Inst().GetLayerTreeController()->SetName(m_sequenceLayer, m_sequenceLayer->GetName());

	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
}
