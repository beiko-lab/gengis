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


class GBIFQuery(GBIFQueryLayout):
	#	Global variables to store queried information
	__obs__ = []
	__conversions__ = []
	__selectedTaxon__= []

	def __init__(self,parent=None):
#		import pdb; pdb.set_trace()
		GBIFQueryLayout.__init__(self,parent)
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico",wx.BITMAP_TYPE_ICO))
		
		self.graphicalElementIds=[]
		
	#	Query GBIF for Taxa in Lat/Lon Boundary
	def OnSearch(self,event):
		wx.BeginBusyCursor()
		#	Clear the results list
		self.m_Result.Clear()
		
		taxon = self.m_TaxonName.GetLineText(0)
		if(taxon=="TEST"):
			taxon="Liolaemus darwinii"
			self.m_MinLat.SetValue(-34)
			self.m_MaxLat.SetValue(-31)
			self.m_MinLon.SetValue(-72)
			self.m_MaxLon.SetValue(-66)
		elif(taxon=="TE"):
			taxon="Apolochiton"
			self.m_MinLat.SetValue(-34)
			self.m_MaxLat.SetValue(-31)
			self.m_MinLon.SetValue(-72)
			self.m_MaxLon.SetValue(-66)
		taxon=taxon.split()
		if(len(taxon)==0):
			wx.MessageBox("You did not enter a taxon name.")
		else:
			minLatitude= self.m_MinLat.GetValue()
			maxLatitude= self.m_MaxLat.GetValue()
			minLongitude= self.m_MinLon.GetValue()
			maxLongitude= self.m_MaxLon.GetValue()
			self.GETTAXRESULT(taxon)
		wx.EndBusyCursor()
		
	#	Create Sequence and Location files for selected Taxa 		
	def OnCalculate(self,event):
		self.m_staticText6.SetLabel("\n")
		records,distLocations = 0,0
		self.__obs__=[]
	#	wx.MessageBox("Go")
		wx.BeginBusyCursor()
		if(self.__selectedTaxon__):
			minLatitude= self.m_MinLat.GetValue()
			maxLatitude= self.m_MaxLat.GetValue()
			minLongitude= self.m_MinLon.GetValue()
			maxLongitude= self.m_MaxLon.GetValue()
		#	wx.MessageBox("%s %s %s %s" %(minLatitude,maxLatitude,minLongitude,maxLongitude))
			self.m_Progress.WriteText("Starting...\n")
			for tax in self.__selectedTaxon__:
				obs,con,recs,distLocs= self.GETOBS(tax.split(),minLatitude,maxLatitude,minLongitude,maxLongitude)
				self.__obs__.append(obs)
				self.__conversions__.append(con)
				records += recs
				distLocations +=distLocs
		#	print ("%d\n" % len(self.__obs__.keys()))
			self.m_Progress.WriteText("Done.\n")
		else:
			wx.MessageBox("Please select some Taxa.")
		
		self.m_staticText6.SetLabel("%d records retrieved.\n%d distinct locations." %(records,distLocations))
		wx.EndBusyCursor()
	
	#	Possibly present the number of records they are about to query.
	#	Used as a check by the user to know they aren't going to produce way too much data.
	def OnPreCalculate(self,event):
		wx.MessageBox("PreGo")

	#	Redirects User to Wiki page for this plugin
	def OnHelp(self, event):
		wx.MessageBox("Help!Help! I'm being oppressed!")
	
	#	Adds Data to GenGIS
	def OnAddData(self,event):
	#	GenGIS.layerTree.GetLocationSetLayer(0).GetController().GetLocationSetFromPython("thisisastringname")
	
	#	NEEDS TO ASK FOR NAME TO GIVE TO GENGIS
		if (len(self.__obs__) > 0):
			OUTLText, OUTSText = self.GETTEXT(self.__obs__,self.__conversions__)
			OUTLArray=self.CPPOUT(OUTLText)
			OUTSArray=self.CPPOUT(OUTSText)
			OUTLArray.insert(0,"Site ID,Latitude,Longitude,Richness")
			OUTSArray.insert(0,"Sequence ID,Site ID,CellLat,CellLong,Taxon,Genus,TrueLat,TrueLong,Count,AllRecords")					
