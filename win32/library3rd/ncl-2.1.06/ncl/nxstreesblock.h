//	Copyright (C) 1999-2003 Paul O. Lewis
//
//	This file is part of NCL (Nexus Class Library) version 2.0.
//
//	NCL is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	NCL is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with NCL; if not, write to the Free Software Foundation, Inc., 
//	59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//

#ifndef NCL_NXSTREESBLOCK_H
#define NCL_NXSTREESBLOCK_H
#include <climits>
#include <cfloat>
#include "ncl/nxstaxablock.h"

class NxsTreesBlockAPI
  : public NxsBlock, public NxsLabelToIndicesMapper
	{
 	public:
		virtual unsigned	GetNumDefaultTree() = 0;
		virtual unsigned	GetNumTrees() = 0;
		virtual NxsString	GetTreeName(unsigned i) = 0;
		virtual NxsString	GetTreeDescription(unsigned i) = 0;
		virtual NxsString	GetTranslatedTreeDescription(unsigned i) = 0;
		virtual bool		IsDefaultTree(unsigned i) = 0;
		virtual bool		IsRootedTree(unsigned i) = 0;
	};

std::string parseNHXComment(const std::string comment, std::map<std::string, std::string> *infoMap);
class NxsFullTreeDescription;
class NxsSimpleNode;
class NxsSimpleEdge
	{
	public:
		bool EdgeLenIsDefaultValue() const
			{
			return defaultEdgeLen;
			}

		bool IsIntEdgeLen() const 
			{
			return hasIntEdgeLens;
			}

		double GetDblEdgeLen() const
			{
			return hasIntEdgeLens ? (double) iEdgeLen : dEdgeLen ;
			}

		int GetIntEdgeLen() const 
			{
			return hasIntEdgeLens ? iEdgeLen : (int) dEdgeLen ;
			}

		std::vector<NxsComment> GetUnprocessedComments() const
			{
			return unprocessedComments;
			}

		/// returns true if `key` was processed from a comment.
		///	If found, (and value is not NULL), the *value will hold the 
		///		value on exit
		bool GetInfo(const std::string &key, std::string *value) const
			{
			std::map<std::string, std::string>::const_iterator kvit = parsedInfo.find(key);
			if (kvit == parsedInfo.end())
				return false;
			if (value != NULL)
				*value = kvit->second;
			return true;
			}

		const NxsSimpleNode * GetParent() const
			{
			return parent;
			}
		const NxsSimpleNode * GetChild() const
			{
			return child;
			}

		void SetDblEdgeLen(double e, const char *asString)
			{
			defaultEdgeLen = false;
			hasIntEdgeLens = false; 
			dEdgeLen = e;
			if (asString)
				lenAsString.assign(asString);

			}
		
		void SetIntEdgeLen(int e, const char *asString) 
			{
			defaultEdgeLen = false;
			hasIntEdgeLens = true; 
			iEdgeLen = e;
			if (asString)
				lenAsString.assign(asString);
			}
		mutable void * scratch;
	private:
		void WriteAsNewick(std::ostream &out, bool nhx) const;
		void DealWithNexusComments(const std::vector<NxsComment> & ecs, bool NHXComments);

		NxsSimpleEdge(NxsSimpleNode  *par, NxsSimpleNode * des, double edgeLen)
			:scratch(0L),
			parent(par),
			child(des),
			defaultEdgeLen(true),
			hasIntEdgeLens(false),
			dEdgeLen(edgeLen)
			{
			}

		NxsSimpleEdge(int edgeLen, NxsSimpleNode *par, NxsSimpleNode * des)
			:scratch(0L),
			parent(par),
			child(des),
			defaultEdgeLen(true),
			hasIntEdgeLens(true),
			iEdgeLen(edgeLen)
			{
			}

		NxsSimpleNode * GetMutableParent() const
			{
			return parent;
			}
		
		NxsSimpleNode * parent;
		NxsSimpleNode * child;
		bool			defaultEdgeLen;
		bool			hasIntEdgeLens;
		int				iEdgeLen;
		double			dEdgeLen;
		std::string		lenAsString; /*easy (but inefficient) means of preserving the formatting of the input branch length */
		std::vector<NxsComment> unprocessedComments;
		std::map<std::string, std::string> parsedInfo;
		friend class NxsSimpleTree;
		friend class NxsSimpleNode;
	};

