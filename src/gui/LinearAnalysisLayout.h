///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __LinearAnalysisLayout__
#define __LinearAnalysisLayout__

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class LinearAnalysisLayout
	///////////////////////////////////////////////////////////////////////////////
	class LinearAnalysisLayout : public wxDialog 
	{
		private:
		
		protected:
			wxPanel* m_pnlPlot;
			wxStaticText* m_lblCriticalValue;
			wxChoice* m_cboCriticalValue;
			wxStaticText* m_lblReplicates;
			wxChoice* m_cboReplicates;
			wxButton* m_btnCriticalValue;
			
			wxButton* m_btnSavePlot;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnMouseLeftUp( wxMouseEvent& event ) { event.Skip(); }
			virtual void OnMouseMove( wxMouseEvent& event ) { event.Skip(); }
			virtual void OnShowPopupMenu( wxMouseEvent& event ) { event.Skip(); }
			virtual void OnCriticalValueChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnPermutationTest( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnSavePlot( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			LinearAnalysisLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Plot"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,400 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
			~LinearAnalysisLayout();
		
	};
	
} // namespace GenGIS

#endif //__LinearAnalysisLayout__
