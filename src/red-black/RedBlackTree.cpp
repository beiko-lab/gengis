//// Author: Larry VanDenHandel
//// Purpose: Manage a red-black tree structure
//// RedBlackTree.cpp
#include "../core/Precompiled.hpp"
#include "../red-black/RedBlackTree.h"
#include "../core/TileModel.hpp"
 
using namespace GenGIS;
const int MinValue = -999999-10+10;
 
// method to swap the colors of nodeptr, parentptr, and grandptr. Reduces redundant code within split()
template <class Comparable>
void RedBlackTree<Comparable>::swapColor(RBTreeNode<Comparable>*& nodePtr, RBTreeNode<Comparable>*& parentPtr, RBTreeNode<Comparable>*& grandPtr)
{
 if(nodePtr->color == RBTreeNode<Comparable>::Black) // recolor node, parent, and grand
	nodePtr->color = RBTreeNode<Comparable>::Red;
 else if(nodePtr->color == RBTreeNode<Comparable>::Red)
	nodePtr->color = RBTreeNode<Comparable>::Black;
 if(parentPtr->color == RBTreeNode<Comparable>::Black)
	parentPtr->color = RBTreeNode<Comparable>::Red;
 else if(parentPtr->color == RBTreeNode<Comparable>::Red)
	parentPtr->color = RBTreeNode<Comparable>::Black;
 if(grandPtr->color == RBTreeNode<Comparable>::Black)
	grandPtr->color = RBTreeNode<Comparable>::Red;
 else if(grandPtr->color == RBTreeNode<Comparable>::Red)
	grandPtr->color = RBTreeNode<Comparable>::Black;
}
 
// constructor create an empty red-black tree
template <class Comparable>
RedBlackTree<Comparable>::RedBlackTree(void)
{
 theLeaf = new RBTreeNode<Comparable>; // create a new leaf
 theLeaf->lChildPtr = theLeaf->rChildPtr = NULL;
 theLeaf->color = RBTreeNode<Comparable>::Black;
 
 m_root = new RBTreeNode<Comparable>();   // pseudo root
 m_root->lChildPtr = m_root->rChildPtr = theLeaf;
 m_root->color = RBTreeNode<Comparable>::Black;
 m_root->value = MinValue; // initialize
}
 
// copy constructor for deep copy
template <class Comparable>
RedBlackTree<Comparable>::RedBlackTree( const RedBlackTree<Comparable>& old )
{
 
 nullNode = new RBTreeNode<Comparable>; // new node for the leaf
 nullNode->lChildPtr = nullNode->rChildPtr = nullNode->pChildPtr = nullNode;
 m_root = new RedBlackNode<Comparable>; // create the new node
 m_root->lChildPtr = m_root->rChildPtr = m_root->pChildPtr =  nullNode;
 *this = old; // set the new node to our old parameter
 
}
 
template <class Comparable>
const RedBlackTree<Comparable> &
RedBlackTree<Comparable>::operator=( const RedBlackTree<Comparable> & rhs )
{
 
 if( this != &rhs ) // if not self assignment
 {
 reclaimMemory( m_root->rChildPtr ); // clear its memory
 m_root->rChildPtr = theLeaf;
 m_root->rChildPtr = clone( rhs.m_root->rChildPtr );
 }
 
 return *this; // return current tree object
 
}
 
// destructor to release memory space
template <class Comparable>
RedBlackTree<Comparable>::~RedBlackTree()
{
 reclaimMemory(m_root); // delete the tree
 delete theLeaf; // delete the leaf
 delete m_root; // delete the root
}
 
