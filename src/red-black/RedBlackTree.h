	// Taken From http://larryvdh.com/blog/red-black-tree-implementation/

	//// Author: Larry VanDenHandel
	//// Purpose: Manage a red-black tree structure
	//// RedBlackTree.h
	
#ifndef _GENGIS_RED_BLACK_TREE_
#define _GENGIS_RED_BLACK_TREE_

#include "../core/Precompiled.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/TileModel.hpp"
#pragma once
 
namespace GenGIS
{
	// class prototype
	template <class Comparable>
	class RedBlackTree;
	// NodeType must be a class/type that has default constructor
	// and overloads < operator
	
	/*
	template <class Comparable>
	class RBTreeNode
	{
	public:
	 enum NodeColor {Red, Black};
	 
	 Comparable  value;
	 RBTreeNode  *lChildPtr;
	 RBTreeNode  *rChildPtr;
//	 RBTreeNode	 *pChildPtr;
	 NodeColor   color;
	 // biggest bottomRigth value
//	 Point2D	 max;
	 // smallest topLeft value
//	 Point2D	 min;
	 
	 //constructor
	 RBTreeNode( Comparable v=Comparable(), RBTreeNode *l=NULL,
	 RBTreeNode *r=NULL, NodeColor c=Black)
	 : value(v), color(c), lChildPtr(l), rChildPtr(r)
	 {}
	 
	 friend class RedBlackTree<Comparable>;
	};
	*/
	template<class Comparable>
	class RBTreeNode
	{
	public:
		enum NodeColor {Red,Black};
		Comparable	value;
		RBTreeNode	*lChildPtr;
		RBTreeNode	*rChildPtr;
		RBTreeNode	*pChildPtr;
		NodeColor	color;
		// biggest bottomRigth value
		 Point2D	 max;
		 // smallest topLeft value
		 Point2D	 min;
	 
		 RBTreeNode( Comparable v =Comparable(), RBTreeNode *l=NULL,
		 RBTreeNode *r=NULL, RBTreeNode*p=NULL, NodeColor c=Black)
		 : value(v), lChildPtr(l), rChildPtr(r), pChildPtr(p), color(c)
		 {
			max = Point2D(-180,-90);
			min = Point2D(180,90);
		 }
		 friend class RedBlackTree<Comparable>;
	};

	template <class Comparable>
	class RedBlackTree
	{
	public:
	 // constructor create an empty red-black tree
	 RedBlackTree(void);
	 
	 // copy constructor for deep copy
	 RedBlackTree( const RedBlackTree<Comparable>& old );
	 
	 // assignment operator
	 const RedBlackTree<Comparable>&
	 operator =( const RedBlackTree<Comparable>& rhs );
	 
	 // destructor to release memory space, including space for theLeaf and
	 // pseudo root.
	 ~RedBlackTree();

	 void DestroyTree(RBTreeNode<Comparable>*& start);
	 
	 // insert a value to red-black tree. This function
	 // throw a bad_alloc exception if no enough space
	// void insert( Comparable newItem, LocationLayerPtr locLayer );
	 void insert( Comparable newItem );
	 
	 // print the whole tree
	 void print( void ) const;
	 
	 RBTreeNode<Comparable>* GetRoot(){ return m_root;}
	private:
	 
	 // swaps color, used in the split method
	 void swapColor(RBTreeNode<Comparable>*& nodePtr, RBTreeNode<Comparable>*& parentPtr, RBTreeNode<Comparable>*& grandPtr);
	 
	 // Perform rightRotate
	 // rotate the nodePtr around its left child
	 // parentPtr points to the parent of nodePtr
	 void rightRotate( RBTreeNode<Comparable>*& nodePtr,
	 RBTreeNode<Comparable>*& parentPtr );
	 
	 // Perform leftRotate
	 // rotate the nodePtr around its right child
	 // parentPtr points to the parent of nodePtr
	 void leftRotate( RBTreeNode<Comparable>*& nodePtr,
	 RBTreeNode<Comparable>*& parentPtr );
	 
	 // split a 4-node: nodePtr
	 // parentPtr points to the parent of nodePtr
	 // grandPtr points to the parent of parentPtr
	 // greatPtr points to the parent of grandPtr
	 void split ( RBTreeNode<Comparable>* nodePtr,
	 RBTreeNode<Comparable>* parentPtr,
	 RBTreeNode<Comparable>* grandPtr,
	 RBTreeNode<Comparable>* greatPtr );
	 
	 // deep copy a tree pointed by nodePtr
	 RBTreeNode<Comparable> * clone( RBTreeNode<Comparable> * nodePtr ) const;
	 
	 // release memory of the tree, but don't release pseudo root and theLeaf
	 void reclaimMemory( RBTreeNode<Comparable> *nodePtr ) const;
	 
	 // print the tree rooted at nodePtr
	 // The parameter level specifies the level of the nodePtr in the tree.
	 // You need to use this parameter to adjust indentation.
	 void print( RBTreeNode<Comparable> *nodePtr, int level ) const;

	 
	 
	private:
	 RBTreeNode<Comparable>*     theLeaf;    // actual leaf node
	 RBTreeNode<Comparable>*     m_root;     // pointer to pseudo root
	};
}

#endif 