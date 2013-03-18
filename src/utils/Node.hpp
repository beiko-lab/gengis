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

#ifndef _GENGIS_NODE_H_
#define _GENGIS_NODE_H_

#include "../core/Precompiled.hpp"

class GenGIS::Node;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::Node * t, const unsigned int file_version);
	}
}

namespace GenGIS
{

	/**
	* @brief Nodes used in trees.
	*/
	class Node
	{
	public:
		enum NODE_FLAGS { NO_DISTANCE = INT_MIN };

	public:  
		/**
		* @brief Constructor.
		* @param id Unique id identifying node.
		*/
		explicit Node(unsigned int id) :
			m_id(id), m_name(_T("")), m_parent(NULL), m_distanceToParent(NO_DISTANCE) {}

		/**
		* @brief Constructor.
		* @param id Unique id identifying node.
		* @param name Name of node.
		*/
		explicit Node(unsigned int id,  const std::wstring & name) :
			m_id(id), m_name(name), m_parent(NULL), m_distanceToParent(NO_DISTANCE) {}

		/** Destructor. */
		virtual ~Node() {}

		/**
		* @brief Copy constructor.
		* 
		* @param node The node to copy.
		*/
		Node(const Node & rhs);

		/**
		* @brief Assignation operator.
		*
		* @param node the node to copy.
		* @return A reference toward this node.
		*/
		Node& operator=(const Node& rhs);

		/** 
		* @brief Copy contents of passed object.
		* @param node Node to copy.
		*/
		void Copy(const Node& node);

		/** Set index of leaf node. */
		void SetLeafOrderIndex(uint index)
		{
			m_leafNodeIndex = index;
		}

		/** Get leaf node index. */
		uint GetLeafOrderIndex() const
		{
			return m_leafNodeIndex;
		}

	public:          
		/** Get id associated with this node. */
		virtual unsigned int GetId() const
		{
			return m_id;
		}
		/**
		* @brief Set node's id.
		* @param id New identity tag.
		*/
		virtual void SetId(unsigned int id)
		{
			m_id = id;
		}

		/** Get id of all children. */
		virtual std::vector<unsigned int> GetChildrenIds() const;

		/** Get the name associated to this node. */
		virtual std::wstring GetName() const
		{
			return m_name;
		}

		/**
		* @brief Set name of node.
		* @param name Name to give node.
		*/
		virtual void SetName(const std::wstring& name)
		{
			m_name = name;
		}

		/** Get the distance to the parent node.	 */     
		virtual float GetDistanceToParent() const
		{
			return m_distanceToParent;
		}

		/**
		* @brief Set the distance to the parent node.
		* @param distance Distance to parent node.
		*/
		virtual void SetDistanceToParent(float distance)
		{
			m_distanceToParent = distance;
		}

		/** Get parent of this node. */     
		virtual Node* GetParent() const
		{
			return m_parent;
		}

		/** Get id of parent node. */
		virtual unsigned int GetParentId() const
		{
			return m_parent->GetId();
		}

		/**
		* @brief Set the parent node.
		* @param node Parent node.
		*/
		virtual void SetParent(Node* parent)
		{
			m_parent = parent;
		}

		/** Remove the parent node. */
		virtual Node* RemoveParent()
		{
			Node* p = m_parent; m_parent = NULL; return p;
		}

		/** Indicate if this node is the root node.	 */
		virtual bool IsRoot() const
		{
			return m_parent == NULL;
		}

		/** Get number of child nodes. */	     
		virtual unsigned int GetNumberOfChildren() const
		{
			return m_children.size();
		}

		/** 
		* @brief Get specified child node. 
		* @param pos Indicates child that should be returned.
		* @return Child at specified index position.
		*/
		virtual Node* GetChild(unsigned int pos) const;

		/** 
		* @brief Add child node at specified index position. 
		* @param pos Position to add child.
		* @param node Child node to add.
		*/
		virtual void AddChild(unsigned int pos, Node* node);

		/** 
		* @brief Add child node.
		* @param node Child node to add.
		*/
		virtual void AddChild(Node* node);

		/** 
		* @brief Remove child node.
		* @param pos Specifies which child to remove.
		*/
		virtual void RemoveChild(unsigned int pos);

		/** 
		* @brief Remove child node.
		* @param node Node to be removed.
		*/
		virtual void RemoveChild(Node* node);

		/** Remove all child nodes. */
		virtual void RemoveChildren();

		/**
		* @brief Get position of specified child node.
		* @param child Child node to determine position of.
		* @return Position of child node.
		*/
		virtual unsigned int GetChildPosition(Node* child) const;

		/** Get all neighbouring nodes (i.e., nodes that are children of this nodes parent). */
		// Note: this function is purposely not virual. Derived classes must implement there own
		// version of this class. if they wish to allow users to return a vector that is not of type Node*.
		std::vector<Node*> GetNeighbors() const;

		/** Get all child nodes. */
		// Note: this function is purposely not virual. Derived classes must implement there own
		// version of this class. if they wish to allow users to return a vector that is not of type Node*.
		std::vector<Node*> GetChildren() const
		{
			return m_children;
		}

		/** Check if node is a leaf node. */
		virtual bool IsLeaf() const
		{
			return m_children.empty() && m_parent != NULL;
		}

		/** 
		* @brief Swap the position of the two indicated children.
		* @brief pos1 Position of first child.
		* @brief pos2 Position of second child.
		*/
		virtual void Swap(unsigned int pos1, unsigned int pos2);

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const Node * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	protected:
		/** Unique id identifying node. */
		unsigned int m_id;

		/** Name of node. */
		std::wstring m_name;

		/** Children of node. */
		std::vector<Node*> m_children;

		/** Parent of node. */
		Node* m_parent;

		/** Distance to parent of this node. */
		float m_distanceToParent;  

		/** Index of leaf node amongst all leaf nodes. */
		uint m_leafNodeIndex;
	};

} 

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::Node * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//uint _id = t->GetId(); // (m_id, unsigned int)
			//ar << _id;
			ar << t->m_id; // unsigned int
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, GenGIS::Node * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			unsigned int _id;
			ar >> _id;

			::new(t)GenGIS::Node(_id);
		}
	}
}

#endif
