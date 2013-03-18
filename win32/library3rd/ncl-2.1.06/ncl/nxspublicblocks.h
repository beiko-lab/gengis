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
#ifndef NCL_NXSPUBLICBLOCKS_H
#define NCL_NXSPUBLICBLOCKS_H

#include <vector>
#include "ncl/nxsblock.h"
#include "ncl/nxsassumptionsblock.h"
#include "ncl/nxscharactersblock.h"
#include "ncl/nxsdatablock.h"
#include "ncl/nxsdistancesblock.h"
#include "ncl/nxstaxablock.h"
#include "ncl/nxstreesblock.h"
#include "ncl/nxsunalignedblock.h"
#include "ncl/nxsreader.h"

class NxsStoreTokensBlockReader
	: public NxsBlock
	{
	public:
		/*---------------------------------------------------------------------------------------
		| If the blockname is empty then, any block will be read by the instance
		*/
		NxsStoreTokensBlockReader(std::string blockName, bool storeTokenInfo)
			:storeAllTokenInfo(storeTokenInfo),
			tolerateEOFInBlock(false)
			{
			id = NxsString(blockName.c_str());
			}
		void Read(NxsToken &token);
		void Reset();
		void Report(std::ostream &out) NCL_COULD_BE_CONST 
			{
			ReportConst(out);
			}
		void WriteAsNexus(std::ostream &out) const;
		/*---------------------------------------------------------------------------------------
		| Results in aliasing of the taxa, assumptionsBlock blocks!
		*/
		NxsStoreTokensBlockReader & operator=(const NxsStoreTokensBlockReader &other)
			{
			Reset();
			CopyBaseBlockContents(static_cast<const NxsBlock &>(other));
			commandsRead = other.commandsRead;
			justTokens = other.justTokens;
			storeAllTokenInfo = other.storeAllTokenInfo;
			tolerateEOFInBlock = other.tolerateEOFInBlock;
			return *this;
			}

		NxsStoreTokensBlockReader * Clone() const
			{
			NxsStoreTokensBlockReader * b = new NxsStoreTokensBlockReader(id, storeAllTokenInfo);
			*b = *this;
			return b;
			}
		
		virtual bool CanReadBlockType(const NxsToken & token)
			{
			if (id.length() == 0)
				{
				id.assign(token.GetTokenReference().c_str());
				id.ToUpper();
				return true;
				}
			return token.Equals(id);
			}
		virtual bool TolerateEOFInBlock() const
			{
			return tolerateEOFInBlock; /*  */
			}
		void SetTolerateEOFInBlock(bool v)
			{
			tolerateEOFInBlock = v;
			}
		const std::list<ProcessedNxsCommand> & GetCommands() const 
			{
			return commandsRead;
			}
	protected:
		void ReadCommand(NxsToken &token);
		void ReportConst(std::ostream &out) const;

		typedef std::vector<std::string> VecString;
		typedef std::list<VecString> ListVecString;


		std::list<ProcessedNxsCommand> commandsRead;
		ListVecString justTokens;
		bool storeAllTokenInfo;
		bool tolerateEOFInBlock;
	};
/*----------------------------------------------------------------------------------------------------------------------
| 	A factory class that delegates calls to the other "default" public block parsers that NCL provides. 
|
|	Provided as a convenience class to make it possible to read all supported blocks with the addition of one factory
|		to the NxsReader.
|
|	
*/
class NxsDefaultPublicBlockFactory
	: public NxsBlockFactory
	{
	public:
		/**----------------------------------------------------------------------------------------------------------------------
		|	Constructor takes two booleans.  
		|	If readUnknownBlocks is "true" then a NxsStoreTokensBlockReader will be spawned for every unknown block.
		|	storeTokenInfo is passed to the NxsStoreTokensBlockReader constructor (true for storage of full token info - such as
		|		file position.
		*/
		NxsDefaultPublicBlockFactory(bool readUnknownBlocks, bool storeTokenInfo)
			:tokenizeUnknownBlocks(readUnknownBlocks),
			storeTokenInfoArg(storeTokenInfo)
			{}
		virtual NxsBlock  *	GetBlockReaderForID(const std::string & id, NxsReader *reader, NxsToken *token);

	protected:
		NxsAssumptionsBlockFactory assumpBlockFact;
		NxsCharactersBlockFactory charBlockFact;
		NxsDataBlockFactory dataBlockFact;
		NxsDistancesBlockFactory distancesBlockFact;
		NxsTaxaBlockFactory taxaBlockFact;
		NxsTreesBlockFactory treesBlockFact;
		NxsUnalignedBlockFactory unalignedBlockFact;

		bool tokenizeUnknownBlocks;
		bool storeTokenInfoArg;
	};