#			GenGIS.layerTree.GetLocationSetLayer(0).GetController().GetLocationSetFromPython("thisisastringname")
			GenGIS.MainWindowWrapper.OpenLocationsCSVFile(OUTSArray, "thisisastringname")
			
		else:
			wx.MessageBox("Please make a successful GBIF Query first.")
		

	#	Exports Location and Sequence Data to a location of the users choice
	def OnExportData(self,event):
#		print ("%d\n" % len(self.__obs__.keys()))
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
				
	#			OUTLText, OUTSText = self.MAKEOUTS(self.__obs__,self.__conversions__)
				OUTLText, OUTSText = self.GETTEXT(self.__obs__,self.__conversions__)
				OUTL=open(OUTLfile,'w')
				OUTS=open(OUTSfile,'w')
				OUTL.write("Site ID,Latitude,Longitude,Richness\n")
				OUTS.write("Sequence ID,Site ID,CellLat,CellLong,Taxon,Genus,TrueLat,TrueLong,Count,AllRecords\n")	
				OUTL.write(OUTLText)
				OUTS.write(OUTSText)
				OUTL.close()
				OUTS.close()
			dlg.Destroy()
		else:
			wx.MessageBox("Please make a successful GBIF Query first.")
	
	#	Add Data from Results Table to ID List
	def OnAdd(self,event):
		#get highlited from Results Table
		#add to __selectedObs__
		#add to IDList
		i=0
		IDCount = self.m_IDList.GetCount()
		for index in self.m_Result.GetSelections():
			selected = self.m_Result.GetString(index)
			self.m_IDList.InsertItems(["%s" % selected],IDCount+i)
			split = selected.split(" | ")
			self.__selectedTaxon__.append(split[1])
#			print self.__selectedTaxon__
			i+=1
			
	#	Remove Data from ID List
	def OnRemove(self,event):
		for index in self.m_IDList.GetSelections():
			selected = self.m_IDList.GetString(index)
			split = selected.split(" | ")
			self.__selectedTaxon__.remove(split[1])
#			print self.__selectedTaxon__
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
		
		
##########################################		
#	HERES LIES THE CORE FUNCTIONS OF THE GBIF QUERY		
#	WITH GREAT POWER COMES GREAT RESPONSIBILITY
##########################################

##########
#	GENERIC
##########	

	def GETTEXT (self,obs_list,conversions_list):
		OUTL=""
		OUTS=""
		for obs,convs in zip(obs_list,conversions_list):
	#		print len(obs_list)
	#		print len(conversions_list)
	#		import pdb; pdb.set_trace()
			locs, seqs = self.MAKEOUTS(obs,convs)
			OUTL+=locs
			OUTS+=seqs
		return(OUTL,OUTS)
	#	Transforms the mined data into text to be output
	def MAKEOUTS (self,obs,conversions):
		OUTLTEXT=""
		OUTSTEXT=""
		seqFileAgg = {}
		for cellOut in sorted(obs.keys()):
			if len(obs[cellOut].keys()) > 0:
				OUTLTEXT += ("%d,%f,%f,%d\n" % (cellOut, conversions[cellOut][0] + 0.5, conversions[cellOut][1] +0.5, len(obs[cellOut].keys()) ))  
				for taxOut in sorted(obs[cellOut].keys()):
					thisList=obs[cellOut][taxOut]
					for ent in thisList:
						toKey = "%d,%f,%f,%s,%s,%s,%s" %(cellOut, conversions[cellOut][0],conversions[cellOut][1],ent[3],taxOut,ent[1],ent[2])
						toKey = re.sub(r'\<.*?\>','',toKey)
						try:
							seqFileAgg[toKey].extend([ent[0]])
						except KeyError:
							seqFileAgg[toKey]=[ent[0]]
		seqFileAgg_items = seqFileAgg.items()
		seqFileAgg_items.sort(key=lambda x: x)
		for outME,IDlist in seqFileAgg_items:
			OUTSTEXT += ("%d,%s,%d,%s\n" %(IDlist[0],outME,len(IDlist),'|'.join(str(i) for i in IDlist)))
		return(OUTLTEXT,OUTSTEXT)
				
	# Populate the Results Table using Taxa and Geographic boundaries
	
	def CPPOUT (self,input):
		array = input.split("\n")
	#	print array[0]
		return (array)
