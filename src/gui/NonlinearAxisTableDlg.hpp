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

#ifndef _GENGIS_NONLINEAR_AXIS_TABLE_DLG
#define _GENGIS_NONLINEAR_AXIS_TABLE_DLG

#include "../core/Precompiled.hpp"

#include "NonlinearAxisTableLayout.h"

#include "../core/GeoTreeOptLeafOrder.hpp"

namespace GenGIS
{

	/**
	* @brief Display plot of geophylogeny linear analysis.
	*/
	class NonlinearAxisTableDlg : public NonlinearAxisTableLayout
	{
	public:
		/**
		* @brief Constructor.
		* @param parent Parent for this frame
		* @param uint nodeId Node analysis is being applied to.
		*/
		explicit NonlinearAxisTableDlg(wxWindow* parent, TreeLayerPtr treeLayere);

		/** Destructor */
		~NonlinearAxisTableDlg();

	protected:
		void OnListBox(wxCommandEvent& event);

	protected:
		/** Tree linear analysis was performed upon. */
		TreeLayerPtr m_treeLayer;
	};

}

#endif 
