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

#ifndef _GENGIS_STUDY_CONTROLLER_
#define _GENGIS_STUDY_CONTROLLER_

#include "../core/Precompiled.hpp"

#include "../core/StudyModel.hpp"

class GenGIS::StudyController;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::StudyController * t, const unsigned int file_version);
	}
}

namespace GenGIS
{

	/**
	* @brief Handle events related to a given study.
	*/
	class StudyController
	{
	public:
		/** Construction. */
		explicit StudyController(StudyModelPtr studyModel): m_studyModel(studyModel) {}

		/** Destructor. */
		~StudyController() {}

		/** Get flag indicating if coordinates are geographic (i.e., lat/long) or projected (i.e., measures in meters or feet) */
		bool IsGeographic() const
		{
			return m_studyModel->IsGeographic();
		}

		/** Set flag indicating if coordinates are geographic (i.e., lat/long) or projected (i.e., measures in meters or feet) */
		void SetGeographic(bool state)
		{
			m_studyModel->SetGeographic(state);
		}

		/**
		* @brief Calculate projection tool.
		* @param oSource Coordinate system of source. If NULL, assume lat/lon with same datum as projected coordinate system.
		*/
		void CalculateProjectionTool(OGRSpatialReference* oSource) const
		{
			return m_studyModel->CalculateProjectionTool(oSource);
		}

		/** Get previously calculated projection tool. */
		ProjectionToolPtr GetProjectionTool() const
		{
			return m_studyModel->GetProjectionTool();
		}
		void SetProjectionTool(OGRCoordinateTransformation* projectionTool) 
		{
			return m_studyModel->SetProjectionTool(projectionTool);

		}


		/** Get specified destination coordinate system. */
		OGRSpatialReference GetDestinationProj() const
		{
			return m_studyModel->GetDestinationProj();
		}

		/** Set flag indicating if data should be projected. */
		void SetProjectData(bool state)
		{
			m_studyModel->SetProjectData(state);
		}

		/** Get flag indicating if data should be projected. */
		bool IsProjectData() const
		{
			return m_studyModel->IsProjectData();
		}

		/** Get flag indicating if data is Geographic. For use with Python*/
		bool IsUsingGeographic() const
		{
			return m_studyModel->IsUsingGeographic();
		}
		/** Get flag indicating if data is projected. For use with Python*/
		bool IsUsingProjection() const
		{
			return m_studyModel->IsUsingProjection();
		}
		/** Set flag indicating if data is Geographic. For use with Python*/
		void SetUsingGeographic(bool state)
		{
			m_studyModel->SetUsingGeographic(state);
		}
		/** Set flag indicating if data is projected. For use with Python*/
		void SetUsingProjection(bool state)
		{
			m_studyModel->SetUsingProjection(state);
		}

		/** Set datum. */
		void SetDatum(const std::wstring& datum)
		{
			m_studyModel->SetDatum(datum);
		}

		/** Get datum. */
		std::wstring GetDatum() const
		{
			return m_studyModel->GetDatum();
		}

		/** Set projection. */
		void SetProjection(const std::wstring& projection)
		{
			m_studyModel->SetProjection(projection);
		}

		/** Get projection. */
		std::wstring GetProjection() const
		{
			return m_studyModel->GetProjection();
		}

		/** Set centre latitude. */
		void SetCentreLatitude(float centre)
		{
			m_studyModel->SetCentreLatitude(centre);
		}

		/** Get centre Longitude. */
		float GetCentreLongitude() const
		{
			return m_studyModel->GetCentreLongitude();
		}

		/** Set centre Longitude. */
		void SetCentreLongitude(float centre)
		{
			m_studyModel->SetCentreLongitude(centre);
		}

		/** Get centre latitude. */
		float GetCentreLatitude() const
		{
			return m_studyModel->GetCentreLatitude();
		}

		/** Set first standard parallel. */
		void SetFirstStandardParallel(float sp)
		{
			m_studyModel->SetFirstStandardParallel(sp);
		}

		/** Get first standard parallel. */
		float GetFirstStandardParallel() const
		{
			return m_studyModel->GetFirstStandardParallel();
		}

		/** Set second standard parallel. */
		void SetSecondStandardParallel(float sp)
		{
			m_studyModel->SetSecondStandardParallel(sp);
		}

		/** Get second standard parallel. */
		float GetSecondStandardParallel() const
		{
			return m_studyModel->GetSecondStandardParallel();
		}

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state)
		{
			m_studyModel->SetActive(state);
		}

		/** Get active state of layer. */
		bool IsActive() const
		{
			return m_studyModel->IsActive();
		}

		/** Toggle active state of layer. */
		void ToggleActive()
		{
			m_studyModel->ToggleActive();
		}

		/** Get study model */
		StudyModelPtr GetStudyModel() const
		{
			return m_studyModel;
		}

	protected:
		/** Study model associated with this controller. */
		StudyModelPtr m_studyModel;

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const StudyController* t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			// Needs to be here
		}
	};

}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const StudyController * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//ar << t->GetStudyModel(); // StudyModelPtr
			ar << t->m_studyModel; // StudyModelPtr
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, StudyController * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			StudyModelPtr _studyModel;
			ar >> _studyModel;

			::new(t)StudyController(_studyModel);
		}
	}
}

#endif
