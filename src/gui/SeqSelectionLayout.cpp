///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../core/Precompiled.hpp"

#include "SeqSelectionLayout.h"

///////////////////////////////////////////////////////////////////////////
using namespace GenGIS;

SeqSelectionLayout::SeqSelectionLayout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_sizerMain;
	m_sizerMain = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_sizerVertical;
	m_sizerVertical = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_sizerFields;
	m_sizerFields = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblField = new wxStaticText( this, wxID_ANY, wxT("Field:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblField->Wrap( -1 );
	m_sizerFields->Add( m_lblField, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cboFields = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	m_sizerFields->Add( m_cboFields, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_sizerVertical->Add( m_sizerFields, 0, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_sizerVertical->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* m_sizerSelection;
	m_sizerSelection = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* m_sizerValues;
	m_sizerValues = new wxBoxSizer( wxVERTICAL );
	
	m_lblAllValues = new wxStaticText( this, wxID_ANY, wxT("Inactive:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAllValues->Wrap( -1 );
	m_sizerValues->Add( m_lblAllValues, 1, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_lstInactiveValues = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB|wxLB_EXTENDED|wxLB_SORT ); 
	m_lstInactiveValues->SetMinSize( wxSize( 200,150 ) );
	
	m_sizerValues->Add( m_lstInactiveValues, 0, wxEXPAND|wxALL, 5 );
	
	m_sizerSelection->Add( m_sizerValues, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerAddRemove;
	m_sizerAddRemove = new wxBoxSizer( wxVERTICAL );
	
	
	m_sizerAddRemove->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnAdd = new wxButton( this, wxID_ANY, wxT("Add -->"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerAddRemove->Add( m_btnAdd, 0, wxALL|wxEXPAND, 5 );
	
	m_btnRemove = new wxButton( this, wxID_ANY, wxT("<-- Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerAddRemove->Add( m_btnRemove, 0, wxALL|wxEXPAND, 5 );
	
	
	m_sizerAddRemove->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sizerSelection->Add( m_sizerAddRemove, 0, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerActiveValues;
	m_sizerActiveValues = new wxBoxSizer( wxVERTICAL );
	
	m_lblActiveValues = new wxStaticText( this, wxID_ANY, wxT("Active:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblActiveValues->Wrap( -1 );
	m_sizerActiveValues->Add( m_lblActiveValues, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_lstActiveValues = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB|wxLB_EXTENDED|wxLB_SORT ); 
	m_lstActiveValues->SetMinSize( wxSize( 200,150 ) );
	
	m_sizerActiveValues->Add( m_lstActiveValues, 0, wxEXPAND|wxALL, 5 );
	
	m_sizerSelection->Add( m_sizerActiveValues, 0, wxEXPAND, 5 );
	
	m_sizerVertical->Add( m_sizerSelection, 0, wxEXPAND, 5 );
	
	m_sizerMain->Add( m_sizerVertical, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_sizerButtons;
	m_sizerButtons = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_sizerButtons->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnOK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_btnOK, 0, wxALL, 5 );
	
	m_btnApply = new wxButton( this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_btnApply, 0, wxALL, 5 );
	
	m_btnCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_btnCancel, 0, wxALL, 5 );
	
	m_sizerMain->Add( m_sizerButtons, 0, wxEXPAND, 5 );
	
	this->SetSizer( m_sizerMain );
	this->Layout();
	m_sizerMain->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_cboFields->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( SeqSelectionLayout::OnFieldChange ), NULL, this );
	m_btnAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeqSelectionLayout::OnAdd ), NULL, this );
	m_btnRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeqSelectionLayout::OnRemove ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeqSelectionLayout::OnOK ), NULL, this );
	m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeqSelectionLayout::OnApply ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeqSelectionLayout::OnCancel ), NULL, this );
}

SeqSelectionLayout::~SeqSelectionLayout()
{
	// Disconnect Events
	m_cboFields->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( SeqSelectionLayout::OnFieldChange ), NULL, this );
	m_btnAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeqSelectionLayout::OnAdd ), NULL, this );
	m_btnRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeqSelectionLayout::OnRemove ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeqSelectionLayout::OnOK ), NULL, this );
	m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeqSelectionLayout::OnApply ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SeqSelectionLayout::OnCancel ), NULL, this );
	
}
