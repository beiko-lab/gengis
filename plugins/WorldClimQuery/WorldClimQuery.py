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

from WorldClimQueryLayout import WorldClimQueryLayout
from WorldClimQueryLayout import DescriptionLayout
import GenGIS
import wx
import pybioclim

class WorldClimQuery( WorldClimQueryLayout ):
	
	fileTranslations = {
	"Annual Mean Temperature" : "BIO1",
	"Mean Diurnal Range (Mean of monthly (max temp - min temp))" : "BIO2",
	"Isothermality (BIO2/BIO7) (* 100)" : "BIO3",
	"Temperature Seasonality (standard deviation *100)" : "BIO4",
	"Max Temperature of Warmest Month" : "BIO5",
	"Min Temperature of Coldest Month" : "BIO6",
	"Temperature Annual Range (BIO5-BIO6)" : "BIO7",
	"Mean Temperature of Wettest Quarter" : "BIO8",
	"Mean Temperature of Driest Quarter" : "BIO9",
	"Mean Temperature of Warmest Quarter" : "BIO10",
	"Mean Temperature of Coldest Quarter" : "BIO11",
	"Annual Precipitation" : "BIO12",
	"Precipitation of Wettest Month" : "BIO13",
	"Precipitation of Driest Month" : "BIO14",
	"Precipitation Seasonality (Coefficient of Variation)" : "BIO15",
	"Precipitation of Wettest Quarter" : "BIO16",
	"Precipitation of Driest Quarter" : "BIO17",
	"Precipitation of Warmest Quarter" : "BIO18",
	"Precipitation of Coldest Quarter" : "BIO19"
	}
	
	def __init__(self, parent=None):
		WorldClimQueryLayout.__init__ ( self, parent )
		self.about = Description()
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		# check if projections exist. UTM format with no projection is not convertible
		# into Lat/Lon coordinates, which are needed for Pybioclim
		geographic = GenGIS.StudyController.IsUsingGeographic(GenGIS.study.GetController())
		projected = GenGIS.StudyController.IsUsingProjection(GenGIS.study.GetController())
		if(not (geographic or projected)):
			wx.MessageBox("The current map has an unrecognised projection. Unfortunately at this time UTM data can not be used with this plugin if the map's projection is unknown.","Improper Coordinate System")
			self.Close()
			return
		
		# check required data has been loaded (doesn't need sequences)
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0:
			wx.MessageBox("This plugin requires map and location data to be loaded.", "Additional data required.")
			self.Close()
			return
		
		#populate measure wxChoice
		for key in sorted( self.fileTranslations.iterkeys() ):
			self.m_Measures.Append(key)
		self.m_Measures.SetSelection(0)
		
		file = self.fileTranslations[ self.m_Measures.GetStringSelection() ]
		pybioclim.get_dataset( file.lower() )
		
		#get descriptions for maps
		desc = {}
		desc = pybioclim.metadata['%s.bil' %file.lower()] 
		self.UpdateDescription( file.lower(), desc )
		
		
	
	# needs to change all of the details in accordance to which measure was picked
	def OnMeasureChanged(self, event):
		file = self.fileTranslations[ self.m_Measures.GetStringSelection() ]
		pybioclim.get_dataset( file.lower() )
		
		#get descriptions for maps
		desc = {}
		desc = pybioclim.metadata['%s.bil' %file.lower()] 
		self.UpdateDescription( file.lower(), desc )

	
	def OnCalculate(self, event):
		wx.BeginBusyCursor()
		
		self.txtLog.AppendText('Retrieving ' + self.m_Measures.GetStringSelection() + ' data...\n')
		
		activeLocLayers = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
		lat_lon = []
		for locLayer in activeLocLayers:
			if 'Latitude' in locLayer.GetController().GetData().keys():
				lat = float(locLayer.GetController().GetData()['Latitude'])
				lon = float(locLayer.GetController().GetData()['Longitude'])
			
			else:
				lon = locLayer.GetController().GetEasting()
				lat = locLayer.GetController().GetNorthing()
				geo = GenGIS.GeoCoord(lon,lat)
				point = GenGIS.Point3D()
				GenGIS.layerTree.GetMapLayer(0).GetController().GeoToGrid( geo, point)
				GenGIS.layerTree.GetMapLayer(0).GetController().GridToGeo( point, geo)
				lon = geo.longitude;
				lat = geo.latitude;
				
				
			lat_lon.append( (lat,lon) )
		# GETS SOME FORM OF DATA
		dataSet = self.fileTranslations[ self.m_Measures.GetStringSelection() ]
		dat = pybioclim.get_dataset(dataSet)
		array = dat.ReadAsArray()
		values = pybioclim.get_values(dataSet,lat_lon)
		metadata = []
		for val in values:
			index = values.index(val)
			tem = str(val)
			metadata.append(tem)
			self.txtLog.AppendText('For ' + activeLocLayers[index].GetName() + ' adding data: ' + tem + '.\n')
			
		# ADD DATA BACK TO LOCATIONS METADATA
		GenGIS.layerTree.GetLocationSetLayer(0).GetController().AddMetadata( str(self.m_Measures.GetStringSelection()) , metadata )
		
		self.txtLog.AppendText('\n' + self.m_Measures.GetStringSelection() + ' results added to location metadata table as "' + str(self.m_Measures.GetStringSelection()) + '".\n\n')
		
		wx.EndBusyCursor()
	
	def OnOK(self, event):
		self.Close()
	
	
	def OnHelp(self, event):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/Description_of_GenGIS_plugins#Environmental_Data_Visualizer' )
		
	def UpdateDescription(self, file, metadata ):
		name = file
		description = ""
		for key in sorted(metadata.iterkeys()):
			value = metadata[key]
			string = "%s : %s\n" %(key,value)
			description = "%s%s" %(description,string)
		
		self.txtFullName.SetValue(file)
		self.txtDescription.SetValue(description)
	
	def OnAbout(self, event):
		self.about.Show()
		
class Description( DescriptionLayout ):
	def __init__(self):
		DescriptionLayout.__init__(self,None)
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico",wx.BITMAP_TYPE_ICO))
		
		text = "This plugin uses Bioclim data retrieved from WorldClim.org and applys it to location points loaded in GenGIS. Granularity of Bioclim data maps to 0.083 latitude by 0.083 longitude, locations closer than this will map to the same environmental data. For more information about Bioclim data or WorldClim, please visit www.worldclim.org."
		self.m_description.SetLabel(text)
		self.m_description.Wrap(250)
		
	def OnClose(self,event):
		self.Hide()
	