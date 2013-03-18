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

#ifndef _GENGIS_GEO_AXIS_POLYLINE_
#define _GENGIS_GEO_AXIS_POLYLINE_

#include "../core/Precompiled.hpp"

#include "../core/NodeGeoTree.hpp"
#include "../core/View.hpp"
#include "../core/VisualPolyLine.hpp"

#include "../utils/Tree.hpp"
#include "../utils/Point3D.hpp"

namespace GenGIS
{

	class GeoAxisPolyline : public View 
	{
	public:
		/** Stage of defining ellipse. */
		enum STAGE { START = 0, LAYING_OUT_PTS, COMPLETE };

		/** Indentify the currently active control point. */
		enum ACTIVE_POINT { NONE = 0, CONTROL_PT };

	public:
		/**
		* @brief Constructor.
		* @param mapController Controller for terrain polyline will be rendered on. 
		*/
		explicit GeoAxisPolyline();

		/** Destructor. */
		~GeoAxisPolyline() {}

		/** Render geographic axis. */
		void Render();

		/** Indicate construction of polyline is complete. */
		void PolylineFinished();

		/** Get stage of polyline construction. */
		STAGE GetStage() const { return m_stage; }

		/** Set tree associated with geographic axis. */
		void SetTree(Tree<NodeGeoTree>::Ptr tree);

		/** 
		* @brief Extend geographic axis.
		*/
		void ExtendGeographicAxis();

		/** 
		* @brief Determine position of geographical location along geographic axis.
		* @param locationView Location view of interest.
		* @param pos Will be set to the position of the geographic location along the geographic axis.
		* @param dist Will be set to the distance of the geographic location from the start of the geographic axis.
		* @return True if geographical location is on geographic axis, else False.
		*/
		bool GeographicLocationPos(LocationViewPtr locationView, uint& pos, float& dist);

		/** 
		* @brief Get the number of leaf nodes associated with geographic locations that are along the geographic axis. 
		* @param bUpdate Flag indicating if the number of active leaf nodes should be recalculated.
		*/
		uint GetNumActiveLeafNodes(bool bUpdate = false);

		/** Get number of possible polyline orderings. */
		uint GetNumPolyLineOrderings() const;

		/** Set desired polyline ordering. */
		std::vector<uint> SetPolyLineOrdering(uint ordering);

		/** Get length of geographic axis. */
		float GetLength() const { return m_length; }

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

	private:
		/** Data associated with a geographic point. */
		typedef struct sGEO_POINT_DATA
		{
			/** Constructor. */
			sGEO_POINT_DATA(uint _pos = 0, float _dist = 0): pos(_pos), dist(_dist) {}

			uint pos;
			float dist;

			/** Serialization for struct. */
			template <typename Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				ar & pos;  // uint
				ar & dist; // float
			}
		} GeoPointData;

	private:
		/** Handle mouse events when manipulating a pre-existing line. */
		bool ModifyLineInteraction(const Point2D& mousePt);

		/** Check if mouse cursor is over a control point. */
		bool IsPointClicked(const Point2D& mousePt, const Point3D& controlPt);

		/** Check if mouse cursor is over any active element of the layout line. */
		bool IsClicked(const Point2D& mousePt);

		/** Find order of geographic locations along geographic axis. */
		void OrderOfGeographicLocations();

		/** 
		* @brief Determine if a geographical location is intersected by a line.
		* @param geoLocationPos Controller for geographic location of interest.
		* @param line Line to check for interesection with
		* @param distFromStart Distance from start of line to geographical location (assuming an intersection has occured)
		* @return True if geographical location and line intersection, else False.
		*/
		bool IntersectGeoLocation(const LocationViewPtr geoLocationPos, const Line3D& line, float& distFromStart);

		/** Serialization. */
		friend class boost::serialization::access;
		
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	private:
		/** Geographic tree model. */
		Tree<NodeGeoTree>::Ptr m_tree;

		/** Polyline defining a geographical axis. */
		std::vector<VisualPolyLinePtr> m_polylines;

		/** Polyline under modification. */
		std::vector<VisualPolyLinePtr> m_modPolylines;

		/** Indicate currently selected polyline. */
		uint m_activePolylineIndex;

		/** Indicate currently selected control point. */
		uint m_activePtIndex;

		/** Tracks stage of polyline construction. */
		STAGE m_stage;

		/** Tracks currently active control point. */
		ACTIVE_POINT m_activePt;

		/** Length of geographic axis. */
		float m_length;

		/** Map which indicates the position of each geographic location on the geographic axis. */
		std::map< LocationViewPtr, GeoPointData > m_geoAxisMap;

		/** Keep track of the number of leaf nodes associated with locations along the geographic axis. */
		uint m_activeLeafNodes;
	};

}

#endif
