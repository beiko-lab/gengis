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

#ifndef _GENGIS_CHART_VIEW_
#define _GENGIS_CHART_VIEW_

#include "../core/Precompiled.hpp"

#include "../core/LocationModel.hpp"
#include "../core/LocationView.hpp"
#include "../core/View.hpp"

#include "../utils/Colour.hpp"
#include "../utils/ColourMapDiscrete.hpp"

class GenGIS::ChartView;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::ChartView * t, const unsigned int file_version);
	}
}

namespace GenGIS
{
	typedef struct ChartProp
	{
		float width;
		float height;

		float borderSize;
		bool bBottomBorder;
		bool bLeftBorder;
		bool bTopBorder;
		bool bRightBorder;

		Colour backgroundColour;
		Colour borderColour;
		bool bAssignBorderLocationColour;

		Point3D centre;
		float depth;

		float cameraPitch;
		float cameraYaw;
		bool bPlot3D;

		ColourMapDiscretePtr colourMap;

		bool bCategoryBorders;

		float assignToOther;

		std::map<std::wstring, float> elementPercentage;

		GLUquadricObj* chartQuadric;
	} ChartProp;

	/**
	* @brief Render sequence data as a chart in an OpenGL viewport.
	*/
	class ChartView : public View
	{
	public:
		/** Types of charts. */
		enum CHART_TYPE { PIE_CHART_2D, PIE_CHART_3D, BAR_CHART_2D, BAR_CHART_3D };

	public:
		/** 
		* @brief Constuctor.
		* @param locationModel Location model associated with this chart view.
		* @param locationView Location view associated with this chart view.
		* @param colourMap Colour map to be used by pie charts.
		*/
		explicit ChartView(LocationModelPtr locationModel, LocationViewPtr locationView, ColourMapDiscretePtr colourMap);

		/** Destructor. */
		~ChartView()
		{
			gluDeleteQuadric(m_chartQuadric);
		}

		/** Render chart. */
		void Render();

		/** Determine if object is visible. */
		bool IsVisible();

		/** Set chart width (diameter). */
		void SetWidth(float width) { m_width = width; }

		/** Get width (diameter) of chart. */
		float GetWidth() const { return m_width; }

		/** Set chart height. */
		void SetHeight(float height) { m_height = height; }

		/** Get height of chart. */
		float GetHeight() const { return m_height; }

		/** Get adjusted chart size based on # of sequences. */
		float GetAdjustedSize() const;

		/** Set normalized relative number of sequences (i.e., map [min. seq., max. seq.] to [0, 1])  */
		void SetNormalizedNumSeq(float percentage) { m_normNumSeq = percentage; }

		/** Set chart border size. */
		void SetBorderSize(float size) { m_borderSize = size; }

		/** Get size of chart border. */
		float GetBorderSize() const { return m_borderSize; }

		/** Set visibility of wedge borders. */
		void SetWedgeBorders(bool state) { m_bWedgeBorders = state; }

		/** Get visibility of wedge borders. */
		bool GetWedgeBorders() const { return m_bWedgeBorders; }

		/** Set flag indicating if size should be adjusted to reflect # of sequences. */
		void SetSizeProportionalToSeq(bool state) { m_bSizeProportionalToSeq = state; }

		/** Get flag indicating if size should be adjusted to reflect # of sequences. */
		bool GetSizeProportionalToSeq() const { return m_bSizeProportionalToSeq; }

		/** Set minimum size of a chart when being set proportionally. */
		void SetMinSize(uint minSize) { m_minSize = minSize; }

		/** Get minimum size of a chart when being set proportionally. */
		uint GetMinSize() const { return m_minSize; }

		/** Set maximum size of a chart when being set proportionally. */
		void SetMaxSize(uint maxSize) { m_maxSize = maxSize; }

		/** Get maximum size of a chart when being set proportionally. */
		uint GetMaxSize() const { return m_maxSize; }

		/** Set border visibility. */
		void SetBottomBorder(bool state) { m_bBottomBorder = state; }
		void SetLeftBorder(bool state) { m_bLeftBorder = state; }
		void SetTopBorder(bool state) { m_bTopBorder = state; }
		void SetRightBorder(bool state) { m_bRightBorder = state; }

