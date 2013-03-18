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

#include "../core/Precompiled.hpp"

#include "../gui/DbSelectSampleDlg.hpp"

#include "../gui/DbSelectAttributesDlg.hpp"

#include "../utils/StringTools.hpp"
#include "../utils/Log.hpp"

#include <cctype>
#include <algorithm>

using namespace GenGIS;
using namespace boost;
using namespace std;

const int ID_GRID                        = 10000;
const int ID_CHOICE_BOOK                 = 10001;
const int ID_POPUP_MNU_REMOVE_SELECTED   = 10002;
const int ID_POPUP_MNU_RETAIN_SELECTED   = 10003;
const int ID_POPUP_MNU_SORT_ASCENDING    = 10004;
const int ID_POPUP_MNU_RETAIN_DESCENDING = 10005;

DbSelectSampleDlg::DbSelectSampleDlg(wxWindow* parent) : DbSelectSampleLayout(parent)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	Init();
	ConnectEvents();
}

DbSelectSampleDlg::~DbSelectSampleDlg()
{
	DisconnectEvents();
}

void DbSelectSampleDlg::ConnectEvents()
{
	this->Connect( ID_POPUP_MNU_REMOVE_SELECTED, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectSampleDlg::OnRemoveSelectedSamples ) );
	this->Connect( ID_POPUP_MNU_RETAIN_SELECTED, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectSampleDlg::OnRetainSelectedSamples ) );

	this->Connect( ID_POPUP_MNU_SORT_ASCENDING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectSampleDlg::OnSortAscending ) );
	this->Connect( ID_POPUP_MNU_RETAIN_DESCENDING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectSampleDlg::OnSortDescending ) );
}

void DbSelectSampleDlg::DisconnectEvents()
{
	this->Disconnect( ID_POPUP_MNU_REMOVE_SELECTED, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectSampleDlg::OnRemoveSelectedSamples ) );
	this->Disconnect( ID_POPUP_MNU_RETAIN_SELECTED, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectSampleDlg::OnRetainSelectedSamples ) );

	this->Disconnect( ID_POPUP_MNU_SORT_ASCENDING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectSampleDlg::OnSortAscending ) );
	this->Disconnect( ID_POPUP_MNU_RETAIN_DESCENDING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectSampleDlg::OnSortDescending ) );
}

void DbSelectSampleDlg::Init()
{
	m_grid->SetSelectionMode(wxGrid::wxGridSelectRows);

	m_numberPanel = CreateNumberFilteringPage(m_choiceBook);
	m_stringPanel = CreateStringFilteringPage(m_choiceBook);
	m_datePanel = CreateDateFilteringPage(m_choiceBook);
	m_binaryPanel = CreateBinaryFilteringPage(m_choiceBook);
	m_blankPanel = new wxPanel(m_choiceBook);

	m_choiceBook->Update();
}

wxPanel* DbSelectSampleDlg::CreateNumberFilteringPage(wxBookCtrlBase *parent)
{
	wxPanel *panel = new wxPanel(parent);

	m_textMin = new wxTextCtrl(panel, wxID_ANY, _T("0"));
	m_textMax = new wxTextCtrl(panel, wxID_ANY, _T("1000"));

	m_chkNumberSelectNulls = new wxCheckBox(panel, wxID_ANY, wxT("Select null values"));
	m_chkNumberSelectNulls->SetValue(false);
	m_chkNumberSelectNulls->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DbSelectSampleDlg::OnSelectNumber ), NULL, this);

	wxBoxSizer *sizerMin = new wxBoxSizer(wxHORIZONTAL);
	sizerMin->Add(new wxStaticText(panel, wxID_ANY, _T("Minimum:")), 0, wxALIGN_CENTER|wxALL, 5);
	sizerMin->Add(m_textMin, 0, wxEXPAND|wxALL, 5);
	m_textMin->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DbSelectSampleDlg::OnSelectNumber ), NULL, this );

	wxBoxSizer *sizerMax = new wxBoxSizer(wxHORIZONTAL);
	sizerMax->Add(new wxStaticText(panel, wxID_ANY, _T("Maximum:")), 0, wxALIGN_CENTER|wxALL, 5);
	sizerMax->Add(m_textMax, 0, wxEXPAND|wxALL, 5);
	m_textMax->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DbSelectSampleDlg::OnSelectNumber ), NULL, this );

	wxButton* btnFilterSelected = new wxButton(panel, wxID_ANY, wxT("Filter selected"));
	btnFilterSelected->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleDlg::OnFilterSelectedNumber ), NULL, this );

	wxButton* btnFilterUnselected = new wxButton(panel, wxID_ANY, wxT("Filter unselected"));
	btnFilterUnselected->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleDlg::OnFilterUnselectedNumber ), NULL, this );

	wxBoxSizer *sizerFilter = new wxBoxSizer(wxHORIZONTAL);
	sizerFilter->Add(btnFilterSelected, 0, wxEXPAND|wxALL, 5);
	sizerFilter->Add(btnFilterUnselected, 0, wxEXPAND|wxALL, 5);

	wxBoxSizer *sizerPanel = new wxBoxSizer(wxVERTICAL);
	sizerPanel->Add(sizerMin, 0, wxEXPAND|wxALL);
	sizerPanel->Add(sizerMax, 0, wxEXPAND|wxALL);
	sizerPanel->Add(m_chkNumberSelectNulls, 0, wxEXPAND|wxALL, 5);
	sizerPanel->Add(new wxStaticLine( panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL ), 0, wxEXPAND | wxALL, 5 );
	sizerPanel->Add(sizerFilter, 0, wxALIGN_CENTER|wxEXPAND|wxALL, 5);
	sizerPanel->Add(0, 0, 1, wxEXPAND, 5);

	panel->SetSizer(sizerPanel);

	return panel;
}

