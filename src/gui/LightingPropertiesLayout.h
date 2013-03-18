///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __LightingPropertiesLayout__
#define __LightingPropertiesLayout__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/slider.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class LightingPropertiesLayout
	///////////////////////////////////////////////////////////////////////////////
	class LightingPropertiesLayout : public wxDialog 
	{
		private:
		
		protected:
			enum
			{
				ID_LBL_LIGHT_BEARING = 1000,
				ID_SLIDER_DIRECTION,
				ID_LBL_LIGHT_ALTITUDE,
				ID_SLIDER_ALTITUDE,
				ID_LBL_AMBIENT_LIGHT_STRENGTH,
				ID_SLIDER_AMBIENT_LIGHT_STRENGTH,
				ID_BTN_APPLY,
			};
			
			wxStaticText* m_lblLightDirection;
			wxSlider* m_sliderLightDirection;
			wxStaticBitmap* m_bitmapCompass;
			wxStaticText* m_lblLightAltitude;
			wxSlider* m_sliderLightAltitude;
			wxStaticBitmap* m_bitmapAltitude;
			wxStaticText* m_lblAmbientLightStrength;
			wxSlider* m_sliderAmbientLightStrength;
			wxButton* m_btnDefaultValues;
			
			wxButton* m_btnOK;
			wxButton* m_btnApply;
			wxButton* m_btnCancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnScrollChanged( wxScrollEvent& event ) { event.Skip(); }
			virtual void OnDefaultValues( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			LightingPropertiesLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Lighting Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 426,454 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSTAY_ON_TOP );
			~LightingPropertiesLayout();
		
	};
	
} // namespace GenGIS

#endif //__LightingPropertiesLayout__
