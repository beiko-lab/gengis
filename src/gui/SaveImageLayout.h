///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __SaveImageLayout__
#define __SaveImageLayout__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class SaveImageLayout
	///////////////////////////////////////////////////////////////////////////////
	class SaveImageLayout : public wxDialog 
	{
		private:
		
		protected:
			wxStaticText* m_staticTextFilename;
			wxTextCtrl* m_txtFilename;
			wxButton* m_btnSelectFilename;
			wxStaticText* m_staticTextResolution;
			wxComboBox* m_cboResolution;
			wxStaticText* m_staticTextErrorMessage;
			wxCheckBox* m_openFileLocation;
			
			wxButton* m_btnOK;
			wxButton* m_btnCancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnSelectFilename( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			SaveImageLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Export As Image..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 565,145 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
			~SaveImageLayout();
		
	};
	
} // namespace GenGIS

#endif //__SaveImageLayout__
