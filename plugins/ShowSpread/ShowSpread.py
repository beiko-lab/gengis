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
	# sort boolean. False = Descending, True = Ascending
	sort = False
	sortString = (filterFunc.lessEqual,filterFunc.greaterEqual)
	sortFloat = (filterFunc.lessEqualFloat, filterFunc.greaterEqualFloat)
	locData = {}
	locationMap = {}
	
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
		
		#Set the default number of steps to take through data
		self.m_StepsCtrl.SetValue( 10 )
		self.m_checkIntensity.SetValue(False)
		
	def OnClose( self, event ):
		#Need to remove the label on close too
		GenGIS.graphics.RemoveLabel(self.label.GetId())
		GenGIS.viewport.Refresh()
		self.Close()
	
	def OnOK( self, event ):
		#clear the old label out if its there
		GenGIS.graphics.RemoveLabel(self.label.GetId())
		
		field = self.m_DataChoice.GetStringSelection()
		stopData = self.m_StopChoice.GetStringSelection()
		startData = self.m_StartChoice.GetStringSelection()
		isDate=False
		#if backslashes in data, assume date
		if re.search(r'(\d+/\d+/\d+)',startData) and re.search(r'(\d+/\d+/\d+)',stopData):
			sD = dateutil.parser.parse(startData)
			stD = dateutil.parser.parse(stopData)
			isDate=True
			self.ConvertDates(isDate, field)
		
		if isDate:
			self.ShowSpread( date( sD.year,sD.month,sD.day ), date( stD.year, stD.month , stD.day ), field, isDate )
		else:
			self.ShowSpread( startData, stopData, field, isDate )
	
	
	def OnSteps( self, event ):
		self.Steps = int( self.m_StepsCtrl.GetValue() )
	
	def OnColourIntensity( self, event ):
		self.ColourIntensity = self.m_checkIntensity.IsChecked()
	
	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/index.php/Description_of_GenGIS_plugins#ShowSpread' )
		
	def ShowSpread( self, startData, stopData, field, isDate ):
		#returns all the location in a location set
		if( self.isSequence ):
			locData = GenGIS.layerTree.GetSequenceLayers()
		else:
			locData = GenGIS.layerTree.GetLocationLayers()
			
		GenGIS.viewport.Refresh()
		
		# set initial visual properties of all location sites
		for i in xrange(0, len(locData)):
			if self.ColourIntensity:
				locData[i].GetController().SetColour(GenGIS.Colour(1.0, 0.5, 0))
				locData[i].GetController().SetSize(4.0)
			locData[i].GetController().SetActive(False)
			
		GenGIS.viewport.Refresh()
		# label for showing current date
		self.label.SetScreenPosition(GenGIS.Point3D(40.0, 300.0, 1.0))
		self.label.SetRenderingStyle(GenGIS.LABEL_RENDERING_STYLE.PERSPECTIVE)
		GenGIS.graphics.AddLabel(self.label)
		# create legend
	#	self.CreateLegend()
		
		if isDate:
			self.DisplayTemporal( locData, field, startData, stopData )
		elif dh.isNumber(startData):
			self.DisplayNumeric( locData, field, startData, stopData )
		else:
			self.DisplayText( locData, field, startData, stopData )
		
	def DEFUNCTDisplayTemporal( self, locData, field, startData, stopData ):
		timeCap = float( self.m_spinTime.GetValue() ) / 10.0
		endData=stopData
		# show spread of virus on a daily basis	
		dayDelta = timedelta(days=1)
		
		# get cases from start date to current simulation date
		curData = startData
		minData = startData
		while(curData <= endData):
			#str(curData.day) + "/" + str(curData.month) + "/" + str(curData.year)
			GenGIS.viewport.Refresh()
			
			startTime = time.time()
			curDateStr = str(curData.month) + '/' + str(curData.day) + '/' + str(curData.year)
			
			#this should still work in newer versions of GenGIS
			#filteredLoc( Data list, field to operate on, Value to filter by, filter fucntion )
			# returns all locations with dates less than curDateStr
			filteredLoc = dh.filter(locData, field , curDateStr, filterFunc.lessEqualDate)
			
			numCases = {}
			lastReportedCase = {}
			uniqueLoc = []
			for loc in filteredLoc:
				locDate = self.dateStrToDate(loc.GetController().GetData()[field])
				loc.GetController().SetActive(False)
				id = loc.GetController().GetId()
				if locDate < minData:
					continue
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
				id= loc.GetController().GetId()
				deltaDate = curData - lastReportedCase[id]
				if self.ColourIntensity:
					loc.GetController().SetColour(self.ColourSetter(deltaDate.days))
				else:
					loc.GetController().SetColour(loc.GetController().GetColour())
					
				#Don't really know what this does, but it screws with how locations are drawn
		#		loc.GetController().SetRenderingOrder(deltaDate.days*10)
		#		loc.GetController().SetSize(self.SizeSetter(numCases[id]))
				loc.GetController().SetActive(True)

			# set date of label
			self.label.SetText(str(curData.month) + "/" + str(curData.day) + "/" + str(curData.year))
			
			GenGIS.SafeYield()
			GenGIS.viewport.Refresh()
			
			
			curData = curData + dayDelta
			
			elapsedTime = time.time() - startTime
			if elapsedTime < timeCap:
				time.sleep(timeCap - elapsedTime)
	
	def DeprecatedDisplayNumeric( self, locData, field, startData, stopData ):
		# show spread of virus on a data basis	
		timeCap = float( self.m_spinTime.GetValue() ) / 10.0
		startData,stopData = float(startData),float(stopData)
		Delta = (stopData - startData) / float(self.m_StepsCtrl.GetValue())

		# get cases from start date to current simulation date
		curData = startData
		minData = startData
		
		#needs a slight rounding adjustment as floats are ugly
		while curData <= (stopData + Delta/10) :
			startTime = time.time()
			curDataStr = str(curData)
			
			#this should still work in newer versions of GenGIS
			filteredLoc = dh.filter(locData, field, curDataStr, filterFunc.lessEqualFloat)
			filteredLoc = dh.filter(filteredLoc, field, minData, filterFunc.greaterEqualFloat)
		
			# determine number of cases and last reported case for all locations
			numCases = {}
			lastReportedCase = {}
			uniqueLoc = []
			for loc in filteredLoc:
				#convert sequence to it's location if possible
				if self.isSequence:
					#get seq data
					fieldData = float(loc.GetController().GetData()[field])
					#switch seq out for loc
					siteId = loc.GetController().GetSiteId()
					loc = [x for x in GenGIS.layerTree.GetLocationLayers() if x.GetController().GetId() == siteId][0]
					
				else:
					fieldData = float(loc.GetController().GetData()[field])
				loc.GetController().SetActive(False)
				id = loc.GetController().GetId()
	
				if id in numCases:
					numCases[id] = numCases[id] + 1
					if lastReportedCase[id] < fieldData:
						lastReportedCase[id] = fieldData
				else:
					numCases[id] = 1
					lastReportedCase[id] = fieldData
					uniqueLoc.append(loc)
					
			# set visual properties of location sites
			for loc in uniqueLoc:
				id = loc.GetController().GetId()
					
				if float(lastReportedCase[id] < 0 ):
					deltaDate = curData + float(lastReportedCase[id])
				else:
					deltaDate = curData - float(lastReportedCase[id])
				
				if self.ColourIntensity:
					loc.GetController().SetColour(self.ColourSetter(deltaDate))
				else:
					loc.GetController().SetColour(loc.GetController().GetColour())
					
				#Don't really know what this does, but it screws with how locations are drawn
			#	loc.GetController().SetRenderingOrder(deltaDate)
			#	loc.GetController().SetSize(self.SizeSetter(numCases[id]))
				loc.GetController().SetActive(True)
	
			# set text of label 
			self.label.SetText("%.2f" %curData)
			
			GenGIS.layerTree.UpdatePythonState()
			GenGIS.SafeYield()
			GenGIS.viewport.Refresh()
		
			curData = float(curData) + Delta

			elapsedTime = time.time() - startTime
			if elapsedTime < timeCap:
				time.sleep(timeCap - elapsedTime)
				
	def DeprecatedDisplayText( self, locData, field, startData, stopData ):
		# show spread of virus on a data basis	
		timeCap = float( self.m_spinTime.GetValue() ) / 10.0
		startData,stopData = startData, stopData
		curIndex = self.m_StartChoice.FindString(startData)
		stopIndex = self.m_StartChoice.FindString(stopData)
		Delta = math.ceil( self.m_StartChoice.GetCount() / float(self.m_StepsCtrl.GetValue()) )

		# get cases from start date to current simulation date
		curData = startData
		minData = startData
		
		#needs a slight rounding adjustment as floats are ugly
		while curIndex <= stopIndex :
			startTime = time.time()
			curDataStr = str(curData)
			
			#this should still work in newer versions of GenGIS
			filteredLoc = dh.filter(locData, field, curDataStr, filterFunc.lessEqual)
			filteredLoc = dh.filter(filteredLoc, field, minData, filterFunc.greaterEqual)
		
			# determine number of cases and last reported case for all locations
			numCases = {}
			lastReportedCase = {}
			uniqueLoc = []
			for loc in filteredLoc:
				loc.GetController().SetActive(False)
				id = loc.GetController().GetId()

				if id in numCases:
					numCases[id] = numCases[id] + 1
					fieldData = float(loc.GetController().GetData()[field])
					if lastReportedCase[id] < fieldData:
						lastReportedCase[id] = curIndex
				else:
					numCases[id] = 1
					lastReportedCase[id] = curIndex
					uniqueLoc.append(loc)
					
			# set visual properties of location sites
			for loc in uniqueLoc:
				id= loc.GetController().GetId()
				deltaDate = stopIndex - lastReportedCase[loc.GetController().GetId()]
				if self.ColourIntensity:
					loc.GetController().SetColour(self.ColourSetter(deltaDate))
				else:
					loc.GetController().SetColour(loc.GetController().GetColour())
				#Don't really know what this does, but it screws with how locations are drawn
		#		loc.GetController().SetRenderingOrder(deltaDate)
		#		loc.GetController().SetSize(self.SizeSetter(numCases[id]))
				loc.GetController().SetActive(True)
			# set date of label 
			self.label.SetText(curData)
			
			GenGIS.layerTree.UpdatePythonState()
			GenGIS.SafeYield()
			GenGIS.viewport.Refresh()
		
			curIndex = curIndex + Delta
			curData = self.m_StartChoice.GetString(curIndex)

			elapsedTime = time.time() - startTime
			if elapsedTime < timeCap:
				time.sleep(timeCap - elapsedTime)
			
		
	def OnDataChange( self, event ):
		# isSequence
		self.m_StartChoice.Clear()
		self.m_StopChoice.Clear()
		field = self.m_DataChoice.GetStringSelection()
		min, max = 0,0
		
		#if there are sequence layers
		if GenGIS.layerTree.GetNumSequenceLayers() > 0:
			#if field is in sequences or locations
			if field in GenGIS.layerTree.GetSequenceLayer(0).GetController().GetData().keys():
				self.isSequence = True
				layers = GenGIS.layerTree.GetSequenceLayers()
			else:
				self.isSequence = False
				layers = GenGIS.layerTree.GetLocationLayers()
		else:
			self.isSequence = False
			layers = GenGIS.layerTree.GetLocationLayers()
		
		values = self.SortFields( layers, field )
		
		for value in values:
			self.m_StartChoice.Append(value)
			self.m_StopChoice.Append(value)
		self.m_StartChoice.SetSelection(0)
		self.m_StopChoice.SetSelection( len(values)-1 )

	# DEPRECATED
	def CreateLegend( self ):
		sizes = [3, 5, 7, 9];
		labels = ["1 case", "< 10 cases", "< 100 cases", ">= 100 cases"]
		
		index = 0
		for size in sizes:	
			marker = GenGIS.VisualMarker(GenGIS.Colour(1.0, 0, 0), size, GenGIS.MARKER_SHAPE.CIRCLE, GenGIS.Point3D(40, 285-25*index, 0))
			marker.SetRenderingStyle(GenGIS.MARKER_RENDERING_STYLE.DECAL)
			GenGIS.graphics.AddMarker(marker)
			
			label = GenGIS.VisualLabel(labels[index], GenGIS.Colour(1.0, 1.0, 1.0), 18, GenGIS.LABEL_RENDERING_STYLE.ORTHO )
			label.SetScreenPosition(GenGIS.Point3D(52,285-25*index-6,1))
			label.SetRenderingStyle(GenGIS.LABEL_RENDERING_STYLE.ORTHO)
			GenGIS.graphics.AddLabel(label)
			
			index = index + 1
			
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

			
	def SortDates( self, dates ):
		dateList=[]
		dateStringList=[]
		dates = set(dates)
		for date in dates:
			date = dateutil.parser.parse(date)
			dateList.append(date)
		for date in sorted(dateList):
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
			loc.GetController().SetActive(True)
	
	# Performs specified sorting on data
	# This will be where the current data map will be updated. Also the only place IsSequence should matter!
	def SortFields( self, layers, field):
		values = []
		self.locData = {}
		for layer in layers:
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
		# Since the fields are unicode values, this sorts them numericaly if applicable, or alphabetically
		# This needs to be ripped out and replaced with SORTING options and algos
		if re.search(r'(\d+/\d+/\d+)',data):
			values = self.SortDates( values )
		elif( dh.isNumber(data) ):
			values = sorted( set(values), key = lambda val: float(val) )
		else:
			values = sorted( set(values) )
		# ################################################
		return values
	
	def OnSort( self, event):
		if self.m_SortChoice.GetStringSelection() == "Ascending":
			self.sort = True
		else:
			self.sort = False
	
	#maps location ID to its index in the location set
	def CreateLocationMap( self ):
		for index in range(0, GenGIS.layerTree.GetNumLocationLayers()):
			id = GenGIS.layerTree.GetLocationLayer(index).GetController().GetId()
			self.locationMap[id]=index
		
	def ShowSequences( self, loc ):
		if loc.GetController().IsActive():
			for seq in loc.GetAllSequenceLayers():
				seq.GetController().SetActive(True)
				
	def DisplayNumeric( self, locData, field, startData, stopData ):
		# show spread of virus on a data basis	
		timeCap = float( self.m_spinTime.GetValue() ) / 10.0
		startData,stopData = float(startData),float(stopData)
		Delta = (stopData - startData) / float(self.m_StepsCtrl.GetValue())

		# get cases from start date to current simulation date
		curData = startData
		minData = startData
		
		for loc in GenGIS.layerTree.GetLocationLayers():
				loc.GetController().SetActive(False)
				
		#needs a slight rounding adjustment as floats are ugly
		while curData <= (stopData + Delta/10) :
			startTime = time.time()
			curDataStr = str(curData)
		
			
				
			# determine number of cases and last reported case for all locations
			numCases = {}
			lastReportedCase = {}
			uniqueLoc = []
			for key in self.locData.keys():
				data = self.locData[key]
				filteredData = dh.genericFilter(data, curDataStr, filterFunc.lessEqualFloat)
				filteredData = dh.genericFilter(filteredData, minData, filterFunc.greaterEqualFloat)
				#convert sequence to it's location if possible
				for fieldData in filteredData:
					#need to get int value of location layer
					
					loc = GenGIS.layerTree.GetLocationLayer(key)
				#	loc.GetController().SetActive(False)
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
			
			GenGIS.layerTree.UpdatePythonState()
			GenGIS.SafeYield()
			GenGIS.viewport.Refresh()
		
			curData = float(curData) + Delta

			elapsedTime = time.time() - startTime
			if elapsedTime < timeCap:
				time.sleep(timeCap - elapsedTime)
				
	def DisplayText( self, locData, field, startData, stopData ):
		# show spread of virus on a data basis	
		timeCap = float( self.m_spinTime.GetValue() ) / 10.0
		startData,stopData = startData, stopData
		curIndex = self.m_StartChoice.FindString(startData)
		stopIndex = self.m_StartChoice.FindString(stopData)
		Delta = math.ceil( self.m_StartChoice.GetCount() / float(self.m_StepsCtrl.GetValue()) )
		# get cases from start date to current simulation date
		curData = startData
		minData = startData

		for loc in GenGIS.layerTree.GetLocationLayers():
			loc.GetController().SetActive(False)
				
		#needs a slight rounding adjustment as floats are ugly
		while curIndex <= stopIndex :
			startTime = time.time()
			curDataStr = str(curData)
			

			
			numCases = {}
			lastReportedCase = {}
			uniqueLoc = []
			for key in self.locData.keys():
				
				data = self.locData[key]
				filteredData = dh.genericFilter(data, curDataStr, filterFunc.lessEqual)
				filteredData = dh.genericFilter(filteredData, minData, filterFunc.greaterEqual)
				#convert sequence to it's location if possible
				for fieldData in filteredData:
					# determine number of cases and last reported case for all locations
					loc = GenGIS.layerTree.GetLocationLayer(key)
			#		loc.GetController().SetActive(False)
					id = loc.GetController().GetId()

					if id in numCases:
						numCases[id] = numCases[id] + 1
					#	fieldData = float(loc.GetController().GetData()[field])
						if lastReportedCase[id] < fieldData:
							lastReportedCase[id] = curIndex
					else:
						numCases[id] = 1
						lastReportedCase[id] = curIndex
						uniqueLoc.append(loc)
						
				# set visual properties of location sites
			for loc in uniqueLoc:
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
			
			GenGIS.layerTree.UpdatePythonState()
			GenGIS.SafeYield()
			GenGIS.viewport.Refresh()
		
			curIndex = curIndex + Delta
			curData = self.m_StartChoice.GetString(curIndex)

			elapsedTime = time.time() - startTime
			if elapsedTime < timeCap:
				time.sleep(timeCap - elapsedTime)
				
	def DisplayTemporal( self, locData, field, startData, stopData ):
		timeCap = float( self.m_spinTime.GetValue() ) / 10.0
		endData=stopData

		# divide time interval into even sections
		daySteps = self.m_StepsCtrl.GetValue()
		dayDelta = ( stopData - startData )/daySteps
		
		# get cases from start date to current simulation date
		curData = startData
		minData = str(curData.month) + '/' + str(curData.day) + '/' + str(curData.year)
		
		for loc in GenGIS.layerTree.GetLocationLayers():
				loc.GetController().SetActive(False)
				
		while(curData <= endData):
			GenGIS.viewport.Refresh()
			
			startTime = time.time()
			curDateStr = str(curData.month) + '/' + str(curData.day) + '/' + str(curData.year)
			
			numCases = {}
			lastReportedCase = {}
			uniqueLoc = []
			for key in self.locData.keys():
				data = self.locData[key]
				filteredData = dh.genericFilter(data, curDateStr, filterFunc.lessEqualDate)
				filteredData = dh.genericFilter(filteredData, minData, filterFunc.greaterEqualDate)	
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
			
			GenGIS.layerTree.UpdatePythonState()
			GenGIS.SafeYield()
			GenGIS.viewport.Refresh()
			
			# increment curData. Takes into account rounding errors when another loop needs to be run,
			# but is shy of the final date
			if curData + dayDelta > endData and curData != endData:
				curData = endData
			else:
				curData = curData + dayDelta
			
			elapsedTime = time.time() - startTime
			if elapsedTime < timeCap:
				time.sleep(timeCap - elapsedTime)