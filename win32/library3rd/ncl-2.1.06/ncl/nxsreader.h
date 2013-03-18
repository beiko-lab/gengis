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

#ifndef NCL_NXSREADER_H
#define NCL_NXSREADER_H

#include "ncl/nxsdefs.h"
#include "ncl/nxsstring.h"
#include "ncl/nxsexception.h"
#include "ncl/nxstoken.h"

class NxsBlock;
class NxsBlockFactory;
class NxsTaxaBlockFactory;
class NxsAssumptionsBlockAPI;
class NxsCharactersBlockAPI;
class NxsTaxaBlockAPI;
class NxsTreesBlockAPI;

typedef std::list<NxsBlock *> BlockReaderList;
typedef std::map<std::string, BlockReaderList> BlockTypeToBlockList;


/*----------------------------------------------------------------------------------------------------------------------
|	This is the class that orchestrates the reading of a NEXUS data file. 
|	
|	In the "classic" NCL API:
|		1. An NxsReader is created.
|		2. pointers to instances of NxsBlocks that are expected to be needed should be added to `blockList' using the 
|			NxsReader::Add() member function. 
|		3. NxsReader::Execute() is then called, which reads the data file until encountering a block name, at which 
|			point the correct block is looked up in `blockList' and that object's NxsBlock::Read() method is called. 
|		4. NxsReader::PostBlockReadingHook(NxsBlock) is called after a block is successfully read.  This allows one to gather
|			the parsed data from the NxsBlock.  If another block of the same type is encountered, then NxsBlock::Reset()
|			will be called and the same NxsBlock instance will be used to read the next block.
|
|	Versions of NCL after 2.0.04 also support a "factory API" augments the former behavior:
|		2b. In addition to NxsBlocks added using the Add method, you can register instances of a NxsBlockFactory using
|			the NxsReader::AddFactory() method.
|		3b. In the NxsReader::Execute() method, if an appropriate block is not found in the `blockList` then the
|			the factories are asked to create a block for the current block name.  The first non-NULL block pointer
|			returned is used.
|		4b. PostBlockReadingHook is still called, but blocks created by a factory will not be "recycled" later in the
|			NxsReader::Execute(), so it is not necessary to pull all of the data out of them.  
|		4c 	If a block created by a factory is skipped or has an error, then the factory will be notified using 
|			NxsBlockFactory::BlockError(NxsBlock *) or NxsBlockFactory::BlockSkipped(NxsBlock *).  In the event of 
|			skipping or an error, NxsReader will never refer to that instance of the factory-created block again.  
|			Hence the base class behavior of BlockError() and BlockSkipped() is to delete the instance.
|		5	Every time a NxsBlock successfully reads a NEXUS block, the NxsBlock is added to container of "used" blocks.
|			When a block is Reset() it is removed from this container.  NxsReader::GetUsedBlocks() can be called at any
|			point to get a copy of this container (which maps a block type name to a list of NxsBlock*). This container
|			thus stores the state of the parsed NEXUS file.  If no blocks were recycled (if all of the blocks came from
|			factories rather than blocks added using NxsReader::Add() method), then the GetUsedBlocks will contain binary
|			representation of every block parsed.
|
|		See basicfactory for an example of this API.	
|
|	Important: The use of the factories that are supplied with NCL can trigger casts of pointers. This can be unsafe if 
|		you create NxsBlocks that do not have the expected inheritance.  For example, if you create a class to 
|		read Taxa blocks, but do NOT derive this class from NxsTaxaBlockAPI then the casts will be unsafe.  If you 
|		do this, and you wish to use the factory API then you must write your own factories.
|
|	Note:
|			- NxsReader does not call delete on any of the blocks that it uses.  
|	
|
*/
class NxsReader
	{
	public:
		enum WarningHandlingMode
			{
			IGNORE_WARNINGS,
			WARNINGS_TO_STDERR,
			WARNINGS_TO_STDOUT,
			WARNINGS_ARE_ERRORS
			};
		enum	NxsTolerateFlags	/* Flags used with data member tolerate used to allow some flexibility with respect to the NEXUS format */
			{
			allowMissingInEquate	= 0x0001,	/* if set, equate symbols are allowed for missing data symbol */
			allowPunctuationInNames	= 0x0002	/* if set, some punctuation is allowed within tokens representing labels for taxa, characters, and sets */
			};
		enum NxsWarnLevel
			{
			UNCOMMON_SYNTAX_WARNING = 0,
			SKIPPING_CONTENT_WARNING = 1,
			OVERWRITING_CONTENT_WARNING = 2,
			DEPRECATED_WARNING = 3,
			AMBIGUOUS_CONTENT_WARNING = 4,
			ILLEGAL_CONTENT_WARNING = 5,
			PROBABLY_INCORRECT_CONTENT_WARNING = 6,
			FATAL_WARNING = 7
			};

						NxsReader();
		virtual			~NxsReader();

		bool			BlockListEmpty();
		unsigned		PositionInBlockList(NxsBlock *b);
		virtual void    Add(NxsBlock *newBlock);
		virtual void    AddFactory(NxsBlockFactory *);
		void			RemoveFactory(NxsBlockFactory *);
		void			Detach(NxsBlock *newBlock);
		void			Reassign(NxsBlock *oldb, NxsBlock *newb);
		virtual void	Execute(NxsToken& token, bool notifyStartStop = true);
		void			ReadFilepath(const char *filename);
		void			ReadFilestream(std::istream & inf);
		virtual void	DebugReportBlock(NxsBlock &nexusBlock);

		const char		*NCLNameAndVersion();
		const char		*NCLCopyrightNotice();
		const char		*NCLHomePageURL();

		virtual void	ExecuteStarting();
		virtual void	ExecuteStopping();

		virtual bool	EnteringBlock(NxsString blockName);
		virtual void	ExitingBlock(NxsString blockName);

		virtual void	OutputComment(const NxsString &comment);
		/*The default NexusWarn behavior is to generate a NexusException for any
			warnLevel >= PROBABLY_INCORRECT_CONTENT_WARNING 
		 	and to ignore all other warnings.
		*/
		virtual void	NexusWarn(const std::string &s, NxsWarnLevel warnLevel, file_pos pos, long line, long col)
			{
			if (warnLevel >= PROBABLY_INCORRECT_CONTENT_WARNING)
				{
				NxsString e(s.c_str());
				throw NxsException(e, pos, line, col);
				}
			}
		void	NexusWarnToken(const std::string &m, NxsWarnLevel warnLevel, const ProcessedNxsToken &token)
			{
			NexusWarn(m , warnLevel, token.GetFilePosition(), token.GetLineNumber(), token.GetColumnNumber());
			}
		void	NexusWarnToken(const std::string &m, NxsWarnLevel warnLevel, const NxsToken &token)
			{
			NexusWarn(m , warnLevel, token.GetFilePosition(), token.GetFileLine(), token.GetFileColumn());
			}

		virtual void	NexusError(NxsString msg, file_pos pos, long line, long col);

		virtual void	SkippingDisabledBlock(NxsString blockName);
		virtual void	SkippingBlock(NxsString blockName);

		
		virtual void			ClearUsedBlockList();
		NxsBlock 			   *CreateBlockFromFactories(const std::string & currBlockName, NxsToken &token, NxsBlockFactory **sourceOfBlock = NULL);
		BlockReaderList 		GetUsedBlocksInOrder();
		BlockReaderList 		GetBlocksFromLastExecuteInOrder();
		BlockTypeToBlockList 	GetUsedBlocks();
		std::set<NxsBlock*> 	GetSetOfAllUsedBlocks();

		NxsTaxaBlockAPI 		*GetLastStoredTaxaBlock();
		NxsCharactersBlockAPI 	*GetLastStoredCharactersBlock();
		NxsTreesBlockAPI 		*GetLastStoredTreesBlock();

		NxsTaxaBlockAPI 		*GetTaxaBlockByTitle(const char *title, unsigned *nMatches);
		NxsCharactersBlockAPI 	*GetCharBlockByTitle(const char *title, unsigned *nMatches);
		NxsTreesBlockAPI 		*GetTreesBlockByTitle(const char *title, unsigned *nMatches);

		NxsTaxaBlockFactory 	*GetTaxaBlockFactory();
		//NxsCharactersBlockFactory 	*GetCharBlockFactory();
		//NxsTreesBlockFactory 	*GetTreesBlockFactory();

		void			SetTaxaBlockFactory(NxsTaxaBlockFactory *);
		//void			SetCharBlockFactory(NxsCharactersBlockFactory *);
		//void			SetTreesBlockFactory(NxsTreesBlockFactory *);

		virtual void    DeleteBlocksFromFactories();
		unsigned		RemoveBlockFromUsedBlockList(NxsBlock *);

		// throws away references to all blocks that that have been read. If the block
		// was registered with the reader, then "Reset" is called on the block. 
		//	If the block came from a factory then the reference to the block is
		//		removed from the reader (resulting in a memory leak if the client 
		//		code does not delete the block).
		// This can be called if the client would like to store the information
		//	from the NEXUS file, and get rid of the blocks to save memory (but 
		//	still maintain things like factories that were registered with the
		//	NxsReader and tweaks to the default settings).
		virtual void	ClearContent();

		///////////////////////////////////////////////////////////////////////
		// This function is useful after ClearContext() has been called.
		// Instances of blocks that have been read can be registered with reader
		//	so that future NEXUS files can be parsed with the context of those blocks.
		// For example one might want to reregister a NxsTaxaBlock before reading
		//	 a trees file, then you could call:
		//	reader->AddReadBlock("TAXA", taxaB);
		// NOTE: if you are using a PublicNexusReader, or MultiFormatReader
		//	you should almost certainly call the type-specific forms such as
		//	reader->AddReadTaxaBlock(taxaB);
		///////////////////////////////////////////////////////////////////////
		virtual void AddReadBlock(const NxsString &blockID, NxsBlock *block)
			{
			this->BlockReadHook(blockID, block);
			}
		
		///////////////////////////////////////////////////////////////////////
		//	Call cullIdenticalTaxaBlocks(true) before reading a file if you want
		// the reader to discard a TaxaBlock that is identical to a previous 
		// taxa block.  Use of this assumes that the reader of taxa blocks is 
		// a NxsTaxaBlockAPI instance.
		///////////////////////////////////////////////////////////////////////
		void cullIdenticalTaxaBlocks(bool v=true)
			{
			this->destroyRepeatedTaxaBlocks = v;
			}
		std::vector<std::string> GetAllTitlesForBlock(const NxsBlock *b) const;
	protected:
		static 			BlockReaderList parseFileWithReader(NxsReader & reader, const char *filepath, bool parsePrivateBlocks=true, bool storeTokenInfo=true);
		bool 			BlockIsASingeltonReader(NxsBlock *) const ;
		void 			BlockReadHook(const NxsString &currBlockName, NxsBlock *currBlock, NxsToken *token = NULL );
		bool 			ExecuteBlock(NxsToken &token, const NxsString &currBlockName, NxsBlock *currBlock, NxsBlockFactory * sourceOfBlock);
		bool			IsRepeatedTaxaBlock(const NxsTaxaBlockAPI *) const;
		NxsTaxaBlockAPI * GetOriginalTaxaBlock(const NxsTaxaBlockAPI *) const;
		void			RegisterAltTitle(const NxsBlock * b, std::string t);
		virtual void 	PostBlockReadingHook(NxsBlock &);


		NxsBlock		*blockList;	/* pointer to first block in list of blocks */
		NxsBlock		*currBlock;	/* pointer to current block in list of blocks */
		typedef std::list<NxsBlockFactory *> BlockFactoryList;
		NxsTaxaBlockFactory *taxaBlockFactory;
		BlockFactoryList factories; /* list of pointers to factories capable of creating NxsBlock objects*/
		bool destroyRepeatedTaxaBlocks;
		
	private:
		NxsBlock 		*FindBlockOfTypeByTitle(const std::string &btype, const char *title, unsigned *nMatches);
		NxsBlock		*FindBlockByTitle(const BlockReaderList & chosenBlockList, const char *title, unsigned *nMatches);
		NxsBlock 		*GetLastStoredBlockByID(const std::string &key);
		void 			NewBlockTitleCheckHook(const std::string &blockname, NxsBlock *p, NxsToken *token);
		void			AddBlockToUsedBlockList(const std::string &, NxsBlock *, NxsToken *);
		bool 			ReadUntilEndblock(NxsToken &token, const std::string & currBlockName);
		
		BlockReaderList blocksInOrder;
		BlockReaderList lastExecuteBlocksInOrder;
		BlockTypeToBlockList blockTypeToBlockList;
		typedef std::pair<unsigned, std::list<std::string> > NxsBlockTitleHistory;
		typedef std::map<std::string, NxsBlockTitleHistory > NxsBlockTitleHistoryMap;
		NxsBlockTitleHistoryMap blockTitleHistoryMap;
		std::map<const NxsBlock *, std::list<std::string> > blockTitleAliases; // to deal with culling blocks and then using the titles of culled copies

	};

