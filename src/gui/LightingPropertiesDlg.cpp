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
#include "../gui/LightingPropertiesDlg.hpp"
#include "../core/Lighting.hpp"

#include "../core/Viewport.hpp"

using namespace GenGIS;

LightingPropertiesDlg::LightingPropertiesDlg(wxWindow* parent): LightingPropertiesLayout(parent)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	Init();
	Fit();
}

LightingPropertiesDlg::~LightingPropertiesDlg()
{

}

void LightingPropertiesDlg::Init()
{
	LightingPtr lighting = App::Inst().GetViewport()->GetLighting();

	float x = lighting->GetPositionX();
	float z = lighting->GetPositionZ();

	float angle = atan2(x, z) * RAD_TO_DEG;
	if(angle < 0)
		angle += 360;

	// a compass actually goes CW as opposed to CCW and must be between 0 and 359
	angle = (360 - angle);
	if(angle == 360)
		angle = 0;

	m_sliderLightDirection->SetValue((int)(angle + 0.5));

	m_sliderLightAltitude->SetValue((int)(lighting->GetAltitude()*90));

	m_sliderAmbientLightStrength->SetValue((int)(lighting->GetGlobalAmbient()*100));
}

void LightingPropertiesDlg::Apply()
{
	LightingPtr lighting = App::Inst().GetViewport()->GetLighting();

	GLfloat position[4];
	position[0] = -sin(m_sliderLightDirection->GetValue()*DEG_TO_RAD);
	position[1] = 1.0f;
	position[2] = cos(m_sliderLightDirection->GetValue()*DEG_TO_RAD);
	position[3] = 0.0f;

	lighting->SetPosition(position);
	lighting->SetAltitude(m_sliderLightAltitude->GetValue() / 90.0);
	lighting->SetGlobalAmbient(m_sliderAmbientLightStrength->GetValue() / 100.0);

	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	App::Inst().GetViewport()->Refresh( false );
}

void LightingPropertiesDlg::OnDefaultValues( wxCommandEvent& event )
{
	App::Inst().GetViewport()->GetLighting()->SetDefaultValues();
	Init();
}

void LightingPropertiesDlg::OnScrollChanged( wxScrollEvent& event )
{

}