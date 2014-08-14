//=======================================================================
// Author: Mike Porter
//
// Copyright 2009 Mike Porter
//
// This file is part of GenGIS.
//
// GenGIS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GenGIS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GenGIS.  If not, see <http://www.gnu.org/licenses/>.
//=======================================================================

// Don't use this code in Windows debug builds!
#ifndef _DEBUG

#ifndef _GENGIS_PYTHONINTERFACE_
#define _GENGIS_PYTHONINTERFACE_

#include <Python.h>
#include <wx/wxPython/wxPython.h>

namespace GenGIS
{

	class PythonInterface: private Uncopyable 
	{
	public:
		// This returns whatever instance of the class is being used. This makes this class a singleton.
		static PythonInterface& Inst()
		{
			static PythonInterface pythonInterface;
			return pythonInterface;
		}

		wxPanel* m_pythonPanel; 
		PythonInterface();               // Constructor.
		~PythonInterface();              // Destructor.
		wxPanel* GetPythonPanel() const; // Gets the wxPython panel from PythonCode.py and returns it.
		void UpdatePythonInterface();
		void PythonErrors() const;	//Prints errors to wxMessageBoxes
	private:
		PyThreadState* m_mainTState;
	};

}

#endif

// end of #ifndef _DEBUG
#endif