wxPanel* DbSelectSampleDlg::CreateDateFilteringPage(wxBookCtrlBase *parent)
{
	wxPanel *panel = new wxPanel(parent);

	m_startDate = new wxCalendarCtrl(panel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxCAL_SUNDAY_FIRST);
	m_startDate->Connect(wxEVT_CALENDAR_SEL_CHANGED, wxCommandEventHandler( DbSelectSampleDlg::OnSelectDate ), NULL, this);

	m_endDate = new wxCalendarCtrl(panel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxCAL_SUNDAY_FIRST);
	m_endDate->Connect(wxEVT_CALENDAR_SEL_CHANGED, wxCommandEventHandler( DbSelectSampleDlg::OnSelectDate ), NULL, this);

	m_chkDateSelectNulls = new wxCheckBox(panel, wxID_ANY, wxT("Select null values"));
	m_chkDateSelectNulls->SetValue(false);
	m_chkDateSelectNulls->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DbSelectSampleDlg::OnSelectDate ), NULL, this);

	wxBoxSizer *sizerStartDate = new wxBoxSizer(wxVERTICAL);
	sizerStartDate->Add(new wxStaticText(panel, wxID_ANY, _T("Start date:")), 0, wxALIGN_CENTER|wxALL, 5);
	sizerStartDate->Add(m_startDate, 0, wxEXPAND|wxALL, 5);

	wxBoxSizer *sizerEndDate = new wxBoxSizer(wxVERTICAL);
	sizerEndDate->Add(new wxStaticText(panel, wxID_ANY, _T("End date:")), 0, wxALIGN_CENTER|wxALL, 5);
	sizerEndDate->Add(m_endDate, 0, wxEXPAND|wxALL, 5);

	wxButton* btnFilterSelected = new wxButton(panel, wxID_ANY, wxT("Filter selected"));
	btnFilterSelected->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleDlg::OnFilterSelectedDate), NULL, this );

	wxButton* btnFilterUnselected = new wxButton(panel, wxID_ANY, wxT("Filter unselected"));
	btnFilterUnselected->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleDlg::OnFilterUnselectedDate ), NULL, this );

	wxBoxSizer *sizerFilter = new wxBoxSizer(wxHORIZONTAL);
	sizerFilter->Add(btnFilterSelected, 0, wxEXPAND|wxALL, 5);
	sizerFilter->Add(btnFilterUnselected, 0, wxEXPAND|wxALL, 5);

	wxBoxSizer *sizerPanel = new wxBoxSizer(wxVERTICAL);
	sizerPanel->Add(sizerStartDate, 0, wxEXPAND|wxALL);
	sizerPanel->Add(sizerEndDate, 0, wxEXPAND|wxALL);
	sizerPanel->Add(m_chkDateSelectNulls, 0, wxEXPAND|wxALL, 5);
	sizerPanel->Add(new wxStaticLine( panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL ), 0, wxEXPAND | wxALL, 5 );
	sizerPanel->Add(sizerFilter, 0, wxALIGN_CENTER|wxEXPAND|wxALL, 5);
	sizerPanel->Add(0, 0, 1, wxEXPAND, 5);

	panel->SetSizer(sizerPanel);

	return panel;
}

wxPanel* DbSelectSampleDlg::CreateStringFilteringPage(wxBookCtrlBase *parent)
{
	wxPanel *panel = new wxPanel(parent);

	wxArrayString choices;
	choices.Add(_T("Contains..."));
	choices.Add(_T("Starts with..."));
	m_cboTextFilterType = new wxComboBox(panel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, choices, wxCB_READONLY);
	m_cboTextFilterType->SetSelection(1);

	m_textFilterStr = new wxTextCtrl(panel, wxID_ANY, _T(""));
	m_textFilterStr->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DbSelectSampleDlg::OnSelectString ), NULL, this );

	m_chkStringSelectNulls = new wxCheckBox(panel, wxID_ANY, _T("Select null values"));
	m_chkStringSelectNulls->SetValue(false);
	m_chkStringSelectNulls->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DbSelectSampleDlg::OnSelectString ), NULL, this);

	wxButton* btnFilterSelected = new wxButton(panel, wxID_ANY, wxT("Filter selected"));
	btnFilterSelected->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleDlg::OnFilterSelectedString ), NULL, this );

	wxButton* btnFilterUnselected = new wxButton(panel, wxID_ANY, wxT("Filter unselected"));
	btnFilterUnselected->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleDlg::OnFilterUnselectedString ), NULL, this );

	wxBoxSizer *sizerFilter = new wxBoxSizer(wxHORIZONTAL);
	sizerFilter->Add(btnFilterSelected, 0, wxEXPAND|wxALL, 5);
	sizerFilter->Add(btnFilterUnselected, 0, wxEXPAND|wxALL, 5);

	wxBoxSizer *sizerPanel = new wxBoxSizer(wxVERTICAL);
	sizerPanel->Add(m_cboTextFilterType, 0, wxEXPAND|wxALL, 5);
	sizerPanel->Add(m_textFilterStr, 0, wxEXPAND|wxALL, 5);
	sizerPanel->Add(m_chkStringSelectNulls, 0, wxEXPAND|wxALL, 5);
	sizerPanel->Add(new wxStaticLine( panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL ), 0, wxEXPAND | wxALL, 5 );
	sizerPanel->Add(sizerFilter, 0, wxALIGN_CENTER|wxEXPAND|wxALL, 5);
	sizerPanel->Add(0, 0, 1, wxEXPAND, 5);

	panel->SetSizer(sizerPanel);

	return panel;
}

