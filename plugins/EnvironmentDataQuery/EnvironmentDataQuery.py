from EnvironmentDataQueryLayout import EnvironmentDataQueryLayout
import GenGIS
import wx
import pybioclim

class EnvironmentDataQuery( EnvironmentDataQueryLayout ):
	
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
		EnvironmentDataQueryLayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
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
			lat = float(locLayer.GetController().GetData()['Latitude'])
			lon = float(locLayer.GetController().GetData()['Longitude'])
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