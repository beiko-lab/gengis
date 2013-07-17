#=======================================================================
# Author: Donovan Parks
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


import re
import wx
import sys
from GBIFGeneric import GBIFGeneric
import urllib2
from xml.dom import minidom
#from GBIFQueryLayout import GBIFQueryLayout

class GBIFSpecific:
	# get source of data sets as well as rights and citation
	def GETRIGHTS(self,fh):
		desc=""
		resource=fh.getElementsByTagName("gbif:dataResources")
		for node in resource:
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
		return(desc)

	#	Queries GBIF to find the number of results for given boundary 
	def GETCOUNT(self,taxon_name,minLat,maxLat,minLon,maxLon,m_Progress):
		taxonReq = '+'.join(taxon_name)			
		cID = self.GETTAXID(taxonReq,m_Progress)
		url= " http://data.gbif.org/ws/rest/occurrence/count?taxonconceptkey=%d&maxlatitude=%d&minlatitude=%d&maxlongitude=%d&minlongitude=%d" % (cID,maxLat,minLat,maxLon,minLon) 
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
					string = taxResult.toxml()
					key_string = re.search('gbifKey="(\d+)"',string)
					id_string=re.search('\d+',key_string.group())
					id = int(id_string.group())
					name=self.Strip(string,"<tn:nameComplete>.*")
					rank=self.Strip(string,"<tn:rankString>.*")
					according=self.Strip(string,"<tc:accordingToString>.*")
					queryLayout.InsertItems(["%d | %s | %s | %s"%(id,name,rank,according)],pos)
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
	
	#	Convert TaxonName to a Concept ID for GBIF
	def GETTAXID (self,taxonName,m_Progress):
		taxConceptID=100000000
		url="http://data.gbif.org/ws/rest/taxon/list?scientificname="+taxonName+"*&dataproviderkey=1&dataresourcekey=1"
		try:	
			html=urllib2.urlopen(url)
		except urllib2.HTTPError as e:
			m_Progress.WriteText("%s\n" % e.code)
			wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
			self.Close()
		taxResponse=html.read()
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
		
	# Search for a Taxon Name in a Geographic boundary
	def GETOBSENTIRERANGE(self,taxon_name,minLatitude,maxLatitude,minLongitude,maxLongitude,m_Progress):
		records=0
		distLocations=set()
		
		conversions = {}
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
		description=""
		for taxonName in taxonList:
			m_Progress.WriteText("########### Querying GBIF for %s ######\n" % taxonName)
			cID = self.GETTAXID(taxonName,m_Progress)
			if(cID == -1):
				m_Progress.WriteText("No concept ID found for %s. Skipping...\n" % taxonName)
				continue
			m_Progress.WriteText("Concept ID: %d\n" % cID)
			### The set of observations
			### hash key #1 = grid cell
			### hash #2 = scientific name
			### value = list of GBIF key + list of lat/longs of genus observations in that grid cell

			fullTaxonomy={}
			resultCount =self.GETCOUNT(taxon_name,minLatitude,maxLatitude,minLongitude,maxLongitude,m_Progress)
			#chek if whole window fits: if not divide into columns
			nodeList=[]
			if resultCount>1000 :
				newCoords = self.GBIFGeneric.SUBDIVIDECOL(minLatitude,maxLatitude,minLongitude,maxLongitude)
				for coords in newCoords:
					colMinLongitude = coords[2]
					colMaxLongitude = coords[3]
					colMinLatitude = coords[0]
					colMaxLatitude = coords[1]
					
					resultCount =self.GETCOUNT(taxon_name,colMinLatitude,colMaxLatitude,colMinLongitude,colMaxLongitude,m_Progress)
					#check if col fits: if not divide into cells
					if resultCount > 1000:
						newCoords = self.GBIFGeneric.SUBDIVIDEROW(colMinLatitude,colMaxLatitude,colMinLongitude,colMaxLongitude)
						for coords in newCoords:
							cellMinLongitude = coords[2]
							cellMaxLongitude = coords[3]
							cellMinLatitude = coords[0]
							cellMaxLatitude = coords[1]
							resultCount =self.GETCOUNT(taxon_name,cellMinLatitude,cellMaxLatitude,cellMinLongitude,cellMaxLongitude,m_Progress)
							#check if cell fits: if not divide into centicells
							print "stage 3"
							if resultCount >1000:
								newCoords = self.GBIFGeneric.SUBDIVIDECOL(cellMinLatitude,cellMaxLatitude,cellMinLongitude,cellMaxLongitude)
								for coords in newCoords:
									ccellMinLongitude = coords[2]
									ccellMaxLongitude = coords[3]
									ccellMinLatitude = coords[0]
									ccellMaxLatitude = coords[1]
									resultCount =self.GETCOUNT(taxon_name,ccellMinLatitude,ccellMaxLatitude,ccellMinLongitude,ccellMaxLongitude,m_Progress)
									print "stage 4"
									if resultCount>1000:
										wx.MessageBox("Smallest Gradient not sufficient.")
									#centicells worked
									else:
										m_Progress.WriteText("Latitude: %0.2f to %0.2f\tLongitude: %0.2f to %0.2f\n" % (ccellMinLatitude,ccellMaxLatitude,ccellMinLongitude,ccellMaxLongitude))
										url="http://data.gbif.org/ws/rest/occurrence/list?taxonconceptkey=%d&maxlatitude=%d&minlatitude=%d&maxlongitude=%d&minlongitude=%d" %(cID,ccellMaxLatitude,ccellMinLatitude,ccellMaxLongitude,ccellMinLongitude)
										try:
											response=urllib2.urlopen(url).read()
										except urllob2.URLError:
											m_Progress.WriteText("%s\n" % e.code)
											wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
											self.Close()
										parser=minidom.parseString(response)
										description+=self.GETRIGHTS(parser)
										temper=parser.getElementsByTagName("to:TaxonOccurrence")
										nodeList.extend(temper)
										m_Progress.WriteText("%d records found\n" % len(temper))
							
							#cells worked
							else:
								m_Progress.WriteText("Latitude: %0.2f to %0.2f\tLongitude: %0.2f to %0.2f\n" % (cellMinLatitude,cellMaxLatitude,cellMinLongitude,cellMaxLongitude))
								url="http://data.gbif.org/ws/rest/occurrence/list?taxonconceptkey=%d&maxlatitude=%d&minlatitude=%d&maxlongitude=%d&minlongitude=%d" %(cID,cellMaxLatitude,cellMinLatitude,cellMaxLongitude,cellMinLongitude)
								try:
									response=urllib2.urlopen(url).read()
								except urllob2.URLError:
									m_Progress.WriteText("%s\n" % e.code)
									wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
									self.Close()
								parser=minidom.parseString(response)
								description+=self.GETRIGHTS(parser)
								temper=parser.getElementsByTagName("to:TaxonOccurrence")
								nodeList.extend(temper)
								m_Progress.WriteText("%d records found\n" % len(temper))
					#cols worked
					else:
						m_Progress.WriteText("Latitude: %0.2f to %0.2f\tLongitude: %0.2f to %0.2f\n" % (colMinLatitude,colMaxLatitude,colMinLongitude,colMaxLongitude))
						url="http://data.gbif.org/ws/rest/occurrence/list?taxonconceptkey=%d&maxlatitude=%d&minlatitude=%d&maxlongitude=%d&minlongitude=%d" %(cID,colMaxLatitude,colMinLatitude,colMaxLongitude,colMinLongitude)
						try:
							response=urllib2.urlopen(url).read()
						except urllob2.URLError:
							m_Progress.WriteText("%s\n" % e.code)
							wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
							self.Close()
						parser=minidom.parseString(response)
						description+=self.GETRIGHTS(parser)
						temp=parser.getElementsByTagName("to:TaxonOccurrence")
						m_Progress.WriteText("%d records found\n" % len(temp))
						nodeList.extend(temp)
						
			#whole thing worked
			else:
				m_Progress.WriteText("Latitude: %0.2f to %0.2f\tLongitude: %0.2f to %0.2f\n" % (minLatitude,maxLatitude,minLongitude,maxLongitude))
				url="http://data.gbif.org/ws/rest/occurrence/list?taxonconceptkey=%d&maxlatitude=%d&minlatitude=%d&maxlongitude=%d&minlongitude=%d" %(cID,maxLatitude,minLatitude,maxLongitude,minLongitude)
				try:
					response=urllib2.urlopen(url).read()
				except urllob2.URLError:
					m_Progress.WriteText("%s\n" % e.code)
					wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
					self.Close()
				parser=minidom.parseString(response)
				description+=self.GETRIGHTS(parser)
				nodeList=parser.getElementsByTagName("to:TaxonOccurrence")
				m_Progress.WriteText("%d records found\n" % len(nodeList))
			if len(nodeList) > 0:
				records += len(nodeList)
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
				fullLat = float(re.sub(r'\<.*?\>','',lat_tem))
				fullLon = float(re.sub(r'\<.*?\>','',long_tem))
				currGrid = (int(fullLat)+90)*360 + (int(fullLon) +180)
				conversions[currGrid]=	[fullLat,fullLon]
				distLocations.add((fullLat,fullLon))
				try:
					obs[currGrid][genus].extend([(rID,lat_tem,long_tem,name)])
				except KeyError:
					try:
						obs[currGrid].update({genus: [(rID,lat_tem,long_tem,name)] })
					except KeyError:
						obs[currGrid] = {genus: [(rID,lat_tem,long_tem,name)] }
		return(obs,conversions,records,len(distLocations),description)

