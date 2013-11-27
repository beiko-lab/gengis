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

#include "../utils/StringTools.hpp"

using namespace GenGIS;
using namespace std;

bool StringTools::IsEmpty(const string& s)
{
	for(uint i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if(c != ' ' && c != '\n' && c != '\t') return false;
	}
	return true;
}

bool StringTools::IsEmpty(const wstring& s)
{
	for(uint i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if(c != ' ' && c != '\n' && c != '\t') return false;
	}
	return true;
}

string StringTools::ToUpper(const string& s)
{
	return 	boost::to_upper_copy(s);;
}

wstring StringTools::ToUpper(const wstring& s)
{
	return 	boost::to_upper_copy(s);;
}

string StringTools::ToLower(const string& s)
{
	return boost::to_lower_copy(s);
}

wstring StringTools::ToLower(const wstring& s)
{
	return boost::to_lower_copy(s);
}

bool StringTools::IsWhiteSpaceCharacter(char c)
{
	return (c == ' ')
		|| (c == '\t')
		|| (c == '\n')
		|| (c == '\r')
		|| (c == '\f');  
}

string StringTools::RemoveWhiteSpaces(const string & s)
{
	// Copy sequence
	string st (s);

	// For all sequence's characters
	for (uint i = 0; i < st.size(); i++)
	{
		if(IsWhiteSpaceCharacter(st[i]))
		{
			st.erase(st.begin() + i); // Remove character
			i--;
		}
	}

	// Send result
	return st;
}

wstring StringTools::RemoveWhiteSpaces(const wstring & s)
{
	// Copy sequence
	wstring st (s);

	// For all sequence's characters
	for (uint i = 0; i < st.size(); i++)
	{
		if(IsWhiteSpaceCharacter(st[i]))
		{
			st.erase(st.begin() + i); // Remove character
			i--;
		}
	}

	// Send result
	return st;
}

string StringTools::RemoveFirstWhiteSpaces(const string & s)
{
	return 	boost::trim_left_copy(s);
}

string StringTools::RemoveLastWhiteSpaces(const string & s)
{
	return boost::trim_right_copy(s);;
}

string StringTools::RemoveSurroundingWhiteSpaces(const string & s)
{
	return boost::trim_copy(s);
}

wstring StringTools::RemoveSurroundingWhiteSpaces(const wstring & s)
{
	return boost::trim_copy(s);
}

void StringTools::ExtractPathAndFilename(const std::string& fullpath, std::string& path, std::string& filename)
{
	uint pos = fullpath.find_last_of("/");
	if(pos == std::string::npos)
	{
		pos = fullpath.find_last_of("\\");
		if(pos == std::string::npos)
			pos = -1;
	}

	std::string temp = fullpath.substr(pos+1);
	filename = temp;

	pos = fullpath.find_last_of("/");
	if(pos == std::string::npos)
	{
		pos = fullpath.find_last_of("\\");
		if(pos == std::string::npos)
			pos = 0;
	}

	temp = fullpath.substr(0,pos);
	path = temp;
}

string StringTools::ToString(const wstring& str)
{
	std::string temp(str.begin(), str.end());
	return temp;
}

wstring StringTools::ToStringW(const string& str)
{
	std::wstring temp(str.length(),L' ');
	std::copy(str.begin(), str.end(), temp.begin());

	return temp;
}

string StringTools::ToString(int i)
{
	ostringstream oss;
	oss << i;
	return oss.str();
}

string StringTools::ToString(char c)
{
	ostringstream oss;
	oss << c;
	return oss.str();
}

string StringTools::ToString(float d, int precision)
{
	ostringstream oss;
	oss << setprecision(precision) << d;
	return oss.str();
}

wstring StringTools::ToStringW(float d, int precision)
{
	ostringstream oss;
	oss << setprecision(precision) << fixed << d;

	string str = oss.str();
	std::wstring temp(str.length(),L' ');
	std::copy(str.begin(), str.end(), temp.begin());

	return temp;
}

wstring StringTools::ToStringW(int i)
{
	ostringstream oss;
	oss << i;

	string str = oss.str();
	std::wstring temp(str.length(),L' ');
	std::copy(str.begin(), str.end(), temp.begin());

	return temp;
}

wstring StringTools::ToStringW(unsigned int i)
{
	ostringstream oss;
	oss << i;

	string str = oss.str();
	std::wstring temp(str.length(),L' ');
	std::copy(str.begin(), str.end(), temp.begin());

	return temp;
}

int StringTools::ToInt(const string & s)
{
	istringstream iss(s);
	int i;
	iss >> i;
	return i;
}

int StringTools::ToInt(const wstring & s)
{
	wistringstream iss(s);
	int i;
	iss >> i;
	return i;
}

