///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __LayoutObjectPropertiesLayout__
#define __LayoutObjectPropertiesLayout__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/clrpicker.h>
#include <wx/combobox.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class LayoutObjectPropertiesLayout
	///////////////////////////////////////////////////////////////////////////////
	class LayoutObjectPropertiesLayout : public wxDialog 
	{
		private:
		
		protected:
			enum
			{
				ID_SPIN_OBJECT_LINE_THICKNESS = 1000,
				ID_CBO_OBJECT_LINE_STYLE,
				ID_SPIN_LAYOUT_LINE_THICKNESS,
				ID_CBO_LAYOUT_LINE_STYLE,
				ID_SPIN_CONTROL_POINT_SIZE,
				ID_LBL_CONTROL_POINT_COLOUR,
				ID_BTN_APPLY,
			};
			
			wxStaticText* m_lblObjectLineThickness;
			wxSpinCtrl* m_spinObjectLineThickness;
			wxStaticText* m_lblObjectLineColour;
			wxColourPickerCtrl* m_colourObjectLine;
			wxStaticText* m_objectLineStyleLabel;
			wxComboBox* m_cboObjectLineStyle;
			wxStaticText* m_lblLayoutLineThickness;
			wxSpinCtrl* m_spinLayoutLineThickness;
			wxStaticText* m_lblLayoutLineColour;
			wxColourPickerCtrl* m_colourLayoutLine;
			wxStaticText* m_layoutLineStyleLabel;
			wxComboBox* m_cboLayoutLineStyle;
			wxStaticText* m_lblControlPointSize;
			wxSpinCtrl* m_spinControlPointSize;
			wxStaticText* m_lblControlPointColour;
			wxColourPickerCtrl* m_colourControlPoint;
			
			wxButton* m_btnOK;
			wxButton* m_btnApply;
			wxButton* m_btnCancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			LayoutObjectPropertiesLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Object Layout Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSTAY_ON_TOP );
			~LayoutObjectPropertiesLayout();
		
	};
	
} // namespace GenGIS

#endif //__LayoutObjectPropertiesLayout__