class NxsSimpleNode
	{
	public:
		NxsSimpleEdge GetEdgeToParent() const
			{
			return edgeToPar;
			}
		
		NxsSimpleNode *GetFirstChild() const
			{
			return lChild;
			}
		NxsSimpleNode * GetNextSib() const
			{
			return rSib;
			}
		NxsSimpleNode * GetLastChild() const
			{
			NxsSimpleNode * currNode = GetFirstChild();
			if (!currNode)
				return NULL;
			NxsSimpleNode * nextNd = currNode->GetNextSib();
			while (nextNd)
				{
				currNode = nextNd;
				nextNd = currNode->GetNextSib();
				}
			return currNode;
			}
		
		std::vector<NxsSimpleNode *> GetChildren() const
			{
			std::vector<NxsSimpleNode *> children;
			NxsSimpleNode * currNode = GetFirstChild();
			while(currNode)
				{
				children.push_back(currNode);
				currNode = currNode->GetNextSib();
				}
			return children;
			}
		// present for every leaf. UINT_MAX for internals labeled with taxlabels
		unsigned GetTaxonIndex() const
			{
			return taxIndex;
			}
			
		// non-empty only for internals that are labelled with names that are NOT taxLabels
		std::string GetName() const
			{
			return name;
			}
		mutable void * scratch;
	private:
		void WriteAsNewick(std::ostream &out, bool nhx) const;
			
		
		NxsSimpleNode(NxsSimpleNode *par, double edgeLen)
			:scratch(0L),
			lChild(0L),
			rSib(0L),
			edgeToPar(par, 0L, edgeLen),
			taxIndex(UINT_MAX)
			{
			edgeToPar.child = this;
			}

		NxsSimpleNode(int edgeLen, NxsSimpleNode *par)
			:scratch(0L),
			lChild(0L),
			rSib(0L),
			edgeToPar(edgeLen, par, 0L),
			taxIndex(UINT_MAX)
			{
			edgeToPar.child = this;
			}

		NxsSimpleNode * GetParent() const
			{
			return edgeToPar.GetMutableParent();
			}

		void AddSib(NxsSimpleNode *n)
			{
			if (rSib)
				rSib->AddSib(n);
			else
				rSib = n;
			}
		void AddChild(NxsSimpleNode *n)
			{
			if (lChild)
				lChild->AddSib(n);
			else
				lChild = n;
			}
		void AddSelfAndDesToPreorder(std::vector<const NxsSimpleNode *> &p) const;
		NxsSimpleNode * FindTaxonIndex(unsigned leafIndex);
		NxsSimpleNode * lChild;
		NxsSimpleNode * rSib;
		NxsSimpleEdge edgeToPar;
		std::string name; // non-empty only for internals that are labelled with names that are NOT taxLabels
		unsigned taxIndex; // present for every leaf. UINT_MAX for internals labeled with taxlabels
		friend class NxsSimpleTree;
	};

