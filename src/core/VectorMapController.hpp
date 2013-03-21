//=======================================================================
// Author: Timothy Mankowski & Somayyeh Zangooei
//
// Copyright 2012 Timothy Mankowski & Somayyeh Zangooei
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

#ifndef _GENGIS_VECTOR_MAP_CONTROLLER_
#define _GENGIS_VECTOR_MAP_CONTROLLER_

#include "../core/Precompiled.hpp"

#include "../core/VectorMapModel.hpp"
#include "../core/VectorMapView.hpp"

namespace GenGIS
{
	/**
	* @brief Controller for map model.
	*/
	class VectorMapController
	{
	public:
		/** 
		* @brief Construction. 
		*/
		explicit VectorMapController()
		{
			m_vectorMapModel = VectorMapModelPtr( new VectorMapModel() );
			m_vectorMapView  = VectorMapViewPtr( new VectorMapView( m_vectorMapModel ) );
		}

		/** Destructor. */
		~VectorMapController() {}

		/** Get OpenGL (3D) vector map view. */
		VectorMapViewPtr GetVectorMapView() const;

		/** Get Vector Map Model. */
		VectorMapModelPtr GetVectorMapModel() const
		{
			return m_vectorMapModel;
		}

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state)
		{
			m_vectorMapModel->SetActive(state);
		}

		/** Get active state of layer. */
		bool IsActive() const
		{
			return m_vectorMapModel->IsActive();
		}

		/** Toggle active state of layer. */
		void ToggleActive()
		{
			m_vectorMapModel->ToggleActive();
		}
		vFileMetaData* GetVectorMetaData() 
		{
			return m_vectorMapModel->GetVectorMetaData(); 
		}
		bool IsLoaded() const
		{
			return m_bVectorMapLoaded;
		}

		/** Set flag indicating if a map is loaded. */
		void SetLoaded(bool state)
		{
			m_bVectorMapLoaded = state;
		}
		/** Get the original extent of the vector map */
		Box2D GetOriginalVectorExtents()
		{
			 return m_vectorMapModel->GetOriginalVectorExtents();
		}
		/** Get the extent of the vector map after projection */
		Box2D GetProjectedVectorExtents()
		{
			 return m_vectorMapModel->GetProjectedVectorExtents();
		}
		/** Get geometry type of features */
		std::string GetGeometryType()
		{
			 return m_vectorMapModel->GetGeometryType();
		}

		/** Get number of features */
		int GetNumFeatures()
		{
			 return m_vectorMapModel->GetNumFeatures();
		}		

	private:

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Vector Map model. */
		VectorMapModelPtr m_vectorMapModel;

		/** OpenGL (3D) view of vector map model. */
		VectorMapViewPtr m_vectorMapView;
		bool m_bVectorMapLoaded;
	};
}

#endif