		/** Get border visibility. */
		bool IsBottomBorder() { return m_bBottomBorder; }
		bool IsLeftBorder() { return m_bLeftBorder; }
		bool IsTopBorder() { return m_bTopBorder; }
		bool IsRightBorder() { return m_bRightBorder; }

		/** Set background colour of chart.*/
		void SetBackgroundColour(const Colour& colour) { m_backgroundColour = colour; }

		/** Get background colour of chart. */
		Colour GetBackgroundColour() const { return m_backgroundColour; }

		/** Set border colour of chart.*/
		void SetBorderColour(const Colour& colour) { m_borderColour = colour; }

		/** Get border colour of chart. */
		Colour GetBorderColour() const { return m_borderColour; }

		/** Set border colour based on associated location marker.*/
		void SetAssignBorderLocationColour(bool state) { m_assignBorderLocationColour = state; }

		/** Check if border colour is based on associated location marker.*/
		bool IsAssignBorderLocationColour() const { return m_assignBorderLocationColour; }

		/** Set field to create chart from. */
		void SetField(const std::wstring& field) { m_field = field; }

		/** Get field chart is created from. */
		std::wstring GetField() const { return m_field; }

		/** Set quantitative field to create chart from. */
		void SetQuantitativeField(const std::wstring& quantitativeField) { m_quantitativeField = quantitativeField; }

		/** Get quantitative field chart is created from. */
		std::wstring GetQuantitativeField() const { return m_quantitativeField; }

		/** Set flag indicating if charts should use quantitative information. */
		void SetQuantitative(bool bQuantitative) { m_bQuantitative = bQuantitative; }

		/** Get flag indicating if charts should use quantitative information. */
		bool GetQuantitative() const { return m_bQuantitative; }

		/** Get names of all wedges. */
		std::vector<std::wstring> GetNames() const;

		/** Get percentage of chart wedge. */
		float GetPercentage(const std::wstring& name) const;

		/** Set colour map to associated with chart. */
		void SetColourMap(ColourMapDiscretePtr colourMap) { m_colourMap = colourMap; }

		/** Get colour map associated with chart. */
		ColourMapDiscretePtr GetColourMap() const { return m_colourMap; }

		/** Set the id of the location layer associated with this view. */
		virtual void SetLocationLayerId(uint locationLayerId) { m_locationLayerId = locationLayerId; }

		/** Get the id of the location layer associated with this view. */
		virtual uint GetLocationLayerId() const { return m_locationLayerId; }

		/** Get location view (for serialization). */
		LocationViewPtr GetLocationView() const { return m_locationView; }

		/** Get location model (for serialization). */
		LocationModelPtr GetLocationModel() const { return m_locationModel; }

		/** Set chart type. */
		void SetChartType(CHART_TYPE type) { m_chartType = type; }

		/** Get chart type. */
		CHART_TYPE GetChartType() const { return m_chartType; }

		/** Set assign to other percentage. */
		void SetAssignToOther(float percentage) { m_assignToOther = percentage; }

		/** Get assign to other percentage. */
		float GetAssignToOther() const { return m_assignToOther; }

		/** Get screen position of chart. */
		Point3D GetScreenPos() const;

		/** Set screen position of chart. */
		void SetScreenPos(const Point3D& pos);

		/** Set grid position of chart. */
		void SetCentre(const Point3D& pt);

		/** Get grid position of chart. */
		Point3D GetCentre() const { return m_centre; }

		/** Get position of location site associate with chart. */
		Point3D GetLocationPos() const;

		/** Get droplines. */
		DropLinesPtr GetDropLines() const { return m_dropLines; }

		/** Set label. */
		void SetLabel(VisualLabelPtr label) { m_label = label; }

		/** Get label. */
		VisualLabelPtr GetLabel() const { return m_label; }

		/** Set label bind to chart flag. */
		void SetLabelBindToChart(bool state) { m_bLabelBindToChart = state; }

		/** Get label bind to chart flag. */
		bool GetLabelBindToChart() const { return m_bLabelBindToChart; }

		/** Get all field values for sequences at the location associated with this pie chart. */
		void GetValues(const std::wstring& field, std::set<std::wstring>& values) const;

