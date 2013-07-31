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

#include "../core/Precompiled.hpp"

#include "../core/LocationSetController.hpp"
#include "../core/App.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/SequenceController.hpp"
#include "../core/LocationController.hpp"
#include "../core/VisualLabel.hpp"

#include "../utils/ColourMapManager.hpp"
#include "../utils/ColourMap.hpp"
#include "../utils/ColourMapDiscrete.hpp"
#include "../utils/ShapeMapManager.hpp"
#include "../utils/ShapeMap.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/Log.hpp"

using namespace GenGIS;

LocationSetController::LocationSetController() :
	m_bLocationSetVisibility(true), m_bUniformColour(true), m_uniformColour(Colour(217, 95, 2)),
	m_minLocationSize(6), m_maxLocationSize(6), m_bUniformShape(true), m_uniformShape(VisualMarker::CIRCLE_FAST),
	m_labelColour(Colour(255,255,255)), m_labelSize(12), m_bLabelVisible(false), m_labelPosition(_T("Right")),
	m_labelStyle(_T("Dynamic")),m_bLabelBindToChart(false), m_borderSize(1), m_borderColour(Colour(0,0,0)), m_bActive(true)
{
	ColourMapManagerPtr colourMapManager = App::Inst().GetColourMapManager();
	ColourMapPtr defaultColourMap = colourMapManager->GetDefaultDiscreteColourMap();
	m_colourMap.reset(new ColourMapDiscrete(defaultColourMap));

	ShapeMapManagerPtr shapeMapManager = App::Inst().GetShapeMapManager();
	ShapeMapPtr defaultShapeMap = shapeMapManager->GetDefaultShapeMap();
	m_shapeMap.reset(new ShapeMap(defaultShapeMap));
}

template<class Archive>
void LocationSetController::serialize(Archive & ar, const unsigned int version)
{
	ar & m_bActive;                // bool
	ar & m_locationLayers;         // std::vector<LocationLayerPtr>
	ar & m_colourField;            // std::wstring
	ar & m_colourMap;              // ColourMapDiscretePtr
	ar & m_bUniformColour;         // bool
	ar & m_uniformColour;          // Colour
	ar & m_bLocationSetVisibility; // bool
	ar & m_shapeField;             // std::wstring
	ar & m_shapeMap;               // ShapeMapPtr
	ar & m_bUniformShape;          // bool
	ar & m_uniformShape;           // VisualMarker::MARKER_SHAPE
	ar & m_sizeField;              // std::wstring
	ar & m_minLocationSize;        // double
	ar & m_maxLocationSize;        // double
	ar & m_labelField;             // std::wstring
	ar & m_labelColour;            // Colour
	ar & m_labelSize;              // uint
	ar & m_labelPosition;          // std::wstring
	ar & m_labelStyle;             // std::wstring
	ar & m_bLabelVisible;          // bool
	ar & m_bLabelBindToChart;      // bool
	ar & m_borderSize;             // uint
	ar & m_borderColour;           // Colour
}
template void LocationSetController::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void LocationSetController::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);


void LocationSetController::SetLocationShapes()
{
	// Set colour of all locations
	for(unsigned int i = 0; i < m_locationLayers.size(); ++i)
	{
		VisualMarker::MARKER_SHAPE shape;
		if(m_bUniformShape)
		{
			shape = m_uniformShape;
		}
		else
		{
			// find shape in shape map corrsponding to field value of location
			std::map<std::wstring,std::wstring> data = m_locationLayers.at(i)->GetLocationController()->GetData();
			std::map<std::wstring,std::wstring>::const_iterator it = data.find(m_shapeField);

			 m_shapeMap->GetShape(it->second, shape);
		}

		LocationViewPtr locationView = m_locationLayers.at(i)->GetLocationController()->GetLocationView();
		locationView->SetShape(shape);
		locationView->SetShapeModified(false);
	}	
	
}

void LocationSetController::SetLocationVisibilities()
{
	for(unsigned int i = 0; i < m_locationLayers.size(); ++i)
	{
		LocationViewPtr locationView = m_locationLayers.at(i)->GetLocationController()->GetLocationView();
		locationView->SetVisibility(m_bLocationSetVisibility);
		locationView->SetVisibleModified(false);
	}
}

void LocationSetController::SetLocationColours()
{
	for(unsigned int i = 0; i < m_locationLayers.size(); ++i)
	{
		Colour colour;
		if(m_bUniformColour)
		{
			colour = m_uniformColour;
		}
		else
		{
			// find colour in colour map corrsponding to field value of location
			std::map<std::wstring,std::wstring> data = m_locationLayers.at(i)->GetLocationController()->GetData();
			std::map<std::wstring,std::wstring>::const_iterator it = data.find(m_colourField);

			if(!m_colourMap->GetColour(it->second, colour))
			{
				Log::Inst().Error("(Error) LocationSetController::SetLocationColours(): no colour associated with name.");
			}
		}

		LocationViewPtr locationView = m_locationLayers.at(i)->GetLocationController()->GetLocationView();
		locationView->SetColour(colour);
		locationView->SetColourModified(false);
	}
}

