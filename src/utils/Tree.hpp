/*
Copyright or © or Copr. CNRS, (November 16, 2004)

This software is a computer program whose purpose is to provide classes
for phylogenetic data analysis.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

//=======================================================================
// Modify by: Donovan Parks
//
// Copyright 2009 Donovan Parks
//
// Based on code by: Julien Dutheil and Celine Scornavacca (Bio++ project)
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


#ifndef _GENGIS_TREE_
#define _GENGIS_TREE_

#include "../core/Precompiled.hpp"
#include "../utils/Log.hpp"
#include "../utils/TreeTools.hpp"

#include <ostream>

namespace GenGIS
{

	/**
	* @brief Build a new tree. The nodes of the tree can be any class derived from Node.
	*
	* Code example:
	* @code
	* @endcode
	*
	* @see Node
	*/
	template<class N> class Tree
	{
	public:
		/** Allows convenient syntax for creating smart pointers of this class. */
		typedef boost::shared_ptr< Tree<N> > Ptr;	

	public:	
		/** Constructor. */
		explicit Tree(): m_root(NULL), m_name(_T("")) {}

		/**
		* @brief Constructor
		* @param root Root of tree.
		*/
		explicit Tree(N* root): m_root(root), m_name(_T("")) {}

		/** Copy constructor. */
		Tree(const Tree<N>& rhs);

		/** Assignment operator. */
		Tree<N>& operator=(const Tree<N>& rhs);

		/** Destructor. */
		~Tree();

		/** Clone tree. */
		typename Tree::Ptr Clone() const
		{
			return typename Tree::Ptr(new Tree<N>(*this));
		}

	public:
		/** Get name of tree. */
		std::wstring GetName() const
		{
			return m_name;
		}

		/**
		* @brief Set name of tree.
		* @param name Desired name of tree.
		*/		
		void SetName(const std::wstring & name)
		{
			m_name = name;
		}

		/** 
		* @brief Set root node of tree.
		* @param root Desired root node.
		*/
		void SetRootNode(N* root)
		{
			m_root = root;
		}

		/** Get root node. */
		N* GetRootNode() const
		{
			return m_root;
		}

		/** Get id of root node. */
		unsigned int GetRootId() const
		{
			return m_root->GetId();
		}

		/** Get node with the given id. */	
		N* GetNode(unsigned int id) const; 

		/** Get node with the given name. */
		N* GetNode(const std::wstring& name) const;

		/** Get number of leaf nodes in tree. */
		unsigned int GetNumberOfLeaves() const
		{
			return TreeTools<N>::GetNumberOfLeaves(m_root);
		}

		/** Get all leaf nodes in tree. */
		std::vector<N*> GetLeaves() const
		{
			return TreeTools<N>::GetLeaves(m_root);
		}

		/** Get id of all leaf nodes. */
		std::vector<unsigned int> GetLeafIds() const
		{
			return TreeTools<N>::GetLeafIds(m_root);
		}

		/** Get name of all leaf nodes in depth first order. */
		std::vector<std::wstring> GetLeafNames() const
		{
			return TreeTools<N>::GetLeafNames(m_root);
		}

		/** Get number of nodes in tree. */
		unsigned int GetNumberOfNodes() const
		{
			return TreeTools<N>::GetNumberOfNodes(m_root);
		}

		/** Get all nodes in tree. */
		std::vector<N*> GetNodes() const
		{
			return TreeTools<N>::GetNodes(m_root);
		}

		/** Get id of all nodes. */
		std::vector<unsigned int> GetNodesId() const
		{
			return TreeTools<N>::GetNodesId(m_root);
		}

		/** Get length of all branches. */
		std::vector<float> GetBranchLengths() const
		{
			return TreeTools<N>::GetBranchLengths(m_root);
		}

		/** Get depth of a give node (i.e. the maximum number of children 'generations'). */
		unsigned int GetDepth(N* node) const
		{
			return TreeTools<N>::GetDepth(node);
		}

		/** Get distance to furthest leaf node. */
		float GetDistToFurthestLeafNode(N* node) const
		{
			return TreeTools<N>::GetDistToFurthestLeafNode(node);
		}

		/** Get distance to root. */
		float GetDistToRoot(N* node) const
		{
			return TreeTools<N>::GetDistToRoot(node);
		}

		/** 
		* @brief Get id of all children to the given node. 
		* @param parentId Id of parent node.
		* @return Id of all chilren to the node with the given id.
		*/
		std::vector<unsigned int> GetChildrenIds(unsigned int parentId) const
		{
			return GetNode(parentId)->GetChildrenIds();
		}

		/** 
		* @brief Get id of parent node to the given node.
		* @param nodeId Id of node of interest.
		* @return Id of parent node.
		*/
		unsigned int GetParentId(unsigned int nodeId) const
		{
			return GetNode(nodeId)->GetParentId();
		}

		/** 
		* @brief Get name of node.
		* @param nodeId Id of node of interest.
		* @return Name of node.
		*/
		std::wstring GetNodeName(int nodeId) const
		{
			return GetNode(nodeId)->GetName();
		}

		/** 
		* @brief Set name of node.
		* @param nodeId Id of node of interest.
		* @param name Desired name of node.
		*/
		void SetNodeName(int nodeId,  const std::wstring & name)
		{
			GetNode(nodeId)->SetName(name);
		}

		/** 
		* @brief Indicate if tree has a node with a given id.
		* @param nodeId Id to check.
		* @return True if tree has a node with the given id.
		*/
		bool HasNode(unsigned int nodeId) const
		{
			return TreeTools<N>::HasNodeWithId(m_root, nodeId);
		}

		/** 
		* @brief Indicate if node is a leaf.
		* @param nodeId Id of node of interest.
		* @return True if node is a leaf.
		*/
		bool IsLeaf(unsigned int nodeId) const
		{
			return GetNode(nodeId)->IsLeaf();
		}

		/** 
		* @brief Indicate if node is the root.
		* @param nodeId Id of node of interest.
		* @return True if node is the root.
		*/
		bool IsRoot(unsigned int nodeId) const
		{
			return TreeTools<N>::IsRoot(GetNode(nodeId));
		}

		/** 
		* @brief Get distance to parent node for the specified node.
		* @param nodeId Id of node of interest.
		* @return Distance to parent node.
		*/
		float GetDistanceToParent(unsigned int nodeId) const
		{
			return GetNode(nodeId)->GetDistanceToParent();
		}

		/** 
		* @brief Set distance to parent node from the specified node.
		* @param nodeId Id of node of interest.
		* @param dist Distance to parent node.
		*/
		void SetDistanceToParent(unsigned int nodeId, float dist)
		{
			GetNode(nodeId)->SetDistanceToParent(dist);
		}

		/** 
		* @brief Reset node ids so they are all unique values.
		* @param nodeId Id of node of interest.
		* @param dist Distance to parent node.
		*/
		void ResetNodeIds();

		/** Indicate if tree is multifurcating. */
		bool IsMultifurcating() const;

		/** Get total length of all branches in tree. */
		float GetTotalLength() const;

		/** 
		* @brief Set all branches to the specified length.
		* @param length Desired length.
		*/
		void SetBranchLengths(float length);

		/**
		* @brief Scale tree distances by a constant factor.
		* @param factor Amount to scale tree distances by.
		*/
		void ScaleTree(float factor);

	protected:		
		void DestroySubtree(N* node);

	protected:
		N* m_root;
		std::wstring m_name;

	private:
		/** Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & m_root;
			ar & m_name;
		}
	};

	template <class N>
	Tree<N>::Tree(const Tree<N>& rhs): m_root(NULL), m_name(rhs.GetName())
	{
		//Perform a hard copy of the nodes:
		m_root = TreeTools<N>::CloneSubtree(rhs.GetRootNode());
	}

	template <class N>
	Tree<N>& Tree<N>::operator=(const Tree<N>& rhs)
	{
		if(this == &rhs)
			return *this;

		// Free memory allocated to LHS tree
		if(m_root) 
		{ 
			DestroySubtree(m_root); 
			delete m_root; 
		}

		// Perform hard copy of nodes on RHS
		m_root = TreeTools<N>::CloneSubtree(rhs.GetRootNode());
		m_name = rhs.m_name;

		return *this;
	}

	template <class N>
	Tree<N>::~Tree()
	{
		if(m_root)
		{
			DestroySubtree(m_root);
			delete m_root;
		}
	}

	template <class N>
	void Tree<N>::DestroySubtree(N* node)
	{
		for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
		{
			N* child = node->GetChild(i);
			DestroySubtree(child);

			delete child;
		}
	}

	template <class N>
	N* Tree<N>::GetNode(unsigned int id) const
	{
		std::vector<N*> nodes = TreeTools<N>::SearchNodeWithId(m_root, id);

		if(nodes.size() > 1)
		{
			Log::Inst().Error("Node::GetNode(): multiple nodes have the same id.");
			return NULL;
		}
		else if(nodes.size() == 0)
		{
			Log::Inst().Error("Node::GetNode(): no nodes with the provided id.");
			return NULL;
		}

		return nodes[0];
	}

	template <class N>
	N* Tree<N>::GetNode(const std::wstring& name) const
	{
		std::vector<N*> nodes = TreeTools<N>::SearchNodeWithName(m_root, name);

		if(nodes.size() > 1)
		{
			std::wstring warning = _T("Node::GetNode(): multiple nodes have id = "); 
			Log::Inst().Warning(warning + name);
			return NULL;
		}
		else if(nodes.size() == 0)
		{
			std::wstring warning = _T("Node::GetNode(): no node with id = "); 
			Log::Inst().Warning(warning + name);
			return NULL;
		}

		return nodes[0];
	}

	template <class N>
	void Tree<N>::ResetNodeIds()
	{
		std::vector<N*> nodes = GetNodes();
		for(unsigned int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->SetId(i);
		}
	}

	template <class N>
	bool Tree<N>::IsMultifurcating() const
	{
		bool b = false;
		for(unsigned int i = 0; i < m_root->GetNumberOfChildren(); i++)
		{
			b = b || TreeTools<N>::IsMultifurcating(m_root->GetChild(i));
		}
		return b;
	}

	template <class N>
	float Tree<N>::GetTotalLength() const
	{
		float length = 0;
		for(unsigned int i = 0; i < m_root->GetNumberOfChildren(); i++)
		{
			length += TreeTools<N>::GetTotalLength(m_root->GetChild(i));
		}
		return length;
	}

	template <class N>
	void Tree<N>::SetBranchLengths(float length)
	{
		for(unsigned int i = 0; i < m_root->GetNumberOfChildren(); i++)
		{
			TreeTools<N>::SetBranchLengths(m_root->GetChild(i), length);
		}
	}

	template <class N>
	void Tree<N>::ScaleTree(float factor)
	{
		for(unsigned int i = 0; i < m_root->GetNumberOfChildren(); i++)
		{
			TreeTools<N>::ScaleTree(m_root->GetChild(i), factor);
		}
	}

}

#endif
