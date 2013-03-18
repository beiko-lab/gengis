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

#ifndef _GENGIS_GEO_TREE_ELLIPTICAL_LAYOUT
#define _GENGIS_GEO_TREE_ELLIPTICAL_LAYOUT

#include "../core/Precompiled.hpp"

#include "../core/NodeGeoTree.hpp"
#include "../utils/Tree.hpp"

class GenGIS::GeoTreeEllipticalLayout;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::GeoTreeEllipticalLayout * t, const unsigned int file_version);
	}
}

namespace GenGIS 
{

	class GeoTreeEllipticalLayout
	{
	public:
		/** Node and associated graphical elements for an elliptical layout. */
		typedef struct sELLIPTICAL_NODE
		{
			/** Constructor. */
			sELLIPTICAL_NODE(NodeGeoTree* _node, Line3D _line = Line3D(), PolyLine3D _arc = PolyLine3D()): node(_node), line(_line), arc(_arc) {}

			/** Node in elliptical layout. */
			NodeGeoTree* node;

			/** Arcs connecting node to its children. */
			PolyLine3D arc;

			/** Line connecting node to its parent arc.*/
			Line3D line;

			/** Serialization for struct. */
			template <typename Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				ar & arc;  // PolyLine3D
				ar & line; // Line3D
			}
		} EllipticalNode;

	public:
		/** 
		* @brief Constructor. 
		* @param mapController Controller for terrain tree is to be laid out on.
		*/
		explicit GeoTreeEllipticalLayout() {}

		/** Destructor. */
		~GeoTreeEllipticalLayout() { }

		/** Layout tree around the layout ellipse with points positioned independently in each quadrant. */
		void LayoutEllipseQuadrant(Tree<NodeGeoTree>::Ptr tree, LayoutEllipsePtr layoutEllipse, float height);

		/** Get elliptical nodes that specify how to render this layout. */
		const std::vector<EllipticalNode>& GetEllipticalNodes() const { return m_ellipticalNodes; }

	protected:
		/**
		* @brief Calculate angle to point between 0 and 2PI in standard ellipse coordinate frame.			
		* @param pt Point on ellipse.
		* @return Angle to point from x-axis in standard ellipse coordinate frame.
		*/
		double GetAngleOnStandardEllipse(LayoutEllipsePtr layoutEllipse,const Point3D& pt) const;

		/**
		* @brief Calculate position of node in elliptical layout.
		* @param angle Angle of node from x-axis in CCW direction (in radians).
		* @param childOffset Distance of child node from layout ellipse. 
		* @param parentOffset Distance of parent node from layout ellipse.
		* @return Position of child node on the arc which connects the node to its parent.
		*/
		Point3D GetEllipseLayoutPt(LayoutEllipsePtr layoutEllipse, double angle, double childOffset, double parentOffset) const;

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const GeoTreeEllipticalLayout * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Nodes and associated graphical elements for an elliptical layout. */
		std::vector<EllipticalNode> m_ellipticalNodes;
	};

}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const GeoTreeEllipticalLayout::EllipticalNode * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			ar << t->node; // NodeGeoTree*
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, GeoTreeEllipticalLayout::EllipticalNode * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			NodeGeoTree* node;
			ar >> node;

			::new(t)GeoTreeEllipticalLayout::EllipticalNode(node);
		}

		template<class Archive>
		inline void save_construct_data(Archive & ar, const GeoTreeEllipticalLayout * t, const unsigned int file_version)
		{
			// Save data required to construct instance
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, GeoTreeEllipticalLayout * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			::new(t)GeoTreeEllipticalLayout();
		}
	}
}

#endif
