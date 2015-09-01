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
import json
#from xml.dom import minidom
from decimal import Decimal

class Location:
	#metadata = {}
	def __init__(self,lat,lon,id,metadata):
		self.metadata = {}
		self.lat = lat
		self.lon = lon
		self.id = id
		self.metadata[id] = metadata
	
	def __eq__(self,other):
		return( self.lat==other.lat and self.lon==other.lon )
		
	def __ne__(self,other):
		result = self.__eq__(other)
		if result is NotImplemented:
			return result
		return not result
	
	def add(self,other):
		# other is new and only consists of one location
		self.metadata[other.id] = other.metadata[other.id]
	#	for key, value in self.metadata.items():
	#		print key
	#		print value
			
	def GetNumSequences(self):
		return len(metadata.keys())
	
	def ToString(self):
		print self.id
		print self.lat
		print self.lon
		print self.metadata.keys()
		print " "
	
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

	#	Queries GBIF to find the number of results for given boundary 
	def GETCOUNT(self,taxon_name,cID,m_Progress):
		taxonReq = '+'.join(taxon_name)			
	#	url= " http://data.gbif.org/ws/rest/occurrence/count?taxonconceptkey=%d&maxlatitude=%f&minlatitude=%f&maxlongitude=%f&minlongitude=%f" % (cID,maxLat,minLat,maxLon,minLon) 
		url = "http://api.gbif.org/v1/occurrence/count?taxonKey=%s&isGeoreferenced=true" %(cID)
		try:	
			response=urllib2.urlopen(url).read()
		except urllib2.HTTPError as e:
			wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
			self.Close()
		count = int(response)
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
		resultList.append("Key | Name | Confidence | Rank ")
		for taxonName in taxonList:	
			offset=0
			limit=100
			endOfRecords=False
			url = "http://api.gbif.org/v1/species/match?verbose=true&name=%s&offset=%d&limit=%d" %(taxonName,offset,limit)
			print url
			try:	
				html=urllib2.urlopen(url)
				offset= offset+limit
			except urllib2.HTTPError as e:
				wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
				self.Close()
			
			response = html.read()
			searchRes = json.loads(response)
			# Present best match
			id = searchRes['usageKey']
			rank = searchRes['rank']
			conf = searchRes['confidence']
			sciName = searchRes['scientificName']
			resultList.append("%s | %s | %s | %s"%(id,sciName,conf,rank))
			
			# Present Alternative Matches
			alternatives = searchRes['alternatives']
			for result in alternatives:				
				id = result['usageKey']
				rank = result['rank']
				conf = result['confidence']
				sciName = result['scientificName']
				resultList.append("%s | %s | %s | %s"%(id,sciName,conf,rank))	
		return resultList
	
	
	#	Get smallest TAX ID. 
	def	Strip (self,Node,tag):
		name = re.search(('%s' % tag),Node)
		name=name.group()
		name = re.sub(r'<.*?\>','',name)
		name = re.sub(r'[^A-Za-z ]+','',name)
		name = re.sub(r' *$','',name)
		return name
	
	def GETOBSENTIRERANGE(self, cID, minLatitude, maxLatitude, minLongitude, maxLongitude, m_Progress):
		distLocations=set()
		uniqueRID = set()
		taxonList=()
		numResults = 0
		
		offset=0
		limit=100
		endOfRecords=False
		obs=[]
		progress = 0
		masterKeys=set()
		while(endOfRecords==False):
			m_Progress.WriteText("Concept ID: %d\n" % cID)
			url = "http://api.gbif.org/v1/occurrence/search?taxonKey=%s&offset=%d&limit=%d&decimalLatitude=%s,%s&decimalLongitude=%s,%s" %(cID,offset,limit,minLatitude,maxLatitude,minLongitude,maxLongitude)
			try:	
				html=urllib2.urlopen(url)
			except urllib2.HTTPError as e:
				print e
				wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
				return obs, masterKeys
			offset= offset+limit
			response = html.read()
			searchRes = json.loads(response)
			endOfRecords = searchRes['endOfRecords']
			results = searchRes['results']
			numResults += len(results)
			if searchRes['count']==0:
				m_Progress.WriteText("No records returned.")
				return obs
			# Grab all of the result fields except the ones that store a list or dictionary.
			# Simplifies the parsing and presentation in line list form
			tempObs, masterKeys = self.ParseResults(results,masterKeys)
			obs.extend(tempObs)
			
			progress = progress + 100*float(len(results))/searchRes['count']
			m_Progress.WriteText("Processed %0.2f%% of records.\n" %(progress))
		return obs, masterKeys, numResults
	
	def ParseResults(self,results,masterKeys):	
		obs=[]
		for result in results:
			for key, value in result.items():
				if isinstance(value,list):
					result.pop(key)
				if isinstance(value,dict):
					result.pop(key)
				else:
					masterKeys.add(key)

			lat = result['decimalLatitude']
			lon = result['decimalLongitude']
			id = result['key']
			loc = Location(lat,lon,id,result)
			if( loc in obs ):
				obs[obs.index(loc)].add(loc)
			else:
				obs.append(loc)
		return (obs,masterKeys)
		