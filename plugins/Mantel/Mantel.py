#=======================================================================
# Author: Donovan Parks
#
# Copyright 2012 Kathryn Dunphy, Rob Beiko, Donovan Parks
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

from MantelLayout import MantelLayout
from ScatterPlot import ScatterPlot

import GenGIS
from dataHelper import isNumber

import wx
import math
import os
import dataHelper as dh

from scipy.stats import linregress
from rpy2 import robjects

class Mantel( MantelLayout ):

	def __init__(self, parent=None):
		MantelLayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0:
			wx.MessageBox("This plugin requires map and location data to be loaded.", "Additional data required.")
			self.Close()
			return
		
		self.graphicalElementIds = []
		
		# set location comboboxes
		locationSetLayer = GenGIS.layerTree.GetLocationSetLayer(0)
		numericFields = locationSetLayer.GetController().GetNumericMetadataFields(True)
		for field in numericFields:
			self.cboIndependent.Append(field) 
			self.cboDependent.Append(field)
		self.cboIndependent.Append('Geographic Distance')
		self.cboIndependent.Append('Euclidean Distance')
		self.cboDependent.Append('Geographic Distance')
		self.cboDependent.Append('Euclidean Distance')
		self.cboIndependent.SetSelection(0)
		self.cboDependent.SetSelection(0)
		
		# set sequence comboboxes
		if GenGIS.layerTree.GetNumSequenceLayers() > 0:
			sequenceLayer = GenGIS.layerTree.GetSequenceLayer(0)
			seqMetadataFields = sequenceLayer.GetController().GetMetadataFields()
			for field in sorted(seqMetadataFields):
				self.cboIndependentIDField.Append(field)
			self.cboIndependentIDField.SetSelection(0)
			
			for field in sorted(seqMetadataFields):
				self.cboDependentIDField.Append(field)
			self.cboDependentIDField.SetSelection(0)
			
			seqNumericMetadataFields = sequenceLayer.GetController().GetNumericMetadataFields(True)
			self.cboIndependentCountField.Append('<none>')
			for field in seqNumericMetadataFields:
				self.cboIndependentCountField.Append(field)
			self.cboIndependentCountField.SetSelection(0)
			
			self.cboDependentCountField.Append('<none>')
			for field in seqNumericMetadataFields:
				self.cboDependentCountField.Append(field)
			self.cboDependentCountField.SetSelection(0)
		
		# set colour map combobox
		continuousColourMapNames = GenGIS.colourMapManager.GetColourMapContinuousNames()
		for name in continuousColourMapNames:
			self.cboColourMap.Append(name)
		
		self.cboColourMap.SetSelection(self.cboColourMap.FindString('Diverging (Blue-White-Red)'))
		
		# initialize plot
		self.plot = ScatterPlot(self.pnlPlot)
		self.plot.DrawEmptyPlot()
		
	def OnIndependentClicked( self, event ):
		# Activate appropriate boxes
		if self.radioLocationIndependent.GetValue():
			self.cboIndependent.Enable( True )
			self.cboIndependentIDField.Enable( False )
			self.cboIndependentSubtypeField.Enable( False )
			self.cboIndependentCountField.Enable( False )
			self.btnSelectFileIndependent.Enable( False )
			self.lblFileIndependent.Enable( False )
		elif self.radioSeqIndependent.GetValue():
			self.cboIndependent.Enable( False )
			self.cboIndependentIDField.Enable( True )
			self.cboIndependentSubtypeField.Enable( True )
			self.cboIndependentCountField.Enable( True )
			self.OnIndependentIDFieldChoice(None)
			self.btnSelectFileIndependent.Enable( False )
			self.lblFileIndependent.Enable( False )
		elif self.radioFileIndependent.GetValue():
			self.cboIndependent.Enable( False )
			self.cboIndependentIDField.Enable( False )
			self.cboIndependentSubtypeField.Enable( False )
			self.cboIndependentCountField.Enable( False )
			self.OnIndependentIDFieldChoice(None)
			self.btnSelectFileIndependent.Enable( True )
			self.lblFileIndependent.Enable( True )
			
	def OnDependentClicked( self, event ):
		# Activate appropriate boxes
		if self.radioLocationDependent.GetValue():
			self.cboDependent.Enable( True )
			self.cboDependentIDField.Enable( False )
			self.cboDependentSubtypeField.Enable( False )
			self.cboDependentCountField.Enable( False )
			self.btnSelectFileDependent.Enable( False )
			self.lblFileDependent.Enable( False )
		if self.radioSeqDependent.GetValue():
			self.cboDependent.Enable( False )
			self.cboDependentIDField.Enable( True )
			self.cboDependentSubtypeField.Enable( True )
			self.cboDependentCountField.Enable( True )
			self.OnDependentIDFieldChoice(None)
			self.btnSelectFileDependent.Enable( False )
			self.lblFileDependent.Enable( False )
		elif self.radioFileDependent.GetValue():
			self.cboDependent.Enable( False )
			self.cboDependentIDField.Enable( False )
			self.cboDependentSubtypeField.Enable( False )
			self.cboDependentCountField.Enable( False )
			self.OnDependentIDFieldChoice(None)
			self.btnSelectFileDependent.Enable( True )
			self.lblFileDependent.Enable( True )
	
	def OnSelectFileIndependent(self, event):
		dlg = wx.FileDialog(self, 'Open dissimilarity matrix', '', '', 'Dissimilarity matrix (*.dst)|*.dst|Tab-separated values (*.tsv)|*.tsv|All files (*.*)|*.*', wx.OPEN)
		if dlg.ShowModal() == wx.ID_OK:
			filename = dlg.GetFilename()
			dir = dlg.GetDirectory()
			
			self.lblFileIndependent.SetLabel(filename)
	 
			# read in dissimilarity matrix
			fin = open(os.path.join(dir, filename), 'rU')
			matrix = fin.readlines()
			fin.close()
			
			# parse dissimilarity matrix
			self.independentMatrixLabelIndex = {}
			self.independentMatrix = []
			for i in xrange(1, len(matrix)):
				data = matrix[i].split()
				self.independentMatrixLabelIndex[data[0]] = i-1
				
				row = []
				for j in xrange(1,len(data)):
					dist = float(data[j])
					row.append(dist)
					
				self.independentMatrix.append(row)

		dlg.Destroy()
		
	def OnSelectFileDependent(self, event):
		dlg = wx.FileDialog(self, 'Open dissimilarity matrix', '', '', 'Dissimilarity matrix (*.dst)|*.dst|Tab-separated values (*.tsv)|*.tsv|All files (*.*)|*.*', wx.OPEN)
		if dlg.ShowModal() == wx.ID_OK:
			filename = dlg.GetFilename()
			dir = dlg.GetDirectory()
			
			self.lblFileDependent.SetLabel(filename)
	 
			# read in dissimilarity matrix
			fin = open(os.path.join(dir, filename), 'rU')
			matrix = fin.readlines()
			fin.close()
			
			# parse dissimilarity matrix
			self.dependentMatrixLabelIndex = {}
			self.dependentMatrix = []
			for i in xrange(1, len(matrix)):
				data = matrix[i].split()
				self.dependentMatrixLabelIndex[data[0]] = i-1
				
				row = []
				for j in xrange(1,len(data)):
					dist = float(data[j])
					row.append(dist)
					
				self.dependentMatrix.append(row)

		dlg.Destroy()
	
	def OnIndependentIDFieldChoice( self, event ):
		wx.BeginBusyCursor()
		
		# get data from each sequence for a specific column
		self.cboIndependentSubtypeField.Clear()
		values = set()
		for seqLayer in GenGIS.layerTree.GetSequenceLayers():
			seqController = seqLayer.GetController()
			if seqController.IsActive():
				values.add(seqController.GetData()[self.cboIndependentIDField.GetStringSelection()])
		
		valList = sorted(list(values))
		for v in valList:
			self.cboIndependentSubtypeField.Append(v)
		self.cboIndependentSubtypeField.SetSelection(0)
		
		wx.EndBusyCursor()
			
	def OnDependentIDFieldChoice( self, event ):
		wx.BeginBusyCursor()
		
		# get data from each sequence for a specific column
		self.cboDependentSubtypeField.Clear()
		values = set()
		for seqLayer in GenGIS.layerTree.GetSequenceLayers():
			seqController = seqLayer.GetController()
			if seqController.IsActive():
				values.add(seqController.GetData()[self.cboDependentIDField.GetStringSelection()])
		
		valList = sorted(list(values))
		for v in valList:
			self.cboDependentSubtypeField.Append(v)
		self.cboDependentSubtypeField.SetSelection(0)
		
		wx.EndBusyCursor()
			
	def OnClose(self, event):
		# remove plotted lines
		for id in self.graphicalElementIds:
			GenGIS.graphics.RemoveLine(id)

		GenGIS.viewport.Refresh()
		
		event.Skip()
		
	def OnOK( self, event ):
		self.Close()
		
	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/index.php/Description_of_GenGIS_plugins#Mantel_Test' )
		
	def OnSavePlot(self, event):
		self.plot.SavePlot()
		
	def OnSaveLog(self, event):
		fileTypes = 'Text file (*.txt)|*.txt'
		dlg = wx.FileDialog(self, "Save plot", "", "", fileTypes, wx.SAVE)
		if dlg.ShowModal()==wx.ID_OK:
			filename =	dlg.GetFilename()
			dir = dlg.GetDirectory()
			self.txtOutput.SaveFile(os.path.join(dir,filename))
			
		dlg.Destroy()
		
	def OnClearLog(self, event):
		self.txtOutput.Clear()
		
	### Get geographic distance using the Haversine formula
	### Code adapted from http://www.codecodex.com/wiki/Calculate_distance_between_two_points_on_a_globe#Python
	def GetGeoDistance(self, lat1, long1, lat2, long2):
		conversion = math.pi/180 #0.0174532925
		
		long1 *= conversion
		lat1 *= conversion
		long2 *= conversion
		lat2 *= conversion
		dlong = long2 - long1
		dlat = lat2 - lat1
		R = 6371
		a = math.sin(dlat / 2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlong / 2)**2
		c = 2 * math.asin(min(1, math.sqrt(a)))
		dist = R * c
		return dist
		
	### Get Euclidean distance between a pair of Cartesian coordinates
	def GetEucDistance(self, lat1, long1, lat2, long2):
		return math.sqrt((lat1 - lat2)**2 + (long1 - long2)**2)

	def OnCalculate(self, event):
		wx.BeginBusyCursor()
		
		# Add ade4 library, save robject.r as variable
		robjects.r('library(ade4)')
		r = robjects.r
		
		# Get Data
		self.GetData()
		
		# Perform Mantel test
		self.txtOutput.AppendText('Running Mantel Test...')
		self.txtOutput.AppendText('\n')
		mantelresult = r["mantel.rtest"](self.dependentdistance, self.independentdistance, nrepet = 1000)
		robjects.globalenv['mr'] = mantelresult
		pvalue = r('mr$p')[0]
		
		self.txtOutput.AppendText('\n')
		self.txtOutput.AppendText('p-value: ' + str(pvalue) + '\n\n')
		
		self.plot.Draw(self.independentdistance, self.dependentdistance, self.independent, self.dependent, self.slope, self.intercept, self.r2_value)
		
		# Print results of regression analysis
		self.txtOutput.AppendText('Results of regression analysis:\n')
		self.txtOutput.AppendText('---------------------------------------\n')
		self.txtOutput.AppendText(' Independent (x): ' + self.independent + '\n')
		self.txtOutput.AppendText(' Dependent (y):   ' + self.dependent + '\n')
		self.txtOutput.AppendText('\n')
		self.txtOutput.AppendText('Slope of the regression line: %f' % self.slope + '\n')
		self.txtOutput.AppendText('Intercept of the regression line: %f' % self.intercept + '\n')
		self.txtOutput.AppendText('R-squared correlation coefficient: %f' % self.r2_value + '\n')
		self.txtOutput.AppendText('Two-sided p-value for test of zero slope: %f' % self.p_value + '\n')
		self.txtOutput.AppendText('Standard error of the estimate: %f' % self.std_err + '\n')
		self.txtOutput.AppendText('---------------------------------------\n')
		self.txtOutput.AppendText('\n\n')
		
		self.ViewportPlot()
		
		wx.EndBusyCursor()

	def GetData(self):
		# Add ade4 library, save robject.r as variable
		robjects.r('library(ade4)')
		r = robjects.r
		locationSetController = GenGIS.layerTree.GetLocationSetLayer(0).GetController()
		switchedLocs = dh.LocationSetWeeder(  )
		print switchedLocs
		# Get distances for Independent Variable:
		if self.radioFileIndependent.GetValue():
			self.independent = str(self.lblFileIndependent.GetLabel())
			dissValues = []
			activeLocs = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
			for i in xrange(0, len(activeLocs)):
				locIndexI = self.independentMatrixLabelIndex[activeLocs[i].GetName()]
				
				for j in xrange(0, len(activeLocs)):
					locIndexJ = self.independentMatrixLabelIndex[activeLocs[j].GetName()]
					dissValues.append(self.independentMatrix[locIndexI][locIndexJ])

			data = r['matrix'](dissValues, nrow = len(activeLocs))
			self.independentdistance = r['as.dist'](data)
			self.x = None
		elif self.radioSeqIndependent.GetValue():
			self.independent = self.cboIndependentSubtypeField.GetStringSelection()
			subtypecounts = {}
			
			# For each unique Site ID, loop through the chosen ID field
			for seqLayer in GenGIS.layerTree.GetSequenceLayers():
				seqController = seqLayer.GetController()
				
				if seqController.IsActive():
					if seqController.GetSiteId() not in subtypecounts:
						subtypecounts[seqController.GetSiteId()] = [0,0]
						
					count = 1
					if self.cboIndependentCountField.GetStringSelection() != '<none>':
						count = float(seqController.GetData()[self.cboIndependentCountField.GetStringSelection()])
						
					if seqController.GetData()[self.cboIndependentIDField.GetStringSelection()] == self.cboIndependentSubtypeField.GetStringSelection():
						subtypecounts[seqController.GetSiteId()][1] += count
					subtypecounts[seqController.GetSiteId()][0] += count
					
			# Calculate percentage and store
			percentageSubtype = []
			for loc in GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers():
				data = subtypecounts[loc.GetName()]
				percentageSubtype.append(data[1]/float(data[0]))
			self.independentdistance = r['dist'](percentageSubtype)
			self.x = None
		else:
			self.independent = self.cboIndependent.GetStringSelection()
			# If "Geographic Distance" is chosen for the independent variable, use R to find the
			# geographic distance between the locations, and then create a list of distances
			# between the locations.  Otherwise, just find distances between the chosen variable.
			if self.independent == "Geographic Distance":
				lat = locationSetController.GetNumericMetadata("Latitude", True)
				long = locationSetController.GetNumericMetadata("Longitude", True)
				geoDistMatrix = []
				for i in range(0, len(lat)):
					for j in range(0, len(lat)):
						geoDistMatrix.append(self.GetGeoDistance(lat[i], long[i], lat[j], long[j]))
				data = r['matrix'](geoDistMatrix, nrow = len(lat))
				self.independentdistance = r['as.dist'](data)
				self.x = None
			elif self.independent == "Euclidean Distance":
				lat = locationSetController.GetNumericMetadata("Latitude", True)
				long = locationSetController.GetNumericMetadata("Longitude", True)
				eucDistMatrix = []
				for i in range(0, len(lat)):
					for j in range(0, len(lat)):
						eucDistMatrix.append(self.GetEucDistance(lat[i], long[i], lat[j], long[j]))
				data = r['matrix'](eucDistMatrix, nrow = len(lat))
				self.independentdistance = r['as.dist'](data)
				self.x = None
			else:
				self.x = locationSetController.GetNumericMetadata(self.independent, True)
				self.independentdistance = r['dist'](self.x)
			
		# Get distances for Dependent Variable:
		if self.radioFileDependent.GetValue():
			self.dependent = str(self.lblFileDependent.GetLabel())
			dissValues = []
			activeLocs = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
			for i in xrange(0, len(activeLocs)):
				locIndexI = self.dependentMatrixLabelIndex[activeLocs[i].GetName()]
				
				for j in xrange(0, len(activeLocs)):
					locIndexJ = self.dependentMatrixLabelIndex[activeLocs[j].GetName()]
					dissValues.append(self.dependentMatrix[locIndexI][locIndexJ])

			data = r['matrix'](dissValues, nrow = len(activeLocs))
			self.dependentdistance = r['as.dist'](data)
			self.y = None
		elif self.radioSeqDependent.GetValue():
			self.dependent = self.cboDependentSubtypeField.GetStringSelection()
			subtypecounts = {}
			
			# For each unique Site ID, loop through the chosen ID field
			for seqLayer in GenGIS.layerTree.GetSequenceLayers():
				seqController = seqLayer.GetController()
				
				if seqController.IsActive():
					if seqController.GetSiteId() not in subtypecounts:
						subtypecounts[seqController.GetSiteId()] = [0,0]
						
					count = 1
					if self.cboDependentCountField.GetStringSelection() != '<none>':
						count = float(seqController.GetData()[self.cboDependentCountField.GetStringSelection()])
						
					if seqController.GetData()[self.cboDependentIDField.GetStringSelection()] == self.cboDependentSubtypeField.GetStringSelection():
						subtypecounts[seqController.GetSiteId()][1] += count
					subtypecounts[seqController.GetSiteId()][0] += count
				
			# Calculate percentage and store
			percentageSubtype = []
			for loc in GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers():
				data = subtypecounts[loc.GetName()]
				percentageSubtype.append(data[1]/float(data[0]))
			self.dependentdistance = r['dist'](percentageSubtype)
			self.y = None
		else:
			self.dependent = self.cboDependent.GetStringSelection()
			# If "Geographic Distance" is chosen for the dependent variable, use R to find the
			# geographic distance between the locations, and then create a list of distances
			# between the locations.  Otherwise, just find distances between the chosen variable.
			if self.dependent == "Geographic Distance":
				lat = locationSetController.GetNumericMetadata("Latitude", True)
				long = locationSetController.GetNumericMetadata("Longitude", True)
				geoDistMatrix = []
				for i in range(0, len(lat)):
					for j in range(0, len(lat)):
						geoDistMatrix.append(self.GetGeoDistance(lat[i], long[i], lat[j], long[j]))
				data = r['matrix'](geoDistMatrix, nrow = len(lat))
				self.dependentdistance = r['as.dist'](data)
				self.y = None
			elif self.dependent == "Euclidean Distance":
				lat = locationSetController.GetNumericMetadata("Latitude", True)
				long = locationSetController.GetNumericMetadata("Longitude", True)
				eucDistMatrix = []
				for i in range(0, len(lat)):
					for j in range(0, len(lat)):
						eucDistMatrix.append(self.GetEucDistance(lat[i], long[i], lat[j], long[j]))
				data = r['matrix'](eucDistMatrix, nrow = len(lat))
				self.dependentdistance = r['as.dist'](data)
				self.y = None
			else:
				self.y = locationSetController.GetNumericMetadata(self.dependent, True)
				self.dependentdistance = r['dist'](self.y)
		
		# perform Linear regression
		self.slope, self.intercept, r_value, self.p_value, self.std_err = linregress(self.independentdistance, self.dependentdistance)
		self.r2_value = r_value**2

		# get colour map
		self.colourMap = GenGIS.colourMapManager.GetColourMap(self.cboColourMap.GetStringSelection())
		dh.LocationSetRestore( switchedLocs )
		
	def ViewportPlot(self):
		terrainController = GenGIS.layerTree.GetMapLayer(0).GetController()
		
		# remove previously plotted lines
		for id in self.graphicalElementIds:
			GenGIS.graphics.RemoveLine(id)
			
		self.graphicalElementIds = []
		
		# get data to plot
		plotType = self.cboViewportPlot.GetStringSelection()
		if plotType == 'x data':
			data = self.x
		elif plotType == 'y data':
			data = self.y

		# bail if data can't be plotted
		if data == None:
			GenGIS.viewport.Refresh()
			return
			
		# desired plot attributes
		lineWidth = self.spinLineWidth.GetValue()
		if isNumber(self.txtScaleFactor.GetValue()):
			userScaleFactor = float(self.txtScaleFactor.GetValue())
		else:
			wx.MessageBox("Scale factor must be numeric.", "Invalid scale factor")
			return
		
		# calculate scale factor
		maxValue = max(max(data), abs(min(data)))
		if maxValue != 0:
			scaleFactor = (0.2 * terrainController.GetWidth()) / maxValue
		else:
			scaleFactor = 0

		scaleFactor *= userScaleFactor

		# get min and max values for mapping colours
		if min(data) < 0 and max(data) > 0:
			# have both negative and positive values so map negative values to 
			# first half of map and positive values to the second half of the map
			maxValue = max(max(data), -min(data))
			minValue = -maxValue
		else:
			# all data is either negative or positive so just map over the full colour map
			minValue = min(data)
			maxValue = max(data)
			
		# plot data
		activeLocLayers = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()

		for i in xrange(0, len(activeLocLayers)):
			locLayer = activeLocLayers[i]
			geoCoord = GenGIS.GeoCoord(locLayer.GetController().GetLongitude(), locLayer.GetController().GetLatitude())
			pos = GenGIS.Point3D()
			terrainController.GeoToGrid(geoCoord, pos)

			colour = self.colourMap.GetInterpolatedColour(data[i], minValue, maxValue)
			
			endPos = GenGIS.Point3D(pos.x, scaleFactor * abs(data[i]), pos.z)
			line = GenGIS.VisualLine(colour, lineWidth, GenGIS.LINE_STYLE.SOLID, GenGIS.Line3D(pos, endPos))
			lineId = GenGIS.graphics.AddLine(line)
			self.graphicalElementIds.append(lineId)
			
		GenGIS.viewport.Refresh()
