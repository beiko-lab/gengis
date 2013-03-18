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

#include "../gui/DbSelectAttributesDlg.hpp"

#include "../gui/DbSequenceSummaryDlg.hpp"

#include "../utils/StringTools.hpp"
#include "../utils/Log.hpp"

using namespace GenGIS;
using namespace boost;
using namespace std;

const int ID_GRID = 11000;
const int ID_CHOICE_BOOK = 11001;
const int ID_POPUP_MNU_REMOVE_SELECTED_SAMPLES = 11002;
const int ID_POPUP_MNU_RETAIN_SELECTED_SAMPLES = 11003;
const int ID_POPUP_MNU_REMOVE_FIELD = 11004;


DbSelectAttributesDlg::DbSelectAttributesDlg(const wxString& fetchString, std::vector< std::wstring > sampleDataFields,
	std::vector< std::vector<std::wstring> > sampleData, wxWindow* parent) :
		m_fetchString(fetchString), m_sampleDataFields(sampleDataFields), m_sampleData(sampleData), DbSelectAttributesLayout(parent)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	Init();
	ConnectEvents();
}

DbSelectAttributesDlg::~DbSelectAttributesDlg()
{
	DisconnectEvents();
}

void DbSelectAttributesDlg::ConnectEvents()
{
	this->Connect( ID_POPUP_MNU_REMOVE_SELECTED_SAMPLES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectAttributesDlg::OnRemoveSelectedSamples ) );
	this->Connect( ID_POPUP_MNU_RETAIN_SELECTED_SAMPLES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectAttributesDlg::OnRetainSelectedSamples ) );

	this->Connect( ID_POPUP_MNU_REMOVE_FIELD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectAttributesDlg::OnRemoveField ) );
}

void DbSelectAttributesDlg::DisconnectEvents()
{
	this->Disconnect( ID_POPUP_MNU_REMOVE_SELECTED_SAMPLES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectAttributesDlg::OnRemoveSelectedSamples ) );
	this->Disconnect( ID_POPUP_MNU_RETAIN_SELECTED_SAMPLES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectAttributesDlg::OnRetainSelectedSamples ) );

	this->Disconnect( ID_POPUP_MNU_REMOVE_FIELD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DbSelectAttributesDlg::OnRemoveField ) );
}

void DbSelectAttributesDlg::Init()
{
	m_grid->SetSelectionMode(wxGrid::wxGridSelectRows);

	m_numberPanel = CreateNumberFieldPage(m_choiceBook);
	m_groupableStringPanel = CreateGroupableStringPage(m_choiceBook);
	m_ungroupableStringPanel = CreateUngroupableStringPage(m_choiceBook);
	m_blankPanel = new wxPanel(m_choiceBook);
	
	FetchAttributesFromDB(m_fetchString);

	m_choiceBook->Update();
}


void DbSelectAttributesDlg::FetchAttributesFromDB(const wxString& fetchString)
{
	wxBusyCursor wait;

	m_fetchString = fetchString;

	wxFileSystem fs;
	wxFSFile* file = fs.OpenFile(m_fetchString);
	wxInputStream* stream = file->GetStream();

	wstring tempFile = wstring((GenGIS::App::Inst().GetUserDataDir() + _T("/_db_attributes.txt")).wchar_str());

	char* tempfilename = (char*)malloc(wcslen(tempFile.c_str()));
	wcstombs(tempfilename, tempFile.c_str(), wcslen(tempFile.c_str()));

	wofstream output(tempfilename, std::ios::out);
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
	PopulateTable(m_fields, m_cols);

	delete[] tempfilename;
}

