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

#ifndef NCL_ASSUMPTIONSBLOCK_H
#define NCL_ASSUMPTIONSBLOCK_H

#include "ncl/nxsdefs.h"
#include "ncl/nxsblock.h"
#include "ncl/nxstreesblock.h"
#include "ncl/nxscharactersblock.h"
#include "ncl/nxstaxablock.h"

class NxsCharactersBlockAPI;
class NxsTaxaBlockAPI;

using std::vector;

class NxsAssumptionsBlockAPI
  : public NxsBlock
	{
	public:
  		virtual void	SetCallback(NxsCharactersBlockAPI *p) = 0;
  		

		
		virtual void SetCharBlockPtr(NxsCharactersBlockAPI * c, NxsBlockLinkStatus s) = 0;
		virtual void SetTaxaBlockPtr(NxsTaxaBlockAPI *, NxsBlockLinkStatus s) = 0;
		virtual void SetTreesBlockPtr(NxsTreesBlockAPI *, NxsBlockLinkStatus s) = 0;

		virtual NxsCharactersBlockAPI * GetCharBlockPtr(int *status=NULL) = 0;
		virtual NxsTaxaBlockAPI * GetTaxaBlockPtr(int *status=NULL) = 0;
		virtual NxsTreesBlockAPI * GetTreesBlockPtr(int *status=NULL) = 0;

		virtual void AddCharPartition(const std::string & name, const NxsPartition &) = 0;
		virtual void AddTaxPartition(const std::string & name, const NxsPartition &) = 0;
		virtual void AddTreePartition(const std::string & name, const NxsPartition &) = 0;
		virtual void AddCodeSet(const std::string & name, const NxsPartition &, bool asterisked) = 0;
		virtual void AddCodonPosSet(const std::string & name, const NxsPartition &, bool asterisked) = 0;

		virtual void FlagCharBlockAsUsed() = 0;
		virtual void FlagTaxaBlockAsUsed() = 0;
		virtual void FlagTreesBlockAsUsed() = 0;

  		virtual void ReadCharsetDef(NxsString charset_name, NxsToken &token, bool asterisked) = 0;
  		virtual void ReadExsetDef(NxsString charset_name, NxsToken &token, bool asterisked) = 0;
		virtual void ReadTaxsetDef(NxsString set_name, NxsToken &token, bool asterisked) = 0;
		virtual void ReadTreesetDef(NxsString set_name, NxsToken &token, bool asterisked) = 0;

		virtual NxsTransformationManager & GetNxsTransformationManagerRef() = 0;
		virtual NxsGeneticCodesManager & GetNxsGeneticCodesManagerRef() = 0;
 	};

