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

#include "../core/App.hpp"
#include "../core/LocationModel.hpp"
#include "../core/LocationSetIO.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/StudyController.hpp"
#include "../core/MapController.hpp"

#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"

#include "../gui/ProgressDlg.hpp"

using namespace boost;
using namespace std;
using namespace GenGIS;

bool LocationSetIO::Read( const wxString& fullPath, StudyControllerPtr studyController, std::vector<LocationModelPtr>& locationModels )
{
	wifstream infile( fullPath.mb_str() );
	if( !infile.is_open() )
	{
		Log::Inst().Warning("(Warning) Failed to open location data file.");	
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

	// if the input failed, we likely have a OS X style file so try reading the file again
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
		Log::Inst().Warning("(Warning) Location file contains no data.");	
		return false;
	}

	// Parse the rows of the CSV file
	ParseCSVFile( rows, studyController, locationModels );

	// closing the file stream.
	infile.close();

	return true;
}

bool LocationSetIO::ParseCSVFile( const std::vector<std::wstring>& csvTableRows, StudyControllerPtr studyController, std::vector<LocationModelPtr>& locationModels )
{
	const int ROWS_TO_SHOW_PROGRESS_DLG = 2000;

	// tokenize column names
	tokenizer< escaped_list_separator<wchar_t>, wstring::const_iterator, wstring > columnTokens(csvTableRows.at(0));
	vector<wstring> columnValues(columnTokens.begin(), columnTokens.end());
	if(columnValues.size() == 1)
	{
		// assume a tab-delimited file
		columnValues.clear();
		split(columnValues, csvTableRows.at(0), boost::is_any_of(_T("\t")));
	}

	ProgressDlgPtr progressDlg;
	if(csvTableRows.size() > ROWS_TO_SHOW_PROGRESS_DLG)
		progressDlg.reset(new ProgressDlg(_T("Loading location set"), _T("Reading locations..."), csvTableRows.size(), App::Inst().GetMainWindow()));

	uint rowNum = 1;
	vector<wstring>::const_iterator it;
	for(it = ++csvTableRows.begin(); it != csvTableRows.end(); ++it)
	{
		if(progressDlg && rowNum % 25 == 0)
			progressDlg->Update(rowNum);		

		rowNum++;

		tokenizer< escaped_list_separator<wchar_t>, wstring::const_iterator, wstring > rowTokens(*it);
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
			warning.Printf(wxT("(Warning) Location file contains %d headings, but line %d contains only %d values. "), columnValues.size(), rowNum, rowValues.size());
			warning += (wxT("This line begins with the value '") + rowValues.at(0) + wxT("'.")).c_str();
			Log::Inst().Warning(warning);	
			return false;
		}

		wstring siteId     = _T("");
		wstring sampleId   = _T("");
		double  northing   = -std::numeric_limits<float>::max();
		double  easting    = -std::numeric_limits<float>::max();
		bool    bIsLatLong = false;
		bool	bReject	   = false;
		double  latitude   = -std::numeric_limits<float>::max();
		double  longitude  = -std::numeric_limits<float>::max();

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
			else if(StringTools::ToLower(header) == _T("northing"))
				northing = StringTools::ToLong(value);
			else if(StringTools::ToLower(header) == _T("easting"))
				easting = StringTools::ToLong(value);
			else if(StringTools::ToLower(header) == _T("latitude"))
			{
				// check if latitude is numeric
				if(!StringTools::IsDecimalNumber( StringTools::ToString(value) ) && !StringTools::IsInteger( StringTools::ToString(value) ) )
					bReject = true;
				latitude = StringTools::ToDouble(value);
				bIsLatLong = true;
			}
			else if(StringTools::ToLower(header) == _T("longitude"))
			{
				// check if longitude is numeric
				if(!StringTools::IsDecimalNumber( StringTools::ToString(value) ) && !StringTools::IsInteger( StringTools::ToString(value) ) )
					bReject = true;
				longitude = StringTools::ToDouble(value);
				bIsLatLong = true;
			}
		}

		// transform lat/lon coordinates to projected coordinates
		if(bIsLatLong)
		{
			double tempLat = latitude;
			double tempLat2 = latitude;
			if(studyController->IsProjectData())
			{
				ProjectionToolPtr projTool = studyController->GetProjectionTool();
				if(!projTool->Transform(1, &longitude, &latitude))
				{
					Log::Inst().Warning("(Warning) Failed to project data. Possibly missing 'Latitude' and 'Longitude' fields.");
					return false;
				}

				// if a point falls off the map, assume the map actually spans the 180 degree line
				if(longitude < App::Inst().GetMapController()->GetHeader()->projExtents.x)
				{
					double longNeg180 = -180;
					projTool->Transform(1, &longNeg180, &tempLat);

					double long180 = 180;
					projTool->Transform(1, &long180, &tempLat2);

					longitude = long180 + (longitude - longNeg180);
				}
			}

			northing = latitude;
			easting = longitude;
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

		if(northing == -std::numeric_limits<float>::max())
		{
			Log::Inst().Warning("(Warning) Missing 'Latitude or Northing' field.");
			return false;
		}

		if(easting == -std::numeric_limits<float>::max())
		{
			Log::Inst().Warning("(Warning) Missing 'Longitude or Easting' field.");
			return false;
		}

		if (!bReject)
		{
			// create a new location model from this data
			LocationModelPtr locationModel(new LocationModel(siteId, northing, easting, data));
			locationModels.push_back(locationModel);
		}
	}

	return true;
}


bool LocationSetIO::ReadSourceFile( const wxString& fullPath, LocationSetLayerPtr locationSetLayer )
{
	// Convert wxString to wxFileName for easier filename operations
	wxFileName sourceFile( fullPath );

	// Strip the filename of its directory path and file extension
	wxString sourceFileName = sourceFile.GetName();

	// Return false if the location filename does not end with "_loc"
	if ( !sourceFileName.EndsWith( wxT( "_locs" ) ) )
		return false;

	// Remove "_loc" from the end of the filename
	sourceFileName.Truncate( sourceFileName.Length()-5 );

	// Append "_source.txt" to filename and prepend directory path
	sourceFileName.Append( wxT( "_source.txt" ) );
	sourceFile.Assign( sourceFile.GetPath(), sourceFileName );
	sourceFileName = sourceFile.GetFullPath();

	// Return false if source file does not exist
	if ( !sourceFile.FileExists() )
		return false;

	// Read in source file
	std::wifstream ifs( sourceFileName.mb_str(), std::ios_base::binary );
	wstring description_field( istreambuf_iterator<wchar_t>(ifs), (istreambuf_iterator<wchar_t>()) );
	ifs.close();
	
	// Add data to description field of location set layer properties dialog
	locationSetLayer->SetDescription( description_field );
	return true;
}
