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

#include "../core/SequenceIO.hpp"

#include "../core/App.hpp"
#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"

#include "../gui/ProgressDlg.hpp"

using namespace std;
using namespace GenGIS;

bool SequenceIO::Read(const wxString& fullPath, std::vector<SequenceModelPtr>& sequenceModels, bool& bCancel)
{
	wifstream infile( fullPath.mb_str() );
	if(!infile.is_open()) 
	{
		Log::Inst().Warning("(Warning) Failed to open sequence data file.");	
		return false;
	}

	// parse all rows
	wstring line;
	vector<wstring> rows;
	while(getline(infile, line))
	{
		if(!StringTools::RemoveSurroundingWhiteSpaces(line).empty())
			rows.push_back(line);
	}

	// if the input failed, we likely have a OS X style file so
	// try reading the file again
	if(rows.size() == 1)
	{
		infile.clear(); 
		infile.seekg(0);

		rows.clear();
		while(getline(infile, line, wxT('\r')))
		{
			if(!StringTools::RemoveSurroundingWhiteSpaces(line).empty())
				rows.push_back(line);
		}
	}

	// check for valid data
	if(rows.size() < 2)
	{
		Log::Inst().Warning("(Warning) Sequence file contains no data.");	
		return false;
	}

	// Parse the rows of the CSV file
	ParseCSVFile( rows, sequenceModels, bCancel );

	// closing the file stream.
	infile.close();

	return true;
}


bool SequenceIO::ParseCSVFile( const std::vector<std::wstring>& csvTableRows, std::vector<SequenceModelPtr>& sequenceModels, bool& bCancel)
{
	bCancel = false;

	// track progress
	ProgressDlg progressDlg(wxT("Loading file"), wxT("Loading sequences..."), 100, App::Inst().GetMainWindow());

	// tokenize column names
	boost::tokenizer< boost::escaped_list_separator<wchar_t>, wstring::const_iterator, wstring  > columnTokens(csvTableRows.at(0));
	vector<wstring> columnValues(columnTokens.begin(), columnTokens.end());
	if(columnValues.size() == 1)
	{
		// assume a tab-delimited file
		columnValues.clear();
		split(columnValues, csvTableRows.at(0), boost::is_any_of(_T("\t")));
	}

	// parse all rows
	uint rowNum = 1;
	vector<wstring>::const_iterator it;
	for(it = ++csvTableRows.begin(); it != csvTableRows.end(); ++it)
	{
		if(rowNum % 100 == 0)
		{
			if(!progressDlg.Update(float(rowNum)/csvTableRows.size() * 100))
			{
				bCancel = true;
				sequenceModels.clear();
				return false;
			}			
		}		

		rowNum++;

		boost::tokenizer< boost::escaped_list_separator<wchar_t>, wstring::const_iterator, wstring  > rowTokens(*it);
		vector<wstring> rowValues(rowTokens.begin(), rowTokens.end());
		if(rowValues.size() == 1)
		{
			// assume a tab-delimited file
			rowValues.clear();
			split(rowValues, *it, boost::is_any_of(_T("\t")));
		}

		if(columnValues.size() != rowValues.size())
		{
			wxString warning;
			warning.Printf(wxT("(Warning) Sequence file contains %d headings, but line %d contains only %d values. "), columnValues.size(), rowNum, rowValues.size());
			warning += (wxT("The erroneous line beings with the value '") + rowValues.at(0) + wxT("'.")).c_str();
			Log::Inst().Warning(warning);	
			return false;
		}

		wstring siteId = _T("");
		wstring sampleId = _T("");
		wstring sequenceId = _T("");

		map<std::wstring,std::wstring> data;
		for(uint c = 0; c < columnValues.size(); ++c)
		{
			wstring header = StringTools::RemoveSurroundingWhiteSpaces(columnValues.at(c));
			wstring value = StringTools::RemoveSurroundingWhiteSpaces(rowValues.at(c));
			data.insert(pair<wstring, wstring>(header, value));

			// determine which column we are looking at (this allows user to put columns in any order)
			if(StringTools::ToLower(header) == _T("site id") || StringTools::ToLower(header) == _T("siteid"))
				siteId = value;
			else if(StringTools::ToLower(header) == _T("sample id") || StringTools::ToLower(header) == _T("sampleid"))
				sampleId = value;
			else if(StringTools::ToLower(header) == _T("sequence id") || StringTools::ToLower(header) == _T("sequenceid"))
				sequenceId = value;
		}

		// check for required data
		if(siteId == _T(""))
		{
			if(sampleId != _T(""))
				siteId = sampleId;
			else
			{
				Log::Inst().Warning("(Warning) Missing 'Site Id' field.");
				return false;
			}
		}

		if(sequenceId == _T(""))
		{
			Log::Inst().Warning("(Warning) Missing 'Sequence Id' field.");
			return false;
		}
		              
		// create a new location model from this data
		SequenceModelPtr sequenceModel(new SequenceModel(sequenceId, siteId, data));

		sequenceModels.push_back(sequenceModel);
	}
	return true;
}
