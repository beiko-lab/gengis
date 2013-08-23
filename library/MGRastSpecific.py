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
import urllib2
import time
import simplejson as json
from operator import itemgetter

class MGRastSpecific:
#	def __init__(self):
		#nothing to see here
	#test =  Akkermansia	
	
	def GETTAXRESULT(self,taxon_name,minlatitude,maxlatitude,minlongitude,maxlongitude,Summary):
		result=[]
		totalRecords=0
		limit=1000
		taxonReq = '+'.join(taxon_name)
		taxonList=()
		FAIL = 0
		taxonList=[taxonReq]
		obs={}
		taxonName=taxonReq		
		url="http://api.metagenomics.anl.gov/1/metagenome?status=public&organism=%s&limit=%s&verbosity=mixs"%(taxonName,limit)
		try:	
			html=urllib2.urlopen(url)
			startTime = time.time()
		except urllib2.HTTPError as e:
			wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
			return False
		response=html.read()
		#sleep here to adhere to MG-RAST's request that no more than a Query per second
		searchRes = json.loads(response)
		nodeList = searchRes['data']
		next = str(searchRes['next'])
		if len(nodeList)==0:
			wx.MessageBox("No Organism Found.")
			return False
		while True:
		#	if (len(nodeList) > 0):
			for node in nodeList:
				#	REMOVE PRIVATE STUDIES: BE SURE TO TURN THIS OFF IF AUTHENTICATION IS ADDED
				latitude,longitude=node['latitude'],node['longitude']
				if node['status'] == "private" or latitude== "" or not(minlatitude <= latitude <= maxlatitude) or not(minlongitude <= longitude <= maxlongitude):
					continue
				id = node['id']
				name = node['project_name']
				according = node['name']
				result.append("%s | %s | %s"%(id,name,according))
		#	else:
		#		wx.MessageBox("No Organism Found.")
		#		return False
			totalRecords+=len(nodeList)
		#	print next
			if next != 'None':
				url = next
				#sleep here to adhere to MG-RAST's request that no more than a Query per second
				runTime = time.time() - startTime
		#		print runTime
				if runTime < 1:
					time.sleep(1)
				try:	
					html=urllib2.urlopen(url)
					startTime = time.time()
				except urllib2.HTTPError as e:
					wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
					return False
				response=html.read()
				searchRes = json.loads(response)
				nodeList = searchRes['data']
				next = str(searchRes['next'])
			else:
				break
		Summary.SetLabel("Found %d studies.\n Returned %d valid studies." %(totalRecords,len(result)))
		return(result)
			
	# test = mgm4440037.3
	def GETOBS(self,taxon_name,searchType,options,m_Progress):
		m_Progress.WriteText("Querying %s...\n"%taxon_name)
		limit=1000
		obs={}
		if searchType == "organism":
			url="http://api.metagenomics.anl.gov/matrix/organism/?id=%s%s"%(taxon_name,options)
			rowMeta = 'taxonomy'
		elif searchType == "function":
			url="http://api.metagenomics.anl.gov/matrix/function/?id=%s%s"%(taxon_name,options)
			rowMeta = 'ontology'
		elif searchType == "feature":
			url="http://api.metagenomics.anl.gov/matrix/feature/?id=%s%s"%(taxon_name,options)
			rowMeta = 'taxonomy'
		try:	
			html=urllib2.urlopen(url)
			startTime = time.time()
		except urllib2.HTTPError as e:
			wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
			return False
		response=html.read()
		matrix = json.loads(response)
		if matrix['matrix_type'] == 'dense':
			nodeList = self.DENSEHANDLER(matrix)
		else:
			nodeList = self.SPARSEHANDLER(matrix)	
		for node in nodeList:
			id = node[0]['id']
			lat = node[1]['metadata']['sample']['data']['latitude']
			lon = node[1]['metadata']['sample']['data']['longitude']
			currGrid = (int(float(lat))+90)*36
			richness = node[2]
			projID = node[1]['id']
			taxonomy = '|'.join(str(i) for i in node[0]['metadata'][rowMeta])
			taxonomy = re.sub(",","",taxonomy)
			try:
				obs[currGrid][id].extend([(richness,lat,lon,taxonomy,projID)])
			except KeyError:
				try:
					obs[currGrid].update({id: [(richness,lat,lon,taxonomy,projID)] })
				except KeyError:
					obs[currGrid] = {id: [(richness,lat,lon,taxonomy,projID)] }
		m_Progress.WriteText("Recieved %d records.\n"%len(nodeList))
		return obs
			
	def SPARSEHANDLER(self,matrix):
		nodeList=[]
		metaData = matrix['columns']
		data = matrix['data']
		id = matrix['rows']
		for dat in data:
			row = dat[0]
			col = dat[1]
			val = dat[2]
			node = [id[row],metaData[col],val]
			nodeList.append(node)
		
		return nodeList
	
	def DENSEHANDLER(self,matrix):

		metaData = matrix['columns']
		data = matrix['data']
		id = matrix['rows']
		if matrix['type'] == 'Taxon table':
			print 'yippee kiyay'
		return 1
		
	def GETTEXT (self,obs_list):
		OUTL=""
		OUTS=""
		for obs in obs_list:
			locs, seqs = self.MAKEOUTS(obs)
			OUTL+=locs
			OUTS+=seqs
		return(OUTL,OUTS)
		
	#	Transforms the mined data into text to be output
	def MAKEOUTS (self,obs):
		uniqueSiteID = set()
		OUTLTEXT=""
		OUTSTEXT=""
		seqFile = []
		for cellOut in sorted(obs.keys()):
			if len(obs[cellOut].keys()) > 0:
				for taxOut in sorted(obs[cellOut].keys()):
					thisList=obs[cellOut][taxOut]
					for ent in sorted(thisList,key=itemgetter(1,2)):
						fullLat = float(ent[1])
						fullLon = float(ent[2])
						siteID = "%s_%f_%f" %(taxOut,fullLat,fullLon)
						siteID = re.sub(' ','_',siteID)
						if siteID not in uniqueSiteID:
							uniqueSiteID.add(siteID)
							OUTLTEXT += ("%s,%f,%f,%s,%s\n" % (siteID, fullLat, fullLon, ent[0], cellOut+(int(fullLon) +180)))
						toKey = "%s,%s,%f,%f,%s" %(ent[4],siteID,fullLat,fullLon,ent[3])
						seqFile.append(toKey)
		for IDlist in seqFile:
			OUTSTEXT += ("%s\n" %(IDlist))
		return(OUTLTEXT,OUTSTEXT)
		