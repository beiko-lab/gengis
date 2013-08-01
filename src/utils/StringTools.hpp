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

#ifndef _GENGIS_STRINGTOOLS_H_
#define _GENGIS_STRINGTOOLS_H_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/**
	* @brief Some utilitary functions that work on strings.
	*/
	class StringTools
	{
	public:

		/**
		* @brief Tell if a std::string is empty.
		*
		* A std::string is considered to be 'empty' if it is only made of white spaces.
		*
		* @param s The std::string to check.
		* @return True if the std::string has only white characters.
		*/
		static bool IsEmpty(const std::string & s);

		/**
		* @brief Tell if a std::wstring is empty.
		*
		* A std::wstring is considered to be 'empty' if it is only made of white spaces.
		*
		* @param s The std::wstring to check.
		* @return True if the std::wstring has only white characters.
		*/
		static bool IsEmpty(const std::wstring & s);

		/**
		* @brief Tell if a std::string is an integer number.
		*
		* @param s The std::string to check.
		* @return True if the std::string is an integer number.
		*/
		static bool IsInteger(const std::string & s) 
		{ 
			if(s.empty())
				return false;

			if(!isdigit(s[0]) && s[0] != '-')
					return false;

			for(uint i = 1; i < s.size(); ++i)
			{
				if(!isdigit(s[i]))
					return false;
			}

			return true;
		}

		/**
		* @brief Tell if a std::string is a decimal number.
		*
		* @param s The std::string to check.
		* @return True if the std::string is an decimal number.
		*/
		static bool IsDecimalNumber(const std::string & s) 
		{ 
			if(s.empty())
				return false;

			if(!isdigit(s[0]) && s[0] != '.' && s[0] != 'e' && s[0] != 'E' && s[0] != '-')
					return false;

			for(uint i = 1; i < s.size(); ++i)
			{
				if(!isdigit(s[i]) && s[i] != '.' && s[i] != 'e' && s[i] != 'E')
					return false;
			}

			return true;
		}

		/**
		* @brief Tell if a std::wstring is a decimal number.
		*
		* @param s The std::wstring to check.
		* @return True if the std::wstring is an decimal number.
		*/
		static bool IsDecimalNumber(const std::wstring & s)
		{
			if(s.empty())
				return false;

			if(!isdigit(s[0]) && s[0] != '.' && s[0] != 'e' && s[0] != 'E' && s[0] != '-')
					return false;

			for(uint i = 1; i < s.size(); ++i)
			{
				if(!isdigit(s[i]) && s[i] != '.' && s[i] != 'e' && s[i] != 'E')
					return false;
			}

			return true;
		}

		/**
		* @brief Make the std::string uppercase.
		*
		* @param s The std::string to analyse.
		* @return The std::string with all chars changed to uppercase.
		*/
		static std::string ToUpper(const std::string & s);

		/**
		* @brief Make the std::wstring uppercase.
		*
		* @param s The std::wstring to analyse.
		* @return The std::wstring with all chars changed to uppercase.
		*/
		static std::wstring ToUpper(const std::wstring & s);

		/**
		* @brief Make the std::string lowercase.
		*
		* @param s The std::string to analyse.
		* @return The std::string with all chars changed to lowercase.
		*/
		static std::string ToLower(const std::string & s);

		/**
		* @brief Make the std::wstring lowercase.
		*
		* @param s The std::wstring to analyse.
		* @return The std::wstring with all chars changed to lowercase.
		*/
		static std::wstring ToLower(const std::wstring & s);

		/**
		* @brief Tell if a character is a white space or not.
		*
		* @param c The character to check.
		* @return True if c is one of the following: ' ', '\\t', '\\n', '\\r' or '\\f'.
		*/
		static bool IsWhiteSpaceCharacter(char c);

		/**
		* @brief Remove all white spaces characters in a std::string.
		*
		* @param s The std::string to parse.
		* @return A copy of 's' without white spaces characters.
		*/
		static std::string RemoveWhiteSpaces (const std::string & s);

		/**
		* @brief Remove all white spaces characters in a std::wstring.
		*
		* @param s The std::wstring to parse.
		* @return A copy of 's' without white spaces characters.
		*/
		static std::wstring RemoveWhiteSpaces (const std::wstring & s);

		/**
		* @brief Remove all white spaces characters at the beginning of a std::string.
		*
		* @param s The std::string to parse.
		* @return The std::string beginning with the first non-white character.
		*/
		static std::string RemoveFirstWhiteSpaces (const std::string & s);

		/**
		* @brief Remove all white spaces characters at the end of a std::string.
		*
		* @param s The std::string to parse.
		* @return The std::string ending with the last non-white character.
		*/
		static std::string RemoveLastWhiteSpaces (const std::string & s);

		/**
		* @brief Remove all white spaces characters at the beginning and the
		* end of a std::string.
		*
		* @param s The std::string to parse.
		* @return The std::string beginning with the first non-white character
		* and ending with the last one.
		*/
		static std::string RemoveSurroundingWhiteSpaces(const std::string & s);

		/**
		* @brief Remove all white spaces characters at the beginning and the
		* end of a std::wstring.
		*
		* @param s The std::string to parse.
		* @return The std::wstring beginning with the first non-white character
		* and ending with the last one.
		*/
		static std::wstring RemoveSurroundingWhiteSpaces(const std::wstring & s);

		/**
		* @brief Extract path and filename from string containing a full path.
		*
		* @param fullPath std::string containing full path to a file.
		* @param path std::string that will contain the path.
		* @param filename std::string that will contain the filename.
		*/
		static void ExtractPathAndFilename(const std::string& fullpath, std::string& path, std::string& filename);

		/**
		* @brief General template method to convert to a std::string.
		*
		* @param t The object to convert.
		* @return A std::string equal to t.
		*/
		template<class T> static std::string ToString(T t)
		{
			std::ostringstream oss;
			oss << t;
			return oss.str();
		}

		/**
		* @brief Template std::string conversion.
		* 
		* @param t The object to convert.
		* @param precision To use (for numbers).
		* @return A std::string equal to t.
		*/
		template<class T>
		static std::string ToString(T t, int precision)
		{
			std::ostringstream oss;
			oss << std::setprecision(precision) << t;
			return oss.str();
		}

		/**
		* @brief General template method to convert from std::string.
		*
		* @param s The std::string to convert.
		* @return An object from std::string t.
		*/
		template<class T> static T FromString(const std::string & s)
		{
			std::istringstream iss(s);
			T obj;
			iss >> obj;
			return obj;
		}

		/**
		* @brief Convert from std::wstring to std::wstring.
		*
		* @param str The string to convert.
		* @return A std::string equal to std::wstring.
		*/
		static std::string ToString(const std::wstring& str);

		/**
		* @brief Convert from std::string to std::wstring.
		*
		* @param str The string to convert.
		* @return A std::wstring equal to std::string.
		*/
		static std::wstring ToStringW(const std::string& str);

		/**
		* @brief Convert from int to std::string.
		*
		* @param i The integer to convert.
		* @return A std::string equal to i.
		*/
		static std::string ToString(int i);

		/**
		* @brief Convert from char to std::string.
		*
		* @param c The character to convert.
		* @return A std::string equal to c.
		*/
		static std::string ToString(char c);

		/**
		* @brief Convert from float to std::string.
		*
		* @param d The float to convert.
		* @param precision To use (for numbers).
		* @return A std::string equal to d.
		*/
		static std::string ToString(float d, int precision = 6);

		/**
		* @brief Convert from float to std::wstring.
		*
		* @param d The float to convert.
		* @param precision To use (for numbers).
		* @return A std::wstring equal to d.
		*/
		static std::wstring ToStringW(float d, int precision = 6);

		/**
		* @brief Convert from int to std::wstring.
		*
		* @param i The int to convert.
		* @return A std::wstring equal to i.
		*/
		static std::wstring ToStringW(int i);

		/**
		* @brief Convert from unsigned int to std::wstring.
		*
		* @param i The unsigned int to convert.
		* @return A std::wstring equal to i.
		*/
		static std::wstring ToStringW(unsigned int i);

		/**
		* @brief Convert from std::string to int.
		*
		* @param s The std::string to parse.
		* @return The integer corresponding to s.
		*/
		static int ToInt(const std::string & s);

		/**
		* @brief Convert from std::wstring to int.
		*
		* @param s The std::wstring to parse.
		* @return The integer corresponding to s.
		*/
		static int ToInt(const std::wstring & s);

		/**
		* @brief Convert from std::string to float.
		*
		* @param s The std::string to parse.
		* @return The float corresponding to s.
		*/
		static double ToDouble(const std::string & s);

		/**
		* @brief Convert from std::wstring to float.
		*
		* @param s The std::wstring to parse.
		* @return The float corresponding to s.
		*/
		static double ToDouble(const std::wstring & s);

		/**
		* @brief Convert from std::string to long.
		*
		* @param s The std::string to parse.
		* @return The long corresponding to s.
		*/
		static long ToLong(const std::string & s);

		/**
		* @brief Convert from std::wstring to long.
		*
		* @param s The std::wstring to parse.
		* @return The long corresponding to s.
		*/
		static long ToLong(const std::wstring & s);

		/**
		* @brief Template to std::string conversion.
		* 
		* @param s The std::string to parse.
		* @return An object of class R corresponding to s.
		*/
		template<class T>
		static T To(const std::string & s)
		{
			std::istringstream iss(s);
			T t;
			iss >> t;
			return t;
		}

		/**
		* @brief Send a std::string of size 'newSize', which is a copy of 's' truncated or
		* filled with character 'fill' at the end.
		*
		* @param s       The std::string to parse.
		* @param newSize The new std::string size.
		* @param fill    The character to use to fill the std::string id length < newSize.
		* @return A std::string of size newsize which is a copy from the left of s.
		*/
		static std::string ResizeRight(const std::string & s, uint newSize, char fill = ' ');

		/**
		* @brief Send a std::string of size 'newSize', which is a copy of 's' truncated or
		* filled with character 'fill' at the beginning.
		*
		* @param s       The std::string to parse.
		* @param newSize The new std::string size.
		* @param fill    The character to use to fill the std::string id length < newSize.
		* @return A std::string of size newsize which is a copy from the right of s.
		*/
		static std::string ResizeLeft(const std::string & s, uint newSize, char fill = ' ');

		/**
		* @brief Split a std::string into parts of size 'n'.
		*
		* The last part may contain < n chars.
		*
		* @param s The std::string to parse.
		* @param n The number of tokens.
		* @return A vector of strings with all tokens.
		*/
		static std::vector<std::string> Split(const std::string & s, uint n);

		/**
		* @brief Remove substrings from a std::string.
		*
		* All substrings beginning with blockBeginning
		* and ending with blockEnding will be removed.
		* Nesting blocks are allowed, the most extern block will be removed.
		*
		* @param s The std::string to parse.
		* @param blockBeginning The character specifying the beginning of each block.
		* @param blockEnding    The character specifying the end of each block.
		* @return The std::string with all blocks removed.
		*/
		static std::string RemoveSubstrings(const std::string & s, char blockBeginning, char blockEnding);

		/**
		* @brief Remove all occurences of a character in a std::string.
		*
		* @param s The std::string to parse.
		* @param c The character to remove.
		* @return The std::string with all specified chars removed.
		*/
		static std::string RemoveChar(const std::string & s, char c);

		/**
		* @brief Replace all occurences of a character in a std::string with another character.
		*
		* @param s The std::string to parse.
		* @param c The character to replace.
		* @param r The replacement character.
		* @return The std::string with all specified chars removed.
		*/
		static std::string ReplaceChar(const std::string & s, char c, char r);

		/**
		* @brief Count the occurences of a given pattern in a std::string.
		*
		* @param s The std::string to search.
		* @param pattern The pattern to use.
		* @return The number of occurences of 'pattern' in 's'.
		*/
		static uint Count(const std::string & s, const std::string & pattern);

		/**
		* @brief Convert all data to its numeric equivalent, sort, and put back into string vector.
		*
		* @param fieldValues The std::wstring to sort.
		*/
		static void SortFieldValues(std::vector<std::wstring>& fieldValues);
	};

}


#endif

