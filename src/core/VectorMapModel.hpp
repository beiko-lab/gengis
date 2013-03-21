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

#ifndef _GENGIS_VECTOR_MAP_MODEL_
#define _GENGIS_VECTOR_MAP_MODEL_

#include "../core/Precompiled.hpp"

#include "../utils/Log.hpp"

namespace GenGIS
{
	typedef struct vFILE_METADATA
	{
		vFILE_METADATA(std::string _originalProj = "", std::string _destinationProj="", std::string _type = "", int _count = 0, Box2D _originalExt =(0.0,0.0), Box2D _currentExt=(0.0,0.0)) :
		originalProjection(_originalProj), destinationProjection(_destinationProj), geometryType (_type), 
		numFeatures ( _count), originalExtents(_originalExt), currentExtents(_currentExt){}		
		
		std::string originalProjection;           // string indicating projection parameters
		std::string destinationProjection;
		std::string geometryType;             //geometry type of features stored in vector file

		int numFeatures;
		Box2D originalExtents;    // extents of Vector (lower, left corner to upper, right)
		Box2D currentExtents;    // extents of Vector after projection (lower, left corner to upper, right)	


		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & originalProjection;        
			ar & destinationProjection;       
			ar & geometryType;       
			ar & numFeatures;       
			ar & originalExtents;      
			ar & currentExtents;   
			
		}

	} vFileMetaData;

	/** Structure specifying boundaries of vector file  */
	typedef struct VECTOR_BOUNDARY 
	{
		VECTOR_BOUNDARY(float _minx = 0, float _miny = 0, float _maxx = 0, float _maxy = 0): min_X(_minx), min_Y(_miny), max_X(_maxx), max_Y(_maxy) {}
		GLdouble min_X;
		GLdouble min_Y;
		GLdouble max_X;		
		GLdouble max_Y;
		GLdouble scaledMin_X;
		GLdouble scaledMin_Y;
		GLdouble scaledMax_X;		
		GLdouble scaledMax_Y;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & min_X;        
			ar & min_Y;       
			ar & max_X;       
			ar & max_Y;  
	
		}

		
	} Vector_Boundary;

	/** Class of OGR feature. */
	class GeoVector
	{
	public:

		double *pointX;
		double *pointY;

		GeoVector() : m_numberOfPoints( 0 ), pointX( NULL ), pointY( NULL ) {}

		~GeoVector()
		{
			delete [] pointX; pointX = NULL;
			delete [] pointY; pointY = NULL;
		}
	
		void SetGeometryType( OGRwkbGeometryType geometryType )
		{
			m_geometryType = geometryType;
		}

		OGRwkbGeometryType GetGeometryType()
		{
			return m_geometryType;
		}

		unsigned int GetNumberOfPoints()
		{
			return m_numberOfPoints;
		}

		bool SetNumberOfPoints( unsigned int count )
		{
			if ( count > 0 )
			{
				pointX = ( double* )realloc( pointX, count * sizeof( double ) );
				pointY = ( double* )realloc( pointY, count * sizeof( double ) );

				if ( ( pointX == NULL ) || ( pointY == NULL ) )
				{
					Log::Inst().Warning("(Error) Could not allocate memory for vector points.");
					return false;
				}
				else
				{
					m_numberOfPoints = count;
				}
			}
			return true;
		}

      private:
		unsigned int     m_numberOfPoints;		
		OGRwkbGeometryType m_geometryType;		
		
	};	
	class VectorMapModel: private Uncopyable  
	{
	public:
		
		/** Constructor. */
		explicit VectorMapModel();

		/** Destructor. */
		~VectorMapModel() {}

		/**
		* @brief Get vector file data information.
		* @return Vector file data information.
		*/
		GeoVector* GetGeoVector( unsigned int index ) { return m_geoVector.at( index ); }		

		/**
		* @brief Add vector file data information.
		* @return Nothing.
		*/
		void AddGeoVector( GeoVector* geoVector ) { m_geoVector.push_back( geoVector );  }		

		unsigned int GetNumberOfGeometries() { return m_geoVector.size(); }

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive( bool state ) { m_bActive = state; }

		/** Get active state of layer. */
		bool IsActive() const { return m_bActive; }

		/** Toggle active state of layer. */
		void ToggleActive() { m_bActive = !m_bActive; }

        Vector_Boundary* GetBoundary()     { return &m_Vector_Boundary; }

		Box2D GetOriginalVectorExtents()
		{
			 return m_vectorMetaData.originalExtents;
		}
		Box2D GetProjectedVectorExtents()
		{
			 return m_vectorMetaData.currentExtents;
		}
		std::string GetGeometryType()
		{
			 return m_vectorMetaData.geometryType;
		}

		int GetNumFeatures()
		{
			 return m_vectorMetaData.numFeatures;
		}		


		bool SetVectorBoundary(OGREnvelope psEnvelope)
		{
			m_Vector_Boundary.min_X=psEnvelope.MinX;
			m_Vector_Boundary.min_Y=psEnvelope.MinY;
			m_Vector_Boundary.max_X=psEnvelope.MaxX;
			m_Vector_Boundary.max_Y=psEnvelope.MaxY;
			return true;
		}

		void SetVectorBoundary_MinX(GLdouble minX)
		{
			 m_Vector_Boundary.min_X=minX;
		}
		void SetVectorBoundary_MaxX(GLdouble maxX)
		{
			 m_Vector_Boundary.max_X=maxX;
		}
		void SetVectorBoundary_MinY(GLdouble minY)
		{
			 m_Vector_Boundary.min_Y=minY;
		}		
		void SetVectorBoundary_MaxY(GLdouble maxY)
		{
			 m_Vector_Boundary.max_Y=maxY;
		}


		void SetVectorBoundary_ScaledMinX(GLdouble scaledMinX)
		{
			 m_Vector_Boundary.scaledMin_X=scaledMinX;
		}
		void SetVectorBoundary_ScaledMinY(GLdouble scaledMinY)
		{
			 m_Vector_Boundary.scaledMin_Y=scaledMinY;
		}
		void SetVectorBoundary_ScaledMaxX(GLdouble scaledMaxX)
		{
			 m_Vector_Boundary.scaledMax_X=scaledMaxX;
		}
		void SetVectorBoundary_ScaledMaxY(GLdouble scaledMaxY)
		{
			 m_Vector_Boundary.scaledMax_Y=scaledMaxY;
		}
		

		GLdouble GetVectorBoundary_ScaledMinX()
		{
			return m_Vector_Boundary.scaledMin_X;
		}
		GLdouble GetVectorBoundary_ScaledMinY()
		{
			 return m_Vector_Boundary.scaledMin_Y;
		}
		GLdouble GetVectorBoundary_ScaledMaxX()
		{
			 return m_Vector_Boundary.scaledMax_X;
		}
		GLdouble GetVectorBoundary_ScaledMaxY()
		{
			 return m_Vector_Boundary.scaledMax_Y;
		}


		GLdouble GetVectorBoundary_MinX()
		{
			return m_Vector_Boundary.min_X;
		}
		GLdouble GetVectorBoundary_MinY()
		{
			return m_Vector_Boundary.min_Y;
		}
		GLdouble GetVectorBoundary_MaxX()
		{
			return m_Vector_Boundary.max_X;
		}
		GLdouble GetVectorBoundary_MaxY()
		{
			return m_Vector_Boundary.max_Y;
		}	


		void setSpatialReference(OGRSpatialReference* crrShpSR){
			currentSRS=crrShpSR;
		}
		OGRSpatialReference* getSpatialReference(){
			return currentSRS;
		}
		void SetHeight(double height){
			vector_height=height;
		}
		double GetHeight()
		{
			return vector_height; 
		}
		vFileMetaData* GetVectorMetaData() { return &m_vectorMetaData; }

	private:
		/** Flag indicating if this sequence is currently in the active set. */
		bool m_bActive;		
		std::vector<GeoVector*> m_geoVector;		
		
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		Vector_Boundary m_Vector_Boundary;
		OGRSpatialReference* currentSRS;
		double vector_height;
		vFileMetaData m_vectorMetaData;

	};
}

#endif