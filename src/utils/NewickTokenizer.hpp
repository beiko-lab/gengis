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

#ifndef _GENGIS_NEWICK_TOKENIZER_
#define _GENGIS_NEWICK_TOKENIZER_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/**
	* @brief Class for tokenizing strings in Newick format.
	*/
	class NewickTokenizer
	{
	public:
		/**
		* @brief Build a new Newick tokenizer.
		*
		* @param description String to tokenize.
		* @throw IOException If tree can not be parsed into Newick tokens.
		*/
		explicit NewickTokenizer(const std::string& description): m_tokens()
		{
			uint tokCount = 0;
			int parCount = 0;
			uint i;

			for(i = 0; i < description.size(); i++)
			{
				if(description[i] == '(') 
					parCount++; 

				if(description[i] == ')') 
					parCount--; 

				// check for closing parenthesis with no opening parenthesis
				assert(parCount >= 0);

				if(description[i] == ',' && parCount == 0)
				{
					// New token found:
					m_tokens.push_back(description.substr(tokCount, i - tokCount));
					tokCount = i + 1;
				}          
			}

			// Add last token:
			m_tokens.push_back(description.substr(tokCount));
		}

		/**
		* @brief Return a specific token
		*
		* @param index Position of token to return.
		* @return Token at the specified position.
		*/
		std::string Token(uint index) const { return m_tokens.at(index); }

		/** Number of tokens. */
		uint NumTokens() const { return m_tokens.size(); }

	protected:
		/** Newick tokens. */
		std::vector<std::string> m_tokens;
	};

}

#endif
