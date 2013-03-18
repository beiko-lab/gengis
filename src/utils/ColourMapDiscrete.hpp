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

#ifndef _GENGIS_COLOUR_MAP_DISCRETE_
#define _GENGIS_COLOUR_MAP_DISCRETE_

#include "../utils/ColourMap.hpp"

#include "../utils/Colour.hpp"

namespace GenGIS
{

	/**
	* @class ColourMapDiscrete
	* @brief Stores a set of colours that can be access by name.
	*/
	class ColourMapDiscrete: public ColourMap
	{
	public:
		/** Constructor. */
		explicit ColourMapDiscrete(): ColourMap(_T("<no name>"), DISCRETE) {}

		/** Constructor. */
		explicit ColourMapDiscrete(const std::wstring& name, COLOUR_MAP_TYPE type = DISCRETE): ColourMap(name, type) {}

		/** Constructor. */
		explicit ColourMapDiscrete(ColourMapPtr colourMap);

		/** Constructor. */
		explicit ColourMapDiscrete(ColourMapDiscretePtr colourMap);

		/** Destructor. */
		~ColourMapDiscrete() {}

		/** 
		* @brief Copy colour map. 
		* @param colourMap Colour map to copy colours from.
		*/
		void CopyColourMap(ColourMapDiscretePtr colourMap);

		/** 
		* @brief Copy colour map. 
		* @param colourMap Colour map to copy colours from.
		*/
		void CopyColourMap(ColourMapPtr colourMap) { ColourMap::CopyColourMap(colourMap); }

		/** 
		* @brief Assign colour to a given index and name. 
		* @param name Name to associate with this colour.
		* @param colour Desired colour.
		*/
		void SetColour(const std::wstring& name, const Colour& colour);

		/** Set colour of colour map at a given index. */
		void SetColour(const Colour& colour, uint index) { ColourMap::SetColour(colour, index); }

		/** Get colour associated with the given name. */
		bool GetColour(const std::wstring& name, Colour& colour);

		/** Get colour from underlying colour map. */
		Colour GetColour(uint index) const { return ColourMap::GetColour(index); }

		/** Interpolate colour from underlying colour map. */
		Colour GetInterpolatedColour(float value, float minValue, float maxValue) const { return ColourMap::GetInterpolatedColour(value, minValue, maxValue); }

		/** Clear names associated with colours. */
		void ClearNames() { m_colourNames.clear(); }

		/** Get map which associates names with colour. */
		std::map<std::wstring, Colour> GetNameToColourMap() { return m_colourNames; }

		/** Get size of map which associates names to colours. */
		uint GetNameToColourMapSize() { return m_colourNames.size(); }

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Name associated with each colour. */
		std::map<std::wstring, Colour> m_colourNames;
	};

}

#endif
