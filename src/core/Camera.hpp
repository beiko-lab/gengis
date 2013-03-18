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

#ifndef _CAMERA_
#define _CAMERA_

#include "../core/Precompiled.hpp"

#include "../utils/Point3D.hpp"

namespace GenGIS
{

	/**
	* @class Camera
	* @brief Intrinsic and extrinsic parameters of an OpenGL camera.
	*/
	class Camera: private Uncopyable
	{
	public:
		/** 
		* @brief Constructor. 
		* @param aspect Aspect ratio of camera.
		* @param fov Field-of-view of camera.
		* @param viewingNear Position of near viewing plane.
		* @param viewingFar Position of far viewing plane. 
		*/
		explicit Camera(float aspect, float fov = 45, float viewingNear = 0.01f, float viewingFar = 20.0f);

		/** Apply extrinsic perspective camera parameters (i.e., position, pitch, and yaw). */
		void ApplyExtrinsicParams();

		/** Set orthographic camera. */
		void SetOrthoCamera();

		/** Unset orhtographic camera and return to a perspective camera. */
		void UnsetOrthoCamera();

		/**
		* @brief Set aspect ratio of camera (i.e., width / height of viewport)
		* @param width Width of viewport.
		* @param height Height of viewport.
		*/
		void SetAspectRatio(unsigned int width, unsigned int height);

		/** Get aspect ratio. */
		float GetAspectRatio() const { return m_aspect; }

		/** Get FOV. */
		float GetFOV() const { return m_fov; }

		/** Get near viewing plane position. */
		float GetNearViewingPlane() const { return m_viewingNear; }

		/** Get far viewing plane position. */
		float GetFarViewingPlane() const { return m_viewingFar; }

		/**
		* @brief Set position of camera.
		* @param x,y Desired position.
		*/
		void SetPosition(float x, float y, float z);

		/** Get position of camera. */
		Point3D GetPosition() const { return m_translation; }

		/**
		* @brief Set height (elevation) of camera.
		* @param z Desired height.
		*/
		void SetHeight(float z);

		/** Get height (zoom) of camera. */
		float GetHeight() const { return m_translation.z; }

		/**
		* @brief Set pitch of camera (i.e., rotation about x-axis).
		* @param angle Desired pitch.
		*/
		void SetPitch(float angle) { m_pitch = angle; }

		/** Get pitch of camera. */
		float GetPitch() const { return m_pitch; }

		/**
		* @brief Set yaw of camera (i.e., rotation about z-axis).
		* @param angle Desired yaw.
		*/
		void SetYaw(float angle) { m_yaw = angle; }	

		/** Get yaw of camera. */
		float GetYaw() const { return m_yaw; }

		/**
		* @brief Change zoom (z-position) of camera by indicated percentage.
		* @param fraction Amount (as a percentage) to change zoom by.
		*/
		void ZoomDelta(float fraction);

		/**
		* @brief Move position of camera.
		* @param dx, dy Amount to move camera by.
		*/
		void TranslateDelta(float dx, float dy);

		/**
		* @brief Change camera pitch.
		* @param angle Amount to change camera pitch by.
		*/
		void PitchDelta(float angle);

		/**
		* @brief Change camera yaw.
		* @param angle Amount to change camera yaw by.
		*/
		void YawDelta(float angle);

		/** Set camera to a reasonable default view. */
		void ResetView();

		/** Set camera to show overhead view of map. */
		void TopView();

		/** Amount of zoom required to view the complete extents of the map. */
		float GetMapExtentsZoom();

		/**
		* @brief Set extents of the model.
		* @param minX, maxX Extents of model in x-dimension.
		* @param minY, maxY Extents of model in y-dimension. 
		*/
		void SetModelSpaceExtents(float minX, float maxX, float minY, float maxY);

		/** Get height on OpenGL viewport window in pixel. */
		unsigned int GetWindowHeight() const { return m_viewportHeight; }

		/** Get width on OpenGL viewport window in pixel. */
		unsigned int GetWindowWidth() const { return m_viewportWidth; }

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

		/** Apply intrinsic camera parameters (i.e., field of view, aspect ratio. */
		void ApplyIntrinsicParams();

	private:
		/** Aspect ratio of viewport. */
		float m_aspect;

		/** Field of view of camera. */
		float m_fov;

		/** Near clipping plane. */
		float m_viewingNear;

		/** Far clipping plane. */
		float m_viewingFar;

		/** Camera translation relative to the center point of the model. */
		Point3D m_translation;

		/** Pitch of camera (rotation about x-axis). */
		float m_pitch;

		/** Yaw of camera (rotation about z-axis). */
		float m_yaw;

		/** Width extents of objects in model space. */
		float m_minX, m_maxX;

		/** Height extents of objects in model space. */
		float m_minY, m_maxY;

		/** Width and height of objects in model space. */
		float m_width, m_height;

		/** Width and height of viewport. */
		unsigned int m_viewportWidth, m_viewportHeight;
	};
}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const Camera * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			float _aspect;
			_aspect = t->GetAspectRatio(); // m_aspect (float)
			ar << _aspect;
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, Camera * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			float _aspect;
			ar >> _aspect;

			::new(t)Camera(_aspect);
		}
	}
}

#endif