double StringTools::ToDouble(const string & s) 
{
	istringstream iss(s);
	double d;
	iss >> d;

	return d;
}

double StringTools::ToDouble(const wstring & s) 
{
	wistringstream iss(s);
	double d;
	iss >> d;
	return d;
}

long StringTools::ToLong(const string & s) 
{
	istringstream iss(s);
	long d;
	iss >> d;
	return d;
}

long StringTools::ToLong(const wstring & s) 
{
	wistringstream iss(s);
	long d;
	iss >> d;
	return d;
}

string StringTools::ResizeRight(const string & s, uint newSize, char fill) 
{
	if(s.size() > newSize) return s.substr(0, newSize);
	else return s + string(newSize - s.size(), fill);
}

string StringTools::ResizeLeft(const string & s, uint newSize, char fill)
{
	if(s.size() > newSize) return s.substr(s.size() - newSize);
	else return string(newSize - s.size(), fill) + s;
}

vector<string> StringTools::Split(const string & s, uint n)
{
	vector<string> v;
	string tmp = s;
	while(tmp.size() > n)
	{
		v.push_back(tmp.substr(0, n));
		tmp = tmp.substr(n);
	}
	v.push_back(tmp);
	return v;
}

string StringTools::RemoveSubstrings(const string & s, char blockBeginning, char blockEnding)
{
	string t = "";
	int blockCount = 0;
	int begPos = 0;
	for(uint i = 0; i < s.size(); i++)
	{
		char current = s[i];
		if(current == blockBeginning)
		{
			blockCount++;
			t += s.substr(begPos, i);
		}
		else if(current == blockEnding)
		{
			blockCount--;
			if(blockCount == 0) {
				begPos = i + 1;
			}
			else if(blockCount < 0)
			{
				blockCount = 0;
			}
		}
	}
	t += s.substr(begPos, s.npos);
	return t;
}

string StringTools::RemoveChar(const string & s, char c)
{
	// Copy sequence
	string st(s);

	// For all sequence's characters
	for (uint i = 0; i < st.size(); i++)
	{
		if (st[i] == c)
		{
			st.erase(st.begin() + i); // Remove character
			i--;
		}
	}

	// Send result
	return st;
}

string StringTools::ReplaceChar(const string & s, char c, char r)
{
	// Copy sequence
	string st(s);

	// For all sequence's characters
	for (uint i = 0; i < st.size(); i++)
	{
		if (st[i] == c)
		{
			st[i] = r; // replace character
		}
	}

	// Send result
	return st;
}

std::wstring StringTools::ReplaceChar(const std::wstring & s, char c, char r)
{
	// Copy sequence
	std::wstring st(s);

	// For all sequence's characters
	for (uint i = 0; i < st.size(); i++)
	{
		if (st[i] == c)
		{
			st[i] = r; // replace character
		}
	}

	// Send result
	return st;
}

uint StringTools::Count(const string & s, const string & pattern)
{
	uint count = 0;
	string::size_type index = s.find(pattern);
	while(index != string::npos)
	{
		count++;
		index = s.find(pattern, index+1);
	}
	return count;
}

void StringTools::SortFieldValues(std::vector<std::wstring>& fieldValues)
{
	// if all data is numerical, then sort in numerical as opposed to lexigraphical order
	bool bNumeric = true;
	foreach(std::wstring str, fieldValues)
	{
		if(!StringTools::IsDecimalNumber(str))
		{
			bNumeric = false;
			break;
		}
	}

	// sort field values either numerically or lexigraphically
	if(bNumeric)
	{
		// convert all data to its numeric equivalent, sort, and put back into string vector
		// Note: this somewhate convoluted way of doing a numerical sort of strings is done to
		// ensure that at the end the field values strings are exactly as they originally appeared.
		// Any added training zeros or rounding may cause problems later on when exact string matching
		// is attempted.
		std::vector<NumericStringSorter> fieldNumeric;
		for(uint i = 0; i < fieldValues.size(); ++i)
		{
			fieldNumeric.push_back(NumericStringSorter(StringTools::ToDouble(fieldValues.at(i)), i));
		}

		std::sort(fieldNumeric.begin(), fieldNumeric.end(), NumericStringSorter::NumericStringPredicate); 

		std::vector<std::wstring> sortedFieldValues;
		foreach(NumericStringSorter numeric, fieldNumeric)
		{
			sortedFieldValues.push_back(fieldValues.at(numeric.index));
		}

		fieldValues = sortedFieldValues;
	}
	else
	{
		// sort in lexigraphical order
		std::sort(fieldValues.begin(), fieldValues.end());
	}
}
