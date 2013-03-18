import GenGIS

import wx
import os, sys

from dataHelper import isNumber

from DissimilarityMatrixViewerLayout import DissimilarityMatrixViewerLayout

class DissimilarityMatrixViewer( DissimilarityMatrixViewerLayout ):
	def __init__(self, parent=None):
		DissimilarityMatrixViewerLayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		self.graphicalElementIds = []
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() ==0:
			wx.MessageBox("This plugin requires map and location data to be loaded.", "Additional data required.")
			self.Close()
			return
			
		# get dictionary indicating the geographic coordinates of each location site
		self.locDict = {}
		activeLocLayers = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
		for locLayer in activeLocLayers:
			self.locDict[locLayer.GetController().GetId()] = [locLayer.GetController().GetLongitude(), locLayer.GetController().GetLatitude()]
			
		# populate combo box with all active locations
		for locLayer in activeLocLayers:
			self.cboLocation.Append(locLayer.GetName())
		self.cboLocation.SetSelection(0)
			
		# setup dissimilarity matrix grid
		self.dissMatrix.EnableScrolling(True, True)
		
		# resize frame to fit widgets
		self.Fit()

	def OnOK( self, event ):
		self.Close()

	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/Description_of_GenGIS_plugins#Dissimilarity_Matrix_Viewer' )
		
	def OnClose(self, event):
		# remove plotted lines
		for id in self.graphicalElementIds:
			GenGIS.graphics.RemoveLine(id)

		GenGIS.viewport.Refresh()
		
		event.Skip()
		
	def OnApply( self, event ):
		''' Apply button pressed. '''
		self.Apply()
		
	def OnLoadMatrix( self, event ):
		''' Load dissimilarity matrix from file. '''
		
		dlg = wx.FileDialog(self, 'Open dissimilarity matrix', '', '', 'Dissimilarity matrix (*.dst)|*.dst|Tab-separated values (*.tsv)|*.tsv|All files (*.*)|*.*', wx.OPEN)
		if dlg.ShowModal() == wx.ID_OK:
			filename = dlg.GetFilename()
			dir = dlg.GetDirectory()
			
			self.lblMatrixFilename.SetLabel(filename)
	 
			# read in dissimilarity matrix
			fin = open(os.path.join(dir, filename), 'rU')
			matrix = fin.readlines()
			fin.close()
			
			# parse dissimilarity matrix
			self.index = {}
			self.matrix = []
			self.minDist = sys.float_info.max
			self.maxDist = -sys.float_info.max
			locationLabels = []
			for i in xrange(1, len(matrix)):
				data = matrix[i].split()
				self.index[data[0]] = i-1
				locationLabels.append(data[0])
				
				row = []
				for j in xrange(1,len(data)):
					dist = float(data[j])
					if dist < self.minDist:
						self.minDist = dist
						
					if dist > self.maxDist:
						self.maxDist = dist
					row.append(dist)
					
				self.matrix.append(row)
				
			# setup dissimilarity matrix grid
			self.dissMatrix.Freeze()
			
			self.dissMatrix.DeleteCols(0, self.dissMatrix.GetNumberCols())
			self.dissMatrix.AppendCols(len(self.index)+1)
			self.dissMatrix.DeleteRows(0, self.dissMatrix.GetNumberRows())
			self.dissMatrix.AppendRows(len(self.index))
			
			self.dissMatrix.SetRowLabelSize(0)
			
			self.dissMatrix.SetColLabelValue(0, '')
			for i in xrange(0, len(self.index)):
				self.dissMatrix.SetColLabelValue(i+1, locationLabels[i])
				self.dissMatrix.SetCellValue(i, 0, locationLabels[i])
				
			for i in xrange(0, len(self.index)):
				for j in xrange(0, len(self.index)):
					self.dissMatrix.SetCellValue(i, j+1, str(self.matrix[i][j]))
				
			self.dissMatrix.AutoSizeColumns()
			self.dissMatrix.Thaw()
			
			# set default values for min and max selection criteria
			self.txtLessThanValue.SetValue(str(self.maxDist))
			self.txtGreaterThanValue.SetValue(str(self.minDist))
				
		dlg.Destroy()
		
	def __InterpolateSize(self, dist, minDist, maxDist):
		"""
		Interpolate size of line.
		
		@param dist: distance value
		@return Size of line with a distance value of dist.
		"""
		minSize = self.spinMinWidth.GetValue()
		maxSize = self.spinMaxWidth.GetValue()
		
		offset = (dist - minDist) / (maxDist - minDist)
		return minSize + offset*(maxSize - minSize)
	
	def __InterpolateColour(self, dist, minDist, maxDist):
		"""
		Interpolate size of line.
		
		@param dist: distance value
		@return Colour of line with a distance of dist.
		"""
		startColour = self.colourPickerStart.GetColour()
		endColour = self.colourPickerEnd.GetColour()
		
		offset = (dist - minDist) /  (maxDist - minDist)
		return GenGIS.Colour((startColour.Red() + offset*(endColour.Red()-startColour.Red())) / 255.0, 
													(startColour.Green() + offset*(endColour.Green() - startColour.Green())) / 255.0, 
													(startColour.Blue() + offset*(endColour.Blue() - startColour.Blue())) / 255.0, 
													(startColour.Alpha() + offset*(endColour.Alpha() - startColour.Alpha())) / 255.0)
													
	def OnAddRangeSelection(self, event):
		""" Select dissimilarity matrix cells meeting specified user criteria. """
		
		if str(self.lblMatrixFilename.GetLabelText()) == '<No file loaded>':
			wx.MessageBox("Please load dissimilarity matrix.", "Dissimilarity matrix not found")
			return
		
		# get user criteria
		if isNumber(self.txtLessThanValue.GetValue()):
			lessThanValue = float(self.txtLessThanValue.GetValue())
		else:
			wx.MessageBox("Selection criteria must be numeric.", "Invalid selection criteria")
			return
			
		if isNumber(self.txtGreaterThanValue.GetValue()):
			greaterThanValue = float(self.txtGreaterThanValue.GetValue())
		else:
			wx.MessageBox("Selection criteria must be numeric.", "Invalid selection criteria")
			return
			
		bSelectUpper = self.chkSelectFromUpperTriangleByRange.IsChecked()
		bSelectLower = self.chkSelectFromLowerTriangleByRange.IsChecked()
		
		# select cells meeting user criteria
		for i in xrange(0, len(self.index)):
				for j in xrange(i+1, len(self.index)):
					if bSelectUpper and self.matrix[i][j] <= lessThanValue and self.matrix[i][j] >= greaterThanValue: 
						self.dissMatrix.SelectBlock(i, j+1, i, j+1, True)
					if bSelectLower and self.matrix[j][i] <= lessThanValue and self.matrix[j][i] >= greaterThanValue: 
							self.dissMatrix.SelectBlock(j, i+1, j, i+1, True)
							
	def OnAddLocationSelection(self, event):
		# get distances to specified location
		dist = {}
		index = self.index[self.cboLocation.GetStringSelection()]
		if self.radioSelectFromUpperTriangleByLoc.GetValue():
			# scan across upper row
			for i in xrange(index+1, len(self.index)):
				dist[self.dissMatrix.GetColLabelValue(i+1)] = self.matrix[index][i]
			# scan down upper column
			for i in xrange(0, index):
				dist[self.dissMatrix.GetCellValue(i, 0)] = self.matrix[i][index]
		if self.radioSelectFromLowerTriangleByLoc.GetValue():
			# scan across lower row
			for i in xrange(0, index):
				dist[self.dissMatrix.GetColLabelValue(i+1)] = self.matrix[index][i]
			# scan down lower column
			for i in xrange(index+1, len(self.index)):
				dist[self.dissMatrix.GetCellValue(i, 0)] = self.matrix[i][index]
		
		# sort dictionary by value
		items = [(v, k) for k, v in dist.items()]
		items.sort()
		
		if self.radioLeast.GetValue():
			items.reverse()
		
		# select specified number of cells
		for i in xrange(0, self.spinNumberToSelect.GetValue()):
			index2 = self.index[items[i][1]]
			if self.radioSelectFromUpperTriangleByLoc.GetValue():
				if index > index2:
					self.dissMatrix.SelectBlock(index2, index+1, index2, index+1, True)
				else:
					self.dissMatrix.SelectBlock(index, index2+1, index, index2+1, True)
			else:
				if index > index2:
					self.dissMatrix.SelectBlock(index, index2+1, index, index2+1, True)
				else:
					self.dissMatrix.SelectBlock(index2, index+1, index2, index+1, True)
					
	def OnClearSelections(self, event):
		self.dissMatrix.ClearSelection()
		
		for id in self.graphicalElementIds:
			GenGIS.graphics.RemoveLine(id)

		self.graphicalElementIds = []
		
		GenGIS.viewport.Refresh()
							
	def DrawLines(self, lines):
		"""
		Draw lines from between locations specified in dissimilarity matrix. For example,
		lines=[['A', 'B']] will draw a line from A to B. The dissimilarity for this line 
		will be taken from row A and column B. Multiple lines can be draw using 
		lines = [['A','B'],['C','D']]. 
		
		@param lines: specifies lines to draw as a list of lists
		"""
		
		terrainController = GenGIS.layerTree.GetMapLayer(0).GetController()
		
		# remove previously plotted lines
		for id in self.graphicalElementIds:
			GenGIS.graphics.RemoveLine(id)
			
		minDist = self.minDist
		maxDist = self.maxDist
		if self.chkRestrictColourRange.IsChecked():
			minDist = sys.float_info.max
			maxDist = -sys.float_info.max
			for line in lines:
				r = self.index[line[0]]
				c = self.index[line[1]]
				dist = self.matrix[r][c]
				
				if dist < minDist:
					minDist = dist
				if dist > maxDist:
					maxDist = dist

		# plot lines
		self.graphicalElementIds = []
		for line in lines:
			r = self.index[line[0]]
			c = self.index[line[1]]
			
			dist = self.matrix[r][c]
			size = self.__InterpolateSize(dist, minDist, maxDist)
			colour = self.__InterpolateColour(dist, minDist, maxDist)
	
			startPt = GenGIS.Point3D()
			terrainController.GeoToGrid(GenGIS.GeoCoord(self.locDict[line[0]][0], self.locDict[line[0]][1]), startPt)
		
			endPt = GenGIS.Point3D()
			terrainController.GeoToGrid(GenGIS.GeoCoord(self.locDict[line[1]][0], self.locDict[line[1]][1]), endPt)
		
			line = GenGIS.VisualLine(colour, size, GenGIS.LINE_STYLE.SOLID, GenGIS.Line3D(startPt, endPt))
			lineId = GenGIS.graphics.AddLine(line)
			self.graphicalElementIds.append(lineId)
		
	def OnPlot(self, event):
		''' Generate visualization using user specified properties. '''
		
		# Draw lines between locations corresponding to selected cells in the dissimilarity matrix grid
		# Note: GetSelectedCells() is not used since this does not track
		#       cells selected programmatically or via a complete row or column
		#       selections
		lines = []
		for i in xrange(0, len(self.matrix)):
			for j in xrange(1, len(self.matrix)+1):
				if self.dissMatrix.IsInSelection(i, j):
					line = []
					line.append(self.dissMatrix.GetCellValue(i, 0))
					line.append(self.dissMatrix.GetColLabelValue(j))
					lines.append(line)
		
		self.DrawLines(lines)

		GenGIS.viewport.Refresh()