//=======================================================================
// Author: Donovan Parks
//
// Copyright 2009 Donovan Parks
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

#ifndef _GENGIS_LINEAR_ANALYSIS_PLOT_DLG_
#define _GENGIS_LINEAR_ANALYSIS_PLOT_DLG_

#include "../core/Precompiled.hpp"

#include "LinearAnalysisLayout.h"

#include "../core/GeoTreeOptLeafOrder.hpp"

class mpWindow;
class mpInfoCoords;
class mpFXYVector;

namespace GenGIS
{

	/**
	* @brief Display plot of geophylogeny linear analysis.
	*/
	class LinearAnalysisPlotDlg : public LinearAnalysisLayout
	{
	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		* @param uint nodeId Node analysis is being applied to.
		*/
		explicit LinearAnalysisPlotDlg(wxWindow* parent, TreeLayerPtr treeLayer, std::vector<LinearResults> linearResults, NodeGeoTree* node);

		/** Destructor */
		~LinearAnalysisPlotDlg();

	protected:
		void SigPlotClicked(float angle);

		void PlotCriticalValue();
		void SetCriticalValue();

		void OnCriticalValueChanged( wxCommandEvent& event );
		void OnPermutationTest( wxCommandEvent& event );
		void OnSavePlot( wxCommandEvent& event );

		void OnMouseMove( wxMouseEvent& event );
		void OnMouseLeftUp( wxMouseEvent& event );
		void OnShowPopupMenu( wxMouseEvent& event );

	protected:
		/** Plot. */
		mpWindow* m_plot;

		/** Display position of mouse on plot. */
		mpInfoCoords* m_infoCoords;

		/** Layer for displaying critical value. */
		mpFXYVector* m_cvLayer;

		/** Tree linear analysis was performed upon. */
		TreeLayerPtr m_treeLayer;

		/** Flag indicating if the permutation test has been run. */
		bool m_bPermutationTestRun;

		/** Critical value. */
		float m_criticalValue;

		/** Critical value for p=0.001, 0.01, 0.05, and 0.1, respectively. */
		float m_criticalValue001;
		float m_criticalValue01;
		float m_criticalValue05;
		float m_criticalValue1;

		/** Safe initial value for critical value bar. */
		float m_initialValue;

		/** Node analysis is being applied to. */
		NodeGeoTree* m_node;
	};

}

#endif 
