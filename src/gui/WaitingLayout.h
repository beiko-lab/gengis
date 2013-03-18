///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WaitingLayout__
#define __WaitingLayout__

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class WaitingLayout
	///////////////////////////////////////////////////////////////////////////////
	class WaitingLayout : public wxDialog 
	{
		private:
		
		protected:
			wxStaticBitmap* m_bitmap;
			wxStaticText* m_messageText;
		
		public:
			
			WaitingLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDIALOG_NO_PARENT );
			~WaitingLayout();
		
	};
	
} // namespace GenGIS

#endif //__WaitingLayout__
