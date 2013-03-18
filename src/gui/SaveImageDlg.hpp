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

#ifndef _GENGIS_SAVE_IMAGE_DLG_
#define _GENGIS_LABEL_TOOL_DLG_

#include "../core/Precompiled.hpp"

#include "../gui/SaveImageLayout.h"

namespace GenGIS
{

	/**
	* @brief Save high resolution image dialog box.
	*/
	class SaveImageDlg : public SaveImageLayout
	{
	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		*/
		explicit SaveImageDlg(wxWindow* parent);

		/** Destructor */
		~SaveImageDlg();

		/** Get filename. */
		wxString GetFilename() const;

		/** Get directory. */
		wxString GetDirectory() const;

		/** Get resolution scaling factor for image. */
		uint GetResolutionScalingFactor() const { return m_resolutionScalingFactor; }

		/** Determine whether the user selected to automatically open file location after save. */
		bool OpenFileLocationAfterSave() { return m_openFileLocation->IsChecked(); }

	private:
		/** Apply dialog settings. */
		void Apply();

		/** File selection dialog. */
		void OnSelectFilename(wxCommandEvent& event);

		/** OK button event handler. */
		void OnOK( wxCommandEvent& event );

		/** Cancel button event handler. */
		void OnCancel( wxCommandEvent& event ){ m_txtFilename->SetValue(_T("")); EndModal(wxID_CANCEL); }

	private:
		uint m_resolutionScalingFactor;
	};

}

#endif 
