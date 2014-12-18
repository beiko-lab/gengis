//=======================================================================
// Author: Alexander Keddy
//
// Copyright 2014 Alexander Keddy
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
//
//
// Based on algorithms presented in: 

#include "../core/Precompiled.hpp"
#include "../red-black/PokeTree.h"


using namespace GenGIS;

PokeTree::PokeTree()
{
//	head = new PokeNode();
//	head->left = theLast;
//	head->right = theLast;
//	head->preferred = theLast;

//	theLast = new PokeNode();
//	theLast->left = NULL;
//	theLast->right = NULL;
//	theLast->preferred = NULL;

	locCount = 0;
	head = NULL;
}

void PokeTree::Insert(TileModelPtr key, LocationLayerPtr curLoc, Point2D locPoint)
{
	if( head != NULL )
		Insert( key, curLoc, locPoint,  head);
	else
	{
		head = new PokeNode(key,NULL,NULL,NULL,NULL);
		locCount++;
	}
}
/*
void PokeTree::Insert(Point2D key)
{
	if( head != NULL )
		Insert( key, head );
	else
	{
		PokeNode head(key,NULL,NULL,NULL);
	}
}
*/
// I think search might actually work off of a Point2D or something
TileModelPtr PokeTree::Search(TileModelPtr key)
{
	return Search(key, head);
}

//TileModelPtr PokeTree::Search(Point2D key, TileModelPtr tile)
TileModelPtr PokeTree::Search(Point2D key)
{
	if( head )	
		return Search(key, head);
	else
	{
		TileModelPtr t;
		return t;
	}
}

PokeNode *PokeTree::SearchNode(Point2D key)
{
	if( head )	
		return SearchNode(key, head);
	else
	{
	//	PokeNode *t = new PokeNode();
	//	return t;
		return NULL;
	}
}

void PokeTree::DestroyTree()
{
	if( head )
		DestroyTree(head);
		head = NULL;
}

void PokeTree::DestroyTree(PokeNode *leaf)
{
	if (leaf!=NULL)
	{
		DestroyTree(leaf->left);
		DestroyTree(leaf->right);
		delete leaf;
	}
}

// Recursive insertion method. Leaf is start as root
/*
void PokeTree::Insert(TileModelPtr key, LocationLayerPtr curLoc, PokeNode *leaf)
{
	// if key left child
//	if( leaf->tile->Compare( key->GetTopLeft() ) < 0 )
	int comp = leaf->tile->Compare( key );
	if( comp == 0 )
	{
		leaf->tile->AddLocationLayer( curLoc );
	}
	// not in, lets try the dynamic link!
	else
	{
		PokeNode *dyn = leaf->preferred;
		Point2D min = dyn->min;
		Point2D max = dyn->max;
		// if key.TL >= min and key.BR <= max
		//	use dynamic link
		if( ( key->Compare(min)>=0 ) && (key->Compare(max) <= 0 ) )
		{
			Insert(key,curLoc, dyn);
			// NOW NEED TO PERFORM CLEANUP!!!
		}
		// else try the static links
		else if( comp < 0 )
		{	
			if(leaf->left!=NULL)
				Insert(key, curLoc, leaf->left);
			else
			{
				leaf->left= new PokeNode(key,NULL,NULL,NULL);
				// NOW NEED TO PERFORM CLEANUP
			}
		}
		// if right child
		else if ( comp > 0 )
		{
			if(leaf->right!=NULL)			
				Insert(key, curLoc, leaf->right);
			else
			{
				leaf->right = new PokeNode(key,NULL,NULL,NULL);
				// NOW NEED TO PERFORM CLEANUP
			}
		}
	}
}
*/

