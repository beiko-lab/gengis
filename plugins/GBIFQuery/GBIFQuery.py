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

from GBIFQueryLayout import GBIFQueryLayout

import GenGIS
import wx
import math
import pickle
import urllib2
import re
import sys
from xml.dom import minidom
from dataHelper import isNumber
from GBIFGeneric import GBIFGeneric
from GBIFSpecific import GBIFSpecific
#from sets import Set

class GBIFQuery(GBIFQueryLayout):
	#	Global variables to store queried information
	__obs__ = []
	__conversions__ = []
	__selectedTaxon__= set()
	__description__=""
	
	def __init__(self,parent=None):
#		import pdb; pdb.set_trace()
		self.GBIFSpecific = GBIFSpecific()
		self.GBIFGeneric = GBIFGeneric()
		GBIFQueryLayout.__init__(self,parent)
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico",wx.BITMAP_TYPE_ICO))
		self.m_Compass.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/GBIF_compass_small.png",wx.BITMAP_TYPE_PNG))
		self.m_Add.SetBitmapLabel(wx.Image(GenGIS.mainWindow.GetExeDir() + "../plugins/GBIFQuery/green_arrow_down.png",wx.BITMAP_TYPE_PNG).ConvertToBitmap())
		self.m_Add.SetBitmapHover(wx.Image(GenGIS.mainWindow.GetExeDir() + "../plugins/GBIFQuery/green_arrow_down_hover.png",wx.BITMAP_TYPE_PNG).ConvertToBitmap())
		self.m_Remove.SetBitmapLabel(wx.Image(GenGIS.mainWindow.GetExeDir() + "../plugins/GBIFQuery/red_arrow_up.png",wx.BITMAP_TYPE_PNG).ConvertToBitmap())
		self.m_Remove.SetBitmapHover(wx.Image(GenGIS.mainWindow.GetExeDir() + "../plugins/GBIFQuery/red_arrow_up_hover.png",wx.BITMAP_TYPE_PNG).ConvertToBitmap())
		
		self.graphicalElementIds=[]
		self.__selectedTaxon__=set()
		self.__obs__ = []
		self.__conversions__ = []
		self.m_IDList.Clear()
		#fix to expand summary box enough to print two lines of text properly
		self.m_Summary.SetLabel("\n")
		
		#No Map Data
		self.m_AddData.Disable()
		
		#Map Data
		if GenGIS.layerTree.GetNumMapLayers() > 0 :
			self.m_AddData.Enable()
			borders = GenGIS.layerTree.GetMapLayer(0).GetController().GetMapBorders()
			print borders.y1
			print type(borders.y1)
			#Text boxes hate non String types. use int to round, and string to make them fit the container
			self.m_MinLat.SetValue(borders.y1)
			self.m_MaxLat.SetValue(borders.dy)
			self.m_MinLon.SetValue(borders.x1)
			self.m_MaxLon.SetValue(borders.dx)
		
	#	Query GBIF for Taxa in Lat/Lon Boundary
	def OnSearch(self,event):
		wx.BeginBusyCursor()
		
		#	Clear the results list
		self.m_Result.Clear()
		
		taxon = self.m_TaxonName.GetLineText(0)
		taxon=taxon.split()
		if(len(taxon)==0):
			wx.MessageBox("You did not enter a taxon name.")
		else:
			minLatitude= self.m_MinLat.GetValue()
			maxLatitude= self.m_MaxLat.GetValue()
			minLongitude= self.m_MinLon.GetValue()
			maxLongitude= self.m_MaxLon.GetValue()
			self.GBIFSpecific.GETTAXRESULT(taxon,self.m_Result)
		wx.EndBusyCursor()
		
	#	Create Sequence and Location files for selected Taxa
	def OnCalculate(self,event):
		self.m_Summary.SetLabel("\n")
		records,distLocations = 0,0
		self.__obs__=[]
		self.__conversions__=[]
		self.__description__=""
		wx.BeginBusyCursor()
		if(self.__selectedTaxon__):
			minLatitude= int(self.m_MinLat.GetValue())
			maxLatitude= int(self.m_MaxLat.GetValue())
			minLongitude= int(self.m_MinLon.GetValue())
			maxLongitude= int(self.m_MaxLon.GetValue())
			self.m_Progress.WriteText("Starting...\n")
			for tax in self.__selectedTaxon__:
				obs,con,recs,distLocs,description= self.GBIFSpecific.GETOBSENTIRERANGE(tax[1].split(),tax[0],minLatitude,maxLatitude,minLongitude,maxLongitude,self.m_Progress)
				self.__obs__.append(obs)
				self.__conversions__.append(con)
				self.__description__+="%s\n" % description
				records += recs
				distLocations +=distLocs
			self.m_Progress.WriteText("Done.\n")
		else:
			wx.MessageBox("Please select some Taxa.")
		

