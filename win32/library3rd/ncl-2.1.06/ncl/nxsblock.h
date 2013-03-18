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
#ifndef NCL_NXSBLOCK_H
#define NCL_NXSBLOCK_H

#include <vector>
#include "ncl/nxsdefs.h"
#include "ncl/nxsexception.h"
#include "ncl/nxstoken.h"

class NxsReader;
class NxsBlock;
class NxsTaxaBlockAPI;

typedef std::vector<NxsBlock *> VecBlockPtr;
typedef std::vector<const NxsBlock *> VecConstBlockPtr;
typedef std::pair<const NxsBlock *, std::string> BlockUniqueID;
typedef std::map<BlockUniqueID, NxsBlock *> NxsBlockMapper;

class NxsLabelToIndicesMapper
	{
	public:
		virtual ~NxsLabelToIndicesMapper(){}
		virtual unsigned GetMaxIndex() const = 0;
		virtual unsigned GetIndicesForLabel(const std::string &label, NxsUnsignedSet *inds) const = 0;
			/* Returns true if the set replaces an old set*/
		virtual unsigned GetIndexSet(const std::string &label, NxsUnsignedSet * toFill) const = 0;
		virtual bool AddNewIndexSet(const std::string &label, const NxsUnsignedSet & inds) = 0;
		virtual bool AddNewPartition(const std::string &label, const NxsPartition & inds) = 0;

		/// AppendNewLabel is only overloaded in Taxa and State LabelToIndexMappers
		virtual unsigned AppendNewLabel(std::string &/*label*/)
			{
			throw NxsUnimplementedException("AppendNewLabel called on fixed label interface");
			}
	protected:
		static unsigned GetIndicesFromSets(const std::string &label, NxsUnsignedSet *inds, const NxsUnsignedSetMap & itemSets);
		static unsigned GetIndicesFromSetOrAsNumber(const std::string &label, NxsUnsignedSet *inds, const NxsUnsignedSetMap & itemSets, const unsigned maxInd, const char * itemType);
	};

class NxsSetVectorItemValidator;

