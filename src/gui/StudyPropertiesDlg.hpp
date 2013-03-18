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

#ifndef _GENGIS_STUDY_PROPERTIES_DLG_
#define _GENGIS_STUDY_PROPERTIES_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/StudyPropertiesLayout.h"

namespace GenGIS
{

	/**
	* @brief Handles all events and GUI processing for study properties dialog.
	*/
	class StudyPropertiesDlg : public StudyPropertiesLayout
	{
	public:
		enum PAGE { GENERAL = 0, PROJECTION = 1, METADATA = 2 };

	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		* @param studyLayer Layer to set properties of.
		*/
		explicit StudyPropertiesDlg(wxWindow* parent, StudyLayerPtr studyLayer);

		/** Destructor */
		~StudyPropertiesDlg();

		/** Apply all parameter values. */
		void Apply();

		/** Set the currently open page. */
		void SetPage(int pageIndex) { m_notebook->SetSelection(pageIndex); }	

	private:
		/** Initialize all dialog box parameters. */
		void Init();

		/** Event handler for custom colour buttons (Mac/Unix releases). */
		void OnCustomColourButtonClicked( wxMouseEvent& event );

		/** OK button event handler. */
		void OnOK( wxCommandEvent& event );

		/** Apply button event handler. */
		void OnApply( wxCommandEvent& event );

		/**Cancel button event handler. */
		void OnCancel( wxCommandEvent& event );

		/** Close dialog. */
		void OnClose( wxCloseEvent& event );

		void OnHelp( wxCommandEvent& event )
		{
			wxLaunchDefaultBrowser( wxT( "http://kiwi.cs.dal.ca/GenGIS/The_GenGIS_2.0_Manual#Study_Layer_Properties" ));
		}

	protected:

		StudyLayerPtr m_studyLayer;
	};

}

#endif 
