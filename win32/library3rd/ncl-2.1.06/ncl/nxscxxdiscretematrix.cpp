//	Copyright (C) 2008 Mark Holder
//
//	This file is part of NCL (Nexus Class Library) version 2.1
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

#include "ncl/nxscxxdiscretematrix.h"
#include "ncl/nxsutilcopy.h"
#include <cassert>
using std::string;
using std::vector;
using std::cout;
using std::endl;


NxsCXXDiscreteMatrix::NxsCXXDiscreteMatrix(const NxsCharactersBlock & cb, bool gapsToMissing)
	{
	Initialize(&cb, gapsToMissing);
	}
	
void NxsCXXDiscreteMatrix::Initialize(const NxsCharactersBlock * cb, bool gapsToMissing)
{
	this->nativeCMatrix.stateList = 0L;
	this->nativeCMatrix.stateListPos = 0L;
	this->nativeCMatrix.matrix = 0L;
	this->nativeCMatrix.symbolsList = 0L;
	this->nativeCMatrix.nStates = 0;
	this->nativeCMatrix.nChar = 0;
	this->nativeCMatrix.nTax = 0L;
	this->nativeCMatrix.nObservedStateSets = 0;
	this->nativeCMatrix.datatype = NxsAltGeneric_Datatype;
	this->symbolsStringAlias.clear();
	this->matrixAlias.Initialize(0, 0);
	this->stateListAlias.clear();
	this->stateListPosAlias.clear();
	this->intWts.clear();
	this->dblWts.clear();
	this->activeExSet.clear();
	if (cb == NULL)
		return;
	std::vector<const NxsDiscreteDatatypeMapper *> mappers = cb->GetAllDatatypeMappers();
	if (mappers.size() > 1)
		throw NxsException("too many mappers");
	if (mappers.empty() || mappers[0] == NULL)
		throw NxsException("no mappers");
		
	const NxsDiscreteDatatypeMapper & mapper = *mappers[0];
	const NxsDiscreteStateMatrix & rawMatrix = cb->GetRawDiscreteMatrixRef();

	NxsCharactersBlock::DataTypesEnum inDatatype = mapper.GetDatatype();
	if (inDatatype < LowestNxsCDatatype || inDatatype > HighestNxsCDatatype)
		throw NxsException("Datatype cannot be converted to NxsCDiscreteMatrix");
	this->nativeCMatrix.datatype = NxsAltDatatypes(inDatatype);
	this->nativeCMatrix.nStates = mapper.GetNumStates();
	const std::string fundamentalSymbols = mapper.GetSymbols();
	const std::string fundamentalSymbolsPlusGaps = mapper.GetSymbolsWithGapChar();
	const bool hadGaps = !(fundamentalSymbols == fundamentalSymbolsPlusGaps);
	
	this->symbolsStringAlias = fundamentalSymbols;
	char missingSym = cb->GetMissingSymbol();	
	NxsCDiscreteState_t newMissingStateCode = this->nativeCMatrix.nStates;
	assert((int)NXS_MISSING_CODE < 0);
	assert((int)NXS_GAP_STATE_CODE < 0);
	const int sclOffset = (hadGaps ? std::min( (int)NXS_GAP_STATE_CODE, (int)NXS_MISSING_CODE) : NXS_MISSING_CODE);
	const int negSCLOffset = -sclOffset;
	const unsigned nMapperStateCodes = mapper.GetNumStateCodes();
	const unsigned recodeVecLen = nMapperStateCodes;
	const unsigned nMapperPosStateCodes = nMapperStateCodes + sclOffset;
	std::vector<NxsCDiscreteState_t> recodeVec(recodeVecLen + negSCLOffset, -2);
	NxsCDiscreteState_t * recodeArr = &recodeVec[negSCLOffset];

	if (fundamentalSymbols.length() < this->nativeCMatrix.nStates)
		throw NxsException("Fundamental states missing from the symbols string");
	const unsigned nfun_sym = (const unsigned)fundamentalSymbols.length();
	for (NxsCDiscreteState_t i = 0; i < (NxsCDiscreteState_t)this->nativeCMatrix.nStates; ++i)
		{
		if (i < (NxsCDiscreteState_t)nfun_sym && (NxsCDiscreteState_t)fundamentalSymbols[i] == '\0' && mapper.PositionInSymbols(fundamentalSymbols[i]) != (int) i)
			{
			//std::cerr << "i=" << int(i)  << "\nfundamentalSymbols = " << fundamentalSymbols << "\n" << nfun_sym << " symbols.\n";
			assert(i >= (NxsCDiscreteState_t)nfun_sym || fundamentalSymbols[i] == '\0' || mapper.PositionInSymbols(fundamentalSymbols[i]) == (int) i);
			}
#		if defined (NDEBUG)
			const std::set<int>	 & ss =  mapper.GetStateSetForCode(i);
			assert(ss.size() == 1);
			assert(*ss.begin() == i);
#		endif
		stateListAlias.push_back(1);
		stateListAlias.push_back(i);
		stateListPosAlias.push_back((unsigned) 2*i);
		recodeArr[i] = i;
		}

	//NXS_INVALID_STATE_CODE

	if (hadGaps)
		recodeArr[NXS_GAP_STATE_CODE] = ((hadGaps && gapsToMissing)? newMissingStateCode : -1);
		
	if (missingSym == '\0')
		missingSym = (hadGaps ? mapper.GetGapSymbol() : '?');
	else
		{
		assert(NXS_MISSING_CODE == mapper.GetStateCodeStored(missingSym));
		}
	recodeArr[NXS_MISSING_CODE] = newMissingStateCode;
	this->symbolsStringAlias.append(1, missingSym);
	const unsigned nCodesInMissing  = this->nativeCMatrix.nStates + (gapsToMissing ?  0 : 1);
	stateListPosAlias.push_back(2*this->nativeCMatrix.nStates);
	stateListAlias.push_back(nCodesInMissing);
	if (!gapsToMissing)
		stateListAlias.push_back(-1);
	for (NxsCDiscreteState_t i = 0; i < (NxsCDiscreteState_t)this->nativeCMatrix.nStates; ++i)
		stateListAlias.push_back(i);
	
	NxsCDiscreteState_t nextStateCode = newMissingStateCode + 1;
	for (int i = (int)this->nativeCMatrix.nStates; i < (int) nMapperPosStateCodes; ++i)
		{
		const std::set<int>	 &ss = mapper.GetStateSetForCode( i);
		const unsigned ns = (const unsigned)ss.size();
		const bool mapToMissing  = (!mapper.IsPolymorphic(i) && (nCodesInMissing + 1 == ns || nCodesInMissing == ns));
		if (mapToMissing)
			recodeArr[i] = newMissingStateCode;
		else
			{
			recodeArr[i] = nextStateCode++;
			stateListPosAlias.push_back((unsigned)stateListAlias.size());
			stateListAlias.push_back(ns);
			for (std::set<int>::const_iterator sIt = ss.begin(); sIt != ss.end(); ++sIt)
				stateListAlias.push_back((NxsCDiscreteState_t) *sIt);
			std::string stateName = mapper.StateCodeToNexusString(i);
			if (stateName.length() != 1)
				this->symbolsStringAlias.append(1, ' ');
			else
				this->symbolsStringAlias.append(1, stateName[0]); 
			}
		}
	assert(stateListPosAlias.size() == (unsigned)nextStateCode);
	assert(symbolsStringAlias.size() == (unsigned)nextStateCode);
	this->nativeCMatrix.nObservedStateSets = nextStateCode;
	
	this->nativeCMatrix.nTax = (unsigned)rawMatrix.size();
	this->nativeCMatrix.nChar = (this->nativeCMatrix.nTax == 0 ? 0 : (unsigned)rawMatrix[0].size());
	this->matrixAlias.Initialize(this->nativeCMatrix.nTax, this->nativeCMatrix.nChar);
	nativeCMatrix.matrix = matrixAlias.ptr;
	const unsigned nt = this->nativeCMatrix.nTax;
	const unsigned nc = this->nativeCMatrix.nChar;
	for (unsigned r = 0; r < nt; ++r)
		{
		NxsCDiscreteStateSet	 * recodedRow = nativeCMatrix.matrix[r];
		const std::vector<int> & rawRowVec = rawMatrix[r];
		//std::cerr <<"row  " << r << '\n';
		if (rawRowVec.empty())
			{
			NxsCDiscreteState_t recodedMissing = recodeArr[NXS_MISSING_CODE];
			for (unsigned c = 0; c < nc; ++c)
				*recodedRow++ = recodedMissing;
			}
		else 
			{
			assert(rawRowVec.size() == nc);
			const int * rawRow = &rawRowVec[0];
			for (unsigned c = 0; c < nc; ++c)
				{
				const int rawC = *rawRow++;
				if ((unsigned)(rawC +  negSCLOffset) >= recodeVecLen)
					{
					//std::cerr << int(rawC) << '\n' << negSCLOffset<< '\n' << recodeVecLen << '\n' << mapper.GetNumStates();
					assert((unsigned)(rawC +  negSCLOffset) < recodeVecLen);
					}
				assert(rawC >= sclOffset);
				const NxsCDiscreteState_t recodedC = recodeArr[rawC];
				assert(recodedC > -2);
				assert(recodedC < nextStateCode);
				//std::cerr << "c" << c << ": " << rawC << " => " << (int) recodedC << '\n';
				*recodedRow++ = recodedC;
				}
			}
		}
	nativeCMatrix.symbolsList = symbolsStringAlias.c_str();
	nativeCMatrix.stateListPos = &stateListPosAlias[0];
	nativeCMatrix.stateList = &stateListAlias[0];
	//std::cerr <<"done with NxsCXXDiscreteMatrix ctor\n";
	
	intWts.clear();
	dblWts.clear();
	const NxsTransformationManager &tm = cb->GetNxsTransformationManagerRef();
	intWts = tm.GetDefaultIntWeights();
	if (intWts.empty())
		dblWts = tm.GetDefaultDoubleWeights();
	activeExSet = cb->GetExcludedIndexSet();
}

