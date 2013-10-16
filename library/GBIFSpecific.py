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
import pdb
import re
import wx
import sys
from GBIFGeneric import GBIFGeneric
import urllib2
from xml.dom import minidom
from decimal import Decimal

class GBIFSpecific:
	# get source of data sets as well as rights and citation
	__description__ = set()
	__warnings__=[0,0]
	__uniqueNodeList__=set()
	__MAXLATITUDE__,__MINLATITUDE__,__MAXLONGITUDE__,__MINLONGITUDE__ = 90,-90,180,-180
	
	def __init__(self):
		self.GBIFGeneric = GBIFGeneric()
		self.__description__ = set()
		self.__warnings__=[0,0]
		self.__uniqueNodeList__=set()
		
	def GETRIGHTS(self,fh):
		description = set()
		resource=fh.getElementsByTagName("gbif:dataResources")
		for node in resource:
			desc=""
			name = node.getElementsByTagName("gbif:name")
			rights = node.getElementsByTagName("gbif:rights")
			citation = node.getElementsByTagName("gbif:citation")
			desc+="##########################\n"
			for tem in name:
				desc+= "Name\n"
				desc+= re.sub(r'<.*?\>','',tem.toprettyxml(' '))
			for tem in rights:
				desc+= "\nRights\n"
				desc+= re.sub(r'<.*?\>','',tem.toprettyxml(' '))
			for tem in citation:
				desc+= "\nCitation\n"
				desc+= re.sub(r'<.*?\>','',tem.toprettyxml(' '))
			desc+="\n"
			description.add(desc)
		return(description)

	#	Queries GBIF to find the number of results for given boundary 
	def GETCOUNT(self,taxon_name,cID,minLat,maxLat,minLon,maxLon,m_Progress):
		taxonReq = '+'.join(taxon_name)			
		url= " http://data.gbif.org/ws/rest/occurrence/count?taxonconceptkey=%d&maxlatitude=%f&minlatitude=%f&maxlongitude=%f&minlongitude=%f" % (cID,maxLat,minLat,maxLon,minLon) 
		try:	
			response=urllib2.urlopen(url).read()
		except urllib2.HTTPError as e:
			wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
			self.Close()
		key_string = re.search('gbif:summary totalMatched="(\d+)"',response)
		count_string=re.search('\d+',key_string.group())
		count = int(count_string.group())
		return(count)
			
	def GETTAXRESULT(self,taxon_name,queryLayout):
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
		resultList=[]	
		for taxonName in taxonList:	
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
					string = taxResult.toxml()
					key_string = re.search('gbifKey="(\d+)"',string)
					id_string=re.search('\d+',key_string.group())
					id = int(id_string.group())
					name=self.Strip(string,"<tn:nameComplete>.*")
					rank=self.Strip(string,"<tn:rankString>.*")
					according=self.Strip(string,"<tc:accordingToString>.*")
					resultList.append("%d | %s | %s | %s"%(id,name,rank,according))
			else:
				wx.MessageBox("No Tax Found.")
				return(-1)
			return resultList
	
	#	Get smallest TAX ID. 
	def	Strip (self,Node,tag):
		name = re.search(('%s' % tag),Node)
		name=name.group()
		name = re.sub(r'<.*?\>','',name)
		name = re.sub(r'[^A-Za-z ]+','',name)
		name = re.sub(r' *$','',name)
		return name
		
	# Search for a Taxon Name in a Geographic boundary
	def GETOBSENTIRERANGE(self,taxon_name,cID,minLatitude,maxLatitude,minLongitude,maxLongitude,m_Progress):
		self.__description__ = set()
		self.__warnings__=[0,0]
		self.__uniqueNodeList__=set()
		warningsFlag = 0
		records=0
		distLocations=set()
		taxonReq = '+'.join(taxon_name)
		####################################
		#	check if text file or command input
		#	if (ends in .txt) then text
		#	if ! then append .txt and check
		####################################
		taxonList=()
		FAIL = 0
		taxonList=[taxonReq]
		obs={}
		description=""
		for taxonName in taxonList:
			m_Progress.WriteText("########### Querying GBIF for %s ######\n" % taxonName)
			m_Progress.WriteText("Concept ID: %d\n" % cID)
			### The set of observations
			### hash key #1 = grid cell
			### hash #2 = scientific name
			### value = list of GBIF key + list of lat/longs of genus observations in that grid cell

			fullTaxonomy={}
			resultCount =self.GETCOUNT(taxon_name,cID,minLatitude,maxLatitude,minLongitude,maxLongitude,m_Progress)
			nodeList=[]
			#this will mine GBIF database for the raw information to process
			m_Progress.WriteText("Latitude: %0.2f to %0.2f\tLongitude: %0.2f to %0.2f\n" % (minLatitude,maxLatitude,minLongitude,maxLongitude))
			nodeList = self.recursiveQuery(taxon_name,cID,minLatitude,maxLatitude,minLongitude,maxLongitude,m_Progress,nodeList,1)
			description = '\n'.join(self.__description__)
			# checks if minimum granularity was met while querying GBIF
			if self.__warnings__[0]==1:
				self.__warnings__[0]=0
				m_Progress.WriteText("\nWarnings were created. Please review progress bar for more information.\n")
			uniqueRID=set()
			for node in nodeList:
				string = node.toprettyxml(indent=' ')
				rID=-1
				key_string = re.search('gbifKey="(\d+)"',string)
				name=node.getElementsByTagName("tn:nameComplete")[0].toxml()
				name = re.sub(r'<.*?\>','',name)
				name = re.sub(r'[^A-Za-z ]+','',name)
				name = re.sub(r' *$','',name)
				genus = re.sub(r' .*','',name.lower())
				lat_tem = node.getElementsByTagName("to:decimalLatitude")[0].toxml()
				long_tem=node.getElementsByTagName("to:decimalLongitude")[0].toxml()
				fullLat = float(re.sub(r'\<.*?\>','',lat_tem))
				fullLon = float(re.sub(r'\<.*?\>','',long_tem))
				#check for weird cases where GBIF gives elements outside of extents.
				if (minLatitude <= fullLat <= maxLatitude) and (minLongitude <= fullLon <= maxLongitude):
					if key_string:
						id_string=re.search('\d+',key_string.group())
						id = int(id_string.group())
						rID=id
						uniqueRID.add(rID)
					else:
						sys.exit("Could not find a friggin GBIF key")
					
					#changed currGrid to Lat only for sorting purposes. Lon is added back in when written to file, so the user is none the wiser
					currGrid = (int(fullLat)+90)*360# + (int(fullLon) +180)
					distLocations.add((fullLat,fullLon))
					
					if( currGrid in obs ):
						if( genus in obs[currGrid]):
							obs[currGrid][genus].extend([(rID,fullLat,fullLon,name)])
						else:
							obs[currGrid].update({genus: [(rID,fullLat,fullLon,name)] })
					else:
						obs[currGrid] = {genus: [(rID,fullLat,fullLon,name)] }
					records = len(uniqueRID)
		return(obs,records,len(distLocations),description)
		
	#############################
	#	Queries GBIF recursively
	#	Case 1: Too many results
	#	Case 2: Success
	#############################
	def recursiveQuery(self,taxon_name,cID,minLatitude,maxLatitude,minLongitude,maxLongitude,m_Progress,nodeList,rowColFlag):	
		stopCoords =0.1
		#1=cols, 2=rows
		resultCount =self.GETCOUNT(taxon_name,cID,minLatitude,maxLatitude,minLongitude,maxLongitude,m_Progress)
		#check if too many resuslts received
		if resultCount > 1000 and self.__warnings__[1]==0:
			m_Progress.WriteText("Too many records, subdividing location.\n")
			#if col division
			if rowColFlag == 1:
				range = (maxLongitude - minLongitude)
				base = range/10
				#check granularity
				if range/10 < stopCoords:
					self.__warnings__[1]=1
					range = (maxLongitude - minLongitude)
					base = stopCoords
				#divides the current geographic range
				newCoords= self.GBIFGeneric.SUBDIVIDECOL(minLatitude,maxLatitude,minLongitude,maxLongitude,range,base)
				rowColFlag = 2
				for coords in newCoords:
					self.recursiveQuery(taxon_name,cID,coords[0],coords[1],coords[2],coords[3],m_Progress,nodeList,rowColFlag)
				return nodeList
			#else row division
			else:
				range = (maxLatitude - minLatitude)
				base = range/10
				#check granularity
				if range/10 < stopCoords:
					self.__warnings__[1]=1
					range = (maxLatitude - minLatitude)
					base = stopCoords
				#divides the current geographic range
				newCoords = self.GBIFGeneric.SUBDIVIDEROW(minLatitude,maxLatitude,minLongitude,maxLongitude,range,base)
				rowColFlag = 1
				for coords in newCoords:
					self.recursiveQuery(taxon_name,cID,coords[0],coords[1],coords[2],coords[3],m_Progress,nodeList,rowColFlag)
				return nodeList
		#query GBIF and process returns
		else:
			if self.__warnings__[1]==1:
				if resultCount==1000:
					m_Progress.write("Warning: Maximum gradient division with maximum records returned. Some records might be omitted.\n")
					self.__warnings__[0]=1
				
			self.__warnings__[1]=0
			m_Progress.WriteText("Latitude: %0.2f to %0.2f\tLongitude: %0.2f to %0.2f\n" % (minLatitude,maxLatitude,minLongitude,maxLongitude))
			url="http://data.gbif.org/ws/rest/occurrence/list?taxonconceptkey=%d&maxlatitude=%2f&minlatitude=%2f&maxlongitude=%2f&minlongitude=%2f" %(cID,maxLatitude,minLatitude,maxLongitude,minLongitude)
			try:
				response=urllib2.urlopen(url).read()
			except urllib2.URLError:
				m_Progress.WriteText("%s\n" % e.code)
				wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
				self.Close()
			parser=minidom.parseString(response)
			self.__description__.update(self.GETRIGHTS(parser))
			temper=parser.getElementsByTagName("to:TaxonOccurrence")
			#matches if whole content is the same. might be good to be more specific in the future if memory gets tighter
			for node in temper:
				text = node.toxml()
				if text in self.__uniqueNodeList__:
					temper.remove(node)
				else:
					self.__uniqueNodeList__.add(text)
			nodeList.extend(temper)
			return(nodeList)