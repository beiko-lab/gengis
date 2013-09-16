
//=======================================================================
// Author: Donovan Parks
//
// Copyright 2009 Donovan Parks
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

#ifndef _GENGIS_PRECOMPILED_
#define _GENGIS_PRECOMPILED_

// ignore known, problematic warning
#ifdef WIN32
	#pragma warning(disable:4099) // known warning in boost serialization libraries
	#pragma warning(disable:4005)	// result of duplicate #define statements in 3rd party libraries 
	#pragma warning(disable:4273) // result of duplicate function definitions in 3rd party libraries
	#pragma warning(disable:4308) // result of duplicate function definitions in 3rd party libraries
#endif

// disable checked iterators as this produces a large performance hit
// (see: http://msdn.microsoft.com/en-us/library/aa985896(VS.80).aspx and google search for SECURE_SCL)
#ifdef NDEBUG
	#define _SECURE_SCL 0
#endif

// disable calls to potentially unsafe methods in the Standard C++ library. Since GenGIS is single threaded
// these functions are safe and extremely useful.
#define _SCL_SECURE_NO_WARNINGS 

// Generally only 3rd party header files should be included in this file. That is, header files that will
// almost never be changed. If a project header is added to this file, the entire project will need to be
// recompiled whenever this file changes. There are cases where this is reasonable (i.e., header files specifying
// common typedef's or constants that must be included in almost every file anyways).

// STL includes
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <utility>

// C/C++ includes
#include <string>
#include <limits>
#include <fstream>
#include <iostream>
#include <cfloat>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <exception>
#include <algorithm>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <ctime>
#include <locale>

// GDAL/OGR includes
#include <gdal_priv.h>
#include <ogr_spatialref.h>
#include <ogrsf_frmts.h>
#include <ogr_core.h>

// Don't use this code in debug builds of Windows!
#ifndef _DEBUG
	// Boost Python includes
	#include <boost/python.hpp>
	#include <boost/python/module.hpp>
	#include <boost/python/class.hpp>
	#include <boost/python/reference_existing_object.hpp>
	#include <boost/python/return_value_policy.hpp>
	#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
	#include <boost/python/stl_iterator.hpp>
#endif

// Boost includes
#include <boost/dynamic_bitset.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals.hpp>
#include <boost/tokenizer.hpp>

#include <boost/algorithm/string.hpp>

#include <boost/archive/codecvt_null.hpp>
#include <boost/archive/text_woarchive.hpp>
#include <boost/archive/text_wiarchive.hpp>

#include <boost/archive/iterators/mb_from_wchar.hpp>
#include <boost/archive/iterators/wchar_from_mb.hpp>

//#include <boost/detail/utf8_codecvt_facet.hpp>

#include <boost/serialization/array.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/level.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/scoped_ptr.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/tracking.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

// OpenGL includes
#if defined(WIN32)
	#define NOMINMAX
	#include <windows.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#else
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#endif

// wxWidgets includes
#include <wx/wx.h>
#include <wx/app.h> 
#include <wx/progdlg.h>
#include <wx/string.h>
#include <wx/glcanvas.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/treectrl.h>
#include <wx/icon.h>
#include <wx/stdpaths.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
//#include <wx/aboutdlg.h>
#include <wx/fontdlg.h>
#include <wx/image.h>
#include <wx/treebase.h>
#include <wx/statline.h>
#include <wx/imaglist.h>
#include <wx/frame.h>
#include <wx/event.h>
#include <wx/filesys.h>
#include <wx/clrpicker.h>
#include <wx/utils.h>
#include <wx/colordlg.h>
#include <wx/stream.h>
#include <wx/tokenzr.h>
#include <wx/calctrl.h>
#include <wx/filesys.h>
#include <wx/fs_inet.h>
#include <wx/dir.h>
#include <wx/splash.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

// Project files (add with caution!)
#include "../core/DataTypes.hpp"
#include "../core/Common.hpp"
#include "../utils/Uncopyable.hpp"

// ~timothym Automatic leak detection with Microsoft VisualC++
// http://wiki.wxwidgets.org/Avoiding_Memory_Leaks
#ifdef __WXMSW__
    #include <wx/msw/msvcrt.h>      // redefines the new() operator

	// Visual Leak Detector for Visual C++
	//#include <vld.h>
#endif 


#endif
