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
		metric_choices=("Richness","Shannon","Simpson","Pielou","Berger_Parker")
		for field in metric_choices:
			self.cboMetric.Append(field) 
		self.cboMetric.SetSelection(0)
		
		# populate Diversity and Count combo boxes with sequence column names
		sequenceLayer = GenGIS.layerTree.GetSequenceLayer(0)
		seqMetadataFields = sequenceLayer.GetController().GetMetadataFields()
		for field in seqMetadataFields:
			self.cboDiversity.Append(field)
			self.cboCount.Append(field)
		
		#try and set intelligent default choices for combo boxes
		good_diversity_choice = "Label"
		if good_diversity_choice in seqMetadataFields:
			self.cboDiversity.SetSelection(seqMetadataFields.index(good_diversity_choice))
		else:
			self.cboDiversity.SetSelection(0)

		good_count_choice = "Count"
		if good_count_choice in seqMetadataFields:
			self.cboCount.SetSelection(seqMetadataFields.index(good_count_choice))
		else:
			self.cboCount.SetSelection(0)
			
	def OnRun( self, event ):
		if hasattr(self,'rca'):
                        self.rca.clearLines()
                        del self.rca
		rca = CABIN_RCA(self.cboDiversity.GetStringSelection(), self.cboCount.GetStringSelection())
		rca.Run_RCA(self.cboMetric.GetStringSelection())
		self.rca=rca
		
	def OnUpdate(self, event):		
		try:
		        #Update the viewport using the metric selected by user
			self.rca.ViewportPlot(self.cboMetric.GetStringSelection())
		except AttributeError:
			wx.MessageBox("Please 'Run' this plugin before attempting to update the plot!")
			return

	def OnOK( self, event ):
		self.OnClose(event)

	def OnClose( self, event ):
		if hasattr(self,'rca'):
			self.rca.clearLines()
			del self.rca		
		self.Destroy()

	def onSaveFile( self, event ):
		try:
			self.rca.SaveResults(self.filePicker.GetPath())
		except AttributeError:
			wx.MessageBox("Please 'Run' this plugin before attempting to save the results!")
			return
	