#		self.m_Summary.SetLabel("%d records retrieved.\n%d distinct locations." %(records,distLocations))
		summaryText = ("%d records retrieved.\n%d distinct locations." %(records,distLocations))
		f = self.m_Summary.GetFont()
		dc = wx.WindowDC(self.m_Summary)
		dc.SetFont(f)
		aSize = dc.GetMultiLineTextExtent(summaryText)
		aSize = wx.Size(aSize[0],aSize[1])
		self.m_Summary.SetSize(aSize)
		
		self.m_Summary.SetLabel(summaryText)
		wx.EndBusyCursor()
	
	#	Present the number of locations a user is about to query
	#	Used as a check by the user to know they aren't going to produce way too much data.
	def OnPreCalculate(self,event):
		wx.BeginBusyCursor()
		self.m_Progress.WriteText("Retrieving record counts.\n")
		self.m_Summary.SetLabel("\n")
		if(self.__selectedTaxon__):
			minLatitude= int(self.m_MinLat.GetValue())
			maxLatitude= int(self.m_MaxLat.GetValue())
			minLongitude= int(self.m_MinLon.GetValue())
			maxLongitude= int(self.m_MaxLon.GetValue())
			count=0
			for tax in self.__selectedTaxon__:
				count+=self.GBIFSpecific.GETCOUNT(tax[1].split(),tax[0],minLatitude,maxLatitude,minLongitude,maxLongitude,self.m_Progress)
		else:
			wx.MessageBox("Please select some Taxa.")
		self.m_Summary.SetLabel("There were %d records for the given location." % count) 
		wx.EndBusyCursor()
	#	Redirects User to Wiki page for this plugin
	def OnHelp(self, event):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/Description_of_GenGIS_plugins#GBIF_Query' )
	
	#	Adds Data to GenGIS
	def OnAddData(self,event):
		if (len(self.__obs__) > 0):
			OUTLText, OUTSText = self.GBIFGeneric.GETTEXT(self.__obs__,self.__conversions__)
			OUTLArray=self.GBIFGeneric.CPPOUT(OUTLText)
			OUTSArray=self.GBIFGeneric.CPPOUT(OUTSText)
			OUTLArray.insert(0,"Site ID,Latitude,Longitude,Richness,Cell ID")
			OUTSArray.insert(0,"Sequence ID,Site ID,CellLat,CellLong,Taxon,Genus,TrueLat,TrueLong,Count,AllRecords")					
			OUTLArray.pop()
			OUTSArray.pop()
			layerName = "GBIFLayer_%d" % GenGIS.layerTree.GetNumLocationLayers()
			GenGIS.mainWindow.OpenLocationsCSVFile(OUTLArray, layerName)
			GenGIS.mainWindow.OpenSequenceCSVFile(OUTSArray, layerName)
			
			#Get the number of last location layer added (the gbif one)
			numLocationLayers=GenGIS.layerTree.GetNumLocationSetLayers()
			locationSetLayer = GenGIS.layerTree.GetLocationSetLayer(numLocationLayers-1)
			locationSetLayer.SetDescription(self.__description__)
			
		else:
			wx.MessageBox("Please make a successful GBIF Query first.")
		

	#	Exports Location and Sequence Data to a location of the users choice
	def OnExportData(self,event):
		if (len(self.__obs__) > 0):
			fileTypes = 'Loc and Seq Files (*.csv)|*.csv'
			dlg = wx.FileDialog(self, "Save plot", "", "", fileTypes, wx.SAVE)
			if dlg.ShowModal()==wx.ID_OK:
				filename =	dlg.GetFilename()
				dir = dlg.GetDirectory()
				file_split = filename.split(".",1)
				#creates the directories
				OUTLfile = ("%s/%s_locs.csv" % (dir,file_split[0]))				
				OUTSfile = ("%s/%s_seqs.csv" % (dir,file_split[0]))
				OUTDfile = ("%s/%s_source.txt" % (dir,file_split[0]))
				OUTLText, OUTSText = self.GBIFGeneric.GETTEXT(self.__obs__,self.__conversions__)
				self.GBIFGeneric.WRITEEXPORT(OUTLfile,OUTLText,"Site ID,Latitude,Longitude,Richness,Cell ID\n")
				self.GBIFGeneric.WRITEEXPORT(OUTSfile,OUTSText,"Sequence ID,Site ID,CellLat,CellLong,Taxon,Genus,TrueLat,TrueLong,Count,AllRecords\n")
				description = self.__description__.encode('utf-8')
				self.GBIFGeneric.WRITEEXPORT(OUTDfile,description,"")
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
			if (int(split[0]),split[1]) not in self.__selectedTaxon__:
				self.m_IDList.InsertItems(["%s" % selected],IDCount+i)
				self.__selectedTaxon__.add((int(split[0]),split[1]))
				i+=1
			
	#	Remove Data from ID List
	def OnRemove(self,event):
		
		#	NEED TO SORT ON DECREASING ORDER
		print self.m_IDList.GetSelections()
		candidates = sorted(self.m_IDList.GetSelections(),reverse=True)
		print candidates
	#	for index in self.m_IDList.GetSelections():
		for index in candidates:
			print index
			selected = self.m_IDList.GetString(index)
			split = selected.split(" | ")
			self.__selectedTaxon__.remove((int(split[0]),split[1]))
			self.m_IDList.Delete(index)
	
	#	Close the Plugin
	def OnClose(self, event):
		# remove plotted lines
		for id in self.graphicalElementIds:
			GenGIS.graphics.RemoveLine(id)

		GenGIS.viewport.Refresh()
		event.Skip()
	
	#	Close the Plugin
	def OnOK( self, event ):
		self.Close()
	