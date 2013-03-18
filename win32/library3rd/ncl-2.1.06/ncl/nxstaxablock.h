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
#ifndef NCL_NXSTAXABLOCK_H
#define NCL_NXSTAXABLOCK_H

#include "ncl/nxsblock.h"

class NxsTaxaBlockAPI
  : public NxsBlock, public NxsLabelToIndicesMapper
  {
  public:
		virtual unsigned			AddTaxonLabel(const std::string & s) = 0;
		virtual void  				ChangeTaxonLabel(unsigned i, NxsString s) = 0;
		virtual unsigned			FindTaxon(const NxsString & label) const  = 0;
		virtual bool  				IsAlreadyDefined(const std::string & label) = 0;
		virtual unsigned			GetMaxTaxonLabelLength() = 0;
		virtual unsigned			GetNumTaxonLabels() const = 0;
		virtual unsigned			GetNTax() const = 0;
		virtual unsigned			GetNTaxTotal() const = 0;
		virtual NxsString 			GetTaxonLabel(unsigned i) const = 0;

		virtual std::vector<std::string> GetAllLabels() const
			{
			const unsigned n = GetNTaxTotal();
			std::vector<std::string> v(n);
			for (unsigned i = 0; i < n; ++i)
				v[i] = std::string(GetTaxonLabel(i).c_str());
			return v;
			}

		virtual unsigned			TaxLabelToNumber(const std::string &label) const = 0;
		
		virtual void 				HandleTaxLabels(NxsToken &token) = 0;
		virtual void 				WriteTaxLabelsCommand(std::ostream &out) const = 0;
		virtual void 				SetNtax(unsigned n) = 0;

		virtual unsigned		 	GetNumActiveTaxa() const = 0;
		virtual bool		 		IsActiveTaxon(unsigned i) const = 0;
		virtual unsigned		 	InactivateTaxa(const std::set<unsigned> &) = 0;
		virtual unsigned		 	ActivateTaxa(const std::set<unsigned> &) = 0;
		virtual unsigned		 	InactivateTaxon(unsigned ) = 0;
		virtual unsigned		 	ActivateTaxon(unsigned ) = 0;


  };