// insert a value to red-black tree. This function may
// throw a bad_alloc exception if no enough space
template <class Comparable>
//void RedBlackTree<Comparable>::insert( Comparable newItem, LocationLayerPtr locLayer )
void RedBlackTree<Comparable>::insert( Comparable newItem )
{
 // Need to add an equal case, in which I insert the tiles data into the existing tile!!!
 // Need to add use of dynamic pointer

 RBTreeNode<Comparable>* nodePtr = m_root->rChildPtr; // points to the real root
 RBTreeNode<Comparable>* parentPtr = m_root; // points to psuedo root
 RBTreeNode<Comparable>* grandPtr = m_root; // points to psuedo root
 RBTreeNode<Comparable>* greatPtr = m_root; // points to psuedo root
 
 while(nodePtr != theLeaf)
 {
	if((nodePtr->lChildPtr->color == RBTreeNode<Comparable>::Red) && (nodePtr->rChildPtr->color == RBTreeNode<Comparable>::Red)) // if a 4node (both children are red)
	{
		 split(nodePtr, parentPtr, grandPtr, greatPtr); // split them
		 m_root->color = RBTreeNode<Comparable>::Black; // recolor psuedo root
		 m_root->rChildPtr->color = RBTreeNode<Comparable>::Black; // recolor real root
		 theLeaf->color = RBTreeNode<Comparable>::Black; // recolor theleaf
	}
 
	 greatPtr = grandPtr;
	 grandPtr = parentPtr;
	 parentPtr = nodePtr;
	 
	 if (newItem <= parentPtr->value) // check if smaller
	 {
		nodePtr = nodePtr->lChildPtr; // not sure if this is right
  	}
	else
	{
		nodePtr = nodePtr->rChildPtr; // not sure if this is right
	}
 
 }
 
/*	if (newItem == parentPtr->value)
	{
		// insert tile data into parentPtr->value data
		parentPtr->value->AddLocationLayer(locLayer);
	}
	else*/ if (newItem <= parentPtr->value) // check if smaller, place left
	{
		parentPtr->lChildPtr = new RBTreeNode<Comparable>(newItem, theLeaf, theLeaf, RBTreeNode<Comparable>::Black);
		nodePtr = parentPtr->lChildPtr;
	}
	else // else place it right
	{
		parentPtr->rChildPtr = new RBTreeNode<Comparable>(newItem, theLeaf, theLeaf, RBTreeNode<Comparable>::Black);
		nodePtr = parentPtr->rChildPtr;
	}
 
	split(nodePtr, parentPtr, grandPtr, greatPtr); // split the node
	m_root->color = RBTreeNode<Comparable>::Black; // recolor psuedo root
	m_root->rChildPtr->color = RBTreeNode<Comparable>::Black; // recolor real root
	theLeaf->color = RBTreeNode<Comparable>::Black; // recolor theleaf
 
}
 
template <class Comparable>
void RedBlackTree<Comparable>::print( void ) const
{
	print(m_root->rChildPtr, 0); // print
}
 
// Perform rightRotate
// Rotate the node around its left child
template <class Comparable>
void RedBlackTree<Comparable>::rightRotate( RBTreeNode<Comparable>*& nodePtr,
 RBTreeNode<Comparable>*& parentPtr )
{
 
	RBTreeNode<Comparable>* temp = nodePtr->lChildPtr; // temp var to hold the left child
	nodePtr->lChildPtr = temp->rChildPtr; // set left to right
 
	if (parentPtr->rChildPtr == nodePtr) // if were dealing with the right child
	{
		parentPtr->rChildPtr = temp; // set right child to temp
	}
	else // else if were dealing with the left child
	{
		parentPtr->lChildPtr = temp; // set left to temp
	}
 
 temp->rChildPtr = nodePtr; // finish up the rotate
 
}
 
// Perform leftRotate
// Rotate the nodePtr around its right child
template <class Comparable>
void RedBlackTree<Comparable>::leftRotate( RBTreeNode<Comparable>*& nodePtr,
 RBTreeNode<Comparable>*& parentPtr )
{
 
	RBTreeNode<Comparable>* temp = nodePtr->rChildPtr; // temp var to hold the right child
	nodePtr->rChildPtr = temp->lChildPtr; // set right to left
 
	if (parentPtr->rChildPtr == nodePtr) // if were dealing with the right child
	{
		parentPtr->rChildPtr = temp; // set it to right
	}
	else // if were dealing with the left child
	{
		parentPtr->lChildPtr = temp; // set it to the left
	}
 
	temp->lChildPtr = nodePtr; // finish the rotation
 
}
 