wxPanel* DbSelectAttributesDlg::CreateNumberFieldPage(wxBookCtrlBase *parent)
{
	wxPanel *panel = new wxPanel(parent);

	m_textLessThan = new wxTextCtrl(panel, wxID_ANY, _T("0"));
	m_textLowerBound = new wxTextCtrl(panel, wxID_ANY, _T("0"));
	m_textUpperBound = new wxTextCtrl(panel, wxID_ANY, _T("0"));
	m_textGreaterThan = new wxTextCtrl(panel, wxID_ANY, _T("0"));
	m_radioAllNumber = new wxRadioButton(panel, wxID_ANY, _T("All"));
	m_radioLessThan = new wxRadioButton(panel, wxID_ANY, _T("Less than:"));
	m_radioBetween = new wxRadioButton(panel, wxID_ANY, _T("Between:"));
	m_radioGreaterThan = new wxRadioButton(panel, wxID_ANY, _T("Greater than:"));
	m_radioNoneNumber = new wxRadioButton(panel, wxID_ANY, _T("None"));

	wxBoxSizer* sizerRange = new wxBoxSizer(wxVERTICAL);
	sizerRange->Add(m_textLowerBound, 0, wxALIGN_CENTER|wxEXPAND|wxALL, 0);
	sizerRange->Add(new wxStaticText(panel, wxID_ANY, _T(" and ")), 0, wxALIGN_CENTER|wxALL, 1);
	sizerRange->Add(m_textUpperBound, 0, wxALIGN_CENTER|wxEXPAND|wxALL, 0);

	wxFlexGridSizer *sizerLessOrGreaterThan = new wxFlexGridSizer(2,2,0,0);
	sizerLessOrGreaterThan->Add(m_radioLessThan, 0, wxALL, 5);
	sizerLessOrGreaterThan->Add(m_textLessThan, 0, wxEXPAND|wxALL, 5);
	sizerLessOrGreaterThan->Add(m_radioGreaterThan, 0, wxALL, 5);
	sizerLessOrGreaterThan->Add(m_textGreaterThan, 0, wxEXPAND|wxALL, 5);
	sizerLessOrGreaterThan->Add(m_radioBetween, 0, wxALL, 5);
	sizerLessOrGreaterThan->Add(sizerRange, 0, wxEXPAND|wxALL, 5);

	wxBoxSizer *sizerPanel = new wxBoxSizer(wxVERTICAL);
	sizerPanel->Add(m_radioAllNumber, 0, wxALL, 5);
	sizerPanel->Add(m_radioNoneNumber, 0, wxALL, 5);
	sizerPanel->Add(sizerLessOrGreaterThan, 0, wxEXPAND|wxALL);
	sizerPanel->Add(0, 0, 1, wxEXPAND, 5);

	panel->SetSizer(sizerPanel);

	return panel;
}