/*----------------------------------------------------------------------------------------------------------------------
|	This class handles reading and storage for the NxsReader block TAXA. It overrides the member functions Read and 
|	Reset, which are abstract virtual functions in the base class NxsBlock. The taxon names are stored in an vector of
|	strings (taxLabels) that is accessible through the member functions GetTaxonLabel(int), AddTaxonLabel(NxsString), 
|	ChangeTaxonLabel(int, NxsString), and GetNumTaxonLabels().
*/
class NxsTaxaBlock
  : public NxsTaxaBlockAPI
	{
	friend class NxsDataBlock;
	friend class NxsAllelesBlock;
	friend class NxsCharactersBlock;
	friend class NxsDistancesBlock;

	public:
							NxsTaxaBlock();
		virtual				~NxsTaxaBlock();

		virtual unsigned	AddTaxonLabel(const std::string & s);
		void  				ChangeTaxonLabel(unsigned i, NxsString s);
		unsigned			TaxLabelToNumber(const std::string &label) const;
		unsigned			FindTaxon(const NxsString & label) const;
		bool  				IsAlreadyDefined(const std::string &label);
		unsigned GetIndexSet(const std::string &label, NxsUnsignedSet * toFill) const
			{
			return NxsLabelToIndicesMapper::GetIndicesFromSets(label, toFill, taxSets);
			}
		unsigned			GetMaxTaxonLabelLength();
		unsigned			GetNTax() const;
		unsigned			GetNTaxTotal() const;
		unsigned			GetNumTaxonLabels() const;
		NxsString 			GetTaxonLabel(unsigned i) const;
		void 				HandleTaxLabels(NxsToken &token);
		bool 				NeedsQuotes(unsigned i);
		virtual void		Report(std::ostream &out) NCL_COULD_BE_CONST ;
		virtual void 		Reset();
		void 				SetNtax(unsigned n);
		void				WriteAsNexus(std::ostream &out) const;


		virtual unsigned		 	GetNumActiveTaxa() const;
		virtual bool		 		IsActiveTaxon(unsigned i) const;
		virtual unsigned		 	InactivateTaxa(const std::set<unsigned> &);
		virtual unsigned		 	ActivateTaxa(const std::set<unsigned> &);
		virtual unsigned		 	InactivateTaxon(unsigned );
		virtual unsigned		 	ActivateTaxon(unsigned );

		class NxsX_NoSuchTaxon {};	/* thrown if FindTaxon cannot locate a supplied taxon label in the taxLabels vector */

		void 				WriteTaxLabelsCommand(std::ostream &out) const;

		unsigned GetMaxIndex() const;
		unsigned GetIndicesForLabel(const std::string &label, NxsUnsignedSet *inds) const;
		bool AddNewIndexSet(const std::string &label, const NxsUnsignedSet & inds);
		bool AddNewPartition(const std::string &label, const NxsPartition & inds);

		/*----------------------------------------------------------------------
		| AppendNewLabel should not be called in most client code.  It is only
		| 	added because some blocks create their own taxa block on-the-fly.
		*/
		virtual unsigned AppendNewLabel(std::string &label)
			{
			while (dimNTax <= taxLabels.size())
				dimNTax++;
			return AddTaxonLabel(label);
			}
		
		NxsTaxaBlock &operator=(const NxsTaxaBlock &other)
			{
			Reset();
			CopyBaseBlockContents(static_cast<const NxsBlock &>(other));
			CopyTaxaContents(other);
			return *this;
			}

		void CopyTaxaContents(const NxsTaxaBlock &other)
			{
			taxLabels = other.taxLabels;
			labelToIndex = other.labelToIndex;
			dimNTax = other.dimNTax;
			taxSets = taxSets;
			taxPartitions = other.taxPartitions; 
			inactiveTaxa = other.inactiveTaxa;
			}
		NxsTaxaBlock * Clone() const
			{
			NxsTaxaBlock *taxa = new NxsTaxaBlock();
			*taxa = *this;
			return taxa;
			}
	protected:
		NxsStringVector	taxLabels;	/* storage for list of taxon labels */
		std::map<std::string, unsigned> labelToIndex;
		unsigned		dimNTax;
		NxsUnsignedSetMap taxSets;
		NxsPartitionsByName taxPartitions;
		std::set<unsigned> inactiveTaxa;
		
		virtual void 	Read(NxsToken &token);
		void CheckCapitalizedTaxonLabel(const std::string &s) const;
		unsigned CapitalizedTaxLabelToNumber(const std::string & s) const;
		void 			RemoveTaxonLabel(unsigned taxInd);
	};

