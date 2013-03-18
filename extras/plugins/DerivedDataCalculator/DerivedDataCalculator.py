import GenGIS
import wx

from dataHelper import isNumber

from DerivedDataCalculatorLayout import DerivedDataCalculatorLayout

class DerivedDataCalculator( DerivedDataCalculatorLayout ):
	def __init__(self, parent=None):
		DerivedDataCalculatorLayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0 or GenGIS.layerTree.GetNumSequenceLayers() == 0:
			wx.MessageBox("This plugin requires map, location, and sequence data to be loaded.", "Additional data required.")
			self.Close()
			return
			
		# populate unique sequence id combobox
		for field in GenGIS.layerTree.GetSequenceLayer(0).GetController().GetMetadataFields():
			self.cboUniqueSeqIdField.Append(field)
		self.cboUniqueSeqIdField.SetSelection(0)
		
		# populate sequence count comboxbox with 'likely' numeric fields
		for field in GenGIS.layerTree.GetSequenceLayer(0).GetController().GetMetadataFields():
			# check if field is numeric for first sequence
			value = GenGIS.layerTree.GetSequenceLayer(0).GetController().GetData()[field]
			if isNumber(value):
				self.cboSeqCountField.Append(field)
				
		if self.cboSeqCountField.GetCount() >= 1:
			self.cboSeqCountField.SetSelection(0)
		else:
			self.cboSeqCountField.Append('<No numeric data>')
			self.cboSeqCountField.SetStringSelection('<No numeric data>')
			self.btnUniqueSeqs.Disable()
			self.btnTotalSeqs.Disable()
			self.btnNormalizeUniqueSeqs.Disable()
			
		# resize frame to fit widgets
		self.Fit()
			
	def OnOK( self, event ):
		self.Close()
		
	def GetData(self):
		# get unique sequence id and sequence count fields
		uniqueSequenceIdField = self.cboUniqueSeqIdField.GetStringSelection()
		sequenceCountField = self.cboSeqCountField.GetStringSelection()
		
		# get normalized number of unique sequences
		self.totalSeqs = []
		self.uniqueSeqs = []
		self.normUniqueSeqs = []
		
		activeLocLayers = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
		for locLayer in activeLocLayers:
			uniqueSeq = set([])
			seqCount = 0
					
			seqLayers = locLayer.GetAllActiveSequenceLayers()
			for seqLayer in seqLayers:
				data = seqLayer.GetController().GetData()
				uniqueSeq.add(data[uniqueSequenceIdField])
				seqCount += float(data[sequenceCountField])
				
			self.totalSeqs.append(str(seqCount))
			self.uniqueSeqs.append(str(len(uniqueSeq)))
			self.normUniqueSeqs.append(str(float(len(uniqueSeq))/seqCount))
		
	def OnUniqueSeqs( self, event ):
		self.GetData()
		GenGIS.layerTree.GetLocationSetLayer(0).GetController().AddMetadata('Unique sequences', self.uniqueSeqs)
		wx.MessageBox("Added 'Unique sequences' field to all locations.", "Metadata field added")
	
	def OnTotalSeqs( self, event ):
		self.GetData()
		GenGIS.layerTree.GetLocationSetLayer(0).GetController().AddMetadata('Total sequences', self.totalSeqs)
		wx.MessageBox("Added 'Total sequences' field to all locations.", "Metadata field added")
	
	def OnNormUniqueSeqs( self, event ):
		self.GetData()
		GenGIS.layerTree.GetLocationSetLayer(0).GetController().AddMetadata('Normalized unique sequence count', self.normUniqueSeqs)
		wx.MessageBox("Added 'Normalized unique sequence count' field to all locations.", "Metadata field added")