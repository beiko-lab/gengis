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

#ifndef _GENGIS_GEO_TREE_LINEAR_LAYOUT
#define _GENGIS_GEO_TREE_LINEAR_LAYOUT

#include "../core/Precompiled.hpp"

#include "../utils/Tree.hpp"
#include "../core/NodeGeoTree.hpp"

namespace GenGIS 
{

	class GeoTreeLinearLayout
	{
	public:
		/** 
		* @brief Constructor. 
		* @param mapController Controller for terrain tree is to be laid out on.
		*/
		explicit GeoTreeLinearLayout() {}

		/** Destructor. */
		~GeoTreeLinearLayout() {}

		/** Layout nodes for 2D cladogram. */
		void LayoutCladogram2D(Tree<NodeGeoTree>::Ptr tree, LayoutLinePtr layoutLine, float height, bool bHorizontal);

		/** Layout nodes for 2D slanted cladogram. */
		void LayoutSlantedCladogram2D(Tree<NodeGeoTree>::Ptr tree, LayoutLinePtr layoutLine, float height, bool bHorizontal);

		/** Layout nodes for 2D phylogram. */
		void LayoutPhylogram2D(Tree<NodeGeoTree>::Ptr tree, LayoutLinePtr layoutLine, float height, bool bHorizontal);

		/** Get position of implicit root. */
		Point3D GetImplicitRoot() const { return m_implicitRoot; }

	protected:
		/** Scale, rotate, and translate a canonical layout. */
		void TransformTreeLayout(Tree<NodeGeoTree>::Ptr tree, LayoutLinePtr layoutLine, float height, bool bHorizontal);

		/**
		* @brief Calculate mean of a set of points.
		* @param points Pointed to calculate mean of.
		* @param mean Mean of points.
		* @return True if mean was calculated, false if one or more nodes was not set.
		*/
		bool CalculateMean(const std::vector<Point3D>&  points, Point3D& mean);

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		Point3D m_implicitRoot;
	};

}

#endif
