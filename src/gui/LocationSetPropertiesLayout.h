///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __LocationSetPropertiesLayout__
#define __LocationSetPropertiesLayout__

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
#include <wx/checkbox.h>
#include <wx/clrpicker.h>
#include <wx/statline.h>
#include <wx/combobox.h>
#include <wx/scrolwin.h>
#include <wx/statbox.h>
#include <wx/spinctrl.h>
#include <wx/notebook.h>
#include <wx/radiobut.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/gbsizer.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GenGIS
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class LocationSetPropertiesLayout
	///////////////////////////////////////////////////////////////////////////////
	class LocationSetPropertiesLayout : public wxDialog 
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
				ID_PNL_LOCATIONS,
				ID_PANEL_LOCATION_SET_COLOUR,
				ID_SIZER_LOCATION_SET_COLOUR,
				ID_CHK_UNIFORM_COLOUR,
				ID_COLOUR_PICKER_UNIFORM_COLOUR,
				ID_LBL_LABEL_COLOUR_FIELD,
				ID_CBO_COLOUR_FIELD,
				ID_LBL_COLOUR_MAP,
				ID_CBO_COLOUR_MAP,
				ID_SCROLLED_WINDOW_COLOUR,
				ID_COLOUR_PICKER_BORDERS,
				ID_PANEL_LOCATION_SET_SHAPE,
				ID_SIZER_LOCATION_SET_SHAPE,
				ID_CHK_UNIFORM_SHAPE,
				ID_CBO_UNIFORM_SHAPE,
				ID_LBL_LABEL_SHAPE_FIELD,
				ID_CBO_SHAPE_FIELD,
				ID_LBL_SHAPE_MAP,
				ID_CBO_SHAPE_MAP,
				ID_SCROLLED_WINDOW_SHAPE,
				ID_PANEL_LOCATION_SET_SIZE,
				ID_SIZER_LOCATION_SET_SIZE,
				ID_LBL_LABEL_SIZE_FIELD,
				ID_CBO_SIZE_FIELD,
				ID_LBL_MIN_SIZE,
				ID_PANEL_LOCATION_SET_LABEL,
				ID_CHECK_SHOW_LABELS,
				ID_LBL_LABEL_FIELD,
				ID_CBO_LABEL_FIELD,
				ID_LBL_FONT_SIZE,
				ID_SPIN_FONT_SIZE,
				ID_LBL_LABEL_COLOUR,
				ID_COLOUR_PICKER_LABEL_COLOUR,
				ID_PNL_CHART,
				ID_NOTEBOOK_CHART,
				ID_LBL_LABEL_CHART_FIELD,
				ID_CBO_CHART_FIELD,
				ID_CBO_CHART_COLOUR_MAP,
				ID_SCROLLED_WINDOW_CHART,
				ID_CBO_CHART_TYPE,
				ID_CHK_CHART_SIZE_BY_SEQ_COUNT,
				IDLBL_CHART_MIN_SIZE,
				ID_TXT_CHART_MIN_SIZE,
				ID_TXT_CHART_MAX_SIZE,
				ID_LBL_CHART_BORDER_SIZE,
				ID_SPIN_CHART_BORDER_SIZE,
				ID_COLOUR_PICKER_CHART_BORDER,
				ID_LBL_CHART_ASSIGN_TO_OTHER,
				ID_COLOUR_PICKER_CHART_BACKGROUND,
				ID_CBO_DROPLINE_STYLE,
				ID_LBL_DROPLINE_THICKNESS,
				ID_SPIN_DROPLINE_THICKNESS,
				ID_COLOUR_PICKER_DROPLINE,
				ID_CHECK_SHOW_CHART,
				wxID_RADIO_TILE_COLOUR_NO_FILL,
				wxID_RADIO_TILE_COLOUR_UNIFORM_FILL,
				wxID_RADIO_TILE_COLOUR_MAP_FILL,
				wxID_DIVIDE_BY_BOX,
				wxID_DIVIDE_INTO_DEGREES,
				wxID_DIVIDE_INTO_PIXELS,
				wxID_DIVIDE_BY_AXIS,
				wxID_RADIO_GRID_DIVIDE_ALONG_LATITUDE,
				wxID_RADIO_GRID_DIVIDE_ALONG_LONGITUDE,
				wxID_RADIO_GRID_ALIGN_TO_ORIGIN,
				wxID_RADIO_GRID_ALIGN_TO_LOCATION,
				wxID_RADIO_GRID_ALIGN_TO_COORDINATES,
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
			
			wxNotebook* m_notebookLocationSet;
			wxPanel* m_pnlLocationSetColour;
			wxCheckBox* m_chkUniformColour;
			wxColourPickerCtrl* m_colourUniform;
			
			wxStaticLine* m_staticline5;
			wxStaticText* m_lblColourField;
			wxComboBox* m_cboColourField;
			wxStaticText* m_lblColourMap;
			wxComboBox* m_cboColourMap;
			wxScrolledWindow* m_scrolledWindowColour;
			wxStaticText* m_staticText28;
			wxSpinCtrl* m_spinBorderSize;
			wxColourPickerCtrl* m_colourBorders;
			wxPanel* m_pnlLocationSetShape;
			wxCheckBox* m_chkUniformShape;
			wxComboBox* m_cboUniformShape;
			
			wxStaticLine* m_staticline4;
			wxStaticText* m_lblShapeField;
			wxComboBox* m_cboShapeField;
			wxStaticText* m_lblShapeMap;
			wxComboBox* m_cboShapeMap;
			wxScrolledWindow* m_scrolledWindowShape;
			wxPanel* m_pnlLocationSetSize;
			wxStaticText* m_lblSizeField;
			wxComboBox* m_cboSizeField;
			
			wxStaticText* m_lblMinLocationSize;
			wxTextCtrl* m_txtMinLocationSize;
			wxStaticText* m_lnlMaxLocationSize;
			wxTextCtrl* m_txtMaxLocationSize;
			wxPanel* m_pnlLocationSetLabel;
			wxCheckBox* m_chkShowLabels;
			wxCheckBox* m_chkBindToCharts;
			wxStaticLine* m_staticline6;
			wxStaticText* m_lblLabelField;
			wxComboBox* m_cboLabelField;
			wxStaticText* m_lblFontSize;
			wxSpinCtrl* m_spinFontSize;
			wxStaticText* m_lblFontPosition;
			wxComboBox* m_cboLabelPosition;
			wxStaticText* m_staticText27;
			wxComboBox* m_cboLabelStyle;
			wxStaticText* m_lblLabelColour;
			wxColourPickerCtrl* m_colourLabel;
			
			wxPanel* m_pnlChart;
			wxNotebook* m_notebookChart;
			wxPanel* m_pnlChartColourMap;
			wxStaticText* m_lblChartField;
			wxComboBox* m_cboChartField;
			wxStaticText* m_lblChartColourMap;
			wxComboBox* m_cboChartColourMap;
			wxScrolledWindow* m_scrolledWindowChart;
			wxCheckBox* m_chkQuantitative;
			wxComboBox* m_cboQuantitativeField;
			wxPanel* m_pnlChartSymbology;
			wxStaticText* m_lblChartType;
			wxComboBox* m_cboChartType;
			wxCheckBox* m_chkChart2D;
			wxStaticLine* m_staticline2;
			wxStaticText* m_lblChartWidth;
			wxSpinCtrl* m_spinChartWidth;
			wxStaticText* m_lblChartHeight;
			wxSpinCtrl* m_spinChartHeight;
			wxCheckBox* m_chkChartSizeBySeqCount;
			wxStaticText* m_chartMinSize;
			wxTextCtrl* m_txtChartMinSize;
			wxStaticText* m_lblChartMaxSize;
			wxTextCtrl* m_txtChartMaxSize;
			wxStaticLine* m_staticline1;
			wxStaticText* m_lblChartBorderSize;
			wxSpinCtrl* m_spinChartBorderSize;
			wxColourPickerCtrl* m_colourChartBorder;
			wxCheckBox* m_chkAssignLocationColour;
			wxCheckBox* m_chkBottomBorder;
			wxCheckBox* m_chkLeftBorder;
			wxCheckBox* m_chkTopBorder;
			wxCheckBox* m_chkRightBorder;
			wxCheckBox* m_chkShowElementBorders;
			wxStaticLine* m_lineAssignTaxaToOther;
			wxStaticText* m_lblChartAssignToOther;
			wxSpinCtrl* m_spinChartFilterTaxaPercentage;
			wxStaticLine* m_lineColourChartBackground;
			wxStaticText* m_lblChartBgColour;
			wxColourPickerCtrl* m_colourChartBackground;
			wxCheckBox* m_chkTransparentBackground;
			wxStaticText* m_lblChartDroplineStyle;
			wxComboBox* m_cboChartDropLineStyle;
			wxStaticText* m_lblChartDroplineThickness;
			wxSpinCtrl* m_spinChartDropLineThickness;
			wxColourPickerCtrl* m_colourChartDropLine;
			
			wxCheckBox* m_chkShowCharts;
			wxPanel* m_pnlGrid;
			wxNotebook* m_notebook6;
			wxPanel* m_panel13;
			wxRadioButton* m_radioGridNoFill;
			
			wxRadioButton* m_radioGridUniformColour;
			wxColourPickerCtrl* m_gridTileColour;
			wxRadioButton* m_radioGridColourMap;
			
			wxStaticText* m_txtTileAlpha;
			wxSlider* m_sliderTileAlpha;
			wxStaticLine* m_staticline13;
			wxStaticText* m_txtGridFieldToChart;
			wxComboBox* m_choiceGridFieldToChart;
			wxStaticText* m_txtGridDefaultColour;
			wxColourPickerCtrl* m_colourGridDefaultColour;
			wxStaticText* m_txtGridColourMap;
			wxComboBox* m_choiceGridColourMap;
			wxStaticText* m_txtDefaultTileOpacity;
			wxSlider* m_sliderDefaultTileAlpha;
			wxScrolledWindow* m_scrolledWindowGridColour;
			wxCheckBox* m_chkShowGridBorders;
			wxStaticLine* m_staticline101;
			wxStaticText* m_txtGridBorderColour;
			wxStaticText* m_txtGridBorderAlpha;
			wxStaticText* m_txtGridBorderThickness;
			wxStaticText* m_txtGridBorderStyle;
			wxColourPickerCtrl* m_gridBorderColour;
			wxSlider* m_sliderBorderAlpha;
			wxSpinCtrl* m_spinGridBorderThickness;
			wxComboBox* m_cboGridBorderStyle;
			wxPanel* m_panel12;
			wxRadioButton* m_radioBox;
			wxSpinCtrl* m_spinBoxDivisions;
			wxRadioButton* m_radioBtnDegrees;
			wxRadioButton* m_radioBtnPixels;
			wxRadioButton* m_radioAxis;
			wxSpinCtrl* m_spinGridDivisions;
			wxRadioButton* m_radioBtnLatitude;
			wxRadioButton* m_radioBtnLongitude;
			wxRadioButton* m_radioAlignToOrigin;
			
			wxRadioButton* m_radioAlignToLocation;
			wxComboBox* m_choiceAlignToLocation;
			wxRadioButton* m_radioAlignToCoordinates;
			wxStaticText* m_txtLatitude;
			wxStaticText* m_txtMinLatitude;
			wxStaticText* m_txtLatitudeLessEqualThan1;
			wxTextCtrl* m_textCtrlLatitude;
			wxStaticText* m_txtLatitudeLessEqualThan2;
			wxStaticText* m_txtMaxLatitude;
			wxStaticText* m_txtLongitude;
			wxStaticText* m_txtMinLongitude;
			wxStaticText* m_txtLongitudeLessEqualThan1;
			wxTextCtrl* m_textCtrlLongitude;
			wxStaticText* m_txtLongitudeLessEqualThan2;
			wxStaticText* m_txtMaxLongitude;
			wxButton* m_buttonGridPositionReset;
			wxButton* m_buttonClickMapToAlign;
			wxCheckBox* m_chkAutoAdjustElevation;
			wxStaticLine* m_staticline8;
			wxStaticText* m_lblVerticalElevation;
			wxTextCtrl* m_textCtrlGridElevation;
			wxStaticText* m_txtTileFieldChoice;
			wxComboBox* m_tileFieldChoice;
			wxCheckBox* m_chkShowGrid;
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
			virtual void OnPageChange( wxNotebookEvent& event ) { event.Skip(); }
			virtual void OnUniformColour( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnColourFieldChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnColourMapChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnUniformShape( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnShapeFieldChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnShapeMapChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnSizeFieldChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnShowLabels( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnChartFieldChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnChartColourMapChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnQuantitative( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnQuantitativeFieldChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnChartTypeChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnSpinChartSize( wxSpinEvent& event ) { event.Skip(); }
			virtual void OnProportionalChartSize( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnSpinBorderSize( wxSpinEvent& event ) { event.Skip(); }
			virtual void OnSpinAssignTaxa( wxSpinEvent& event ) { event.Skip(); }
			virtual void OnCboFieldChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnSpinLineThickness( wxSpinEvent& event ) { event.Skip(); }
			virtual void OnRadioColourFill( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnChoiceGridFieldToChartChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnGridColourMapChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnShowGridBorders( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRadioDivideBy( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnNumberOfDivisions( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRadioDivideType( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRadioLatitudeLongitude( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRadioAlignTo( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnGridChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnAlignToLocationChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCoordinateReset( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnAlignCoordinateToMouse( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnAutoAdjustElevation( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnTileFieldChoiceChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			wxPanel* m_pnlLocations;
			
			LocationSetPropertiesLayout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Location Set Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 503,579 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
			~LocationSetPropertiesLayout();
		
	};
	
} // namespace GenGIS

#endif //__LocationSetPropertiesLayout__
