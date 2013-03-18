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

#ifndef _GENGIS_LOCATION_VIEW_
#define _GENGIS_LOCATION_VIEW_

#include "../core/Precompiled.hpp"

#include "../core/Camera.hpp"
#include "../core/View.hpp"
#include "../core/VisualMarker.hpp"
#include "../core/LocationModel.hpp"
#include "../core/MapController.hpp"

#include "../utils/Colour.hpp"

class GenGIS::LocationView;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::LocationView * t, const unsigned int file_version);
	}
}

namespace GenGIS
{
	/**
	* @brief Render location as marker in an OpenGL viewport.
	*/
	class LocationView : public View
	{
	private:
		/** Indicate which view within a location view is currently active. */
		enum ACTIVE_VIEW { NO_ACTIVE_VIEW = 0, LOCATION_VIEW = 1, LABEL = 2 };

		/** Flags indicating different boolean properties of a location view. */
		static const int NUM_FLAGS = 10;
		enum FLAGS {
			BOUND_TO_CHART = 0, COLOUR_MOD = 1, SIZE_MOD = 2, SHAPE_MOD = 3, 
			VISIBILITY_MOD = 4, LABEL_SIZE_MOD = 5, LABEL_POSITION_MOD = 6,
			LABEL_COLOUR_MOD = 7, LABEL_VISIBILITY_MOD = 8, LABEL_STYLE_MOD = 9 };

	public:
		/** 
		* @brief Constuctor.
		* @param locationModel Location associated with this view.
		* @param camera Camera associated with OpenGL viewport.
		*/
		explicit LocationView(LocationModelPtr locationModel, CameraPtr camera, uint id);

		/** Destructor. */
		~LocationView() {}

		/** Render location model. */
		void Render();

		/** Get unique id. */
		uint GetId() const
		{
			return m_id;
		}

		/** Set unique id. */ 
		void SetId(uint id)
		{
			m_id = id;
		}

		/** Get id of location. */
		std::wstring GetLocationId() const
		{
			return m_location->GetId();
		}

		/** Get location model (for serialization). */
		LocationModelPtr GetLocationModel() const
		{
			return m_location;
		}

		/** Set the id of the location layer associated with this view. */
		virtual void SetLocationLayerId(uint locationLayerId)
		{
			m_locationLayerId = locationLayerId;
		}

		/** Get the id of the location layer associated with this view. */
		virtual uint GetLocationLayerId() const
		{
			return m_locationLayerId;
		}

		/** Get grid position of location marker. */
		Point3D GetPosition() const;

		/** Get pointer to the camera object (for serialization). */
		CameraPtr GetCamera() const
		{
			return m_camera;
		}

		/** Get colour of location marker. */
		Colour GetColour() const
		{
			return m_colour;
		}

		/** Set location marker colour. */
		void SetColour(const Colour& colour)
		{
			m_colour = colour;
		}

		/** Get size of location marker. */
		float GetSize() const
		{
			return m_size;
		}

