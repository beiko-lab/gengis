///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __UpdateGenGISLayout__
#define __UpdateGenGISLayout__

#include <wx/html/htmlwin.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class UpdateGenGISLayout
///////////////////////////////////////////////////////////////////////////////
class UpdateGenGISLayout : public wxFrame 
{
	private:
	
	protected:
		wxHtmlWindow* m_htmlUpdateMessage;
		wxCheckBox* m_checkBoxSendOSInformation;
		wxButton* m_buttonOpenGenGISDownloadPage;
		wxButton* m_buttonCloseDialog;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OpenDownloadPage( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		UpdateGenGISLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Open GenGIS download page?"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxCAPTION|wxCLOSE_BOX|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		~UpdateGenGISLayout();
	
};

#endif //__UpdateGenGISLayout__
