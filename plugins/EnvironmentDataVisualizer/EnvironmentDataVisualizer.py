#=======================================================================
# Author: Alexander Keddy based on work by Donovan Parks; Adapting the 'Alpha Diversity Plugin'
#
# Copyright 2013 Alexander Keddy
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

from EnvironmentDataVisualizerLayout import EnvironmentDataVisualizerLayout
import GenGIS

from dataHelper import isNumber
from ScatterPlot import ScatterPlot

import wx
import operator

from AlphaDiversityMeasures import AlphaDiversityMeasures
from scipy.stats import linregress

class EnvironmentDataVisualizer( EnvironmentDataVisualizerLayout ):
	def __init__(self, parent=None):
		EnvironmentDataVisualizerLayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		# initialize member variables
		self.graphicalLines = []
		self.graphicalMarkers = []
		self.selectedRows = []
		self.regressionResults = {}
		
		self.spinLineWidth.SetValue(5)
		self.txtScaleFactor.SetValue(str(1.0))
		self.spinDotSize.SetValue(5)
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0:
			wx.MessageBox("This plugin requires map and location data to be loaded.", "Additional data required.")
			self.Close()
			return
		'''
		#initialize table
		self.table.Freeze()
		self.table.SetSelectionMode(wx.grid.Grid.wxGridSelectRows)
		self.table.EnableScrolling(True, True)
		self.table.EnableDragRowSize(True)
		self.table.SetRowLabelSize(0)
		'''
		# set field (column) labels
		locationSetLayer = GenGIS.layerTree.GetLocationSetLayer(0)
		'''
		self.table.DeleteCols(0, self.table.GetNumberCols());
		self.table.AppendCols(6)
		self.table.SetColLabelValue(0, 'Field')
		self.table.SetColLabelValue(1, 'R-Squared Value')
		self.table.SetColLabelValue(2, 'P-Value')
		self.table.SetColLabelValue(3, 'Standard Error')
		self.table.SetColLabelValue(4, 'Slope')
		self.table.SetColLabelValue(5, 'Intercept')
		'''	
		# set site ids in first column
		locationSetLayer = GenGIS.layerTree.GetLocationSetLayer(0)
		numericFields = locationSetLayer.GetController().GetNumericMetadataFields(True)
		self.cboField.SetColumns = len(numericFields)
		
		for i in xrange(0, len(numericFields)):
			self.cboField.Append(numericFields[i])
		
		self.cboField.SetSelection(0)
		'''
		self.table.AutoSizeColumns()
		self.table.Update()
		self.table.Thaw()
		'''
		# set measure combobox
		self.AlphaDiversityMeasures = AlphaDiversityMeasures()
		'''
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
		'''
		# set colour map combobox
		continuousColourMapNames = GenGIS.colourMapManager.GetColourMapContinuousNames()
		for name in continuousColourMapNames:
			self.cboColourMap.Append(name)
		self.cboColourMap.SetSelection(self.cboColourMap.FindString('Diverging (Blue-White-Red)'))
		
		# Set colour map comboboxes (dots)
		for name in continuousColourMapNames:
			self.cboColourMapDot.Append(name)
		self.cboColourMapDot.SetSelection(self.cboColourMapDot.FindString('Diverging (Blue-White-Red)'))
		'''
		# initialize plot
		self.plot = ScatterPlot(self.pnlPlot)
		self.plot.DrawEmptyPlot()
		
		self.UpdateInfo()
		'''
	def OnOK( self, event ):
		self.Close()
		
	def OnClose(self, event):
		# remove plotted lines
		for id in self.graphicalLines:
			GenGIS.graphics.RemoveLine(id)
			
		# remove plotted dots
		for id in self.graphicalMarkers:
			GenGIS.graphics.RemoveMarker(id)

		GenGIS.viewport.Refresh()
		
		event.Skip()

	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/Description_of_GenGIS_plugins#Environmental_Data_Visualizer' )
	'''	
	def OnPerformSubsampling( self, event ):
		self.cboReplicates.Enable(self.chkSubsample.IsChecked())
		self.txtSeqsToDraw.Enable(self.chkSubsample.IsChecked())
		self.btnMinimumSeqs.Enable(self.chkSubsample.IsChecked())
	'''
	'''	
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
	'''	
	def OnMeasureChanged( self, event ):
		self.UpdateInfo()
	'''	
	def UpdateInfo(self):
		info = self.AlphaDiversityMeasures.Info(self.cboMeasure.GetStringSelection())
		self.txtFullName.SetValue(info[0])
		self.txtFormula.SetValue(info[1])
		self.txtDescription.SetValue(info[2])
		
	def OnSavePlot(self, event):
		self.plot.SavePlot()
	'''	

	def OnCalculate( self, event ):
		wx.BeginBusyCursor()
		# get dictionary indicating the number of times each field value is present
		# in each location
		sampleDict = {}
