from BarGraphLayout import BarGraphLayout
from BarGraphGeneric import *
import GenGIS
import wx
# import re

class BarGraph( BarGraphLayout ):
	def __init__(self, parent = None):
		BarGraphLayout.__init__ ( self, parent )
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		# check that required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0 or GenGIS.layerTree.GetNumSequenceLayers() == 0:
			wx.MessageBox("This plugin requires map, location, and sequence data to be loaded.", "Additional data required.")
			self.Close()
			return
		
		for m in GenGIS.layerTree.GetLocationSetLayer(0).GetController().GetMetadataFields():
			self.chcFieldSel.Append(m)

		self.chcLocSel.Disable()
		self.chcLocSel2.Disable()
		self.btnCalculate.Disable()
	
		# set variable comboboxes
		sequenceSetLayer = GenGIS.layerTree.GetSequenceLayer(0)
		numericFields = sequenceSetLayer.GetController().GetMetadataFields()
		for field in numericFields:
			self.chcVarSel.Append(field)
		self.chcVarSel.SetSelection(0)

		# initialize plot
		self.plot = BarGraphGeneric(self.pnlHistogram)
		
		self.locationTotals = dict()
		for layer in GenGIS.layerTree.GetSequenceLayers():
			controller = layer.GetController()
			if (controller.IsActive()):
				data = controller.GetData()
				if ("Site ID" in data):
					siteID = data["Site ID"]
				elif ("Site id" in data):
					siteID = data["Site id"]
				elif ("Site Id" in data):
					siteID = data["Site Id"]

				if (siteID not in self.locationTotals):
					self.locationTotals[siteID] = 0
				if ("Count" in data):
					self.locationTotals[siteID] += int(data["Count"])
				else:
					self.locationTotals[siteID] += 1
	
	def OnChoice( self, event ):
		choices = set()
		for loc in GenGIS.layerTree.GetLocationLayers():
			if (loc.GetController().GetData()[self.chcFieldSel.GetStringSelection()] not in choices):
				choices.add(loc.GetController().GetData()[self.chcFieldSel.GetStringSelection()])
	
		self.match = dict()
		for location in GenGIS.layerTree.GetLocationLayers():
			temp = location.GetController().GetData()
			if ("Site ID" in temp):
				siteID = temp["Site ID"]
			elif ("Site id" in temp):
				siteID = temp["Site id"]
			elif ("Site Id" in temp):
				siteID = temp["Site Id"]
			self.match[siteID] = temp[self.chcFieldSel.GetStringSelection()]
	
		self.chcLocSel.Clear()
		self.chcLocSel2.Clear()
		
		#Puts Site IDs in drop menus
		valList = sorted(list(choices))
		self.chcLocSel2.Append("N/A")
		for v in valList:
			self.chcLocSel.Append(v)
			self.chcLocSel2.Append(v)
		self.chcLocSel.SetSelection(0)
		self.chcLocSel2.SetSelection(0)
		
		self.chcLocSel.Enable()
		self.chcLocSel2.Enable()
		self.btnCalculate.Enable()
			
	def OnCalculate( self, event ):
		wx.BeginBusyCursor()
		
		if self.chcLocSel.Enabled == True:
			self.groupIDOne = self.chcLocSel.GetStringSelection()
		if self.chcLocSel2.Enabled == True:
			self.groupIDTwo = self.chcLocSel2.GetStringSelection()
			
		if self.chcVarSel.Enabled == True:
			self.Variable = self.chcVarSel.GetStringSelection()
			
		getData = self.GetData()
		groupOne = getData[0]
		groupTwo = getData[1]
		
		print "Mean and standard deviation for " + self.groupIDOne
		meansOne, standardDeviationsOne = self.Statistics(groupOne, self.groupIDOne)
		print "\n\nMean and standard deviation for " + self.groupIDTwo
		meansTwo, standardDeviationsTwo = self.Statistics(groupTwo, self.groupIDTwo)
		
		typeOne = dict()
		
		#Calculates total to use for ratio
		totalOne = 0
		for item in groupOne.items():
			for number in item[1].values():
				if (item[0] not in typeOne):
					typeOne[item[0]] = list()
				totalOne += number
				typeOne[item[0]].append(number)

		typeTwo = dict()
		
		#Calculates total to use for ratio
		totalTwo = 0
		for item in groupTwo.items():
			for number in item[1].values():
				if (item[0] not in typeTwo):
					typeTwo[item[0]] = list()
				totalTwo += number
				typeTwo[item[0]].append(number)
		
		self.Draw(typeOne, typeTwo, totalOne, totalTwo, standardDeviationsOne, standardDeviationsTwo)

		wx.EndBusyCursor()

	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/Description_of_GenGIS_plugins#Bar_Graph' )
		
	def GetData(self):
		groupOne = dict()
		groupTwo = dict()
		
		for seqLayer in GenGIS.layerTree.GetSequenceLayers():
			seqController = seqLayer.GetController()
			
			if seqController.IsActive():
				siteID = seqController.GetSiteId()
				if (self.match[siteID] == self.groupIDOne):
					line = seqController.GetData()
					variable = line[self.Variable]
					if (variable not in groupOne):
						groupOne[variable] = dict()
						
					count = 1
					if ("Count" in line):
						count = int(line["Count"])
						
					if (siteID not in groupOne[variable]):
						groupOne[variable][siteID] = count / float(self.locationTotals[siteID])	
					else:
						groupOne[variable][siteID] += count / float(self.locationTotals[siteID])
					
				if (self.match[siteID] == self.groupIDTwo):
					line = seqController.GetData()
					variable = line[self.Variable]
					if (variable not in groupTwo):
						groupTwo[variable] = dict()
						
					count = 1
					if ("Count" in line):
						count = int(line["Count"])

					if (siteID not in groupTwo[variable]):
						groupTwo[variable][siteID] = count / float(self.locationTotals[siteID])	
					else:
						groupTwo[variable][siteID] += count / float(self.locationTotals[siteID])

		return (groupOne, groupTwo)
		
	#Returns a dictionary of means and standard deviations
	def Statistics( self, group, groupID ):
		standardDeviations = dict()
		means = dict()
		for (type, locationAndValues) in sorted(group.items()):
			listOfValues = list (locationAndValues.values())
			for (location, matchValue) in self.match.items():
				if (matchValue == groupID):
					if (location not in locationAndValues.keys()):
						listOfValues.append(0)

			avg = sum(listOfValues)/float(len(listOfValues))
			std = ((sum([((x - avg) ** 2) for x in listOfValues])/float(len(listOfValues))) ** 0.5)
			print str(type) + " Mean: " + str(avg)
			print str(type) + " Standard Deviation: " + str(std) + "\n"
			means[type] = avg
			standardDeviations[type]  = std
		return means, standardDeviations


	def Draw(self, graphOne, graphTwo, totalOne, totalTwo, stdOne, stdTwo):
		xCounter = 0
		self.x = []
		self.yOne = []
		self.yTwo = []
		self.standardErrorOne = []
		self.standardErrorTwo = []
	
		for element in sorted(set(graphOne.keys()) | set(graphTwo.keys())):
			self.x.append(xCounter)
			xCounter += 1
			if (element in graphOne.keys()):
				self.yOne.append(sum(graphOne[element]) / float(totalOne))
				self.standardErrorOne.append(stdOne[element])
			else:
				self.yOne.append(0)
				self.standardErrorOne.append(0)
				
			if (element in graphTwo):
				self.yTwo.append(sum(graphTwo[element]) / float(totalTwo))
				self.standardErrorTwo.append(stdTwo[element])
			else:
				self.yTwo.append(0)
				self.standardErrorTwo.append(0)
		
		sortedNames = sorted(set(graphOne) | set(graphTwo))
		
		self.plot.Draw(self.x, self.yOne, self.yTwo, self.groupIDOne, self.groupIDTwo,
						stdOne = self.standardErrorOne, stdTwo = self.standardErrorTwo,
						xLabel = self.Variable, xTicks = sortedNames)
		
	def OnClose( self, event ):
		self.Close()