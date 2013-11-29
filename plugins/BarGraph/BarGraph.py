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

from BarGraphLayout import BarGraphLayout
from BarGraphGeneric import *

import GenGIS
import wx

class BarGraph( BarGraphLayout ):
	def __init__(self, parent = None):
		BarGraphLayout.__init__ ( self, parent )
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		# check that required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0 or GenGIS.layerTree.GetNumSequenceLayers() == 0:
			wx.MessageBox("This plugin requires map, location, and sequence data to be loaded.", "Additional data required.")
			self.Close()
			return
		
		# populate fields that can be used to define groupings
		for field in GenGIS.layerTree.GetLocationSetLayer(0).GetController().GetMetadataFields():
			self.cboGroupField.Append(field)

		self.cboGroup1.Disable()
		self.cboGroup2.Disable()
		self.btnPlot.Disable()

		# set valid data fields for generating bar graph
		sequenceSetLayer = GenGIS.layerTree.GetSequenceLayer(0)
		numericFields = sorted(sequenceSetLayer.GetController().GetMetadataFields())
		for field in numericFields:
			self.cboDataField.Append(field)
		self.cboDataField.SetSelection(0)
		
		# set count field combobox
		numericFields = ['<none>'] + numericFields
		for field in numericFields:
			self.cboCountField.Append(field)
		self.cboCountField.SetSelection(0)
		
		# initialize plot
		self.plot = BarGraphGeneric(self.pnlHistogram)
	
	def OnChangeGroupField( self, event ):
		# get value of field for each location
		self.locationValue = dict()
		for location in GenGIS.layerTree.GetLocationLayers():
			temp = location.GetController().GetData()
			if ("Site ID" in temp):
				siteId = temp["Site ID"]
			elif ("Site id" in temp):
				siteId = temp["Site id"]
			elif ("Site Id" in temp):
				siteId = temp["Site Id"]
			self.locationValue[siteId] = temp[self.cboGroupField.GetStringSelection()]
			
		# determine fields that can be used to define groups of sample sites
		choices = set()
		for loc in GenGIS.layerTree.GetLocationLayers():
			choices.add(loc.GetController().GetData()[self.cboGroupField.GetStringSelection()])

		self.cboGroup1.Clear()
		self.cboGroup2.Clear()
		valList = sorted(list(choices))
		for v in valList:
			self.cboGroup1.Append(v)
			self.cboGroup2.Append(v)
		self.cboGroup1.SetSelection(0)
		self.cboGroup2.SetSelection(0)
		if len(valList) >= 2:
			self.cboGroup2.SetSelection(1)
		
		self.cboGroup1.Enable()
		self.cboGroup2.Enable()
		self.btnPlot.Enable()
			
	def OnPlot( self, event ):
		wx.BeginBusyCursor()
		
		group1, totalSeqs1, group2, totalSeqs2 = self.GetData()

		means1, standardDeviations1 = self.Statistics(group1, self.cboGroup1.GetStringSelection())
		means2, standardDeviations2 = self.Statistics(group2, self.cboGroup2.GetStringSelection())
		
		# calculates total to use for ratio
		bars1 = {}
		for value in group1:
			for data in group1[value].items():
				bars1[value] = bars1.get(value, 0) + data[1]

		bars2 = {}
		for value in group2:
			for data in group2[value].items():
				bars2[value] = bars2.get(value, 0) + data[1]
		
		self.Draw(bars1, bars2, standardDeviations1, standardDeviations2)

		wx.EndBusyCursor()
		
	def GetData(self):
		group1 = {}
		totalSeq1 = 0
		group2 = {}
		totalSeq2 = 0
		for seqLayer in GenGIS.layerTree.GetSequenceLayers():
			seqController = seqLayer.GetController()
			
			if seqController.IsActive():
				siteId = seqController.GetSiteId()
				data = seqController.GetData()
				value = data[self.cboDataField.GetStringSelection()]
				
				if self.locationValue[siteId] == self.cboGroup1.GetStringSelection():
					if (value not in group1):
						group1[value] = {}
						
					count = 1.0
					if self.cboCountField.GetStringSelection() != "<none>":
						count = float(data[self.cboCountField.GetStringSelection()])
						
					group1[value][siteId] = group1[value].get(siteId, 0) + count
					totalSeq1 += count
					
				if self.locationValue[siteId] == self.cboGroup2.GetStringSelection() :
					if (value not in group2):
						group2[value] = dict()
						
					count = 1.0
					if self.cboCountField.GetStringSelection() != "<none>":
						count = float(data[self.cboCountField.GetStringSelection()])
						
					group2[value][siteId] = group2[value].get(siteId, 0) + count
					totalSeq2 += count
					
		# normalize data
		for value in group1:
			for siteId in group1[value]:
				group1[value][siteId] /= totalSeq1
				
		for value in group2:
			for siteId in group2[value]:
				group2[value][siteId] /= totalSeq2

		return (group1, totalSeq1, group2, totalSeq2)

	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/index.php/Description_of_GenGIS_plugins#Bar_Graph' )
		
	#Returns a dictionary of means and standard deviations
	def Statistics( self, group, groupId ):
		standardDeviations = dict()
		means = dict()
		for (type, locationAndValues) in sorted(group.items()):
			listOfValues = list (locationAndValues.values())
			for (location, matchValue) in self.locationValue.items():
				if (matchValue == groupId):
					if (location not in locationAndValues.keys()):
						listOfValues.append(0)

			avg = sum(listOfValues)/float(len(listOfValues))
			std = ((sum([((x - avg) ** 2) for x in listOfValues])/float(len(listOfValues))) ** 0.5)
			means[type] = avg
			standardDeviations[type]  = std
		return means, standardDeviations

	
	def Draw(self, bars1, bars2, std1, std2):
		xCounter = 0
		x = []
		y1 = []
		y2 = []
		standardError1 = []
		standardError2 = []
		
		sortedNames = sorted(set(bars1) | set(bars2))
		for element in sortedNames:
			x.append(xCounter)
			xCounter += 1
			
			if element in bars1.keys():
				y1.append(bars1[element])
				standardError1.append(std1[element])
			else:
				y1.append(0)
				standardError1.append(0)
				
			if element in bars2.keys():
				y2.append(bars2[element])
				standardError2.append(std2[element])
			else:
				y2.append(0)
				standardError2.append(0)

		self.plot.Draw(x, y1, y2, self.cboGroup1.GetStringSelection(), self.cboGroup2.GetStringSelection(),
						stdOne = standardError1, stdTwo = standardError2,
						xLabel = self.cboDataField.GetStringSelection(), xTicks = sortedNames)

	def OnSavePlot(self, event):
		self.plot.SavePlot()

	def OnOK( self, event ):
		self.Close()