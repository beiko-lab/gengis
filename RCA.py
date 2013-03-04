from RCALayout import RCALayout

import GenGIS
from rpy2 import robjects

from CABIN_RCA import *

import wx
import math
import os

class RCA( RCALayout ):

	def __init__(self, parent=None):
		RCALayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0 or GenGIS.layerTree.GetNumSequenceLayers() == 0:
			wx.MessageBox("This plugin requires map, location, and sequence data to be loaded.", "Additional data required.")
			self.Close()
			return
		
		# populate Metric combo box (cboMetric) 
		#Diversity measure choices
		metric_choices=("Richness","Shannon","Simpson","Pielou")
		for field in metric_choices:
			self.cboMetric.Append(field) 
		self.cboMetric.SetSelection(0)
		
		# populate Diversity and Count combo boxes with sequence column names
		sequenceLayer = GenGIS.layerTree.GetSequenceLayer(0)
		seqMetadataFields = sequenceLayer.GetController().GetMetadataFields()
		for field in sorted(seqMetadataFields):
			self.cboDiversity.Append(field)
			self.cboCount.Append(field)
		
		self.cboDiversity.SetSelection(0)
		self.cboCount.SetSelection(0)
			
	def OnRun( self, event ):
		rca = CABIN_RCA(self.cboDiversity.GetStringSelection(), self.cboCount.GetStringSelection())
		rca.Run_RCA(self.cboMetric.GetStringSelection())
		
	def OnOK( self, event ):
		self.Close()
