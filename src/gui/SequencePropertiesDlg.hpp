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

#ifndef _GENGIS_SEQUENCE_PROPERTIES_DLG_
#define _GENGIS_SEQUENCE_PROPERTIES_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/SequencePropertiesLayout.h"

namespace GenGIS
{

	/**
	* @brief Handles all events and GUI processing for sequence properties dialog.
	*/
	class SequencePropertiesDlg : public SequencePropertiesLayout
	{
	public:
		enum PAGE { GENERAL = 0, METADATA = 1 };

	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		* @param sequenceLayer Sequence layer to modify properties of.
		*/
		explicit SequencePropertiesDlg(wxWindow* parent, SequenceLayerPtr sequenceLayer);

		/** Destructor */
		~SequencePropertiesDlg();

		/** Set the currently open page. */
		void SetPage(int pageIndex) { m_notebook->SetSelection(pageIndex); }

	private:
		/** Initialize all dialog box parameters. */
		void Init();

		/** Apply all parameter values. */
		void Apply();	

		/** OK button event handler. */
		void OnOK( wxCommandEvent& event ){ Apply(); Destroy(); }

		/** Apply button event handler. */
		void OnApply( wxCommandEvent& event ){ Apply(); }

		/**Cancel button event handler. */
		void OnCancel( wxCommandEvent& event ){ Destroy(); }

		/** Close dialog. */
		void OnClose( wxCloseEvent& event ) { Destroy(); }

		void OnHelp( wxCommandEvent& event )
		{
			wxLaunchDefaultBrowser( wxT( "http://kiwi.cs.dal.ca/GenGIS/The_GenGIS_2.0_Manual#Sequence_Layer_Properties" ));
		}

	protected:
		SequenceLayerPtr m_sequenceLayer;
	};

}

#endif 