void PokeTree::Insert(TileModelPtr key, LocationLayerPtr curLoc, Point2D locPoint, PokeNode *leaf)
{
	PokeNode* parentPtr = head; // points to psuedo root
	PokeNode* grandPtr = head; // points to psuedo root
	PokeNode* greatPtr = head; // points to psuedo root
	// node is always not the head!
	int nodeDepth = 2;
	// if key left child
	while (leaf != NULL )
	{
		
		int comp = leaf->tile->Compare( locPoint );
		if( comp == 0 )
		{
			leaf->tile->AddLocationLayer( curLoc );
			locCount++;
			break;
		}
		// not in, lets try the dynamic link!
		else
		{
			// Dynamic stuff isn't ready!
			PokeNode *dyn = leaf->dynamic;
			if( dyn != NULL )
			{
				Point2D min = dyn->min;
				Point2D max = dyn->max;
			// if key.TL >= min and key.BR <= max
			//	use dynamic link
				if( ( key->Compare(min)>=0 ) && (key->Compare(max) <= 0 ) )
				{
					leaf = dyn;
					// dynamic link worked so recalculate the compare
					comp = leaf->tile->Compare( locPoint );
					if( comp == 0 )
					{
						leaf->tile->AddLocationLayer( curLoc );
						locCount++;
						break;
					}
				}
			}
		// else try the static links
		//	else if( comp < 0 )
			if( comp < 0 )
			{	
				if(leaf->left!=NULL)
				{
					leaf->preferred = leaf->left;
					leaf = leaf->left;
					nodeDepth++;
				}				
					//Insert(key, curLoc, leaf->left);
				else
				{
					PokeNode * n = new PokeNode(key,NULL,NULL,NULL,NULL,leaf);
					leaf->left= n;
					leaf->preferred = n;
					leaf->dynamic = n;
					n->depth = nodeDepth;
					locCount++;
					UpdateMinMax(n);
					UpdatePokeTree();
					// Update Dynamic pointers and min/max
					break;
				}
			}
			// if right child
			else if ( comp > 0 )
			{
				if(leaf->right!=NULL)
				{
					leaf->preferred = leaf->right;
					leaf = leaf->right;
					nodeDepth++;
				}				
					//	Insert(key, curLoc, leaf->right);
				else
				{
					PokeNode * n = new PokeNode(key,NULL,NULL,NULL,NULL, leaf);
					n->depth = nodeDepth;
					leaf->right= n;
					leaf->preferred = n;
					leaf->dynamic = n;
					locCount++;
					UpdateMinMax(n);
					UpdatePokeTree();
					// Update Dynamic pointers and min/max
					break;
				}
			}
		}
		// cleanup
		// THIS KEEPS THE RED BLACK INVARIANTS GOOD
		if( leaf->left != NULL && leaf->right != NULL)
		{
			if((leaf->left->color == Red) && (leaf->right->color == Red)) // if a 4node (both children are red)
			{
				 split(leaf, parentPtr, grandPtr, greatPtr); // split them
				 head->color = Black; // recolor psuedo root
				 head->left->color = Black; // recolor real root
			//	 theLast->color = Black; // recolor theleaf
			}
		}
		greatPtr = grandPtr;
		grandPtr = parentPtr;
		parentPtr = leaf;
	}
}

// travel down preferred path from head and to leaf -> preferred
// then bottom up fix dynamic links
void PokeTree::UpdatePokeTree()
{
	PokeNode *leaf = head;
	// Successor of leaf
	PokeNode *s = head;
	// Successor of Successor of leaf
	PokeNode *ss = head;
	int sdLength=0;
	int sddLength=0;

	//travel to leaf
	while ( leaf->preferred != NULL )
		leaf = leaf->preferred;


	while(leaf != NULL )
	{
		if( leaf->dynamic != NULL )
		{
			s = leaf->dynamic;
			if( s->dynamic != NULL )
			{
				ss = s->dynamic;
				//Do length comparison
				sdLength = s->depth - leaf->depth;
				sddLength = ss->depth - s->depth;
				if( sdLength == sddLength )
					leaf->dynamic = ss->dynamic;
				else
					leaf->dynamic = leaf->preferred;
			}
			else
			{
				leaf->dynamic = leaf->preferred;
			}
		}
		else
			leaf->dynamic = leaf->preferred;
		leaf = leaf->parent;
	}
	
}

Point2D PokeTree::PokeMin(Point2D l, Point2D r)
{
	if( l.y > r.y )
		return l;
	else if( l.y < r.y )
		return r;
	else if ( l.y == r.y )
	{
		if ( l.x < r.x )
			return l;
		else
			return r;
	}
}

Point2D PokeTree::PokeMax(Point2D l, Point2D r)
{
	if( l.y > r.y )
		return r;
	else if( l.y < r.y )
		return l;
	else if ( l.y == r.y )
	{
		if ( l.x < r.x )
			return r;
		else
			return l;
	}
}

