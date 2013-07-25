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

#ifndef _GENGIS_VISUAL_MARKER_
#define _GENGIS_VISUAL_MARKER_

#include "../core/Precompiled.hpp"

#include "../core/VisualObject.hpp"

namespace GenGIS
{

	/** A marker to be rendered. */
	class VisualMarker : public VisualObject
	{
	public:
		/** Different shapes of visual markers. */
		enum MARKER_SHAPE { CIRCLE, CIRCLE_FAST, SQUARE, TRIANGLE, STAR, PLUS_SIGN, OCTAGON, DIAMOND, INVERTED_TRIANGLE };

		/** Different methods for drawing a marker. */
		enum MARKER_RENDERING_STYLE { DECAL, ALWAYS_VISIBLE, MARKER_3D, SCREEN_POS };

	public:
		/** Constructor. */
		explicit VisualMarker();

		/** Constructor. */
		explicit VisualMarker(Colour colour, float size, MARKER_SHAPE shape, const Point3D& position);

		/** Destructor. */
		~VisualMarker() {}

		/** Render marker. */
		void Render();

		static void RenderAlwaysVisibleDecal(const Point3D& position, const Colour& colour, float size, MARKER_SHAPE shape, bool bSelection, int renderingOffset);

		/** Render marker as a decal on the terrain which can be subject to occlusion. */
		static void RenderDecal(const Point3D& position, const Colour& colour, float size, MARKER_SHAPE shape, 
			const Colour& borderColour, float borderSize, bool bSelection, float renderingOrder);

		/** Render marker such that it is always visible (i.e., not subject to occlusion). */
		static void RenderAlwaysVisible(const Point3D& position, const Colour& colour, float size, MARKER_SHAPE shape, 
			float rotation, const Colour& borderColour, float borderSize, float depth);

		/** Render marker in screen space (useful for creating movies with labels). */
		static void RenderAtScreenPosition(const Point3D& position, const Colour& colour, float size, MARKER_SHAPE shape, 
			float depth);

		/** Render marker such that it is subject to occlusion. */
		static void Render3D(const Point3D& position, const Colour& colour, float size, MARKER_SHAPE shape, 
			float rotation, bool bSelection);

		/** Render marker. */
		static void RenderMarker(float size, const Colour& colour, MARKER_SHAPE shape);

		/** Set position of marker. */
		void SetPosition(const Point3D& pos)
		{
			m_position = pos;
		}

		/** Get position of marker. */
		Point3D GetPosition() const
		{
			return m_position;
		}

		/** Set shape of marker. */
		void SetShape(MARKER_SHAPE shape)
		{
			m_shape = shape;
		}

		/** Get shape of marker. */
		MARKER_SHAPE GetShape() const
		{
			return m_shape;
		}

		/** Set border size. */
		void SetBorderSize(float size)
		{
			m_borderSize = size;
		}

		/** Get border size. */
		float GetBorderSize() const
		{
			return m_borderSize;
		}

		/** Set border colour. */
		void SetBorderColour(const Colour& colour)
		{
			m_borderColour = colour;
		}

		/** Get border colour. */
		Colour GetBorderColour() const
		{
			return m_borderColour;
		}

		/** Set rotation about x-axis of marker. */
		void SetRotation(float rotation)
		{
			m_rotation = rotation;
		}

		/** Get rotation of marker about x-axis. */
		float GetRotation() const
		{
			return m_rotation;
		}

		/** Set depth of marker. */
		void SetDepth(float depth)
		{
			m_depth = depth;
		}

		/** Get depth of marker. */
		float GetDepth() const
		{
			return m_depth;
		}

		/** Set render style for marker. */
		void SetRenderingStyle(MARKER_RENDERING_STYLE state)
		{
			m_bRenderingStyle = state;
		}

		/** Get render style for marker */
		MARKER_RENDERING_STYLE GetRenderingStyle() const
		{
			return m_bRenderingStyle;
		}

		/** Get rendering order. */
		float GetRenderingOrder() const
		{
			return m_renderingOrder;
		}

		/** Set rendering order. */
		void SetRenderingOrder(float renderingOrder)
		{
			m_renderingOrder = renderingOrder;
		}

	protected:
		/** Get 3D size of marker so it has a constant pixel size. */
		static float GetMarkerSize(const Point3D& position, float size);

	private:
		/** serialization. */
		friend class boost::serialization::access;
		template<class archive>
		void serialize(archive & ar, const unsigned int version);

	private:
		/** Position of marker. */
		Point3D m_position;

		/** Desired shape of marker. */
		MARKER_SHAPE m_shape;

		/** Rotation of marker about x-axis. */
		float m_rotation; 

		/** Render style for marker */
		MARKER_RENDERING_STYLE m_bRenderingStyle; 

		/** Depth of marker. */
		float m_depth;

		/** Rendering order of marker. */
		float m_renderingOrder;

		/** Border size. */
		float m_borderSize;

		/** Border colour. */
		Colour m_borderColour;
	};

}

#endif