wxPanel* DbSelectSampleDlg::CreateBinaryFilteringPage(wxBookCtrlBase *parent)
{
	wxPanel *panel = new wxPanel(parent);

	m_radioChoice1 = new wxRadioButton(panel, wxID_ANY, _T("Choice1"));
	m_radioChoice1->Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DbSelectSampleDlg::OnSelectBinary ), NULL, this );

	m_radioChoice2 = new wxRadioButton(panel, wxID_ANY, _T("Choice2"));
	m_radioChoice2->Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DbSelectSampleDlg::OnSelectBinary ), NULL, this );

	m_chkBinarySelectNulls = new wxCheckBox(panel, wxID_ANY, _T("Select null values"));
	m_chkBinarySelectNulls->SetValue(false);
	m_chkBinarySelectNulls->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DbSelectSampleDlg::OnSelectBinary), NULL, this);

	wxButton* btnFilterSelected = new wxButton(panel, wxID_ANY, wxT("Filter selected"));
	btnFilterSelected->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleDlg::OnFilterSelectedBinary ), NULL, this );

	wxButton* btnFilterUnselected = new wxButton(panel, wxID_ANY, wxT("Filter unselected"));
	btnFilterUnselected->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectSampleDlg::OnFilterUnselectedBinary ), NULL, this );

	wxBoxSizer *sizerFilter = new wxBoxSizer(wxHORIZONTAL);
	sizerFilter->Add(btnFilterSelected, 0, wxEXPAND|wxALL, 5);
	sizerFilter->Add(btnFilterUnselected, 0, wxEXPAND|wxALL, 5);

	wxBoxSizer *sizerPanel = new wxBoxSizer(wxVERTICAL);
	sizerPanel->Add(m_radioChoice1, 0, wxEXPAND|wxALL, 5);
	sizerPanel->Add(m_radioChoice2, 0, wxEXPAND|wxALL, 5);
	sizerPanel->Add(m_chkBinarySelectNulls, 0, wxEXPAND|wxALL, 5);
	sizerPanel->Add(new wxStaticLine( panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL ), 0, wxEXPAND | wxALL, 5 );
	sizerPanel->Add(sizerFilter, 0, wxALIGN_CENTER|wxEXPAND|wxALL, 5);
	sizerPanel->Add(0, 0, 1, wxEXPAND, 5);

	panel->SetSizer(sizerPanel);

	return panel;
}

bool DbSelectSampleDlg::ReadData(const std::wstring& file)
{
	wifstream fin(string(file.begin(),file.end()).c_str());
	if(!fin.is_open()) 
	{
		Log::Inst().Error("(Warning) Failed to open database metadata file.");	
		return false;
	}

	bool bSuccess = ReadFieldData(fin);
	if(!bSuccess)
	{
		fin.close();
		return false;
	}

	bSuccess = ReadSampleData(fin);
	if(!bSuccess)
	{
		fin.close();
		return false;
	}

	fin.close();
	return true;
}

bool DbSelectSampleDlg::ReadFieldData(std::wifstream& fin)
{
	m_fields.clear();
	m_fieldDataTypes.clear();

	// read in column names
	wstring fields;
	getline(fin, fields);

	// break string into comma seperated tokens
	tokenizer< escaped_list_separator<wchar_t>, wstring::const_iterator, wstring > dataTokens(fields);

	// process each token
	uint fieldIndex = 0;
	tokenizer< escaped_list_separator<wchar_t>, wstring::const_iterator, wstring >::iterator dataIter; 
	for(dataIter = dataTokens.begin(); dataIter != dataTokens.end(); ++dataIter)
	{
		wstring fieldWithType = *dataIter;

		int typeStart = fieldWithType.find_first_of(_T('('));
		int typeEnd = fieldWithType.find_last_of(_T(')'));
		if(typeStart != wstring::npos && typeEnd != wstring::npos)
		{
			wstring field = fieldWithType.substr(0, typeStart);
			wstring type = fieldWithType.substr(typeStart+1, typeEnd-typeStart-1);

			m_fields.push_back(field);
			m_fieldDataTypes.push_back(type);

			if(fieldWithType.find(_T('*')) != wstring::npos)
				m_dbKeys.push_back(fieldIndex);

			fieldIndex++;
		}
		else
		{
			Log::Inst().Error("(Error) Error parsing sample file.");
			return false;
		}
	}

	return true;
}

bool DbSelectSampleDlg::ReadSampleData(std::wifstream& fin)
{
	m_rows.clear();
	
	// read in column names
	wstring data;
	while(getline(fin, data)) 
	{
		if(data.empty())
			break;

		tokenizer< escaped_list_separator<wchar_t>, wstring::const_iterator, wstring > dataTokens(data);		

		uint numTokens = 0;
		vector<wstring> row;
		tokenizer< escaped_list_separator<wchar_t>, wstring::const_iterator, wstring >::iterator dataIter; 
		for(dataIter = dataTokens.begin(); dataIter != dataTokens.end(); ++dataIter)
		{
			row.push_back(*dataIter);

			numTokens++;
		}

		if(numTokens == m_fields.size())
		{
			m_rows.push_back(row);
		}
		else
		{
			Log::Inst().Error("(Error) Problem parsing sample data.");	
			return false;
		}
	}

	return true;
}

