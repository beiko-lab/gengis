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

#include "../utils/Log.hpp"
#include "../core/App.hpp"

using namespace GenGIS;
using namespace boost::filesystem;

Log::Log()
{
	wxString logFile = GenGIS::App::Inst().GetUserDataDir() + wxT("/GenGIS_Log.txt");
	std::string logFileStr(std::string(logFile.begin(), logFile.end()));
	m_log.open(logFileStr.c_str());
	m_logData.reserve(1000);	// allocate enough space to store a thousand entries

	m_console = NULL;
}
