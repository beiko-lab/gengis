#=======================================================================
# Author: Alexander Keddy
#		  Using algorithm by Donovan Parks
#
# Copyright 2014 Alexander Keddy
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

from ShowSpreadLayout import ShowSpreadLayout
import GenGIS
import wx
import math
import re
from datetime import date
from datetime import timedelta
import dateutil.parser
import time
import dataHelper as dh
import filterFunc

class ShowSpread ( ShowSpreadLayout ):
	isSequence = False
	ColourIntensity = False
	label = GenGIS.VisualLabel("", GenGIS.Colour(0.0, 0.0, 0.0), 20, GenGIS.LABEL_RENDERING_STYLE.PERSPECTIVE )
	dateFormat = False
	# sort boolean. False = Descending, True = Descending
	sort = True
	sortString = (filterFunc.lessEqual,filterFunc.greaterEqual)
	sortFloat = (filterFunc.lessEqualFloat, filterFunc.greaterEqualFloat)
	locData = {}
	locationMap = {}
	# A list containning the colour and size of locations
	DefaultAttrib = {}
	# Map of Key -> State (Active/Off)
	StartingState = {}
	
	def __init__( self, parent=None ):
		ShowSpreadLayout.__init__ ( self, parent )
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0:
			wx.MessageBox("This plugin requires map and location data to be loaded.", "Additional data required.")
			self.Close()
			return
		
		# map all locations to their indexes in the set
		self.CreateLocationMap()
		
		locationLayer = GenGIS.layerTree.GetLocationLayer(0)

		# set data field with location and sequence data (if available)
		locLayerController = locationLayer.GetController()
		fields = locLayerController.GetData().keys()

		for field in fields:
			if field.lower() != 'siteid' and field.lower() != 'site id':
				self.m_DataChoice.Append(field)
		self.m_DataChoice.SetSelection(0)
		self.OnDataChange("fake")
		
		if GenGIS.layerTree.GetNumSequenceLayers() > 0:
			seqLayerController = GenGIS.layerTree.GetSequenceLayer(0).GetController()
			fields = seqLayerController.GetData().keys()

			for field in fields:
				if field.lower() != 'siteid' and field.lower() != 'site id':
					self.m_DataChoice.Append(field)
			self.m_DataChoice.SetSelection(0)
			#Set Start and Stop
			self.OnDataChange("fake")
		
		self.m_CheckIntensity.SetValue(False)
		self.OnBinning("fake")
		# Get starting state of locations
		self.SetStartAttrib()
		
	def OnOK( self, event ):
		#Need to remove the label on close too
		GenGIS.graphics.RemoveLabel(self.label.GetId())
		GenGIS.viewport.Refresh()
		self.Close()
	
	def OnRun( self, event ):
		#clear the old label out if its there
		GenGIS.graphics.RemoveLabel(self.label.GetId())

		#init min and max field values for grid
		GenGIS.layerTree.GetLocationSetLayer(0).GetLocationGrid().InitTileMinMax()
		
		field = self.m_DataChoice.GetStringSelection()
		stopData = self.m_StopChoice.GetStringSelection()
		startData = self.m_StartChoice.GetStringSelection()
		isDate=self.IsDate(startData,stopData)
		#if backslashes in data, assume date
		if isDate:
			sD = dateutil.parser.parse(startData)
			stD = dateutil.parser.parse(stopData)
			isDate=True
			self.ConvertDates(isDate, field)
		
		if isDate:
			self.ShowSpread( date( sD.year,sD.month,sD.day ), date( stD.year, stD.month , stD.day ), field, isDate )
		else:
			self.ShowSpread( startData, stopData, field, isDate )
	
	def IsDate( self, startData, stopData):
		startData, stopData = str(startData), str(stopData)
		if re.search(r'(\d+/\d+/\d+)',startData) and re.search(r'(\d+/\d+/\d+)',stopData):
			return True
		else:
			return False
	
	def OnSteps( self, event ):
		self.Steps = int( self.m_StepsCtrl.GetValue() )
	
	def OnColourIntensity( self, event ):
		self.ColourIntensity = self.m_CheckIntensity.IsChecked()
	
	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/index.php/Description_of_GenGIS_plugins#ShowSpread' )
		
	def ShowSpread( self, startData, stopData, field, isDate ):
		#returns all the location in a location set
		if( self.isSequence ):
		#	locData = GenGIS.layerTree.GetSequenceLayers()
			locData = dh.GetNonNullSequences( field )
		else:
		#	locData = GenGIS.layerTree.GetLocationLayers()
			locData = dh.GetNonNullLocations( field )

		GenGIS.viewport.Refresh()
		
		# set initial visual properties of all location sites
		for i in xrange(0, len(locData)):
			if self.isSequence:
				id = locData[i].GetController().GetSiteId()
				index = self.locationMap[id]
				loc = GenGIS.layerTree.GetLocationLayer(index)
				if self.ColourIntensity:
					loc.GetController().SetColour( GenGIS.Colour(1.0, 0.5, 0) )
					loc.GetController().SetSize(4.0)
				
				# Set initial state
				self.StartingState[loc.GetName()] = loc.GetController().IsActive()
			else:
				if self.ColourIntensity:
					locData[i].GetController().SetColour(GenGIS.Colour(1.0, 0.5, 0))
					locData[i].GetController().SetSize(4.0)
				
				# Set Initial State
		#		print "ID: ",locData[i].GetName()," State: ",locData[i].GetController().IsActive()
				self.StartingState[locData[i].GetName()] = locData[i].GetController().IsActive()
			locData[i].GetController().SetActive(False)
			
		GenGIS.viewport.Refresh()
		# label for showing current date
		self.label.SetScreenPosition(GenGIS.Point3D(40.0, 300.0, 1.0))
		self.label.SetRenderingStyle(GenGIS.LABEL_RENDERING_STYLE.PERSPECTIVE)
		GenGIS.graphics.AddLabel(self.label)
		
		if isDate:
			self.DisplayTemporal( locData, field, startData, stopData )
		elif dh.isNumber(startData):
			self.DisplayNumeric( locData, startData, stopData )
		else:
			self.DisplayText( locData, startData, stopData )
	
	# Gets the size, Active and colour of every location before ShowSpread is run
	# This data is used by the restore function
	def SetStartAttrib( self ):
		for loc in GenGIS.layerTree.GetLocationLayers():
			id = loc.GetName()
			colour = loc.GetController().GetColour()
			size = loc.GetController().GetSize()
			state = loc.GetController().IsActive()
			self.DefaultAttrib[id] = [colour,size,state]
	
	def OnRestore( self, event ):
		for key, value in self.DefaultAttrib.iteritems():
			index = self.locationMap[key]
			loc = GenGIS.layerTree.GetLocationLayer(index)
			loc.GetController().SetColour( value[0] )
			loc.GetController().SetSize( value[1] )
			loc.GetController().SetActive( value[2] )
		self.OnDataChange( "fake" )
		# NEEDS TO ALSO RESET START,STOP

		for i in xrange(0, GenGIS.layerTree.GetNumTreeLayers()):
                        GenGIS.layerTree.GetTreeLayer(i).GetGeoTreeView().RestoreTree()
			
		GenGIS.layerTree.GetLocationSetLayer(0).UpdateGridAndPolygons()
		GenGIS.viewport.Refresh()
	
	def OnDataChange( self, event ):
		# isSequence
		self.m_StartChoice.Clear()
		self.m_StopChoice.Clear()
		field = self.m_DataChoice.GetStringSelection()
		
		#if there are sequence layers
		if GenGIS.layerTree.GetNumSequenceLayers() > 0:
			#if field is in sequences or locations
			if field in GenGIS.layerTree.GetSequenceLayer(0).GetController().GetData().keys():
				self.isSequence = True
			#	layers = GenGIS.layerTree.GetSequenceLayers()
				layers = dh.GetNonNullSequences( field )
			else:
				self.isSequence = False
			#	layers = GenGIS.layerTree.GetLocationLayers()
				layers = dh.GetNonNullLocations( field )
		else:
			self.isSequence = False
		#	layers = GenGIS.layerTree.GetLocationLayers()
			layers = dh.GetNonNullLocations( field )
		
		values = self.SortFields( layers, field )
		
		for value in values:
			self.m_StartChoice.Append(value)
			self.m_StopChoice.Append(value)
		self.m_StartChoice.SetSelection(0)
		self.m_StopChoice.SetSelection( len(values)-1 )
		
		# Update the size of steps
		startData = self.m_StartChoice.GetStringSelection()
		stopData = self.m_StopChoice.GetStringSelection()
		steps = self.m_StepsCtrl.GetValue()
		
		#Compute Deltas to display to the user. For use with Binning
		if self.IsDate( startData,stopData ):
			startData = dateutil.parser.parse(startData)
			stopData = dateutil.parser.parse(stopData)
			if self.sort:
				Delta = ( startData - stopData ) /steps
			else:
				Delta = ( stopData - startData ) / steps
		elif dh.isNumber(startData):
			Delta = ( float(stopData) - float(startData) ) / float(steps)
		else:
			Delta = math.ceil( self.m_StartChoice.GetCount() / float(steps) )
		self.m_StepSizeTextCtrl.SetValue( str(Delta) )
		self.m_BinStartCtrl.SetValue( str( abs(Delta/2) ) )
		self.m_BinEndCtrl.SetValue( str( abs(Delta/2) ) )