void DbSelectSampleDlg::PopulateTable(const vector<wstring>& fields, const vector< vector<wstring> >& rows)
{
	m_grid->Freeze();
	m_choiceBook->ClearBackground();

	// set size of table
	m_grid->DeleteCols(0, m_grid->GetNumberCols());
	m_grid->AppendCols(fields.size());
	m_grid->DeleteRows(0, m_grid->GetNumberRows());
	m_grid->AppendRows(rows.size());

	// set field labels
	for(uint i = 0; i < fields.size(); ++i)
	{
		wstring dataType = m_fieldDataTypes.at(i);
		if(dataType == _T("Number"))
			m_choiceBook->AddPage(m_numberPanel, wxString(fields.at(i).c_str(), wxConvISO8859_1));
		else if(dataType == _T("String"))
			m_choiceBook->AddPage(m_stringPanel, wxString(fields.at(i).c_str(), wxConvISO8859_1));
		else if(dataType == _T("Date"))
			m_choiceBook->AddPage(m_datePanel, wxString(fields.at(i).c_str(), wxConvISO8859_1));
		else if(dataType == _T("Binary"))
			m_choiceBook->AddPage(m_binaryPanel, wxString(fields.at(i).c_str(), wxConvISO8859_1));
		else
			m_choiceBook->AddPage(m_blankPanel, wxString(fields.at(i).c_str(), wxConvISO8859_1));
		m_grid->SetColLabelValue(i, wxString(fields.at(i).c_str(), wxConvISO8859_1));
	}

	// set field values
	for(uint i = 0; i < rows.size(); ++i)
	{
		for(uint j = 0; j < fields.size(); ++j)
		{
			m_grid->SetCellValue(i, j, wxString(rows.at(i).at(j).c_str(), wxConvISO8859_1));
		}
	}		

	m_grid->AutoSizeColumns();
	m_grid->Thaw();

	// *** hack to get the page to redraw properly 
	m_choiceBook->SetSelection(1);
	m_choiceBook->SetSelection(0);
}

void DbSelectSampleDlg::OnLabelLeftClick(wxGridEvent& event)
{
	int col = event.GetCol();
	if(col != -1 && col < (int)m_choiceBook->GetPageCount())
	{
		m_choiceBook->SetSelection(col);
	}
}

void DbSelectSampleDlg::OnLabelRightClick(wxGridEvent& event)
{
	if(event.GetCol() != -1 && event.GetCol() < (int)m_choiceBook->GetPageCount())
	{
		wxMenu popupMenu;
		popupMenu.Append(ID_POPUP_MNU_SORT_ASCENDING, wxT("Ascending sort"));
		popupMenu.Append(ID_POPUP_MNU_RETAIN_DESCENDING, wxT("Desending sort"));

		this->PopupMenu(&popupMenu);
	}
}

void DbSelectSampleDlg::OnFieldChanged(wxChoicebookEvent& event)
{
	int index = m_choiceBook->GetSelection();
	wstring dataType = m_fieldDataTypes.at(index);
	if(dataType == _T("Number"))
	{
		// set minimum and maximum values
		double min = numeric_limits<double>::max();
		double max = -numeric_limits<double>::max();
		for(int r = 0; r < m_grid->GetNumberRows(); ++r)
		{
			wxString data = m_grid->GetCellValue(r, index);

			double value;
			bool bValid = data.ToDouble(&value);
			if(bValid)
			{
				if(value > max)
					max = value;
				if(value < min)
					min = value;
			}
		}

		wxString minStr = wxString::Format(_T("%f"), min);
		if(minStr.find(_T(".")) != -1)
		{
			minStr = minStr.Left(minStr.find_last_not_of(_T(".0"))+1);
		}

		wxString maxStr = wxString::Format(_T("%f"), max);
		if(maxStr.find(_T(".")) != -1)
			maxStr = maxStr.Left(maxStr.find_last_not_of(_T(".0"))+1);

		m_textMin->SetValue(minStr);
		m_textMax->SetValue(maxStr);
	}
	else if(dataType == _T("String"))
	{
		m_textFilterStr->SetValue(_T(""));
	}
	else if(dataType == _T("Date"))
	{
		// set minimum and maximum values
		wxDateTime earliestDate, latestDate;
		earliestDate.ParseDate(_T("1/1/9999"));
		latestDate.ParseDate(_T("1/1/0"));
		for(int r = 0; r < m_grid->GetNumberRows(); ++r)
		{
			wxString data = m_grid->GetCellValue(r, index);
			wxDateTime date;
			const wxChar *converted = date.ParseDate(data);
			if(converted != NULL)
			{
				if(date.IsEarlierThan(earliestDate))
					earliestDate = date;

				if(date.IsLaterThan(latestDate))
					latestDate = date;
			}
		}

		m_startDate->SetDate(earliestDate);
		m_endDate->SetDate(latestDate);
	}
	else if(dataType == _T("Binary"))
	{
		set<wxString> values;
		for(int r = 0; r < m_grid->GetNumberRows(); ++r)
		{
			wxString data = m_grid->GetCellValue(r, index);
			if(!data.empty() && data != _T("none"))
			{
				values.insert(data);
				if(values.size() == 2)
					break;
			}
		}

		if(values.size() == 2)
		{
			m_radioChoice1->SetLabel(*values.begin());
			m_radioChoice2->SetLabel(*(++values.begin()));
		}

		m_radioChoice1->SetValue(true);
		m_grid->ClearSelection();
		OnModifyBinaryCol(MODIFIER_SELECT);
	}
	else
	{
		Log::Inst().Warning("(Warning) DbSelectSampleDlg::OnFieldChanged(): Unknown data type.");
	}
}

void DbSelectSampleDlg::OnRestoreAllSamples( wxCommandEvent& event )
{
	PopulateTable(m_fields, m_rows);
}

