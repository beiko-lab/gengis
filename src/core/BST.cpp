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
#include "../core/BST.hpp"


using namespace GenGIS;

void BST::Insert(TileModelPtr key)
{
	if( head != NULL )
		Insert( key, head );
	else
	{
		head = new BSTNode(key,NULL,NULL,NULL);
	}
}
/*
void BST::Insert(Point2D key)
{
	if( head != NULL )
		Insert( key, head );
	else
	{
		BSTNode head(key,NULL,NULL,NULL);
	}
}
*/
// I think search might actually work off of a Point2D or something
TileModelPtr BST::Search(TileModelPtr key)
{
	return Search(key, head);
}

//TileModelPtr BST::Search(Point2D key, TileModelPtr tile)
TileModelPtr BST::Search(Point2D key)
{
	if( head )	
		return Search(key, head);
	else
	{
		TileModelPtr t;
		return t;
	}
}

BSTNode *BST::SearchNode(Point2D key)
{
	if( head )	
		return SearchNode(key, head);
	else
	{
	//	BSTNode *t = new BSTNode();
	//	return t;
		return NULL;
	}
}

void BST::DestroyTree()
{
	if( head )
		DestroyTree(head);
		head = NULL;
}

void BST::DestroyTree(BSTNode *leaf)
{
	if (leaf!=NULL)
	{
		DestroyTree(leaf->left);
		DestroyTree(leaf->right);
		delete leaf;
	}
}

// Recursive insertion method. Leaf is start as root
void BST::Insert(TileModelPtr key, BSTNode *leaf)
{
	// if key left child
//	if( leaf->tile->Compare( key->GetTopLeft() ) < 0 )
	int comp = leaf->tile->Compare( key );
	if( comp < 0 )
	{	
		if(leaf->left!=NULL)
			Insert(key, leaf->left);
		else
		{
			leaf->left= new BSTNode(key,NULL,NULL,NULL);
		}
	}
	// if right child
	else if ( comp > 0 )
	{
		if(leaf->right!=NULL)
			Insert(key,leaf->right);
		else
		{
			leaf->right = new BSTNode(key,NULL,NULL,NULL);
		}
	}
}

// Recursive insertion method. Leaf is start as root
/*
void BST::Insert(Point2D key, BSTNode *leaf)
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
TileModelPtr BST::Search(TileModelPtr key, BSTNode *leaf)
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

//TileModelPtr BST::Search(Point2D key, BSTNode *leaf, TileModelPtr tile)
TileModelPtr BST::Search(Point2D key, BSTNode *leaf)
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

void BST::Combine()
{
	Combine( head );
}

//use combination method on every tile
void BST::Combine(BSTNode *leaf)
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

bool BST::IsHead()
{
	if(head)
		return 1;
	else
		return 0;


}

void BST::SetColour(TileModelPtr tile, Colour colour)
{
	TileModelPtr cur = Search(tile);
	cur->SetColour(colour);
}

void BST::SetHead(TileModelPtr t)
{
	BSTNode *node = new BSTNode(t,NULL,NULL,NULL);
	head = node;

}

BSTNode *BST::SearchNode(Point2D locPoint, BSTNode *leaf)
{

	//BSTNode *res;	
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






// BETTER INSERT


void BST::Insert(TileModelPtr key, LocationLayerPtr curLoc, Point2D locPoint)
{
	if( head != NULL )
		Insert( key, curLoc, locPoint,  head);
	else
	{
		head = new BSTNode(key,NULL,NULL,NULL);
	}
}

//void BST::Insert(TileModelPtr key, LocationLayerPtr curLoc, Point2D locPoint, BSTNode *leaf)
void BST::Insert(TileModelPtr key, LocationLayerPtr curLoc, Point2D locPoint, BSTNode *leaf)
{
	// if key left child
	while (leaf != NULL )
	{
		
		int comp = leaf->tile->Compare( locPoint );
		if( comp == 0 )
		{
			leaf->tile->AddLocationLayer( curLoc );
			break;
		}
		// not in, lets try the dynamic link!
		else
		{
		// else try the static links
		//	else if( comp < 0 )
			if( comp < 0 )
			{	
				if(leaf->left!=NULL)
				{
					leaf->preferred = leaf->left;
					leaf = leaf->left;
		//			nodeDepth++;
				}				
					//Insert(key, curLoc, leaf->left);
				else
				{
					BSTNode * n = new BSTNode(key,NULL,NULL,NULL);
					leaf->left= n;
					leaf->preferred = n;
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
				}				
					//	Insert(key, curLoc, leaf->right);
				else
				{
					BSTNode * n = new BSTNode(key,NULL,NULL,NULL);
					leaf->right= n;
					leaf->preferred = n;
					break;
				}
			}
		}
	}
}