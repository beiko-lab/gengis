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

// Don't use this code in Windows debug builds!
#ifndef _DEBUG

#ifndef _PLUGIN_MANAGER_
#define _PLUGIN_MANAGER_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	class PluginManager: public wxEvtHandler
	{
	public:
		/** Destructor */
		~PluginManager();

		/** Get the one and only instance of this class.. */
		static PluginManager& Inst()
		{
			static PluginManager pluginManager;
			return pluginManager;
		}

		/** Set main GenGIS window. */
		static void SetMainWindow(wxFrame* mainWindow) { m_mainWindow = mainWindow; }

		/** Load all plugins and place them in the Plugins menu. */
		static void LoadPlugins();

		/** Configure R installation (Windows only). */
		static bool ConfigureRInstallation();

		/** Get the current R version ('a.b.c' format). */
		static std::wstring GetRVersion();

	protected:
		/** Private singleton constructor (must instantiate this class through Inst()). */
		PluginManager() {}

		/** Private copy constructor (must instantiate this class through Inst()). */
		PluginManager(const PluginManager&);

		/** Private assignment constructor (must instantiate this class through Inst()). */
		PluginManager& operator= (const PluginManager&);

		/** Plugins->Plugin manager event handler. */
		void OnPluginsPluginManager( wxCommandEvent& event );

		/** Handle execution of plugins. */
		void OnExecutePlugin( wxCommandEvent& event );

		/** Performs a post-install configuration of R. */
		bool CheckPluginRDependencies( int pluginId );

		/** Adds the R directory to the Windows Path. */
		static bool SetRPath( const wchar_t* rPath );

		static bool TransferDllFilesR();

		static std::wstring GetPathToR();

	private:
		static wxFrame* m_mainWindow;

		static wxMenu* m_mnuPlugins;

		static wxMenu* m_mnuData;

		/** Metadata fields. */
		static std::vector<std::string> m_fields;

		/** Metadata for each plugin. */
		static std::vector< std::vector<std::string> > m_pluginMetadata;

		/** Maps object ids to a python module name. */
		static std::map<uint, std::string> m_objectIdToPluginModuleName;

		/** Maps object ids to a variable indicating if it requires R. */
		static std::map<uint, bool> m_objectIdToRequiresR;

		/** Maps object ids to a variable any R dependencies (if applicable). */
		static std::map<uint, std::string> m_objectIdToRDependencies;
	};

}

#endif

#endif //_DEBUG