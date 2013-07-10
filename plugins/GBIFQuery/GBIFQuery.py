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

class GBIFQuery(GBIFQueryLayout):
	#	Global variables to store queried information
	__obs__ = []
	__conversions__ = []
	__selectedTaxon__= []
	__description__=""
	
	def __init__(self,parent=None):
#		import pdb; pdb.set_trace()
		self.GBIFSPECIFIC = GBIFSpecific()
		self.GBIFGENERIC = GBIFGeneric()
		GBIFQueryLayout.__init__(self,parent)
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico",wx.BITMAP_TYPE_ICO))
		self.m_bitmap3.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/GBIF_compass_small.png",wx.BITMAP_TYPE_PNG))
		self.graphicalElementIds=[]
		self.__selectedTaxon__=[]
		self.__obs__ = []
		self.__conversions__ = []
		self.m_IDList.Clear()
		#No Map Data
		self.m_AddData.Disable()
		#Map Data
		if GenGIS.layerTree.GetNumMapLayers() > 0 :
			self.m_AddData.Enable()
			borders = GenGIS.layerTree.GetMapLayer(0).GetController().GetMapBorders()
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
		#TEST  CASES REMOVE BEFORE LAUNCH
		if(taxon=="TEST"):
			taxon="Liolaemus darwinii"
		#	self.m_MinLat.SetValue(-34)
		#	self.m_MaxLat.SetValue(-31)
		#	self.m_MinLon.SetValue(-72)
		#	self.m_MaxLon.SetValue(-66)
		elif(taxon=="TE"):
			taxon="Apolochiton"
			self.m_MinLat.SetValue(-34)
			self.m_MaxLat.SetValue(-31)
			self.m_MinLon.SetValue(-72)
			self.m_MaxLon.SetValue(-66)
		elif(taxon=="Galaxiidae"):
			taxon="Galaxiidae"
			self.m_MinLat.SetValue(-42)
			self.m_MaxLat.SetValue(-26)
			self.m_MinLon.SetValue(127)
			self.m_MaxLon.SetValue(167)
		taxon=taxon.split()
		if(len(taxon)==0):
			wx.MessageBox("You did not enter a taxon name.")
		else:
			minLatitude= self.m_MinLat.GetValue()
			maxLatitude= self.m_MaxLat.GetValue()
			minLongitude= self.m_MinLon.GetValue()
			maxLongitude= self.m_MaxLon.GetValue()
			self.GBIFSPECIFIC.GETTAXRESULT(taxon,self.m_Result)
		wx.EndBusyCursor()
		
	#	Create Sequence and Location files for selected Taxa 		
	def OnCalculate(self,event):
		self.m_staticText6.SetLabel("\n")
		records,distLocations = 0,0
		self.__obs__=[]
		self.__conversions__=[]
		wx.BeginBusyCursor()
		if(self.__selectedTaxon__):
			minLatitude= self.m_MinLat.GetValue()
			maxLatitude= self.m_MaxLat.GetValue()
			minLongitude= self.m_MinLon.GetValue()
			maxLongitude= self.m_MaxLon.GetValue()
			self.m_Progress.WriteText("Starting...\n")
			for tax in self.__selectedTaxon__:
				obs,con,recs,distLocs,description= self.GBIFSPECIFIC.GETOBSENTIRERANGE(tax.split(),minLatitude,maxLatitude,minLongitude,maxLongitude,self.m_Progress)
				self.__obs__.append(obs)
				self.__conversions__.append(con)
				self.__description__+="%s\n" % description
				records += recs
				distLocations +=distLocs
			self.m_Progress.WriteText("Done.\n")
		else:
			wx.MessageBox("Please select some Taxa.")
		
		self.m_staticText6.SetLabel("%d records retrieved.\n%d distinct locations." %(records,distLocations))
		wx.EndBusyCursor()
	
	#	Present the number of locations a user is about to query
	#	Used as a check by the user to know they aren't going to produce way too much data.
	def OnPreCalculate(self,event):
		self.m_staticText6.SetLabel("\n")
		if(self.__selectedTaxon__):
			minLatitude= self.m_MinLat.GetValue()
			maxLatitude= self.m_MaxLat.GetValue()
			minLongitude= self.m_MinLon.GetValue()
			maxLongitude= self.m_MaxLon.GetValue()
			count=0
			for tax in self.__selectedTaxon__:
				count+=self.GBIFSPECIFIC.GETCOUNT(tax.split(),minLatitude,maxLatitude,minLongitude,maxLongitude,self.m_Progress)
		else:
			wx.MessageBox("Please select some Taxa.")
		self.m_staticText6.SetLabel("There were %d records for the given location." % count) 
	
	#	Redirects User to Wiki page for this plugin
	def OnHelp(self, event):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/Description_of_GenGIS_plugins#Linear_Regression' )
	
	#	Adds Data to GenGIS
	def OnAddData(self,event):
		if (len(self.__obs__) > 0):
			OUTLText, OUTSText = self.GBIFGENERIC.GETTEXT(self.__obs__,self.__conversions__)
			OUTLArray=self.GBIFGENERIC.CPPOUT(OUTLText)
			OUTSArray=self.GBIFGENERIC.CPPOUT(OUTSText)
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
				OUTLText, OUTSText = self.GBIFGENERIC.GETTEXT(self.__obs__,self.__conversions__)
				self.GBIFGENERIC.WRITEEXPORT(OUTLfile,OUTLText,"Site ID,Latitude,Longitude,Richness,Cell ID\n")
				self.GBIFGENERIC.WRITEEXPORT(OUTSfile,OUTSText,"Sequence ID,Site ID,CellLat,CellLong,Taxon,Genus,TrueLat,TrueLong,Count,AllRecords\n")
				description = self.__description__.encode('utf-8')
				self.GBIFGENERIC.WRITEEXPORT(OUTDfile,description,"")
			dlg.Destroy()
		else:
			wx.MessageBox("Please make a successful GBIF Query first.")
	
	#	Add Data from Results Table to ID List
	def OnAdd(self,event):
		i=0
		IDCount = self.m_IDList.GetCount()
		for index in self.m_Result.GetSelections():
			selected = self.m_Result.GetString(index)
			self.m_IDList.InsertItems(["%s" % selected],IDCount+i)
			split = selected.split(" | ")
			self.__selectedTaxon__.append(split[1])
			i+=1
			
	#	Remove Data from ID List
	def OnRemove(self,event):
		for index in self.m_IDList.GetSelections():
			selected = self.m_IDList.GetString(index)
			split = selected.split(" | ")
			self.__selectedTaxon__.remove(split[1])
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