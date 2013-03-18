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

#include "../gui/LinearAnalysisPlotDlg.hpp"

#include "../core/App.hpp"
#include "../core/GeoTreeOptLeafOrder.hpp"
#include "../core/TreeLayer.hpp"
#include "../core/GeoTreeView.hpp"

#include "../utils/MathPlot.hpp"
#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"


using namespace GenGIS;


LinearAnalysisPlotDlg::LinearAnalysisPlotDlg(wxWindow* parent, TreeLayerPtr treeLayer,
	std::vector<LinearResults> linearResults, NodeGeoTree* node) :
		LinearAnalysisLayout(parent, wxID_ANY, wxT("Linear axes plot")),
		m_treeLayer(treeLayer), m_bPermutationTestRun(false), m_node(node)
{
	SetIcon(wxIcon(App::Inst().GetExeDir() + wxT("images/CrazyEye.ico"), wxBITMAP_TYPE_ICO));

	// create a vector layer
	mpFXYVector* vectorLayer = new mpFXYVector(_(""));
	std::vector<double> vectorx, vectory;
	for(uint i = 0; i < linearResults.size()-1; ++i)
	{
		vectorx.push_back(linearResults[i].angle);
		vectory.push_back(linearResults[i].numCrossings);

		vectorx.push_back(linearResults[i+1].angle-1e-6);
		vectory.push_back(linearResults[i].numCrossings);
	}
	vectorx.push_back(linearResults[linearResults.size()-1].angle);
	vectory.push_back(linearResults[linearResults.size()-1].numCrossings);

	m_initialValue = linearResults[0].numCrossings;

	vectorLayer->SetData(vectorx, vectory);
	vectorLayer->SetContinuity(true);
	wxPen vectorpen(*wxBLUE, 1, wxSOLID);
	vectorLayer->SetPen(vectorpen);
	vectorLayer->SetDrawOutsideMargins(false);

	// create axes
	mpScaleX* xaxis = new mpScaleX(wxT("Azimuth (in degrees)"), mpALIGN_BOTTOM, true);
	mpScaleY* yaxis = new mpScaleY(wxT("Number of crossings"), mpALIGN_LEFT, false);
	wxFont graphFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	xaxis->SetFont(graphFont);
	yaxis->SetFont(graphFont);
	xaxis->SetDrawOutsideMargins(false);
	yaxis->SetDrawOutsideMargins(false);
	xaxis->SetLabelFormat(_T("%.0f"));
	yaxis->SetLabelFormat(_T("%.0f"));

	// create overlay to indicate mouse position in plot coordinates
	m_infoCoords = new mpInfoCoords(wxRect(5,5,60,20), wxTRANSPARENT_BRUSH);
	m_infoCoords->SetDrawOutsideMargins(false);

	// mark critical value
	m_cvLayer = new mpFXYVector(_(""));
	std::vector<double> cvX, cvY;
	cvX.push_back(90);
	cvY.push_back(m_initialValue);
	cvX.push_back(270);
	cvY.push_back(m_initialValue);

	m_cvLayer->SetData(cvX, cvY);
	m_cvLayer->SetContinuity(true);
	m_cvLayer->SetPen(wxPen(*wxRED, 2, wxSOLID));
	m_cvLayer->SetDrawOutsideMargins(false);
	m_cvLayer->SetVisible(false);

	// create plot window
	m_plot = new mpWindow( this, -1, wxPoint(0,0), wxSize(100,100), wxSUNKEN_BORDER );
	m_plot->SetMargins(15, 15, 45, 50);	
	m_plot->AddLayer(xaxis);
	m_plot->AddLayer(yaxis);
	m_plot->AddLayer(vectorLayer);
	m_plot->AddLayer(m_infoCoords); 	
	m_plot->AddLayer(m_cvLayer);

	// add plot to dialog
	wxBoxSizer* m_sizerPlot;
	m_sizerPlot = new wxBoxSizer( wxVERTICAL );
	m_pnlPlot->SetSizer( m_sizerPlot );
	m_pnlPlot->Layout();
	m_sizerPlot->Fit( m_pnlPlot );
	m_sizerPlot->Add(m_plot, 1, wxEXPAND | wxALL, 0);

	m_plot->EnableDoubleBuffer(true);
	m_plot->SetMPScrollbars(false);
	m_plot->Fit();

	m_plot->SignalPlotClicked(boost::bind(&LinearAnalysisPlotDlg::SigPlotClicked, this, _1));
}

LinearAnalysisPlotDlg::~LinearAnalysisPlotDlg()
{

}

void LinearAnalysisPlotDlg::PlotCriticalValue()
{
	if(!m_bPermutationTestRun)
		return;

	if(m_cboCriticalValue->GetStringSelection() == wxT("<none>"))
	{
		m_cvLayer->SetVisible(false);

		std::vector<double> cvX, cvY;
		cvX.push_back(90);
		cvY.push_back(m_initialValue);
		cvX.push_back(270);
		cvY.push_back(m_initialValue);
		m_cvLayer->SetData(cvX, cvY);
	}
	else
	{
		m_cvLayer->SetVisible(true);

		std::vector<double> cvX, cvY;
		cvX.push_back(90);
		cvY.push_back(m_criticalValue);
		cvX.push_back(270);
		cvY.push_back(m_criticalValue);
		m_cvLayer->SetData(cvX, cvY);

		std::string resultStr = "Angles with fewer than " + StringTools::ToString(m_criticalValue) + " crossings are significant.";
		Log::Inst().Write(resultStr);
	}

	m_plot->Fit();
}