#		self.m_StepsCtrl.SetValue( min( 10, self.m_StartChoice.GetCount() ) )
#		self.m_StepsCtrl.SetValue( self.m_StartChoice.GetCount()  )

	def dateStrToDate( self, dateStr ):
		"""
		Convert date stored in a string as month, day, year (i.e. 12/1/2009) to a date object 
		"""
		dateSplit = dateStr.split('/')
		month = int(dateSplit[0])
		day = int(dateSplit[1])
		year = int(dateSplit[2])
		try:
			return date(year, month, day)
		except:
			wx.MessageBox("The wrong date format has been selected. Please format the selected date field and try again.")
			self.CrashCleanup()
			self.OnClose("fake")
	
	def ColourSetter(self,deltaDate):
		alpha = 1.0 - float(deltaDate)/int(self.m_StepsCtrl.GetValue())
		if alpha < 0.0:
			alpha = 0.0
		if alpha > 1.0:
			alpha = 1.0
		
		return  GenGIS.Colour(1.0, 0.0, 0.0, alpha)
		
	def SizeSetter(self,count):
		numSeqs = GenGIS.layerTree.GetNumSequenceLayers()
		numLocs = GenGIS.layerTree.GetNumLocationLayers()
		N = numSeqs / numLocs
		bin = N/4
		if count <= bin:
			return 3
		elif count < bin*2:
			return 5
		elif count < bin*3:
			return 7
		else:
			return 9

			
	def SortDates( self, dates, ascend ):
		dateList=[]
		dateStringList=[]
		dates = set(dates)
		for date in dates:
			date = dateutil.parser.parse(date)
			dateList.append(date)
		for date in sorted(dateList, reverse = ascend):
			dateStringList.append( "%s/%s/%s/" %(date.month, date.day, date.year) )
		return dateStringList
	
	# Changes the date format present in the data
	def OnDateRadio( self, event ):
		if not self.dateFormat:
			self.dateFormat = True
		else:
			self.dateFormat = False
	
	# Converts Dates to strings
	def ConvertDates( self, isDate, field ):
		if isDate and self.dateFormat:
			locations = GenGIS.layerTree.GetLocationLayers()
			for loc in locations:
				data = loc.GetController().GetData()[field]
				date = data.split('/')
				day = date[0]
				month = date[1]
				year = date[2]
				loc.GetController().AddData(field, "%s/%s/%s" %(month,day,year))
				
	# Resets all the layers to active in event of a crash
	def CrashCleanup( self ):
		for loc in GenGIS.layerTree.GetLocationLayers():
			if self.StartingState[loc.GetName()]:
				loc.GetController().SetActive(True)
	
	# Performs specified sorting on data
	# This will be where the current data map will be updated. Also the only place IsSequence should matter!
	def SortFields( self, layers, field):
		values = []
		self.locData = {}
		for layer in layers:
			if not layer.GetController().IsActive():
				continue
			data = layer.GetController().GetData()[field]
			if self.isSequence:
				id = layer.GetController().GetSiteId()
			else:
				id = layer.GetController().GetId()
			layerNum = self.locationMap[id]
			if layerNum in self.locData.keys():
				self.locData[layerNum].append(data)
			else:
				self.locData[layerNum] = [data]
			values.append(data)
		# a flag for whether or not sorting should be descending or ascending
		if self.sort:
			ascend = True
		else:
			ascend = False
		# Since the fields are unicode values, this sorts them numericaly if applicable, or alphabetically
		if re.search(r'(\d+/\d+/\d+)',data):
			values = self.SortDates( values , ascend)
		elif( dh.isNumber(data) ):
			values = sorted( set(values), key = lambda val: float(val), reverse= ascend )
		else:
			values = sorted( set(values), reverse = ascend )
		return values
	
	def OnSort( self, event ):
		if self.m_SortChoice.GetStringSelection() == "Ascending":
			self.sort = False
		else:
			self.sort = True
		self.OnDataChange("fake")
	
	def OnBinning( self, event ):
		check=self.m_CheckBin.IsChecked()
		
		self.m_StepSizeText.Enable(check)
		self.m_StepSizeTextCtrl.Enable(check)
		self.m_BinStartText.Enable(check)
		self.m_BinStartCtrl.Enable(check)
		self.m_BinEndText.Enable(check)
		self.m_BinEndCtrl.Enable(check)
	
	# Simple function to determine if the display function's conditional has been met.
	def SortedPass( self, curData, stopData ):
		if not self.sort:
			if type(curData) is float:
				return( curData <= stopData + 0.00001 )

			return( curData <= stopData )
		else:
			if type(curData) is float:
				return( curData >= stopData - 0.00001 )
				
			return( curData >= stopData )
		
	#maps location ID to its index in the location set
	def CreateLocationMap( self ):
		for index in range(0, GenGIS.layerTree.GetNumLocationLayers()):
			id = GenGIS.layerTree.GetLocationLayer(index).GetController().GetId()
			self.locationMap[id]=index
		
	def ShowSequences( self, loc ):
		if loc.GetController().IsActive():
			for seq in loc.GetAllSequenceLayers():
				seq.GetController().SetActive(True)
				
	def DisplayNumeric( self, locData, startData, stopData ):
		# show spread of virus on a data basis	
		timeCap = float( self.m_SpinTime.GetValue() ) / 10.0
		startData,stopData = float(startData),float(stopData)
		Delta = (stopData - startData) / float(self.m_StepsCtrl.GetValue())

		curData = startData
		minData = startData
		lastData = startData
		
		uniqueLoc = []
		if self.m_CheckBin.IsChecked():
			BinFloor = float( self.m_BinStartCtrl.GetValue() )
			BinCeil = float( self.m_BinEndCtrl.GetValue() )
		else:
			BinFloor,BinCeil = 0,0
		for loc in GenGIS.layerTree.GetLocationLayers():
				loc.GetController().SetActive(False)
				
		while self.SortedPass( curData, stopData ):
			
			if self.m_CheckBin.IsChecked():
			#	minData = lastData
				minData = curData
				for loc in uniqueLoc:
					loc.GetController().SetActive(False)
					
			startTime = time.time()
				
			# determine number of cases and last reported case for all locations
			numCases = {}
			lastReportedCase = {}
			uniqueLoc = []
	#		print "Cur = ",curData," Ceil = ",curData + BinCeil, " Floor = ", minData - BinFloor
			for key in self.locData.keys():
				data = self.locData[key]
				# ascending
				if not self.sort:
					filteredData = dh.genericFilter(data, curData + BinCeil, filterFunc.lessEqualFloat)
					filteredData = dh.genericFilter(filteredData, minData - BinFloor, filterFunc.greaterEqualFloat)
				#descending
				else:
					filteredData = dh.genericFilter(data, curData - BinFloor , filterFunc.greaterEqualFloat)
					filteredData = dh.genericFilter(filteredData, minData + BinCeil , filterFunc.lessEqualFloat)
				#convert sequence to it's location if possible
				for fieldData in filteredData:
					#need to get int value of location layer
					
					loc = GenGIS.layerTree.GetLocationLayer(key)
					id = loc.GetController().GetId()
		
					if id in numCases.keys():
						numCases[id] = numCases[id] + 1
						if lastReportedCase[id] < fieldData:
							lastReportedCase[id] = fieldData
					else:
						numCases[id] = 1
						lastReportedCase[id] = fieldData
						uniqueLoc.append(loc)
					
			# set visual properties of location sites
			for loc in uniqueLoc:
				if not self.StartingState[loc.GetName()]:
					continue
				id = loc.GetController().GetId()
					
				if float(lastReportedCase[id] < 0 ):
					deltaDate = curData + float(lastReportedCase[id])
				else:
					deltaDate = curData - float(lastReportedCase[id])
				
				if self.ColourIntensity:
					loc.GetController().SetColour(self.ColourSetter(deltaDate))
				else:
					loc.GetController().SetColour(loc.GetController().GetColour())
				
			#	loc.GetController().SetRenderingOrder(deltaDate)
				loc.GetController().SetActive(True)
				if self.isSequence:
					loc.GetController().SetSize(self.SizeSetter(numCases[id]))
					self.ShowSequences(loc)
	
			# set text of label 
			self.label.SetText("%.2f" %curData)

			# update grid and polygons
			GenGIS.layerTree.GetLocationSetLayer(0).UpdateGridAndPolygons()
			
			GenGIS.layerTree.UpdatePythonState()
			GenGIS.SafeYield()
			GenGIS.viewport.Refresh()
		
			#curData = float(curData) + Delta
		#	if curData + Delta > float(stopData) and curData != float(stopData):
		#		curData = float(stopData)
		#	else:
			lastData = curData
			curData = float(curData) + Delta
			
			elapsedTime = time.time() - startTime
			if elapsedTime < timeCap:
				time.sleep(timeCap - elapsedTime)
				
	def DisplayText( self, locData, startData, stopData ):
		# show spread of virus on a data basis	
		timeCap = float( self.m_SpinTime.GetValue() ) / 10.0
		# Need to add a fudge of -1 so that a Delta of 2 will include 0,1 then 2,3 etc
		Delta = math.ceil( self.m_StartChoice.GetCount() / float(self.m_StepsCtrl.GetValue()) )
		# get cases from start date to current simulation date
	#	curData = startData
		curIndex = self.m_StartChoice.FindString(startData)
		stopIndex = self.m_StartChoice.FindString(stopData)

		minDataStr = str(startData)
		
		uniqueLoc = []
		if self.m_CheckBin.IsChecked():
			BinFloor = float( self.m_BinStartCtrl.GetValue() )
			BinCeil = float( self.m_BinEndCtrl.GetValue() )
		else:
			BinFloor,BinCeil = 0,0
		
		for loc in GenGIS.layerTree.GetLocationLayers():
			loc.GetController().SetActive(False)
		
		curIndex = curIndex + Delta - 1
		#needs a slight rounding adjustment as floats are ugly
		while curIndex <= stopIndex :
			#get current data
			curData = self.m_StartChoice.GetString(curIndex)
		#	if curIndex + BinCeil <= stopIndex:
		#		binCeilDataStr = self.m_StartChoice.GetString(curIndex + BinCeil)
		#	else:
			binCeilDataStr = self.m_StartChoice.GetString(curIndex)
			curDataStr = str(curData)
			
			# check if binning is used. If it is, update the indexes
			if self.m_CheckBin.IsChecked():
				if curIndex - BinFloor < 0:
					lastData = self.m_StartChoice.GetString(0)
				else:
					lastData = self.m_StartChoice.GetString(curIndex - BinFloor)
				minDataStr = str(lastData)
				
				for loc in uniqueLoc:
					loc.GetController().SetActive(False)
			
			startTime = time.time()
			
			numCases = {}
			lastReportedCase = {}
			uniqueLoc = []
			
			for key in self.locData.keys():
				data = self.locData[key]
				if not self.sort:
					filteredData = dh.genericFilter(data, binCeilDataStr , filterFunc.lessEqual)
					filteredData = dh.genericFilter(filteredData, minDataStr , filterFunc.greaterEqual)
				else:
					filteredData = dh.genericFilter(data, binCeilDataStr , filterFunc.greaterEqual)
					filteredData = dh.genericFilter(filteredData, minDataStr, filterFunc.lessEqual)
				#convert sequence to it's location if possible
				for fieldData in filteredData:
					# determine number of cases and last reported case for all locations
					loc = GenGIS.layerTree.GetLocationLayer(key)
					id = loc.GetController().GetId()

					if id in numCases:
						numCases[id] = numCases[id] + 1
						if lastReportedCase[id] < fieldData:
							lastReportedCase[id] = curIndex
					else:
						numCases[id] = 1
						lastReportedCase[id] = curIndex
						uniqueLoc.append(loc)
						
				# set visual properties of location sites
			for loc in uniqueLoc:
				if not self.StartingState[loc.GetName()]:
					continue
				id= loc.GetController().GetId()
				deltaDate = stopIndex - lastReportedCase[loc.GetController().GetId()]
				if self.ColourIntensity:
					loc.GetController().SetColour(self.ColourSetter(deltaDate))
				else:
					loc.GetController().SetColour(loc.GetController().GetColour())
				#Don't really know what this does, but it screws with how locations are drawn
		#		loc.GetController().SetRenderingOrder(deltaDate)
				
				loc.GetController().SetActive(True)
				if self.isSequence:
					loc.GetController().SetSize(self.SizeSetter(numCases[id]))
					self.ShowSequences(loc)
				
			# set date of label 
			self.label.SetText(curData)

			# update grid and polygons
			GenGIS.layerTree.GetLocationSetLayer(0).UpdateGridAndPolygons()
			
			GenGIS.layerTree.UpdatePythonState()
			GenGIS.SafeYield()
			GenGIS.viewport.Refresh()
				
			curIndex = curIndex + Delta

			elapsedTime = time.time() - startTime
			if elapsedTime < timeCap:
				time.sleep(timeCap - elapsedTime)
				
	def DisplayTemporal( self, locData, field, startData, stopData ):
		timeCap = float( self.m_SpinTime.GetValue() ) / 10.0
		endData=stopData
		
		# divide time interval into even sections
		daySteps = self.m_StepsCtrl.GetValue()
		
		if self.sort:
			dayDelta = ( startData - stopData ) /daySteps
		else:
			dayDelta = ( stopData - startData ) / daySteps

		# IMPORTANT NOTE
		# ALL DATES WILL BE BINNED BY DAYS
		uniqueLoc = []
		if self.m_CheckBin.IsChecked():
			BinFloor = float( self.m_BinStartCtrl.GetValue() )
			BinCeil = float( self.m_BinEndCtrl.GetValue() )
		else:
			BinFloor,BinCeil = 0,0
		
		# get cases from start date to current simulation date
		curData = startData
	#	minData = str(curData.month) + '/' + str(curData.day) + '/' + str(curData.year)
		minData = startData
		lastData = minData
		
		for loc in GenGIS.layerTree.GetLocationLayers():
				loc.GetController().SetActive(False)
				
		while self.SortedPass( curData, endData ):
		#	GenGIS.viewport.Refresh()
			if self.m_CheckBin.IsChecked():
				minData = curData
				for loc in uniqueLoc:
					loc.GetController().SetActive(False)
			
			startTime = time.time()
		#	curDateStr = str(curData.month) + '/' + str(curData.day) + '/' + str(curData.year)
			
			numCases = {}
			lastReportedCase = {}
			uniqueLoc = []
			if not self.sort:
				temCur = curData + timedelta(days=BinCeil)
				temMin = minData - timedelta(days=BinFloor)
			else:
				temCur = curData - timedelta(days=BinFloor)
				temMin = minData + timedelta(days=BinCeil)
			temCur = str(temCur.month) + '/' + str(temCur.day) + '/' + str(temCur.year)
			temMin = str(temMin.month) + '/' + str(temMin.day) + '/' + str(temMin.year)
			
			for key in self.locData.keys():
				data = self.locData[key]
				if not self.sort:
					filteredData = dh.genericFilter(data, temCur, filterFunc.lessEqualDate)
					filteredData = dh.genericFilter(filteredData, temMin, filterFunc.greaterEqualDate)	
				else:
					filteredData = dh.genericFilter(data, temCur, filterFunc.greaterEqualDate)
					filteredData = dh.genericFilter(filteredData, temMin, filterFunc.lessEqualDate)
					
				for fieldData in filteredData:
					loc = GenGIS.layerTree.GetLocationLayer(key)
					
					locDate = self.dateStrToDate(loc.GetController().GetData()[field])
					id = loc.GetController().GetId()
					if id in numCases:
						numCases[id] = numCases[id] + 1

						if lastReportedCase[id] < locDate:
							lastReportedCase[id] = locDate
					else:
						numCases[id] = 1
						lastReportedCase[id] = locDate
						uniqueLoc.append(loc)
						
				# set visual properties of location sites
			for loc in uniqueLoc:
				if not self.StartingState[loc.GetName()]:
					continue
				id= loc.GetController().GetId()
				deltaDate = curData - lastReportedCase[id]
				if self.ColourIntensity:
					loc.GetController().SetColour(self.ColourSetter(deltaDate.days))
				else:
					loc.GetController().SetColour(loc.GetController().GetColour())
					
				loc.GetController().SetActive(True)
				if self.isSequence:
					loc.GetController().SetSize(self.SizeSetter(numCases[id]))
					self.ShowSequences(loc)
					
			# set date of label
			self.label.SetText(str(curData.month) + "/" + str(curData.day) + "/" + str(curData.year))

			# update grid and polygons
			GenGIS.layerTree.GetLocationSetLayer(0).UpdateGridAndPolygons()
			
			GenGIS.layerTree.UpdatePythonState()
			GenGIS.SafeYield()
			GenGIS.viewport.Refresh()
			
			# increment curData. Takes into account rounding errors when another loop needs to be run,
			# but is shy of the final date
			
			lastData = curData
			if self.sort:
				if curData - dayDelta < endData and curData != endData:
					curData = endData
				else:
					curData = curData - dayDelta
			else:
				if curData + dayDelta > endData and curData != endData:
					curData = endData
				else:
					curData = curData + dayDelta
			
			elapsedTime = time.time() - startTime
			if elapsedTime < timeCap:
				time.sleep(timeCap - elapsedTime)
				
			