typedef NxsBlock NexusBlock;
typedef NxsReader Nexus;

class ExceptionRaisingNxsReader : public NxsReader
	{
	public:
		ExceptionRaisingNxsReader(NxsReader::WarningHandlingMode mode=NxsReader::WARNINGS_TO_STDERR)
			:warnMode(mode),
			warningToErrorThreshold(PROBABLY_INCORRECT_CONTENT_WARNING)
			{}
		static BlockReaderList parseFileOrThrow(const char *filepath, 
												NxsReader::WarningHandlingMode mode = NxsReader::WARNINGS_TO_STDERR, 
												bool parsePrivateBlocks=true, 
												bool storeTokenInfo=true);
		void NexusError(NxsString msg, file_pos pos, long line, long col)
			{
			throw NxsException(msg, pos, line, col);
			}
		virtual void NexusWarn(const std::string & msg, NxsWarnLevel level, file_pos pos, long line, long col);		
	
		void SkippingBlock(NxsString blockName);
		void SkippingDisabledBlock(NxsString blockName);
		void SetWarningToErrorThreshold(int t)
			{
			warningToErrorThreshold = t;
			}
		virtual void ClearContent() 
			{
			NxsReader::ClearContent();
			}
	private:
		NxsReader::WarningHandlingMode warnMode;
		int warningToErrorThreshold;
	};

