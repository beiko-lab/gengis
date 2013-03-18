//	Copyright (C) 1999-2003 Paul O. Lewis and Mark T. Holder
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

#ifndef NCL_NXSMULTIFORMAT_H
#define NCL_NXSMULTIFORMAT_H
#include <iostream>
#include "ncl/nxsdefs.h"
#include "ncl/nxspublicblocks.h"



class FileToCharBuffer
{
		char prevChar;
		std::istream & inf;
		unsigned remaining;
		unsigned pos;
	public:
		unsigned totalSize;
	protected:
		unsigned lineNumber;
		unsigned prevNewlinePos;
	public:
		/* reads at most MAX_BUFFER_SIZE characters from inf into the buffer that is
		returned. The caller must delete the buffer.  On exit `len` will store the 
		length of the buffer.
		*/

		FileToCharBuffer(std::istream & instream);

		/* reads at most maxLen characters from `inf` into the `buffer`
		Returns false if no characters are read.
		If true is returned then `maxLen` will indicate the number of characters read.
		*/
		bool refillBuffer(unsigned offset);
		char current() const
			{
			return buffer[pos];
			}
		bool advance()
			{
			if (pos + 1 >= inbuffer)
				{
				if (!refillBuffer(0))
					return false;
				}
			else
				++pos;
			const char c = current();
			if (c == 13)
				{
				++lineNumber;
				prevNewlinePos = position();
				}
			else if (c == 10)
				{
				if (prev() != 13) 
					++lineNumber;
				prevNewlinePos = position();
				}
			return true;
			}
		char prev() const
			{
			if (pos == 0)
				return prevChar;
			return buffer[pos - 1];
			}
		~FileToCharBuffer()
			{
			delete [] buffer;
			}
		unsigned position() const
			{
			return totalSize +  pos - remaining - inbuffer;
			}
		unsigned line() const
			{
			return lineNumber;
			}
		unsigned column() const
			{
			unsigned p = position();
			if (p < prevNewlinePos)
				return 0;
			return p - prevNewlinePos;
			}
		char * buffer;
		unsigned inbuffer; 
		
};

class MultiFormatReader: public PublicNexusReader
{
	public:
		static std::vector<std::string> getFormatNames();
		enum DataFormatType
			{
				NEXUS_FORMAT,
				FASTA_DNA_FORMAT,
				FASTA_AA_FORMAT,
				FASTA_RNA_FORMAT,
				PHYLIP_DNA_FORMAT,
				PHYLIP_RNA_FORMAT,
				PHYLIP_AA_FORMAT,
				PHYLIP_DISC_FORMAT,
				INTERLEAVED_PHYLIP_DNA_FORMAT,
				INTERLEAVED_PHYLIP_RNA_FORMAT,
				INTERLEAVED_PHYLIP_AA_FORMAT,
				INTERLEAVED_PHYLIP_DISC_FORMAT,
				RELAXED_PHYLIP_DNA_FORMAT,
				RELAXED_PHYLIP_RNA_FORMAT,
				RELAXED_PHYLIP_AA_FORMAT,
				RELAXED_PHYLIP_DISC_FORMAT,
				INTERLEAVED_RELAXED_PHYLIP_DNA_FORMAT,
				INTERLEAVED_RELAXED_PHYLIP_RNA_FORMAT,
				INTERLEAVED_RELAXED_PHYLIP_AA_FORMAT,
				INTERLEAVED_RELAXED_PHYLIP_DISC_FORMAT,
				PHYLIP_TREE_FORMAT,
				RELAXED_PHYLIP_TREE_FORMAT,
				UNSUPPORTED_FORMAT // keep this last
			};
		static DataFormatType formatNameToCode(const std::string &);
		
		
		MultiFormatReader(const int blocksToRead = -1, NxsReader::WarningHandlingMode mode=NxsReader::WARNINGS_TO_STDERR)
			:PublicNexusReader(blocksToRead, mode)
			{}
		virtual ~MultiFormatReader(){}
		void ReadFilepath(const char * filepath, DataFormatType format);
		void readFastaFile(std::istream & inf, NxsCharactersBlock::DataTypesEnum dt);
	private:
		void addTaxaNames(const std::list<std::string> & taxaName, NxsTaxaBlockAPI * taxa);
		void moveDataToDataBlock(const std::list<std::string> & taxaNames, std::list<NxsDiscreteStateRow> & matList, const unsigned nchar, NxsDataBlock * dataB);
		void moveDataToMatrix(std::list<NxsDiscreteStateRow> & matList,  NxsDiscreteStateMatrix &mat);
		void moveDataToUnalignedBlock(const std::list<std::string> & taxaNames, std::list<NxsDiscreteStateRow> & matList, NxsUnalignedBlock * uB);
		bool readFastaSequences(FileToCharBuffer & ftcb, const NxsDiscreteDatatypeMapper &dm, std::list<std::string> & taxaNames, std::list<NxsDiscreteStateRow> & matList, size_t & longest);
		void readPhylipFile(std::istream & inf, NxsCharactersBlock::DataTypesEnum dt, bool relaxedNames, bool interleaved);
		void readPhylipTreeFile(std::istream & inf, bool relaxedNames);
		unsigned readPhylipHeader(std::istream & inf, unsigned & ntax, unsigned & nchar);
		void readPhylipData(FileToCharBuffer & ftcb, const NxsDiscreteDatatypeMapper &dm, std::list<std::string> & taxaNames, std::list<NxsDiscreteStateRow> & matList, const unsigned n_taxa, const unsigned n_char, bool relaxedNames);
		void readInterleavedPhylipData(FileToCharBuffer & ftcb, const NxsDiscreteDatatypeMapper &dm, std::list<std::string> & taxaNames, std::list<NxsDiscreteStateRow> & matList, const unsigned n_taxa, const unsigned n_char, bool relaxedNames);
		std::string readPhylipName(FileToCharBuffer & ftcb, unsigned i, bool relaxedNames);

};

#endif

