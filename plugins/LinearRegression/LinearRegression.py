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

from LinearRegressionLayout import LinearRegressionLayout
from ScatterPlot import ScatterPlot

import GenGIS
from dataHelper import isNumber

import wx
import math

from scipy.stats import linregress

class LinearRegression( LinearRegressionLayout ):

	def __init__(self, parent=None):
		LinearRegressionLayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		self.graphicalElementIds = []
		
		# check that required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0 or GenGIS.layerTree.GetNumSequenceLayers() == 0:
			wx.MessageBox("This plugin requires map and location data to be loaded.", "Additional data required.")
			self.Close()
			return
		
		# set location comboboxes
		locationSetLayer = GenGIS.layerTree.GetLocationSetLayer(0)
		numericFields = locationSetLayer.GetController().GetNumericMetadataFields(True)
		for field in numericFields:
			self.cboIndependent.Append(field) 
			self.cboDependent.Append(field)
		self.cboIndependent.SetSelection(0)
		self.cboDependent.SetSelection(0)
		
		# set sequence comboboxes
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
		if self.cboIndependentVariableDataType.GetStringSelection() == 'Use Location Data':
			self.cboIndependent.Enable( True )
			self.cboIndependentIDField.Enable( False )
			self.cboIndependentSubtypeField.Enable( False )
			self.cboIndependentCountField.Enable( False )
		if self.cboIndependentVariableDataType.GetStringSelection() == 'Use Sequence Data':
			self.cboIndependent.Enable( False )
			self.cboIndependentIDField.Enable( True )
			self.cboIndependentSubtypeField.Enable( True )
			self.cboIndependentCountField.Enable( True )
			self.OnIndependentIDFieldChoice(None)
			
	def OnDependentClicked( self, event ):
		# Activate appropriate boxes
		if self.cboDependentVariableDataType.GetStringSelection() == 'Use Location Data':
			self.cboDependent.Enable( True )
			self.cboDependentIDField.Enable( False )
			self.cboDependentSubtypeField.Enable( False )
			self.cboDependentCountField.Enable( False )
		if self.cboDependentVariableDataType.GetStringSelection() == 'Use Sequence Data':
			self.cboDependent.Enable( False )
			self.cboDependentIDField.Enable( True )
			self.cboDependentSubtypeField.Enable( True )
			self.cboDependentCountField.Enable( True )
			self.OnDependentIDFieldChoice(None)
	
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
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/Description_of_GenGIS_plugins#Linear_Regression' )
		
	def OnSavePlot(self, event):
		self.plot.SavePlot()
		
	def OnSaveLog(self, event):
		fileTypes = 'Text file (*.txt)|*.txt'
		dlg = wx.FileDialog(self, "Save plot", "", "", fileTypes, wx.SAVE)
		if dlg.ShowModal()==wx.ID_OK:
			filename =	dlg.GetFilename()
			dir = dlg.GetDirectory()
			self.txtOutput.SaveFile(dir + '/' + filename)
			
		dlg.Destroy()
		
	def OnClearLog(self, event):
		self.txtOutput.Clear()
		
	def OnCalculate(self, event):
		wx.BeginBusyCursor()
		
		# Get Data
		self.GetData()
		
		# Perform linear regression
		self.txtOutput.AppendText('\n')
		self.txtOutput.AppendText('Running Linear Regression...')
		self.txtOutput.AppendText('\n')
		
		self.plot.Draw(self.x, self.y, self.independent, self.dependent, self.slope, self.intercept, self.r2_value)
		
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
		self.txtOutput.AppendText('\n\n')
		
		self.ViewportPlot()
		
		wx.EndBusyCursor()

	def GetData(self):
		# Add ade4 library, save robject.r as variable
		locationSetController = GenGIS.layerTree.GetLocationSetLayer(0).GetController()
		
		# Get distances for Independent Variable:
		if self.cboIndependentSubtypeField.Enabled == True:
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
			self.x = []
			for loc in GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers():
				data = subtypecounts[loc.GetName()]
				self.x.append(data[1]/float(data[0]))
		else:
			self.independent = self.cboIndependent.GetStringSelection()
			self.x = locationSetController.GetNumericMetadata(self.independent, True)
			
		# Get distances for Dependent Variable:
		if self.cboDependentSubtypeField.Enabled == True:
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
			self.y = []
			for loc in GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers():
				data = subtypecounts[loc.GetName()]
				self.y.append(data[1]/float(data[0]))
		else:
			self.dependent = self.cboDependent.GetStringSelection()
			self.y = locationSetController.GetNumericMetadata(self.dependent, True)
		
		# perform Linear regression
		self.slope, self.intercept, r_value, self.p_value, self.std_err = linregress(self.x, self.y)
		self.r2_value = r_value**2
		
		# calculate residuals
		self.residuals = []
		for i in xrange(0, len(self.x)):
			y_hat = self.slope*self.x[i] + self.intercept
			self.residuals.append(self.y[i] - y_hat)
			
		# get colour map
		self.colourMap = GenGIS.colourMapManager.GetColourMap(self.cboColourMap.GetStringSelection())

	def ViewportPlot(self):
		terrainController = GenGIS.layerTree.GetMapLayer(0).GetController()
		
		# desired plot attributes
		lineWidth = self.spinLineWidth.GetValue()
		if isNumber(self.txtScaleFactor.GetValue()):
			userScaleFactor = float(self.txtScaleFactor.GetValue())
		else:
			wx.MessageBox("Scale factor must be numeric.", "Invalid scale factor")
			return
					
		# get data to plot
		plotType = self.cboViewportPlot.GetStringSelection()
		if plotType == 'x data':
			data = self.x
		elif plotType == 'y data':
			data = self.y
		elif plotType == 'Residuals':
			data = self.residuals
		
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
			
		# remove previously plotted lines
		for id in self.graphicalElementIds:
			GenGIS.graphics.RemoveLine(id)

		# plot data
		activeLocLayers = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()

		self.graphicalElementIds = []
			
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
