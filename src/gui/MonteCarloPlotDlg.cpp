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

#include "../core/Precompiled.hpp"

#include "../gui/MonteCarloPlotDlg.hpp"

#include "../core/App.hpp"
#include "../core/GeoTreeOptLeafOrder.hpp"
#include "../core/TreeLayer.hpp"
#include "../core/GeoTreeView.hpp"

#include "../utils/MathPlot.hpp"
#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"


using namespace GenGIS;


MonteCarloPlotDlg::MonteCarloPlotDlg(wxWindow* parent, TreeLayerPtr treeLayer,
	const std::map<uint, uint>& pdf, uint originalNumberCrossings, float pValue) :
		PlotLayout(parent, wxID_ANY, wxT("Monte Carlo permutation test")), m_treeLayer(treeLayer)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	// create null distribution
	mpFXYVector* nullLayer = new mpFXYVector(_(""));
	mpFXYVector* nullLayerPts = new mpFXYVector(_(""));
	std::vector<double> nullX, nullY;
	uint maxOccurrences = 0;

	std::map<uint, uint>::const_iterator it; 
	for(it = pdf.begin(); it != pdf.end(); ++it)
	{
		nullX.push_back(it->first);
		nullY.push_back(it->second);

		if(it->second > maxOccurrences)
			maxOccurrences = it->second;
	}

	nullLayer->SetData(nullX, nullY);
	nullLayer->SetContinuity(true);	
	nullLayer->SetPen(wxPen(*wxBLUE, 1, wxSOLID));
	nullLayer->SetDrawOutsideMargins(false);

	nullLayerPts->SetData(nullX, nullY);
	nullLayerPts->SetContinuity(false);	
	nullLayerPts->SetPen(wxPen(*wxBLUE, 6, wxSOLID));
	nullLayerPts->SetDrawOutsideMargins(false);

	// mark number of crossings with original data
	mpFXYVector* originalLayer = new mpFXYVector(_(""));
	std::vector<double> originalX, originalY;
	originalX.push_back(originalNumberCrossings);
	originalY.push_back(0);
	originalX.push_back(originalNumberCrossings);
	originalY.push_back(maxOccurrences);
	originalLayer->SetData(originalX, originalY);
	originalLayer->SetContinuity(true);
	originalLayer->SetPen(wxPen(*wxRED, 2, wxSOLID));
	originalLayer->SetDrawOutsideMargins(false);

	// create axes
	mpScaleX* xaxis = new mpScaleX(wxT("Number of crossings"), mpALIGN_BOTTOM, true);
	mpScaleY* yaxis = new mpScaleY(wxT("Occurrences under random model"), mpALIGN_LEFT, true);
	wxFont graphFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	xaxis->SetFont(graphFont);
	yaxis->SetFont(graphFont);
	xaxis->SetDrawOutsideMargins(false);
	yaxis->SetDrawOutsideMargins(false);
	xaxis->SetLabelFormat(_T("%.0f"));
	yaxis->SetLabelFormat(_T("%.0f"));

	// indicate p-value
	wxString pValueStr;
	pValueStr.Printf(wxT("p-Value = %0.3f"), pValue);

	mpText* pValueLabel = new mpText(pValueStr, -1, -1);
	pValueLabel->SetDrawOutsideMargins(true);

	// create plot window
	m_plot = new mpWindow( this, -1, wxPoint(0,0), wxSize(100,100), wxSUNKEN_BORDER );
	m_plot->SetMargins(15, 15, 45, 50);	
	m_plot->AddLayer(xaxis);
	m_plot->AddLayer(yaxis);
	m_plot->AddLayer(nullLayer);
	m_plot->AddLayer(nullLayerPts);
	m_plot->AddLayer(originalLayer);
	m_plot->AddLayer(pValueLabel);

	// layout plot
	wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
	topsizer->Add( m_plot, 1, wxEXPAND );

	SetAutoLayout( true );
	SetSizer( topsizer );

	m_plot->EnableDoubleBuffer(true);
	m_plot->SetMPScrollbars(false);
	m_plot->Fit();

	m_plot->SignalPlotClicked(boost::bind(&MonteCarloPlotDlg::SigPlotClicked, this, _1));
}

MonteCarloPlotDlg::~MonteCarloPlotDlg()
{

}

void MonteCarloPlotDlg::SigPlotClicked(float y)
{

}
