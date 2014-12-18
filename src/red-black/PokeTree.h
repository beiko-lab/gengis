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

#ifndef _GENGIS_PokeTree_TREE_
#define _GENGIS_PokeTree_TREE_

#include "../core/Precompiled.hpp"
#include "../core/TileModel.hpp"
#include "../core/LocationModel.hpp"

namespace GenGIS
{
	enum NodeColor {Red, Black};
	// The structure of a PokeTree Node
	typedef struct PokeNode
	{

		PokeNode(TileModelPtr t, PokeNode *l, PokeNode *r, PokeNode *p, PokeNode *d = NULL, PokeNode *par=NULL):tile(t), left(l), right(r), preferred(p), dynamic(d) , parent(par)
		{
			depth = 1;
			color = Black;
			max = Point2D( t->GetBottomRight().x, t->GetBottomRight().y );
			min = Point2D( t->GetTopLeft().x, t->GetTopLeft().y );
		};
		
		PokeNode(){
			color = Black;
			left = NULL;
			right = NULL;
			preferred = NULL;
			parent = NULL;
			max = Point2D(-180,-90);
			min = Point2D(180,90);
//			std::map<std::wstring,std::wstring> data;
//			tile(new TileModelPtr(StringTools::ToStringW(""),min,max,data));
//			tile( new TileModel() );
		};

		PokeNode(PokeNode *p){
			parent = p;
			color = Black;
			left = NULL;
			right = NULL;
			preferred = NULL;
			parent = NULL;
			max = Point2D(-180,-90);
			min = Point2D(180,90);
//			std::map<std::wstring,std::wstring> data;
//			tile(new TileModelPtr(StringTools::ToStringW(""),min,max,data));
//			tile( new TileModel() );
		};

		TileModelPtr tile;
		PokeNode *left;
		PokeNode *right;
		PokeNode *preferred;
		PokeNode *dynamic;
		PokeNode *parent;
		// smallest top left value
		Point2D min;
		// biggest bottom right value
		Point2D max;
		NodeColor color;
		// this is the depth from the root.
		int depth;


		void AddChild(PokeNode *p)
		{
			if(tile->Compare(p->tile) < 0)
				left = p;
			else
				right = p;
		}
	
	} PokeNode;
		
	class PokeTree
	{
	public:
		/** 
		* @brief Constructor. 
		*/
	//	explicit PokeTree(){head = NULL;}
		explicit PokeTree();
		~PokeTree(){ DestroyTree(); }

	//	void Insert(TileModelPtr key, LocationLayerPtr curLoc);
		void Insert(TileModelPtr key, LocationLayerPtr curLoc, Point2D locPoint);
		void Insert(Point2D key);
		// returns 0 for fail and 1 for success on search. Alsog fills result with tile.
		TileModelPtr Search(TileModelPtr key);
	//	int Search(TileModelPtr key);
		TileModelPtr Search(Point2D key);
	//	int Search(Point2D key, TileModelPtr tile);
		PokeNode *SearchNode(Point2D key);
		void DestroyTree();
	//	PokeNode *GetHead(){return head;}
		void Combine();
		bool IsHead();
		PokeNode *GetHead(){return head;}
		void SetHead(TileModelPtr t);

		void SetColour(TileModelPtr tile, Colour colour);

		void Attach(Point2D locPoint);

		// swaps color, used in the split method
		 void swapColor(PokeNode* nodePtr, PokeNode* parentPtr, PokeNode* grandPtr);
		 
		 // Perform rightRotate
		 // rotate the nodePtr around its left child
		 // parentPtr points to the parent of nodePtr
		 void rightRotate( PokeNode* nodePtr, PokeNode* parentPtr );
		 
		 // Perform leftRotate
		 // rotate the nodePtr around its right child
		 // parentPtr points to the parent of nodePtr
		 void leftRotate( PokeNode* nodePtr, PokeNode* parentPtr );
		 
		 
		 // split a 4-node: nodePtr
		 // parentPtr points to the parent of nodePtr
		 // grandPtr points to the parent of parentPtr
		 // greatPtr points to the parent of grandPtr
		 void split ( PokeNode* nodePtr,  PokeNode* parentPtr, PokeNode* grandPtr, PokeNode* greatPtr );

		 // return min of two points
		 Point2D PokeMin(Point2D l, Point2D r);
		 Point2D PokeMax(Point2D l, Point2D r);

		int locCount;

		int GetLocCount(){ return locCount; }
	private:
		PokeNode *head;
		// nulled node. End of The Line
		PokeNode *theLast;
		void DestroyTree(PokeNode *leaf);
	//	void Insert(TileModelPtr key, LocationLayerPtr curLoc, PokeNode *leaf);
		void Insert(TileModelPtr key, LocationLayerPtr curLoc, Point2D locPoint, PokeNode *leaf);
		void Insert(Point2D key, PokeNode *leaf);
		TileModelPtr Search(TileModelPtr key, PokeNode *leaf);
		TileModelPtr Search(Point2D key, PokeNode *leaf);
	//	int Search(Point2D key, PokeNode *leaf, TileModelPtr tile);
		void Combine( PokeNode *leaf );

		PokeNode *SearchNode(Point2D key, PokeNode *leaf);

		// Function to fix up dynamic nodes and min/max's
		void UpdatePokeTree();

		void UpdateMinMax( PokeNode *leaf );
	};
}

#endif