class NxsTaxaBlockSurrogate
	{
	public:
		void SetCreateImpliedBlock(bool v)
        	{
        	createImpliedBlock = v;
        	}

		int GetTaxaLinkStatus() const 
			{
			return taxaLinkStatus;
			}

		void SetTaxaLinkStatus(NxsBlock::NxsBlockLinkStatus s);
		NxsTaxaBlockAPI * GetTaxaBlockPtr(int *status) const;

		virtual const std::string & GetBlockName() const = 0;

		virtual unsigned			GetNTax() const;
		virtual unsigned			GetNTaxTotal() const;
		virtual unsigned			GetNumActiveTaxa() const;
		virtual bool				IsActiveTaxon(unsigned i) const;
		virtual unsigned			InactivateTaxa(const std::set<unsigned> &);
		virtual unsigned			ActivateTaxa(const std::set<unsigned> &);
		virtual unsigned			InactivateTaxon(unsigned );
		virtual unsigned			ActivateTaxon(unsigned );
		
		NxsTaxaBlockSurrogate &operator=(const NxsTaxaBlockSurrogate &other)
			{
			ResetSurrogate();
			CopyTaxaBlockSurrogateContents(other);
			return *this;
			}
			
		/*
		|  Aliases the same taxa block as `other`, but `other` retains ownership!!
		*/
		virtual void CopyTaxaBlockSurrogateContents(const NxsTaxaBlockSurrogate &other)
			{
			ResetSurrogate();
			taxa = other.taxa;
			taxaLinkStatus = other.taxaLinkStatus;
			newtaxa = other.newtaxa;
			ownsTaxaBlock = false;
			passedRefOfOwnedBlock = other.passedRefOfOwnedBlock; 
			createImpliedBlock = other.createImpliedBlock;
			nxsReader = other.nxsReader;
			}

	protected:
		NxsTaxaBlockSurrogate(NxsTaxaBlockAPI *tb, NxsReader * reader)
			:taxa(tb),
			newtaxa(false),
			ownsTaxaBlock(false),
			passedRefOfOwnedBlock(false),
			createImpliedBlock(false),
			nxsReader(reader)
			{
			taxaLinkStatus = (tb == NULL ? NxsBlock::BLOCK_LINK_UNINITIALIZED : NxsBlock::BLOCK_LINK_TO_ONLY_CHOICE);
			}
		virtual ~NxsTaxaBlockSurrogate()
			{
			ResetSurrogate();
			}

		void SetTaxaBlockPtr(NxsTaxaBlockAPI *c, NxsBlock::NxsBlockLinkStatus s);
		
		VecBlockPtr GetCreatedTaxaBlocks();
		void FlagTaxaBlockAsUsed() 
			{
			taxaLinkStatus |= NxsBlock::BLOCK_LINK_USED;
			}
		
		void AssureTaxaBlock(bool allocBlock, NxsToken &, const char *cmd);
		void ResetSurrogate();
		void SetNexusReader(NxsReader *nxsptr)
			{
			nxsReader = nxsptr;
			}
		virtual void 			HandleTaxLabels(NxsToken & token);
		virtual void			HandleLinkTaxaCommand(NxsToken & );
		virtual void			WriteLinkTaxaCommand(std::ostream &out) const;
		bool					SurrogateSwapEquivalentTaxaBlock(NxsTaxaBlockAPI * tb);
		
		NxsTaxaBlockAPI			*taxa;				/* pointer to the TAXA block in which taxon labels are stored */
		int						taxaLinkStatus;
		bool					newtaxa;
		bool					ownsTaxaBlock;
		bool					passedRefOfOwnedBlock; 
		bool					createImpliedBlock; /*if true and NEWTAXA is read in the DIMENSIONS command, then a new TaxaBlock will be allocated (instead of resetting the TAXA block). false by default.*/
		NxsReader 				*nxsReader;
	};
// The following typedef maintains compatibility with existing code.
// The TaxaBlock class name is deprecated; please use NxsTaxaBlock instead.
//
typedef NxsTaxaBlock TaxaBlock;
class NxsTaxaBlockFactory
	:public NxsBlockFactory
	{
	public:
		virtual NxsTaxaBlock  *	GetBlockReaderForID(const std::string & id, NxsReader *reader, NxsToken *token);
	};

inline unsigned NxsTaxaBlock::GetNTax() const
	{
	return dimNTax;
	}
inline unsigned NxsTaxaBlock::GetNTaxTotal() const
	{
	return dimNTax;
	}

inline unsigned NxsTaxaBlock::CapitalizedTaxLabelToNumber(const std::string &r) const
	{
	std::map<std::string, unsigned>::const_iterator rIt = labelToIndex.find(r);
	if (rIt == labelToIndex.end())
		return 0;
	return rIt->second + 1;
	}

	