void DbSelectSampleDlg::OnContinue( wxCommandEvent& event )
{
	wxString dbString = wxString(App::Inst().GetDatabaseConnectionString().c_str(), wxConvISO8859_1);
	wxString connectString = dbString.substr(0, dbString.find_last_of(_T("/")));

	int typePosStart = dbString.find(_T("type="));
	wxString typeString = dbString.substr(typePosStart);

	wxString sampleMetadataStr = connectString;
	sampleMetadataStr += _T("/fetch_sample_attribute_sets.cgi?");
	sampleMetadataStr += typeString;

	// generate RESTful string for acquiring metadata
	sampleMetadataStr += _T("&samples=");
	for(int r = 0; r < m_grid->GetNumberRows(); ++r)
	{
		wxString projectId = m_grid->GetCellValue(r, PROJECT_ID_COLUMN);
		wxString sampleId = m_grid->GetCellValue(r, SAMPLE_ID_COLUMN);
		sampleMetadataStr += projectId + _T(",") + sampleId;
		if(r != m_grid->GetNumberRows()-1)
			sampleMetadataStr += _T("+");
	}

	// grab selected samples along with desired metadata
	vector< vector<wstring> > selectedSamples;
	for(int i = 0; i < m_grid->GetNumberRows(); ++i)
	{
		vector<wstring> row;
		for(int j = 0; j < m_grid->GetNumberCols(); ++j)
		{
			row.push_back(m_grid->GetCellValue(i, j).c_str());
		}
		selectedSamples.push_back(row);
	}			

	// grad field headers
	vector<wstring> fieldHeaders;
	for(int i = 0; i < m_grid->GetNumberCols(); ++i)
		fieldHeaders.push_back(m_grid->GetColLabelValue(i).c_str());

	// open attribute selection dialog
	DbSelectAttributesDlg* dlg = new DbSelectAttributesDlg(sampleMetadataStr, fieldHeaders, selectedSamples, App::Inst().GetMainWindow());
	dlg->Show();	

	Destroy();
}

void DbSelectSampleDlg::OnFilterSelectedNumber( wxCommandEvent& event )
{
	OnModifyNumberCol(MODIFIER_DELETE_SELECTED);
}

void DbSelectSampleDlg::OnFilterUnselectedNumber( wxCommandEvent& event )
{
	OnModifyNumberCol(MODIFIER_DELETE_UNSELECTED);
}

void DbSelectSampleDlg::OnSelectNumber( wxCommandEvent& event )
{
	m_grid->ClearSelection();
	OnModifyNumberCol(MODIFIER_SELECT);
}

void DbSelectSampleDlg::OnModifyNumberCol(MODIFIER modifier)
{
	double min, max;
	bool bValid = m_textMin->GetValue().ToDouble(&min);
	bValid &= m_textMax->GetValue().ToDouble(&max);
	if(!bValid)
	{
		if(modifier == MODIFIER_DELETE_SELECTED)
			wxMessageBox(_T("The minimum and maximum values must be numerical."), _T("Filtering failed."), wxOK | wxICON_INFORMATION);
		else
			return;	// user is interacting with textbox
	}

	bool bSelectNulls = m_chkNumberSelectNulls->GetValue();

	int colIndex = m_choiceBook->GetSelection();	

	// find rows to filter
	m_grid->Freeze();
	for(int r = m_grid->GetNumberRows()-1; r >= 0; --r)
	{
		wxString data = m_grid->GetCellValue(r, colIndex);
		std::transform(data.begin(), data.end(), data.begin(), (int(*)(int)) std::tolower);
		if(data.empty() || data == _T("none"))
		{
			if(bSelectNulls)
			{
				if(modifier == MODIFIER_DELETE_SELECTED)
					m_grid->DeleteRows(r);
				else if(modifier == MODIFIER_SELECT)
					m_grid->SelectRow(r, true);
			}
			else
			{
				if(modifier == MODIFIER_DELETE_UNSELECTED)
					m_grid->DeleteRows(r);
			}
		}
		else
		{
			double value;
			data.ToDouble(&value);

			if(value < min || value > max)
			{
				if(modifier == MODIFIER_DELETE_UNSELECTED)
					m_grid->DeleteRows(r);				
			}
			else
			{
				if(modifier == MODIFIER_DELETE_SELECTED)
					m_grid->DeleteRows(r);
				else if(modifier == MODIFIER_SELECT)
					m_grid->SelectRow(r, true);
			}
		}
	}	
	m_grid->Thaw();
}

void DbSelectSampleDlg::OnFilterSelectedString( wxCommandEvent& event )
{
	OnModifyStringCol(MODIFIER_DELETE_SELECTED);
}

void DbSelectSampleDlg::OnFilterUnselectedString( wxCommandEvent& event )
{
	OnModifyStringCol(MODIFIER_DELETE_UNSELECTED);
}

void DbSelectSampleDlg::OnSelectString( wxCommandEvent& event )
{
	m_grid->ClearSelection();
	OnModifyStringCol(MODIFIER_SELECT);	
}