std::string GetBlockIDTitleString(NxsBlock &);
/*----------------------------------------------------------------------------------------------------------------------
|	This is the base class from which all block classes are derived. A NxsBlock-derived class encapsulates a Nexus block
|	(e.g. DATA block, TREES block, etc.). The abstract virtual function Read must be overridden for each derived class 
|	to provide the ability to read everything following the block name (which is read by the NxsReader object) to the 
|	end or endblock statement. Derived classes must provide their own data storage and access functions. The abstract
|	virtual function Report must be overridden to provide some feedback to user on contents of block. The abstract
|	virtual function Reset must be overridden to empty the block of all its contents, restoring it to its 
|	just-constructed state.
*/
class NxsBlock
	{
	friend class NxsReader;

	public:
		enum NxsBlockLinkStatus
			{
			BLOCK_LINK_UNINITIALIZED = 	       0x00,
			BLOCK_LINK_UNKNOWN_STATUS =        0x01, /*backwards compatibility, this is the status of old block links*/
			BLOCK_LINK_TO_ONLY_CHOICE =        0x02,
			BLOCK_LINK_TO_MOST_RECENT =        0x04,
			BLOCK_LINK_TO_IMPLIED_BLOCK =      0x08,
			BLOCK_LINK_FROM_LINK_CMD =         0x10,
			BLOCK_LINK_EQUIVALENT_TO_IMPLIED = 0x20,
			BLOCK_LINK_UNUSED_MASK =           0x3F,
			BLOCK_LINK_USED =                  0x40
			};
		enum NxsCommandResult
			{
			STOP_PARSING_BLOCK,
			HANDLED_COMMAND,
			UNKNOWN_COMMAND
			};
							NxsBlock();
		virtual				~NxsBlock();

		virtual void SetNexus(NxsReader *nxsptr);
		NxsReader *GetNexus() const;
		virtual bool CanReadBlockType(const NxsToken & token)
			{
			return token.Equals(id);
			}
			
		NxsString			GetID() const;
		bool				IsEmpty() NCL_COULD_BE_CONST ;

		void				Enable();
		void				Disable();
		bool				IsEnabled() NCL_COULD_BE_CONST ;
		bool				IsUserSupplied() NCL_COULD_BE_CONST ;

		virtual unsigned	CharLabelToNumber(NxsString s) NCL_COULD_BE_CONST ;
		virtual unsigned	TaxonLabelToNumber(NxsString s) const;

		virtual void		SkippingCommand(NxsString commandName);

		virtual void		HandleBlockIDCommand(NxsToken &token);
		virtual void		HandleEndblock(NxsToken &token);
		virtual void		HandleLinkCommand(NxsToken &token);
		virtual void		HandleTitleCommand(NxsToken &token);

		virtual void		Report(std::ostream &out) NCL_COULD_BE_CONST ;
		virtual void		Reset();

		mutable NxsString	errormsg;			/* workspace for creating error messages */

			
		virtual VecBlockPtr GetImpliedBlocks();
		virtual VecConstBlockPtr	GetImpliedBlocksConst() const;
		BlockUniqueID		GetInstanceIdentifier() const
			{
			return BlockUniqueID(this, GetInstanceName());
			}
			
		const std::string  &GetInstanceName() const
			{
			return title;
			}
		virtual NxsBlock			*CloneBlock(NxsBlockMapper &memo) const;
		bool				ImplementsLinkAPI() const;
		void				SetImplementsLinkAPI(bool v);

		virtual void				WriteAsNexus(std::ostream &out) const;
		virtual void 				WriteBlockIDCommand(std::ostream &out) const;
		virtual void 				WriteLinkCommand(std::ostream &out) const;
		virtual void 				WriteTitleCommand(std::ostream &out) const;
		std::string GetTitle() const
			{
			return title;
			}
		void SetTitle(const std::string &t, bool autogeneratedTitle)
			{
			title = t;
			autoTitle = autogeneratedTitle;
			}
		bool IsAutoGeneratedTitle() const
			{
			return autoTitle;
			}
		void StoreSkippedCommands(bool v) 
			{
			storeSkippedCommands = v;
			}
		void ClearStoredSkippedCommands() 
			{
			skippedCommands.clear();
			}
			
		/*----------------------------------------------------------------------------------------
		| Copies all NxsBlock fields - execept  the `nexusReader` and `next` pointers.
		|	Aliasing of Block pointers results in very dangerous implication of copying for
		|	many subclasses of NxsBlock.
		| Copying of blocks should be restricted to empty blocks without linkages (e.g. 
		|	the CloneFactory mechanism requires some form of copy, but should typically be used with
		|	empty blocks.
		*/

		virtual void CopyBaseBlockContents(const NxsBlock &other)
			{
			errormsg = other.errormsg;
			isEmpty = other.isEmpty;
			isEnabled = other.isEnabled;
			isUserSupplied = other.isUserSupplied;
			id = other.id;
			title = other.title;
			blockIDString = other.blockIDString;
			linkAPI = other.linkAPI;
			storeSkippedCommands = other.storeSkippedCommands;
			skippedCommands = other.skippedCommands;
			autoTitle = other.autoTitle;
			}
			
		virtual NxsBlock * Clone() const
			{
			NxsBlock * b = new NxsBlock();
			b->CopyBaseBlockContents(*this);
			b->nexusReader = NULL;
			b->next = NULL;
			return b;
			}
		
		unsigned			ReadVectorPartitionDef(NxsPartition &np, NxsLabelToIndicesMapper &ltm, const std::string & partName, const char * ptype, const char * cmd, NxsToken & token, bool warnAsterisked, bool demandAllInds, NxsSetVectorItemValidator & v);
		void 				ReadPartitionDef(NxsPartition &np, NxsLabelToIndicesMapper &ltm, const std::string & partName, const char * ptype, const char * cmd, NxsToken & token, bool warnAsterisked, bool demandAllInds, bool storeAsPartition);
		virtual bool		TolerateEOFInBlock() const
			{
			return false;
			}
		void 				WarnDangerousContent(const std::string &s, const NxsToken &t);
		void 				WarnDangerousContent(const std::string &s, const ProcessedNxsToken &t);

		void				WriteBasicBlockCommands(std::ostream & out) const;
		virtual void		WriteSkippedCommands(std::ostream & out) const;
		// used internally to deal with multiple blocks spawning the same TAXA block
		virtual bool 		SwapEquivalentTaxaBlock(NxsTaxaBlockAPI * ) 
		{
			return false;
		}
	protected:
		void				SkipCommand(NxsToken & token);
		
		NxsCommandResult	HandleBasicBlockCommands(NxsToken & token);
		void				DemandEndSemicolon(NxsToken &token, const char *contextString) const;
		void				DemandEquals(NxsToken &token, const char *contextString) const;
		void				DemandEquals(ProcessedNxsCommand::const_iterator &tokIt, const ProcessedNxsCommand::const_iterator & endIt, const char *contextString) const ;
		void				DemandIsAtEquals(NxsToken &token, const char *contextString) const;
		unsigned 			DemandPositiveInt(NxsToken &token, const char *contextString) const;
		void				GenerateNxsException(NxsToken &token, const char *message = NULL) const;
		void				GenerateUnexpectedTokenNxsException(NxsToken &token, const char *expected = NULL) const;
		bool				isEmpty;			/* true if this object is currently storing data */
		bool				isEnabled;			/* true if this block is currently ebabled */
		bool				isUserSupplied;		/* true if this object has been read from a file; false otherwise */
		NxsReader			*nexusReader;		/* pointer to the Nexus file reader object */
		NxsBlock			*next;				/* DEPRECATED field pointer to next block in list */
		NxsString			id;					/* holds name of block (e.g., "DATA", "TREES", etc.) */
		std::string			title;				/* holds the title of the block empty by default */
		std::string 		blockIDString; 		/* Mesquite generates these. I don't know what they are for */
		bool				linkAPI;
		bool				autoTitle;			/* true if the title was generated internally*/
		bool				storeSkippedCommands;
		std::list<ProcessedNxsCommand> skippedCommands; /* commands accumulate by SkipCommand or by some other means */
		
		virtual void		Read(NxsToken &token);
		private:
			NxsBlock &operator=(const NxsBlock &other); /*intentionally not defined because of aliasing issues */

	};

