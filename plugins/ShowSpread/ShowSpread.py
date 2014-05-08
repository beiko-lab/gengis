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
	
	def __init__( self, parent=None ):
		ShowSpreadLayout.__init__ ( self, parent )
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0:
			wx.MessageBox("This plugin requires map and location data to be loaded.", "Additional data required.")
			self.Close()
			return
		
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
		field = self.m_DataChoice.GetStringSelection()
		stopData = self.m_StopChoice.GetStringSelection()
		startData = self.m_StartChoice.GetStringSelection()
		#sD = startData.split('/')
		#stD = stopData.split('/')
		isDate=False
		#if backslashes in data, assume date
	#	if '/' in startData and '/' in stopData:
		if re.search(r'(\d+/\d+/\d+)',startData) and re.search(r'(\d+/\d+/\d+)',stopData):
			sD = dateutil.parser.parse(startData)
			stD = dateutil.parser.parse(stopData)
			isDate=True
			self.ConvertDates(isDate, field)
		
		if isDate:
			self.ShowSpread( date( sD.year,sD.month,sD.day ), date( stD.year, stD.month , stD.day ), field, isDate )
		else:
			self.ShowSpread( startData, stopData, field, isDate )
		#self.ShowSpread( date( sD.day,sD.month,sD.year ), date( stD.day, stD.month , stD.year ), field )
	
	
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
			
		# set camera position
	#	GenGIS.camera.SetYaw(0.0)
	#	GenGIS.camera.SetPitch(90.0)
	#	GenGIS.camera.SetPosition(0.0, 0.0, 1.5785574913024902)
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
			self.DisplayData( locData, field, startData, stopData )
		
		#self.DisplayTemporal( locData, field, startData, stopData )
		#self.DisplayData( locData, field, startData, stopData )
		
	def DisplayTemporal( self, locData, field, startData, stopData ):
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
		#	curDateStr = str(curData.day) + '/' + str(curData.month) + '/' + str(curData.year)
		#	print curDateStr
			
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
				loc.GetController().SetRenderingOrder(deltaDate.days*10)
				loc.GetController().SetSize(self.SizeSetter(numCases[id]))
				loc.GetController().SetActive(True)

			# set date of label
			self.label.SetText(str(curData.month) + "/" + str(curData.day) + "/" + str(curData.year))
			
			GenGIS.SafeYield()
			GenGIS.viewport.Refresh()
			
			
			curData = curData + dayDelta
			
			elapsedTime = time.time() - startTime
			if elapsedTime < timeCap:
				time.sleep(timeCap - elapsedTime)
	
	def DisplayData( self, locData, field, startData, stopData ):
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
				loc.GetController().SetActive(False)
				id = loc.GetController().GetId()

				if id in numCases:
					numCases[id] = numCases[id] + 1
					fieldData = float(loc.GetController().GetData()[field])
					if lastReportedCase[id] < fieldData:
						lastReportedCase[id] = fieldData
				else:
					numCases[id] = 1
					lastReportedCase[id] = loc.GetController().GetData()[field]
					uniqueLoc.append(loc)
					
			# set visual properties of location sites
			for loc in uniqueLoc:
				id= loc.GetController().GetId()
				if float(lastReportedCase[id] < 0 ):
					deltaDate = curData + float(lastReportedCase[id])
				else:
					deltaDate = curData - float(lastReportedCase[id])
				if self.ColourIntensity:
					loc.GetController().SetColour(self.ColourSetter(deltaDate))
				else:
					loc.GetController().SetColour(loc.GetController().GetColour())
				loc.GetController().SetRenderingOrder(deltaDate)
				loc.GetController().SetSize(self.SizeSetter(numCases[id]))
				loc.GetController().SetActive(True)
			# set date of label 
			self.label.SetText("%.2f" %curData)
			
			GenGIS.layerTree.UpdatePythonState()
			GenGIS.SafeYield()
			GenGIS.viewport.Refresh()
		
			curData = float(curData) + Delta

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
		
		values = []
		for layer in layers:
			data = layer.GetController().GetData()[field]
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
		
		for value in values:
			self.m_StartChoice.Append(value)
			self.m_StopChoice.Append(value)
		self.m_StartChoice.SetSelection(0)
		self.m_StopChoice.SetSelection( len(values)-1 )

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
	
	#Need a new ay to set color
	def ColourSetter(self,deltaDate):
		alpha = 1.0 - float(deltaDate)/int(self.m_StepsCtrl.GetValue())
		if alpha < 0.0:
			alpha = 0.0
		if alpha > 1.0:
			alpha = 1.0
		
		return  GenGIS.Colour(1.0, 0.0, 0.0, alpha)
		
	def SizeSetter(self,count):
		if count <= 1:
			return 3
		elif count < 10:
			return 5
		elif count < 100:
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
	
	def OnDateRadio( self, event ):
		if not self.dateFormat:
			self.dateFormat = True
		else:
			self.dateFormat = False
		print self.dateFormat
	
	def ConvertDates( self, isDate, field ):
		if isDate and self.dateFormat:
			locations = GenGIS.layerTree.GetLocationLayers()
			for loc in locations:
				data = loc.GetController().GetData()[field]
				print data
				date = data.split('/')
				day = date[0]
				month = date[1]
				year = date[2]
				loc.GetController().AddData(field, "%s/%s/%s" %(month,day,year))
				print loc.GetController().GetData()[field]
				
	def CrashCleanup( self ):
		for loc in GenGIS.layerTree.GetLocationLayers():
			loc.GetController().SetActive(True)