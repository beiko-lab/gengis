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



#include <istream>
#include <fstream>
#include "ncl/nxspublicblocks.h"
#include "ncl/nxsreader.h"
using namespace std;



BlockReaderList ExceptionRaisingNxsReader::parseFileOrThrow(
    const char *filepath, /* path of file to parse */
    NxsReader::WarningHandlingMode mode, 
    bool parsePrivateBlocks, /* true to store the commands found in  private blocks */
    bool storeTokenInfo)
    {
    ExceptionRaisingNxsReader nexusReader(mode);
    return NxsReader::parseFileWithReader(nexusReader, filepath, parsePrivateBlocks, storeTokenInfo);
    }

BlockReaderList DefaultErrorReportNxsReader::parseFile(
    const char *filepath, /* path of file to parse */
    std::ostream * stdOutstream, 
    std::ostream * errOutstream,
    bool parsePrivateBlocks, /* true to store the commands found in  private blocks */
    bool storeTokenInfo)
    {
    DefaultErrorReportNxsReader nexusReader(stdOutstream, errOutstream);
    return NxsReader::parseFileWithReader(nexusReader, filepath, parsePrivateBlocks, storeTokenInfo);
    }

/*==============================================================================
|   Returns a list of NxsBlock pointers (which the caller must delete)
|   corresponding to the NxsBlocks found in the file.
|   Raises NxsExceptions on errors.
*/
BlockReaderList NxsReader::parseFileWithReader(
    NxsReader & nexusReader,
    const char *filepath, /* path of file to parse */
    bool parsePrivateBlocks, /* true to store the commands found in  private blocks */
    bool storeTokenInfo) /* true for storage of full token info (such as file position) for private blocks */
    {
    if (!filepath)
        nexusReader.NexusError("Invalid (NULL) file specified to be parsed", 0, -1, -1);
    ifstream inf(filepath, ios::binary);
    if (!inf.good())
        {
        NxsString err;
        err << "Could not parse the file \"" << filepath <<"\"";
        nexusReader.NexusError(err, 0, -1, -1);
        }
    cerr << "Creating token" <<endl;
	NxsToken token(inf);	
	NxsDefaultPublicBlockFactory factory(parsePrivateBlocks, storeTokenInfo);
	nexusReader.AddFactory(&factory);
	try {
        cerr << "Executing" <<endl;
	    nexusReader.Execute(token);
	    }
	catch(...) 
	    {
        nexusReader.RemoveFactory(&factory);
        throw;
	    }
	nexusReader.RemoveFactory(&factory);
	BlockReaderList brl = nexusReader.GetBlocksFromLastExecuteInOrder();
	return brl;
    }


void NxsStoreTokensBlockReader::Reset()
	{
	NxsBlock::Reset();
	commandsRead.clear();
	}

void NxsStoreTokensBlockReader::ReportConst(std::ostream &out) const
	{
	out << id << " block contains ";
	if (storeAllTokenInfo)
		{
		out << (unsigned)commandsRead.size() << " commands:\n";
		for (std::list<ProcessedNxsCommand>::const_iterator cIt = commandsRead.begin(); cIt != commandsRead.end(); ++cIt)
			{
			const ProcessedNxsToken & t = (*cIt)[0];
			out << "    " << t.GetToken() << "\n";
			}
		}
	else
		{
		out << (unsigned)justTokens.size() << " commands:\n";
		for (ListVecString::const_iterator cIt = justTokens.begin(); cIt != justTokens.end(); ++cIt)
			out << "    " << cIt->at(0) << "\n";
		}
	}

void NxsStoreTokensBlockReader::ReadCommand(
  NxsToken &token)	/* the token used to read from in */
	{
	if (storeAllTokenInfo)
		{
		ProcessedNxsCommand fullTokens;
		token.ProcessAsCommand(&fullTokens);
		if (!fullTokens.empty())
			commandsRead.push_back(fullTokens);
		}
	else
		{
		VecString justString;
		while (!token.Equals(";"))
			{
			justString.push_back(token.GetToken());
			token.GetNextToken();
			}
		if (!justString.empty())
			justTokens.push_back(justString);
		}
	}
	
