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

#ifndef _GENGIS_LOCATION_MODEL_
#define _GENGIS_LOCATION_MODEL_

#include "../core/Precompiled.hpp"

class GenGIS::LocationModel;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::LocationModel * t, const unsigned int file_version);
	}
}

namespace GenGIS 
{

	/**
	* @brief Location of samples in projected coordinate space.
	*/
	class LocationModel
	{
	public:
		/** 
		* @brief Constuctor.
		* @param siteId Unique id for location site.
		* @param northing Northing coordinate of location.
		* @param easting Easting coordinate of locaiton.
		* @param data Data associated with this location.
		*/
		explicit LocationModel(const std::wstring siteId, float northing, float easting, std::map<std::wstring,std::wstring> data) :
			m_id(siteId), m_northing(northing), m_easting(easting), m_data(data), m_bActive(true) {} 

		/** Destructor. */
		~LocationModel() {}

		/** Get id of location site. */
		const std::wstring GetId() const
		{
			return m_id;
		}

		/**
		* @brief Set northing coordinates of location.
		* @param Northing coordinates.
		*/
		void SetNorthing( float northing )
		{
			m_northing = northing;
		}

		/** Get northing position of location. */
		const float& GetNorthing() const
		{
			return m_northing;
		}

		/**
		* @brief Set easting coordinates of location.
		* @param Easting coordinates.
		*/
		void SetEasting( float easting )
		{
			m_easting = easting;
		}

		/** Get easting position of location. */
		const float& GetEasting() const
		{
			return m_easting;
		}

		/** Get projected coordinates of location. */
		GeoCoord GetProjection() const
		{
			return GeoCoord(m_easting, m_northing);
		}

		/**
		* @brief Add sequence to location.
		* @param sequence Sequence to add.
		*/
		void AddSequence(SequenceModelPtr sequence)
		{
			m_sequences.push_back(sequence);
		}

		/** Get number of sequence at this location. */
		unsigned int GetNumSequence() const
		{
			return m_sequences.size();
		}

		/**
		* @brief Get sequence from location.
		* @param index Index of desired sequence.
		* @return Sample at given index.
		*/
		SequenceModelPtr GetSequence(unsigned int index) const
		{
			return m_sequences.at(index);
		}

		/**
		* @brief Remove sequence from location.
		* @param index Sequence to remove.
		*/
		void RemoveSequence(unsigned int index)
		{
			m_sequences.erase(m_sequences.begin() + index);
		}

		/** Remove all sequence from location. */
		void RemoveAllSequence()
		{
			m_sequences.clear();
		}

		/** Get data associate with location site. */
		std::map<std::wstring,std::wstring> GetData() const
		{
			return m_data;
		}

		/** 
		* @brief Get data for specified field. 
		* @param field Field to get data for. 
		* @return Data associated with the provided field.
		*/
		std::wstring GetData(const std::wstring& field)
		{
			return m_data[field];
		}

		/** 
		* @brief Add metadata to location.
		* @param field New field.
		* @param value Value of field.
		*/
		void AddData(const std::wstring& field, const std::wstring& value)
		{
			m_data[field] = value;
		}

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state)
		{
			m_bActive = state;
		}

		/** Get active state of layer. */
		bool IsActive() const
		{
			return m_bActive;
		}

		/** Toggle active state of layer. */
		void ToggleActive()
		{
			m_bActive = !m_bActive;
		}

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const LocationModel * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Flag indicating if this sequence is currently in the active set. */
		bool m_bActive;

		/** Unique id for location site. */
		std::wstring m_id;

		/** Northing coordinate of location. */
		float m_northing;

		/** Easting coordinate of location. */
		float m_easting;

		/** Data associated with location site. */
		std::map<std::wstring,std::wstring> m_data;

		/** All samples taken at this location. */
		std::vector<SequenceModelPtr> m_sequences;
	};

}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::LocationModel * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//ar << t->GetId();              // (m_id, std::wstring&)
			//ar << t->GetNorthing();        // (m_northing, float)
			//ar << t->GetEasting();         // (m_easting, float)
			//ar << t->GetData();            // (m_data, std::map<std::wstring,std::wstring>)

			ar << t->m_id;       // std::wstring&
			ar << t->m_northing; // float
			ar << t->m_easting;  // float
			ar << t->m_data;     // std::map<std::wstring,std::wstring>
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, GenGIS::LocationModel * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			std::wstring _id;
			ar >> _id;

			float _northing;
			ar >> _northing;

			float _easting;
			ar >> _easting;

			std::map<std::wstring,std::wstring> _data;
			ar >> _data;

			::new(t)LocationModel(_id, _northing, _easting, _data);
		}
	}
}

#endif
