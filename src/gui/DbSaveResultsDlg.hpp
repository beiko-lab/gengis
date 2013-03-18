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

#ifndef _GENGIS_DB_SAVE_RESULTS_DLG_
#define _GENGIS_DB_SAVE_RESULTS_DLG_

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"

#include "../gui/DbSaveResultsLayout.h"

namespace GenGIS
{

/**
 * @brief Select sample and sequence filenames.
 */
class DbSaveResultsDlg : public DbSaveResultsLayout
{
public:
	/**
	 * @brief Constructor.
	 * @param parent Parent for this frame
	 */
	explicit DbSaveResultsDlg(wxWindow* parent);

	/** Destructor */
	~DbSaveResultsDlg();

	wxString GetSampleFile() const { return m_txtSampleFilename->GetValue(); }
	wxString GetSequenceFile() const { return m_txtSequenceFilename->GetValue(); }

private:
	void OnSampleFilename(wxCommandEvent& event);
	void OnSequenceFilename(wxCommandEvent& event);

	/** OK button event handler. */
	void OnOK( wxCommandEvent& event ){ EndModal(wxID_OK); }

	/**Cancel button event handler. */
	void OnCancel( wxCommandEvent& event ){ EndModal(wxID_CANCEL); }
};

}

#endif 
