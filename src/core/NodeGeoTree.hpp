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

#ifndef _GENGIS_NODE_GEO_TREE_H_
#define _GENGIS_NODE_GEO_TREE_H_

#include "../core/Precompiled.hpp"

#include "../utils/NodePhylo.hpp"
#include "../utils/Colour.hpp"

class GenGIS::NodeGeoTree;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::NodeGeoTree * t, const unsigned int file_version);
	}
}

namespace GenGIS
{

	/**
	* @brief Nodes used for displaying geographic tree models (GTM).
	*/
	class NodeGeoTree : public NodePhylo
	{
	public:  
		/**
		* @brief Constructor.
		* @param id Unique id identifying node.
		*/
		explicit NodeGeoTree(int id) :
			NodePhylo(id), m_gridCoord(Point3D(NO_DISTANCE, NO_DISTANCE)), m_bSelected(false), m_bProcessed(false),
			m_crossings(0), m_baryCenter(0), m_layoutPos(0), m_layoutDist(0), m_bActive(true), m_bCollapsed(false) {}

		/**
		* @brief Constructor.
		* @param id Unique id identifying node.
		* @param name Name of node.
		*/
		explicit NodeGeoTree(int id,  const std::wstring & name) :
			NodePhylo(id, name), m_gridCoord(Point3D(NO_DISTANCE, NO_DISTANCE)), m_bSelected(false), m_bProcessed(false),
			m_crossings(0),	m_baryCenter(0), m_layoutPos(0), m_layoutDist(0), m_bActive(true), m_bCollapsed(false) {}

		/** Destructor. */
		~NodeGeoTree() {}

		/**
		* @brief Copy constructor.
		* 
		* @param node The node to copy.
		*/
		NodeGeoTree(const NodeGeoTree& rhs): NodePhylo(rhs)
		{
			Copy(rhs);
		}

		/**
		* @brief Assignation operator.
		*
		* @param node the node to copy.
		* @return A reference toward this node.
		*/
		NodeGeoTree& operator=(const NodeGeoTree& rhs)
		{
			if(this == &rhs)
				return *this;

			NodePhylo::operator=(rhs);

			Copy(rhs);

			return *this;
		}

		/** 
		* @brief Copy contents of passed object.
		* @param node Node to copy.
		*/
		void Copy(const NodeGeoTree& node)
		{
			m_gridCoord       = node.GetGridCoord();
			m_locationLayerId = node.GetLocationLayerId();
			m_colour          = node.GetColour();
			m_bProcessed      = node.GetProcessed();
			m_baryCenter      = node.GetBaryCenter();
			m_layoutPos       = node.GetLayoutPos();
			m_layoutDist      = node.GetLayoutDistance();
			m_bSelected       = node.GetSelected();
			m_crossings       = node.GetNumCrossings();
			m_bActive         = node.GetActive();
			m_bCollapsed	  = node.IsCollapsed();
		}

		/** 
		* @brief Get specified child node. 
		* @param pos Indicates child that should be returned.
		* @return Child at specified index position.
		*/
		NodeGeoTree* GetChild(unsigned int pos) const { return (NodeGeoTree*)Node::GetChild(pos); }

		/** Get parent of this node. */     
		NodeGeoTree* GetParent() const  { return (NodeGeoTree*)Node::GetParent(); }

		/** Remove the parent node. */
		NodeGeoTree* RemoveParent() { return (NodeGeoTree*)Node::RemoveParent(); }

		/** Get all neighbouring nodes (i.e., nodes that are children of this nodes parent). */
		// Note: this function is purposely not virual. Derived classes must implement there own
		// version of this class. if they wish to allow users to return a vector that is not of type NodeGeoTree*.
		std::vector<NodeGeoTree*> GetNeighbors() const;

		/** Get all child nodes. */
		// Note: this function is purposely not virual. Derived classes must implement there own
		// version of this class if they wish to allow users to return a vector that is not of type NodeGeoTree*.
		std::vector<NodeGeoTree*> GetChildren() const;

		/** 
		* @brief Set child node.
		* @param index Index of child node to set.
		* @param childNode Child node to set.
		*/
		void SetChild(uint index, NodePhylo* childNode) { m_children.at(index) = childNode; }

		/** Get the grid coordinates of this node.	 */     
		const Point3D& GetGridCoord() const { return m_gridCoord; }