##############
#	GBIF SPECIFIC
##############

	def GETTAXRESULT(self,taxon_name):
		taxonReq = '+'.join(taxon_name)

		taxonList=()
		FAIL = 0
		try:
			taxonList = [line.strip() for line in open(taxonReq)]
		except IOError:
			FAIL=1
		if(FAIL==1):
			taxonReq_2 = taxonReq+".txt"
			FAIL=0
			try:
				taxonList = [line.strip() for line in open(taxonReq_2)]
			except IOError:
				FAIL=1
		if(FAIL==1):
			taxonList=[taxonReq]
		obs={}
				
		for taxonName in taxonList:	
			pos=0
			url="http://data.gbif.org/ws/rest/taxon/list?scientificname="+taxonName+"*&dataproviderkey=1&dataresourcekey=1"
			try:	
				html=urllib2.urlopen(url)
			except urllib2.HTTPError as e:
				wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
				self.Close()
			taxResponse=html.read()
			taxxmldoc=minidom.parseString(taxResponse)
			taxNodeList=taxxmldoc.getElementsByTagName("tc:TaxonConcept")
		
			if (len(taxNodeList) > 0):
				for taxResult in taxNodeList:
					#string = taxResult.toprettyxml(indent=' ')
					string = taxResult.toxml()
					key_string = re.search('gbifKey="(\d+)"',string)
					id_string=re.search('\d+',key_string.group())
					id = int(id_string.group())
					name=self.Strip(string,"<tn:nameComplete>.*")
					rank=self.Strip(string,"<tn:rankString>.*")
					according=self.Strip(string,"<tc:accordingToString>.*")
					self.m_Result.InsertItems(["%d | %s | %s | %s"%(id,name,rank,according)],pos)
					pos+=1
			else:
				wx.MessageBox("No Tax Found.")
				return(-1)
			return(1)
	
	#	Get smallest TAX ID. 
	def	Strip (self,Node,tag):
		name = re.search(('%s' % tag),Node)
		name=name.group()
		name = re.sub(r'<.*?\>','',name)
		name = re.sub(r'[^A-Za-z ]+','',name)
		name = re.sub(r' *$','',name)
		return name
	
	def GETTAXID (self,taxonName):
		taxConceptID=100000000
		self.m_Progress.WriteText("########### Querying GBIF for %s ######\n" % taxonName)
		url="http://data.gbif.org/ws/rest/taxon/list?scientificname="+taxonName+"*&dataproviderkey=1&dataresourcekey=1"
		try:	
			html=urllib2.urlopen(url)
		except urllib2.HTTPError as e:
			self.m_Progress.WriteText("%s\n" % e.code)
		#	print e.read()
			wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
			self.Close()
		taxResponse=html.read()
		###############
		#	Need a handler for if taxonName is not found
		###############
		taxxmldoc=minidom.parseString(taxResponse)
		taxNodeList=taxxmldoc.getElementsByTagName("tc:TaxonConcept")
		for taxResult in taxNodeList:
			string = taxResult.toprettyxml(indent=' ')
			key_string = re.search('gbifKey="(\d+)"',string)
			if key_string:
				id_string=re.search('\d+',key_string.group())
				id = int(id_string.group())
				if id < taxConceptID:
					taxConceptID = id
		if taxConceptID == 100000000:
	#		sys.exit("No frigging taxon concept found")
			return(-1)
		return(taxConceptID)
	
	#	Create observation from TAXID
	def GETOBS(self,taxon_name,minLatitude,maxLatitude,minLongitude,maxLongitude):
		records=0
		distLocations=0
		conversions = {}