/*----------------------------------------------------------------------------------------------------------------------
|	This class handles reading and storage for the NxsReader block ASSUMPTIONS. It overrides the member functions Read 
|	and Reset, which are abstract virtual functions in the base class NxsBlock. Adding a new data member? Don't forget
|	to:
|~
|	o Describe it in the class declaration using a C-style comment.
|	o Initialize it (unless it is self-initializing) in the constructor and re-initialize it in the Reset function.
|	o Describe the initial state in the constructor documentation.
|	o Delete memory allocated to it in both the destructor and Reset function.
|	o Report it in some way in the Report function.
|~
*/
class NxsAssumptionsBlock
  : public NxsAssumptionsBlockAPI
	{
	enum NameOfAssumpBlockAsRead
		{
		UNREAD_OR_GENERATED_BLOCK,
		ASSUMPTIONS_BLOCK_READ,
		SETS_BLOCK_READ,
		CODONS_BLOCK_READ
		};
		
	
	public:
							NxsAssumptionsBlock(NxsTaxaBlockAPI *t);
		virtual				~NxsAssumptionsBlock();

		virtual bool		CanReadBlockType(const NxsToken & token);

		void				ReplaceTaxaBlockPtr(NxsTaxaBlockAPI *tb);
		void				SetCallback(NxsCharactersBlockAPI *p);

		int					GetNumCharSets() const;
		void				GetCharSetNames(NxsStringVector &names) const;
		const NxsUnsignedSet *GetCharSet(NxsString nm) const;

		int					GetNumCharPartitions();
		void				GetCharPartitionNames(vector<std::string> &names);
		const NxsPartition		*GetCharPartition(std::string nm) const;

		int					GetNumTaxSets();
		void				GetTaxSetNames(NxsStringVector &names);
		NxsUnsignedSet		&GetTaxSet(NxsString nm);

		int					GetNumExSets();
		void				GetExSetNames(NxsStringVector &names);
		NxsUnsignedSet		&GetExSet(NxsString nm);
		NxsString			GetDefExSetName();
		void				ApplyExset(NxsString nm);

		virtual void		Read(NxsToken& token);
		virtual void		Report(std::ostream& out) NCL_COULD_BE_CONST ;
		virtual void		Reset();
		virtual void 		WriteAsNexus(std::ostream &out) const;

		/*only used it the linkAPI is enabled*/
		virtual void		HandleLinkCommand(NxsToken & );
		virtual void		WriteLinkCommand(std::ostream &out) const;

		virtual VecBlockPtr		GetImpliedBlocks()
			{
			return GetCreatedTaxaBlocks();
			}

		int					GetCharLinkStatus() {return charLinkStatus;}
		int					GetTaxaLinkStatus() {return taxaLinkStatus;}
		int					GetTreesLinkStatus() {return treesLinkStatus;}

		void				FlagCharBlockAsUsed() {charLinkStatus |= NxsBlock::BLOCK_LINK_USED;}
		void				FlagTaxaBlockAsUsed() {taxaLinkStatus |= NxsBlock::BLOCK_LINK_USED;}
		void				FlagTreesBlockAsUsed() {treesLinkStatus |= NxsBlock::BLOCK_LINK_USED;}

		void				SetCharLinkStatus(NxsBlockLinkStatus s);
		void				SetTaxaLinkStatus(NxsBlockLinkStatus s);
		void				SetTreesLinkStatus(NxsBlockLinkStatus s);
		
		void				SetCharBlockPtr(NxsCharactersBlockAPI * c, NxsBlockLinkStatus s);
		void				SetTaxaBlockPtr(NxsTaxaBlockAPI *, NxsBlockLinkStatus s);
		void				SetTreesBlockPtr(NxsTreesBlockAPI *, NxsBlockLinkStatus s);
		NxsCharactersBlockAPI * GetCharBlockPtr(int *status=NULL);
		NxsTaxaBlockAPI *	GetTaxaBlockPtr(int *status=NULL);
		NxsTreesBlockAPI *	GetTreesBlockPtr(int *status=NULL);

		NxsTransformationManager & GetNxsTransformationManagerRef()
			{
			return transfMgr;
			}
		NxsGeneticCodesManager & GetNxsGeneticCodesManagerRef()
			{
			return codesMgr;
			}			
		virtual void AddCharPartition(const std::string & name, const NxsPartition &);
		virtual void AddTaxPartition(const std::string & name, const NxsPartition &);
		virtual void AddTreePartition(const std::string & name, const NxsPartition &);
		virtual void AddCodeSet(const std::string & name, const NxsPartition &, bool asterisked);
		virtual void AddCodonPosSet(const std::string & name, const NxsPartition &, bool asterisked);
		
		/*---------------------------------------------------------------------------------------
		| Results in aliasing of the taxa, trees, and characters blocks!
		*/
		NxsAssumptionsBlock &operator=(const NxsAssumptionsBlock &other)
			{
			CopyBaseBlockContents(static_cast<const NxsBlock &>(other));
			CopyAssumptionsContents(other);
			return *this;
			}

		/*---------------------------------------------------------------------------------------
		| Results in aliasing of the taxa, trees, and characters blocks!
		|
		| passedRefOfOwnedBlock is set to this->true to avoid double deletion (other 
		|	retains ownership of these blocks
		*/
		virtual void CopyAssumptionsContents(const NxsAssumptionsBlock &other)
			{
			taxa = other.taxa;
			charBlockPtr = other.charBlockPtr;
			treesBlockPtr = other.treesBlockPtr;
			charsets = other.charsets;
			taxsets = other.taxsets;
			treesets = other.treesets;
			exsets = other.exsets;
			charPartitions = other.charPartitions;
			taxPartitions = other.taxPartitions;
			treePartitions = other.treePartitions;
			def_exset = other.def_exset;
			charLinkStatus = other.charLinkStatus;
			taxaLinkStatus = other.taxaLinkStatus;
			treesLinkStatus = other.treesLinkStatus;
			passedRefOfOwnedBlock = true;
			readAs = other.readAs;
			transfMgr = other.transfMgr;
			codesMgr = other.codesMgr;
			createdSubBlocks = other.createdSubBlocks;
			polyTCountValue = other.polyTCountValue;
			gapsAsNewstate = other.gapsAsNewstate;
			codonPosSets = other.codonPosSets;
			def_codonPosSet = other.def_codonPosSet;
			codeSets = other.codeSets;
			def_codeSet = other.def_codeSet;
			}

		virtual NxsAssumptionsBlock * Clone() const
			{
			NxsAssumptionsBlock * a = new NxsAssumptionsBlock(taxa);
			*a = *this;
			return a;
			}
			

	protected:
		typedef std::vector<NxsAssumptionsBlockAPI *> VecAssumpBlockPtr;

		virtual void 		ReadCharsetDef(NxsString charset_name, NxsToken &token, bool asterisked);
		virtual void 		ReadExsetDef(NxsString charset_name, NxsToken &token, bool asterisked);
		virtual void 		ReadTreesetDef(NxsString set_name, NxsToken &token, bool asterisked);
		virtual void 		ReadTaxsetDef(NxsString set_name, NxsToken &token, bool asterisked);


		VecBlockPtr 		GetCreatedTaxaBlocks();
		virtual unsigned	TaxonLabelToNumber(NxsString s) const;

		void				HandleCharPartition(NxsToken& token);
		void				HandleCharSet(NxsToken& token);
		void				HandleCodeSet(NxsToken& token);
		void				HandleCodonPosSet(NxsToken& token);
		void				HandleExSet(NxsToken& token);
		void				HandleOptions(NxsToken & token);
		void				HandleTaxPartition(NxsToken& token);
		void				HandleTaxSet(NxsToken& token);
		void				HandleTreePartition(NxsToken& token);
		void				HandleTreeSet(NxsToken& token);
		void				HandleTypeSet(NxsToken& token);
		void				HandleUserType(NxsToken& token);
		void				HandleWeightSet(NxsToken& token);
		
		void				WriteCharSet(std::ostream &out) const
			{
			NxsWriteSetCommand("CHARSET", charsets, out);
			}
		void				WriteCharPartition(std::ostream &out) const
			{
			NxsWritePartitionCommand("CharPartition", charPartitions, out);
			}
		void				WriteExSet(std::ostream &out) const
			{
			NxsWriteSetCommand("EXSET", exsets, out, def_exset.c_str());
			}
		void				WriteOptions(std::ostream &out) const;
		void				WriteTaxPartition(std::ostream &out) const
			{
			NxsWritePartitionCommand("TaxPartition", taxPartitions, out);
			}
		void				WriteTaxSet(std::ostream &out) const
			{
			NxsWriteSetCommand("TAXSET", taxsets, out);
			}
		void				WriteTreePartition(std::ostream &out) const
			{
			NxsWritePartitionCommand("TreePartition", treePartitions, out);
			}
		void				WriteTreeSet(std::ostream &out) const
			{
			NxsWriteSetCommand("TREESET", treesets, out);
			}
		void WriteCodeSet(std::ostream &out) const
			{
			NxsWritePartitionCommand("CodeSet", codeSets, out, def_codeSet.c_str());
			}
		void WriteCodonPosSet(std::ostream &out) const
			{
			NxsWritePartitionCommand("CodonPosSet", codonPosSets, out, def_codonPosSet.c_str());
			}

	private:
		NxsAssumptionsBlockAPI  *GetAssumptionsBlockForCharTitle(const char *title, NxsToken &token, const char *cmd);
		NxsAssumptionsBlockAPI  *GetAssumptionsBlockForTaxaTitle(const char *title, NxsToken &token, const char *cmd);
		NxsAssumptionsBlockAPI  *GetAssumptionsBlockForTreesTitle(const char *title, NxsToken &token, const char *cmd);
		
		NxsAssumptionsBlockAPI  *GetAssumptionsBlockForCharBlock(NxsCharactersBlockAPI *, NxsBlockLinkStatus, NxsToken &token);
		NxsAssumptionsBlockAPI  *GetAssumptionsBlockForTaxaBlock(NxsTaxaBlockAPI *, NxsBlockLinkStatus, NxsToken &token);
		NxsAssumptionsBlockAPI  *GetAssumptionsBlockForTreesBlock(NxsTreesBlockAPI *, NxsBlockLinkStatus, NxsToken &token);
		
		NxsAssumptionsBlockAPI  *CreateNewAssumptionsBlock(NxsToken &token);
		NxsAssumptionsBlockAPI *DealWithPossibleParensInCharDependentCmd(NxsToken &token, const char *cmd, const std::vector<std::string> *unsupported = NULL, bool * isVect = false);
		bool					HasAssumptionsBlockCommands() const;
		bool					HasSetsBlockCommands() const;
		bool					HasCodonsBlockCommands() const;


		NxsTaxaBlockAPI			*taxa;				/* pointer to the NxsTaxaBlockAPI object */
		NxsCharactersBlockAPI	*charBlockPtr;		/* pointer to the NxsCharactersBlockAPI-derived object to be notified in the event of exset changes */
		NxsTreesBlockAPI		*treesBlockPtr;		/* pointer to the NxsTreesBlockAPI-derived object to be notified in the event of exset changes */



		NxsUnsignedSetMap	charsets;
		NxsUnsignedSetMap	taxsets;
		NxsUnsignedSetMap	treesets;
		NxsUnsignedSetMap	exsets;
		
		NxsPartitionsByName charPartitions;
		NxsPartitionsByName taxPartitions;
		NxsPartitionsByName treePartitions;

		NxsString			def_exset;			/* the default exset */
		
		NxsPartitionsByName codonPosSets;
		NxsString			def_codonPosSet;	/* the default codonPosSet */
		NxsPartitionsByName codeSets;
		NxsString			def_codeSet;		/* the default codeSet */
	
		int					charLinkStatus;
		int					taxaLinkStatus;
		int					treesLinkStatus;
		bool				passedRefOfOwnedBlock;
		NameOfAssumpBlockAsRead	readAs;
		NxsTransformationManager transfMgr;
		NxsGeneticCodesManager	codesMgr;

		std::vector<NxsAssumptionsBlockAPI *> createdSubBlocks;
		enum PolyTCountValue
			{
			POLY_T_COUNT_UNKNOWN,
			POLY_T_COUNT_MIN,
			POLY_T_COUNT_MAX
			};
		PolyTCountValue		polyTCountValue;
		bool				gapsAsNewstate;

		friend class NxsAssumptionsBlockFactory;
		friend class PublicNexusReader;
	};

class NxsAssumptionsBlockFactory
	:public NxsBlockFactory
	{
	public:
		virtual NxsAssumptionsBlock * GetBlockReaderForID(const std::string & id, NxsReader *reader, NxsToken *token);
	};

typedef NxsAssumptionsBlock AssumptionsBlock;	// for backward compatibility

#endif