		/**
		* @brief Set the grid coordinates of this node.	
		* @param gridCoord Grid coordinates to assign to node.
		*/
		void SetGridCoord(const Point3D& gridCoord) { m_gridCoord = gridCoord; }

		/** Get location view associated with this node. */
		LocationViewPtr GetLocationView() const;

		/** Set location view associated with this node. */
		void SetLocationView(LocationViewPtr locationView);


		/** Set state of processed flag. Used to indicate if a node has already been processed by an algorithm. */ 
		void SetProcessed(bool state) { m_bProcessed = state; }

		/** Get state of processed flag. Used to indicate if a node has already been processed by an algorithm. */ 
		bool GetProcessed() const { return m_bProcessed; }

		/** Get node colour. */
		Colour GetColour() const { return m_colour; }

		/** Set node colour. */
		void SetColour(const Colour& colour) { m_colour = colour; }

		/** Set barycenter for this node. */
		void SetBaryCenter(double value) { m_baryCenter = value; }

		/** Get barycenter for this node. */
		double GetBaryCenter() const { return m_baryCenter; }

		/** Set desired position of node along a layout primative. */
		void SetLayoutPos(uint pos) { m_layoutPos = pos; }

		/** Get desired position of node along a layout primative. */
		uint GetLayoutPos() const { return m_layoutPos; }

		/** Set desired distance of node along a layout primative. */
		void SetLayoutDistance(float dist) { m_layoutDist = dist; }

		/** Get desired distance of node along a layout primative. */
		float GetLayoutDistance() const { return m_layoutDist; }

		/** Set if node is within a selected subtree. */
		void SetSelected(bool bSelected) { m_bSelected = bSelected; }

		/** Get flag indicating if node is within a selected subtree. */
		bool GetSelected() const { return m_bSelected; }

		/** Set select flag for entire subtree. */
		void SetSelectedSubtree(bool state);

		/** Set number of crossings below this node. */
		void SetNumCrossings(uint crossings) { m_crossings = crossings; }

		/** Get number of crossings below this node. */
		uint GetNumCrossings() const { return m_crossings; }

		/** Set flag indicating if node is in the active set (i.e., nodes that should be considered in an analysis). */
		void SetActive(bool bActive) { m_bActive = bActive; }

		/** Get flag indicating if node is in the active set (i.e., nodes that should be considered in an analysis). */
		bool GetActive() const { return m_bActive; }

		/** Get the id of the corresponding location layer.  */
		uint GetLocationLayerId() const { return m_locationLayerId; }

		/** Get flag indicating if the node is in a collapsed subtree */
		bool IsCollapsed() const { return m_bCollapsed; }

		/** Check if the node is the parent node of a collapsed subtree */
		bool IsCollapsedParent() const { return ( !m_bCollapsed && !m_children.empty() && GetChild(0)->IsCollapsed() ); }

		/** Set flag indicating if the node is in a collapsed subtree */
		void SetCollapsed(bool collapsed) { m_bCollapsed = collapsed; }

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const NodeGeoTree * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Grid (world) coordinates of node. */
		Point3D m_gridCoord;

		/** Location view associated with this node. */
		//LocationViewPtr m_locationView;

		/** Id of location layer associated with this node. */
		uint m_locationLayerId;

		/** Colour associated with node. Used during rendering. */
		Colour m_colour;

		/** Simple flag the can be used to indicate if the node has already been processed. */
		bool m_bProcessed;

		/** Barycentre of node. Used for heuristic graph layout. */
		double m_baryCenter;

		/** Desired position of node along a layout primative. */
		uint m_layoutPos;

		/** Desired distance of node along a layout primative. */
		float m_layoutDist;

		/** Indicate if node is within a selected subtree. */
		bool m_bSelected;

		/** Number of crossings in subtree defined by this node. */
		uint m_crossings;

		/** Flag indicating if node is in the active set (i.e., nodes that should be considered in an analysis). */
		bool m_bActive;

		/** Flag indicating if node is in a collapsed subtree */
		bool m_bCollapsed;
	};

} 

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const NodeGeoTree * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//uint _id = t->GetId(); // (m_id, unsigned int)
			//ar << _id;
			ar << t->m_id; // unsigned int
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, NodeGeoTree * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			unsigned int _id;
			ar >> _id;

			::new(t)NodeGeoTree(_id);
		}
	}
}

#endif

