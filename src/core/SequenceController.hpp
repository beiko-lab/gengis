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

#ifndef _GENGIS_SEQUENCE_CONTROLLER
#define _GENGIS_SEQUENCE_CONTROLLER

#include "../core/Precompiled.hpp"

#include "../core/SequenceModel.hpp"

class GenGIS::SequenceController;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::SequenceController * t, const unsigned int file_version);
	}
}

namespace GenGIS
{

	/**
	* @brief Handle events related to a given sequence.
	*/
	class SequenceController
	{
	public:
		/** 
		* @brief Constuctor.
		* @param sequenceModel Sequence model to associate with this controller.
		*/
		explicit SequenceController(SequenceModelPtr sequenceModel): m_sequenceModel(sequenceModel) {}

		/** Destructor. */
		~SequenceController() {}

		/** Get sequence model (for serialization). */
		SequenceModelPtr GetSequenceModel() const { return m_sequenceModel; }

		/** Get id of sequence. */
		const std::wstring GetSequenceId() const { return m_sequenceModel->GetSequenceId(); }

		/** Get id of location site associated with sequence. */
		const std::wstring GetSiteId() const { return m_sequenceModel->GetSiteId(); }

		/** Get data associate with sequence. */
		std::map<std::wstring,std::wstring> GetData() const { return m_sequenceModel->GetData(); }

		/** Set field being used to select active sequences. */
		static void SetActiveField(const std::wstring& activeField) { m_activeField = activeField; }

		/** Get field being used to select active sequences. */
		static std::wstring GetActiveField() { return m_activeField; }

		/** Get fields for sequence metadata. */
		std::vector<std::wstring> GetMetadataFields() const;

		/** Get numeric fields for sequence data. */
		std::vector<std::wstring> GetNumericMetadataFields(bool bOnlyActiveLocs = false) const;

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state) { m_sequenceModel->SetActive(state); }

		/** Get active state of layer. */
		bool IsActive() const { return m_sequenceModel->IsActive(); }

		/** Toggle active state of layer. */
		void ToggleActive() { m_sequenceModel->ToggleActive(); }

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const SequenceController * t, const unsigned int file_version);
		
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Sequence model associated with this controller. */
		SequenceModelPtr m_sequenceModel;

		/** Field being used to select active sequences. */
		static std::wstring m_activeField; 
	};
}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const SequenceController * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//ar << t->GetSequenceModel(); // m_sequenceModel (SequenceModelPtr)
			ar << t->m_sequenceModel; // SequenceModelPtr
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, SequenceController * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			SequenceModelPtr _sequenceModel;
			ar >> _sequenceModel;

			::new(t)SequenceController(_sequenceModel);
		}
	}
}

#endif
