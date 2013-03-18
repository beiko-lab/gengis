///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "../gui/DbSaveResultsDlg.hpp"

using namespace GenGIS;

DbSaveResultsDlg::DbSaveResultsDlg(wxWindow* parent): DbSaveResultsLayout(parent)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));
}

DbSaveResultsDlg::~DbSaveResultsDlg()
{

}

void DbSaveResultsDlg::OnSampleFilename(wxCommandEvent& event)
{
	wxFileDialog* saveDlg = new wxFileDialog(this, wxT("Save sample data..."), wxEmptyString, wxEmptyString, wxT("*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
	wxString wildcards = wxT("Comma separated values (*.csv)|*.csv");
 	saveDlg->SetWildcard(wildcards);

  if(saveDlg->ShowModal() == wxID_OK)
  {
		m_txtSampleFilename->SetValue(saveDlg->GetDirectory() + _T("/") + saveDlg->GetFilename());
	}
}

void DbSaveResultsDlg::OnSequenceFilename(wxCommandEvent& event)
{
	wxFileDialog* saveDlg = new wxFileDialog(this, wxT("Save sequence data..."), wxEmptyString, wxEmptyString, wxT("*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
	wxString wildcards = wxT("Comma separated values (*.csv)|*.csv");
 	saveDlg->SetWildcard(wildcards);

  if(saveDlg->ShowModal() == wxID_OK)
  {
		m_txtSequenceFilename->SetValue(saveDlg->GetDirectory() + _T("/") + saveDlg->GetFilename());
	}
}

