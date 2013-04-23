///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __VECTORMAPPROPERTIESLAYOUT_H__
#define __VECTORMAPPROPERTIESLAYOUT_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
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
#include <wx/clrpicker.h>
#include <wx/statline.h>
#include <wx/statbox.h>
#include <wx/notebook.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class VectorMapPropertiesLayout
	///////////////////////////////////////////////////////////////////////////////
	class VectorMapPropertiesLayout : public wxDialog 
	{
		private:
		
		protected:
			enum
			{
				ID_NOTEBOOK = 1000,
				ID_PNL_GENERAL,
				ID_LBL_LAYER_NAME,
				ID_TXT_LAYER_NAME,
				ID_LBL_DESCRIPTION,
				ID_TXT_LAYER_DESCRIPTION,
				ID_LBL_AUTHORS,
				ID_TXT_AUTHOURS,
				ID_PNL_POINT,
				ID_Symbol_POINT_PROPERTIES,
				ID_CBO_POINT_SHAPE,
				ID_LBL_POINT_SIZE,
				ID_TXT_CTRL_POINT_SIZE,
				ID_LBL_POINT_COLOUR,
				ID_POINT_FILL_COLOUR_PICKER,
				ID_LBL_POINT_BORDER_COLOUR,
				ID_POINT_BORDER_COLOUR_PICKER,
				ID_LBL_POINT_BORDER_THICKNESS,
				ID_TXT_CTRL_POINT_BORDER_THICKNESS,
				ID_PNL_LINE,
				ID_Symbol_POLYLINE_PROPERTIES,
				ID_LBL_POLYLINE_STYLE,
				ID_CBO_POLYLINE_STYLE,
				ID_LBL_POLYLINE_THICKNESS,
				ID_TXT_CTRL_POLYLINE_THICKNESS,
				ID_LBL_POLYLINE_COLOUR,
				ID_POLYLINE_FILL_COLOUR_PICKER,
				ID_LBL_POLYLINE_BORDER_COLOUR,
				ID_POLYLINE_BORDER_COLOUR_PICKER,
				ID_LBL_POLYLINE_BORDER_THICKNESS,
				ID_TXT_CTRL_POLYLINE_BORDER_THICKNESS,
				ID_PNL_POLYGON,
				ID_Symbol_POLYGON_PROPERTIES,
				ID_LBL_POLYGON_STYLE,
				ID_CBO_POLYGON_STYLE,
				ID_LBL_POLYGON_Thickness,
				ID_TXT_CTRL_POLYGON_Thickness,
				ID_LBL_POLYGON_COLOUR,
				ID_POLYGON_BORDER_COLOUR_PICKER,
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
			wxStaticText* m_lblDescription;
			wxTextCtrl* m_txtLayerDescription;
			wxStaticText* m_lblAuthours;
			wxTextCtrl* m_txtAuthours;
			wxPanel* m_pnlSymbology;
			wxNotebook* m_symbology_Notebook;
			wxPanel* m_pnlPoint;
			wxStaticText* m_lblPointShape;
			wxComboBox* m_CboPointShape;
			wxStaticText* m_lblPointSize;
			wxTextCtrl* m_TxtCtrlPointSize;
			wxStaticText* m_lblPointColour;
			wxColourPickerCtrl* m_FillPointColourPicker;
			wxStaticLine* m_staticline2;
			wxStaticText* m_lblPointBorderColour;
			wxColourPickerCtrl* m_PointBorderColourPicker;
			wxStaticText* m_lblPointBorderThickness;
			wxTextCtrl* m_TxtCtrlPointBorderThickness;
			wxPanel* m_pnlLine;
			wxStaticText* m_lblPolylineStyle;
			wxComboBox* m_CboPolylineStyle;
			wxStaticText* m_lblPolylineThickness;
			wxTextCtrl* m_TxtCtrlPolylineThickness;
			wxStaticText* m_lblPolylineColour;
			wxColourPickerCtrl* m_PolylineFillcolourPicker;
			wxStaticLine* m_staticline21;
			wxStaticText* m_lblPolylineBorderColour;
			wxColourPickerCtrl* m_PolylineBordercolourPicker;
			wxStaticText* m_lblPolylineBorderThickness;
			wxTextCtrl* m_TxtCtrlPolylineBorderThickness;
			wxPanel* m_pnlPolygon;
			wxStaticText* m_lblPolygonStyle;
			wxComboBox* m_CboPolygonStyle;
			wxStaticText* m_lblPolygonThickness;
			wxTextCtrl* m_TxtCtrlPolygonThickness;
			wxStaticText* m_lblPolygonColour;
			wxColourPickerCtrl* m_PolygonBordercolourPicker;
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
			virtual void findme( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			VectorMapPropertiesLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Vector Map Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 313,374 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
			~VectorMapPropertiesLayout();
		
	};
	
} // namespace GenGIS

#endif //__VECTORMAPPROPERTIESLAYOUT_H__
