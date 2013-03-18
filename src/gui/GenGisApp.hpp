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

#ifndef _GEOPHYLOGENY_APP_
#define _GEOPHYLOGENY_APP_

#include "../core/Precompiled.hpp"

class InterfaceGenGUI;

/**
* @brief Entry point for wxWidget application.
*/
class GenGisApp : public wxApp
{
public:
	/**
	* @brief Perform required initialization before starting the application.
	*/
	virtual bool OnInit();

	/**
	* @brief Perform any necessary clean up before application terminates.
	*/
	virtual int OnExit();

	/**
	* @brief Enables the application to handle files that are dropped on the
	* application icon and respond to double-clicking on session files within
	* Finder
	*/
	virtual void MacOpenFile(const wxString &fileName);
	/**
	* @brief Catch any exceptions the propagate to the top-level window.
	*/
	//virtual void OnFatalException();
};

#endif
