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

#ifndef _GENGIS_SAMPLE_MODEL_
#define _GENGIS_SAMPLE_MODEL_

#include "../core/Precompiled.hpp"

class GenGIS::SequenceModel;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::SequenceModel * t, const unsigned int file_version);
	}
}

namespace GenGIS 
{

	/**
	* @brief Sequence data.
	*/
	class SequenceModel
	{
	public:
		/** 
		* @brief Constuctor.
		* @param sequenceId Unique id for sequence.
		* @param siteId Unique id of location site associated with sequence.
		* @param data Data associated with this location.
		*/
		explicit SequenceModel(const std::wstring& sequenceId, const std::wstring& siteId, std::map<std::wstring,std::wstring> data) :
			m_sequenceId(sequenceId), m_siteId(siteId), m_bActive(true), m_data(data) {} 

		/** Destructor. */
		~SequenceModel() {}

		/** Get id of sequence. */
		std::wstring GetSequenceId() const
		{
			return m_sequenceId;
		}

		/** Get id of location site associated with sequence. */
		std::wstring GetSiteId() const
		{
			return m_siteId;
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

		/** Get data associate with sequence. */
		std::map<std::wstring,std::wstring> GetData() const
		{
			return m_data;
		}

		/** 
		* @brief Get data for specified field. 
		* @param field Field to get data for. 
		* @return Data associated with the provided field.
		*/
		std::wstring GetData(const std::wstring& field) const;

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const SequenceModel * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	private:
		/** Unique id for sequence. */
		std::wstring m_sequenceId;

		/** Unique id for location site associated with sequence. */
		std::wstring m_siteId;

		/** Data associated with sequence. */
		std::map<std::wstring,std::wstring> m_data;

		/** Flag indicating if this sequence is currently in the active set. */
		bool m_bActive;
	};

}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::SequenceModel * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//ar << t->GetSequenceId(); // m_sequenceId (std::wstring)
			//ar << t->GetSiteId();     // m_siteId     (std::wstring)
			//ar << t->GetData();       // m_data       (std::map<std::wstring,std::wstring>)

			ar << t->m_sequenceId; // std::wstring
			ar << t->m_siteId;     // std::wstring
			ar << t->m_data;       // std::map<std::wstring,std::wstring>
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, GenGIS::SequenceModel * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			std::wstring _sequenceId;
			ar >> _sequenceId;

			std::wstring _siteId;
			ar >> _siteId;

			std::map<std::wstring,std::wstring> _data;
			ar >> _data;

			::new(t)SequenceModel(_sequenceId, _siteId, _data);
		}
	}
}

#endif
