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

#ifndef _GENGIS_NODE_PHYLO_H_
#define _GENGIS_NODE_PHYLO_H_

#include "../core/Precompiled.hpp"

#include "Node.hpp"

class GenGIS::NodePhylo;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::NodePhylo * t, const unsigned int file_version);
	}
}

namespace GenGIS
{
	/**
	* @brief Nodes used for phylogenetic trees.
	*/
	class NodePhylo : public Node
	{   
	public:  
		/**
		* @brief Constructor.
		* @param id Unique id identifying node.
		*/
		explicit NodePhylo(int id): Node(id), m_bootstrap(NO_DISTANCE) {}

		/**
		* @brief Constructor.
		* @param id Unique id identifying node.
		* @param name Name of node.
		*/
		explicit NodePhylo(int id,  const std::wstring & name) 
			: Node(id, name), m_bootstrap(NO_DISTANCE) {}

		/** Destructor. */
		virtual ~NodePhylo() {}

		/**
		* @brief Copy constructor.
		* 
		* @param node The node to copy.
		*/
		NodePhylo(const NodePhylo & node): Node(node), m_bootstrap(node.GetBootstrapToParent())  {}

		/**
		* @brief Assignation operator.
		*
		* @param node the node to copy.
		* @return A reference toward this node.
		*/
		NodePhylo& operator=(const NodePhylo& rhs)
		{
			if(this == &rhs)
				return *this;

			Node::operator=(rhs);

			m_bootstrap = rhs.GetBootstrapToParent();

			return *this;
		}

		/** 
		* @brief Get specified child node. 
		* @param pos Indicates child that should be returned.
		* @return Child at specified index position.
		*/
		virtual NodePhylo* GetChild(unsigned int pos) const { return (NodePhylo*)Node::GetChild(pos); }

		/** Get parent of this node. */     
		virtual NodePhylo* GetParent() const  { return (NodePhylo*)Node::GetParent(); }

		/** Remove the parent node. */
		virtual NodePhylo* RemoveParent() { return (NodePhylo*)Node::RemoveParent(); }

		/** Get all neighbouring nodes (i.e., nodes that are children of this nodes parent). */
		std::vector<NodePhylo*> GetNeighbors() const
		{
			std::vector<NodePhylo*> neighbors;

			NodePhylo* parent = GetParent();

			for(unsigned int i = 0; i < parent->GetNumberOfChildren(); i++) 
			{
				if(parent->GetChild(i) != this)
					neighbors.push_back(parent->GetChild(i));
			}

			return neighbors;
		}

		/** Get all child nodes. */
		// Note: this function is purposely not virual. Derived classes must implement there own
		// version of this class. if they wish to allow users to return a vector that is not of type NodeGeoTree*.
		std::vector<NodePhylo*> GetChildren() const
		{ 
			std::vector<NodePhylo*> children;

			for(unsigned int i = 0; i < GetNumberOfChildren(); ++i)
			{
				children.push_back((NodePhylo*)GetChild(i));
			}

			return children; 
		}


		/** Get the bootstap value between this node and its parent node.	 */     
		virtual float GetBootstrapToParent() const { return m_bootstrap; }

		/**
		* @brief Set the bootstap value between this node and its parent node.
		* @param bootstrap Bootstrap value to parent node.
		*/
		virtual void SetBootstrapToParent(float bootstrap) { m_bootstrap = bootstrap; }

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const NodePhylo * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<Node>(*this);
			ar & m_bootstrap; // float
		}

	protected:
		float m_bootstrap;
	};

} 

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const NodePhylo * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//uint _id = t->GetId(); // (m_id, unsigned int)
			//ar << _id;
			ar << t->m_id; // unsigned int
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, NodePhylo * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			unsigned int _id;
			ar >> _id;

			::new(t)NodePhylo(_id);
		}
	}
}

#endif
