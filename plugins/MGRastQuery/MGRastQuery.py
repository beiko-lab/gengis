#=======================================================================
# Author: Alexander Keddy
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

from MGRASTQueryLayout import MGRASTQueryLayout
from MGRASTQueryLayout import OptionsFrame
import GenGIS
import wx
import math
import pickle
import urllib2
import re
import sys
import time
from MGRastSpecific import MGRastSpecific
from GBIFGeneric import GBIFGeneric

class Options(OptionsFrame):
	def __init__(self):
		OptionsFrame.__init__(self,None)
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico",wx.BITMAP_TYPE_ICO))
	
	def GetSearchType(self):
		if self.m_searchTypeCheck.IsChecked():
			return "%s" %self.m_searchType.GetString(self.m_searchType.GetSelection())
		else:
			return 'organism'
	
	def GetFilterLevel(self):
		if self.m_filterLevelCheck.IsChecked():
			return "&filter_level=%s" %self.m_filterLevel.GetString(self.m_filterLevel.GetSelection())
		else:
			return ""
	
	def GetFilterSource(self):	
		if self.m_filterSourceCheck.IsChecked():
			return "&filer_source=%s" %self.m_filterSource.GetString(self.m_filterSource.GetCurrentSelection())
		else:
			return ""
			
	def GetGroupLevel(self):
		if self.m_groupLevelCheck.IsChecked():
			return "&group_level=%s" %self.m_groupLevel.GetString(self.m_groupLevel.GetCurrentSelection())
		else:
			return ""
			
	def GetHitType(self):
		if self.m_hitTypeCheck.IsChecked():
			return "&hit_type=%s" %self.m_hitType.GetString(self.m_hitType.GetCurrentSelection())
		else:
			return ""
			
	def GetIdentity(self):		
		if self.m_identityCheck.IsChecked():
			return "&identity=%s" %self.m_identity.GetValue()
		else:
			return ""
			
	def GetLength(self):		
		if self.m_lengthCheck.IsChecked():
			return "&length=%s" %self.m_length.GetValue()
		else:
			return ""
			
	def GetSource(self):		
		if self.m_sourceCheck.IsChecked():
			return "&source=%s" %self.m_source.GetString(self.m_source.GetCurrentSelection())
		else:
			return ""
			
	def OnOK(self,event):
		self.Hide()
	
	def OnClose(self):
		self.Hide()
		
