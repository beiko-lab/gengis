///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __StudyPropertiesLayout__
#define __StudyPropertiesLayout__

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
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class StudyPropertiesLayout
	///////////////////////////////////////////////////////////////////////////////
	class StudyPropertiesLayout : public wxDialog 
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
				ID_PNL_PROJECTION,
				ID_LBL_DATUM,
				ID_CBO_DATUM,
				ID_LBL_PROJECTION,
				ID_CBO_PROJECTION,
				ID_PNL_SYMBOLOGY,
				ID_COLOUR_PICKER_BG,
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
			
			wxPanel* m_pnlProjection;
			wxStaticText* m_lblDatum;
			wxComboBox* m_cboDatum;
			wxStaticText* m_lblProjection;
			wxComboBox* m_cboProjection;
			wxTextCtrl* m_txtProjectionNote;
			wxPanel* m_pnlSymbology;
			wxStaticText* m_lblColourBackground;
			wxColourPickerCtrl* m_colourBackground;
			wxStaticText* m_staticText9;
			wxComboBox* m_cboTerrainResolution;
			wxTextCtrl* m_textCtrl6;
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
			
			StudyPropertiesLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Study Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
			~StudyPropertiesLayout();
		
	};
	
} // namespace GenGIS

#endif //__StudyPropertiesLayout__
