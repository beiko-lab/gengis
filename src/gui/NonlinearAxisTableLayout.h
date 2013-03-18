///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __NonlinearAxisTableLayout__
#define __NonlinearAxisTableLayout__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class NonlinearAxisTableLayout
	///////////////////////////////////////////////////////////////////////////////
	class NonlinearAxisTableLayout : public wxDialog 
	{
		private:
		
		protected:
			wxStaticText* m_staticText3;
			wxListBox* m_listbox;
			
			wxButton* m_btnOK;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnListBox( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			NonlinearAxisTableLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Nonlinear Axis Table"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 292,316 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
			~NonlinearAxisTableLayout();
		
	};
	
} // namespace GenGIS

#endif //__NonlinearAxisTableLayout__