class MGRastQuery(MGRASTQueryLayout):
	#	Global variables to store queried information
	__obs__ = []
	__selectedTaxon__= set()
	__description__=""
	__options__=""
	__metaVals__ = {}
	__TAXON__ = ["Domain","Phylum","Class","Order","Family","Genus","Species","Strain"]
	
	def __init__(self,parent=None):
		self.__options__ = Options()
		self.MGRastSpecific = MGRastSpecific()
		MGRASTQueryLayout.__init__(self,parent)
		self.GBIFGeneric = GBIFGeneric()
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico",wx.BITMAP_TYPE_ICO))
		self.m_Add.SetBitmapLabel(wx.Image(GenGIS.mainWindow.GetExeDir() + "images/green_arrow_down.png",wx.BITMAP_TYPE_PNG).ConvertToBitmap())
		self.m_Add.SetBitmapHover(wx.Image(GenGIS.mainWindow.GetExeDir() + "images/green_arrow_down_hover.png",wx.BITMAP_TYPE_PNG).ConvertToBitmap())
		self.m_Remove.SetBitmapLabel(wx.Image(GenGIS.mainWindow.GetExeDir() + "images/red_arrow_up.png",wx.BITMAP_TYPE_PNG).ConvertToBitmap())
		self.m_Remove.SetBitmapHover(wx.Image(GenGIS.mainWindow.GetExeDir() + "images/red_arrow_up_hover.png",wx.BITMAP_TYPE_PNG).ConvertToBitmap())
		self.m_Compass.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/GBIF_compass_small.png",wx.BITMAP_TYPE_PNG))
		MaxLon,MinLon,MaxLat,MinLat = 180,-180,90,-90
		
		self.m_IDList.Clear()
		#fix to expand summary box enough to print two lines of text properly
		self.m_Summary.SetLabel("\n\n")
		
		
		#Map Data
		if GenGIS.layerTree.GetNumMapLayers() > 0 :
			self.m_AddData.Enable()
			borders = GenGIS.layerTree.GetMapLayer(0).GetController().GetMapBorders()
			#check if geographic coordinates are used or some other measure; only geographic are compatible
			geographic = GenGIS.StudyController.IsUsingGeographic(GenGIS.study.GetController())
			projected = GenGIS.StudyController.IsUsingProjection(GenGIS.study.GetController())
			if(not (geographic or projected)):
				wx.MessageBox("Geographic coordinates are not being used in the current map file. Only geographic coordinates are compatible with GBIF. Geographic range will need to be manually set, and any returned data will not display correctly.","Warning")
				self.m_AddData.Disable()
				self.m_MinLat.SetValue(str(MinLat))
				self.m_MaxLat.SetValue(str(MaxLat))
				self.m_MinLon.SetValue(str(MinLon))
				self.m_MaxLon.SetValue(str(MaxLon))
			else:
				#Text boxes hate non String types. use int to round, and string to make them fit the container
				self.m_MinLat.SetValue(str(max(MinLat,borders.y1)))
				self.m_MaxLat.SetValue(str(min(MaxLat,borders.dy)))
				self.m_MinLon.SetValue(str(max(MinLon,borders.x1)))
				self.m_MaxLon.SetValue(str(min(MaxLon,borders.dx)))

			
	#	Query GBIF for Taxa in Lat/Lon Boundary
	def OnSearch(self,event):
		wx.BeginBusyCursor()
		#	Clear the results list
		self.m_Result.Clear()
		searchType = self.__options__.GetSearchType()
		taxon = self.m_TaxonName.GetLineText(0)
		taxon=taxon.split()
		if(len(taxon)==0):
			wx.MessageBox("You did not enter a taxon name.")
		else:
			minLatitude= float(self.m_MinLat.GetValue())
			maxLatitude= float(self.m_MaxLat.GetValue())
			minLongitude= float(self.m_MinLon.GetValue())
			maxLongitude= float(self.m_MaxLon.GetValue())
			if searchType == "study":
				self.__selectedTaxon__.clear()
			#	test data for multiple study search mgm4440037.3 mgm4440055.3 mgm4440064.3
				for tax in taxon:
					self.__selectedTaxon__.add((tax,"dummy value"))
				self.OnCalculate(wx.EVT_BUTTON)
			else:
				matches=self.MGRastSpecific.GETTAXRESULT(taxon,searchType,minLatitude,maxLatitude,minLongitude,maxLongitude,self.m_Summary)				
				if matches:
					self.m_Result.InsertItems(matches,0)
		wx.EndBusyCursor()
		
	#	Create Sequence and Location files for selected Taxa
	######################
	#
	#	CAN NOT PERFORM MORE THAN ONE QUERY PER SECOND
	#
	######################
	def OnCalculate(self,event):
		self.__obs__=[]
		self.__metaVals__={}
		self.m_Summary.SetLabel("\n")
		records,distLocations = 0,0
		searchType = self.__options__.GetSearchType()
		additFields = "%s%s%s%s%s%s%s"%(self.__options__.GetFilterLevel(),self.__options__.GetFilterSource(),self.__options__.GetGroupLevel(),self.__options__.GetHitType(),self.__options__.GetIdentity(),self.__options__.GetLength(),self.__options__.GetSource())
		if(self.__selectedTaxon__):
			self.m_Progress.WriteText("Starting...\n")
			#index of which taxon is being used
			index = 0
			for tax in self.__selectedTaxon__:
				startTime = time.time()
				obs, metaVals, taxonLength = self.MGRastSpecific.GETOBS(tax[0],searchType,additFields,self.m_Progress)
				taxonomy = []
				for i in range(0,taxonLength):
					if i < len(self.__TAXON__):
						taxonomy.append(self.__TAXON__[i])
					else:
						taxonomy[i].append("Level_%s"%i)
				self.__TAXON__= taxonomy
				if obs:	
					self.__obs__.append(obs)
				# add all unique key/val pairs from json files
				for key,value in metaVals.iteritems():
					if key in self.__metaVals__:
						self.__metaVals__[key][index] = value
					else:
						list = ['']*len(self.__selectedTaxon__)
						list[index] = value
						self.__metaVals__[key] = list
				index += 1
				if (time.time() - startTime) < 1:
					sleep.time(1)
		else:
			wx.MessageBox("Please select some Taxa.")
		self.m_Progress.WriteText("Done\n")
		
	#	Present the number of locations a user is about to query
	#	Used as a check by the user to know they aren't going to produce way too much data.
	def OnPreCalculate(self,event):
		#stufstufstu
		return 0
		
	#	Redirects User to Wiki page for this plugin
	def OnHelp(self, event):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/Description_of_GenGIS_plugins#GBIF_Query' )
	
	#	Adds Data to GenGIS
	def OnAddData(self,event):
		# check required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0:
			wx.MessageBox("This action requires a Map layer. Please load a Map.")
			self.Close()
			return
		taxonLevels = ','.join(str(i) for i in self.__TAXON__)
		
		if (len(self.__obs__) > 0):
			OUTLText, OUTSText = self.MGRastSpecific.GETTEXT(self.__obs__,self.__metaVals__)
			OUTLArray=self.GBIFGeneric.CPPOUT(OUTLText)
			OUTSArray=self.GBIFGeneric.CPPOUT(OUTSText)
			metKey = ','.join(sorted(self.__metaVals__.keys()))
			OUTLArray.insert(0,"Site ID,Latitude,Longitude,Cell ID,%s" %metKey)
			OUTSArray.insert(0,"Sequence ID,Site ID,CellLat,CellLong,Richness,%s,Taxonomy" %taxonLevels)					
			OUTLArray.pop()
			OUTSArray.pop()
			layerName = "MGRASTLayer_%d" % GenGIS.layerTree.GetNumLocationLayers()
			GenGIS.mainWindow.OpenLocationsCSVFile(OUTLArray, layerName)
			GenGIS.mainWindow.OpenSequenceCSVFile(OUTSArray, layerName)
			
			#Get the number of last location layer added (the gbif one)
			numLocationLayers=GenGIS.layerTree.GetNumLocationSetLayers()
			locationSetLayer = GenGIS.layerTree.GetLocationSetLayer(numLocationLayers-1)
		#	locationSetLayer.SetDescription(self.__description__)
			
		else:
			wx.MessageBox("Please make a successful GBIF Query first.")
		
	#	Exports Location and Sequence Data to a location of the users choice
	def OnExportData(self,event):
		if (len(self.__obs__) > 0):
			taxonLevels = ','.join(str(i) for i in self.__TAXON__)
			fileTypes = 'Loc and Seq Files (*.csv)|*.csv'
			dlg = wx.FileDialog(self, "Save plot", "", "", fileTypes, wx.SAVE)
			if dlg.ShowModal()==wx.ID_OK:
				filename =	dlg.GetFilename()
				dir = dlg.GetDirectory()
				file_split = filename.split(".",1)
				#creates the directories
				OUTLfile = ("%s/%s_locs.csv" % (dir,file_split[0]))				
				OUTSfile = ("%s/%s_seqs.csv" % (dir,file_split[0]))
				OUTLText, OUTSText = self.MGRastSpecific.GETTEXT(self.__obs__,self.__metaVals__)
			#	metKey = ','.join(str(x) for x in self.__metaKeys__)
				metKey = ','.join(sorted(self.__metaVals__.keys()))
			#	print metKey
				self.GBIFGeneric.WRITEEXPORT(OUTLfile,OUTLText,"Site ID,Latitude,Longitude,Cell ID,%s\n" %metKey)
				self.GBIFGeneric.WRITEEXPORT(OUTSfile,OUTSText,"Sequence ID,Site ID,CellLat,CellLong,Richness,%s,Taxonomy\n" %taxonLevels)
			dlg.Destroy()
		else:
			wx.MessageBox("Please make a successful GBIF Query first.")
	
	#	Add Data from Results Table to ID List
	def OnAdd(self,event):
		i=0
		IDCount = self.m_IDList.GetCount()
		for index in self.m_Result.GetSelections():
			selected = self.m_Result.GetString(index)
			split = selected.split(" | ")
			if (split[0],split[1]) not in self.__selectedTaxon__:
				self.m_IDList.InsertItems(["%s" % selected],IDCount+i)
				#add study ID and study location
				self.__selectedTaxon__.add((split[0],split[1]))
				i+=1
			
	#	Remove Data from ID List
	def OnRemove(self,event):
		candidates = sorted(self.m_IDList.GetSelections(),reverse=True)
		for index in candidates:
			selected = self.m_IDList.GetString(index)
			split = selected.split(" | ")
			self.__selectedTaxon__.remove((split[0],split[1]))
			self.m_IDList.Delete(index)
	
	#	Close the Plugin
	def OnClose(self, event):
		event.Skip()
	
	#	Close the Plugin
	def OnOK( self, event ):
		self.Close()	
	
	def OnLatEnter(self,event):
		str = event.GetString()
		str2 = re.sub('[^\d\.\-]','',str)
		if str2 and str !="-":
			str3=float(str2)
			if str3 > 90:
				str2 = str2[:-1]
			elif str3 < -90:
				str2 = str2[:-1]
		event.GetClientData().SetValue(str2)
		event.GetClientData().SetInsertionPoint(len(str2))
		
	def OnLonEnter(self,event):
		str = event.GetString()
		str2 = re.sub('[^\d\.\-]','',str)
		if str2 and str !="-":
			str3=float(str2)
			if str3 > 180:
				str2 = str[:-1]
			elif str3 < -180:
				str2 = str[:-1]
		event.GetClientData().SetValue(str2)
		event.GetClientData().SetInsertionPoint(len(str2))
	
	def OnOptions(self,event):
		self.__options__.Show()
		
