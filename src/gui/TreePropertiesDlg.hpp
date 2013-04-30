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

#ifndef _GENGIS_TREE_PROPERTIES_DLG_
#define _GENGIS_TREE_PROPERTIES_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/TreePropertiesLayout.h"

namespace GenGIS
{

	/**
	* @brief Handles all events and GUI processing for tree properties dialog.
	*/
	class TreePropertiesDlg : public TreePropertiesLayout
	{
	public:
		enum PAGE { GENERAL = 0, SYMBOLOGY = 1, LABEL = 2, METADATA = 3 };

	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		* @param treeLayer Layer to set properties of.
		*/
		explicit TreePropertiesDlg(wxWindow* parent, TreeLayerPtr treeLayer);

		/** Destructor */
		~TreePropertiesDlg();

		/** Set the currently open page. */
		void SetPage(int pageIndex) { m_notebook->SetSelection(pageIndex); }

	private:
		/** Initialize all dialog box parameters. */
		void Init();

		/** Apply all parameter values. */
		void Apply();

		/** Initialize the metadata text control. */
		void SetMetaData();

		/** OK button event handler. */
		void OnOK( wxCommandEvent& event ){ Apply(); Destroy(); }

		/** Apply button event handler. */
		void OnApply( wxCommandEvent& event ){ Apply(); }

		/** Cancel button event handler. */
		void OnCancel( wxCommandEvent& event ){ Destroy(); }

		/** Close dialog. */
		void OnClose( wxCloseEvent& event ) { Destroy(); }

		/** Handle change events for layout style combobox. */
		void OnLayoutStyleChange( wxCommandEvent& event );

		/** Event handler for selecting colours under Mac/Unix. */
		void OnCustomColourButtonClicked( wxMouseEvent& event );

		/** Replace the standard wxColourPickerCtrl with a Mac/Unix compatible version. */
		void ReplaceColourPicker( wxColourPickerCtrl* wxColourPicker, Colour& colour );

		void OnHelp( wxCommandEvent& event )
		{
			wxLaunchDefaultBrowser( wxT( "http://kiwi.cs.dal.ca/GenGIS/The_GenGIS_2.1_Manual#Tree_Layer_Properties" ));
		}

	protected:
		TreeLayerPtr m_treeLayer;
	};

}

#endif 
