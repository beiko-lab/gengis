#=======================================================================
# Author: Donovan Parks
#
# Copyright 2012 Morgan Langille and Rob Beiko
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


from CanonicalCorrelationAnalysisLayout import CanonicalCorrelationAnalysisLayout
import GenGIS
import wx
import rpy2.robjects as robjects
import dataHelper as dh

class CanonicalCorrelationAnalysis( CanonicalCorrelationAnalysisLayout ):
	def __init__(self, parent = None):
		layout = CanonicalCorrelationAnalysisLayout.__init__( self, parent )
		
		self.chcCountField.Append("N/A")
		for numDataField in GenGIS.layerTree.GetSequenceLayer(0).GetController().GetNumericMetadataFields(True):
			self.chcCountField.Append(numDataField)
		self.chcCountField.SetSelection(0)
		try:
			self.chcCountField.SetStringSelection("Count")
		except:
			None
			
		for field in GenGIS.layerTree.GetSequenceLayer(0).GetController().GetMetadataFields():
			self.chcFieldToUse.Append(field)
		self.chcFieldToUse.SetSelection(0)
		
		for numDataField in GenGIS.layerTree.GetLocationSetLayer(0).GetController().GetNumericMetadataFields(True):
			if numDataField == "Latitude" or numDataField == "Longitude":
				self.lstNotIncluded.Append(numDataField)
			else:
				self.lstIncluded.Append(numDataField)
		self.matrix = None
	
	def RunCCA(self, enviroVarsToUse, countField, categoryField):
		switchedLocations = dh.LocationSetWeeder()
		'''
		Runs the CCA R plugin. Either does (1) a Grid Search (2) a Grid Search plus CCA (3) Matrix Correlation, depending on what was selected under "Operation to Perform"
		'''
		# Add CCA library, save robject.r as variable
		robjects.r('library(CCA)')
		r = robjects.r
		
		# Identify all active location layers
		locations = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
		# Identify all active sequence layers
		sequences = GenGIS.layerTree.GetSequenceLayers()
		
		# Dictionary of sequence counts by location for each of the requested labels
		locationMatrix = dict()
		sequenceMatrix = dict()
		
		# All names in category to use (e.g. Phylum -> Fermicute, Bactroidete)
		self.setCategoryToUse = sorted(set((seq.GetController().GetData()[categoryField] for seq in sequences)))
		
		self.enviroVarsToUse = enviroVarsToUse
		
		# Iterate through the sequence data and build the dictionary of frequencies
		for loc in locations:
			locData = loc.GetController().GetData()
			locName = locData["Site ID"]
			locationMatrix[locName] = {}
			sequenceMatrix[locName] = {}
			for category in self.setCategoryToUse:
				sequenceMatrix[locName][category] = 0.0
			for numDataField in GenGIS.layerTree.GetLocationSetLayer(0).GetController().GetNumericMetadataFields(True):
				if numDataField in self.enviroVarsToUse:
					locationMatrix[locName][numDataField] = (locData[numDataField])
		# print locationMatrix
		
		# Iterate through the sequence data and build the dictionary of frequencies
		for seq in sequences:
			seqData = seq.GetController().GetData()
			siteId = seq.GetController().GetSiteId()
			if str(siteId) in sequenceMatrix.keys():
				if countField == "N/A":
						sequenceMatrix[str(siteId)][seqData[categoryField]] += 1
				else:
						sequenceMatrix[str(siteId)][seqData[categoryField]] += float(seqData[countField])
		# print sequenceMatrix
		
		numLocs = len(locations)
		self.numEnviroVars = len(self.enviroVarsToUse)
		self.numCategoryVars = len(self.setCategoryToUse)
		
		# Now build the R matrix
		locationList = list()
		sequenceList = list()
		for i in xrange(0,numLocs):
			locName = locations[i].GetController().GetData()["Site ID"]
			for j in xrange(0,self.numEnviroVars):
				locationList.append(locationMatrix[locName][self.enviroVarsToUse[j]])
			for j in xrange(0, self.numCategoryVars):
				sequenceList.append(sequenceMatrix[locName][self.setCategoryToUse[j]])
		
		rLocMat = r["c"](robjects.FloatVector(locationList))
		rSeqMat = r["c"](robjects.FloatVector(sequenceList))
		rLocationMatrix = r.matrix(rLocMat, nrow = numLocs, byrow = True)
		rSequenceMatrix = r.matrix(rSeqMat, nrow = numLocs, byrow = True)
		
		robjects.globalenv["rLocationMatrix"] = rLocationMatrix
		robjects.globalenv["rSequenceMatrix"] = rSequenceMatrix
		
		# Matrix Correlation
		if self.operationMatrixCorrelation.GetValue():
			correlation = r("matcor(rLocationMatrix, rSequenceMatrix)")
			r["img.matcor"](correlation, type = 2)
		
		# Grid Search (Used for Regularised CCA)
		if self.operationGridSearch.GetValue():
			try:
				robjects.globalenv["minLoc"]  = float(self.txtMinLoc.GetValue())
				robjects.globalenv["maxLoc"]  = float(self.txtMaxLoc.GetValue())
				robjects.globalenv["cutsLoc"] = float(self.txtCutsLoc.GetValue())
				robjects.globalenv["minSeq"]  = float(self.txtMinSeq.GetValue())
				robjects.globalenv["maxSeq"]  = float(self.txtMaxSeq.GetValue())
				robjects.globalenv["cutsSeq"] = float(self.txtCutsSeq.GetValue())
			except:
				wx.MessageBox("Text field values must be a number")
				return
			robjects.globalenv["gridSearch"] = r("estim.regul(rLocationMatrix, rSequenceMatrix, grid1 = seq(minLoc, maxLoc, l=cutsLoc), grid2 = seq(minSeq, maxSeq, l=cutsSeq))")
			lambdaOne = r("gridSearch$lambda1")
			lambdaTwo = r("gridSearch$lambda2")
		
		# Regularised Canonical Correlation Analysis
		if self.operationCCA.GetValue():
			self.matrix = None
			try:
				lambdaOne = float(self.manualL1.GetValue())
				lambdaTwo = float(self.manualL2.GetValue())
			except:
				wx.MessageBox("Text field values must be a number")
				return
			ccaResult = r["rcc"](rLocationMatrix, rSequenceMatrix, lambdaOne, lambdaTwo)
			robjects.globalenv["enviroVarsToUse"] = self.enviroVarsToUse
			robjects.globalenv["catsToUse"] = self.setCategoryToUse
			robjects.globalenv["ccaResult"] = ccaResult
			r("plt.cc(ccaResult, d1=1, d2=2, var.label = TRUE, Xnames = enviroVarsToUse, Ynames = catsToUse)")
			# Build the matrix of dot-producted CCA results
			self.xScores = r("ccaResult$scores$corr.X.xscores")
			self.yScores = r("ccaResult$scores$corr.Y.yscores")
			self.operationPEN.Enable()
		
		# PEN
		if self.operationPEN.GetValue():
			self.PEN()
		dh.LocationSetRestore(switchedLocations)
	
	# Build correlation matrix for visualisation
	def BuildCorrelationMatrix( self ):
		r = robjects.r
		self.matrix = []
		numberOfDimensions = self.dimensions.GetValue()
		self.minabscorrelation = float("inf")
		self.maxabscorrelation = 0.0
		# Fill enviro var rows first
		for row in range(1,self.numEnviroVars+1):
			temprow = []
			# For every environ var column in this row...
			for column in range(1,self.numEnviroVars+1):
				# Start cell at 0, for every coeff, incease cell
				correlation = r('ccaResult$scores$corr.X.xscores['+str(row)+',1:'+str(numberOfDimensions)+'] %*% ccaResult$scores$corr.X.xscores['+str(column)+',1:'+str(numberOfDimensions)+']')[0]
				temprow.append(correlation)
				if abs(correlation) > self.maxabscorrelation: self.maxabscorrelation = abs(correlation)
				if abs(correlation) < self.minabscorrelation: self.minabscorrelation = abs(correlation)
			# For every category var column in this row...
			for column in range(1,self.numCategoryVars+1):
				# Start cell at 0, for every coeff, incease cell
				correlation = r('ccaResult$scores$corr.X.xscores['+str(row)+',1:'+str(numberOfDimensions)+'] %*% ccaResult$scores$corr.Y.yscores['+str(column)+',1:'+str(numberOfDimensions)+']')[0]
				temprow.append(correlation)
				if abs(correlation) > self.maxabscorrelation: self.maxabscorrelation = abs(correlation)
				if abs(correlation) < self.minabscorrelation: self.minabscorrelation = abs(correlation)
			self.matrix.append(temprow)
		# Fill category var rows
		for row in range(1,self.numCategoryVars+1):
			temprow = []
			# For every environ var column in this row...
			for column in range(1,self.numEnviroVars+1):
				# Start cell at 0, for every coeff, incease cell
				correlation = r('ccaResult$scores$corr.Y.yscores['+str(row)+',1:'+str(numberOfDimensions)+'] %*% ccaResult$scores$corr.X.xscores['+str(column)+',1:'+str(numberOfDimensions)+']')[0]
				temprow.append(correlation)
				if abs(correlation) > self.maxabscorrelation: self.maxabscorrelation = abs(correlation)
				if abs(correlation) < self.minabscorrelation: self.minabscorrelation = abs(correlation)
			# For every category var column in this row...
			for column in range(1,self.numCategoryVars+1):
				# Start cell at 0, for every coeff, incease cell
				correlation = r('ccaResult$scores$corr.Y.yscores['+str(row)+',1:'+str(numberOfDimensions)+'] %*% ccaResult$scores$corr.Y.yscores['+str(column)+',1:'+str(numberOfDimensions)+']')[0]
				temprow.append(correlation)
				if abs(correlation) > self.maxabscorrelation: self.maxabscorrelation = abs(correlation)
				if abs(correlation) < self.minabscorrelation: self.minabscorrelation = abs(correlation)
			self.matrix.append(temprow)
	
	def PEN(self):
		'''
		Uses correlation matrix to construct PEN.
		'''
		if self.matrix == None: self.BuildCorrelationMatrix()
		thresholdValue = float("0."+str(self.threshold.GetValue()))
		# Make sure threshold value is not too high
		if thresholdValue > self.maxabscorrelation:
			dialog = wx.MessageDialog(self, 'Threshold of '+str(thresholdValue)+' will exclude all data. Please use a threshold of '+str(self.maxabscorrelation)+' or lower.', 'Warning', wx.OK)
			dialog.ShowModal()
			dialog.Destroy()
			return
		robjects.r('library(igraph)')
		r = robjects.r
		r("PEN <- graph.empty(n="+str(self.numEnviroVars+self.numCategoryVars)+", directed=FALSE)")
		# Get vertex ids as vector (the ids seem to start at 1 on Mac and 0 on Windows)
		vertices = r('V(PEN)')
		# Assign vertex labels and colours
		r('V(PEN)$label <- c("'+'","'.join(self.enviroVarsToUse+self.setCategoryToUse)+'")')
		r('V(PEN)$shape <- "rectangle"')
		r('V(PEN)$size <- c('+','.join([str(len(label)*7.25) for label in self.enviroVarsToUse+self.setCategoryToUse])+')')
		r('V(PEN)['+str(vertices[0])+':'+str(vertices[self.numEnviroVars-1])+']$color <- "cyan"')
		r('V(PEN)['+str(vertices[self.numEnviroVars])+':'+str(vertices[self.numEnviroVars+self.numCategoryVars-1])+']$color <- "grey"')
		disconnectedvertices = set(vertices)
		seenedges = set()
		# Add edges
		for row in range(self.numEnviroVars+self.numCategoryVars):
			for column in range(self.numEnviroVars+self.numCategoryVars):
				# Avoid self-loops
				if row == column: continue
				if abs(self.matrix[row][column]) >= thresholdValue:
					if (column,row) not in seenedges:
						# Avoid reciprocal edges
						seenedges.add((row,column))
						r("PEN <- add.edges(PEN, c("+str(vertices[row])+","+str(vertices[column])+"))")
						disconnectedvertices.discard(vertices[row])
						disconnectedvertices.discard(vertices[column])
						# Assign colour
						if self.matrix[row][column] > 0:    r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$color <- "green"')
						elif self.matrix[row][column] == 0: r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$color <- "yellow"')
						else:                               r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$color <- "red"')
						# Assign line type (solid or dashed)
						if (row < self.numEnviroVars and column < self.numEnviroVars) or (row >= self.numEnviroVars and column >= self.numEnviroVars): r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$lty <- 2')
						else:                                                                                                                          r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$lty <- 1')
		# Remove unconnected vertices
		r('PEN <- delete.vertices(PEN,c('+','.join([str(v) for v in disconnectedvertices])+'))')
		# calculate scaling
		coords = r("PENlayout <- layout.kamada.kawai(PEN)")
		coords = list(coords)
		coords = zip(coords[0:len(coords)/2],coords[len(coords)/2:])
		xlim = (min(i[0] for i in coords), max(i[0] for i in coords))
		ylim = (min(i[1] for i in coords), max(i[1] for i in coords))
		# plot it
		r("plot(PEN,layout=PENlayout,rescale=FALSE,xlim=c("+str(xlim[0]-(xlim[0]/50.0))+","+str(xlim[1]+(xlim[1]/50.0))+"),ylim=c("+str(ylim[0]-(ylim[0]/50.0))+","+str(ylim[1]+(ylim[1]/50.0))+"))")
	
	def OnAdd( self, event ):
		'''
		Moves environmental variables from "Not Included" to "Included"
		'''
		# Move them
		selections = sorted(self.lstNotIncluded.GetSelections())
		for index in selections:                      self.lstIncluded.Append(self.lstNotIncluded.GetString(index))
		for selectionindex in range(len(selections)): self.lstNotIncluded.Delete(selections[selectionindex]-selectionindex)
		self.operationPEN.Disable()
		self.matrix = None
		if self.operationPEN.GetValue():
			self.operationCCA.SetValue(True)
			self.OnChooseCCA()
		# If there's something in the "Included" list, enable "Operations to Perform" and the "Go" button
		if self.lstIncluded.GetCount() > 0:
			self.operationMatrixCorrelation.Enable()
			self.operationGridSearch.Enable()
			self.operationCCA.Enable()
			self.btnGo.Enable()
		
	def OnRemove( self, event ):
		'''
		Moves environmental variables from "Included" to "Not Included"
		'''
		# Move them
		selections = sorted(self.lstIncluded.GetSelections())
		for index in selections:                      self.lstNotIncluded.Append(self.lstIncluded.GetString(index))
		for selectionindex in range(len(selections)): self.lstIncluded.Delete(selections[selectionindex]-selectionindex)
		self.operationPEN.Disable()
		self.matrix = None
		if self.operationPEN.GetValue():
			self.operationCCA.SetValue(True)
			self.OnChooseCCA()
		# If there's nothing left in the "Included" list, disable the "Operations to Perform" selection and the "Go" buttons
		if self.lstIncluded.GetCount() == 0:
			self.operationMatrixCorrelation.Disable()
			self.operationGridSearch.Disable()
			self.operationCCA.Disable()
			self.btnGo.Disable()
	
	def OnGo( self, event ):
		'''
		Runs the "Operation to Perform".
		'''
		wx.BeginBusyCursor()
		# RunCCA takes (1) list of "Included" environmental vars, (2) "Count Field", (3) "Field to use"
		self.RunCCA([self.lstIncluded.GetString(i) for i in range(self.lstIncluded.GetCount())], self.chcCountField.GetStringSelection(), self.chcFieldToUse.GetStringSelection())
		wx.EndBusyCursor()
	
	def OnIncludedClick(self, event):
		'''
		If the "Included" list box is clicked, enable (or disable) the Remove button if an item is selected (or if none is selected).
		'''
		if len(self.lstIncluded.GetSelections()) > 0: self.btnRemove.Enable()
		else:                                         self.btnRemove.Disable()
	
	def OnNotIncludedClick(self, event):
		'''
		If the "Not Included" list box is clicked, enable (or disable) the Add button if an item is selected (or if none is selected).
		'''
		if len(self.lstNotIncluded.GetSelections()) > 0: self.btnAdd.Enable()
		else:                                            self.btnAdd.Disable()
	
	def OnChooseCCA(self,event):
		'''
		If the CCA operation is chosen, enable relevant gui controls
		'''
		self.txtMinLoc.Disable()
		self.txtMinLoc.SetForegroundColour((204,204,204))
		self.txtMaxLoc.Disable()
		self.txtMaxLoc.SetForegroundColour((204,204,204))
		self.txtCutsLoc.Disable()
		self.txtCutsLoc.SetForegroundColour((204,204,204))
		self.txtMinSeq.Disable()
		self.txtMinSeq.SetForegroundColour((204,204,204))
		self.txtMaxSeq.Disable()
		self.txtMaxSeq.SetForegroundColour((204,204,204))
		self.txtCutsSeq.Disable()
		self.txtCutsSeq.SetForegroundColour((204,204,204))
		self.numOfBlocks.Disable()
		self.maximumSearch.Disable()
		self.minimumSearch.Disable()
		self.manualL1.Enable()
		self.manualL1.SetForegroundColour("black")
		self.manualL2.Enable()
		self.manualL2.SetForegroundColour("black")
		self.manualL1.SetFocus()
		self.threshold.Disable()
		self.dimensions.Disable()
		self.PENTxt.Disable()
		self.thresholdZero.Disable()
		self.RCCATxt.Enable()
		self.btnExport.Disable()
	
	def OnChooseMatrixCorrelation(self,event):
		'''
		If the Matrix Correlation is chosen, disable all lambda GUI controls.
		'''
		self.txtMinLoc.Disable()
		self.txtMinLoc.SetForegroundColour((204,204,204))
		self.txtMaxLoc.Disable()
		self.txtMaxLoc.SetForegroundColour((204,204,204))
		self.txtCutsLoc.Disable()
		self.txtCutsLoc.SetForegroundColour((204,204,204))
		self.txtMinSeq.Disable()
		self.txtMinSeq.SetForegroundColour((204,204,204))
		self.txtMaxSeq.Disable()
		self.txtMaxSeq.SetForegroundColour((204,204,204))
		self.txtCutsSeq.Disable()
		self.txtCutsSeq.SetForegroundColour((204,204,204))
		self.numOfBlocks.Disable()
		self.maximumSearch.Disable()
		self.minimumSearch.Disable()
		self.manualL1.Disable()
		self.manualL1.SetForegroundColour((204,204,204))
		self.manualL2.Disable()
		self.manualL2.SetForegroundColour((204,204,204))
		self.threshold.Disable()
		self.dimensions.Disable()
		self.PENTxt.Disable()
		self.thresholdZero.Disable()
		self.RCCATxt.Disable()
		self.btnExport.Disable()
	
	def OnChooseGridSearch(self,event):
		'''
		If Grid Search is chosen, enable those GUI controls and disable the Manual controls.
		'''
		self.txtMinLoc.Enable()
		self.txtMinLoc.SetForegroundColour("black")
		self.txtMaxLoc.Enable()
		self.txtMaxLoc.SetForegroundColour("black")
		self.txtCutsLoc.Enable()
		self.txtCutsLoc.SetForegroundColour("black")
		self.txtMinSeq.Enable()
		self.txtMinSeq.SetForegroundColour("black")
		self.txtMaxSeq.Enable()
		self.txtMaxSeq.SetForegroundColour("black")
		self.txtCutsSeq.Enable()
		self.txtCutsSeq.SetForegroundColour("black")
		self.numOfBlocks.Enable()
		self.maximumSearch.Enable()
		self.minimumSearch.Enable()
		self.manualL1.Disable()
		self.manualL1.SetForegroundColour((204,204,204))
		self.manualL2.Disable()
		self.manualL2.SetForegroundColour((204,204,204))
		self.txtMinLoc.SetFocus()
		self.threshold.Disable()
		self.dimensions.Disable()
		self.PENTxt.Disable()
		self.thresholdZero.Disable()
		self.RCCATxt.Disable()
		self.btnExport.Disable()
	
	def OnChoosePEN(self,event):
		self.txtMinLoc.Disable()
		self.txtMinLoc.SetForegroundColour((204,204,204))
		self.txtMaxLoc.Disable()
		self.txtMaxLoc.SetForegroundColour((204,204,204))
		self.txtCutsLoc.Disable()
		self.txtCutsLoc.SetForegroundColour((204,204,204))
		self.txtMinSeq.Disable()
		self.txtMinSeq.SetForegroundColour((204,204,204))
		self.txtMaxSeq.Disable()
		self.txtMaxSeq.SetForegroundColour((204,204,204))
		self.txtCutsSeq.Disable()
		self.txtCutsSeq.SetForegroundColour((204,204,204))
		self.numOfBlocks.Disable()
		self.maximumSearch.Disable()
		self.minimumSearch.Disable()
		self.manualL1.Disable()
		self.manualL1.SetForegroundColour((204,204,204))
		self.manualL2.Disable()
		self.manualL2.SetForegroundColour((204,204,204))
		self.threshold.Enable()
		self.dimensions.Enable()
		self.PENTxt.Enable()
		self.thresholdZero.Enable()
		self.threshold.SetFocus()
		self.RCCATxt.Disable()
		self.btnExport.Enable()
	
	def OnChooseSequenceField(self,event):
		'''
		When the sequence "Field to use" is changed, update the "Dimensions" spin control with a new range of values.
		'''
		sequences = GenGIS.layerTree.GetSequenceLayers()
		self.dimensions.SetRange(1,min(self.lstIncluded.GetCount(),len(set((seq.GetController().GetData()[self.chcFieldToUse.GetStringSelection()] for seq in sequences)))))
	
	def OnExportPEN(self,event):
		'''
		Exports PEN network in xgmml cytoscape format.
		'''
		if self.matrix == None: self.BuildCorrelationMatrix()
		# Make sure threshold is not too high
		thresholdValue = float("0."+str(self.threshold.GetValue()))
		if thresholdValue > self.maxabscorrelation:
			dialog = wx.MessageDialog(self, 'Threshold of '+str(thresholdValue)+' will exclude all data. Please use a threshold of '+str(self.maxabscorrelation)+' or lower.', 'Warning', wx.OK)
			dialog.ShowModal()
			dialog.Destroy()
			return
		# Where to save file?
		dialog = wx.FileDialog( None, style = wx.SAVE | wx.OVERWRITE_PROMPT )
		if dialog.ShowModal() == wx.ID_OK:
		   cytoscapefile = open(dialog.GetPath(),'w')
		else:
			dialog.Destroy()
			return
		dialog.Destroy()
		#r = robjects.r
		#r('V(PEN)[0:'+str(self.numEnviroVars-1)+']$color <- "cyan"')
		#r('V(PEN)['+str(self.numEnviroVars)+':'+str(self.numEnviroVars+self.numCategoryVars-1)+']$color <- "grey"')
		cytoscapefile.write('<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n<graph label="PENnetwork" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:cy="http://www.cytoscape.org" xmlns="http://www.cs.rpi.edu/XGMML"  directed="0">\n')
		seenvertices = set()
		seenedges = set()
		# Add edges
		for row,rowlabel in zip(range(self.numEnviroVars+self.numCategoryVars),self.enviroVarsToUse+self.setCategoryToUse):
			for column,columnlabel in zip(range(self.numEnviroVars+self.numCategoryVars),self.enviroVarsToUse+self.setCategoryToUse):
				# Avoid self-loops
				if row == column: continue
				if abs(self.matrix[row][column]) >= thresholdValue:
					if row not in seenvertices:
						seenvertices.add(row)
						if row < self.numEnviroVars:
							cytoscapefile.write('<node id="'+str(row+1)+'" label="'+rowlabel+'" ><graphics type="ELLIPSE" fill="#00FFFF" width="1" outline="#666666" cy:borderLineType="solid"/></node>\n')
						else:
							cytoscapefile.write('<node id="'+str(row+1)+'" label="'+rowlabel+'" ><graphics type="ELLIPSE" fill="#999999" width="1" outline="#666666" cy:borderLineType="solid"/></node>\n')
					if column not in seenvertices:
						seenvertices.add(column)
						if column < self.numEnviroVars:
							cytoscapefile.write('<node id="'+str(column+1)+'" label="'+columnlabel+'" ><graphics type="ELLIPSE" fill="#00FFFF" width="1" outline="#666666" cy:borderLineType="solid"/></node>\n')
						else:
							cytoscapefile.write('<node id="'+str(column+1)+'" label="'+columnlabel+'" ><graphics type="ELLIPSE" fill="#999999" width="1" outline="#666666" cy:borderLineType="solid"/></node>\n')
					if (column,row) not in seenedges:
						# Avoid reciprocal edges
						seenedges.add((row,column))
						# Assign colour
						if self.matrix[row][column] > 0:
							# Assign line type (solid or dashed)
							if (row < self.numEnviroVars and column < self.numEnviroVars) or (row >= self.numEnviroVars and column >= self.numEnviroVars):
								cytoscapefile.write('<edge label="'+str(row+1)+' (edge) '+str(column+1)+'" source="'+str(row+1)+'" target="'+str(column+1)+'"><graphics cy:edgeLineType="DASH" width="2" fill="#00FF00"/><att type="real" name="correlation" value="'+str(self.matrix[row][column])+'"/></edge>\n')
								#r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$lty <- 2')
							else:
								cytoscapefile.write('<edge label="'+str(row+1)+' (edge) '+str(column+1)+'" source="'+str(row+1)+'" target="'+str(column+1)+'"><graphics cy:edgeLineType="SOLID" width="2" fill="#00FF00"/><att type="real" name="correlation" value="'+str(self.matrix[row][column])+'"/></edge>\n')
								#r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$lty <- 1')
							#r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$color <- "red"')
						elif self.matrix[row][column] == 0:
							# Assign line type (solid or dashed)
							if (row < self.numEnviroVars and column < self.numEnviroVars) or (row >= self.numEnviroVars and column >= self.numEnviroVars):
								#r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$lty <- 2')
								cytoscapefile.write('<edge label="'+str(row+1)+' (edge) '+str(column+1)+'" source="'+str(row+1)+'" target="'+str(column+1)+'"><graphics cy:edgeLineType="DASH" width="2" fill="#FFFF00"/><att type="real" name="correlation" value="'+str(self.matrix[row][column])+'"/></edge>\n')
							else:
								#r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$lty <- 1')
								cytoscapefile.write('<edge label="'+str(row+1)+' (edge) '+str(column+1)+'" source="'+str(row+1)+'" target="'+str(column+1)+'"><graphics cy:edgeLineType="SOLID" width="2" fill="#FFFF00"/><att type="real" name="correlation" value="'+str(self.matrix[row][column])+'"/></edge>\n')
							#r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$color <- "yellow"')
						else:
							# Assign line type (solid or dashed)
							if (row < self.numEnviroVars and column < self.numEnviroVars) or (row >= self.numEnviroVars and column >= self.numEnviroVars):
								#r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$lty <- 2')
								cytoscapefile.write('<edge label="'+str(row+1)+' (edge) '+str(column+1)+'" source="'+str(row+1)+'" target="'+str(column+1)+'"><graphics cy:edgeLineType="DASH" width="2" fill="#FF0000"/><att type="real" name="correlation" value="'+str(self.matrix[row][column])+'"/></edge>\n')
							else:
								#r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$lty <- 1')
								cytoscapefile.write('<edge label="'+str(row+1)+' (edge) '+str(column+1)+'" source="'+str(row+1)+'" target="'+str(column+1)+'"><graphics cy:edgeLineType="SOLID" width="2" fill="#FF0000"/><att type="real" name="correlation" value="'+str(self.matrix[row][column])+'"/></edge>\n')
							#r('E(PEN)['+str(vertices[row])+' %--% '+str(vertices[column])+']$color <- "green"')
		cytoscapefile.write('</graph>')
		cytoscapefile.close()
