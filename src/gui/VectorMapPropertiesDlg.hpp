//=======================================================================
// Author: Somayyeh Zangooei
//
// Copyright 2013 Somayyeh Zangooei
//
// This file is part of GenGIS.
//
// GenGIS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GenGIS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GenGIS.  If not, see <http://www.gnu.org/licenses/>.
//=======================================================================

#ifndef _GENGIS_VECTOR_MAP_PROPERTIES_DLG_
#define _GENGIS_VECTOR_MAP_PROPERTIES_DLG_

#include "../core/Precompiled.hpp"

#include "VectorMapPropertiesLayout.h"

namespace GenGIS
{

	/**
	* @brief Handles all events and GUI processing for map properties dialog.
	*/
	class VectorMapPropertiesDlg : public VectorMapPropertiesLayout
	{
	public:
		enum PAGE { GENERAL = 0, SYMBOLOGY = 1, METADATA = 2  };
		enum SUBPAGE { POINT = 0 , POLYLINE= 1 , POLYGON= 2 };

	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		* @param mapLayer Layer to set properties of.
		*/
		explicit VectorMapPropertiesDlg(wxWindow* parent, VectorMapLayerPtr vectorMapLayer);

		/** Destructor */
		~VectorMapPropertiesDlg();

		/** Apply all parameter values. */
		void Apply();
		void ApplyPointSymbology();
		void ApplyPolyLineSymbology();
		void ApplyPolygonSymbology();




		/** Set the currently open page. */
		void SetPage(int pageIndex) { m_notebook->SetSelection(pageIndex); }
		void SetSubPage(int pageIndex) { m_symbology_Notebook->SetSelection(pageIndex); }
		//wxWindow* GetSymbology_PointWindow(int Index)   {ID_PNL_SYMBOLOGY->FindItem(index);} 
		void DisablePointPanel() { m_pnlPoint->Enable(false); }
		void DisablePolyLinePanel() { m_pnlLine->Enable(false); }
		void DisablePolygonPanel() { m_pnlPolygon->Enable(false); }


		
	private:
		/** Initialize all dialog box parameters. */
		void InitGeneral();

		/** Initialize controls on Symbology->Colour Map page. */
		//void InitColourMap();

		/** Initialize the metadata text control. */
		void InitMetaData();
		void InitMeta_Data();

		void InitPointSymbology();
		void InitPolyLineSymbology();
		void InitPolygonSymbology();


		void ReplaceColourPicker( wxColourPickerCtrl* wxColourPicker, Colour& colour );

		/** Apply controls on Symbology->Colour Map page. */
		//void ApplyColourMap();

		/** Number of entires specified by spin control on Symbology->Colour Map page has changed. */
		//void OnNumEntriesChange( wxSpinEvent& event ) { OnNumEntriesChange(); }
	//	void OnNumEntriesChange();

		/** Colour map combobox value on Symbology->Colour Map page has changed. */
		//void OnColourMapChange( wxCommandEvent& event );

		/** Interpolation combobox on Symbology->Colour Map page has changed. */
		//void OnInterpolationChange( wxCommandEvent& event );

		/** Evenly distribute elevation amongst colour map entries. */
		//void OnEvenlySpace( wxCommandEvent& event );

		/** Scale elevation data so it can be used with a level-of-detail wxSlider. */
		//int ScaleElevation(float elevation);

		/** Scale elevation data so it can be used to control the level-of-detail in the map. */
		//float ScaleElevation(int elevation);

		/** OK button event handler. */
		void OnOK( wxCommandEvent& event );

		/** Apply button event handler. */
		void OnApply( wxCommandEvent& event );

		/** Cancel button event handler. */
		void OnCancel( wxCommandEvent& event );

		/** Close dialog. */
		void OnClose( wxCloseEvent& event );

		void OnHelp( wxCommandEvent& event )
		{
			wxLaunchDefaultBrowser( wxT( "http://kiwi.cs.dal.ca/GenGIS/The_GenGIS_2.0_Manual#Map_Layer_Properties" ));
		}

	protected:
		/** Map layer to display properties of. */
		VectorMapLayerPtr m_vectorMapLayer;

		/** Helper functions for interacting with terrain maps. */
		//TerrainMapWidgetPtr m_terrainMapWidget;

		/** Flag indicating if terrain colour map has changed. */
		//bool m_bColourMapChanged;
	};

}

#endif 