/*----------------------------------------------------------------------------------------------------------------------
| 	A factory class that takes examplar that will be cloned to read each block.
|
|	To use this factory you MUST overload NxsBlock::Clone() for class that you would like to use to parse blocks
*/
class NxsCloneBlockFactory
	: public NxsBlockFactory
	{
	public:
		NxsCloneBlockFactory()
			:defPrototype(NULL)
			{}
		virtual NxsBlock  *	GetBlockReaderForID(const std::string & id, NxsReader *, NxsToken *)
			{
			std::string b(id.c_str());
			NxsString::to_upper(b);
			std::map<std::string , const NxsBlock *>::const_iterator pIt = prototypes.find(b);
			if (pIt == prototypes.end())
				return (defPrototype ? defPrototype->Clone() : NULL);
			return pIt->second->Clone();
			}

		bool AddDefaultPrototype(const NxsBlock * exemplar)
			{
			bool replaced = defPrototype != NULL;
			defPrototype = exemplar;
			return replaced;
			}
		bool AddPrototype(const NxsBlock * exemplar, const char * blockName = NULL)
			{
			std::string b;
			if (blockName)
				b.assign(blockName);
			else 
				{
				if (exemplar == NULL)
					return false;
				NxsString bId  = exemplar->GetID();
				b.assign(bId.c_str());
				}
			NxsString::to_upper(b);
			bool replaced = prototypes.find(b) != prototypes.end();
			prototypes[b] = exemplar;
			return replaced;
			}

	protected:
		std::map<std::string , const NxsBlock *> prototypes;
		const NxsBlock * defPrototype;
	};

/* bits used in the blocksToRead arg of readNexusFile */
const int NEXUS_TAXA_BLOCK_BIT = 0x01;
const int NEXUS_TREES_BLOCK_BIT = 0x02;		
const int NEXUS_CHARACTERS_BLOCK_BIT = 0x04;	
const int NEXUS_ASSUMPTIONS_BLOCK_BIT = 0x08;	
const int NEXUS_SETS_BLOCK_BIT = 0x10;	
const int NEXUS_UNALIGNED_BLOCK_BIT = 0x20;	
const int NEXUS_DISTANCES_BLOCK_BIT = 0x40;	
const int NEXUS_UNKNOWN_BLOCK_BIT = 0x80;	

/* block to index mapping used in the NumBlockReadSequence sequence returned by readNexusFile */
const int NEXUS_TAXA_BLOCK_INDEX = 0;
const int NEXUS_TREES_BLOCK_INDEX = 1;		
const int NEXUS_CHARACTERS_BLOCK_INDEX = 2;	
const int NEXUS_ASSUMPTIONS_BLOCK_INDEX = 3;	
const int NEXUS_SETS_BLOCK_INDEX = 4;	
const int NEXUS_UNALIGNED_BLOCK_INDEX = 5;	
const int NEXUS_DISTANCES_BLOCK_INDEX = 6;	
const int NEXUS_UNKNOWN_BLOCK_INDEX = 8;	