void DbSelectSampleDlg::OnModifyStringCol(MODIFIER modifier)
{
	wstring filterStr = wstring(m_textFilterStr->GetValue().wchar_str());
	wstring filterType = wstring(m_cboTextFilterType->GetValue().wchar_str());
	bool bSelectNulls = m_chkStringSelectNulls->GetValue();

	std::transform(filterStr.begin(), filterStr.end(), filterStr.begin(), (int(*)(int)) std::tolower);

	int colIndex = m_choiceBook->GetSelection();	

	// find rows to filter
	m_grid->Freeze();
	for(int r = m_grid->GetNumberRows()-1; r >= 0; --r)
	{
		wxString data = m_grid->GetCellValue(r, colIndex);
		std::transform(data.begin(), data.end(), data.begin(), (int(*)(int)) std::tolower);
		if(data.empty() || data == _T("none"))
		{
			if(bSelectNulls)
			{
				if(modifier == MODIFIER_DELETE_SELECTED)
					m_grid->DeleteRows(r);
				else if(modifier == MODIFIER_SELECT)
					m_grid->SelectRow(r, true);
			}
			else
			{
				if(modifier == MODIFIER_DELETE_UNSELECTED)
					m_grid->DeleteRows(r);
			}
		}
		else
		{
			if(filterType == _T("Contains..."))
			{
				if(data.Find(filterStr.c_str()) != -1 && !filterStr.empty())
				{
					if(modifier == MODIFIER_DELETE_SELECTED)
						m_grid->DeleteRows(r);
					else if(modifier == MODIFIER_SELECT)
						m_grid->SelectRow(r, true);
				}
				else
				{
					if(modifier == MODIFIER_DELETE_UNSELECTED)
						m_grid->DeleteRows(r);
				}
			}
			else if(filterType == _T("Starts with..."))
			{
				if(data.Find(filterStr.c_str()) == 0 && !filterStr.empty())
				{
					if(modifier == MODIFIER_DELETE_SELECTED)
						m_grid->DeleteRows(r);
					else if(modifier == MODIFIER_SELECT)
						m_grid->SelectRow(r, true);
				}
				else
				{
					if(modifier == MODIFIER_DELETE_UNSELECTED)
						m_grid->DeleteRows(r);
				}
			}
		}
	}	
	m_grid->Thaw();	
}

void DbSelectSampleDlg::OnFilterSelectedDate( wxCommandEvent& event )
{
	OnModifyDateCol(MODIFIER_DELETE_SELECTED);
}

void DbSelectSampleDlg::OnFilterUnselectedDate( wxCommandEvent& event )
{
	OnModifyDateCol(MODIFIER_DELETE_UNSELECTED);
}

void DbSelectSampleDlg::OnSelectDate( wxCommandEvent& event )
{
	m_grid->ClearSelection();
	OnModifyDateCol(MODIFIER_SELECT);
}

void DbSelectSampleDlg::OnModifyDateCol(MODIFIER modifier)
{
	wxDateTime startDate = m_startDate->GetDate();
	wxDateTime endDate = m_endDate->GetDate();
	bool bSelectNulls = m_chkDateSelectNulls->GetValue();

	int colIndex = m_choiceBook->GetSelection();	

	// find rows to filter
	m_grid->Freeze();
	for(int r = m_grid->GetNumberRows()-1; r >= 0; --r)
	{
		wxString data = m_grid->GetCellValue(r, colIndex);
		std::transform(data.begin(), data.end(), data.begin(), (int(*)(int)) std::tolower);
		if(data.empty() || data == _T("none"))
		{
			if(bSelectNulls)
			{
				if(modifier == MODIFIER_DELETE_SELECTED)
					m_grid->DeleteRows(r);
				else if(modifier == MODIFIER_SELECT)
					m_grid->SelectRow(r, true);
			}
			else
			{
				if(modifier == MODIFIER_DELETE_UNSELECTED)
					m_grid->DeleteRows(r);
			}
		}
		else
		{
			wxDateTime date;
			const wxChar* conversion = date.ParseDate(data);
			if(conversion != NULL)
			{
				if(!date.IsBetween(startDate, endDate))
				{
					if(modifier == MODIFIER_DELETE_UNSELECTED)
						m_grid->DeleteRows(r);
				}
				else
				{
					if(modifier == MODIFIER_DELETE_SELECTED)
						m_grid->DeleteRows(r);
					else if(modifier == MODIFIER_SELECT)
						m_grid->SelectRow(r, true);
				}
			}
			else
			{
				// effectively treat row as being a NULL
				if(bSelectNulls)
				{
					if(modifier == MODIFIER_DELETE_UNSELECTED)
						m_grid->DeleteRows(r);
				}
				else
				{
					if(modifier == MODIFIER_DELETE_SELECTED)
						m_grid->DeleteRows(r);
					else if(modifier == MODIFIER_SELECT)
						m_grid->SelectRow(r, true);
				}
			}
		}
	}	
	m_grid->Thaw();	
}

void DbSelectSampleDlg::OnFilterSelectedBinary( wxCommandEvent& event )
{
	OnModifyBinaryCol(MODIFIER_DELETE_SELECTED);
}

void DbSelectSampleDlg::OnFilterUnselectedBinary( wxCommandEvent& event )
{
	OnModifyBinaryCol(MODIFIER_DELETE_UNSELECTED);
}

void DbSelectSampleDlg::OnSelectBinary( wxCommandEvent& event )
{
	m_grid->ClearSelection();
	OnModifyBinaryCol(MODIFIER_SELECT);
}

