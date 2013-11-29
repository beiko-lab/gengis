#=======================================================================
# Author: Donovan Parks
#
# Copyright 2012 Brett O'Donnell
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


from MultiTreeOptimalCrossingTestLayout import MultiTreeOptimalCrossingTestLayout
from MultiTreeGraph import *
from BarGraphGeneric import *
import GenGIS
import wx
import scipy.stats.stats as stats

class MultiTreeOptimalCrossingTest( MultiTreeOptimalCrossingTestLayout ):
	def __init__(self, parent = None):
		MultiTreeOptimalCrossingTestLayout.__init__ ( self, parent )
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		self.layerTree = GenGIS.layerTree
		
		# initialize plot
		self.plot = MultiTreeGraph(self.pnlStepGraph)
		self.plotTwo = BarGraphGeneric(self.pnlBarGraph)
		
		# check that required data has been loaded
		if self.layerTree.GetNumTreeLayers() == 0:
			wx.MessageBox("This plugin requires one or more trees to be loaded.", "Additional data required.")
			self.Close()
			
		x = 0
		while (x < self.layerTree.GetNumTreeLayers()):
			self.chcTreeSel.Append(self.layerTree.GetTreeLayer(x).GetName())
			x += 1
		self.chcTreeSel.SetSelection(0)
		
		treeCounter = 0
		results = list()
		self.treeNames = list()
		# Pulls all tree layers from GenGIS
		while (treeCounter < self.layerTree.GetNumTreeLayers()):
			if (self.layerTree.GetTreeLayer(treeCounter).IsActive()):
				results.append(self.layerTree.GetTreeLayer(treeCounter).GetGeoTreeView().PerformLinearAxesAnalysisOnRootNode())
				self.treeNames.append(self.layerTree.GetTreeLayer(treeCounter).GetName())
			treeCounter += 1
			
		# Makes a list for each tree, initialises empty lists to be used for manipulation
		self.treeCrossingLists = list()
		self.geoTreeViewList = list()
		listCounter = 0
		for treeAnalysis in results:
			self.treeCrossingLists.append(list())
			self.geoTreeViewList.append(self.layerTree.GetTreeLayer(listCounter).GetGeoTreeView())
			# Stores every crossing change as a tuple value
			for crossingChange in treeAnalysis:
				self.treeCrossingLists[listCounter].append((crossingChange.angle, crossingChange.numCrossings))
			listCounter += 1
		# print self.treeCrossingLists
		
		self.longestList = 0
		listCounter = 0
		self.optimalCrossingList = list()
		self.leastOptimalCrossingList = list()
		for listOfAngles in self.treeCrossingLists:
			# Optimal angle for each tree
			self.optimalCrossingList.append((self.FindOptimal(listOfAngles)))
			# Least optimal angle for each tree
			self.leastOptimalCrossingList.append((self.FindLeastOptimal(listOfAngles)))
			# Finds the longest list
			if (len(listOfAngles) > len(self.treeCrossingLists[self.longestList])):
				self.longestList = listCounter
			listCounter += 1
			
		#Checks if min and max crossings are the same
		listCounter = 0
		while (listCounter < len(self.treeCrossingLists)):
			if (self.leastOptimalCrossingList[listCounter] == self.optimalCrossingList[listCounter]):
				wx.MessageBox("This plugin requires one or more trees to be loaded.", "Additional data required.")
				self.Close()
			listCounter += 1
		
	def OnCalculate( self, event ):		
		listOfIndices = [0] * len(self.treeNames)
		listOfPreviousCrossings = [0] * len(self.treeNames)
			
		# Where the magic happens
		currentTotalCrossings = 0
		totalCrossingList = list()
		while (listOfIndices[self.longestList] < len(self.treeCrossingLists[self.longestList])):
			nextHighestAngle = 0
			loopCounter = 0
			while (loopCounter < len(self.treeCrossingLists)):
				# print "Loop Counter: " + str(loopCounter)
				# print "List of Indices[Loop Counter]: " + str(listOfIndices[loopCounter])
				if (listOfIndices[loopCounter] < len(self.treeCrossingLists[loopCounter]) and self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][0] > nextHighestAngle):
					nextHighestAngle = self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][0]
				# print "nextHighestAngle: " + str(nextHighestAngle)
				loopCounter += 1
					
			loopCounter = 0
			while (loopCounter < len(self.treeCrossingLists)):
				if (listOfIndices[loopCounter] < len(self.treeCrossingLists[loopCounter]) and nextHighestAngle == self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][0]):
					# Normalised (Squeeze each tree between 0.0 an 1.0)
					if (self.chcCrossingWeighted.GetStringSelection() == "Normalised"):
						currentTotalCrossings = currentTotalCrossings - listOfPreviousCrossings[loopCounter]/float(self.leastOptimalCrossingList[loopCounter][1] - self.optimalCrossingList[loopCounter][1]) + ((self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1] - self.optimalCrossingList[loopCounter][1])/float(self.leastOptimalCrossingList[loopCounter][1] - self.optimalCrossingList[loopCounter][1]))
						listOfPreviousCrossings[loopCounter] = self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1] - self.optimalCrossingList[loopCounter][1]
					# Unweighted
					elif (self.chcCrossingWeighted.GetStringSelection() == "Unweighted"):
						currentTotalCrossings = currentTotalCrossings - listOfPreviousCrossings[loopCounter] + self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1]
						listOfPreviousCrossings[loopCounter] = self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1]
					# # # Least-Optimal-Crossing Weighted
					# # if (self.chcCrossingWeighted.GetStringSelection() == "Least-Optimal-Crossing Weighted"):
						# # currentTotalCrossings = currentTotalCrossings - listOfPreviousCrossings[loopCounter]/float(self.leastOptimalCrossingList[loopCounter][1]) + ((self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1])/float(self.leastOptimalCrossingList[loopCounter][1]))
						# # listOfPreviousCrossings[loopCounter] = self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1]
					# # # Optimal-Crossing Weighted
					# # elif (self.chcCrossingWeighted.GetStringSelection() == "Optimal-Crossing Weighted"):
						# # currentTotalCrossings = currentTotalCrossings - listOfPreviousCrossings[loopCounter]/float(self.optimalCrossingList[loopCounter][1]) + ((self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1])/float(self.optimalCrossingList[loopCounter][1]))
						# # listOfPreviousCrossings[loopCounter] = self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1]
					# # # Optimal-Crossing Weighted and Zeroed
					# # elif (self.chcCrossingWeighted.GetStringSelection() == "Optimal-Crossing Weighted and Zeroed"):
						# # currentTotalCrossings = currentTotalCrossings - listOfPreviousCrossings[loopCounter]/float(self.optimalCrossingList[loopCounter][1]) + ((self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1] - self.optimalCrossingList[loopCounter][1])/float(self.optimalCrossingList[loopCounter][1]))
						# # listOfPreviousCrossings[loopCounter] = self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1] - self.optimalCrossingList[loopCounter][1]
					# # # Tree-Size Weighted
					# # elif (self.chcCrossingWeighted.GetStringSelection() == "Tree-Size Weighted"):
						# # currentTotalCrossings = currentTotalCrossings - listOfPreviousCrossings[loopCounter]/float(self.geoTreeViewList[loopCounter].GetNumberOfLeaves()) + self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1]/float(self.geoTreeViewList[loopCounter].GetNumberOfLeaves())
						# # listOfPreviousCrossings[loopCounter] = self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1]
					# # # Tree-Size Weighted and Zeroed
					# # elif (self.chcCrossingWeighted.GetStringSelection() == "Tree-Size Weighted and Zeroed"):
						# # currentTotalCrossings = currentTotalCrossings - listOfPreviousCrossings[loopCounter]/float(self.geoTreeViewList[loopCounter].GetNumberOfLeaves()) + ((self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1] - self.optimalCrossingList[loopCounter][1])/float(self.geoTreeViewList[loopCounter].GetNumberOfLeaves()))
						# # listOfPreviousCrossings[loopCounter] = self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1] - self.optimalCrossingList[loopCounter][1]
					# # # Zeroed
					# # elif (self.chcCrossingWeighted.GetStringSelection() == "Zeroed"):
						# # currentTotalCrossings = currentTotalCrossings - (listOfPreviousCrossings[loopCounter]) + (self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1] - self.optimalCrossingList[loopCounter][1])
						# # listOfPreviousCrossings[loopCounter] = self.treeCrossingLists[loopCounter][listOfIndices[loopCounter]][1] - self.optimalCrossingList[loopCounter][1]
					listOfIndices[loopCounter] += 1
				loopCounter += 1
			totalCrossingList.append((nextHighestAngle, currentTotalCrossings))
		# print totalCrossingList
		# print listOfIndices
		
		self.globalOptimalAngle, globalOptimalCrossings = self.FindOptimal(totalCrossingList)
		
		print("Global Optimal Angle: " + str(self.globalOptimalAngle) + "\tNumber of Crossings: " + str(globalOptimalCrossings))
		self.lblGlobalOptimalAngle.SetLabel(str(self.globalOptimalAngle))
		
		angles = list()
		crossings = list()
		for tuple in totalCrossingList:
			angles.append(tuple[0])
			crossings.append(tuple[1])
		
		# Top graph (Step graph)
		self.plot.Draw(angles, crossings, globalOptimalCrossings)
			
		# Defaults bargraph to optimal angle, unless another angle is given
		if (self.txtAngle.GetValue() != "" and self.isNumber(self.txtAngle.GetValue())):
			barGraphValues = self.CompareOptimalAngles( float(self.txtAngle.GetValue()), self.optimalCrossingList )
		else:
			barGraphValues = self.CompareOptimalAngles( self.globalOptimalAngle, self.optimalCrossingList )
		
		someList = range(0, len(barGraphValues))
			
		zipSortList = zip(barGraphValues, self.treeNames)
		values = list()
		names = list()
		for value, name in sorted(zipSortList, reverse = True):
			values.append(value)
			names.append(name)
			
		# Bottom graph (Bar graph)
		self.plotTwo.Draw(someList, values, xTicks = names)
		
		loopCounter = 0
		for tree in self.treeCrossingLists:
			print "Spearman Correlation for tree " + str(self.treeNames[loopCounter]) + " crossings and global crossings: "
			self.SpearmanCorrelation(totalCrossingList, tree)
			loopCounter += 1
			
		self.btnSetAngle.Enable()
		
	def CompareOptimalAngles( self, comparitiveAngle, optimalCrossingList ):
		angleFitComparisons = list()
		# Compare each tree's optimal angle to global optimal
		loopCounter = 0
		for tuple in optimalCrossingList:
			# print tuple[1]
			# print self.treeCrossingLists[loopCounter]
			# print comparitiveAngle
			# print self.CrossingsAtAngle(self.treeCrossingLists[loopCounter], comparitiveAngle)
			angleFitComparisons.append(tuple[1] / float(self.CrossingsAtAngle(self.treeCrossingLists[loopCounter], comparitiveAngle)))
			loopCounter += 1
		print "Goodness of optimal angle for each tree: " + str(angleFitComparisons)
		return angleFitComparisons
	
	def CrossingsAtAngle( self, crossingList, angleToFind):
		crossings = -1
		# print crossingList
		# print angleToFind
		for tuple in crossingList:
			# print tuple
			if (tuple[0] >= angleToFind):
				crossings = tuple[1]
			else:
				return crossings
		return crossings
	
	def OnClose( self, event ):
		self.Close()
	
	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/index.php/Description_of_GenGIS_plugins#Multi-Tree_Optimal-Crossing_Test' )

	# Finds the angle with the least number of crossings of list with tuples of the form (angle, crossingNumber)
	def FindOptimal( self, crossingList ):
		degree = -1
		minCrossing = -1
		if (len(crossingList) != 0):
			degree = crossingList[0][0]
			minCrossing = crossingList[0][1]
		for tuple in crossingList:
			if (tuple[1] < minCrossing):
				degree = tuple[0]
				minCrossing = tuple[1]
		return degree, minCrossing
		
	# Finds the angle with the greatest number of crossings of list with tuples of the form (angle, crossingNumber)
	def FindLeastOptimal( self, crossingList ):
		degree = -1
		maxCrossing = -1
		if (len(crossingList) != 0):
			degree = crossingList[0][0]
			maxCrossing = crossingList[0][1]
		for tuple in crossingList:
			if (tuple[1] > maxCrossing):
				degree = tuple[0]
				maxCrossing = tuple[1]
		return degree, maxCrossing
		
	def SpearmanCorrelation( self, shortList, longList ):
		# comparableLongList = list()
		# for tuple in shortList:
			# comparableLongList.append((tuple[0], self.CrossingsAtAngle(longList, tuple[0])))

		crossingsShort = list()
		crossingsLong = list()
		i = 0
		while (i < len(shortList)):
			crossingsShort.append(shortList[i][1])
			crossingsLong.append(self.CrossingsAtAngle(longList, shortList[i][0]))
			i += 1
		
		print str(stats.spearmanr(crossingsLong, crossingsShort))
		# # print (crossingsLong)
		# # print (crossingsShort)
		
	def isNumber(self, s):
		try:
			float(s) # for int, long and float
		except ValueError:
			try:
				complex(s) # for complex
			except ValueError:
				return False

		return True
		
	def OnSetAngle( self, event ):
		treeGeoView = self.layerTree.GetTreeLayer(self.chcTreeSel.GetSelection()).GetGeoTreeView()
		if (treeGeoView.IsLayoutLine()):
			if (self.txtTreeAngle.GetValue() != "" and self.isNumber(self.txtTreeAngle.GetValue())):
				treeGeoView.SetGeographicAxisAngleOnRoot(float(self.txtTreeAngle.GetValue()))
			else:
				treeGeoView.SetGeographicAxisAngleOnRoot(float(self.globalOptimalAngle))
		else:
			wx.MessageBox("Selected tree needs to be set to a 2D format.", "Layout line required.")
			
	def OnWeightSelection( self, event ):
		if (self.chcCrossingWeighted.GetStringSelection() == "Normalised"):
			self.txtWeightInfo.SetValue("Compresses the values for each tree's number of crossings between 0.0 and 1.0 by subtracting the optimal number of crossings from each value and then dividing by the difference of the least-optimal number of crossings and optimal number of crossings. The value 0.0 represents the optimal number of crossings for the tree and 1.0 represents the least-optimal number of crossings. The results from each tree are summed together.")
		elif (self.chcCrossingWeighted.GetStringSelection() == "Unweighted"):
			self.txtWeightInfo.SetValue("The total number of crossings from accross all trees is determined by summing the unweighted number of crossings for each tree.")