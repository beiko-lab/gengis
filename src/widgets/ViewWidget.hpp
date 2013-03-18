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

#ifndef _GENGIS_VIEW2D_
#define _GENGIS_VIEW2D_

#include "../core/Precompiled.hpp"

#include "../core/View.hpp"

namespace GenGIS
{
	/**
	* @class ViewWidget
	* @brief Base class for 2D (orthographic) widgets.
	*/
	class ViewWidget : public View, private Uncopyable  
	{
	public:
		/** Constructor. */
		explicit ViewWidget() 
		{ 
			SetScale( 1, 1, 1 ); 
			m_textured = false; 
		}

		/** Destructor. */
		virtual ~ViewWidget() {}

		/** Render widget. */
		virtual void Render();

		/** Get screen coordinates of widget. */
		virtual Point3D GetScreenCoordinates() const { return m_screenCoordinates; }

		/** Set screen coordinates of widget. */
		virtual void SetScreenCoords( const Point3D& screenC  );

		/** Get scaling factors of widget. */
		virtual const Point3D& GetScale() const {	return m_scale; }

		/** Set scaling factors of widget. */
		virtual void SetScale( float sx, float sy, float sz = 1 ) 
		{
			m_scale.x = sx;
			m_scale.y = sy;
			m_scale.z = sz;
		}            

		/** Get flag indicating if widget is textured. */
		virtual bool IsTextured() const { return m_textured; }

		/** Set texture for widget. */
		virtual void SetTexture( TexturePtr texture );

		/** Get bounding box of widget in screen coordinates. */
		virtual Box2D GetBoundingBox() const;

	protected:
		/** Scaling factors of widget. */
		Point3D m_scale;

		/** Flag indicating if widget is textured. */
		bool m_textured;

		/** Widget texture. */
		TexturePtr m_texture;

		/** Screen coordinates of widget. */
		Point3D m_screenCoordinates;
	};

}

#endif
