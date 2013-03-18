///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __FontPropertiesLayout__
#define __FontPropertiesLayout__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class FontPropertiesLayout
	///////////////////////////////////////////////////////////////////////////////
	class FontPropertiesLayout : public wxDialog 
	{
		private:
		
		protected:
			enum
			{
				ID_BTN_APPLY = 1000,
			};
			
			wxStaticText* m_staticText1;
			wxComboBox* m_cboFont;
			
			wxButton* m_btnOK;
			wxButton* m_btnApply;
			wxButton* m_btnCancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			FontPropertiesLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Label Font Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 269,103 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSTAY_ON_TOP );
			~FontPropertiesLayout();
		
	};
	
} // namespace GenGIS

#endif //__FontPropertiesLayout__