void LinearAnalysisPlotDlg::SigPlotClicked(float angle)
{
	if(m_treeLayer)
	{
		uint numCrossings = m_treeLayer->GetGeoTreeView()->SetGeographicAxisAngle(angle, m_node);

		wxString numCrossingsStr;
		numCrossingsStr.Printf(wxT("%d"), numCrossings);

		wxString angleStr;
		angleStr.Printf(wxT("%.1f"), angle);
		//~mikep. Fix this conversion... see note on ToStdWstring
		std::wstring numCrossingsStr_temp = numCrossingsStr.wc_str(*wxConvCurrent);
		std::wstring angleStr_temp = angleStr.wc_str(*wxConvCurrent);

		std::wstring str = _T("There are ") + numCrossingsStr_temp + _T(" crossings when the linear axis is at an angle of ") + angleStr_temp + _T(" degrees.");
		Log::Inst().Write(StringTools::ToString(str));
	}
}

void LinearAnalysisPlotDlg::OnCriticalValueChanged( wxCommandEvent& event )
{
	SetCriticalValue();
}

void LinearAnalysisPlotDlg::SetCriticalValue()
{
	if(!m_bPermutationTestRun)
		return;

	if(m_cboCriticalValue->GetStringSelection() == wxT("0.001"))
		m_criticalValue = m_criticalValue001;
	else if(m_cboCriticalValue->GetStringSelection() == wxT("0.01"))
		m_criticalValue = m_criticalValue01;
	else if(m_cboCriticalValue->GetStringSelection() == wxT("0.05"))
		m_criticalValue = m_criticalValue05;
	else if(m_cboCriticalValue->GetStringSelection() == wxT("0.1"))
		m_criticalValue = m_criticalValue1;

	PlotCriticalValue();
}

void LinearAnalysisPlotDlg::OnPermutationTest( wxCommandEvent& event )
{
	m_bPermutationTestRun = true;

	// desired number of replicates
	long replicates;
	m_cboReplicates->GetStringSelection().ToLong(&replicates);

	std::map<uint, uint> pdf;
	uint originalNumberCrossings;
	bool bSuccess;
	double pValue = m_treeLayer->GetGeoTreeView()->SignificanceTest(replicates, pdf, originalNumberCrossings, m_node, bSuccess);

	if(!bSuccess)
		return;

	// get keys of pdf in ascending order
	std::vector<uint> keys;
	std::map<uint, uint>::iterator it;	
	for(it=pdf.begin(); it != pdf.end(); it++)
		keys.push_back(it->first);

	std::sort(keys.begin(), keys.end());

	// determine critical values
	m_criticalValue001 = m_criticalValue01 = m_criticalValue05 = m_criticalValue1 = keys.at(0)-0.5;

	uint numNullReplicates = 0;
	for(uint i = 0; i < keys.size(); ++i)
	{
		numNullReplicates += pdf[keys.at(i)];

		float pValue = float(numNullReplicates) / replicates;

		if(pValue < 0.001)
			m_criticalValue001 = keys.at(i) + 0.5;

		if(pValue < 0.01)
			m_criticalValue01 = keys.at(i) + 0.5;

		if(pValue < 0.05)
			m_criticalValue05 = keys.at(i) + 0.5;

		if(pValue < 0.1)
			m_criticalValue1 = keys.at(i) + 0.5;

		if(pValue > 0.1)
			break;
	}

	SetCriticalValue();
}


void LinearAnalysisPlotDlg::OnSavePlot( wxCommandEvent& event )
{
	wxFileDialog* saveDlg = new wxFileDialog(this, wxT("Save plot..."), wxEmptyString, wxEmptyString, wxT("*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);

	wxString wildcards = wxT("Portable Network Graphics (*.png)|*.png");
	saveDlg->SetWildcard(wildcards);

	if(saveDlg->ShowModal() != wxID_OK)
	{
		saveDlg->Destroy();
		return;
	}

	m_plot->SaveScreenshot(saveDlg->GetPath(), wxBITMAP_TYPE_PNG);
}

void LinearAnalysisPlotDlg::OnMouseMove( wxMouseEvent& event )
{
	// ### This is a hack. It should be possible for m_plot to receive mouse event directly.
	m_plot->OnMouseMove(event);
	event.Skip();
}

void LinearAnalysisPlotDlg::OnMouseLeftUp( wxMouseEvent& event )
{
	// ### This is a hack. It should be possible for m_plot to receive mouse event directly.
	m_plot->OnMouseLeftUp(event);
	event.Skip();
}

void LinearAnalysisPlotDlg::OnShowPopupMenu( wxMouseEvent& event )
{
	// ### This is a hack. It should be possible for m_plot to receive mouse event directly.
	m_plot->OnShowPopupMenu(event);
	event.Skip();
}