void DbSelectSampleDlg::OnModifyBinaryCol(MODIFIER modifier)
{
	wstring valueToRetain;
	if(m_radioChoice1->GetValue())
		valueToRetain = m_radioChoice1->GetLabel();
	else
		valueToRetain = m_radioChoice2->GetLabel();

	std::transform(valueToRetain.begin(), valueToRetain.end(), valueToRetain.begin(), (int(*)(int)) std::tolower);

	bool bSelectNulls = m_chkBinarySelectNulls->GetValue();

	int colIndex = m_choiceBook->GetSelection();	

	// find rows to filter
	m_grid->Freeze();
	for(int r = m_grid->GetNumberRows()-1; r >= 0; --r)
	{
		wxString data = m_grid->GetCellValue(r, colIndex);
		std::transform(data.begin(), data.end(), data.begin(), (int(*)(int)) std::tolower);
		if(data.empty() || data == _T("none"))
		{
			if(bSelectNulls)
			{
				if(modifier == MODIFIER_DELETE_SELECTED)
					m_grid->DeleteRows(r);
				else if(modifier == MODIFIER_SELECT)
					m_grid->SelectRow(r, true);
			}
			else
			{
				if(modifier == MODIFIER_DELETE_UNSELECTED)
					m_grid->DeleteRows(r);
			}
		}
		else
		{
			if(data != wxString(valueToRetain.c_str(), wxConvISO8859_1))
			{
				if(modifier == MODIFIER_DELETE_UNSELECTED)
					m_grid->DeleteRows(r);
			}
			else
			{
				if(modifier == MODIFIER_DELETE_SELECTED)
					m_grid->DeleteRows(r);
				else if(modifier == MODIFIER_SELECT)
					m_grid->SelectRow(r, true);
			}
		}
	}	
	m_grid->Thaw();
}

void DbSelectSampleDlg::OnFetchSamplesFromDB( wxCommandEvent& event )
{
	wxBusyCursor wait;

	wxFileSystem fs;
	wxString connectionStr = wxString(App::Inst().GetDatabaseConnectionString().c_str(), wxConvISO8859_1);
	wxFSFile* file = fs.OpenFile(connectionStr);
	if(file == NULL)
	{
		wxMessageBox(_T("Connection string failed:\n") + connectionStr, _T("Connection failed."), wxOK | wxICON_INFORMATION);
		return;
	}
	wxInputStream* stream = file->GetStream();
	
	wstring tempFile = wstring(GenGIS::App::Inst().GetExeDir().wchar_str()) + _T("/Database/_db_samples.csv");
	wofstream output(string(tempFile.begin(), tempFile.end()).c_str(), std::ios::out);
	if(!output.is_open()) 
	{
		Log::Inst().Error("(Error) Failed to open temp file for storing database stream.");	
		return;
	}

	while(!stream->Eof())
	{
		char ch = stream->GetC();
		if(stream->LastRead() == 1 && ch != '\\')
			output << ch;
	}
	output.close();

	wxDELETE(file);

	ReadData(tempFile);
	PopulateTable(m_fields, m_rows);
}

