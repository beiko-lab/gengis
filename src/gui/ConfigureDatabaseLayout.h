///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ConfigureDatabaseLayout__
#define __ConfigureDatabaseLayout__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class ConfigureDatabaseLayout
	///////////////////////////////////////////////////////////////////////////////
	class ConfigureDatabaseLayout : public wxDialog 
	{
		private:
		
		protected:
			wxStaticText* m_staticText1;
			wxChoice* m_choiceMOA;
			wxStaticText* m_staticText2;
			wxTextCtrl* m_txtCustomStr;
			
			wxButton* m_btnOK;
			wxButton* m_btnCancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnTextChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			ConfigureDatabaseLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Configure database"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 495,123 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );
			~ConfigureDatabaseLayout();
		
	};
	
} // namespace GenGIS

#endif //__ConfigureDatabaseLayout__
