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


#if !defined(NXS_CXX_DISCRETE_MATRIX_H)
#define NXS_CXX_DISCRETE_MATRIX_H

#include <string>
#include <vector>

#include "ncl/nxsallocatematrix.h"
#include "ncl/nxscharactersblock.h"
#include "ncl/nxscdiscretematrix.h"

	/**
	 * A C++ class that wraps a CDiscretMatrix in order to handle the memory
	 management more cleanly. This is intended to be an alternate, low-level way
	 to get character data out of a NxsCharactersBlock
	 */
class NxsCXXDiscreteMatrix 
	{
	public:
		NxsCXXDiscreteMatrix()
			{
			Initialize(0L, false);
			}
		NxsCXXDiscreteMatrix(const NxsCDiscreteMatrix & );
		NxsCXXDiscreteMatrix(const NxsCharactersBlock & cb, bool convertGapsToMissing);
		
		void Initialize(const NxsCharactersBlock * cb, bool convertGapsToMissing);

		const NxsCDiscreteMatrix & getConstNativeC() const
			{
			return nativeCMatrix;
			}

		NxsCDiscreteMatrix & getNativeC()
			{
			return nativeCMatrix;
			}

		unsigned	getNChar() const
			{
			return nativeCMatrix.nChar;
			}

		unsigned	getNTax() const
			{
			return nativeCMatrix.nTax;
			}

		unsigned	getNStates() const
			{
			return nativeCMatrix.nStates;
			}

		const char *	getSymbolsList() const   //POL added 15-Nov-2005 
			{
			return nativeCMatrix.symbolsList;
			}

		const std::vector<int8_t> &getStateList() const
			{
			return stateListAlias;
			}

		const std::vector<unsigned> &getStateListPos() const
			{
			return stateListPosAlias;
			}

		const NxsCDiscreteStateSet *getRow(unsigned i) const 
			{
			assert(i < nativeCMatrix.nTax);
			return nativeCMatrix.matrix[i];
			}

		const std::vector<int8_t> getRowAsVector(unsigned i) const 
			{
			assert(i < nativeCMatrix.nTax);
			std::vector<int8_t> v;
			for (unsigned j = 0; j < nativeCMatrix.nChar; j++)
				{
				v.push_back(nativeCMatrix.matrix[i][j]);
				}
			return v;
			}

		const NxsCDiscreteStateSet * const * getMatrix() const 
			{
			return nativeCMatrix.matrix;
			}

		const int getDatatype() const
			{
			return (int)nativeCMatrix.datatype;
			}

		bool hasWeights() const
			{
			return hasIntWeights() || hasDblWeights();
			}

		bool hasIntWeights() const
			{
			return !(intWts.empty());
			}
		
		bool hasDblWeights() const
			{
			return !(dblWts.empty());
			}

		std::vector<int> & getIntWeights()
			{
			return intWts;
			}

		std::vector<double> & getDblWeights()
			{
			return dblWts;
			}

		const std::vector<int> & getIntWeightsConst() const
			{
			return intWts;
			}

		const std::vector<double> & getDblWeightsConst() const
			{
			return dblWts;
			}

		const std::set<unsigned> & getExcludedCharIndices() const
			{
			return activeExSet;
			}

		std::vector<unsigned> getExcludedCharIndicesAsVector() const
			{
			return std::vector<unsigned>(activeExSet.begin(), activeExSet.end());
			}

	private:
		typedef ScopedTwoDMatrix<NxsCDiscreteStateSet> ScopedStateSetTwoDMatrix;
		
		NxsCDiscreteMatrix			nativeCMatrix; 		/** taxa x characters matrix in a C struct*/
		std::string					symbolsStringAlias;	/** memory management alias to symbols field of nativeCMatrix */
		ScopedStateSetTwoDMatrix	matrixAlias;		/** memory management alias to matrix field of nativeCMatrix */
		std::vector<NxsCDiscreteState_t>	stateListAlias;		/** memory management alias to ambigList field of nativeCMatrix */
		std::vector<unsigned>		stateListPosAlias;		/** memory management alias to symbolsList field of nativeCMatrix */
		std::vector<int>			intWts;
		std::vector<double>			dblWts;
		std::set<unsigned>			activeExSet;
		NxsCXXDiscreteMatrix(const NxsCXXDiscreteMatrix &); /** don't define, not copyable*/
		NxsCXXDiscreteMatrix & operator=(const NxsCXXDiscreteMatrix &); /** don't define, not copyable*/
	};

#endif  // NXS_CXX_DISCRETE_MATRIX_H
