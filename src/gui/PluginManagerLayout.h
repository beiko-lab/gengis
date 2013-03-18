///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __PluginManagerLayout__
#define __PluginManagerLayout__

#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/font.h>
#include <wx/grid.h>
#include <wx/gdicmn.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class PluginManagerLayout
	///////////////////////////////////////////////////////////////////////////////
	class PluginManagerLayout : public wxDialog 
	{
		private:
		
		protected:
			enum
			{
				ID_GRID = 1000,
			};
			
			wxGrid* m_grid;
			
			wxButton* m_btnOK;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnGridRightClick( wxGridEvent& event ) { event.Skip(); }
			virtual void OnLabelLeftClick( wxGridEvent& event ) { event.Skip(); }
			virtual void OnLabelRightClick( wxGridEvent& event ) { event.Skip(); }
			
		
		public:
			
			PluginManagerLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Plugin manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,700 ), long style = wxCAPTION|wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSTAY_ON_TOP );
			~PluginManagerLayout();
		
	};
	
} // namespace GenGIS

#endif //__PluginManagerLayout__