wxPanel* DbSelectAttributesDlg::CreateGroupableStringPage(wxBookCtrlBase *parent)
{
	wxPanel *panel = new wxPanel(parent);

	m_textFilterStr = new wxTextCtrl(panel, wxID_ANY, _T("0"));
	m_listAvailable = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_EXTENDED|wxLB_NEEDED_SB|wxLB_HSCROLL|wxLB_SORT);
	m_listRetrieve = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_EXTENDED|wxLB_NEEDED_SB|wxLB_HSCROLL|wxLB_SORT);

	m_btnAdd = new wxButton(panel, wxID_ANY, wxT("Add"));
	m_btnAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectAttributesDlg::OnGroupableStringAdd ), NULL, this );

	m_btnRemove = new wxButton(panel, wxID_ANY, wxT("Remove"));
	m_btnRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectAttributesDlg::OnGroupableStringRemove ), NULL, this );

	m_btnClear = new wxButton(panel, wxID_ANY, wxT("Clear"));
	m_btnClear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSelectAttributesDlg::OnGroupableStringClear ), NULL, this );

	wxBoxSizer *sizerAvailable = new wxBoxSizer(wxVERTICAL);
	sizerAvailable->Add(new wxStaticText(panel, wxID_ANY, _T("Available:")), 0, wxLEFT|wxRIGHT|wxTOP, 5);
	sizerAvailable->Add(m_textFilterStr, 0, wxEXPAND|wxLEFT|wxRIGHT, 5);
	sizerAvailable->Add(m_listAvailable, 1, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer *sizerAddRemove = new wxBoxSizer(wxHORIZONTAL);
	sizerAddRemove->Add(0, 0, 1, wxEXPAND, 5);
	sizerAddRemove->Add(m_btnAdd, 0, wxLEFT|wxRIGHT|wxBOTTOM, 5);
	sizerAddRemove->Add(m_btnRemove, 0, wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer *sizerReturn = new wxBoxSizer(wxVERTICAL);
	sizerReturn->Add(new wxStaticText(panel, wxID_ANY, _T("Retrieve:")), 0, wxLEFT|wxRIGHT|wxTOP, 5);
	sizerReturn->Add(m_listRetrieve, 1, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer *sizerClear = new wxBoxSizer(wxHORIZONTAL);
	sizerClear->Add(0, 0, 1, wxEXPAND, 5);
	sizerClear->Add(m_btnClear, 0, wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer *sizerPanel = new wxBoxSizer(wxVERTICAL);
	sizerPanel->Add(sizerAvailable, 1, wxEXPAND|wxALL);
	sizerPanel->Add(sizerAddRemove, 0, wxEXPAND|wxALL);
	sizerPanel->Add(sizerReturn, 1, wxEXPAND|wxALL);
	sizerPanel->Add(sizerClear, 0, wxEXPAND|wxALL);

	panel->SetSizer(sizerPanel);

	return panel;
}

wxPanel* DbSelectAttributesDlg::CreateUngroupableStringPage(wxBookCtrlBase *parent)
{
	wxPanel *panel = new wxPanel(parent);

	m_textContainsStr = new wxTextCtrl(panel, wxID_ANY, _T("0"));
	m_radioAllString = new wxRadioButton(panel, wxID_ANY, _T("All"));
	m_radioStringContains = new wxRadioButton(panel, wxID_ANY, _T("Where item contains:"));
	m_radioNoneString = new wxRadioButton(panel, wxID_ANY, _T("None"));

	wxBoxSizer *sizerContains = new wxBoxSizer(wxVERTICAL);
	sizerContains->Add(m_radioStringContains, 0, wxTOP|wxRIGHT|wxLEFT, 5);
	sizerContains->Add(m_textContainsStr, 0, wxEXPAND|wxALL, 5);

	wxBoxSizer *sizerPanel = new wxBoxSizer(wxVERTICAL);
	sizerPanel->Add(m_radioAllString, 0, wxALL, 5);
	sizerPanel->Add(m_radioNoneString, 0, wxALL, 5);
	sizerPanel->Add(sizerContains, 0, wxEXPAND|wxALL);
	sizerPanel->Add(0, 0, 1, wxEXPAND, 5);

	panel->SetSizer(sizerPanel);

	return panel;
}

bool DbSelectAttributesDlg::ReadData(const std::wstring& file)
{
	m_fields.clear();
	m_fieldDataTypes.clear();
	m_bGroupable.clear();
	m_cols.clear();

	m_numberData.clear();
	m_groupableStringData.clear();
	m_ungroupableStringData.clear();

	char* tempfilename = (char*)malloc(wcslen(file.c_str()));
	wcstombs(tempfilename, file.c_str(), wcslen(file.c_str()));

	wifstream fin(tempfilename);
	if(!fin.is_open()) 
	{
		Log::Inst().Warning("(Warning) Failed to open file.");	
		return false;
	}

	// setup project and sample id field
	m_fields.push_back(_T("MOAmetagenome_project_id"));
	m_fieldDataTypes.push_back(_T("Number"));
	m_fieldMap[_T("MOAmetagenome_project_id")] = 0;
	m_bGroupable.push_back(false);

	m_fields.push_back(_T("sample_id"));
	m_fieldDataTypes.push_back(_T("String"));
	m_fieldMap[_T("sample_id")] = 1;
	m_bGroupable.push_back(true);

	// get all other fields
	wstring line;
	vector<wstring> col, sampleIds, projectIds;
	int colNum = 2;
	while(getline(fin, line)) 
	{
		if(line.empty())
			break;

		if(line.find(_T("ATTRIBUTE")) != wstring::npos)
		{
			int start = line.find_last_of(_T("|"));
			m_fields.push_back(line.substr(start+1));
			m_fieldMap[m_fields.at(m_fields.size()-1)] = colNum;
		}
		else if(line.find(_T("TYPE")) != wstring::npos)
		{
			int start = line.find_last_of(_T("|"));
			m_fieldDataTypes.push_back(line.substr(start+1));

		}
		else if(line.find(_T("GROUPABLE")) != wstring::npos)
		{
			int start = line.find_last_of(_T("|"));
			wstring groupable = line.substr(start+1);

			if(groupable == _T("0"))
			{
				m_bGroupable.push_back(false);

				if(m_fieldDataTypes.at(colNum) == _T("String"))
				{
					UngroupableStringData ungroupableStringData;
					m_ungroupableStringData[colNum] = ungroupableStringData;
				}
			}
			else
			{
				m_bGroupable.push_back(true);
			}

			col.clear();
		}
		else if(line.find(_T("DEFINED")) != wstring::npos)
		{
			line = StringTools::RemoveSurroundingWhiteSpaces(line);
			col.push_back(line.substr(line.find_first_of(_T(" "))+1));

			if(colNum == 2)
			{
				int projectIdStart = line.find_first_of(_T("|"))+1;
				int sampleIdStart = line.find_first_of(_T(","))+1;
				int sampleIdEnd = line.find_first_of(_T(" "));

				projectIds.push_back(line.substr(projectIdStart, sampleIdStart-projectIdStart-1));
				sampleIds.push_back(line.substr(sampleIdStart, sampleIdEnd-sampleIdStart));
			}
		}
		else if(line.find(_T("END")) != wstring::npos)
		{
			if(colNum == 2)
			{
				NumberData numberData;
				numberData.lessThan = numberData.lowerBound = wxString(projectIds.at(0).c_str(), wxConvISO8859_1);
				numberData.greaterThan = numberData.upperBound = wxString(projectIds.at(projectIds.size()-1).c_str(), wxConvISO8859_1);
				m_numberData[0] = numberData;

				GroupableStringData stringData;
				for(uint i = 0; i < sampleIds.size(); ++i)
					stringData.availableItems.push_back(wxString(sampleIds.at(i).c_str(), wxConvISO8859_1));
				m_groupableStringData[1] = stringData;

				m_cols.push_back(projectIds);
				m_cols.push_back(sampleIds);
			}

			m_cols.push_back(col);
			colNum++;
		}
		else
		{	
			// parse data for given data type
			if(m_fieldDataTypes.at(colNum) == _T("Number"))
				ReadNumberData(line, colNum);
			else if(m_fieldDataTypes.at(colNum) == _T("String") && m_bGroupable.at(colNum))
			{
				ReadGroupableStringData(line, fin, colNum);
				m_cols.push_back(col);
				colNum++;
			}
			else
			{
				Log::Inst().Warning("(Warning) Failed to parse data file.");	
				return false;
			}
		}
	}

	fin.close();
	return true;
}

bool DbSelectAttributesDlg::ReadNumberData(const std::wstring& line, int colNum)
{
	NumberData numberData;

	int posFirstPipe = line.find_first_of(_T("|"));
	int posLastPipe = line.find_last_of(_T("|"));
	wstring lowerBound = line.substr(posFirstPipe+1, posLastPipe-posFirstPipe-1);
	wstring upperBound = line.substr(posLastPipe+1);

	numberData.lessThan = numberData.lowerBound = wxString(lowerBound.c_str(), wxConvISO8859_1);
	numberData.greaterThan = numberData.upperBound = wxString(upperBound.c_str(), wxConvISO8859_1);

	m_numberData[colNum] = numberData;

	return true;
}

bool DbSelectAttributesDlg::ReadGroupableStringData(std::wstring& line, std::wifstream& fin, int colNum)
{
	GroupableStringData groupableStringData;

	do
	{
		if(line == _T(" END"))
			break;

		int posFirstPipe = line.find_first_of(_T("|"));
		int posLastPipe = line.find_last_of(_T("|"));

		wxString labelId = wxString(line.substr(posFirstPipe+1, posLastPipe-posFirstPipe-1).c_str(), wxConvISO8859_1);
		wxString labelStr = wxString(line.substr(posLastPipe+1).c_str(), wxConvISO8859_1);

		groupableStringData.labelMap[labelStr] = labelId;
		groupableStringData.availableItems.push_back(labelStr);
	}
	while(getline(fin, line));

	m_groupableStringData[colNum] = groupableStringData;

	return true;
}

void DbSelectAttributesDlg::PopulateTable(const vector<wstring>& fields, const vector< vector<wstring> >& cols)
{
	m_grid->Freeze();
	m_choiceBook->ClearBackground();

	// set size of table
	m_grid->DeleteCols(0, m_grid->GetNumberCols());
	m_grid->AppendCols(fields.size());
	m_grid->DeleteRows(0, m_grid->GetNumberRows());
	m_grid->AppendRows(cols.at(0).size());

	// set field labels
	for(uint i = 0; i < fields.size(); ++i)
	{
		wstring dataType = m_fieldDataTypes.at(i);
		bool bGroupable = m_bGroupable.at(i);

		if(dataType == _T("Number"))
			m_choiceBook->AddPage(m_numberPanel, wxString(fields.at(i).c_str(), wxConvISO8859_1));
		else if(dataType == _T("String") && bGroupable)
			m_choiceBook->AddPage(m_groupableStringPanel, wxString(fields.at(i).c_str(), wxConvISO8859_1));
		else if(dataType == _T("String") && !bGroupable)
			m_choiceBook->AddPage(m_ungroupableStringPanel, wxString(fields.at(i).c_str(), wxConvISO8859_1));
		else
			m_choiceBook->AddPage(m_blankPanel, wxString(fields.at(i).c_str(), wxConvISO8859_1));
		m_grid->SetColLabelValue(i, wxString(fields.at(i).c_str(), wxConvISO8859_1));
	}

	// set field values
	for(uint c = 0; c < fields.size(); ++c)
	{
		for(uint r = 0; r < cols.at(c).size(); ++r)
		{
			m_grid->SetCellValue(r, c, wxString(cols.at(c).at(r).c_str(), wxConvISO8859_1));
		}
	}		

	m_grid->AutoSizeColumns();
	m_grid->Thaw();

	// *** hack to get the page to redraw properly 
	m_choiceBook->SetSelection(1);
	m_choiceBook->SetSelection(0);
}

void DbSelectAttributesDlg::OnLabelLeftClick(wxGridEvent& event)
{
	int col = event.GetCol();
	if(col != -1 && col < (int)m_choiceBook->GetPageCount())
	{
		m_choiceBook->SetSelection(col);
	}
}

void DbSelectAttributesDlg::OnLabelRightClick(wxGridEvent& event)
{
	m_selectedCol = event.GetCol();
	if(event.GetCol() != -1 && event.GetCol() < (int)m_choiceBook->GetPageCount())
	{
		wxMenu popupMenu;
		popupMenu.Append(ID_POPUP_MNU_REMOVE_FIELD, wxT("Remove field"));

		this->PopupMenu(&popupMenu);
	}
}

void DbSelectAttributesDlg::OnFieldChanged(wxChoicebookEvent& event)
{
	int index = m_fieldMap[m_choiceBook->GetPageText(m_choiceBook->GetSelection()).c_str()];
	wstring dataType = m_fieldDataTypes.at(index);
	bool bGroupable = m_bGroupable.at(index);

	if(dataType == _T("Number"))
	{
		NumberData numberData = m_numberData[index];

		m_textLessThan->SetValue(numberData.lessThan);
		m_textLowerBound->SetValue(numberData.lowerBound);
		m_textUpperBound->SetValue(numberData.upperBound);
		m_textGreaterThan->SetValue(numberData.greaterThan);

		m_radioAllNumber->SetValue(numberData.bAll);
		m_radioNoneNumber->SetValue(numberData.bNone);
		m_radioLessThan->SetValue(numberData.bLessThan);
		m_radioBetween->SetValue(numberData.bBetween);
		m_radioGreaterThan->SetValue(numberData.bGreaterThan);		
	}
	else if(dataType == _T("String") && bGroupable)
	{
		GroupableStringData groupableStringData = m_groupableStringData[index];

		m_textFilterStr->SetValue(groupableStringData.filterStr);

		m_listAvailable->Freeze();
		m_listRetrieve->Freeze();

		m_listAvailable->Clear();
		m_listRetrieve->Clear();

		for(uint i = 0; i < groupableStringData.availableItems.size(); ++i)
			m_listAvailable->Append(groupableStringData.availableItems.at(i).c_str());

		for(uint i = 0; i < groupableStringData.returnableItems.size(); ++i)
			m_listRetrieve->Append(groupableStringData.returnableItems.at(i).c_str());

		m_listAvailable->Thaw();
		m_listRetrieve->Thaw();
	}
	else if(dataType == _T("String") && !bGroupable)
	{
		UngroupableStringData ungroupableStringData = m_ungroupableStringData[index];

		m_textContainsStr->SetValue(ungroupableStringData.containsStr);
		m_radioAllString->SetValue(ungroupableStringData.bAll);
		m_radioStringContains->SetValue(ungroupableStringData.bStringContains);
		m_radioNoneString->SetValue(ungroupableStringData.bNone);
	}

	else
	{
		Log::Inst().Warning("(Warning) DbSelectAttributesDlg::OnFieldChanged(): Unknown data type.");
	}
}

void DbSelectAttributesDlg::OnFieldChanging(wxChoicebookEvent& event)
{
	int index = m_fieldMap[m_choiceBook->GetPageText(m_choiceBook->GetSelection()).c_str()];
	wstring dataType = m_fieldDataTypes.at(index);
	bool bGroupable = m_bGroupable.at(index);

	if(dataType == _T("Number"))
	{
		m_numberData[index].lessThan = m_textLessThan->GetValue();
		m_numberData[index].lowerBound = m_textLowerBound->GetValue();
		m_numberData[index].upperBound = m_textUpperBound->GetValue();
		m_numberData[index].greaterThan = m_textGreaterThan->GetValue();

		m_numberData[index].bAll = m_radioAllNumber->GetValue();
		m_numberData[index].bNone = m_radioNoneNumber->GetValue();
		m_numberData[index].bLessThan = m_radioLessThan->GetValue();
		m_numberData[index].bBetween = m_radioBetween->GetValue();
		m_numberData[index].bGreaterThan = m_radioGreaterThan->GetValue();
	}
	else if(dataType == _T("String") && bGroupable)
	{
		m_groupableStringData[index].filterStr = m_textFilterStr->GetValue();

		m_groupableStringData[index].availableItems.clear();
		for(uint i = 0; i < m_listAvailable->GetCount(); ++i)
			m_groupableStringData[index].availableItems.push_back(m_listAvailable->GetString(i));

		m_groupableStringData[index].returnableItems.clear();
		for(uint i = 0; i < m_listRetrieve->GetCount(); ++i)
			m_groupableStringData[index].returnableItems.push_back(m_listRetrieve->GetString(i));
	}
	else if(dataType == _T("String") && !bGroupable)
	{
		m_ungroupableStringData[index].containsStr = m_textContainsStr->GetValue();
		m_ungroupableStringData[index].bAll = m_radioAllString->GetValue();
		m_ungroupableStringData[index].bStringContains = m_radioStringContains->GetValue();
		m_ungroupableStringData[index].bNone = m_radioNoneString->GetValue();
	}
	else
	{
		// moving away from page with unknown data type... user will already have been
		// informed that the data type is unknown
	}
}

void DbSelectAttributesDlg::OnRestoreOriginalTable( wxCommandEvent& event )
{
	PopulateTable(m_fields, m_cols);
}

void DbSelectAttributesDlg::OnContinue( wxCommandEvent& event )
{
	// make sure data on last page has been save
	wxChoicebookEvent tempevent = wxChoicebookEvent();
	OnFieldChanging(tempevent);

	// request sample data
	wxString sequenceDataStr = GenerateSequenceDataQuery();

	// modify sample data to reflect any deleted samples
	vector<uint> sampleToKeep;
	for(uint i = 0; i < m_sampleData.size(); ++i)
	{
		bool bFound = false;
		for(int j = 0; j < m_grid->GetNumberRows(); ++j)
		{
			if(wxString(m_sampleData.at(i).at(PROJECT_ID_COLUMN).c_str(), wxConvISO8859_1) == m_grid->GetCellValue(j, PROJECT_ID_COLUMN) &&
			   wxString(m_sampleData.at(i).at(SAMPLE_ID_COLUMN).c_str(), wxConvISO8859_1)  == m_grid->GetCellValue(j, SAMPLE_ID_COLUMN))
			{
				bFound = true;
				break;
			}
		}

		if(bFound)
			sampleToKeep.push_back(i);
	}

	vector< vector<wstring> > sampleData;
	for(uint i = 0; i < sampleToKeep.size(); ++i)
		sampleData.push_back(m_sampleData.at(sampleToKeep.at(i)));

	// show retrieved sequences
	DbSequenceSummaryDlg* dlg = new DbSequenceSummaryDlg(sequenceDataStr, m_sampleDataFields, sampleData, App::Inst().GetMainWindow());
	dlg->Show();	

	Destroy();
}

wxString DbSelectAttributesDlg::GenerateSequenceDataQuery()
{
	// generate RESTful string for acquiring sample data
	wxString dbString = wxString(App::Inst().GetDatabaseConnectionString().c_str());
	wxString connectString = dbString.substr(0, dbString.find_last_of(_T("/")));

	int typePosStart = dbString.find(_T("type="));
	wxString typeString = dbString.substr(typePosStart);

	wxString sequenceDataStr = connectString;
	sequenceDataStr += _T("/fetch_sample_data_sets.cgi?");
	sequenceDataStr += typeString;

	// samples
	sequenceDataStr += _T("&samples=");
	for(int r = 0; r < m_grid->GetNumberRows(); ++r)
	{
		wxString projectId = m_grid->GetCellValue(r, PROJECT_ID_COLUMN);
		wxString sampleId = m_grid->GetCellValue(r, SAMPLE_ID_COLUMN);
		sequenceDataStr += projectId + _T(",") + sampleId;
		if(r != m_grid->GetNumberRows()-1)
			sequenceDataStr += _T("+");
	}

	// attributes
	for(int c = SAMPLE_ID_COLUMN+1; c < m_grid->GetNumberCols(); ++c)
	{
		wxString fieldName = m_grid->GetColLabelValue(c);
		int index = m_fieldMap[fieldName.c_str()];
		wstring dataType = m_fieldDataTypes.at(index);
		bool bGroupable = m_bGroupable.at(index);

		sequenceDataStr += _T("&");
		sequenceDataStr += fieldName.c_str();
		sequenceDataStr += _T("=");

		if(dataType == _T("Number"))
		{
			if(m_numberData[index].bAll)
			{
				// no constraints
			}
			else if(m_numberData[index].bNone)
			{
				// remove attribute from query
				sequenceDataStr = sequenceDataStr.substr(0, sequenceDataStr.find_last_of(_T("&")));
			}
			else if(m_numberData[index].bLessThan)
			{
				sequenceDataStr += m_numberData[index].lessThan;
				sequenceDataStr += _T(",X");
			}
			else if(m_numberData[index].bBetween)
			{
				sequenceDataStr += m_numberData[index].lowerBound;
				sequenceDataStr += _T(",");
				sequenceDataStr += m_numberData[index].upperBound;
			}
			else if(m_numberData[index].bGreaterThan)
			{
				sequenceDataStr += _T("X,");
				sequenceDataStr += m_numberData[index].greaterThan;
			}
		}
		else if(dataType == _T("String") && bGroupable)
		{
			if(m_groupableStringData[index].returnableItems.size() == 0)
			{
				// remove attribute from query
				sequenceDataStr = sequenceDataStr.substr(0, sequenceDataStr.find_last_of(_T("&")));
			}
			else
			{
				for(uint i = 0; i < m_groupableStringData[index].returnableItems.size(); ++i)
				{
					wstring value = m_groupableStringData[index].returnableItems.at(i).wc_str();
					wxString itemIndex = m_groupableStringData[index].labelMap[wxString(value.c_str())];
					sequenceDataStr += itemIndex.c_str();
					if(i != m_groupableStringData[index].returnableItems.size()-1)
						sequenceDataStr += _T(",");
				}
			}
		}
		else if(dataType == _T("String") && !bGroupable)
		{
			if(m_ungroupableStringData[index].bAll)
			{
				// no constraints
			}				
			else if(m_ungroupableStringData[index].bNone)
			{
				// remove attribute from query
				sequenceDataStr = sequenceDataStr.substr(0, sequenceDataStr.find_last_of(_T("&")));
			}
			else
			{
				sequenceDataStr += m_ungroupableStringData[index].containsStr;
			}
		}
	}

	return sequenceDataStr;
}

void DbSelectAttributesDlg::OnGridRightClick(wxGridEvent& event)
{
	wxMenu popupMenu;
	popupMenu.Append(ID_POPUP_MNU_REMOVE_SELECTED_SAMPLES, wxT("Remove selected samples"));
	popupMenu.Append(ID_POPUP_MNU_RETAIN_SELECTED_SAMPLES, wxT("Retain selected samples"));

	this->PopupMenu(&popupMenu);
}

void DbSelectAttributesDlg::OnRemoveSelectedSamples( wxCommandEvent& event )
{
	m_grid->Freeze();
	for(int r = m_grid->GetNumberRows()-1; r >= 0; --r)
	{
		if(m_grid->IsInSelection(r, 0))
			m_grid->DeleteRows(r);
	}	
	m_grid->Thaw();
}

void DbSelectAttributesDlg::OnRetainSelectedSamples( wxCommandEvent& event )
{
	m_grid->Freeze();
	for(int r = m_grid->GetNumberRows()-1; r >= 0; --r)
	{
		if(!m_grid->IsInSelection(r, 0))
			m_grid->DeleteRows(r);
	}	
	m_grid->Thaw();
}

void DbSelectAttributesDlg::OnRemoveField( wxCommandEvent& event )
{
	m_grid->Freeze();

	vector<wxString> fields;
	for(int i = 0; i < m_grid->GetNumberCols(); ++i)
		fields.push_back(m_grid->GetColLabelValue(i));

	if(m_selectedCol != 0 && m_selectedCol != 1)
	{
		m_grid->DeleteCols(m_selectedCol);
		m_choiceBook->RemovePage(m_selectedCol);
		for(int i = m_selectedCol; i < m_grid->GetNumberCols(); ++i)
			m_grid->SetColLabelValue(i, fields.at(i+1));
	}
	else
	{
		wxMessageBox(wxT("This field may not be removed."), wxT("Required field"), wxOK | wxICON_INFORMATION); 
	}
	m_grid->Thaw();
}

void DbSelectAttributesDlg::OnGroupableStringAdd( wxCommandEvent& event )
{
	m_listAvailable->Freeze();
	m_listRetrieve->Freeze();

	// moved selected items in available list to the retrieve list
	wxArrayInt selections;
	int numSelections = m_listAvailable->GetSelections(selections);
	for(int i = numSelections-1; i >= 0; --i)
	{
		wxString selItem = m_listAvailable->GetString(selections[i]);
		m_listRetrieve->Append(selItem);
		m_listAvailable->Delete(selections[i]);
	}

	m_listAvailable->Thaw();
	m_listRetrieve->Thaw();
}

void DbSelectAttributesDlg::OnGroupableStringRemove( wxCommandEvent& event )
{
	m_listAvailable->Freeze();
	m_listRetrieve->Freeze();

	// moved selected items in retrieve list to the available list
	wxArrayInt selections;
	int numSelections = m_listRetrieve->GetSelections(selections);
	for(int i = numSelections-1; i >= 0; --i)
	{
		wxString selItem = m_listRetrieve->GetString(selections[i]);
		m_listAvailable->Append(selItem);
		m_listRetrieve->Delete(selections[i]);
	}

	m_listAvailable->Thaw();
	m_listRetrieve->Thaw();
}

void DbSelectAttributesDlg::OnGroupableStringClear( wxCommandEvent& event )
{
	m_listAvailable->Freeze();
	m_listRetrieve->Freeze();

	// moved selected items in retrieve list to the available list
	for(uint i = 0; i < m_listRetrieve->GetCount(); ++i)
	{
		wxString selItem = m_listRetrieve->GetString(i);
		m_listAvailable->Append(selItem);
	}

	m_listRetrieve->Clear();

	m_listAvailable->Thaw();
	m_listRetrieve->Thaw();
}

/*
void DbSelectAttributesDlg::IndicateRequestedData()
{


	m_grid->SetColAttr(PROJECT_ID_COLUMN, &requestedData);
	m_grid->SetColAttr(SAMPLE_ID_COLUMN, &requestedData);

	for(int c = SAMPLE_ID_COLUMN+1; c < m_grid->GetNumberCols(); ++c)
	{
		wxString colLabel = m_grid->GetColLabelValue(c);
		uint index = m_fieldMap[colLabel.c_str()];
		wstring dataType = m_fieldDataTypes.at(index);
		bool bGroupable = m_bGroupable.at(index);

		if(dataType == _T("Number"))
		{
			if(m_numberData[index].bNone)
			{
				m_grid->SetColAttr(c, &noRequestedData);
			}
			else
			{
				m_grid->SetColAttr(c, &requestedData);
			}
		}
		else if(dataType == _T("String") && bGroupable)
		{
			if(m_groupableStringData[index].returnableItems.size() == 0)
			{
				m_grid->SetColAttr(c, &noRequestedData);
			}
			else
			{
				m_grid->SetColAttr(c, &requestedData);
			}
		}
		else if(dataType == _T("String") && !bGroupable)
		{
			if(m_ungroupableStringData[index].bNone)
			{
				m_grid->SetColAttr(c, &noRequestedData);
			}
			else
			{
				m_grid->SetColAttr(c, &requestedData);
			}
		}
	}
}
*/