void LocationSetController::SetLocationBorders()
{
	for(unsigned int i = 0; i < m_locationLayers.size(); ++i)
	{

		LocationViewPtr locationView = m_locationLayers.at(i)->GetLocationController()->GetLocationView();
		locationView->SetBorderColour(m_borderColour);
		locationView->SetBorderSize(m_borderSize);
	}
}

void LocationSetController::FindMinMaxLocationSizes( double& min, double& max )
{
	for(unsigned int i = 0; i < m_locationLayers.size(); ++i)
	{
		// calculate min and max values
		std::map<std::wstring,std::wstring> data = m_locationLayers.at(i)->GetLocationController()->GetData();

		if ( m_sizeField.empty() )
		{
			std::vector<std::wstring> fields = GetNumericMetadataFields();
			m_sizeField = fields[0];
		}
		std::map<std::wstring,std::wstring>::const_iterator it = data.find(m_sizeField);

		double value = StringTools::ToDouble(it->second);
		if(value > max)
			max = value;

		if(value < min)
			min = value;
	}
}

void LocationSetController::SetLocationSizes()
{
	// Find min and max value for selected field
	double min = INT_MAX;
	double max = INT_MIN;

	FindMinMaxLocationSizes( min, max );

	// Set size of each location
	float minSize = m_minLocationSize;
	float maxSize = m_maxLocationSize;
	for(unsigned int i = 0; i < m_locationLayers.size(); ++i)
	{
		// get value of field for this location
		std::map<std::wstring,std::wstring> data = m_locationLayers.at(i)->GetLocationController()->GetData();			
		std::map<std::wstring,std::wstring>::const_iterator it = data.find(m_sizeField);
		double value = StringTools::ToDouble(it->second);

		// set relative size
		LocationViewPtr locationView = m_locationLayers.at(i)->GetLocationController()->GetLocationView();
		if(max-min > 0.0)
			locationView->SetSize(minSize + (value-min)/(max-min)*(maxSize-minSize));
		else
			locationView->SetSize(minSize);
		locationView->SetSizeModified(false);
	}
}

void LocationSetController::SetLocationLabels(bool bUpdateSize, bool bUpdatePosition, bool bUpdateStyle, bool bUpdateColour, bool bUpdateVisibility)
{
	// Set label properties for all locations
	for(unsigned int i = 0; i < m_locationLayers.size(); ++i)
	{
		std::map<std::wstring,std::wstring> data = m_locationLayers.at(i)->GetLocationController()->GetData();
		std::map<std::wstring,std::wstring>::const_iterator it = data.find(m_labelField);

		LocationViewPtr locationView = m_locationLayers.at(i)->GetLocationController()->GetLocationView();

		locationView->SetLabelBindToChart(m_bLabelBindToChart);

		VisualLabelPtr label = locationView->GetLabel();		
		label->SetText(it->second);		

		if(bUpdateVisibility)
		{
			label->SetVisibility(m_bLabelVisible);
			locationView->SetLabelVisibilityModified(false);
		}

		if(bUpdateSize)
		{
			label->SetSize(m_labelSize);
			locationView->SetLabelSizeModified(false);
		}

		if(bUpdateColour)
		{
			label->SetColour(m_labelColour);
			locationView->SetLabelColourModified(false);
		}

		if(bUpdatePosition)
		{
			label->SetLabelPosition(m_labelPosition);
			locationView->SetLabelPositionModified(false);
		}

		if(bUpdateStyle)
		{
			label->SetLabelStyle(m_labelStyle);
			locationView->SetLabelStyleModified(false);
		}

		locationView->SetLabel(label);
	}	
}

std::vector<std::wstring> LocationSetController::GetMetadataFields() const
{
	std::vector<std::wstring> fields;

	if(m_locationLayers.size() == 0)
	{
		Log::Inst().Error("(Error) LocationSetController::GetMetadataFields(): no location layers.");
	}
	else
	{
		std::map<std::wstring,std::wstring> metadata = m_locationLayers.at(0)->GetLocationController()->GetData();

		std::map<std::wstring,std::wstring>::iterator it;
		for(it = metadata.begin(); it != metadata.end(); ++it)
			fields.push_back(it->first);
	}

	return fields;
}

void LocationSetController::GetSortedFieldValues(const std::wstring& field, std::vector<std::wstring>& fieldValues)
{
	if ( field.empty() ) return;

	// get all unique field values within the given field
	std::set<std::wstring> uniqueFieldValues;
	for(unsigned int i = 0; i < m_locationLayers.size(); ++i)
	{
		std::map<std::wstring,std::wstring> data = m_locationLayers.at(i)->GetLocationController()->GetData();

		std::map<std::wstring,std::wstring>::const_iterator it = data.find(field);
		uniqueFieldValues.insert(it->second);
	}

	fieldValues.clear();
	fieldValues = std::vector<std::wstring>(uniqueFieldValues.begin(), uniqueFieldValues.end());

	StringTools::SortFieldValues( fieldValues );
}