void NxsStoreTokensBlockReader::Read(
  NxsToken &token)	/* the token used to read from in */
	{
	isEmpty = false;
	isUserSupplied = true;
	NxsString begcmd("BEGIN ");
	begcmd += this->id;
	DemandEndSemicolon(token, begcmd.c_str());

	for(;;)
		{
		token.GetNextToken();
        if (token.Equals("END") || token.Equals("ENDBLOCK"))
            {
            HandleEndblock(token);
            return ;
            }        
		this->ReadCommand(token);
		}
	}

void NxsStoreTokensBlockReader::WriteAsNexus(std::ostream &out) const
	{
	out << "BEGIN " << NxsString::GetEscaped(this->id) << ";\n";
	if (storeAllTokenInfo)
		{
		for (std::list<ProcessedNxsCommand>::const_iterator cIt = commandsRead.begin(); cIt != commandsRead.end(); ++cIt)
			{
			const ProcessedNxsCommand & t = *cIt;
			if (WriteCommandAsNexus(out, t))
    			out << '\n';
			}
		}
	else
		{
		for (ListVecString::const_iterator cIt = justTokens.begin(); cIt != justTokens.end(); ++cIt)
			{
			const VecString & t = *cIt;
			out << "   ";
			for (VecString::const_iterator wIt = t.begin(); wIt != t.end(); ++wIt)
				out << ' ' << NxsString::GetEscaped(*wIt);
			out << ";\n";
			}
		}
	WriteSkippedCommands(out);
	out << "END;\n";
	}


NxsBlock  *NxsDefaultPublicBlockFactory::GetBlockReaderForID(const std::string & id, NxsReader *reader, NxsToken *token)
	{
	if (id == "ASSUMPTIONS" || id == "SETS")
		return assumpBlockFact.GetBlockReaderForID(id, reader, token);
	if (id == "CHARACTERS")
		return charBlockFact.GetBlockReaderForID(id, reader, token);
	if (id == "DATA")
		return dataBlockFact.GetBlockReaderForID(id, reader, token);
	if (id == "DISTANCES")
		return distancesBlockFact.GetBlockReaderForID(id, reader, token);
	if (id == "TAXA")
		return taxaBlockFact.GetBlockReaderForID(id, reader, token);
	if (id == "TREES")
		return treesBlockFact.GetBlockReaderForID(id, reader, token);
	if (id == "UNALIGNED")
		return unalignedBlockFact.GetBlockReaderForID(id, reader, token);
	if (tokenizeUnknownBlocks)
		{
		NxsStoreTokensBlockReader * nb = new NxsStoreTokensBlockReader(id, storeTokenInfoArg);
		nb->SetImplementsLinkAPI(false);
    	return nb;
        }
	return NULL;
	}

