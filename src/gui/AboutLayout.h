///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __AboutLayout__
#define __AboutLayout__

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/html/htmlwin.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class AboutLayout
	///////////////////////////////////////////////////////////////////////////////
	class AboutLayout : public wxFrame 
	{
		private:
		
		protected:
			wxStaticBitmap* m_bitmap;
			wxNotebook* m_notebook;
			wxScrolledWindow* m_scrolledWindow1;
			wxHtmlWindow* m_htmlReleaseNotes;
			wxScrolledWindow* m_scrolledWindow2;
			wxHtmlWindow* m_htmlAuthors;
			wxScrolledWindow* m_scrolledWindow3;
			wxHtmlWindow* m_htmlLicense;
			wxStaticText* m_staticTextGenGisVersion;
			wxButton* m_button;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnLinkClicked( wxHtmlLinkEvent& event ) { event.Skip(); }
			virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			AboutLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About GenGIS"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 588,568 ), long style = wxCAPTION|wxSTAY_ON_TOP|wxTAB_TRAVERSAL );
			~AboutLayout();
		
	};
	
} // namespace GenGIS

#endif //__AboutLayout__
