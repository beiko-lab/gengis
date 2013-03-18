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

#include "../gui/DbSequenceSummaryDlg.hpp"

#include "../gui/DbSaveResultsDlg.hpp"

#include "../utils/StringTools.hpp"
#include "../utils/Log.hpp"

using namespace GenGIS;
using namespace boost;
using namespace std;

DbSequenceSummaryDlg::DbSequenceSummaryDlg(const wxString& fetchString, std::vector< std::wstring > sampleDataFields,
	std::vector< std::vector<std::wstring> > sampleData, wxWindow* parent) :
		m_fetchString(fetchString), m_sampleDataFields(sampleDataFields), m_sampleData(sampleData), DbSequenceSummaryLayout(parent)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	Init();
	ConnectEvents();
}

DbSequenceSummaryDlg::~DbSequenceSummaryDlg()
{
	DisconnectEvents();
}

void DbSequenceSummaryDlg::ConnectEvents()
{

}

void DbSequenceSummaryDlg::DisconnectEvents()
{

}


void DbSequenceSummaryDlg::Init()
{
	FetchSequencesFromDB(m_fetchString);
}

void DbSequenceSummaryDlg::FetchSequencesFromDB(const wxString& fetchString)
{
	wxBusyCursor wait;

	wxFileSystem fs;
	wxFSFile* file = fs.OpenFile(fetchString);
	wxInputStream* stream = file->GetStream();
	
	wstring tempFile = wstring((GenGIS::App::Inst().GetUserDataDir() + _T("/_db_sequences.txt")).wchar_str());
	wofstream output(string(tempFile.begin(),tempFile.end()).c_str(), std::ios::out);
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

bool DbSequenceSummaryDlg::ReadData(const std::wstring& file)
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

	bSuccess = ReadSequenceData(fin);
	if(!bSuccess)
	{
		fin.close();
		return false;
	}

	fin.close();
	return true;
}

bool DbSequenceSummaryDlg::ReadFieldData(std::wifstream& fin)
{
	m_fields.clear();

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
		wstring field = *dataIter;
		m_fields.push_back(field);
	}

	return true;
}

bool DbSequenceSummaryDlg::ReadSequenceData(std::wifstream& fin)
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

void DbSequenceSummaryDlg::PopulateTable(const vector<wstring>& fields, const vector< vector<wstring> >& rows)
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
		m_grid->SetColLabelValue(i, wxString(fields.at(i).c_str(), wxConvISO8859_1));

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
	//m_choiceBook->SetSelection(1);
	//m_choiceBook->SetSelection(0);
}

void DbSequenceSummaryDlg::OnOK( wxCommandEvent& event )
{
	DbSaveResultsDlg* dlg = new DbSaveResultsDlg(App::Inst().GetMainWindow());

	if(dlg->ShowModal())
	{
		wxString sampleFile = dlg->GetSampleFile();
		wxString sequenceFile = dlg->GetSequenceFile();

		SaveSampleFile(_T("test_location.csv"));
		SaveSequenceFile(_T("test_sequence.csv"));
	}
}

void DbSequenceSummaryDlg::SaveSampleFile( const std::wstring& filename )
{
	wofstream output(string(filename.begin(),filename.end()).c_str(), std::ios::out);
	if(!output.is_open()) 
	{
		Log::Inst().Error("(Error) Failed to save sample file.");	
		return;
	}

	// output field headers
	output << "Project Id,Site Id,";
	for(uint i = 2; i < m_sampleDataFields.size(); ++i)
	{
		output << m_sampleDataFields.at(i);
		if(i != m_sampleDataFields.size()-1)
			output << _T(",");
	}
	output << endl;

	// output sample data
	for(uint i = 0; i < m_sampleData.size(); ++i)
	{
		for(uint j = 0; j < m_sampleData.at(i).size(); ++j)
		{
			output << m_sampleData.at(i).at(j);
			if(j != m_sampleData.at(i).size()-1)
				output << _T(",");
		}
		output << endl;
	}

	output.close();
}

void DbSequenceSummaryDlg::SaveSequenceFile( const std::wstring& filename )
{
	wofstream output(string(filename.begin(),filename.end()).c_str(), std::ios::out);
	if(!output.is_open()) 
	{
		Log::Inst().Error("(Error) Failed to save sequence file.");	
		return;
	}

	// output field headers
	output << "Project Id,Site Id,Sequence Id,";
	for(uint i = 3; i < m_fields.size(); ++i)
	{
		output << m_fields.at(i);
		if(i != m_fields.size()-1)
			output << _T(",");
	}
	output << endl;

	// output sample data
	for(uint i = 0; i < m_rows.size(); ++i)
	{
		for(uint j = 0; j < m_rows.at(i).size(); ++j)
		{
			output << m_rows.at(i).at(j);
			if(j != m_rows.at(i).size()-1)
				output << _T(",");
		}
		output << endl;
	}

	output.close();
}