#		print "#################"
#		print taxon_name
#		print "#################"
		taxonReq = '+'.join(taxon_name)
		####################################
		#	check if text file or command input
		#	if (ends in .txt) then text
		#	if ! then append .txt and check
		####################################
		taxonList=()
		FAIL = 0
		try:
			taxonList = [line.strip() for line in open(taxonReq)]
		except IOError:
			FAIL=1
		if(FAIL==1):
			taxonReq_2 = taxonReq+".txt"
			FAIL=0
			try:
				taxonList = [line.strip() for line in open(taxonReq_2)]
			except IOError:
				FAIL=1
		if(FAIL==1):
			taxonList=[taxonReq]
		obs={}
		for taxonName in taxonList:
			
			cID = self.GETTAXID(taxonName)
			if(cID == -1):
				self.m_Progress.WriteText("No concept ID found for %s. Skipping...\n" % taxonName)
				continue
			self.m_Progress.WriteText("Concept ID: %d\n" % cID)
			#print("Concept ID \n",cID)
			
			### The set of observations
			### hash key #1 = grid cell
			### hash #2 = scientific name
			### value = list of GBIF key + list of lat/longs of genus observations in that grid cell

			fullTaxonomy={}

			for lat in range(minLatitude,maxLatitude):
				for lon in range(minLongitude,maxLongitude):
					currGrid= (lat+90)*360 + (lon +180)
					conversions[currGrid]=	[lat,lon]
					
					self.m_Progress.WriteText("Latitude: %d\tLongitude\t%d\tGrid ID: %d\t |" % (lat,lon,currGrid))
					#print("Latitude: ",lat,"\tLongitude\t",lon,"\tGrid ID: ",currGrid,"\t |")
					url= "http://data.gbif.org/ws/rest/occurrence/list?taxonconceptkey=%d&cellid=%d&coordinatestatus=true&coordinateissues=false&format=brief" % (cID,currGrid) 
					response=urllib2.urlopen(url).read()
					parser=minidom.parseString(response)
					nodeList=parser.getElementsByTagName("to:TaxonOccurrence")
					if len(nodeList)==1000:
						nodeList=()
						for j in range(0,100):
							url = "http://data.gbif.org/ws/rest/occurrence/list?taxonconceptkey=%d&cellid=%d&coordinatestatus=true&coordinateissues=false&format=brief" % (cID,currGrid)
							
							for k in range(0,9):
								url+="&centicellid="
								url+=j+k
							response=urllib2.urlopen(url).read()
							parser=minidom.parseString(response) 
							temp=parser.getElementsByTagName("to:TaxonOccurrence")
							if len(temp)==1000:
								temp=()
								for jj in range(j,j+10):
									url = "http://data.gbif.org/ws/rest/occurrence/list?taxonconceptkey=$cID&cellid=%d&coordinatestatus=true&coordinateissues=false&format=brief&centicellid=%d" %(cID,currGrid,jj)
									self.m_Progress.WriteText("\t\t%s\n" % url)
									#print("\t\t%s\n" % url)
									response=urllib2.urlopen(url).read()
									parser=minidom.parseString(response)
									temper=parser.getElementsByTagName("to:TaxonOccurrence")
									self.m_Progress.WriteText("\t\tSubsubset $d: $d records found\n" %(jj,len(temper)))
									#print("\t\tSubsubset $d: $d records found\n" %(jj,len(temper)))
									temp.append(temper)
							self.m_Progress.WriteText("\n\tSubset $jj: %d records found.\n\n" % temp)
							#print("\n\tSubset $jj: %d records found.\n\n" % temp)
							nodeList.append(temp)		
					self.m_Progress.WriteText("%d records found\n" % len(nodeList))
					if len(nodeList) > 0:
						records += len(nodeList)
						distLocations += 1
					#print("%d records found\n" % len(nodeList))
					for node in nodeList:
						string = node.toprettyxml(indent=' ')
						rID=-1
						key_string = re.search('gbifKey="(\d+)"',string)
						if key_string:
							id_string=re.search('\d+',key_string.group())
							id = int(id_string.group())
							rID=id
						else:
							sys.exit("Could not find a friggin GBIF key")
						name=node.getElementsByTagName("tn:nameComplete")[0].toxml()
						name = re.sub(r'<.*?\>','',name)
						name = re.sub(r'[^A-Za-z ]+','',name)
						name = re.sub(r' *$','',name)
						genus = re.sub(r' .*','',name.lower())
						lat_tem = node.getElementsByTagName("to:decimalLatitude")[0].toxml()
						long_tem=node.getElementsByTagName("to:decimalLongitude")[0].toxml()
						try:
							obs[currGrid][genus].extend([(rID,lat_tem,long_tem,name)])
						except KeyError:
							try:
								obs[currGrid].update({genus: [(rID,lat_tem,long_tem,name)] })
							except KeyError:
								obs[currGrid] = {genus: [(rID,lat_tem,long_tem,name)] }
		return(obs,conversions,records,distLocations)

