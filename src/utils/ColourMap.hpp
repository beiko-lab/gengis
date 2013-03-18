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

#ifndef _GENGIS_COLOUR_MAP_
#define _GENGIS_COLOUR_MAP_

#include "../utils/Colour.hpp"

namespace GenGIS
{

	/**
	* @class ColourMap
	* @brief Stores a set of colours that can be access by index or name.
	*/
	class ColourMap
	{
	public:
		/** Different types of colour maps. */
		enum COLOUR_MAP_TYPE { UNKNOWN, CONTINUOUS, DISCRETE, DISCRETIZED_CONTINUOUS };

	public:
		/** Constructor. */
		explicit ColourMap(const std::wstring& name = _T("<no name>"), COLOUR_MAP_TYPE type = UNKNOWN): m_name(name), m_type(type), m_bCustom(false) {}

		/** Constructor. */
		explicit ColourMap(ColourMapPtr colourMap);

		/** Destructor. */
		virtual ~ColourMap() {}

		/** Add colour to colour map. */
		virtual void AddColour(const Colour& colour) { m_colourMap.push_back(colour); }

		/** Set colour of colour map at a given index. */
		virtual void SetColour(const Colour& colour, uint index);

		/** Get colour from colour map. */
		virtual Colour GetColour(uint index) const;

		/** Interpolate colour from colour map. */
		virtual Colour GetInterpolatedColour(float value, float minValue, float maxValue) const;

		/** Set name of colour map. */
		void SetName(const std::wstring& name) { m_name = name; }

		/** Get name of colour map. */
		std::wstring GetName() const { return m_name; }

		/** Get name of colour map with either "Continuous: " or "Discrete: " prefixed to it. */
		virtual std::wstring GetExtendedName();

		/** Reduce an extended colour name (i.e., strip the "Continuous: " or "Discrete: " prefixed). */
		std::wstring ReduceExtendedName(const std::wstring& name);

		/** Get the size of the colour map. */
		uint GetSize() const;

		/** Set type of colour map. */
		void SetType(COLOUR_MAP_TYPE type) { m_type = type; }

		/** Get type of colour map. */
		COLOUR_MAP_TYPE GetType() const { return m_type; }

		/** 
		* @brief Copy colour map. 
		* @param colourMap Colour map to copy colours from.
		*/
		virtual void CopyColourMap(ColourMapPtr colourMap);

		/** 
		* @brief Read type information from a colour map file.
		* @param filename Filename (including path) of colour map to read.
		* @return Type of colour map.
		*/
		COLOUR_MAP_TYPE ReadType(const std::string& filename);

		/** 
		* @brief Load colour map from file. 
		* @param filename Filename (including path) of colour map to load.
		* @return True if colour maps loaded successfully, otherwise False.
		*/
		bool Load(const std::string& filename);

		/** Determine if colour map has been modified (i.e., is a custom colour map). */
		bool IsCustom() const { return m_bCustom; }

		/** Set colour map as being custom or predefined. */
		void SetCustom(bool custom);

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Colour map. */
		std::vector<Colour> m_colourMap;

		/** Name of colour map. */
		std::wstring m_name;

		/** Type of colour map. */
		COLOUR_MAP_TYPE m_type;

		/** Flag indicating if colour map is custom. */
		bool m_bCustom;
	};

}

#endif