class NxsSimpleTree
	{
	public:
		NxsSimpleTree(const NxsFullTreeDescription &ftd, const int defaultIntEdgeLen, const double defaultDblEdgeLen)
			:defIntEdgeLen(defaultIntEdgeLen),
			defDblEdgeLen(defaultDblEdgeLen),
			realEdgeLens(false)
			{
			Initialize(ftd);
			}
		NxsSimpleTree(const int defaultIntEdgeLen, const double defaultDblEdgeLen)
			:defIntEdgeLen(defaultIntEdgeLen),
			defDblEdgeLen(defaultDblEdgeLen),
			realEdgeLens(false)
			{}
		~NxsSimpleTree()
			{
			Clear();
			}
		void Initialize(const NxsFullTreeDescription &);
			
		
		std::vector<const NxsSimpleNode *> GetPreorderTraversal() const;
		std::vector<std::vector<int> > GetIntPathDistances(bool toMRCA=false) const;
		std::vector<std::vector<double> > GetDblPathDistances(bool toMRCA=false) const;
		
		/** Writes just the newick description with numbers for leaf labels.
			Neither the tree name or NEXUS ; are written
		*/
		void WriteAsNewick(std::ostream &out, bool nhx) const
			{
			if (root)
				root->WriteAsNewick(out, nhx);
			}
		void RerootAt(unsigned leafIndex);
		
		const NxsSimpleNode * GetRootConst() const 
			{
			return root;
			}
	protected:
		std::vector<NxsSimpleNode *> allNodes;
		std::vector<NxsSimpleNode *> leaves;
		NxsSimpleNode * root;
		int defIntEdgeLen;
		double defDblEdgeLen;
		bool realEdgeLens;
	private:
		NxsSimpleNode * AllocNewNode(NxsSimpleNode *p)
			{
			NxsSimpleNode * nd;
			if (realEdgeLens)
				nd = new NxsSimpleNode(p, defDblEdgeLen);
			else
				nd = new NxsSimpleNode(defIntEdgeLen, p);
			allNodes.push_back(nd);
			return nd;
			}
		
		void Clear()
			{
			root = NULL;
			for (std::vector<NxsSimpleNode *>::iterator nIt = allNodes.begin(); nIt != allNodes.end(); ++nIt)
				delete *nIt;
			allNodes.clear();
			leaves.clear();
			}
		void FlipRootsChildToRoot(NxsSimpleNode *subRoot);
		NxsSimpleTree(const NxsSimpleTree &); //not defined.  Not copyable
		NxsSimpleTree & operator=(const NxsSimpleTree &); //not defined.  Not copyable
	};