#		bCountField = (self.cboCountField.GetStringSelection() != '<none>')
		bCountField = False
		activeLocLayers = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()

		for loc in activeLocLayers:
			locName = loc.GetName()
			sampleDict[locName] = {}

			seqLayers = loc.GetAllActiveSequenceLayers()
			'''
			for seqLayer in seqLayers:
				data = seqLayer.GetController().GetData()
				print data
			#	value = data[self.cboCategoryField.GetStringSelection()]
				value = data["AllRecords"]
				count = 1
				if bCountField:
				#	count = data[self.cboCountField.GetStringSelection()]
					count = data["AllRecords"]
				sampleDict[locName][value] = sampleDict[locName].get(value, 0) + float(count)
			'''
		'''
		if self.chkSubsample.IsChecked():
			replicates = int(self.cboReplicates.GetStringSelection())
			seqsToDraw = int(self.txtSeqsToDraw.GetValue())
			indices = self.AlphaDiversityMeasures.CalculateJackknife(self.cboMeasure.GetStringSelection(), sampleDict, self.chkIgnoreOther.IsChecked(), replicates, seqsToDraw)
		else:	'''
		#	indices = self.AlphaDiversityMeasures.Calculate(self.cboMeasure.GetStringSelection(), sampleDict, self.chkIgnoreOther.IsChecked())
		indices = self.AlphaDiversityMeasures.Calculate("Gini-Simpson index", sampleDict, False)
		
		# For every numeric field:
		numericFields = GenGIS.layerTree.GetLocationSetLayer(0).GetController().GetNumericMetadataFields(True)
		
		self.regressionResults = {}
		for field in numericFields:
			x = []
			y = []
			
			# Loop through each location:
			for i in range(0, len(activeLocLayers)):
				locLayer = activeLocLayers[i]
				
				# Get field value for location
				value = float(locLayer.GetController().GetData()[field])
				x.append(value)
				
				# Get alpha diversity for location
				name = locLayer.GetName()
				y.append(indices[name])
			slope, intercept, r_value, p_value, std_err = linregress(x, y)
			
			# calculate residuals
			residuals = []
			for i in xrange(0, len(x)):
				y_hat = slope*x[i] + intercept
				residuals.append(y[i] - y_hat)
				
			self.regressionResults[field] = [r_value**2, p_value, std_err, slope, intercept, x, y, residuals]
		'''
		# place regression results in table
		self.table.Freeze()
		for r in xrange(0, len(numericFields)):
			field = numericFields[r]
			regressionData = self.regressionResults[field]
			for c in xrange(0, 5):
				self.table.SetCellValue(r, c+1, '%.4f' % regressionData[c])
			
		self.table.AutoSizeColumns()
		self.table.Thaw()
		'''
		# update visualization if a row was previously selected
		self.RowSelected()
		
		wx.EndBusyCursor()
	
	def RowSelected(self):
		wx.BeginBusyCursor()
		
	#	if len(self.selectedRows) == 1 and len(self.regressionResults) != 0:
		if len(self.regressionResults) != 0:
			# Get linear regression results for selected field
		#	selectedRow = self.selectedRows[0]
			selectedRow = self.cboField.GetSelection()
			numericFields = GenGIS.layerTree.GetLocationSetLayer(0).GetController().GetNumericMetadataFields(True)
			selectedField = numericFields[selectedRow]
			
			regressionData = self.regressionResults[selectedField]
			r2_value = regressionData[0]
			slope = regressionData[3]
			intercept = regressionData[4]
			x = regressionData[5]
			y = regressionData[6]
			residuals = regressionData[7]
			'''
			# Create linear regression plot
			self.plot.Draw(x, y, selectedField, self.cboMeasure.GetStringSelection(), slope, intercept, r2_value)
			'''
			# Plot data in viewport
			self.ViewportPlot(x, y, residuals)
			self.ViewportDotPlot(x, y, residuals)
			
		wx.EndBusyCursor()
			
	def ViewportPlot(self, x, y, residuals):
		self.colourMap = GenGIS.colourMapManager.GetColourMap(self.cboColourMap.GetStringSelection())
		terrainController = GenGIS.layerTree.GetMapLayer(0).GetController()
		
		# desired plot attributes
		lineWidth = self.spinLineWidth.GetValue()
		if isNumber(self.txtScaleFactor.GetValue()):
			userScaleFactor = float(self.txtScaleFactor.GetValue())
		else:
			wx.MessageBox("Scale factor must be numeric.", "Invalid scale factor")
			return
		'''
		# get data to plot
		plotType = self.cboViewportPlot.GetStringSelection()
		if plotType == 'Alpha Diversity':
			data = y
		elif plotType == 'Environmental Data':
			data = x
		elif plotType == 'Residuals':
			data = residuals
		elif plotType == '<none>':
			for id in self.graphicalLines:
				GenGIS.graphics.RemoveLine(id)
			return
		'''
		#plot type is environmental data
		data=x
		
		# remove previously plotted lines
		for id in self.graphicalLines:
			GenGIS.graphics.RemoveLine(id)
		
		# get scale factor such the the tallest bar is 20% of the map width
		terrainController = GenGIS.layerTree.GetMapLayer(0).GetController()
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

		self.graphicalLines = []
		for i in xrange(0, len(activeLocLayers)):
			locLayer = activeLocLayers[i]
			geoCoord = GenGIS.GeoCoord(locLayer.GetController().GetLongitude(), locLayer.GetController().GetLatitude())
			pos = GenGIS.Point3D()
			terrainController.GeoToGrid(geoCoord, pos)

			colour = self.colourMap.GetInterpolatedColour(data[i], minValue, maxValue)

			endPos = GenGIS.Point3D(pos.x, scaleFactor * abs(data[i]), pos.z)
			line = GenGIS.VisualLine(colour, lineWidth, GenGIS.LINE_STYLE.SOLID, GenGIS.Line3D(pos, endPos))
			lineId = GenGIS.graphics.AddLine(line)
			self.graphicalLines.append(lineId)
			
		GenGIS.viewport.Refresh()
		
	def ViewportDotPlot(self, x, y, residuals):
		self.colourMapDot = GenGIS.colourMapManager.GetColourMap(self.cboColourMapDot.GetStringSelection())
		terrainController = GenGIS.layerTree.GetMapLayer(0).GetController()
		
		# desired plot attributes
		dotWidth = self.spinDotSize.GetValue()

		# get data to plot
		plotType = self.cboViewportDot.GetStringSelection()
		if plotType == 'Alpha Diversity':
			data = y
		elif plotType == 'Environmental Data':
			data = x
		elif plotType == 'Residuals':
			data = residuals
		elif plotType == '<none>':
			for id in self.graphicalMarkers:
				GenGIS.graphics.RemoveMarker(id)
			return
		data=x
		# remove previously plotted lines
		for id in self.graphicalMarkers:
			GenGIS.graphics.RemoveMarker(id)
		
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

		self.graphicalMarkers = []
		for i in xrange(0, len(activeLocLayers)):
			locLayer = activeLocLayers[i]
			geoCoord = GenGIS.GeoCoord(locLayer.GetController().GetLongitude(), locLayer.GetController().GetLatitude())
			pos = GenGIS.Point3D()
			terrainController.GeoToGrid(geoCoord, pos)

			colour = self.colourMapDot.GetInterpolatedColour(data[i], minValue, maxValue)

			marker = GenGIS.VisualMarker(colour, dotWidth, GenGIS.MARKER_SHAPE.CIRCLE, pos)
			marker.SetRenderingStyle(GenGIS.MARKER_RENDERING_STYLE.DECAL)
			marker.SetBorderSize(1)
			marker.SetRenderingOrder(-500)
			markerId = GenGIS.graphics.AddMarker(marker)
			self.graphicalMarkers.append(markerId)
			
		GenGIS.viewport.Refresh()
	'''	
	def OnSelectedRange( self, event ):
		"""Internal update to the selection tracking list"""
		if event.Selecting():
			# adding to the list...
			for index in range( event.GetTopRow(), event.GetBottomRow()+1):
				if index not in self.selectedRows:
					self.selectedRows.append( index )
		else:
			# removal from list
			for index in range( event.GetTopRow(), event.GetBottomRow()+1):
				while index in self.selectedRows:
					self.selectedRows.remove( index )
		self.RowSelected()
		event.Skip()
				
	def OnSelectedCell( self, event ):
		"""Internal update to the selection tracking list"""
		self.selectedRows = [ event.GetRow() ]
		self.RowSelected()
		event.Skip()
	'''	
	def OnPlotTypeChanged( self, event ):
		self.RowSelected()
		
	def OnDotTypeChanged( self, event ):
		self.RowSelected()
	
	def OnColorMapChanged( self, event ):
		self.RowSelected()
	
	def OnDotColorMapChanged( self, event ):
		self.RowSelected()
	
	def OnDotSizeChanged( self, event ):
		self.RowSelected()
		
	def OnLineWidthChanged( self, event ):
		self.RowSelected()
	
	def OnScaleFactorChanged( self, event ):
		self.RowSelected()
		