/*----------------------------------------------------------------------------------------------------------------------
|	Returns true if taxon `i' is active. If taxon `i' has been deleted, returns false. Assumes `i' is in the range 
|	[0..`ntax').
*/
inline bool NxsTaxaBlockSurrogate::IsActiveTaxon(
  unsigned taxInd) const	/* the taxon in question, in the range [0..`ntax') */
	{
	if (!taxa)
	    throw NxsNCLAPIException("Calling IsActiveTaxon on uninitialized block");
	return taxa->IsActiveTaxon(taxInd);
	}
inline unsigned NxsTaxaBlockSurrogate::GetNumActiveTaxa() const
	{
	if (!taxa)
	    throw NxsNCLAPIException("Calling GetNumActiveTaxa on uninitialized block");
	return taxa->GetNumActiveTaxa();
	}
inline unsigned NxsTaxaBlockSurrogate::InactivateTaxa(const std::set<unsigned> &s)
	{
	if (!taxa)
	    throw NxsNCLAPIException("Calling InactivateTaxa on uninitialized block");
	return taxa->InactivateTaxa(s);
	}
inline unsigned NxsTaxaBlockSurrogate::ActivateTaxa(const std::set<unsigned> &s)
	{
	if (!taxa)
	    throw NxsNCLAPIException("Calling ActivateTaxa on uninitialized block");
	return taxa->ActivateTaxa(s);
	}

inline unsigned NxsTaxaBlockSurrogate::InactivateTaxon(unsigned i)
	{
	if (!taxa)
	    throw NxsNCLAPIException("Calling InactivateTaxon on uninitialized block");
	return taxa->InactivateTaxon(i);
	}

inline unsigned NxsTaxaBlockSurrogate::ActivateTaxon(unsigned i)
	{
	if (!taxa)
	    throw NxsNCLAPIException("Calling ActivateTaxon on uninitialized block");
	return taxa->ActivateTaxon(i);
	}

inline unsigned NxsTaxaBlockSurrogate::GetNTax() const
	{
	if (!taxa)
	    throw NxsNCLAPIException("Calling GetNTax on uninitialized block");
	return taxa->GetNTax();
	}
inline unsigned NxsTaxaBlockSurrogate::GetNTaxTotal() const
	{
	if (!taxa)
	    throw NxsNCLAPIException("Calling GetNTaxTotal on uninitialized block");
	return taxa->GetNTaxTotal();
	}


inline unsigned NxsTaxaBlock::GetNumActiveTaxa() const
	{
	return GetNTax() - inactiveTaxa.size();
	}
inline bool NxsTaxaBlock::IsActiveTaxon(unsigned i) const
	{
	return i < GetNTax() && (inactiveTaxa.count(i) == 0);
	}
inline unsigned NxsTaxaBlock::InactivateTaxa(const std::set<unsigned> &s)
	{
	for (std::set<unsigned>::const_iterator sIt = s.begin(); sIt != s.end(); ++sIt)
		InactivateTaxon(*sIt);
	return GetNumActiveTaxa();
	}
	
inline unsigned NxsTaxaBlock::ActivateTaxa(const std::set<unsigned> &s)
	{
	for (std::set<unsigned>::const_iterator sIt = s.begin(); sIt != s.end(); ++sIt)
		ActivateTaxon(*sIt);
	return GetNumActiveTaxa();
	}
inline unsigned NxsTaxaBlock::InactivateTaxon(unsigned i)
	{
	if (i > GetNTax())
		throw NxsNCLAPIException("Taxon index out of range in InactivateTaxon");
	inactiveTaxa.insert(i);
	return GetNumActiveTaxa();
	}
inline unsigned NxsTaxaBlock::ActivateTaxon(unsigned i)
	{
	if (i > GetNTax())
		throw NxsNCLAPIException("Taxon index out of range in InactivateTaxon");
	inactiveTaxa.erase(i);
	return GetNumActiveTaxa();
	}


#endif