class NxsFullTreeDescription
	{
	public:
		enum TreeDescFlags 
			{ 	NXS_IS_ROOTED_BIT					= 0x0001,
				NXS_HAS_SOME_EDGE_LENGTHS_BIT		= 0x0002,
				NXS_MISSING_SOME_EDGE_LENGTHS_BIT	= 0x0004,
				NXS_EDGE_LENGTH_UNION 				= 0x0006, 
				NXS_INT_EDGE_LENGTHS_BIT 			= 0x0008,
				NXS_HAS_ALL_TAXA_BIT				= 0x0010,
				NXS_HAS_NHX_BIT 					= 0x0020,
				NXS_HAS_DEG_TWO_NODES_BIT			= 0x0040,
				NXS_HAS_POLYTOMY_BIT				= 0x0080,
				NXS_HAS_INTERNAL_NAMES_BIT			= 0x0100,
				NXS_HAS_NEW_INTERNAL_NAMES_BIT		= 0x0200,
				NXS_KNOWN_INTERNAL_NAMES_BIT		= 0x0400, 
				NXS_SOME_ZERO_EDGE_LEN_BIT			= 0x0800, 
				NXS_SOME_NEGATIVE_EDGE_LEN_BIT		= 0x1000, 
				NXS_TREE_PROCESSED 					= 0x2000
			};
		NxsFullTreeDescription(const std::string & newickStr, const std::string &treeName, int infoFlags)
			:newick(newickStr),
			name(treeName),
			flags(infoFlags),
			minIntEdgeLen(INT_MAX),
			minDblEdgeLen(DBL_MAX)
			{}
			
	
		/** returns a newick string with 1-based numbers corresponding to (1 + Taxa block's index of taxon)*/
		const std::string &	GetNewick() const
			{
			return newick;
			}
		const std::string &	GetName() const 
			{
			return name;
			}
		bool IsProcessed() const 
			{
			return (flags&NXS_TREE_PROCESSED) != 0;
			}
		void AssertProcessed() const 
			{
			if (!IsProcessed())
				throw NxsNCLAPIException("Tree description queries are only supported on processed tree descriptions.");
			}
		bool IsRooted() const 
			{
			AssertProcessed();
			return (flags&NXS_IS_ROOTED_BIT) != 0;
			}
		bool AllEdgesHaveLengths() const 
			{
			AssertProcessed();
			return (flags&NXS_EDGE_LENGTH_UNION) == NXS_HAS_SOME_EDGE_LENGTHS_BIT;
			}
		bool SomeEdgesHaveLengths() const 
			{
			AssertProcessed();
			return (flags&NXS_HAS_SOME_EDGE_LENGTHS_BIT) != 0;
			}
		bool EdgeLengthsAreAllIntegers() const
			{
			AssertProcessed();
			return (flags&NXS_INT_EDGE_LENGTHS_BIT) != 0;
			}
		bool AllTaxaAreIncluded() const
			{
			AssertProcessed();
			return (flags&NXS_HAS_ALL_TAXA_BIT) != 0;
			}
		bool HasNHXComments() const
			{
			AssertProcessed();
			return (flags&NXS_HAS_NHX_BIT) != 0;
			}
		bool HasPolytomies() const
			{
			AssertProcessed();
			return (flags&NXS_HAS_POLYTOMY_BIT) != 0;
			}
		bool HasDegreeTwoNodes() const
			{
			AssertProcessed();
			return (flags&NXS_HAS_DEG_TWO_NODES_BIT) != 0;
			}
		/**---------------------------------------------------------------------
		|	If EdgeLengthsAreAllIntegers returns true then this will return the 
		|	shortest edge length in the tree (useful as means of checking for 
		|	constraints by programs that prohibit 0 or negative branch lengths)
		*/
		int smallestIntEdgeLength() const
			{
			return minIntEdgeLen;
			}
		/**---------------------------------------------------------------------
		|	If EdgeLengthsAreAllIntegers returns false then this will return the 
		|	shortest edge length in the tree (useful as means of checking for 
		|	constraints by programs that prohibit 0 or negative branch lengths)
		*/
		double smallestRealEdgeLength() const
			{
			return minDblEdgeLen;
			}
	private:
		std::string newick; /*with 1-based numbers corresponding to (1 + Taxa block's index of taxon)*/
		std::string name;
		int flags;
		int minIntEdgeLen; /* if EdgeLengthsAreAllIntegers returns true then this will hold shortest edge length in the tree (useful as means of checking for constraints by programs that prohibit 0 or negative branch lengths)*/
		double minDblEdgeLen; /* if EdgeLengthsAreAllIntegers returns false then this will hold shortest edge length in the tree (useful as means of checking for constraints by programs that prohibit 0 or negative branch lengths)*/

	friend class NxsTreesBlock;
	};