class DefaultErrorReportNxsReader : public NxsReader
	{
	public:
		static BlockReaderList parseFile(const char *filepath, std::ostream * stdOutstream, std::ostream * errOutstream, bool parsePrivateBlocks=true, bool storeTokenInfo=true);

		DefaultErrorReportNxsReader(std::ostream * stdOutstream, std::ostream * errOutstream)
			:NxsReader(),
			stdOut(stdOutstream),
			errOut(errOutstream)
			{
			}

		virtual ~DefaultErrorReportNxsReader() {}
	
		virtual bool EnteringBlock(NxsString )
			{
			return true;	
			}
	
		void SkippingBlock(NxsString blockName)
			{
			if (stdOut != 0L)
				{
				*stdOut << "[!Skipping unknown block (" << blockName << ")...]\n";
				stdOut->flush();
				}
			}

		void SkippingDisabledBlock(NxsString blockName) 
			{
			if (stdOut != 0L)
				{
				*stdOut << "[!Skipping disabled block (" << blockName << ")...]\n";
				stdOut->flush();
				}
			}

		void NexusWarn(const std::string & msg, NxsWarnLevel warnLevel, file_pos pos, long line, long col)
			{
			if (warnLevel >= PROBABLY_INCORRECT_CONTENT_WARNING)
				{
				NxsString e(msg.c_str());
				throw NxsException(e, pos, line, col);
				}

			if (errOut != 0)
				{
				*errOut << "\nWarning:  ";
				if (line >= 0)
					*errOut << "at line " << line << ", column " << col << " (file position " << pos << "):\n";
				*errOut  << msg << std::endl;
				}
			else if (stdOut != 0L)
				{
				*stdOut << "\nWarning:  ";
				if (line >= 0)
					*stdOut << "at line " << line << ", column " << col << " (file position " << pos << "):\n";
				*stdOut  << msg << std::endl;
				}
			}
			
		void NexusError(NxsString msg, file_pos pos, long line, long col)
			{
			NexusWarn(msg, NxsReader::FATAL_WARNING, pos, line, col);
			throw NxsException(msg, pos, line, col);
			}

		std::ostream * stdOut;
		std::ostream * errOut;
	};

