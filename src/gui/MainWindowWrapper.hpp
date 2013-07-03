//=======================================================================
// Author: Donovan Parks
//
// Copyright 2011 Donovan Parks
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

#ifndef _GENGIS_MAIN_WINDOW_WRAPPER
#define _GENGIS_MAIN_WINDOW_WRAPPER

#include "../core/Precompiled.hpp"

#include "../core/Viewport.hpp"

#include "../gui/GenGisFrame.hpp"

namespace GenGIS
{

	class MainWindowWrapper
	{
	public:
		explicit MainWindowWrapper() {}
		~MainWindowWrapper() {}

		wxWindow* GetPointer() const
		{
			return App::Inst().GetMainWindow();
		}

		void SetSidePanelVisibility(bool bVisible)
		{
			((GenGisFrame*)App::Inst().GetMainWindow())->SetSidePanelVisibility(bVisible);
		}

		void SetConsoleVisibility(bool bVisible)
		{
			((GenGisFrame*)App::Inst().GetMainWindow())->SetConsoleVisibility(bVisible);
		}

		void OpenMapFile(std::string file)
		{
			((GenGisFrame*)App::Inst().GetMainWindow())->LayerOpenMap(file);
		}
		void OpenVectorMapFile(std::string file)
		{
			((GenGisFrame*)App::Inst().GetMainWindow())->LayerOpenVectorMap(file);
		}

		void OpenLocationsFile(std::string file)
		{
			((GenGisFrame*)App::Inst().GetMainWindow())->LayerOpenLocations(file);
		}

		void OpenLocationsCSVFile( std::vector<std::wstring> csvTableRows, std::wstring locationNames )
		{
			((GenGisFrame*)App::Inst().GetMainWindow())->LayerOpenLocations( csvTableRows, locationNames );
		}

		void OpenSequenceFile(std::string file)
		{
			((GenGisFrame*)App::Inst().GetMainWindow())->LayerOpenSequenceData(file);
		}

		void OpenTreeFile(std::string file)
		{
			((GenGisFrame*)App::Inst().GetMainWindow())->LayerOpenTree(file);
		}

		std::wstring GetExeDir()
		{
			return std::wstring(App::Inst().GetExeDir().c_str());
		}

		void Quit()
		{
			((GenGisFrame*)App::Inst().GetMainWindow())->Close(TRUE);
		}

		void Write(wxString text)
		{
			Log::Inst().Write(text);
		}

		void Warning(wxString text)
		{
			Log::Inst().Warning(text);
		}

		void Error(wxString text)
		{
			Log::Inst().Error(text);
		}

		void Yield()
		{
			::wxSafeYield();
		}
	};

}

#endif