		/** Update chart to reflect any changes in the underlying data (e.g., a change in the set of active sequences). */
		bool UpdateChart(const std::wstring& field, const std::set<std::wstring>& values);

		/** Get number of active sequences. */
		float GetNumActiveSequences() const { return m_numActiveSeq; }

		/** Show properties dialog for this view. */
		void ShowPropertiesDlg();

		/** 
		* @brief Process mouse left button down events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		bool MouseLeftDown(const Point2D& mousePt);

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
		bool MouseMove(const Point2D& mousePt) { return true; }

		/** 
		* @brief Process keyboard events. 
		* @param mousePt Screen position of mouse.
		*/
		void KeyboardEvent(wxKeyEvent& event);

	protected:
		/** Render location label. */
		void RenderLabel();

		/** Check if current position of mouse overlays with chart. */
		bool IsClicked(const Point2D& mousePt);

		/** Get 2D bounding box for chart. */
		Box2D GetBoundingBox() const;

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const ChartView * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Location model associated with this chart. */
		LocationModelPtr m_locationModel;

		/** Location view associated with this chart. */
		LocationViewPtr m_locationView;

		/** Colour map associated with chart. */
		ColourMapDiscretePtr m_colourMap;

		/** Id of location layer associated with this view. */
		uint m_locationLayerId;

		/** Used to draw chart. */
		GLUquadricObj* m_chartQuadric;

		/** Width (diameter) of chart. */
		float m_width;

		/** Height of chart. */
		float m_height;

		/** Size of chart border. */
		float m_borderSize;

		/** Flag indicating if wedge borders should be rendered. */
		bool m_bWedgeBorders;

		/** Background colour of chart. */
		Colour m_backgroundColour;

		/** Border colour of chart. */
		Colour m_borderColour;

		/** Set border colour based on associated location marker. */
		bool m_assignBorderLocationColour;

		/** Type of chart. */
		CHART_TYPE m_chartType; 

		/** Field to create chart from. */
		std::wstring m_field;

		/** Field indicating quantitative information for a sequence. */
		std::wstring m_quantitativeField;

		/** Flag indicating if charts should use quantitative information. */
		bool m_bQuantitative; 

		/** Flag indicating if size should be adjusted to reflect # of sequences. */
		bool m_bSizeProportionalToSeq;

		/** Normalized relative number of sequences. */
		float m_normNumSeq;

		/** Percentage of each wedge in the chart. */
		std::map<std::wstring, float> m_elementPercentage;

		/** Centre of chart. */
		Point3D m_centre;

		/** Dropline from chart to geographic location. */
		DropLinesPtr m_dropLines;

		/** Threshold used to assign taxa to the 'other' category. */
		float m_assignToOther;

		/** Store previous mouse position. */
		Point2D m_mousePrevPt;

		/** Minimum size of chart. */
		uint m_minSize;

		/** Maximum size of chart. */
		uint m_maxSize;

		/** Label for location. */
		VisualLabelPtr m_label;

		/** Flag indicating if label is bound to chart. */
		bool m_bLabelBindToChart;

		/** Number of active sequences. */
		float m_numActiveSeq;

		/** Draw select border elements. */
		bool m_bBottomBorder;
		bool m_bLeftBorder;
		bool m_bTopBorder;
		bool m_bRightBorder;
	};

}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const ChartView * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//ar << t->GetLocationModel(); // m_locationModel (LocationModelPtr, LocationModel class)
			//ar << t->GetLocationView();  // m_locationView (LocationViewPtr, LocationView class)
			//ar << t->GetColourMap();     // m_colourMap (ColourMapDiscretePtr, ColourMapDiscrete class)

			ar << t->m_locationModel; // LocationModelPtr (LocationModel class)
			ar << t->m_locationView;  // LocationViewPtr (LocationView class)
			ar << t->m_colourMap;     // ColourMapDiscretePtr (ColourMapDiscrete class)
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, ChartView * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance

			LocationModelPtr _locationModel;
			ar >> _locationModel;

			LocationViewPtr _locationView;
			ar >> _locationView;

			ColourMapDiscretePtr _colourMap;
			ar >> _colourMap;

			::new(t)ChartView(_locationModel, _locationView, _colourMap);
		}
	}
}

#endif
