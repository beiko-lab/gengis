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
#include "../utils/Font.hpp"
#include "../gui/FontPropertiesDlg.hpp"

#include "../core/Viewport.hpp"

using namespace GenGIS;

FontPropertiesDlg::FontPropertiesDlg(wxWindow* parent): FontPropertiesLayout(parent)
{
	SetIcon( wxIcon( App::Inst().GetExeDir() + wxT( "images/CrazyEye.ico" ), wxBITMAP_TYPE_ICO ) );

	Init();
	Fit();
}

FontPropertiesDlg::~FontPropertiesDlg()
{

}

void FontPropertiesDlg::Init()
{
	FontPtr font = App::Inst().GetFont();
	std::string fontFile = font->FontFile();

	if ( fontFile == "arial.ttf" )
		m_cboFont->SetValue( _T( "Arial" ) );

	else if ( fontFile == "couri.ttf" )
		m_cboFont->SetValue( _T( "Courier" ) );

	else if ( fontFile == "times.ttf" )
		m_cboFont->SetValue( _T( "Times New Roman" ) );

	else if ( fontFile == "timesbd.ttf" )
		m_cboFont->SetValue( _T( "Times New Roman (bold)" ) );

	else if ( fontFile == "verdana.ttf" )
		m_cboFont->SetValue( _T( "Verdana" ) );
}

void FontPropertiesDlg::Apply()
{
	if ( m_cboFont->GetValue() == _T( "Arial" ) )
		App::Inst().InitFont( _T( "arial.ttf" ) );

	else if ( m_cboFont->GetValue() == _T( "Courier" ) )
		App::Inst().InitFont( _T( "couri.ttf" ) );

	else if ( m_cboFont->GetValue() == _T( "Times New Roman" ) )
		App::Inst().InitFont( _T( "times.ttf" ) );

	else if ( m_cboFont->GetValue() == _T( "Times New Roman (bold)" ) )
		App::Inst().InitFont( _T( "timesbd.ttf" ) );

	else if ( m_cboFont->GetValue() == _T( "Verdana" ) )
		App::Inst().InitFont( _T( "verdana.ttf" ) );

	App::Inst().SetSaveStatus( SESSION_NOT_SAVED );
	App::Inst().GetViewport()->Refresh( false );
}