/*----------------------------------------------------------------------------------------------------------------------
|	Returns a map from all block ids that have been read to all instances that the NxsReader knows have been read and 
|		have NOT been cleared.
|	NOTE:  If the factory interface to NCL is not being used this may not be a complete list of all of the blocks that 
|		have been read!!!
*/
inline BlockTypeToBlockList NxsReader::GetUsedBlocks()
	{
	return blockTypeToBlockList;
	}

inline bool NxsReader::IsRepeatedTaxaBlock(const NxsTaxaBlockAPI * testB) const
	{
	return (GetOriginalTaxaBlock(testB) != NULL);
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Returns a list of  all blocks  that the NxsReader knows have been read and have NOT been cleared.
|	NOTE:  If the factory interface to NCL is not being used this may not be a complete list of all of the blocks that 
|		have been read!!!
*/
inline BlockReaderList NxsReader::GetUsedBlocksInOrder()
	{
	return blocksInOrder;
	}
	
inline BlockReaderList NxsReader::GetBlocksFromLastExecuteInOrder()
	{
	return lastExecuteBlocksInOrder;
	}


/*----------------------------------------------------------------------------------------------------------------------
|	Clears the lists of all of the blocks that have been read.
|	NOTE: does not free any memory or call Reset() on any blocks"!!
*/
inline void NxsReader::ClearUsedBlockList()
	{
	blockTypeToBlockList.clear();
	}
	
inline NxsTaxaBlockFactory * NxsReader::GetTaxaBlockFactory()
	{
	return this->taxaBlockFactory;
	}



#endif

