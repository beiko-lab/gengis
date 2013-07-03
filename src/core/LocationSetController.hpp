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
#ifndef _GENGIS_LOCATION_SET_CONTROLLER_
#define _GENGIS_LOCATION_SET_LAYER_

#include "../core/Precompiled.hpp"

#include "../core/VisualMarker.hpp"

namespace GenGIS
{
	/**
	* @brief A location set controller allows visual properties to be set for an entire set of locations.
	*/
	class LocationSetController
	{
	public:
		/** Constructor. */
		explicit LocationSetController();

		/** Destructor. */
		~LocationSetController() {}

		/** Set location set layers associated with this controller. */
		void SetLocationSetLayers(const std::vector<LocationLayerPtr>& locationLayers)
		{
			m_locationLayers = locationLayers;
		}

		/** Set visibility of all locations. */
		void SetLocationVisibilities();

		/** Set colour of all locations. */
		void SetLocationColours();

		/** Set border properties of all locations. */
		void SetLocationBorders();

		/** Find minimum and maximum location sizes. */
		void FindMinMaxLocationSizes( double& min, double& max );

		/** Set size of all locations. */
		void SetLocationSizes();

		/** Set shape of all locations. */
		void SetLocationShapes();

		/** Set visual properties of all location labels. */
		void SetLocationLabels(bool bUpdateSize, bool bUpdatePosition, bool bUpdateStyle, bool bUpdateColour, bool bUpdateVisibility);

		/** Set field used to determine location colour. */
		void SetColourField(const std::wstring& field)
		{
			m_colourField = field;
		}

		/** Get field used to determine location colour. */
		std::wstring GetColourField() const
		{
			return m_colourField;
		}

		/** Set colour map associated with location set. */
		void SetColourMap(ColourMapDiscretePtr colourMap)
		{
			m_colourMap = colourMap;
		}

		/** Get colour map associated with location set. */
		ColourMapDiscretePtr GetColourMap() const
		{
			return m_colourMap;
		}

		/** Set flag indicating if all locations should have the same colour. */
		void SetUniformColourFlag(bool state)
		{
			m_bUniformColour = state;
		}

		/** Get flag indicating if all locations should have the same colour. */
		bool GetUniformColourFlag() const
		{
			return m_bUniformColour;
		}

		/** Set uniform colour. */
		void SetUniformColour(Colour colour)
		{
			m_uniformColour = colour;
		}

		/** Get uniform colour. */
		Colour GetUniformColour() const
		{
			return m_uniformColour;
		}

		/** Set border size. */
		void SetBorderSize(uint size)
		{
			m_borderSize = size;
		}

		/** Get border size. */
		uint GetBorderSize() const
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

		/** Set visibility of location set. */
		void SetLocationSetVisibility(bool state)
		{
			m_bLocationSetVisibility = state;
		}

		/** Get visibility of location set. */
		bool GetLocationSetVisibility() const
		{
			return m_bLocationSetVisibility;
		}

		/** Set field used to determine location shape. */
		void SetShapeField(const std::wstring& field)
		{
			m_shapeField = field;
		}

		/** Get field used to determine location shape. */
		std::wstring GetShapeField() const
		{
			return m_shapeField;
		}

		/** Set shape map associated with location set. */
		void SetShapeMap(ShapeMapPtr shapeMap)
		{
			m_shapeMap = shapeMap;
		}

		/** Get shape map associated with location set. */
		ShapeMapPtr GetShapeMap() const
		{
			return m_shapeMap;
		}

		/** Set flag indicating if all locations should have the same shape. */
		void SetUniformShapeFlag(bool state)
		{
			m_bUniformShape = state;
		}

		/** Get flag indicating if all locations should have the same shape. */
		bool GetUniformShapeFlag() const
		{
			return m_bUniformShape;
		}

		/** Set uniform shape. */
		void SetUniformShape(VisualMarker::MARKER_SHAPE shape)
		{
			m_uniformShape = shape;
		}

		/** Get uniform shape. */
		VisualMarker::MARKER_SHAPE GetUniformShape() const
		{
			return m_uniformShape;
		}

		/** Set field used to determine location size. */
		void SetSizeField(const std::wstring& field)
		{
			m_sizeField = field;
		}

		/** Set field used to determine location size. */
		std::wstring GetSizeField() const
		{
			return m_sizeField;
		}

		/** Set min location size. */
		void SetMinLocationSize(double size)
		{
			m_minLocationSize = size;
		}

		/** Get min location size. */
		double GetMinLocationSize() const
		{
			return m_minLocationSize;
		}

		/** Set max location size. */
		void SetMaxLocationSize(double size)
		{
			m_maxLocationSize = size;
		}

		/** Get max location size. */
		double GetMaxLocationSize() const
		{
			return m_maxLocationSize;
		}

		/** Set field used to determine location label. */
		void SetLabelField(const std::wstring& field)
		{
			m_labelField = field;
		}

		/** Get field used to determine location label. */
		std::wstring GetLabelField() const
		{
			return m_labelField;
		}

		/** Set label colour. */
		void SetLabelColour(Colour colour)
		{
			m_labelColour = colour;
		}

		/** Get label colour. */
		Colour GetLabelColour() const
		{
			return m_labelColour;
		}

