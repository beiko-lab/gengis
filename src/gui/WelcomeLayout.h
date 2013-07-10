///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WelcomeLayout__
#define __WelcomeLayout__

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/bmpbuttn.h>
#include <wx/checkbox.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class WelcomeLayout
	///////////////////////////////////////////////////////////////////////////////
	class WelcomeLayout : public wxFrame 
	{
		private:
		
		protected:
			wxStaticBitmap* m_bitmap;
			wxListBox* m_listBoxRecentSessions;
			wxButton* m_button1;
			wxBitmapButton* m_newSession;
			wxButton* m_buttonNewSession;
			
			
			wxBitmapButton* m_openSession;
			wxButton* m_buttonOpenSession;
			
			wxBitmapButton* m_bpButton4;
			wxButton* m_button5;
			
			
			wxBitmapButton* m_exit;
			wxButton* m_buttonExit;
			
			wxCheckBox* m_checkBoxShowOnStartup;
			
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnRecentSession( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOpenRecentSession( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnNewSession( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOpenSession( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnGenGISManual( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnExitGenGIS( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCheckShowOnStartup( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			WelcomeLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Welcome to GenGIS"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 580,392 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxFRAME_NO_TASKBAR|wxSTAY_ON_TOP|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
			~WelcomeLayout();
		
	};
	
} // namespace GenGIS

#endif //__WelcomeLayout__
