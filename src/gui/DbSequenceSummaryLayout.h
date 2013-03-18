///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DbSequenceSummaryLayout__
#define __DbSequenceSummaryLayout__

#include <wx/choicebk.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/grid.h>
#include <wx/splitter.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class DbSequenceSummaryLayout
	///////////////////////////////////////////////////////////////////////////////
	class DbSequenceSummaryLayout : public wxDialog 
	{
		private:
		
		protected:
			enum
			{
				ID_CHOICE_BOOK = 1000,
				ID_GRID,
			};
			
			wxSplitterWindow* m_splitter;
			wxPanel* m_panel2;
			wxChoicebook* m_choiceBook;
			wxPanel* m_tablePanel;
			wxGrid* m_grid;
			
			wxButton* m_btnOK;
			wxButton* m_btnCancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnFieldChanged( wxChoicebookEvent& event ) { event.Skip(); }
			virtual void OnFieldChanging( wxChoicebookEvent& event ) { event.Skip(); }
			virtual void OnGridRightClick( wxGridEvent& event ) { event.Skip(); }
			virtual void OnLabelLeftClick( wxGridEvent& event ) { event.Skip(); }
			virtual void OnLabelRightClick( wxGridEvent& event ) { event.Skip(); }
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			DbSequenceSummaryLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sequence summary"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,700 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSTAY_ON_TOP );
			~DbSequenceSummaryLayout();
			
			void m_splitterOnIdle( wxIdleEvent& )
			{
				m_splitter->SetSashPosition( 250 );
				m_splitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( DbSequenceSummaryLayout::m_splitterOnIdle ), NULL, this );
			}
		
	};
	
} // namespace GenGIS

#endif //__DbSequenceSummaryLayout__
