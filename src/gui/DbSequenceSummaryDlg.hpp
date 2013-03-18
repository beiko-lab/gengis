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

#ifndef _GENGIS_DB_SEQUENCE_SUMMARY_DLG_
#define _GENGIS_DB_SEQUENCE_SUMMARY_DLG_

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"

#include "../gui/DbSequenceSummaryLayout.h"

namespace GenGIS
{
	/**
	* @brief Provides a graphical interface displaying all retrieved sequences.
	*/
	class DbSequenceSummaryDlg : public DbSequenceSummaryLayout
	{
	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		*/
		explicit DbSequenceSummaryDlg(const wxString& fetchString, std::vector< std::wstring > sampleDataFields,
			std::vector< std::vector<std::wstring> > sampleData, wxWindow* parent);

		/** Destructor */
		~DbSequenceSummaryDlg();

	private:
		/** Initialize controls. */
		void Init();

		void FetchSequencesFromDB(const wxString& fetchString);

		void ConnectEvents();
		void DisconnectEvents();

		/** Read data. */
		bool ReadData(const std::wstring& file);

		/** Read field data. */
		bool ReadFieldData(std::wifstream& fin);

		/** Read sequence data. */
		bool ReadSequenceData(std::wifstream& fin);

		/** Populate table with data. */
		void PopulateTable(const std::vector<std::wstring>& fields, const std::vector< std::vector<std::wstring> >& cols);

		/** Cancel button event handler. */
		void OnCancel( wxCommandEvent& event ){ Destroy(); }

		/** Close dialog. */
		void OnClose( wxCommandEvent& event ) { Destroy(); }

		void OnOK( wxCommandEvent& event );

		void SaveSampleFile( const std::wstring& filename );
		void SaveSequenceFile( const std::wstring& filename );

	protected:
		std::vector<std::wstring> m_fields;
		std::vector< std::vector<std::wstring> > m_rows;

		wxString m_fetchString;

		std::vector< std::wstring > m_sampleDataFields;
		std::vector< std::vector<std::wstring> > m_sampleData;
	};

}

#endif 
