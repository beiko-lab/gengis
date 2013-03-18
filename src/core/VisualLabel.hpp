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

#ifndef _GENGIS_VISUAL_LABEL_
#define _GENGIS_VISUAL_LABEL_

#include "../core/Precompiled.hpp"

#include "../core/VisualObject.hpp"
#include "../core/App.hpp"

#include "../utils/Colour.hpp"
#include "../utils/Font.hpp"

namespace GenGIS
{

	/** A label to be rendered. */
	class VisualLabel : public VisualObject
	{
	public:
		/** Different methods for drawing a label. */
		enum LABEL_RENDERING_STYLE { PERSPECTIVE, ORTHO };

	public:
		/** Constructor. */
		explicit VisualLabel();

		/** Constructor. */
		explicit VisualLabel(const std::wstring& text, const Colour& colour, float size);

		/** Constructor for user generated labels. */
		explicit VisualLabel(const std::wstring& text, const Colour& colour, float size, LABEL_RENDERING_STYLE m_renderingStyle);

		/** Destructor. */
		~VisualLabel() {}

		/** Render label. */
		void Render();

		/** Set label text. */
		void SetText(const std::wstring& text)
		{
			m_text = text;
		}

		/** Get label text. */
		std::wstring GetText() const
		{
			return m_text;
		}

		/** Set label id. */
		void SetId(uint id)
		{
			m_id = id;
		}

		/** Get label id. */
		uint GetId() const
		{
			return m_id;
		}

		/** Set label position. */
		void SetLabelPosition(const std::wstring& position)
		{
			m_labelPosition = position;
		}

		/** Get label position. */
		std::wstring GetLabelPosition() const
		{
			return m_labelPosition;
		}

		/** Set label style. */
		void SetLabelStyle(const std::wstring& style);

		/** Get label style. */
		std::wstring GetLabelStyle() const
		{
			return m_labelStyle;
		}

		/** Set grid position of label. */
		void SetScreenPosition(const Point3D& pos)
		{
			m_screenPos = pos;
		}

		/** Get screen position of label. */
		Point3D GetScreenPosition() const
		{
			return m_screenPos;
		}

		/** Set grid position of label. */
		void SetGridPosition(const Point3D& pos)
		{
			m_bGridPosSet = true;
			m_gridPos     = pos;
		}

		/** Get grid position of label. */
		Point3D GetGridPosition() const
		{
			return m_gridPos;
		}

		/** Set size of label. */
		void SetSize(float size);

		/** Get size of label. */
		float GetSize() const
		{
			return m_size;
		}

		/** Set flag indicating if label should be rotated onto plane. */
		void SetRotateLabel(bool bRotate)
		{
			m_bRotateX = bRotate;
		}

		/** Get flag indicating if label should be rotated onto plane. */
		bool GetRotateLabel() const
		{
			return m_bRotateX;
		}

		/** Set flag indicating if label should be rotated about Z-axis. */
		void SetRotateZ(bool bRotate)
		{
			m_bRotateZ = bRotate;
		}

		/** Get flag indicating if label should be rotated about Z-axis. */
		bool GetRotateZ() const
		{
			return m_bRotateZ;
		}

		/** Set z-rotation of label. */
		void SetRotationZ(float rotation)
		{
			m_zRotation = rotation;
		}

		/** Get z-rotation of label. */
		float GetRotationZ() const
		{
			return m_zRotation;
		}

		/** Set render style for label. */
		void SetRenderingStyle(VisualLabel::LABEL_RENDERING_STYLE state)
		{
			m_renderingStyle = state;
		}

		/** Get render style for label */
		VisualLabel::LABEL_RENDERING_STYLE GetRenderingStyle() const
		{
			return m_renderingStyle;
		}

		/** Get bounding box of label. */
		Box2D GetBoundingBox() const;

		/** Show properties dialog for this view. */
		void ShowPropertiesDlg();

		/** 
		* @brief Process mouse left button down events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		bool MouseLeftDown(const Point2D& mousePt);

		/** 
		* @brief Process mouse left button up events. 
		* @param mousePt Screen position of mouse.
		*/
		void MouseLeftUp(const Point2D& mousePt);

		/** 
		* @brief Process mouse moving events. 
		* @param mousePt Screen position of mouse.
		* @return True if the OpenGL viewport should be refreshed.
		*/
		bool MouseMove(const Point2D& mousePt);

		/** 
		* @brief Process mouse dragging events. 
		* @param mousePt Screen position of mouse.
		*/
		void MouseDragging(const Point2D& mousePt);

		/** 
		* @brief Process mouse right button down events. 
		* @param mousePt Screen position of mouse.
		* @param popupMenu Popup menu to populate.
		* @return True if view is processing the event, else false.
		*/
		bool MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu);

	private:
		/** serialization. */
		friend class boost::serialization::access;
		template<class archive>
		void serialize(archive & ar, const unsigned int version);

	private:
		/** Unique id. */
		uint m_id;

		/** Text to display with location. */
		std::wstring m_text;

		/** Label position. */
		std::wstring m_labelPosition;

		/** Label style. */
		std::wstring m_labelStyle;

		/** Screen position of label. */
		Point3D m_screenPos;

		/** Grid position of label. */
		Point3D m_gridPos;

		/** Flag indicating if label should be rotated onto the plane. */
		bool m_bRotateX; 

		/** Flag indicating if label should be rotated about the z-axis. */
		bool m_bRotateZ;

		/** Rotation of text around z-axis. */
		float m_zRotation;

		/** Previous position of mouse. Used to allow labels to be interactively moved. */
		Point2D m_mousePrevPt;

		/** Rendering style for label. */
		LABEL_RENDERING_STYLE m_renderingStyle;

		/** Flag indicating if label is currently being dragged. */
		bool m_bDragging;

		/** Flag indicating if the grid position has been set for this label. */
		bool m_bGridPosSet;
	};

}

#endif
