///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __LocationPropertiesLayout__
#define __LocationPropertiesLayout__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/combobox.h>
#include <wx/clrpicker.h>
#include <wx/statbox.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class LocationPropertiesLayout
	///////////////////////////////////////////////////////////////////////////////
	class LocationPropertiesLayout : public wxDialog 
	{
		private:
		
		protected:
			enum
			{
				ID_NOTEBOOK = 1000,
				ID_PNL_GENERAL,
				ID_LBL_LAYER_NAME,
				ID_TXT_LAYER_NAME,
				ID_TXT_LAYER_DESCRIPTION,
				ID_LBL_AUTHORS,
				ID_TXT_AUTHOURS,
				ID_PNL_SYMBOLOGY,
				ID_LBL_SYMBOL_SHAPE,
				ID_LBL_FONT_SIZE,
				ID_TXT_SYMBOL_SIZE,
				ID_LBL_SYMBOL_COLOUR,
				ID_PNL_METADATA,
				ID_LBL_LAYER_SOURCE,
				ID_TXT_LAYER_SOURCE,
				ID_TXT_METADATA,
				ID_BTN_APPLY,
			};
			
			wxNotebook* m_notebook;
			wxPanel* m_pnlGeneral;
			wxStaticText* m_lblLayerName;
			wxTextCtrl* m_txtLayerName;
			wxStaticText* m_lblLayerName1;
			wxTextCtrl* m_txtLayerDescription;
			wxStaticText* m_lblAuthours;
			wxTextCtrl* m_txtAuthours;
			
			wxPanel* m_pnlSymbology;
			wxStaticText* m_lblSymbolShape;
			wxComboBox* m_cboSymbolShape;
			wxStaticText* m_lbSymbolSize;
			wxTextCtrl* m_txtSymbolSize;
			wxStaticText* m_lblSymbolColour;
			wxColourPickerCtrl* m_colourSymbol;
			wxStaticText* m_lblFontSize;
			wxSpinCtrl* m_spinLabelSize;
			wxStaticText* m_lblFontPosition;
			wxComboBox* m_cboLabelPosition;
			wxStaticText* m_staticText11;
			wxComboBox* m_cboLabelStyle;
			wxStaticText* m_lblFontColour;
			wxColourPickerCtrl* m_colourLabel;
			
			wxCheckBox* m_chkLabelVisible;
			wxPanel* m_pnlMetadata;
			wxStaticText* m_lblLayerSource;
			wxTextCtrl* m_txtLayerSource;
			wxRichTextCtrl* m_txtMetaData;
			wxButton* m_btnHelp;
			
			wxButton* m_btnOK;
			wxButton* m_btnApply;
			wxButton* m_btnCancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
			virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			LocationPropertiesLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Location Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
			~LocationPropertiesLayout();
		
	};
	
} // namespace GenGIS

#endif //__LocationPropertiesLayout__
