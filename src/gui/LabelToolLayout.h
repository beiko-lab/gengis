///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __LabelToolLayout__
#define __LabelToolLayout__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/clrpicker.h>
#include <wx/spinctrl.h>
#include <wx/combobox.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class LabelToolLayout
	///////////////////////////////////////////////////////////////////////////////
	class LabelToolLayout : public wxDialog 
	{
		private:
		
		protected:
			enum
			{
				ID_LABEL_TEXT = 1000,
				ID_LABEL_SIZE,
			};
			
			wxStaticText* m_lblText;
			wxTextCtrl* m_txtLabel;
			wxStaticText* m_lblColour;
			wxColourPickerCtrl* m_colourLabel;
			wxStaticText* m_lblLabelSize;
			wxSpinCtrl* m_spinLabelSize;
			wxStaticText* m_lblLabelStyle;
			wxComboBox* m_cboLabelStyle;
			
			wxButton* m_btnOK;
			wxButton* m_btnCancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			LabelToolLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Label Tool"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 312,223 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSTAY_ON_TOP );
			~LabelToolLayout();
		
	};
	
} // namespace GenGIS

#endif //__LabelToolLayout__
