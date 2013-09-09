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

#ifndef _GENGIS_CHART_SET_VIEW_
#define _GENGIS_CHART_SET_VIEW_

#include "../core/Precompiled.hpp"

#include "../utils/Point3D.hpp"
#include "../core/View.hpp"

namespace GenGIS
{
	/**
	* @class ChartSetView
	* @brief Collection of chart views. This class is responsible for the layout of these charts.
	*/
	class ChartSetView : public View
	{
	public:
		/** Desired layout of charts. */
		enum LAYOUT_CHART { NONE, LAYOUT_LINE_EVEN_SPACING, LAYOUT_LINE_PROJECTED, ELLIPSE_CLOSEST_PT, ELLIPSE_EVEN_SPACING, ELLIPSE_QUADRANT, DEFAULT, LAYOUT_TREE_LEAVES };

		/** Currently selected visual element. */
		enum SELECTED_ELEMENT { NO_SELECTED_ELEMENT, LAYOUT_LINE, LAYOUT_ELLIPSE, CHART };

	public:
		/** Constructor. */
		explicit ChartSetView(): m_selectedElement(NO_SELECTED_ELEMENT), m_bLayoutPrimativeMoved(false) {}

		/** Destructor. */
		~ChartSetView() {}

		/** Render chart set layout primative. */
		void Render();

		/** 
		* @brief Add chart view to this controller. 
		* @param chartView Smart pointer to chart view to add to controller.
		*/
		void AddChart(ChartViewPtr chartView) { m_chartViews.push_back(chartView); }

		unsigned int GetSize(){return (unsigned int)m_chartViews.size();}
		/** 
		* @brief Get chart associated with this controller. 
		* @param index Index of chart of interest.
		*/
		ChartViewPtr GetChart(uint index) const { return m_chartViews.at(index); }

		/** 
		* @brief Clear all charts associated with this controller.
		* @param none.
		*/
		void ClearAllCharts() { m_chartViews.clear(); }

		/** 
		* @brief Set layout line used to layout charts. 
		* @param layoutLine Layout line to associate with chart set.
		*/
		void SetLayoutLine(LayoutLinePtr layoutLine);

		/** Check if a layout line has been assigned to the tree. */
		bool IsLayoutLine() const { return m_layoutLine; }

		/** Get visibility of layout line. */
		bool GetLayoutLineVisibility() const;

		/** 
		* @brief Set visibility of layout line. 
		* @param state Desired visibility state for layout line.
		*/
		void SetLayoutLineVisibility(bool state);

		/** 
		* @brief Set layout ellipse used to layout charts.
		* @param ellipse Layout ellipse to associate with chart set.
		*/
		void SetLayoutEllipse(LayoutEllipsePtr ellipse);

		/** Check if a layout ellipse has been assigned to the tree. */
		bool IsLayoutEllipse() const { return m_layoutEllipse; }

		/** Get visibility of layout ellipse. */
		bool GetLayoutEllipseVisibility() const;

		/** 
		* @brief Set visibility of layout ellipse. 
		* @param state Desired visibility state for layout ellipse.
		*/
		void SetLayoutEllipseVisibility(bool state);

		/** 
		* @brief Calculate grid positions of all nodes in tree for a given layout style. 
		* @param layout Desired layout style.
		*/
		void SetLayout(LAYOUT_CHART layout);

		/** Get current layout style. */
		LAYOUT_CHART GetLayout() const { return m_layout; }

		/** Show properties dialog for this view. */
		void ShowPropertiesDlg();

		/** Set the visibility of the currently select component of a view. */
		void SetSelectionVisiblity( bool visible );

		/** Update all charts to reflect any changes in properties. */
		void UpdateCharts(const std::wstring& field = _T(""));

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
		* @brief Process mouse left button float click events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		void MouseLeftDblClick(const Point2D& mousePt);

		/** 
		* @brief Process mouse right button down events. 
		* @param mousePt Screen position of mouse.
		* @param popupMenu Popup menu to populate.
		* @return True if view is processing the event, else false.
		*/
		bool MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu);

		/** 
		* @brief Process mouse dragging events. 
		* @param mousePt Screen position of mouse.
		*/
		void MouseDragging(const Point2D& mousePt);

		/** 
		* @brief Process mouse moving events. 
		* @param mousePt Screen position of mouse.
		* @return True if the OpenGL viewport should be refreshed.
		*/
		bool MouseMove(const Point2D& mousePt);

		/** 
		* @brief Process keyboard events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		void KeyboardEvent(wxKeyEvent& event);

	protected:
		/** Layout charts directly above locations (default placement). */
		void DefaultPosition();

		/** Layout charts directly above the leaves of a tree. */
		void LayoutTreeLeaves();

		/** Layout charts evenly spaced along layout line. */
		void LayoutLineEvenlySpaced();

		/** Project charts to layout line.*/
		void LayoutLineProjected();

		/** Layout charts indepdently in each quadrant of an ellipse. */
		void LayoutEllipseQuadrant();

		/** Layout charts evenly spaced along layout ellipse. */
		void LayoutEllipseEvenlySpaced();

		/** Project charts to closest point of layout ellipse. */
		void LayoutEllipseClosestPt();

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	private:
		/** Pointers to all chart views associated with this controller. */
		std::vector<ChartViewPtr> m_chartViews;

		/** Layout line used to layout charts. */
		LayoutLinePtr m_layoutLine;

		/** Layout ellipse used to layout charts. */
		LayoutEllipsePtr m_layoutEllipse;

		/** Desired layout of charts. */
		LAYOUT_CHART m_layout;

		/** Flag indicating if the layout line has moved. */
		bool m_bLayoutPrimativeMoved;

		/** Track the currently selected element. */
		SELECTED_ELEMENT m_selectedElement;
	};
}

#endif