/**
 *	Constructs  from the native C struct NxsCDiscreteMatrix
 *		by deep copy.
 */
NxsCXXDiscreteMatrix::NxsCXXDiscreteMatrix(const NxsCDiscreteMatrix & mat)
	:nativeCMatrix(mat),//aliases pointers, but we'll fix this below
	symbolsStringAlias(mat.symbolsList), 
	matrixAlias(mat.nTax, mat.nChar),
	stateListPosAlias(mat.stateListPos, (mat.stateListPos + mat.nObservedStateSets))
	{
	nativeCMatrix.symbolsList = symbolsStringAlias.c_str();
	nativeCMatrix.stateListPos = &stateListPosAlias[0];
	if (mat.nObservedStateSets > 0)
		{
		const unsigned lastStateIndex = nativeCMatrix.stateListPos[nativeCMatrix.nObservedStateSets - 1];
		const unsigned lenAmbigList = lastStateIndex + mat.stateList[lastStateIndex] + 1;
		//	cout << "lenAmbigList = "<< lenAmbigList <<endl;
		stateListAlias.reserve(lenAmbigList);
		ncl_copy(mat.stateList, (mat.stateList + lenAmbigList), std::back_inserter(stateListAlias));
		}
	nativeCMatrix.stateList = &stateListAlias[0];
	nativeCMatrix.matrix = matrixAlias.ptr;
	
	// cout << "Matrix in NxsCXXDiscreteMatrix ctor:" << mat.nTax << ' '<< mat.nChar<< endl;
	for (unsigned i = 0; i < mat.nTax; ++i)
		{
		if (mat.nChar > 0)
			ncl_copy(mat.matrix[i], mat.matrix[i] + mat.nChar, nativeCMatrix.matrix[i]);
		}
	
	}
			
