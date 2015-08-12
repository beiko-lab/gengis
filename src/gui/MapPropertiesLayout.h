///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MapPropertiesLayout__
#define __MapPropertiesLayout__

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
#include <wx/spinctrl.h>
#include <wx/scrolwin.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/notebook.h>
#include <wx/statbmp.h>
#include <wx/gbsizer.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class MapPropertiesLayout
	///////////////////////////////////////////////////////////////////////////////
	class MapPropertiesLayout : public wxDialog 
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
				ID_LBL_COLOUR_MAP,
				ID_CBO_COLOUR_MAP,
				ID_LBL_INTERPOLATE,
				ID_CBO_INTERPOLATE,
				ID_LBL_NUM_ENTRIES,
				ID_SPIN_NUM_ENTRIES,
				ID_SCROLLED_WINDOW_COLOUR_MAP,
				ID_BTN_EVENLY_SPACE,
				ID_LBL_LEVEL_OF_DETAIL,
				ID_TXT_VERTICAL_EXAGGERATION,
				ID_SLIDER_LEVEL_OF_DETAIL,
				ID_SLIDER_TRANSPARENCY,
				ID_CHECK_WIREFRAME,
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
			wxNotebook* m_notebook3;
			wxPanel* m_pnlColourMap;
			wxStaticText* m_lblColourMap;
			wxComboBox* m_cboColourMap;
			wxStaticText* m_lblInterpolation;
			wxComboBox* m_cboInterpolation;
			wxStaticText* m_lblEntries;
			wxSpinCtrl* m_spinNumEntries;
			wxScrolledWindow* m_scrolledWindowColourMap;
			
			wxButton* m_btnEvenlySpace;
			wxPanel* m_pnlAdvanced;
			wxStaticText* m_lblVerticalExaggeration;
			wxTextCtrl* m_txtVerticalExaggeration;
			wxStaticText* m_lblLevelOfDetail;
			wxSlider* m_sliderLevelOfDetail;
			wxStaticText* m_lblMinLevelOfDetail;
			
			wxStaticText* m_lblMaxLevelOfDetail;
			wxSlider* m_sliderTransparency;
			
			wxCheckBox* m_chkWireframe;
			wxPanel* m_pnlCartogram;
			wxStaticText* m_lblAreaFudge;
			wxTextCtrl* m_textAreaFudge;
			wxStaticText* m_lblValueFudge;
			wxTextCtrl* m_textValueFudge;
			wxStaticBitmap* m_bitmapCart;
			wxButton* m_btnMakeCartogram;
			wxButton* m_btnUndoCartogram;
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
			virtual void findme( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnColourMapChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnInterpolationChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnNumEntriesChange( wxSpinEvent& event ) { event.Skip(); }
			virtual void OnEvenlySpace( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnAreaFudgeClick( wxMouseEvent& event ) { event.Skip(); }
			virtual void OnValueFudgeClick( wxMouseEvent& event ) { event.Skip(); }
			virtual void OnCartogram( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnUndoCartogram( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			MapPropertiesLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Map Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
			~MapPropertiesLayout();
		
	};
	
} // namespace GenGIS

#endif //__MapPropertiesLayout__
