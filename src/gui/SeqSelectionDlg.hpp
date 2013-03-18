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

#ifndef _GENGIS_SEQ_SELECTION_DLG_
#define _GENGIS_SEQ_SELECTION_DLG_

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"

#include "../gui/SeqSelectionLayout.h"

namespace GenGIS
{

	/**
	* @brief Handles adding/removing sequences from the active set of sequences.
	*/
	class SeqSelectionDlg : public SeqSelectionLayout
	{
	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		* @param locationSetLayer Layer to set properties of.
		*/
		explicit SeqSelectionDlg(wxWindow* parent, LocationSetLayerPtr locationSetLayer);

		/** Destructor */
		~SeqSelectionDlg();

	private:
		/** Initialize controls. */
		void Init();

		/** Apply all parameter values. */
		void Apply();

		/** Field combobox value has changed. */
		void OnFieldChange( wxCommandEvent& event );

		/** Get all field values for a given field in sorted order. */
		void GetSortedFieldValues(const std::wstring& field, bool bActive, std::vector<std::wstring>& fieldValues);

		/** Sort field values either in lexigraphically or numerically. */
		void SortFieldValues(std::vector<std::wstring>& fieldValues);

		/** Add sequences to active set. */
		void OnAdd( wxCommandEvent& event );

		/** Remove sequences from active set. */
		void OnRemove( wxCommandEvent& event );

		/** OK button event handler. */
		void OnOK( wxCommandEvent& event ){ Apply(); Destroy(); }

		/** Apply button event handler. */
		void OnApply( wxCommandEvent& event ){ Apply(); }

		/** Cancel button event handler. */
		void OnCancel( wxCommandEvent& event ){ Destroy(); }

		/** Close dialog. */
		void OnClose( wxCommandEvent& event ) { Destroy(); }

	protected:
		/** Location set to display properties of. */
		LocationSetLayerPtr m_locationSetLayer;

		/** Location set controller for locations. */
		LocationSetControllerPtr m_locationSetController;
	};

}

#endif 