class NxsBlockFactory
	{
	public:
		virtual ~NxsBlockFactory()
			{
			}
		virtual NxsBlock  *	GetBlockReaderForID(const std::string & id, NxsReader *reader, NxsToken *token) = 0;
		virtual void		BlockError(NxsBlock *b)
			{
			delete b;
			}
		virtual void BlockSkipped(NxsBlock *b)
			{
			delete b;
			}
	};

/*----------------------------------------------------------------------------------------------------------------------
|	Sets the nexusReader data member of the NxsBlock object to 'nxsptr'.
*/
inline void NxsBlock::SetNexus(
  NxsReader *nxsptr)	/* pointer to a NxsReader object */
	{
	nexusReader = nxsptr;
	}
/*----------------------------------------------------------------------------------------------------------------------
|	Gets the nexusReader data member of the NxsBlock object to 'nxsptr'.
*/
inline NxsReader * NxsBlock::GetNexus() const
	{
	return nexusReader;
	}
 

/*----------------------------------------------------------------------------------------------------------------------
|	Advances the token, and raise an exception if it is not an equals sign.
|
| 	Sets errormsg and raises a NxsException on failure.
|	`contextString` is used in error messages:
|		"Expecting '=' ${contextString} but found..."
*/
inline void NxsBlock::DemandEquals(NxsToken &token, const char *contextString) const
	{
	token.GetNextToken();
	DemandIsAtEquals(token, contextString);
	}
#endif