		/** Set location marker size. */
		void SetSize(float size)
		{
			m_size = size;
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

		/** Get shape of location marker. */
		VisualMarker::MARKER_SHAPE GetShape() const
		{
			return m_shape;
		}

		/** Set location marker shape. */
		void SetShape(VisualMarker::MARKER_SHAPE shape)
		{
			m_shape = shape;
		}

		/** Set label. */
		void SetLabel(VisualLabelPtr label)
		{
			m_label = label;
		}

		/** Get label. */
		const VisualLabelPtr GetLabel() const
		{
			return m_label;
		}

		/** Set label bind to chart flag. */
		void SetLabelBindToChart(bool state)
		{
			m_flags[BOUND_TO_CHART] = state;
		}

		/** Get label bind to chart flag. */
		bool GetLabelBindToChart() const
		{
			return m_flags[BOUND_TO_CHART];
		}

		/** Get 2D bounding box for chart. */
		Box2D GetBoundingBox() const;

		/** Add a leaf node that is associated with this location. */
		void AddLeafNode(NodeGeoTree* leafNode)
		{
			m_leafNodes.push_back(leafNode);
		}

		/** Get leaf nodes associated with this location. */
		std::vector< NodeGeoTree* > GetLeafNode() const
		{
			return m_leafNodes;
		}

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
		* @brief Process mouse move events. 
		* @param mousePt Screen position of mouse.
		*/
		bool MouseMove(const Point2D& mousePt);

		/** 
		* @brief Process mouse dragging events. 
		* @param mousePt Screen position of mouse.
		*/
		void MouseDragging(const Point2D& mousePt);

		/** 
		* @brief Process keyboard events. 
		* @param mousePt Screen position of mouse.
		* @return True if view is processing the event, else false.
		*/
		void KeyboardEvent(wxKeyEvent& event);

		/** Set flag indicating if location colour has been modified. */
		void SetColourModified(bool state)
		{
			m_flags[COLOUR_MOD] = state;
		}

		/** Get flag indicating if location colour has been modified. */
		bool GetColourModified() const
		{
			return m_flags[COLOUR_MOD];
		}

		/** Set flag indicating if location size has been modified. */
		void SetSizeModified(bool state)
		{
			m_flags[SIZE_MOD] = state;
		}

		/** Get flag indicating if location size has been modified. */
		bool GetSizeModified() const
		{
			return m_flags[SIZE_MOD];
		}

		/** Set flag indicating if location shape has been modified. */
		void SetShapeModified(bool state)
		{
			m_flags[SHAPE_MOD] = state;
		}

		/** Get flag indicating if location shape has been modified. */
		bool GetShapeModified() const
		{
			return m_flags[SHAPE_MOD];
		}

		/** Set flag indicating if location visibility has been modified. */
		void SetVisibleModified(bool state)
		{
			m_flags[VISIBILITY_MOD] = state;
		}

		/** Get flag indicating if location visibility has been modified. */
		bool GetVisibleModified() const
		{
			return m_flags[VISIBILITY_MOD];
		}

		/** Set flag indicating if label size has been modified. */
		void SetLabelSizeModified(bool state)
		{
			m_flags[LABEL_SIZE_MOD] = state;
		}

		/** Get flag indicating if label size has been modified. */
		bool GetLabelSizeModified() const
		{
			return m_flags[LABEL_SIZE_MOD];
		}

		/** Set flag indicating if label position has been modified. */
		void SetLabelPositionModified(bool state)
		{
			m_flags[LABEL_POSITION_MOD] = state;
		}

		/** Get flag indicating if label position has been modified. */
		bool GetLabelPositionModified() const
		{
			return m_flags[LABEL_POSITION_MOD];
		}

		/** Set flag indicating if label sty;e has been modified. */
		void SetLabelStyleModified(bool state)
		{
			m_flags[LABEL_STYLE_MOD] = state;
		}

		/** Get flag indicating if label style has been modified. */
		bool GetLabelStyleModified() const
		{
			return m_flags[LABEL_STYLE_MOD];
		}

		/** Set flag indicating if label colour has been modified. */
		void SetLabelColourModified(bool state)
		{
			m_flags[LABEL_COLOUR_MOD] = state;
		}

		/** Get flag indicating if label colour has been modified. */
		bool GetLabelColourModified() const
		{
			return m_flags[LABEL_COLOUR_MOD];
		}

		/** Set flag indicating if label visibility has been modified. */
		void SetLabelVisibilityModified(bool state)
		{
			m_flags[LABEL_VISIBILITY_MOD] = state;
		}

		/** Get flag indicating if label visibility has been modified. */
		bool GetLabelVisibilityModified() const
		{
			return m_flags[LABEL_VISIBILITY_MOD];
		}

		/** Set the visibility of view. */
		void SetVisibility( bool visible )
		{
			m_location->SetActive(visible);
		}

		/** Set the visibility of view. */
		bool IsVisible() const
		{
			return m_location->IsActive();
		}

		/** Get projected coordinates of location. */
		GeoCoord GetProjection() const { return m_location->GetProjection(); }

	protected:
		/** Render location label. */
		void RenderLabel();

		/** 
		* @brief Find node in tree with the given id.
		* @param id ID of node of interest.
		* @return Node with the given id or NULL if no such node is found.
		*/
		NodeGeoTree* GetTreeNode(const std::wstring& id);

		/** Determine if the location site has been clicked. */
		bool IsClicked(const Point2D& mousePt);

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		
		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const LocationView * t, const unsigned int file_version);
		
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);
		
		template<class Archive>
		void save(Archive & ar, const unsigned int version) const;
		
		template<class Archive>
		void load(Archive & ar, const unsigned int version);

	private:
		/** Location model associated with this view. */
		LocationModelPtr m_location;

		/** Camera of OpenGL viewport associated with this location set. */
		CameraPtr m_camera;

		/** Unique id. */
		uint m_id;

		/** Id of location layer associated with this view. */
		uint m_locationLayerId;

		/** Leaf nodes associated with this location. */
		std::vector< NodeGeoTree* > m_leafNodes;

		/** Colour of location marker. */
		Colour m_colour;

		/** Size of location marker. */
		float m_size;

		/** Shape of location marker. */
		VisualMarker::MARKER_SHAPE m_shape;

		/** Rendering order. */
		float m_renderingOrder;

		/** Label for location. */
		VisualLabelPtr m_label;

		/** Flags indicating different boolean properties of a location view. */
		boost::dynamic_bitset<> m_flags;

		/** Currently active view. */
		ACTIVE_VIEW m_activeView;

		/** Border size. */
		uint m_borderSize;

		/** Border colour. */
		Colour m_borderColour;
	};
}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const LocationView * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			ar << t->m_location; // LocationModelPtr
			ar << t->m_camera;   // CameraPtr
			ar << t->m_id;       // uint
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, LocationView * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			LocationModelPtr _location;
			ar >> _location;

			CameraPtr _camera;
			ar >> _camera;

			uint _id;
			ar >> _id;

			::new(t)LocationView(_location, _camera, _id);
		}
	}
}

#endif