// Propogate Up through tree updating min and max
void PokeTree::UpdateMinMax(PokeNode *leaf)
{
	Point2D min = leaf->min;
	Point2D max = leaf->max;
	Point2D tempMin = leaf->min;
	Point2D tempMax = leaf->max;
	bool keepGoing = false;

	leaf = leaf->parent;

	while( leaf!=NULL )
	{
		tempMin = PokeMin( leaf->min, min );
		tempMax = PokeMax( leaf->max, max );
		if( tempMin == leaf->min && tempMax == leaf->max )
			break;
		if( tempMin == min )
		{
			leaf->min == min;
			keepGoing = true;
		}
		if( tempMax == max )
		{
			leaf->max = max; 
			keepGoing = true;
		}
		if ( keepGoing )
		{
			leaf = leaf->parent;
			keepGoing = false;
		}

	}
}

// Recursive insertion method. Leaf is start as root
/*
void PokeTree::Insert(Point2D key, PokeNode *leaf)
{
	if ( leaf->tile->Compare( key ) == 0 )
	{
		return leaf->tile;
	}
	// if key left child
	else if( leaf->tile->Compare( key ) < 0 )
	{	
		if(leaf->left!=NULL)
			Insert(key, leaf->left);
	}
	// if right child
	else if ( leaf->tile->Compare( key ) > 0 )
	{
		if(leaf->right!=NULL)
			Insert(key,leaf->right);
	}
}
*/
TileModelPtr PokeTree::Search(TileModelPtr key, PokeNode *leaf)
{
	// create null TileModelPtr
	TileModelPtr res;
	if( leaf->tile->Compare(key) == 0 )
		return leaf->tile;
	// if key left child
	else if( leaf->tile->Compare( key ) < 0 )
	{	
		if(leaf->left!=NULL)
			return Search(key, leaf->left);
		else
		{
			return res;		
		}
	}
	// if right child
	else if ( leaf->tile->Compare( key ) > 0 )
	{
		if(leaf->right!=NULL)
			return Search(key,leaf->right);
		else
			return res;
	}
	// err not found
	return res;

}

//TileModelPtr PokeTree::Search(Point2D key, PokeNode *leaf, TileModelPtr tile)
TileModelPtr PokeTree::Search(Point2D key, PokeNode *leaf)
{
	TileModelPtr res;	
	/*
	* If the tile contians the key, or has no children ( is a leaf )
	*/

//	bool test = leaf->tile > key;
	if( leaf->tile->Compare(key) == 0 || (leaf->left == NULL && leaf->right == NULL) )
	{
		return leaf->tile;
	}	
	// if key left child
	else if( leaf->tile->Compare( key ) < 0 )
	{	
		if(leaf->left!=NULL)
			return Search(key, leaf->left);
		else
		{
			return res;	
		}
	}
	// if right child
	else if ( leaf->tile->Compare( key ) > 0 )
	{
		if(leaf->right!=NULL)
			return Search(key,leaf->right);
		else
			return res;
	}
	return res;
}

void PokeTree::Combine()
{
	Combine( head );
}

//use combination method on every tile
void PokeTree::Combine(PokeNode *leaf)
{
	if( leaf != NULL )
	{
		if( leaf->tile->GetNumLocations() > 0 )
		{
			leaf->tile->CombineData();
			//combine sequence layers if there are any
			if(leaf->tile->GetNumSequence() > 0)
				leaf->tile->CombineSequenceData();
		}
	Combine( leaf->right );
	Combine( leaf->left );
	}
}

bool PokeTree::IsHead()
{
	if(head)
		return 1;
	else
		return 0;


}

void PokeTree::SetColour(TileModelPtr tile, Colour colour)
{
	TileModelPtr cur = Search(tile);
	cur->SetColour(colour);
}

void PokeTree::SetHead(TileModelPtr t)
{
	PokeNode *node = new PokeNode(t,NULL,NULL,NULL,NULL);
	head = node;

}

PokeNode *PokeTree::SearchNode(Point2D locPoint, PokeNode *leaf)
{

	//PokeNode *res;	
	/*
	* If the tile contians the key, or has no children ( is a leaf )
	*/
	if( leaf->tile->Compare(locPoint) == 0 || (leaf->left == NULL && leaf->right == NULL) )
	{
		return leaf;
	}	
	// if key left child
	else if( leaf->tile->Compare( locPoint ) < 0 )
	{	
		if(leaf->left!=NULL)
			return SearchNode(locPoint, leaf->left);
		else
		{
			return leaf;	
		}
	}
	// if right child
	else if ( leaf->tile->Compare( locPoint ) > 0 )
	{
		if(leaf->right!=NULL)
			return SearchNode(locPoint,leaf->right);
		else
			return leaf;
	}
	return NULL;
}

