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

#ifndef _GENGIS_DB_SELECT_SAMPLE_DLG_
#define _GENGIS_DB_SELECT_SAMPLE_DLG_

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"

#include "../gui/DbSelectSampleLayout.h"

namespace GenGIS
{

	/**
	* @brief Provides a graphical interface to select samples for a database.
	*/
	class DbSelectSampleDlg : public DbSelectSampleLayout
	{
	protected:
		enum MODIFIER { MODIFIER_DELETE_SELECTED, MODIFIER_DELETE_UNSELECTED, MODIFIER_SELECT };

	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		*/
		explicit DbSelectSampleDlg(wxWindow* parent);

		/** Destructor */
		~DbSelectSampleDlg();

	private:
		static const int PROJECT_ID_COLUMN = 0;
		static const int SAMPLE_ID_COLUMN = 1;

		/** Initialize controls. */
		void Init();

		void ConnectEvents();
		void DisconnectEvents();

		/** Read data. */
		bool ReadData(const std::wstring& file);

		/** Read field data. */
		bool ReadFieldData(std::wifstream& fin);

		/** Read sample data. */
		bool ReadSampleData(std::wifstream& fin);

		/** Populate table with data. */
		void PopulateTable(const std::vector<std::wstring>& fields, const std::vector< std::vector<std::wstring> >& rows);

		/** OK button event handler. */
		//void OnOK( wxCommandEvent& event ){ Apply(); Destroy(); }

		/** Apply button event handler. */
		//void OnApply( wxCommandEvent& event ){ Apply(); }

		/** Cancel button event handler. */
		void OnCancel( wxCommandEvent& event ){ Destroy(); }

		/** Close dialog. */
		void OnClose( wxCommandEvent& event ) { Destroy(); }

		void OnRestoreAllSamples( wxCommandEvent& event );

		void OnContinue( wxCommandEvent& event );

		void OnFetchSamplesFromDB( wxCommandEvent& event );
		void OnLoadSampleList( wxCommandEvent& event );
		void OnSaveSampleList( wxCommandEvent& event );

		void OnModifyNumberCol(MODIFIER modifier);
		void OnFilterSelectedNumber( wxCommandEvent& event );
		void OnFilterUnselectedNumber( wxCommandEvent& event );
		void OnSelectNumber( wxCommandEvent& event );

		void OnModifyStringCol(MODIFIER modifier);
		void OnFilterSelectedString( wxCommandEvent& event );
		void OnFilterUnselectedString( wxCommandEvent& event );
		void OnSelectString( wxCommandEvent& event );

		void OnModifyDateCol(MODIFIER modifier);
		void OnFilterSelectedDate( wxCommandEvent& event );
		void OnFilterUnselectedDate( wxCommandEvent& event );
		void OnSelectDate( wxCommandEvent& event );

		void OnModifyBinaryCol(MODIFIER modifier);
		void OnFilterSelectedBinary( wxCommandEvent& event );
		void OnFilterUnselectedBinary( wxCommandEvent& event );
		void OnSelectBinary( wxCommandEvent& event );

		void OnLabelLeftClick(wxGridEvent& event);
		void OnLabelRightClick(wxGridEvent& event);
		void OnGridRightClick(wxGridEvent& event);
		void OnFieldChanged(wxChoicebookEvent& event);

		void OnRemoveSelectedSamples( wxCommandEvent& event );
		void OnRetainSelectedSamples( wxCommandEvent& event );

		void OnSort(bool bAscending);
		void OnSortAscending( wxCommandEvent& event ) { OnSort(true); }
		void OnSortDescending( wxCommandEvent& event ) { OnSort(false); }

		wxPanel* CreateNumberFilteringPage(wxBookCtrlBase *parent);
		wxPanel* CreateStringFilteringPage(wxBookCtrlBase *parent);
		wxPanel* CreateDateFilteringPage(wxBookCtrlBase *parent);
		wxPanel* CreateBinaryFilteringPage(wxBookCtrlBase *parent);

	protected:
		std::vector<std::wstring> m_fields;
		std::vector<std::wstring> m_fieldDataTypes;
		std::vector<uint> m_dbKeys;
		std::vector< std::vector<std::wstring> > m_rows;

		// blank panel
		wxPanel* m_blankPanel;

		// number panel
		wxPanel* m_numberPanel;
		wxTextCtrl* m_textMin;
		wxTextCtrl* m_textMax;
		wxCheckBox* m_chkNumberSelectNulls;

		// string panel
		wxPanel* m_stringPanel;
		wxTextCtrl* m_textFilterStr;
		wxComboBox* m_cboTextFilterType;
		wxCheckBox* m_chkStringSelectNulls;

		// date panel
		wxPanel* m_datePanel;
		wxCalendarCtrl* m_startDate;
		wxCalendarCtrl* m_endDate;
		wxCheckBox* m_chkDateSelectNulls;

		// binary panel
		wxPanel* m_binaryPanel;
		wxRadioButton* m_radioChoice1;
		wxRadioButton* m_radioChoice2;	
		wxCheckBox* m_chkBinarySelectNulls;
	};

}

#endif 