std::vector<std::wstring> LocationSetController::GetNumericMetadataFields(bool bOnlyActiveLocs) const
{
	if(m_locationLayers.size() == 0)
	{
		Log::Inst().Error("(Error) LocationSetController::GetNumericMetadataFields(): no location layers.");
		return std::vector<std::wstring>();
	}

	std::vector<std::wstring> numericFields;
	foreach(const std::wstring& field, GetMetadataFields())
	{
		bool bNumeric = true;
		foreach(LocationLayerPtr locLayer, m_locationLayers)
		{
			if(locLayer->IsActive() || !bOnlyActiveLocs)
			{
				std::wstring value = locLayer->GetLocationController()->GetData()[field];
				
				if(!StringTools::IsDecimalNumber(value))
				{
					bNumeric = false;
					break;
				}
			}
		}

		if(bNumeric)
			numericFields.push_back(field);
	}

	return numericFields;
}

std::vector<std::wstring> LocationSetController::GetMetadata(const std::wstring& field, bool bOnlyActiveLocs) const
{
	if(m_locationLayers.size() == 0)
	{
		Log::Inst().Error("(Error) LocationSetController::GetMetadata(): no location layers.");
	}

	std::vector<std::wstring> data;
	for(uint i = 0; i < m_locationLayers.size(); ++i)
	{
		if(m_locationLayers.at(i)->IsActive() || !bOnlyActiveLocs)
			data.push_back(m_locationLayers.at(i)->GetLocationController()->GetData()[field]);
	}

	return data;
}

std::vector<float> LocationSetController::GetNumericMetadata(const std::wstring& field, bool bOnlyActiveLocs) const
{
	if(m_locationLayers.size() == 0)
	{
		Log::Inst().Error("(Error) LocationSetController::GetNumericMetadata(): no location layers.");
	}

	std::vector<float> data;
	for(uint i = 0; i < m_locationLayers.size(); ++i)
	{
		if(m_locationLayers.at(i)->IsActive() || !bOnlyActiveLocs)
			data.push_back(StringTools::ToDouble(m_locationLayers.at(i)->GetLocationController()->GetData()[field]));
	}

	return data;
}

void LocationSetController::AddMetadata(const std::wstring& field, const std::vector<std::wstring>& data) const
{
	if(data.size() != m_locationLayers.size())
	{
		wxMessageBox(_T("There must be a value specified for each location."), _T("Invalid data."), wxOK | wxICON_ERROR);
		return;
	}

	for(uint i = 0; i < m_locationLayers.size(); ++i)
		m_locationLayers.at(i)->GetLocationController()->AddData(field, data[i]);
}

std::map<std::wstring,std::wstring> LocationSetController::GetSequenceMetadata() const
{
	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		if(locationLayer->GetNumSequenceLayers() > 0)
		{
			return locationLayer->GetSequenceLayer(0)->GetSequenceController()->GetData();
		}
	}

	std::map<std::wstring,std::wstring> empty;
	return empty;
}

bool LocationSetController::ModifiedShape()
{
	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		if(locationLayer->GetLocationController()->GetLocationView()->GetShapeModified())
			return true;
	}

	return false;
}

bool LocationSetController::ModifiedSize()
{
	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		if(locationLayer->GetLocationController()->GetLocationView()->GetSizeModified())
			return true;
	}

	return false;
}

bool LocationSetController::ModifiedColour()
{
	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		if(locationLayer->GetLocationController()->GetLocationView()->GetColourModified())
			return true;
	}

	return false;
}

bool LocationSetController::ModifiedVisibility()
{
	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		if(locationLayer->GetLocationController()->GetLocationView()->GetVisibleModified())
			return true;
	}

	return false;
}

bool LocationSetController::ModifiedLabelSize()
{
	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		if(locationLayer->GetLocationController()->GetLocationView()->GetLabelSizeModified())
			return true;
	}

	return false;
}

bool LocationSetController::ModifiedLabelPosition()
{
	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		if(locationLayer->GetLocationController()->GetLocationView()->GetLabelPositionModified())
			return true;
	}

	return false;
}

bool LocationSetController::ModifiedLabelStyle()
{
	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		if(locationLayer->GetLocationController()->GetLocationView()->GetLabelStyleModified())
			return true;
	}

	return false;
}

bool LocationSetController::ModifiedLabelColour()
{
	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		if(locationLayer->GetLocationController()->GetLocationView()->GetLabelColourModified())
			return true;
	}

	return false;
}

bool LocationSetController::ModifiedLabelVisibility()
{
	foreach(LocationLayerPtr locationLayer, m_locationLayers)
	{
		if(locationLayer->GetLocationController()->GetLocationView()->GetLabelVisibilityModified())
			return true;
	}

	return false;
}