PublicNexusReader::PublicNexusReader(const int blocksToRead, NxsReader::WarningHandlingMode warnModeArg)
	:ExceptionRaisingNxsReader(warnModeArg),
	assumptionsBlockTemplate(0L),
	charactersBlockTemplate(0L),
	dataBlockTemplate(0L),
	distancesBlockTemplate(0L),
	storerBlockTemplate(0L),
	taxaBlockTemplate(0L),
	treesBlockTemplate(0L),
	unalignedBlockTemplate(0L)
{
	this->AddFactory(&cloneFactory);
	
	taxaBlockTemplate = new NxsTaxaBlock();
	taxaBlockTemplate->SetImplementsLinkAPI(false);
	cloneFactory.AddPrototype(taxaBlockTemplate);
			
	if (blocksToRead & NEXUS_ASSUMPTIONS_BLOCK_BIT)
		{
		assumptionsBlockTemplate = new NxsAssumptionsBlock(0L);
		assumptionsBlockTemplate->SetImplementsLinkAPI(true);
		cloneFactory.AddPrototype(assumptionsBlockTemplate, "ASSUMPTIONS");
		cloneFactory.AddPrototype(assumptionsBlockTemplate, "SETS");
		cloneFactory.AddPrototype(assumptionsBlockTemplate, "CODONS");
		}

	if (blocksToRead & NEXUS_TREES_BLOCK_BIT)
		{
		treesBlockTemplate = new NxsTreesBlock(NULL);
		treesBlockTemplate->SetCreateImpliedBlock(true);
		treesBlockTemplate->SetImplementsLinkAPI(true);
		treesBlockTemplate->SetProcessAllTreesDuringParse(true);
		treesBlockTemplate->SetAllowImplicitNames(true);
		treesBlockTemplate->SetWriteFromNodeEdgeDataStructure(true);
		cloneFactory.AddPrototype(treesBlockTemplate);
		}
	if (blocksToRead & NEXUS_CHARACTERS_BLOCK_BIT)
		{
		charactersBlockTemplate = new NxsCharactersBlock(NULL, NULL);
		charactersBlockTemplate->SetCreateImpliedBlock(true);
		charactersBlockTemplate->SetImplementsLinkAPI(true);
		charactersBlockTemplate->SetSupportMixedDatatype(true);
		charactersBlockTemplate->SetConvertAugmentedToMixed(true);
		
		dataBlockTemplate = new NxsDataBlock(NULL, NULL);
		dataBlockTemplate->SetCreateImpliedBlock(true);
		dataBlockTemplate->SetImplementsLinkAPI(true);
		dataBlockTemplate->SetSupportMixedDatatype(true);
		dataBlockTemplate->SetConvertAugmentedToMixed(true);
		cloneFactory.AddPrototype(charactersBlockTemplate, "CHARACTERS");
		cloneFactory.AddPrototype(dataBlockTemplate, "DATA");
		}
	if (blocksToRead & NEXUS_UNALIGNED_BLOCK_BIT)
		{
		unalignedBlockTemplate = new NxsUnalignedBlock(NULL);
		unalignedBlockTemplate->SetCreateImpliedBlock(true);
		unalignedBlockTemplate->SetImplementsLinkAPI(true);
		cloneFactory.AddPrototype(unalignedBlockTemplate);
		}
	if (blocksToRead & NEXUS_DISTANCES_BLOCK_BIT)
		{
		distancesBlockTemplate = new NxsDistancesBlock(NULL);
		distancesBlockTemplate->SetCreateImpliedBlock(true);
		distancesBlockTemplate->SetImplementsLinkAPI(true);
		cloneFactory.AddPrototype(distancesBlockTemplate);
		}
	if (blocksToRead & NEXUS_UNKNOWN_BLOCK_BIT)
		{
		std::string emptyString;
		storerBlockTemplate = new NxsStoreTokensBlockReader(emptyString, true);
		storerBlockTemplate->SetImplementsLinkAPI(false);
		cloneFactory.AddDefaultPrototype(storerBlockTemplate);
		}
}

void PublicNexusReader::Execute(NxsToken& token, bool notifyStartStop)
{
	NxsReader::Execute(token, notifyStartStop);
	PostExecuteHook();
}
				
void PublicNexusReader::PostExecuteHook()
{
	BlockReaderList blocks = GetBlocksFromLastExecuteInOrder();
	for (BlockReaderList::const_iterator bIt = blocks.begin(); bIt != blocks.end(); ++bIt)
		{
		NxsBlock * b = *bIt;
		const std::string id = b->GetID();
		const std::string capId = NxsString::get_upper(id);
		const char * capIdP = capId.c_str();
		if (strcmp(capIdP, "TAXA") == 0)
			taxaBlockVec.push_back(static_cast<NxsTaxaBlock *>(b));
		else if (strcmp(capIdP, "TREES") == 0)
			treesBlockVec.push_back(static_cast<NxsTreesBlock *>(b));
		else if ((strcmp(capIdP, "CHARACTERS") == 0) || (strcmp(capIdP, "DATA") == 0))
			charactersBlockVec.push_back(static_cast<NxsCharactersBlock *>(b));
		else if ((strcmp(capIdP, "ASSUMPTIONS") == 0) || (strcmp(capIdP, "SETS") == 0) || (strcmp(capIdP, "CODONS") == 0))
			assumptionsBlockVec.push_back(static_cast<NxsAssumptionsBlock *>(b));
		else if (strcmp(capIdP, "DISTANCES") == 0)
			distancesBlockVec.push_back(static_cast<NxsDistancesBlock *>(b));
		else if (strcmp(capIdP, "UNALIGNED") == 0)
			unalignedBlockVec.push_back(static_cast<NxsUnalignedBlock *>(b));
		else 
			{
			storerBlockVec.push_back(static_cast<NxsStoreTokensBlockReader *>(b));
			}
		}
}

void PublicNexusReader::AddFactory(NxsBlockFactory *f)
{
	if (f == &cloneFactory)
		NxsReader::AddFactory(f);
	else
		{
		assert(false);
		}
}