/**----------------------------------------------------------------------------
| NxsReader that uses clone factories to read public blocks.  
|	The blocks created by reading a file must be delete by the caller 
|	(either by a call to DeleteBlocksFromFactories() or by requesting each 
|	pointer and then deleting them in turn.
*/
class PublicNexusReader: public ExceptionRaisingNxsReader
	{
	public: 

		PublicNexusReader(const int blocksToRead = -1, NxsReader::WarningHandlingMode mode=NxsReader::WARNINGS_TO_STDERR);
		virtual ~PublicNexusReader();
		
		virtual void	Execute(NxsToken& token, bool notifyStartStop = true);
		std::string GetErrorMessage()
			{
			return errorMsg;
			}

		NxsAssumptionsBlock * GetAssumptionsBlockTemplate() {return assumptionsBlockTemplate;}
		NxsCharactersBlock * GetCharactersBlockTemplate() {return charactersBlockTemplate;}
		NxsDataBlock * GetDataBlockTemplate() {return dataBlockTemplate;}
		NxsDistancesBlock * GetDistancesBlockTemplate() {return distancesBlockTemplate;}
		NxsTaxaBlock * GetTaxaBlockTemplate() {return taxaBlockTemplate;}
		NxsTreesBlock * GetTreesBlockTemplate() {return treesBlockTemplate;}
		NxsUnalignedBlock * GetUnalignedBlockTemplate() {return unalignedBlockTemplate;}
		NxsStoreTokensBlockReader * GetUnknownBlockTemplate() const {return storerBlockTemplate;}
			
		unsigned GetNumAssumptionsBlocks(const NxsTaxaBlock *taxa) const;
		unsigned GetNumAssumptionsBlocks(const NxsCharactersBlock *taxa) const;
		unsigned GetNumAssumptionsBlocks(const NxsTreesBlock *taxa) const;
		NxsAssumptionsBlock * GetAssumptionsBlock(const NxsTaxaBlock *taxa, unsigned index) const;
		NxsAssumptionsBlock * GetAssumptionsBlock(const NxsCharactersBlock *taxa, unsigned index) const;
		NxsAssumptionsBlock * GetAssumptionsBlock(const NxsTreesBlock *taxa, unsigned index) const;

		unsigned GetNumUnknownBlocks() const;
		NxsStoreTokensBlockReader * GetUnknownBlock(unsigned index) const;

		unsigned GetNumCharactersBlocks(const NxsTaxaBlock *taxa) const;
		NxsCharactersBlock * GetCharactersBlock(const NxsTaxaBlock *taxa, unsigned index) const;

		unsigned GetNumDistancesBlocks(const NxsTaxaBlock *taxa) const;
		NxsDistancesBlock * GetDistancesBlock(const NxsTaxaBlock *taxa, unsigned index) const;

		unsigned GetNumTaxaBlocks() const;
		NxsTaxaBlock * GetTaxaBlock(unsigned index) const;

		unsigned GetNumTreesBlocks(const NxsTaxaBlock *taxa) const;
		NxsTreesBlock * GetTreesBlock(const NxsTaxaBlock *taxa, unsigned index) const;

		unsigned GetNumUnalignedBlocks(const NxsTaxaBlock *taxa) const;
		NxsUnalignedBlock * GetUnalignedBlock(const NxsTaxaBlock *taxa, unsigned index) const;

		virtual void DeleteBlocksFromFactories()
			{
			NxsReader::DeleteBlocksFromFactories();
			ClearUsedBlockList();
			}
		virtual void ClearUsedBlockList();
		virtual void ClearContent() 
			{
			assumptionsBlockVec.clear();
			charactersBlockVec.clear();
			dataBlockVec.clear();
			distancesBlockVec.clear();
			storerBlockVec.clear();
			taxaBlockVec.clear();
			treesBlockVec.clear();
			unalignedBlockVec.clear();
			ExceptionRaisingNxsReader::ClearContent();
			}
		void AddReadAssumptionsBlock(NxsAssumptionsBlock * block) 
			{
			assumptionsBlockVec.push_back(block);
			AddReadBlock("ASSUMPTIONS", block);
			}
		void AddReadCharactersBlock(NxsCharactersBlock * block) 
			{
			charactersBlockVec.push_back(block);
			AddReadBlock("CHARACTERS", block);
			}
		void AddReadDataBlock(NxsDataBlock * block) 
			{
			dataBlockVec.push_back(block);
			AddReadBlock("CHARACTERS", block);
			}
		void AddReadDistancesBlock(NxsDistancesBlock * block) 
			{
			distancesBlockVec.push_back(block);
			AddReadBlock("DISTANCES", block);
			}
		void AddReadTaxaBlock(NxsTaxaBlock * block) 
			{
			taxaBlockVec.push_back(block);
			AddReadBlock("TAXA", block);
			}
		void AddReadTreesBlock(NxsTreesBlock * block) 
			{
			treesBlockVec.push_back(block);
			AddReadBlock("TREES", block);
			}
		void AddReadUnalignedBlock(NxsUnalignedBlock * block) 
			{
			unalignedBlockVec.push_back(block);
			AddReadBlock("UNKNOWN", block);
			}
		void AddReadUnknownBlock(NxsStoreTokensBlockReader * block) 
			{
			storerBlockVec.push_back(block);
			AddReadBlock(block->GetID(), block);
			}
	protected:
		void PostExecuteHook();
		virtual void    AddFactory(NxsBlockFactory *);
		NxsCloneBlockFactory cloneFactory;

		NxsAssumptionsBlock * assumptionsBlockTemplate;
		NxsCharactersBlock * charactersBlockTemplate;
		NxsDataBlock * dataBlockTemplate;
		NxsDistancesBlock * distancesBlockTemplate;
		NxsStoreTokensBlockReader * storerBlockTemplate;
		NxsTaxaBlock * taxaBlockTemplate;
		NxsTreesBlock * treesBlockTemplate;
		NxsUnalignedBlock * unalignedBlockTemplate;

		std::vector<NxsAssumptionsBlock *> assumptionsBlockVec;
		std::vector<NxsCharactersBlock *> charactersBlockVec;
		std::vector<NxsDataBlock *> dataBlockVec;
		std::vector<NxsDistancesBlock *> distancesBlockVec;
		std::vector<NxsStoreTokensBlockReader *> storerBlockVec;
		std::vector<NxsTaxaBlock *> taxaBlockVec;
		std::vector<NxsTreesBlock *> treesBlockVec;
		std::vector<NxsUnalignedBlock *> unalignedBlockVec;

		std::string errorMsg;

		
	};


#endif


