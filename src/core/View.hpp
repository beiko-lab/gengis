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

#ifndef _GENGIS_VIEW_
#define _GENGIS_VIEW_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/**
	* @brief Abstract base class for all visible objects.
	*/
	class View 
	{
	public:
		explicit View(): m_bVisible(true), m_bSelected(false), m_depth(0) {}

		virtual ~View();

		/** Render view. */
		virtual void Render() = 0;

		/** Set the visibility of view. */
		virtual void SetVisibility( bool visible )
		{
			m_bVisible = visible;
		}

		/** Set the visibility of view. */
		virtual bool IsVisible() const
		{
			return m_bVisible;
		}

		/** Set flag indicating if view is selected or not. */
		virtual void SetSelected(bool state)
		{
			m_bSelected = state;
		}

		/** Determine whether view is currently selected. */
		virtual bool IsSelected()
		{
			return m_bSelected;
		}

		/** Set depth of object. Used to specify layering order in orthographic view. */
		virtual void SetDepth(float depth)
		{
			m_depth = depth;
		}

		/** Get depth of object. */
		virtual float GetDepth() const
		{
			return m_depth;
		}

		/** Show properties dialog for this view. */
		virtual void ShowPropertiesDlg() {}

		/** 
		* @brief Process mouse left button down events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		virtual bool MouseLeftDown(const Point2D& mousePt)
		{
			return false;
		}

		/** 
		* @brief Process mouse left button up events. 
		* @param mousePt Screen position of mouse.
		*/
		virtual void MouseLeftUp(const Point2D& mousePt)
		{
			return;
		}

		/** 
		* @brief Process mouse left button float click events. 
		* @param mousePt Screen position of mouse.
		*/
		virtual void MouseLeftDblClick(const Point2D& mousePt)
		{
			return;
		}

		/** 
		* @brief Process mouse right button down events. 
		* @param mousePt Screen position of mouse.
		* @param popupMenu Popup menu to populate.
		* @return True if view is processing the event, else false.
		*/
		//bool MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu);

		/** 
		* @brief Process mouse dragging events. 
		* @param mousePt Screen position of mouse.
		*/
		virtual void MouseDragging(const Point2D& mousePt)
		{
			return;
		}

		/** 
		* @brief Process mouse moving events. 
		* @param mousePt Screen position of mouse.
		* @return True if the OpenGL viewport should be refreshed.
		*/
		virtual bool MouseMove(const Point2D& mousePt)
		{
			return false;
		}

		/** 
		* @brief Process keyboard events. 
		* @param mousePt Screen position of mouse.
		*/
		virtual void KeyboardEvent(wxKeyEvent& event)
		{
			return;
		}

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Perform addition rendering so view appears selected. */
		virtual void RenderSelect() {}

	protected:
		/** Flag indicating if object should be visible or not. */
		bool m_bVisible;

		/** Depth of object. Used to specify layering order in orthographic view. */
		float m_depth;

		/** Flag indicating if object is currently selected. */
		bool m_bSelected;
	};
}

#endif