		/** Set label size. */
		void SetLabelSize(uint size)
		{
			m_labelSize = size;
		}

		/** Get label size. */
		uint GetLabelSize() const
		{
			return m_labelSize;
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
		void SetLabelStyle(const std::wstring& style)
		{
			m_labelStyle = style;
		}

		/** Get label style. */
		std::wstring GetLabelStyle() const
		{
			return m_labelStyle;
		}

		/** Set label visibility. */
		void SetLabelVisibility(bool state)
		{
			m_bLabelVisible = state;
		}

		/** Get label visibility. */
		bool GetLabelVisibility() const
		{
			return m_bLabelVisible;
		}

		/** Set flag indicating if label is bound to chart. */
		void SetLabelBindToChart(bool state)
		{
			m_bLabelBindToChart = state;
		}

		/** Get flag indicating if label is bound to chart. */
		bool GetLabelBindToChart() const
		{
			return m_bLabelBindToChart;
		}

		/** Get name of all active locations. */
		std::vector<std::wstring> GetActiveLocations() const;

		/** Get name of all active sequences. */
		std::vector<std::wstring> GetActiveSequences() const;

		/** Get fields for location metadata. */
		std::vector<std::wstring> GetMetadataFields() const;

		/** Get all unique field values within the given field */
		void GetSortedFieldValues(const std::wstring& field, std::vector<std::wstring>& fieldValues);

		/** Get numeric fields for location metadata. */
		std::vector<std::wstring> GetNumericMetadataFields(bool bOnlyActiveLocs = false) const;

		/** Get location data for a specified field. */ 
		std::vector<std::wstring> GetMetadata(const std::wstring& field, bool bOnlyActiveLocs = false) const;

		/** Get numeric location data for a specified field. */ 
		std::vector<float> GetNumericMetadata(const std::wstring& field, bool bOnlyActiveLocs = false) const;

		/** Add field to location metadata. */ 
		void AddMetadata(const std::wstring& field, const std::vector<std::wstring>& data) const;

		/** Get map which indicates metadata associates with sequences. */
		std::map<std::wstring,std::wstring> GetSequenceMetadata() const;

		/** Function to handle incoming location set data from plugins. */
		void GetLocationSetFromPython(const std::wstring& field) const;

		/** Check if any locations have changed their shape. */
		bool ModifiedShape();

		/** Check if any locations have changed their size. */
		bool ModifiedSize();

		/** Check if any locations have changed their colour. */
		bool ModifiedColour();

		/** Check if any locations have changed their visibility. */
		bool ModifiedVisibility();

		/** Check if any location labels have changed their size. */
		bool ModifiedLabelSize();

		/** Check if any location labels have changed their position. */
		bool ModifiedLabelPosition();

		/** Check if any location labels have changed their style. */
		bool ModifiedLabelStyle();

		/** Check if any location labels have changed their colour. */
		bool ModifiedLabelColour();

		/** Check if any location labels have changed their visibility. */
		bool ModifiedLabelVisibility();

		/** Check if any location labels have changed their bind to chart property. */
		bool ModifiedLabelBindToChart();

		/** Get number of location layers beneath this location set layer. */
		uint GetNumLocationLayers() const
		{
			return m_locationLayers.size();
		}

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state)
		{
			m_bActive = state;
		}

		/** Get active state of layer. */
		bool IsActive() const
		{
			return m_bActive;
		}

		/** Toggle active state of layer. */
		void ToggleActive()
		{
			m_bActive = !m_bActive;
		}

		void RemoveAllLocationLayers()
		{
			m_locationLayers.clear();
		}

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Flag indicating if location set is active. */
		bool m_bActive;

		/** Location layers beneath this location set layer. */
		std::vector<LocationLayerPtr> m_locationLayers;

		/** Field used to determine location colour. */
		std::wstring m_colourField;

		/** Colour map associated with location set. */
		ColourMapDiscretePtr m_colourMap;

		/** Flag indicating if all locations should have the same colour. */
		bool m_bUniformColour;

		/** Uniform colour to assign to locations. */
		Colour m_uniformColour;

		/** Visibility of location set. */
		bool m_bLocationSetVisibility;

		/** Field used to determine location shape. */
		std::wstring m_shapeField;

		/** Shape map associated with location set. */
		ShapeMapPtr m_shapeMap;

		/** Flag indicating if all locations should have the same shape. */
		bool m_bUniformShape;

		/** Uniform shape to assign to locations. */
		VisualMarker::MARKER_SHAPE m_uniformShape;

		/** Field used to determine location size. */
		std::wstring m_sizeField;

		/** Min location size. */
		double m_minLocationSize;

		/** Max location size. */
		double m_maxLocationSize;

		/** Field used to determine location label. */
		std::wstring m_labelField;

		/** Label colour. */
		Colour m_labelColour;

		/** Label size. */
		uint m_labelSize;

		/** Label position. */
		std::wstring m_labelPosition;

		/** Label style. */
		std::wstring m_labelStyle;

		/** Label visibility. */
		bool m_bLabelVisible;

		/** Flag indicating if label is bound to chart. */
		bool m_bLabelBindToChart;

		/** Border size. */
		uint m_borderSize;

		/** Border colour. */
		Colour m_borderColour;
	};

}

#endif
