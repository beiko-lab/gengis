from LinearRegressionLayout import LinearRegressionLayout
from ScatterPlot import ScatterPlot

import GenGIS
from dataHelper import isNumber

import wx

from scipy.stats import linregress

class LinearRegression( LinearRegressionLayout ):
	def __init__(self, parent=None):
		LinearRegressionLayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		self.graphicalElementIds = []
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0:
			wx.MessageBox("This plugin requires map and location data to be loaded.", "Additional data required.")
			self.Close()
			return

		# set regression comboboxes
		locationSetLayer = GenGIS.layerTree.GetLocationSetLayer(0)
		for field in locationSetLayer.GetController().GetNumericMetadataFields():
			self.cboIndependent.Append(field)
			self.cboDependent.Append(field)
			
		self.cboIndependent.SetSelection(0)
		
		if self.cboDependent.GetCount() >= 2:
			self.cboDependent.SetSelection(1)
		else:
			self.cboDependent.SetSelection(0)
			
		# set colour map combobox
		continuousColourMapNames = GenGIS.colourMapManager.GetColourMapContinuousNames()
		for name in continuousColourMapNames:
			self.cboColourMap.Append(name)
			
		self.cboColourMap.SetSelection(self.cboColourMap.FindString('Diverging (Blue-White-Red)'))
					
		# initialize plot
		self.plot = ScatterPlot(self.pnlPlot)
		self.plot.DrawEmptyPlot()
		
	def OnClose(self, event):
		# remove plotted lines
		for id in self.graphicalElementIds:
			GenGIS.graphics.RemoveLine(id)

		GenGIS.viewport.Refresh()
		
		event.Skip()
		
	def OnOK( self, event ):
		self.Close()
		
	def OnSavePlot(self, event):
		self.plot.SavePlot()
		
	def OnSaveLog(self, event):
		fileTypes = 'Text file (*.txt)|*.txt'
		dlg = wx.FileDialog(self, "Save plot", "", "", fileTypes, wx.SAVE)
		if dlg.ShowModal()==wx.ID_OK:
			filename =	dlg.GetFilename()
			dir = dlg.GetDirectory()
			self.txtOutput.SaveFile(dir + '\\' + filename)
			
		dlg.Destroy()
		
	def OnClearLog(self, event):
		self.txtOutput.Clear()
		
	def OnCalculate(self, event):
		wx.BeginBusyCursor()
		
		self.GetData()
		
		self.plot.Draw(self.x, self.y, self.independent, self.dependent, self.slope, self.intercept, self.r2_value)
		
		self.txtOutput.AppendText('Results of regression analysis:\n')
		self.txtOutput.AppendText('---------------------------------------\n')
		self.txtOutput.AppendText(' Independent (x): ' + self.independent + '\n')
		self.txtOutput.AppendText(' Dependent (y):   ' + self.dependent + '\n')
		self.txtOutput.AppendText('\n')
		self.txtOutput.AppendText('Slope of the regression line: %.3f' % self.slope + '\n')
		self.txtOutput.AppendText('Intercept of the regression line: %.3f' % self.intercept + '\n')
		self.txtOutput.AppendText('R-squared correlation coefficient: %.3f' % self.r2_value + '\n')
		self.txtOutput.AppendText('Two-sided p-value for test of zero slope: %.3f' % self.p_value + '\n')
		self.txtOutput.AppendText('Standard error of the estimate: %.3f' % self.std_err + '\n')
		self.txtOutput.AppendText('\n\n')
		
		self.ViewportPlot()
		
		wx.EndBusyCursor()
		
	def GetData(self):
		# get desired data for all visible locations
		self.independent = self.cboIndependent.GetStringSelection()
		self.dependent = self.cboDependent.GetStringSelection()
		
		locationSetController = GenGIS.layerTree.GetLocationSetLayer(0).GetController()
		self.x = locationSetController.GetNumericMetadata(self.independent, True)
		self.y = locationSetController.GetNumericMetadata(self.dependent, True)
		
		# perform linear regression
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
		
		# get data to plot
		plotType = self.cboViewportPlot.GetStringSelection()
		if plotType == 'x data':
			data = self.x
		elif plotType == 'y data':
			data = self.y
		elif plotType == 'Residuals':
			data = self.residuals
		
		# desired plot attributes
		lineWidth = self.spinLineWidth.GetValue()
			
		# get scale factor such the the tallest bar is 20% of the map width
		if isNumber(self.txtScaleFactor.GetValue()):
			userScaleFactor = float(self.txtScaleFactor.GetValue())
		else:
			wx.MessageBox("Scale factor must be numeric.", "Invalid scale factor")
			return
		
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