class NxsTreesBlock;
typedef bool (* ProcessedTreeValidationFunction)(NxsFullTreeDescription &, void *, NxsTreesBlock *);
/*----------------------------------------------------------------------------------------------------------------------
|	This class handles reading and storage for the NEXUS block TREES. It overrides the member functions Read and Reset,
|	which are abstract virtual functions in the base class NxsBlock. The translation table (if one is supplied) is 
|	stored in the `translateList'. The tree names are stored in `treeName' and the tree descriptions in 
|	`treeDescription'. Information about rooting of trees is stored in `rooted'. Note that no checking is done to 
|	ensure that the tree descriptions are valid. The validity of the tree descriptions could be checked after the TREES
|	block has been read (but before the next block in the file has been read) by overriding the NxsReader::ExitingBlock
|	member function, but no functionality for this is provided by the NCL. Below is a table showing the correspondence
|	between the elements of a TREES block and the variables and member functions that can be used to access each piece 
|	of information stored. 
|>
|	NEXUS command     Data members    Member functions
|	-----------------------------------------------------
|	TRANSLATE         translateList
|	
|	TREE              treeName        GetTreeName
|	                                  GetTreeDescription
|	                                  GetNumTrees
|	                                  GetNumDefaultTree
|	                                  IsDefaultTree
|	
|	                  rooted          IsRootedTree
|	-----------------------------------------------------
|>
*/
class NxsTreesBlock 
  : public NxsTreesBlockAPI, public NxsTaxaBlockSurrogate
	{
 	public:
							NxsTreesBlock(NxsTaxaBlockAPI *tb);
		virtual				~NxsTreesBlock();

		void		ReplaceTaxaBlockPtr(NxsTaxaBlockAPI *tb);
		unsigned GetIndexSet(const std::string &label, NxsUnsignedSet * toFill) const
			{
			return NxsLabelToIndicesMapper::GetIndicesFromSets(label, toFill, treeSets);
			}

		unsigned	GetNumDefaultTree();
		unsigned	GetNumTrees();
		unsigned	GetNumTrees() const;
		const NxsFullTreeDescription & GetFullTreeDescription(unsigned i) const;
		unsigned	TreeLabelToNumber(const std::string & name) const;
		NxsString	GetTreeName(unsigned i);
		NxsString	GetTreeDescription(unsigned i);
		NxsString	GetTranslatedTreeDescription(unsigned i);
		bool		IsDefaultTree(unsigned i);
		bool		IsRootedTree(unsigned i);
		virtual void		Report(std::ostream &out) NCL_COULD_BE_CONST ;
		virtual void		BriefReport(NxsString &s) NCL_COULD_BE_CONST ;
		virtual void		Reset();
		void				SetNexus(NxsReader *nxsptr)
			{
			NxsBlock::SetNexus(nxsptr);
			NxsTaxaBlockSurrogate::SetNexusReader(nxsptr);
			}
        virtual const std::string & GetBlockName() const
            {
            return id;
            }

		void WriteAsNexus(std::ostream &out) const;

		virtual VecBlockPtr	GetImpliedBlocks()
			{
			return GetCreatedTaxaBlocks();
			}

		/*only used it the linkAPI is enabled*/
		virtual void		HandleLinkCommand(NxsToken & token)
			{
			HandleLinkTaxaCommand(token);
			}
		virtual void		WriteLinkCommand(std::ostream &out) const
			{
			WriteLinkTaxaCommand(out);
			}

		unsigned GetMaxIndex() const;
		unsigned GetIndicesForLabel(const std::string &label, NxsUnsignedSet *inds) const;
		bool AddNewIndexSet(const std::string &label, const NxsUnsignedSet & inds);
		bool AddNewPartition(const std::string &label, const NxsPartition & inds);

		bool GetAllowImplicitNames() const 
			{
			return allowImplicitNames;
			}
		bool GetProcessAllTreesDuringParse() const 
			{
			return processAllTreesDuringParse;
			}
		void SetAllowImplicitNames(bool s) 
			{	
			allowImplicitNames = s;
			}
		void SetProcessAllTreesDuringParse(bool s)
			{
			processAllTreesDuringParse = s;
			}
		/*Interprets the newick string as a tree (builds trees as in memory
			structures, which may reveal illegal newick strings that were not
			detected as illegal on the parse).
		*/
		void ProcessTree(NxsFullTreeDescription &treeDesc) const;
		void ProcessAllTrees() const
			{
			std::vector<NxsFullTreeDescription>::iterator trIt = trees.begin();
			for (; trIt != trees.end(); ++trIt)
				ProcessTree(*trIt);
			}


		/*---------------------------------------------------------------------------------------
		| Results in aliasing of the taxa, assumptionsBlock blocks!
		*/
		NxsTreesBlock & operator=(const NxsTreesBlock &other)
			{
			Reset();
			CopyBaseBlockContents(static_cast<const NxsBlock &>(other));
			CopyTaxaBlockSurrogateContents(other);
			CopyTreesBlockContents(other);
			return *this;
			}

		/*---------------------------------------------------------------------------------------
		| Results in aliasing of the taxa, assumptionsBlock blocks!
		*/
		virtual void CopyTreesBlockContents(const NxsTreesBlock &other)
			{
			allowImplicitNames = other.allowImplicitNames;
			processAllTreesDuringParse = other.processAllTreesDuringParse;
			writeFromNodeEdgeDataStructure = other.writeFromNodeEdgeDataStructure;
			constructingTaxaBlock = other.constructingTaxaBlock;
			newtaxa = other.newtaxa;
			trees = other.trees;
			capNameToInd = other.capNameToInd;
			defaultTreeInd = other.defaultTreeInd;
			treeSets = other.treeSets;
			treePartitions = other.treePartitions;
			processedTreeValidationFunction = other.processedTreeValidationFunction;
			ptvArg = other.ptvArg;
			}

		virtual NxsTreesBlock * Clone() const 
			{
			NxsTreesBlock * a = new NxsTreesBlock(taxa);
			*a = *this;
			return a;
			}
		static void ProcessTokenVecIntoTree(const ProcessedNxsCommand & token, NxsFullTreeDescription & ftd, NxsLabelToIndicesMapper *, std::map<std::string, unsigned> &capNameToInd, bool allowNewTaxa, NxsReader * nexusReader, const bool respectCase=false);
		static void ProcessTokenStreamIntoTree(NxsToken & token, NxsFullTreeDescription & ftd, NxsLabelToIndicesMapper *, std::map<std::string, unsigned> &capNameToInd, bool allowNewTaxa, NxsReader * nexusReader, const bool respectCase=false);
		
		void SetWriteFromNodeEdgeDataStructure(bool v)
			{
			writeFromNodeEdgeDataStructure = v;
			}
		/* 	Processes all trees and then
			Provides lowlevel access to the "raw" vector of trees stored in the trees block
		*/
		std::vector<NxsFullTreeDescription> & GetProcessedTrees()
			{
			ProcessAllTrees();
			return trees;
			}
		void setValidationCallbacks(ProcessedTreeValidationFunction func, void * arg)
			{
			this->processedTreeValidationFunction = func;
			this->ptvArg = arg;
			}
		bool 		SwapEquivalentTaxaBlock(NxsTaxaBlockAPI * tb)
		{
			return SurrogateSwapEquivalentTaxaBlock(tb);
		}
		void ReadPhylipTreeFile(NxsToken & token);
	protected :
		void ReadTreeFromOpenParensToken(NxsFullTreeDescription &td, NxsToken & token);

		void WriteTranslateCommand(std::ostream & out) const;
		void WriteTreesCommand(std::ostream & out) const;
		void ConstructDefaultTranslateTable(NxsToken &token, const char * cmd);

		bool allowImplicitNames; /** false by default, true causes the trees block to create a taxa block from the labels found in the trees. */
		bool processAllTreesDuringParse; /** true by default, false speeds processing but disables detection of errors*/
		bool constructingTaxaBlock; /** true if new names are being tolerated */
		bool writeFromNodeEdgeDataStructure; /**this will probably only ever be set to true in testing code. If true the WriteTrees function will convert each tree to NxsSimpleTree object to write the newick*/

		mutable std::vector<NxsFullTreeDescription> trees;
		mutable std::map<std::string, unsigned> capNameToInd;
		unsigned			defaultTreeInd;		/* 0-offset index of default tree specified by user, or 0 if user failed to specify a default tree using an asterisk in the NEXUS data file */
		NxsUnsignedSetMap 	treeSets;		
		NxsPartitionsByName treePartitions;	

		ProcessedTreeValidationFunction processedTreeValidationFunction;
		void * ptvArg;
		
		virtual	void		Read(NxsToken &token);
		void				HandleTranslateCommand(NxsToken &token);
		void				HandleTreeCommand(NxsToken &token, bool rooted);

		friend class PublicNexusReader;
	};

typedef NxsTreesBlock TreesBlock;
class NxsTreesBlockFactory
	:public NxsBlockFactory
	{
	public:
		virtual NxsTreesBlock  *	GetBlockReaderForID(const std::string & id, NxsReader *reader, NxsToken *token);
	};

#endif
