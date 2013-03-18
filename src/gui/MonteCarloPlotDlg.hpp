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

#ifndef _GENGIS_MONTE_CARLO_PLOT_DLG_
#define _GENGIS_MONTE_CARLO_PLOT_DLG_

#include "../core/Precompiled.hpp"

#include "PlotLayout.h"

class mpWindow;

namespace GenGIS
{

	/**
	* @brief Display plot for a Monte Carlo statistical test.
	*/
	class MonteCarloPlotDlg : public PlotLayout
	{
	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		*/
		explicit MonteCarloPlotDlg(wxWindow* parent, TreeLayerPtr treeLayer, const std::map<uint, uint>& pdf, uint originalNumberCrossings, float pValue);

		/** Destructor */
		~MonteCarloPlotDlg();

	protected:
		void SigPlotClicked(float y);


	protected:
		mpWindow* m_plot;

		TreeLayerPtr m_treeLayer;
	};

}

#endif 
