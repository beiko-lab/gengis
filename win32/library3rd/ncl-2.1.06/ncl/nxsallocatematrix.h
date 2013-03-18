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

// This code is based on code developed by Mark Holder for the CIPRES project

#if !defined (NXS_ALLOCATE_MATRIX_H)
# define NXS_ALLOCATE_MATRIX_H

#include <cassert> 

template<typename T>
T *** NewThreeDArray(unsigned f , unsigned s , unsigned t);
template<typename T>
T ** NewTwoDArray(unsigned f , unsigned s);
template<typename T> 
void DeleteThreeDArray(T ***& ptr);
template<typename T>
void DeleteTwoDArray(T **& ptr);

/*--------------------------------------------------------------------------------------------------------------------------
| Allocates a three dimensional array of doubles as one contiguous block of memory
| the dimensions are f two dimensional arrays that are s by t.  
| the array is set up so that 
| for(i = 0 ; i < f ; i++)
|	for (j = 0 ; j < s ; j++)
|		for (k = 0 ; k < t; k++)
|			array[i][j][k];
|
| would be the same order of access as: 
| 
|	T *ptr = **array;
|	for (i = 0 ; i < f*s*t ; i++)
|		*ptr++;
*/
template<typename T> T *** NewThreeDArray(unsigned f , unsigned s , unsigned t)
	{
	assert(f > 0 && s > 0 && t> 0);
	const unsigned twoDStride = s*t;
	T ***ptr;
	ptr = new T **[f];
	ptr[0] = new T *[f * s];
	ptr[0][0] = new T[f * s * t];
	for (unsigned sIt = 1 ; sIt < s ; sIt++)
		ptr[0][sIt] = ptr[0][sIt-1] + t ;
	for (unsigned fIt = 1 ; fIt < f ; fIt ++)
		{
		ptr[fIt] = ptr[fIt -1] +  s ;
		ptr[fIt][0] = ptr[fIt -1][0] + twoDStride;
		for (unsigned sIt = 1 ; sIt < s ; sIt++)
			ptr[fIt][sIt] = ptr[fIt][sIt-1] + t ;
		}
	return ptr;
	}

/*--------------------------------------------------------------------------------------------------------------------------
| Delete a Three Dimensional Array that has been allocated using NewThreeDArray and sets the pointer to NULL
*/
template<typename T> void DeleteThreeDArray	(T *** & ptr)
	{
	if (ptr)
		{
		if (*ptr)
			{
			delete [] **ptr;
			delete [] * ptr;
			}
		delete [] ptr;
		}
	ptr = NULL;
	}
	
/*--------------------------------------------------------------------------------------------------------------------------
| 	Allocates a two dimensional array of doubles as one contiguous block of memory
| 	the dimensions are f by s.  
| 	the array is set up so that 
| 	
|	for(i = 0 ; i < f ; i++)
|		for (j = 0 ; j < s ; j++)
|			array[i][j];
| 	
|	would be the same order of access as: 
| 
|  	T *ptr = **array;
|	for (i = 0 ; i < f*s*t ; i++)
|		*ptr++;
*/
template<typename T> T **NewTwoDArray(unsigned f , unsigned s)
	{
	assert(f > 0 && s > 0);
	T **ptr;
	ptr = new T *[f];
	*ptr = new T [f * s];
	for (unsigned fIt = 1 ; fIt < f ; fIt ++)
		ptr[fIt] = ptr[fIt -1] +  s ;
	return ptr;
	}

/*--------------------------------------------------------------------------------------------------------------------------
| Delete a 2 Dimensional Array NewTwoDArray and set the ptr to NULL
*/
template<typename T> inline void DeleteTwoDArray	(T ** & ptr)
	{
	if (ptr)
		{
		delete [] * ptr;
		delete [] ptr;
		ptr = NULL;
		}
	}

template<typename T>
class ScopedThreeDMatrix
	{
	public:
		T *** ptr;
		ScopedThreeDMatrix(unsigned f = 0, unsigned s = 0, unsigned t = 0)
			:ptr(NULL)
			{
			Initialize(f, s, t);
			}
		void Initialize(unsigned f = 0, unsigned s = 0, unsigned t = 0)
			{
			if (f > 0 && s > 0 && t > 0)
				ptr = NewThreeDArray<T>(f, s, t);
			else
				DeleteThreeDArray<T>(ptr);
			}
		T ***Surrender()
			{
			T ***temp = ptr;
			ptr = NULL;
			return temp;
			}		
		~ScopedThreeDMatrix()
			{
			if (!ptr)
				DeleteThreeDArray<T>(ptr);
			}
	};


template<typename T>
class ScopedTwoDMatrix
	{
	public:
		T ** ptr;
		ScopedTwoDMatrix(unsigned f = 0, unsigned s = 0)
			:ptr(NULL)
			{
			Initialize(f, s);
			}
		void Initialize(unsigned f, unsigned s)
			{
			if (f > 0 && s > 0)
				ptr = NewTwoDArray<T>(f, s);
			else
				DeleteTwoDArray<T>(ptr);
			}
		T **Surrender()
			{
			T** temp = ptr;
			ptr = NULL;
			return temp;
			}
		~ScopedTwoDMatrix()
			{
			if (ptr != NULL)
				DeleteTwoDArray<T>(ptr);
			}
	};

typedef ScopedTwoDMatrix<double> ScopedDblTwoDMatrix;
typedef ScopedTwoDMatrix<unsigned> ScopedUIntTwoDMatrix;

typedef ScopedThreeDMatrix<double> ScopedDblThreeDMatrix;
typedef ScopedThreeDMatrix<unsigned> ScopedUIntThreeDMatrix;

#endif
