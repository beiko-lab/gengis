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

#ifndef _GENGIS_STUDY_MODEL_
#define _GENGIS_STUDY_MODEL_

#include "../core/Precompiled.hpp"


namespace GenGIS
{

	/**
	* @brief Study specific properties.
	*/
	class StudyModel 
	{
	public:

		//** Construction. */
		explicit StudyModel(
			const std::wstring& datum = _T("WGS84"),const std::wstring& projection = _T("Plate Carree (Equirectangular)"), 
			uint utmZone = 30, const std::wstring& utmHemisphere = _T("North") ) :
				m_bProjectData(true), m_bUsingGeographic(true), m_bUsingProjection(true), m_datum(datum), m_projection(projection), m_centreLat(INT_MIN),	
				m_centreLong(INT_MIN), m_standardParallel1(INT_MIN), m_standardParallel2(INT_MIN), m_projectionTool(ProjectionToolPtr()), m_bActive(true) {}

		/** Destructor. */
		~StudyModel() {}



		/** Get flag indicating if coordinates are geographic (i.e., lat/long) or projected (i.e., measures in meters or feet) */
		bool IsGeographic() const
		{
			return m_bGeographic;
		}

		/** Set flag indicating if coordinates are geographic (i.e., lat/long) or projected (i.e., measures in meters or feet) */
		void SetGeographic(bool state)
		{
			m_bGeographic = state;
		}

		/**
		* @brief Calculate projection tool.
		* @param oSource Coordinate system of source. If NULL, assume lat/lon with same datum as projected coordinate system.
		*/
		void CalculateProjectionTool(OGRSpatialReference* oSource);

		/** Get previously calculated projection tool. */
		ProjectionToolPtr GetProjectionTool() const
		{
			return m_projectionTool;
		}
		void SetProjectionTool(OGRCoordinateTransformation* projectionTool) 
		{
			m_projectionTool.reset(projectionTool);
		}


		/** Get specified destination coordinate system. */
		OGRSpatialReference GetDestinationProj()
		{
			SetDestinationProjection();
			return m_oDestination;
		}

		/** Set flag indicating if data should be projected. */
		void SetProjectData(bool state)
		{
			m_bProjectData = state;
		}

		/** Get flag indicating if data should be projected. */
		bool IsProjectData() const
		{
			return m_bProjectData;
		}
		
		/** Get flag indicating if data is Geographic. For use with Python*/
		bool IsUsingGeographic() const
		{
			return m_bUsingGeographic;
		}
		/** Get flag indicating if data is projected. For use with Python*/
		bool IsUsingProjection() const
		{
			return m_bUsingProjection;
		}
		/** Set flag indicating if data is Geographic. For use with Python*/
		void SetUsingGeographic(bool state)
		{
			m_bUsingGeographic=state;
		}
		/** Set flag indicating if data is projected. For use with Python*/
		void SetUsingProjection(bool state)
		{
			m_bUsingProjection=state;
		}


		/** Set datum. */
		void SetDatum(const std::wstring& datum)
		{
			m_datum = datum;
		}

		/** Get datum. */
		std::wstring GetDatum() const
		{
			return m_datum;
		}

		/** Set projection. */
		void SetProjection(const std::wstring& projection)
		{
			m_projection = projection;
		}

		/** Get projection. */
		std::wstring GetProjection() const
		{
			return m_projection;
		}

		/** Set centre latitude. */
		void SetCentreLatitude(float centre);

		/** Get centre Longitude. */
		float GetCentreLongitude() const
		{
			return m_centreLat;
		}

		/** Set centre Longitude. */
		void SetCentreLongitude(float centre);

		/** Get centre latitude. */
		float GetCentreLatitude() const
		{
			return m_centreLong;
		}

		/** Set first standard parallel. */
		void SetFirstStandardParallel(float sp);

		/** Get first standard parallel. */
		float GetFirstStandardParallel() const
		{
			return m_standardParallel1;
		}

		/** Set second standard parallel. */
		void SetSecondStandardParallel(float sp);

		/** Get second standard parallel. */
		float GetSecondStandardParallel() const
		{
			return m_standardParallel2;
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

	protected:
		/** Set desired projection. */
		void SetDestinationProjection();

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		//template<class Archive>
		//friend void boost::serialization::save_construct_data(Archive & ar, const StudyModel * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);
		template<class Archive>
		void save(Archive & ar, const unsigned int version) const;
		template<class Archive>
		void load(Archive & ar, const unsigned int version);

	protected:
		/** Flag indicating if this sequence is currently in the active set. */
		bool m_bActive;

		/** Desired geographic coordinate system. */
		OGRSpatialReference m_oDestination;

		ProjectionToolPtr m_projectionTool;

		/** Flag indicating if coordinates are geographic (i.e., lat/long) or projected (i.e., measures in meters or feet) */
		bool m_bGeographic;

		/** Flag indicating if data should be projected. */
		bool m_bProjectData;

		/** Currently active datum. */
		std::wstring m_datum;

		/** Currently active projection. */
		std::wstring m_projection;

		/** Latitude at centre of map. */
		float m_centreLat;

		/** Longitude at centre of map. */
		float m_centreLong;

		/** First standard parallel to use for map projection. */
		float m_standardParallel1;

		/** Second standard parallel to use for map projection. */
		float m_standardParallel2;

		/** Flag indicating if coordinates are geographic (i.e., lat/long) or projected (i.e., measures in meters or feet)
		*   For Python use only
		*/
		bool m_bUsingGeographic;

		/** Flag indicating if data should be projected.
		*   For Python use only
		*/
		bool m_bUsingProjection;
	};
}

#endif
