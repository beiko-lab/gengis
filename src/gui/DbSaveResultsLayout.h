///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DbSaveResultsLayout__
#define __DbSaveResultsLayout__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class DbSaveResultsLayout
	///////////////////////////////////////////////////////////////////////////////
	class DbSaveResultsLayout : public wxDialog 
	{
		private:
		
		protected:
			wxStaticText* m_staticText1;
			wxTextCtrl* m_txtSampleFilename;
			wxButton* m_btnSampleFilename;
			wxStaticText* m_staticText2;
			wxTextCtrl* m_txtSequenceFilename;
			wxButton* m_btnSequenceFilename;
			
			wxButton* m_btnOK;
			
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnSampleFilename( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnSequenceFilename( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			DbSaveResultsLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Save sample and sequence data"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 311,127 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSTAY_ON_TOP );
			~DbSaveResultsLayout();
		
	};
	
} // namespace GenGIS

#endif //__DbSaveResultsLayout__
