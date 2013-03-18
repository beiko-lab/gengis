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

#ifndef _GENGIS_PLUGIN_MANAGER_DLG_
#define _GENGIS_PLUGIN_MANAGER_DLG_

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"

#include "../gui/PluginManagerLayout.h"

namespace GenGIS
{

	/**
	* @brief Provides a graphical interface to all plugins within GenGIS.
	*/
	class PluginManagerDlg : public PluginManagerLayout
	{
	public:
		explicit PluginManagerDlg(wxWindow* parent);
		~PluginManagerDlg();

		/** Populate table with data. */
		void PopulateTable(const std::vector<std::string>& fields, const std::vector< std::vector<std::string> >& rows);	
	};

}

#endif 
