///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __TreePropertiesLayout__
#define __TreePropertiesLayout__

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
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/clrpicker.h>
#include <wx/notebook.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class TreePropertiesLayout
	///////////////////////////////////////////////////////////////////////////////
	class TreePropertiesLayout : public wxDialog 
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
				ID_PANEL_TREE_PROP,
				ID_LBL_SYMBOL_SHAPE,
				ID_CBO_TREE_LAYOUT,
				ID_LBL_TREE_THICKNESS,
				ID_SPIN_TREE_THICKNESS,
				ID_TXT_TREE_HEIGHT,
				ID_LBL_2D_ORIENTATION,
				ID_LBL_TREE_COLOUR,
				ID_CHECK_DRAW_LAYOUT_PRIMATIVE,
				ID_CHECK_DRAW_GEO_AXIS,
				ID_LBL_LOCATION_LINE_STYLE,
				ID_CBO_LOCATION_LINE_STYLE,
				ID_LBL_LOCATION_LINE_THICKNESS,
				ID_SPIN_LOCATION_LINE_THICKNESS,
				ID_LBL_CORRELATION_LINE_STYLE,
				ID_CBO_CORRELATION_LINE_STYLE,
				ID_LBL_CORRELATIN_LINE_THICKNESS,
				ID_SPIN_CORRELATION_LINE_THICKNESS,
				ID_SPIN_DROP_LINE_THICKNESS,
				ID_CBO_DROP_LINE_STYLE,
				ID_COLOUR_PICKER_CORRELATION_LINE,
				ID_LBL_GEOGRAPHY_LINE_OFFSET,
				ID_LBL_GEOGRAPHY_LINE_STYLE,
				ID_CBO_GEOGRAPHY_LINE_STYLE,
				ID_SPIN_GEO_POINT_SIZE,
				ID_CHECK_DRAW_CORRELATION_LINES,
				ID_PNL_LABEL,
				ID_LBL_LABEL_SIZE,
				ID_SPIN_LABEL_SIZE,
				ID_LBL_LABEL_COLOUR,
				ID_COLOUR_LABEL,
				ID_CHECK_SHOW_LABELS,
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
			wxNotebook* m_symbologyNotebook;
			wxPanel* m_panelTreeProp;
			wxStaticText* m_lblTreeLayout;
			wxComboBox* m_cboTreeLayout;
			wxStaticText* m_lblTreeThickness;
			wxSpinCtrl* m_spinTreeThickness;
			wxStaticText* m_lblTreeHeight;
			wxTextCtrl* m_txtTreeHeight;
			wxStaticText* m_lbl2dOrientation;
			wxComboBox* m_cbo2dOrientation;
			wxCheckBox* m_chkOptimizeLeafNodes;
			wxStaticText* m_staticText331;
			wxComboBox* m_cboTreeColourStyle;
			wxStaticText* m_lblTreeColour;
			wxColourPickerCtrl* m_colourTree;
			
			wxStaticText* m_lblBranchBorderSize;
			wxSpinCtrl* m_spinBranchBorderSize;
			wxColourPickerCtrl* m_colourBranchBorder;
			wxStaticText* m_lblLeafNodeSize;
			wxSpinCtrl* m_spinLeafNodeSize;
			wxStaticText* m_staticText30;
			wxSpinCtrl* m_spinLeafNodeBorderSize;
			wxColourPickerCtrl* m_colourLeafNodeBorder;
			wxStaticText* m_lblInternalNodeSize;
			wxSpinCtrl* m_spinInternalNodeSize;
			wxStaticText* m_staticText31;
			wxSpinCtrl* m_spinInternalNodeBorderSize;
			wxColourPickerCtrl* m_colourInternalNodeBorder;
			wxCheckBox* m_chkDrawLayoutPrimative;
			wxCheckBox* m_chkDrawGeoAxis;
			
			wxPanel* m_pnlConnectingLineProp;
			wxStaticText* m_lblLocationLineStyle;
			wxComboBox* m_cboLocationLineStyle;
			wxStaticText* m_lblLocationLineThickness;
			wxSpinCtrl* m_spinLocationLineThickness;
			wxStaticText* m_staticText35;
			wxSpinCtrl* m_spinLocationLineBorderSize;
			wxColourPickerCtrl* m_colourLocationLineBorder;
			wxStaticText* m_lblCorrelationLineStyle;
			wxComboBox* m_cboCorrelationLineStyle;
			wxStaticText* m_lblCorrelationLineThickness;
			wxSpinCtrl* m_spinCorrelationLineThickness;
			wxStaticText* m_lblCorrelationLineBorderSize;
			wxSpinCtrl* m_spinCorrelationLineBorderSize;
			wxColourPickerCtrl* m_colourCorrelationLineBorder;
			wxStaticText* m_lblDropLineThickness;
			wxSpinCtrl* m_spinInternalDropLineThickness;
			wxStaticText* m_lblDropLineStyle;
			wxComboBox* m_cboInternalDropLineStyle;
			wxColourPickerCtrl* m_colourInternalDropLine;
			wxStaticText* m_staticText32;
			wxComboBox* m_cboLeafDropLineStyle;
			wxPanel* m_pnlGeographyLineProp;
			wxStaticText* m_lblGeoLineOffset;
			wxSpinCtrl* m_spinGeoLineOffset;
			wxStaticText* m_lblGeoLineStyle;
			wxComboBox* m_cboGeoLineStyle;
			wxStaticText* m_lblGeoLineThickness;
			wxSpinCtrl* m_spinGeoLineThickness;
			wxColourPickerCtrl* m_colourGeoLine;
			
			wxStaticText* m_lbGeoPointSize;
			wxSpinCtrl* m_spinGeoPointSize;
			wxStaticText* m_staticText33;
			wxSpinCtrl* m_spinGeoPointBorderSize;
			wxColourPickerCtrl* m_colourGeoPointBorder;
			
			wxCheckBox* m_chkEvenlySpeadGeoPts;
			
			wxCheckBox* m_chkDrawGeoPoint;
			wxPanel* m_pnlLabel;
			wxStaticText* m_lblLabelSize;
			wxSpinCtrl* m_spinLabelSize;
			wxStaticText* m_lblLabelColour;
			wxColourPickerCtrl* m_colourLabel;
			
			wxCheckBox* m_chkShowLabels;
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
			virtual void OnLayoutStyleChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			TreePropertiesLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Tree Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 387,595 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
			~TreePropertiesLayout();
		
	};
	
} // namespace GenGIS

#endif //__TreePropertiesLayout__