PublicNexusReader::~PublicNexusReader()
{
	delete assumptionsBlockTemplate;
	delete charactersBlockTemplate;
	delete dataBlockTemplate;
	delete distancesBlockTemplate;
	delete storerBlockTemplate;
	delete taxaBlockTemplate;
	delete treesBlockTemplate;
	delete unalignedBlockTemplate;
}

unsigned PublicNexusReader::GetNumAssumptionsBlocks(const NxsTaxaBlock *taxa) const
	{
	unsigned n = 0;
	std::vector<NxsAssumptionsBlock *>::const_iterator bIt = assumptionsBlockVec.begin();
	for (; bIt != assumptionsBlockVec.end(); ++bIt)
		{
		NxsAssumptionsBlock * b = *bIt;
		if (taxa && taxa != b->taxa)
			continue;
		n++;
		}
	return n;
	}
	
NxsAssumptionsBlock * PublicNexusReader::GetAssumptionsBlock(const NxsTaxaBlock *taxa, unsigned index) const
	{
	unsigned n = 0;
	std::vector<NxsAssumptionsBlock *>::const_iterator bIt = assumptionsBlockVec.begin();
	for (; bIt != assumptionsBlockVec.end(); ++bIt)
		{
		NxsAssumptionsBlock * b = *bIt;
		if (taxa && taxa != b->taxa)
			continue;
		if (index == n)
			return b;
		n++;
		}
	return 0L;
	}

unsigned PublicNexusReader::GetNumAssumptionsBlocks( const NxsCharactersBlock * chars) const
	{
	unsigned n = 0;
	std::vector<NxsAssumptionsBlock *>::const_iterator bIt = assumptionsBlockVec.begin();
	for (; bIt != assumptionsBlockVec.end(); ++bIt)
		{
		NxsAssumptionsBlock * b = *bIt;
		if (chars && chars != b->charBlockPtr)
			continue;
		n++;
		}
	return n;
	}
	
NxsAssumptionsBlock * PublicNexusReader::GetAssumptionsBlock(const NxsCharactersBlock * chars, unsigned index) const
	{
	unsigned n = 0;
	std::vector<NxsAssumptionsBlock *>::const_iterator bIt = assumptionsBlockVec.begin();
	for (; bIt != assumptionsBlockVec.end(); ++bIt)
		{
		NxsAssumptionsBlock * b = *bIt;
		if (chars && chars != b->charBlockPtr)
			continue;
		if (index == n)
			return b;
		n++;
		}
	return 0L;
	}

unsigned PublicNexusReader::GetNumAssumptionsBlocks(const NxsTreesBlock *tree) const
	{
	unsigned n = 0;
	std::vector<NxsAssumptionsBlock *>::const_iterator bIt = assumptionsBlockVec.begin();
	for (; bIt != assumptionsBlockVec.end(); ++bIt)
		{
		NxsAssumptionsBlock * b = *bIt;
		if (tree && tree != b->treesBlockPtr)
			continue;
		n++;
		}
	return n;
	}
	
NxsAssumptionsBlock * PublicNexusReader::GetAssumptionsBlock(const NxsTreesBlock *tree, unsigned index) const
	{
	unsigned n = 0;
	std::vector<NxsAssumptionsBlock *>::const_iterator bIt = assumptionsBlockVec.begin();
	for (; bIt != assumptionsBlockVec.end(); ++bIt)
		{
		NxsAssumptionsBlock * b = *bIt;
		if (tree && tree != b->treesBlockPtr)
			continue;
		if (index == n)
			return b;
		n++;
		}
	return 0L;
	}

unsigned PublicNexusReader::GetNumCharactersBlocks(const NxsTaxaBlock *taxa) const
	{
	unsigned n = 0;
	std::vector<NxsCharactersBlock *>::const_iterator bIt = charactersBlockVec.begin();
	for (; bIt != charactersBlockVec.end(); ++bIt)
		{
		NxsCharactersBlock * b = *bIt;
		if (!taxa || taxa == b->taxa)
			n++;
		}
	return n;
	}
	
NxsCharactersBlock * PublicNexusReader::GetCharactersBlock(const NxsTaxaBlock *taxa, unsigned index) const
	{
	unsigned n = 0;
	std::vector<NxsCharactersBlock *>::const_iterator bIt = charactersBlockVec.begin();
	for (; bIt != charactersBlockVec.end(); ++bIt)
		{
		NxsCharactersBlock * b = *bIt;
		if (!taxa || taxa == b->taxa)
			{
			if (index == n)
				return b;
			n++;
			}
		}
	return 0L;
	}

