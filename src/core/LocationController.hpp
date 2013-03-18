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

#ifndef _GENGIS_LOCATION_CONTROLLER
#define _GENGIS_LOCATION_CONTROLLER

#include "../core/Precompiled.hpp"

#include "../core/LocationModel.hpp"
#include "../core/LocationView.hpp"
#include "../core/ChartView.hpp"

class GenGIS::LocationController;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::LocationController * t, const unsigned int file_version);
	}
}

namespace GenGIS
{

	/**
	* @brief Handle events related to a given location.
	*/
	class LocationController 
	{
	public:
		/** Types of visual objects (views) which can be selected. */
		enum SELECTED_OBJ { LOCATION_MARKER, CHART };

	public:
		/** 
		* @brief Constuctor.
		* @param locationModel Location model to associate with this controller.
		* @param locationView Location view to associate with this controller.
		* @param chartView Pie chart view to associate with this controller.
		*/
		explicit LocationController(LocationModelPtr locationModel, LocationViewPtr locationView, ChartViewPtr chartView) :
			m_locationModel(locationModel), m_locationView(locationView), m_chartView(chartView), m_bRenderChart(true) {}

		/** Destructor. */
		~LocationController() {}

		/** Get id of location site. */
		std::wstring GetId() const
		{
			return m_locationModel->GetId();
		}

		/** Get northing position of location. */
		float GetNorthing() const
		{
			return m_locationModel->GetNorthing();
		}

		/** Get easting position of location. */
		float GetEasting() const
		{
			return m_locationModel->GetEasting();
		}

		/** Get projected coordinates of location. */
		GeoCoord GetProjection() const
		{
			return m_locationModel->GetProjection();
		}

		/** Render location model. */
		void Render();

		/**
		* @brief Add sequence to location.
		* @param sequence Sequence to add.
		*/
		void AddSequence(SequenceModelPtr sequence)
		{
			m_locationModel->AddSequence(sequence);
		}

		/** Get number of sequence at this location. */
		unsigned int GetNumSequence() const
		{
			return m_locationModel->GetNumSequence();
		}

		/**
		* @brief Get sequence from location.
		* @param index Index of desired sequence.
		* @return Sample at given index.
		*/
		SequenceModelPtr GetSequence(unsigned int index) const
		{
			return m_locationModel->GetSequence(index);
		}

		/**
		* @brief Remove sequence from location.
		* @param index Sequence to remove.
		*/
		void RemoveSequence(unsigned int index)
		{
			m_locationModel->RemoveSequence(index);
		}

		/** Remove all sequence from location. */
		void RemoveAllSequence()
		{
			m_locationModel->RemoveAllSequence();
		}

		/** Get location model (for serialization). */
		LocationModelPtr GetLocationModel() const
		{
			return m_locationModel;
		}

		/** Get 3D location marker associated with this controller. */
		LocationViewPtr GetLocationView() const
		{
			return m_locationView;
		}

		/** Get chart view associated with this controller. */
		ChartViewPtr GetChartView() const
		{
			return m_chartView;
		}

		/** Get data associate with location site. */
		std::map<std::wstring,std::wstring> GetData() const
		{
			return m_locationModel->GetData();
		}

		/** 
		* @brief Add metadata to location.
		* @param field New field.
		* @param value Value of field.
		*/
		void AddData(const std::wstring& field, const std::wstring& value)
		{
			m_locationModel->AddData(field, value);
		}

		/** Set flag indicating if chart should be rendered. */
		void SetRenderChart(bool bRender)
		{
			m_bRenderChart = bRender;
		}

		/** Get flag indicating if chart will be rendered. */
		bool GetRenderChart() const
		{
			return m_bRenderChart;
		}

		/** Get colour of location marker. */
		Colour GetColour() const
		{
			return m_locationView->GetColour();
		}

		/** Set colour of location. */
		void SetColour(const Colour& colour)
		{
			m_locationView->SetColour(colour);
		}

		/** Get size of location marker. */
		float GetSize() const
		{
			return m_locationView->GetSize();
		}

		/** Set size of location. */
		void SetSize(float size)
		{
			m_locationView->SetSize(size);
		}

		/** Set border size. */
		void SetBorderSize(uint size)
		{
			m_locationView->SetBorderSize(size);
		}

		/** Get border size. */
		uint GetBorderSize() const
		{
			return m_locationView->GetBorderSize();
		}

		/** Get rendering order. */
		float GetRenderingOrder() const
		{
			return m_locationView->GetRenderingOrder();
		}

		/** Set rendering order. */
		void SetRenderingOrder(float renderingOrder)
		{
			m_locationView->SetRenderingOrder(renderingOrder);
		}

		/** Get shape of location marker. */
		VisualMarker::MARKER_SHAPE GetShape() const
		{
			return m_locationView->GetShape();
		}

		/** Set location marker shape. */
		void SetShape(VisualMarker::MARKER_SHAPE shape)
		{
			m_locationView->SetShape(shape);
		}

		/** Set label. */
		void SetLabel(VisualLabelPtr label)
		{
			m_locationView->SetLabel(label);
		}

		/** Get label. */
		const VisualLabelPtr GetLabel() const
		{
			return m_locationView->GetLabel();
		}

		/** Get grid position of location marker. */
		Point3D GetPosition() const;

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state)
		{
			m_locationModel->SetActive(state);
		}

		/** Get active state of layer. */
		bool IsActive() const
		{
			return m_locationModel->IsActive();
		}

		/** Toggle active state of layer. */
		void ToggleActive()
		{
			m_locationModel->ToggleActive();
		}

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const LocationController * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Location model associated with this controller. */
		LocationModelPtr m_locationModel;

		/** Location view associated with this controller. */
		LocationViewPtr m_locationView;

		/** Pie chart view associated with this controller. */
		ChartViewPtr m_chartView;

		/** Flag indicating if the chart associated with this location should be rendered. */
		bool m_bRenderChart;
	};
	
}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const LocationController * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//ar << t->GetLocationModel(); // m_locationModel (LocationModelPtr)
			//ar << t->GetLocationView();  // m_locationView (LocationViewPtr)
			//ar << t->GetChartView();     // m_chartView (ChartViewPtr)

			ar << t->m_locationModel; // LocationModelPtr
			ar << t->m_locationView;  // LocationViewPtr
			ar << t->m_chartView;     // ChartViewPtr
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, LocationController * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			LocationModelPtr _locationModel;
			ar >> _locationModel;

			LocationViewPtr _locationView;
			ar >> _locationView;

			ChartViewPtr _chartView;
			ar >> _chartView;

			::new(t)LocationController(_locationModel, _locationView, _chartView);
		}
	}
}

#endif
