///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "LinearAnalysisLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

LinearAnalysisLayout::LinearAnalysisLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_pnlPlot = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer1->Add( m_pnlPlot, 1, wxEXPAND | wxALL, 0 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblCriticalValue = new wxStaticText( this, wxID_ANY, wxT("Critical value:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblCriticalValue->Wrap( -1 );
	bSizer2->Add( m_lblCriticalValue, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_cboCriticalValueChoices[] = { wxT("<none>"), wxT("0.001"), wxT("0.01"), wxT("0.05"), wxT("0.1") };
	int m_cboCriticalValueNChoices = sizeof( m_cboCriticalValueChoices ) / sizeof( wxString );
	m_cboCriticalValue = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cboCriticalValueNChoices, m_cboCriticalValueChoices, 0 );
	m_cboCriticalValue->SetSelection( 0 );
	bSizer2->Add( m_cboCriticalValue, 0, wxALL, 5 );
	
	m_lblReplicates = new wxStaticText( this, wxID_ANY, wxT("Replicates:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblReplicates->Wrap( -1 );
	bSizer2->Add( m_lblReplicates, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_cboReplicatesChoices[] = { wxT("1000"), wxT("5000"), wxT("10000") };
	int m_cboReplicatesNChoices = sizeof( m_cboReplicatesChoices ) / sizeof( wxString );
	m_cboReplicates = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cboReplicatesNChoices, m_cboReplicatesChoices, 0 );
	m_cboReplicates->SetSelection( 0 );
	bSizer2->Add( m_cboReplicates, 0, wxALL, 5 );
	
	m_btnCriticalValue = new wxButton( this, wxID_ANY, wxT("Run permutation test"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_btnCriticalValue, 0, wxALL, 5 );
	
	
	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnSavePlot = new wxButton( this, wxID_ANY, wxT("Save plot"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_btnSavePlot, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_pnlPlot->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( LinearAnalysisLayout::OnMouseLeftUp ), NULL, this );
	m_pnlPlot->Connect( wxEVT_MOTION, wxMouseEventHandler( LinearAnalysisLayout::OnMouseMove ), NULL, this );
	m_pnlPlot->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( LinearAnalysisLayout::OnShowPopupMenu ), NULL, this );
	m_cboCriticalValue->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( LinearAnalysisLayout::OnCriticalValueChanged ), NULL, this );
	m_btnCriticalValue->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LinearAnalysisLayout::OnPermutationTest ), NULL, this );
	m_btnSavePlot->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LinearAnalysisLayout::OnSavePlot ), NULL, this );
}

LinearAnalysisLayout::~LinearAnalysisLayout()
{
	// Disconnect Events
	m_pnlPlot->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( LinearAnalysisLayout::OnMouseLeftUp ), NULL, this );
	m_pnlPlot->Disconnect( wxEVT_MOTION, wxMouseEventHandler( LinearAnalysisLayout::OnMouseMove ), NULL, this );
	m_pnlPlot->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( LinearAnalysisLayout::OnShowPopupMenu ), NULL, this );
	m_cboCriticalValue->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( LinearAnalysisLayout::OnCriticalValueChanged ), NULL, this );
	m_btnCriticalValue->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LinearAnalysisLayout::OnPermutationTest ), NULL, this );
	m_btnSavePlot->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LinearAnalysisLayout::OnSavePlot ), NULL, this );
	
}
