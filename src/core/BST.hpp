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
//=======================================================================

#ifndef _GENGIS_BST_TREE_
#define _GENGIS_BST_TREE_

#include "../core/Precompiled.hpp"
#include "../core/TileModel.hpp"

namespace GenGIS
{
	// The structure of a BST Node
	typedef struct BSTNode
	{
		BSTNode(TileModelPtr t, BSTNode *l, BSTNode *r, BSTNode *p):tile(t), left(l), right(r), preferred(p) { };
		
		BSTNode(){
			left = NULL;
			right = NULL;
			preferred = NULL;
//			tile(new TileModel());
//			tile( new TileModel() );
		};

		TileModelPtr tile;
		BSTNode *left;
		BSTNode *right;
		BSTNode *preferred;

		void AddChild(BSTNode *p)
		{
			if(tile->Compare(p->tile) < 0)
				left = p;
			else
				right = p;
		}
	
	} BSTNode;
		
	class BST
	{
	public:
		/** 
		* @brief Constructor. 
		*/
		explicit BST(){head = NULL;}
		~BST(){ DestroyTree(); }

		void Insert(TileModelPtr key);
		void Insert(Point2D key);



		void Insert(TileModelPtr key, LocationLayerPtr curLoc, Point2D locPoint);
		void Insert(TileModelPtr key, LocationLayerPtr curLoc, Point2D locPoint, BSTNode *leaf);

		// returns 0 for fail and 1 for success on search. Alsog fills result with tile.
		TileModelPtr Search(TileModelPtr key);
	//	int Search(TileModelPtr key);
		TileModelPtr Search(Point2D key);
	//	int Search(Point2D key, TileModelPtr tile);
		BSTNode *SearchNode(Point2D key);
		void DestroyTree();
	//	BSTNode *GetHead(){return head;}
		void Combine();
		bool IsHead();
		BSTNode *GetHead(){return head;}
		void SetHead(TileModelPtr t);

		void SetColour(TileModelPtr tile, Colour colour);

		void Attach(Point2D locPoint);

	private:
		BSTNode *head;
		void DestroyTree(BSTNode *leaf);
		void Insert(TileModelPtr key, BSTNode *leaf);
		void Insert(Point2D key, BSTNode *leaf);
		TileModelPtr Search(TileModelPtr key, BSTNode *leaf);
		TileModelPtr Search(Point2D key, BSTNode *leaf);
	//	int Search(Point2D key, BSTNode *leaf, TileModelPtr tile);
		void Combine( BSTNode *leaf );

		BSTNode *SearchNode(Point2D key, BSTNode *leaf);
	};
}

#endif