// split a 4-node:
template <class Comparable>
void RedBlackTree<Comparable>::split (  RBTreeNode<Comparable>* nodePtr,
 RBTreeNode<Comparable>* parentPtr,
 RBTreeNode<Comparable>* grandPtr,
 RBTreeNode<Comparable>* greatPtr )
{
 
	nodePtr->rChildPtr->color = RBTreeNode<Comparable>::Black; // recolor these as black
	nodePtr->lChildPtr->color = RBTreeNode<Comparable>::Black;
 
	if(parentPtr->color == RBTreeNode<Comparable>::Black) // if a 2 node
	{
		nodePtr->color = RBTreeNode<Comparable>::Red; // recolor
	}
	else
	{
		if(parentPtr->lChildPtr->color != parentPtr->rChildPtr->color) //if colors are different
		{
			nodePtr->color = RBTreeNode<Comparable>::Red; // set to RBTreeNode<Comparable>::Red
		}
		else if((parentPtr->lChildPtr == nodePtr) && (grandPtr->lChildPtr == parentPtr)) // if nodePtr is left child of its parent and
		{                                                                            // parentPtr is left child of its parent
			rightRotate(grandPtr, greatPtr); // rotate
	 
			swapColor(nodePtr, parentPtr, grandPtr); // swap the colors of nodePtr, parentPtr, and grandPtr
		}
		else if((parentPtr->lChildPtr == nodePtr) && (grandPtr->rChildPtr == parentPtr))
		{
			rightRotate(parentPtr, grandPtr); // rotate
			leftRotate(grandPtr, greatPtr);
	 
			swapColor(nodePtr, parentPtr, grandPtr); // swap the colors of nodePtr, parentPtr, and grandPtr
		}
		else if((parentPtr->rChildPtr == nodePtr) && (grandPtr->lChildPtr == parentPtr))
		{
			leftRotate(parentPtr, grandPtr); // rotate
			rightRotate(grandPtr, greatPtr);
	 
			swapColor(nodePtr, parentPtr, grandPtr); // swap the colors of nodePtr, parentPtr, and grandPtr
		}
		else if((parentPtr->rChildPtr == nodePtr) && (grandPtr->rChildPtr == parentPtr))
		{
			leftRotate(grandPtr, greatPtr); // rotate
	 
			swapColor(nodePtr, parentPtr, grandPtr); // swap the colors of nodePtr, parentPtr, and grandPtr
		}
	}
 
}
 
template <class Comparable>
RBTreeNode<Comparable> *
RedBlackTree<Comparable>::clone( RBTreeNode<Comparable> * nodePtr ) const
{
 
	if( nodePtr == nodePtr->lChildPtr )  // if points to special leaf node
		return theLeaf;
	else
		return new RedBlackNode<Comparable>( nodePtr->value, clone( nodePtr->lChildPtr ), clone( nodePtr->rChildPtr ), nodePtr->color); // recursive function calls
 
}
 
template <class Comparable>
void RedBlackTree<Comparable>::reclaimMemory( RBTreeNode<Comparable> *nodePtr ) const
{
 
 if ( nodePtr != theLeaf ) // if not the leaf
 {
	reclaimMemory(nodePtr->lChildPtr); // destroy left subtree
	reclaimMemory(nodePtr->rChildPtr); // destroy right subtree
	delete nodePtr; // destroy the node
 }
 
}

template<class Comparable>
void RedBlackTree<Comparable>::DestroyTree(RBTreeNode<Comparable>*& start)
{
	if (start!=NULL)
	{
		DestroyTree(leaf->lChildPtr);
		DestroyTree(leaf->rChildPtr);
		delete start;
	}
}
 
// print the tree rooted at nodePtr
template <class Comparable>
void RedBlackTree<Comparable>::print( RBTreeNode<Comparable> *nodePtr, int level ) const
{
 
 if ( nodePtr != theLeaf )
 {
	print( nodePtr->rChildPtr, level+1 );
	for( int i=0; i<level; i++ )
		cout << "    ";
		cout << ((nodePtr->color==RBTreeNode<Comparable>::Black) ? "b[" : "r[")
		<< nodePtr->value << "]" << endl;
		print( nodePtr->lChildPtr, level+1 );
	}
}