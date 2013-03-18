from AlphaDiversityLayout import AlphaDiversityLayout
import GenGIS
from dataHelper import isNumber

import wx

from AlphaDiversityMeasures import AlphaDiversityMeasures

class AlphaDiversity( AlphaDiversityLayout ):
	def __init__(self, parent=None):
		AlphaDiversityLayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		self.graphicalElementIds = []
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0 or GenGIS.layerTree.GetNumSequenceLayers() == 0:
			wx.MessageBox("This plugin requires map, location, and sequence data to be loaded.", "Additional data required.")
			self.Close()
			return

		# set measure combobox
		self.AlphaDiversityMeasures = AlphaDiversityMeasures()
		for measure in self.AlphaDiversityMeasures.measures:
			self.cboMeasure.Append(measure)
		self.cboMeasure.SetSelection(0)
		
		# set category field combobox
		seqLayerController = GenGIS.layerTree.GetSequenceLayer(0).GetController()
		fields = seqLayerController.GetData().keys()

		for field in fields:
			if field.lower() != 'siteid' and field.lower() != 'site id':
				self.cboCategoryField.Append(field)
		self.cboCategoryField.SetSelection(0)
		
		# set count field combobox
		numericFields = ['<none>']
		for field in sorted(fields):
			bNumeric = True
			for i in xrange(0, GenGIS.layerTree.GetNumSequenceLayers()):
				if isNumber(GenGIS.layerTree.GetSequenceLayer(i).GetController().GetData()[field]) == False:
					bNumeric = False
					break
					
			if bNumeric:
				numericFields.append(field)
				
		for field in numericFields:
			self.cboCountField.Append(field)
		self.cboCountField.SetSelection(0)
		
		self.UpdateInfo()
		
	def OnOK( self, event ):
		self.Close()
		
	def OnPerformSubsampling( self, event ):
		self.cboReplicates.Enable(self.chkSubsample.IsChecked())
		self.txtSeqsToDraw.Enable(self.chkSubsample.IsChecked())
		self.btnMinimumSeqs.Enable(self.chkSubsample.IsChecked())
		
	def OnSetMinimumSeqs( self, event ):
		wx.BeginBusyCursor()
		
		bCountField = (self.cboCountField.GetStringSelection() != '<none>')
		
		activeLocLayers = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
		minSeqs = None
		for locLayer in activeLocLayers:
			seqLayers = locLayer.GetAllActiveSequenceLayers()
			numSeqs = 0
			for seqLayer in seqLayers:
				data = seqLayer.GetController().GetData()
				value = data[self.cboCategoryField.GetStringSelection()]
				
				count = 1
				if bCountField:
					count = float(data[self.cboCountField.GetStringSelection()])
					
				numSeqs += count
				
			if minSeqs == None or numSeqs < minSeqs:
				minSeqs = numSeqs
				
		self.txtSeqsToDraw.SetValue(str(int(minSeqs)))
		
		wx.EndBusyCursor()
		
	def OnMeasureChanged( self, event ):
		self.UpdateInfo()
		
	def UpdateInfo(self):
		info = self.AlphaDiversityMeasures.Info(self.cboMeasure.GetStringSelection())
		self.txtFullName.SetValue(info[0])
		self.txtFormula.SetValue(info[1])
		self.txtDescription.SetValue(info[2])

	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/Description_of_GenGIS_plugins#Alpha_Diversity' )
		
	def OnCalculate( self, event ):
		wx.BeginBusyCursor()
		
		self.txtLog.AppendText('Calculating ' + self.cboMeasure.GetStringSelection() + ' diversity...\n')
		sampleDict = {}
		
		# get dictionary indicating the number of times each field value is present
		# in each location
		bCountField = (self.cboCountField.GetStringSelection() != '<none>')
		
		activeLocLayers = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
		for locLayer in activeLocLayers:
			locName = locLayer.GetName()
			sampleDict[locName] = {}
			
			seqLayers = locLayer.GetAllActiveSequenceLayers()
			for seqLayer in seqLayers:
				data = seqLayer.GetController().GetData()
				value = data[self.cboCategoryField.GetStringSelection()]
				
				count = 1
				if bCountField:
					count = float(data[self.cboCountField.GetStringSelection()])
					
				sampleDict[locName][value] = sampleDict[locName].get(value, 0) + float(count)
				
		if self.chkSubsample.IsChecked():
			replicates = int(self.cboReplicates.GetStringSelection())
			seqsToDraw = int(self.txtSeqsToDraw.GetValue())
			indices = self.AlphaDiversityMeasures.CalculateJackknife(self.cboMeasure.GetStringSelection(), sampleDict, self.chkIgnoreOther.IsChecked(), replicates, seqsToDraw)
		else:
			indices = self.AlphaDiversityMeasures.Calculate(self.cboMeasure.GetStringSelection(), sampleDict, self.chkIgnoreOther.IsChecked())
		
		self.txtLog.AppendText(self.cboMeasure.GetStringSelection() + ':\n')
		for sampleName in indices.keys():
			self.txtLog.AppendText('  ' + sampleName + ': ' + str(indices[sampleName]) + '\n')
			
		# add alpha diversity results to location metadata table
		orderedIndexValues = []
		for loc in activeLocLayers:
			orderedIndexValues.append(str(indices[loc.GetName()]))
			
		newDivName = self.cboMeasure.GetStringSelection() + ' (' + self.cboCategoryField.GetStringSelection() + ')'
		
		GenGIS.layerTree.GetLocationSetLayer(0).GetController().AddMetadata(newDivName, orderedIndexValues)
		
		self.txtLog.AppendText('\n' + self.cboMeasure.GetStringSelection() + ' results added to location metadata table as "' + newDivName + '".\n\n')
		
		wx.EndBusyCursor()