void DbSelectSampleDlg::OnLoadSampleList( wxCommandEvent& event )
{
	wxFileDialog* openDlg = new wxFileDialog(this, wxT("Open sample list..."), wxEmptyString,
		wxEmptyString, wxT("*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	wxString wildcards = wxT("Comma separated values (*.csv)|*.csv|")
		wxT("All files (*.*)|*.*");
	openDlg->SetWildcard(wildcards);

	if(openDlg->ShowModal() == wxID_OK)
	{
		std::wstring filename = openDlg->GetFilename().wc_str(*wxConvCurrent);
		std::wstring path = openDlg->GetDirectory().wc_str(*wxConvCurrent);

		ReadData(path + _T("/") + filename);

		PopulateTable(m_fields, m_rows);
	}

	openDlg->Destroy();
}

void DbSelectSampleDlg::OnSaveSampleList( wxCommandEvent& event )
{
	wxFileDialog* saveDlg = new wxFileDialog(this, wxT("Save sample list..."), wxEmptyString, wxEmptyString, wxT("*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);

	wxString wildcards = wxT("Comma separated values (*.csv)|*.csv");
	saveDlg->SetWildcard(wildcards);

	if(saveDlg->ShowModal() == wxID_OK)
	{
		std::wstring filename = wstring((saveDlg->GetDirectory() + _T("/") + saveDlg->GetFilename()).wchar_str());
		wofstream output(string(filename.begin(),filename.end()).c_str(), std::ios::out);
		if(!output.is_open()) 
		{
			Log::Inst().Warning("(Warning) Failed to save sample list.");	
			return;
		}

		// output field info
		for(uint i = 0; i < m_fields.size(); ++i)
		{
			output << m_fields.at(i) << _T("(") << m_fieldDataTypes.at(i);
			if(i != m_fields.size()-1)
				output << _T("),");
			else
				output << _T(")") << endl;
		}

		// output sample data
		for(int r = 0; r < m_grid->GetNumberRows(); ++r)
		{
			for(int c = 0;	c < m_grid->GetNumberCols(); ++c)
			{
				wxString data = m_grid->GetCellValue(r, c);
				if(m_fieldDataTypes.at(c) == _T("String"))
				{
					output << _T("\"") << data.c_str() << _T("\"");
				}
				else
				{
					output << data.c_str();
				}

				if(c != m_grid->GetNumberCols()-1)
					output << _T(",");
			}
			output << endl;
		}
	}

	saveDlg->Destroy();
}

void DbSelectSampleDlg::OnGridRightClick(wxGridEvent& event)
{
	wxMenu popupMenu;
	popupMenu.Append(ID_POPUP_MNU_REMOVE_SELECTED, wxT("Remove selected samples"));
	popupMenu.Append(ID_POPUP_MNU_RETAIN_SELECTED, wxT("Retain selected samples"));

	this->PopupMenu(&popupMenu);
}

void DbSelectSampleDlg::OnRemoveSelectedSamples( wxCommandEvent& event )
{
	m_grid->Freeze();
	for(int r = m_grid->GetNumberRows()-1; r >= 0; --r)
	{
		if(m_grid->IsInSelection(r, 0))
			m_grid->DeleteRows(r);
	}	
	m_grid->Thaw();
}

void DbSelectSampleDlg::OnRetainSelectedSamples( wxCommandEvent& event )
{
	m_grid->Freeze();
	for(int r = m_grid->GetNumberRows()-1; r >= 0; --r)
	{
		if(!m_grid->IsInSelection(r, 0))
			m_grid->DeleteRows(r);
	}	
	m_grid->Thaw();
}

struct Sorter
{
	Sorter() {}
	Sorter(wstring  _data, int _index): data(_data), index(_index) {}

	wstring data;
	int index;
};

bool SortStringAscending(const Sorter& s1, const Sorter& s2)
{

	if(s1.data != _T("None") && s2.data == _T("None"))
		return true;

	if(s1.data == _T("None") && s2.data != _T("None"))
		return false;

	wstring str1 = s1.data;
	wstring str2 = s2.data;
	std::transform(str1.begin(), str1.end(), str1.begin(), (int(*)(int)) std::tolower);
	std::transform(str2.begin(), str2.end(), str2.begin(), (int(*)(int)) std::tolower);

	return str1 < str2;
}

bool SortStringDescending(const Sorter& s1, const Sorter& s2)
{
	if(s1.data != _T("None") && s2.data == _T("None"))
		return true;

	if(s1.data == _T("None") && s2.data != _T("None"))
		return false;

	wstring str1 = s1.data;
	wstring str2 = s2.data;
	std::transform(str1.begin(), str1.end(), str1.begin(), (int(*)(int)) std::tolower);
	std::transform(str2.begin(), str2.end(), str2.begin(), (int(*)(int)) std::tolower);

	return str1 > str2;
}

bool SortNumberAscending(const Sorter& s1, const Sorter& s2)
{
	if(s1.data != _T("None") && s2.data == _T("None"))
		return true;

	if(s1.data == _T("None") && s2.data != _T("None"))
		return false;

	return StringTools::ToDouble(s1.data) < StringTools::ToDouble(s2.data);
}

bool SortNumberDescending(const Sorter& s1, const Sorter& s2)
{
	if(s1.data != _T("None") && s2.data == _T("None"))
		return true;

	if(s1.data == _T("None") && s2.data != _T("None"))
		return false;

	return StringTools::ToDouble(s1.data) > StringTools::ToDouble(s2.data);
}

bool SortDateAscending(const Sorter& s1, const Sorter& s2)
{
	if(s1.data != _T("None") && s2.data == _T("None"))
		return true;

	if(s1.data == _T("None") && s2.data != _T("None"))
		return false;

	wxDateTime date1, date2;
	date1.ParseDate(s1.data.c_str());
	date2.ParseDate(s2.data.c_str());

	return date1.IsEarlierThan(date2);
}

bool SortDateDescending(const Sorter& s1, const Sorter& s2)
{
	if(s1.data != _T("None") && s2.data == _T("None"))
		return true;

	if(s1.data == _T("None") && s2.data != _T("None"))
		return false;

	wxDateTime date1, date2;
	date1.ParseDate(s1.data.c_str());
	date2.ParseDate(s2.data.c_str());

	return date1.IsLaterThan(date2);
}

void DbSelectSampleDlg::OnSort( bool bAscending )
{
	int colIndex = m_choiceBook->GetSelection();	

	// get values
	std::vector<Sorter> columnSorter(m_grid->GetNumberRows());
	for(int r = 0; r < m_grid->GetNumberRows(); ++r)
	{
		columnSorter[r].data = m_grid->GetCellValue(r, colIndex);
		columnSorter[r].index = r;
	}

	// sort values
	if(bAscending)
	{
		if(m_fieldDataTypes.at(colIndex) == _T("String") || m_fieldDataTypes.at(colIndex) == _T("Binary"))
			sort(columnSorter.begin(), columnSorter.end(), SortStringAscending );
		else if(m_fieldDataTypes.at(colIndex) == _T("Number"))
			sort(columnSorter.begin(), columnSorter.end(), SortNumberAscending);
		else if(m_fieldDataTypes.at(colIndex) == _T("Date"))
			sort(columnSorter.begin(), columnSorter.end(), SortDateAscending );
		else
			return;	// unknown data type
	}
	else
	{
		if(m_fieldDataTypes.at(colIndex) == _T("String") || m_fieldDataTypes.at(colIndex) == _T("Binary"))
			sort(columnSorter.begin(), columnSorter.end(), SortStringDescending );
		else if(m_fieldDataTypes.at(colIndex) == _T("Number"))
			sort(columnSorter.begin(), columnSorter.end(), SortNumberDescending );
		else if(m_fieldDataTypes.at(colIndex) == _T("Date"))
			sort(columnSorter.begin(), columnSorter.end(), SortDateDescending );
		else
			return; // unknown data type
	}

	// sorted rows	
	vector< vector<wstring> > sortedRows(m_grid->GetNumberRows(), vector<wstring>(m_fields.size()));
	for(int i = 0; i < m_grid->GetNumberRows(); ++i)
	{
		for(uint j = 0; j < m_fields.size(); ++j)
		{
			uint index = columnSorter.at(i).index;
			wstring value = wstring(m_grid->GetCellValue(index, j).wchar_str());
			sortedRows[i][j] = value;
		}
	}		

	// display sorted rows

	m_grid->Freeze();
	for(int i = 0; i < m_grid->GetNumberRows(); ++i)
	{
		for(uint j = 0; j < m_fields.size(); ++j)
		{
			m_grid->SetCellValue(i, j, wxString(sortedRows.at(i).at(j).c_str(), wxConvISO8859_1));
		}
	}
	m_grid->Thaw();
}