unsigned PublicNexusReader::GetNumDistancesBlocks(const NxsTaxaBlock *taxa) const
	{
	unsigned n = 0;
	std::vector<NxsDistancesBlock *>::const_iterator bIt = distancesBlockVec.begin();
	for (; bIt != distancesBlockVec.end(); ++bIt)
		{
		NxsDistancesBlock * b = *bIt;
		if (!taxa || taxa == b->taxa)
			n++;
		}
	return n;
	}
	
NxsDistancesBlock * PublicNexusReader::GetDistancesBlock(const NxsTaxaBlock *taxa, unsigned index) const
	{
	unsigned n = 0;
	std::vector<NxsDistancesBlock *>::const_iterator bIt = distancesBlockVec.begin();
	for (; bIt != distancesBlockVec.end(); ++bIt)
		{
		NxsDistancesBlock * b = *bIt;
		if (!taxa || taxa == b->taxa)
			{
			if (index == n)
				return b;
			n++;
			}
		}
	return 0L;
	}

unsigned PublicNexusReader::GetNumUnalignedBlocks(const NxsTaxaBlock *taxa) const
	{
	unsigned n = 0;
	std::vector<NxsUnalignedBlock *>::const_iterator bIt = unalignedBlockVec.begin();
	for (; bIt != unalignedBlockVec.end(); ++bIt)
		{
		NxsUnalignedBlock * b = *bIt;
		if (!taxa || taxa == b->taxa)
			n++;
		}
	return n;
	}
	
NxsUnalignedBlock * PublicNexusReader::GetUnalignedBlock(const NxsTaxaBlock *taxa, unsigned index) const
	{
	unsigned n = 0;
	std::vector<NxsUnalignedBlock *>::const_iterator bIt = unalignedBlockVec.begin();
	for (; bIt != unalignedBlockVec.end(); ++bIt)
		{
		NxsUnalignedBlock * b = *bIt;
		if (!taxa || taxa == b->taxa)
			{
			if (index == n)
				return b;
			n++;
			}
		}
	return 0L;
	}


unsigned PublicNexusReader::GetNumTreesBlocks(const NxsTaxaBlock *taxa) const
	{
	unsigned n = 0;
	std::vector<NxsTreesBlock *>::const_iterator bIt = treesBlockVec.begin();
	for (; bIt != treesBlockVec.end(); ++bIt)
		{
		NxsTreesBlock * b = *bIt;
		if (!taxa || taxa == b->taxa)
			n++;
		}
	return n;
	}
	
NxsTreesBlock * PublicNexusReader::GetTreesBlock(const NxsTaxaBlock *taxa, unsigned index) const
	{
	unsigned n = 0;
	std::vector<NxsTreesBlock *>::const_iterator bIt = treesBlockVec.begin();
	for (; bIt != treesBlockVec.end(); ++bIt)
		{
		NxsTreesBlock * b = *bIt;
		if (!taxa || taxa == b->taxa)
			{
			if (index == n)
				return b;
			n++;
			}
		}
	return 0L;
	}

unsigned PublicNexusReader::GetNumUnknownBlocks() const
	{
	return (unsigned)storerBlockVec.size();
	}
	
NxsStoreTokensBlockReader * PublicNexusReader::GetUnknownBlock(unsigned index) const
	{
	if (index < storerBlockVec.size())
		return storerBlockVec[index];
	return 0L;
	}

unsigned PublicNexusReader::GetNumTaxaBlocks() const
	{
	return (unsigned)taxaBlockVec.size();
	}
	
NxsTaxaBlock * PublicNexusReader::GetTaxaBlock(unsigned index) const
	{
	if (index < taxaBlockVec.size())
		return taxaBlockVec[index];
	return 0L;
	}

void PublicNexusReader::ClearUsedBlockList()
	{
	NxsReader::ClearUsedBlockList();
	assumptionsBlockVec.clear();
	charactersBlockVec.clear();
	dataBlockVec.clear();
	distancesBlockVec.clear();
	storerBlockVec.clear();
	taxaBlockVec.clear();
	treesBlockVec.clear();
	unalignedBlockVec.clear();
	}
