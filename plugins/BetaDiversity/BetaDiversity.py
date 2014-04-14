#=======================================================================
# Author: Donovan Parks
#
# Copyright 2009 Donovan Parks
#
# This file is part of GenGIS.
#
# GenGIS is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# GenGIS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GenGIS.  If not, see <http://www.gnu.org/licenses/>.
#=======================================================================

from BetaDiversityLayout import BetaDiversityLayout
import GenGIS
from dataHelper import isNumber

import wx
import os

from BetaDiversityMeasures import BetaDiversityMeasures

from scipy.cluster.hierarchy import single, complete, average, weighted, to_tree

class BetaDiversity( BetaDiversityLayout ):
	def __init__(self, parent=None):
		BetaDiversityLayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		self.graphicalElementIds = []
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0 or GenGIS.layerTree.GetNumSequenceLayers() == 0:
			wx.MessageBox("This plugin requires map, location, and sequence data to be loaded.", "Additional data required.")
			self.Close()
			return

		# set measure combobox
		self.betaDiversityMeasures = BetaDiversityMeasures()
		for measure in self.betaDiversityMeasures.measures:
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
		self.results = None
		self.newickStr = None
		
	def OnOK( self, event ):
		self.Close()
	
	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/index.php/Description_of_GenGIS_plugins#Beta_Diversity_Calculator' )
		
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
		info = self.betaDiversityMeasures.Info(self.cboMeasure.GetStringSelection())
		self.txtFullName.SetValue(info[0])
		self.txtFormula.SetValue(info[1])
		self.txtDescription.SetValue(info[2])
		
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
			self.results = self.betaDiversityMeasures.CalculateJackknife(self.cboMeasure.GetStringSelection(), sampleDict, self.chkIgnoreOther.IsChecked(), replicates, seqsToDraw)
		else:
			self.results = self.betaDiversityMeasures.Calculate(self.cboMeasure.GetStringSelection(), sampleDict, self.chkIgnoreOther.IsChecked())
		
		self.txtLog.AppendText(self.cboMeasure.GetStringSelection() + ':\n')
		for sampleNameI in self.results.keys():
			for sampleNameJ in self.results[sampleNameI].keys():
				self.txtLog.AppendText('  ' + sampleNameI + '-' + sampleNameJ + ': ' + str(self.results[sampleNameI][sampleNameJ]) + '\n')
		self.txtLog.AppendText('\n')
		
		self.newickStr = self.CalculateClusterTree()
		
		self.txtLog.AppendText(self.newickStr + '\n\n')
		
		wx.EndBusyCursor()
		
	def CalculateClusterTree(self):
		fullMatrix = self.GenerateFullMatrix(self.results)
		dissMatrix = []
		labels = fullMatrix.keys()
		for i in xrange(0, len(labels)):
			sampleNameI = labels[i]
			for j in xrange(i+1, len(labels)):
				sampleNameJ = labels[j]
				dissMatrix.append(fullMatrix[sampleNameI][sampleNameJ])
				
		# calculate hierarchical cluster tree
		if self.radioSingleLinkage.GetValue():
			linkageMatrix = single(dissMatrix)
		elif self.radioUPGMA.GetValue():
			linkageMatrix = average(dissMatrix)
		elif self.radioCompleteLinkage.GetValue():
			linkageMatrix = complete(dissMatrix)
		elif self.radioWeighted.GetValue():
			linkageMatrix = weighted(dissMatrix)
			
		root = to_tree(linkageMatrix)
		
		# create Newick string
		return self.CreateNewickString(root, labels) + ';'
		
	def CreateNewickString(self, node, labels):
		if node.is_leaf():
			return labels[node.get_id()]
		else:
			output = ""
			output += "("
			output += self.CreateNewickString(node.get_left(), labels)
			height = node.dist - node.get_left().dist
			output += ":" + str(height) + ","
			output += self.CreateNewickString(node.get_right(), labels)
			height = node.dist - node.get_right().dist
			output += ":" + str(height) + ")"
			
			return output
			
	def GenerateFullMatrix(self, resultsDict):
		sampleNames = resultsDict.keys()
		fullMatrix = {}
		for sampleNameI in sampleNames:
			if sampleNameI not in fullMatrix:
				fullMatrix[sampleNameI] = {}

			fullMatrix[sampleNameI][sampleNameI] = 0
			for sampleNameJ in resultsDict[sampleNameI].keys():
				fullMatrix[sampleNameI][sampleNameJ] = resultsDict[sampleNameI][sampleNameJ]
				if sampleNameJ not in fullMatrix:
					fullMatrix[sampleNameJ] = {}
				fullMatrix[sampleNameJ][sampleNameI] = resultsDict[sampleNameI][sampleNameJ]
		return fullMatrix

	def OnSaveDissMatrix( self, event ):
		if self.results == None:
			wx.MessageBox("You must run a beta-diversity measure before trying to save a dissimilarity matrix.", "Run beta-diversity measure")
			return
			
		fileTypes = 'Dissimilarity matrix (*.dst)|*.dst|Tab-separated values (*.tsv)|*.tsv|All files (*.*)|*.*'
		
		dlg = wx.FileDialog(self, "Save dissimilarity matrix", "", "", fileTypes, wx.SAVE)
		if dlg.ShowModal() == wx.ID_OK:
			filename = os.path.join( dlg.GetDirectory(), dlg.GetFileName() )
			
			# generate full matrix
			fullMatrix = self.GenerateFullMatrix(self.results)
					
			# write out results
			fout = open(filename, 'w')
			fout.write(str(len(fullMatrix)) + '\n')
			sampleNames = fullMatrix.keys()
			for sampleNameI in sampleNames:
				fout.write(sampleNameI)
				for sampleNameJ in sampleNames:
					fout.write('\t' + str(fullMatrix[sampleNameI][sampleNameJ]))
				fout.write('\n')
			fout.close()
			
	def OnSaveTree( self, event ):
		if self.newickStr == None:
			wx.MessageBox("You must run a beta-diversity measure before trying to save a cluster tree.", "Run beta-diversity measure")
			return

		fileTypes = 'Tree file (*.tre)|*.tre|All files (*.*)|*.*'

		dlg = wx.FileDialog(self, "Save tree file", "", "", fileTypes, wx.SAVE)
		if dlg.ShowModal() == wx.ID_OK:
			filename = dlg.GetDirectory() + '\\' + dlg.GetFilename()

			fout = open(filename, 'w')
			fout.write(self.newickStr)
			fout.close()
