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
		
		# populate Group combo box with location column names
		locationSetLayer = GenGIS.layerTree.GetLocationSetLayer(0)
		numericFields = locationSetLayer.GetController().GetMetadataFields()
		for field in numericFields:
			self.cboGroup.Append(field) 
		self.cboGroup.SetSelection(0)
		
		# populate Diversity and Count combo boxes with sequence column names
		sequenceLayer = GenGIS.layerTree.GetSequenceLayer(0)
		seqMetadataFields = sequenceLayer.GetController().GetMetadataFields()
		for field in sorted(seqMetadataFields):
			self.cboDiversity.Append(field)
			self.cboCount.Append(field)
		
		self.cboDiversity.SetSelection(0)
		self.cboCount.SetSelection(0)
			
	def OnRun( self, event ):
		rca = CABIN_RCA(self.cboGroup.GetStringSelection(), self.cboDiversity.GetStringSelection(), self.cboCount.GetStringSelection())
		rca.Run_RCA("OoverE")
		
	def OnOK( self, event ):
		self.Close()