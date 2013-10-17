///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __LocationMergeLayout__
#define __LocationMergeLayout__

#include <wx/string.h>
#include <wx/checklst.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class LocationMergeLayout
///////////////////////////////////////////////////////////////////////////////
class LocationMergeLayout : public wxFrame 
{
	private:
	
	protected:
		wxBoxSizer* bSizer1;
		wxCheckListBox* m_locationSetCheckbox;
		wxCheckBox* m_remove;
		wxStaticText* m_description;
		wxButton* m_OK;
		wxButton* m_Close;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		LocationMergeLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Location Set Merger"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 260,186 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~LocationMergeLayout();
	
};

#endif //__LocationMergeLayout__