// swaps color, used in the split method
void PokeTree::swapColor(PokeNode* nodePtr, PokeNode* parentPtr, PokeNode* grandPtr)
 {
	 if(nodePtr->color == Black) // recolor node, parent, and grand
		nodePtr->color = Red;
	 else if(nodePtr->color == Red)
		nodePtr->color = Black;
	 if(parentPtr->color == Black)
		parentPtr->color = Red;
	 else if(parentPtr->color == Red)
		parentPtr->color = Black;
	 if(grandPtr->color == Black)
		grandPtr->color = Red;
	 else if(grandPtr->color == Red)
		grandPtr->color = Black;
 }
 
 // Perform rightRotate
 // rotate the nodePtr around its left child
 // parentPtr points to the parent of nodePtr
void PokeTree::rightRotate( PokeNode* nodePtr, PokeNode* parentPtr )
 {
	PokeNode* temp = nodePtr->left; // temp var to hold the left child
	nodePtr->left = temp->right; // set left to right
 
	if (parentPtr->right == nodePtr) // if were dealing with the right child
	{
		parentPtr->right = temp; // set right child to temp
	}
	else // else if were dealing with the left child
	{
		parentPtr->left = temp; // set left to temp
	}
 
	temp->right = nodePtr; // finish up the rotate
 }
 // Perform leftRotate
 // rotate the nodePtr around its right child
 // parentPtr points to the parent of nodePtr
void PokeTree::leftRotate( PokeNode* nodePtr, PokeNode* parentPtr )
 {
	PokeNode* temp = nodePtr->right; // temp var to hold the right child
	nodePtr->right = temp->left; // set right to left
 
	if (parentPtr->right == nodePtr) // if were dealing with the right child
	{
		parentPtr->right = temp; // set it to right
	}
	else // if were dealing with the left child
	{
		parentPtr->left = temp; // set it to the left
	}
 
	temp->left = nodePtr; // finish the rotation
 }
 
 // split a 4-node: nodePtr
 // parentPtr points to the parent of nodePtr
 // grandPtr points to the parent of parentPtr
 // greatPtr points to the parent of grandPtr
void PokeTree::split ( PokeNode* nodePtr,  PokeNode* parentPtr, PokeNode* grandPtr, PokeNode* greatPtr )
 {
	nodePtr->right->color = Black; // recolor these as black
	nodePtr->left->color = Black;
 
	if(parentPtr->color == Black) // if a 2 node
	{
		nodePtr->color = Red; // recolor
	}
	else
	{
		if(parentPtr->left->color != parentPtr->right->color) //if colors are different
		{
			nodePtr->color = Red; // set to RBTreeNode<Comparable>::Red
		}
		else if((parentPtr->left == nodePtr) && (grandPtr->left == parentPtr)) // if nodePtr is left child of its parent and
		{                                                                            // parentPtr is left child of its parent
			rightRotate(grandPtr, greatPtr); // rotate
	 
			swapColor(nodePtr, parentPtr, grandPtr); // swap the colors of nodePtr, parentPtr, and grandPtr
		}
		else if((parentPtr->left == nodePtr) && (grandPtr->right == parentPtr))
		{
			rightRotate(parentPtr, grandPtr); // rotate
			leftRotate(grandPtr, greatPtr);
	 
			swapColor(nodePtr, parentPtr, grandPtr); // swap the colors of nodePtr, parentPtr, and grandPtr
		}
		else if((parentPtr->right == nodePtr) && (grandPtr->left == parentPtr))
		{
			leftRotate(parentPtr, grandPtr); // rotate
			rightRotate(grandPtr, greatPtr);
	 
			swapColor(nodePtr, parentPtr, grandPtr); // swap the colors of nodePtr, parentPtr, and grandPtr
		}
		else if((parentPtr->right == nodePtr) && (grandPtr->right == parentPtr))
		{
			leftRotate(grandPtr, greatPtr); // rotate
	 
			swapColor(nodePtr, parentPtr, grandPtr); // swap the colors of nodePtr, parentPtr, and grandPtr
		}
	}
 }