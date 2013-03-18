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

#ifndef _GENGIS_DB_SELECT_ATTRIBUTES_DLG_
#define _GENGIS_DB_SELECT_ATTRIBUTES_DLG_

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"

#include "../gui/DbSelectAttributesLayout.h"

namespace GenGIS
{

	/**
	* @brief Provides a graphical interface to select sample attributes from a database.
	*/
	class DbSelectAttributesDlg : public DbSelectAttributesLayout
	{
	protected:
		enum MODIFIER { MODIFIER_DELETE_SELECTED, MODIFIER_DELETE_UNSELECTED, MODIFIER_SELECT };

	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		*/
		explicit DbSelectAttributesDlg(const wxString& fetchString, std::vector< std::wstring > sampleDataFields,
			std::vector< std::vector<std::wstring> > sampleData, wxWindow* parent);

		/** Destructor */
		~DbSelectAttributesDlg();

	private:
		/** Initialize controls. */
		void Init();

		void ConnectEvents();
		void DisconnectEvents();

		void FetchAttributesFromDB(const wxString& fetchString);

		/** Read data. */
		bool ReadData(const std::wstring& file);
		bool ReadNumberData(const std::wstring& line, int colNum);
		bool ReadGroupableStringData(std::wstring& line, std::wifstream& fin, int colNum);

		/** Populate table with data. */
		void PopulateTable(const std::vector<std::wstring>& fields, const std::vector< std::vector<std::wstring> >& cols);

		/** Cancel button event handler. */
		void OnCancel( wxCommandEvent& event ){ Destroy(); }

		void OnContinue( wxCommandEvent& event );

		void OnRestoreOriginalTable( wxCommandEvent& event );

		void OnRemoveSelectedSamples( wxCommandEvent& event );
		void OnRetainSelectedSamples( wxCommandEvent& event );

		void OnRemoveField( wxCommandEvent& event );

		void OnModifyNumberField();
		void OnModifyGroupableStringField();
		void OnModifyUngroupableStringField();

		void OnLabelLeftClick(wxGridEvent& event);
		void OnLabelRightClick(wxGridEvent& event);
		void OnGridRightClick(wxGridEvent& event);
		void OnFieldChanged(wxChoicebookEvent& event);
		void OnFieldChanging(wxChoicebookEvent& event);

		wxPanel* CreateNumberFieldPage(wxBookCtrlBase *parent);
		wxPanel* CreateGroupableStringPage(wxBookCtrlBase *parent);
		wxPanel* CreateUngroupableStringPage(wxBookCtrlBase *parent);

		void OnGroupableStringAdd( wxCommandEvent& event );
		void OnGroupableStringRemove( wxCommandEvent& event );
		void OnGroupableStringClear( wxCommandEvent& event );

		wxString GenerateSequenceDataQuery();

		/** Close dialog. */
		void OnClose( wxCommandEvent& event ) { Destroy(); }

	protected:
		static const int PROJECT_ID_COLUMN = 0;
		static const int SAMPLE_ID_COLUMN = 1;

		struct NumberData {
			NumberData(): bAll(false), bLessThan(false), bBetween(false), bGreaterThan(false), bNone(true) {}
			wxString lessThan;
			wxString lowerBound;
			wxString upperBound;
			wxString greaterThan;
			bool bAll;
			bool bLessThan;
			bool bBetween;
			bool bGreaterThan;
			bool bNone;
		};

		struct GroupableStringData {
			wxString filterStr;
			std::vector<wxString> availableItems;
			std::vector<wxString> returnableItems;
			std::map<wxString, wxString> labelMap;
		};

		struct UngroupableStringData {
			UngroupableStringData(): bAll(false), bStringContains(false), bNone(true) {}

			wxString containsStr;
			bool bAll;
			bool bStringContains;
			bool bNone;
		};

		std::vector<std::wstring> m_fields;
		std::map<std::wstring, uint> m_fieldMap;
		std::vector<std::wstring> m_fieldDataTypes;
		std::vector<bool> m_bGroupable;
		std::vector< std::vector<std::wstring> > m_cols;

		std::map<uint, NumberData> m_numberData;
		std::map<uint, GroupableStringData> m_groupableStringData;
		std::map<uint, UngroupableStringData> m_ungroupableStringData;

		uint m_selectedCol;

		// string to fetch metadata about samples
		wxString m_fetchString;

		// sample data table
		std::vector< std::wstring > m_sampleDataFields;
		std::vector< std::vector<std::wstring> > m_sampleData;

		// blank panel
		wxPanel* m_blankPanel;

		// number panel
		wxPanel* m_numberPanel;
		wxTextCtrl* m_textLessThan;
		wxTextCtrl* m_textLowerBound;
		wxTextCtrl* m_textUpperBound;
		wxTextCtrl* m_textGreaterThan;
		wxRadioButton* m_radioAllNumber;
		wxRadioButton* m_radioLessThan;
		wxRadioButton* m_radioBetween;
		wxRadioButton* m_radioGreaterThan;
		wxRadioButton* m_radioNoneNumber;

		// groupable string panel
		wxPanel* m_groupableStringPanel;
		wxTextCtrl* m_textFilterStr;
		wxListBox* m_listAvailable;
		wxListBox* m_listRetrieve;
		wxButton* m_btnAdd;
		wxButton* m_btnRemove;
		wxButton* m_btnClear;

		// ungroupable string panel
		wxPanel* m_ungroupableStringPanel;
		wxTextCtrl* m_textContainsStr;
		wxRadioButton* m_radioAllString;
		wxRadioButton* m_radioStringContains;
		wxRadioButton* m_radioNoneString;
	};

}

#endif 
