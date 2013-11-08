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
import copy
import unicodedata
from operator import itemgetter

class MGRastSpecific:
#	def __init__(self):
		#nothing to see here
	#test =  Akkermansia	
	
	def GETTAXRESULT(self,taxon_name,searchType,minlatitude,maxlatitude,minlongitude,maxlongitude,Summary):
	#	result=[]
		result = {}
		count=0
		totalRecords=0
		limit=1000
		taxonReq = '+'.join(taxon_name)
		taxonList=()
		FAIL = 0
		taxonList=[taxonReq]
		obs={}
		taxonName=taxonReq		
		if searchType == "organism":
			url="http://api.metagenomics.anl.gov/1/metagenome?status=public&organism=%s&limit=%s&verbosity=mixs"%(taxonName,limit)
			rowMeta = 'taxonomy'
		elif searchType == "function":
			url="http://api.metagenomics.anl.gov/1/metagenome?status=public&function=%s&limit=%s&verbosity=mixs"%(taxonName,limit)
			rowMeta = 'ontology'
		elif searchType == "feature":
			url="http://api.metagenomics.anl.gov/1/metagenome?status=public&feature=%s&limit=%s&verbosity=mixs"%(taxonName,limit)
			rowMeta = 'taxonomy'
		try:	
			html=urllib2.urlopen(url)
			startTime = time.time()
		except urllib2.HTTPError as e:
			wx.EndBusyCursor()
			wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
			return False
		response=html.read()
		searchRes = json.loads(response)
		if( 'data' in searchRes ):
			nodeList = searchRes['data']
			next = str(searchRes['next'])
		else:
			nodeList = []
		if len(nodeList)==0:
			wx.MessageBox("No Organism Found.")
			return False
		while True:
			for node in nodeList:
				#	REMOVE PRIVATE STUDIES: BE SURE TO TURN THIS OFF IF AUTHENTICATION IS ADDED
				latitude,longitude=node['latitude'],node['longitude']
				if node['status'] == "private" or latitude== "" or not(minlatitude <= latitude <= maxlatitude) or not(minlongitude <= longitude <= maxlongitude):
					continue
				id = node['id']
				name = node['project_name']
				according = node['name']
				enviro = node['biome']
				
			#	result.append("%s | %s | %s"%(id,name,according))
				if enviro in result:
					# no duplicate study id's exist. that would kind of break the whole idea of a study id
					result[enviro].update( { id:[name,according] } )
					
				else:
					result[enviro]= { id:[name,according] }
					
				count+=1
			totalRecords+=len(nodeList)
			if next != 'None':
				url = next
				#sleep here to adhere to MG-RAST's request that no more than a Query per second
				runTime = time.time() - startTime
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
		matches = []
		# sort and order the results
		for key in sorted(result.iterkeys()):
			for val in sorted(result[key].iterkeys()):
				matches.append("%s | %s | %s | %s" %(key,val,result[key][val][0],result[key][val][1]))
		Summary.SetLabel("Found %d studies.\n Returned %d valid studies." %(totalRecords,count))
		return(matches)
			
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
		elif searchType =="study":
			url="http://api.metagenomics.anl.gov/matrix/organism/?id=%s%s"%(taxon_name,options)
			rowMeta = 'taxonomy'
		try:	
			html=urllib2.urlopen(url)
			startTime = time.time()
		except urllib2.HTTPError as e:
			wx.MessageBox("The server is temporarily unreachable.\nPlease try again later.")
			m_Progress.WriteText("%s"%taxon_name)
			return False
		response=html.read()
		matrix = json.loads(response)
		metaVals = {}
	
		#check for errors in the return
		if 'ERROR' in matrix:
			wx.MessageBox("Error in data: Please consult MG-RAST to verify the study you entered is valid.")
			m_Progress.WriteText("Error. Consult MG-RAST.\n")
			return (obs,metaVals)
			
		if matrix['type'] != "Taxon table":
			wx.MessageBox("Unexpected data format.")
			return (obs,metaVals)
		if matrix['matrix_type'] == 'dense':
			nodeList = self.DENSEHANDLER(matrix)
		else:
			nodeList,metaVals = self.SPARSEHANDLER(matrix)
		if nodeList == False:
			return (obs,metaVals)
		
		for node in nodeList:
			id = node[0]['id']
			lat = node[1]['metadata']['sample']['data']['latitude']
			lon = node[1]['metadata']['sample']['data']['longitude']
			currGrid = (int(float(lat))+90)*36
			richness = node[2]
			projID = node[1]['id']
			taxonomyLength = len(node[0]['metadata'][rowMeta])
			taxonomy = '|'.join(str(i) for i in node[0]['metadata'][rowMeta])
			taxonomy = re.sub(",","",taxonomy)
			labeled_taxonomy = re.sub("\|",",",taxonomy)
			if( currGrid in obs ):
				if( id in obs[currGrid]):
					obs[currGrid][id].extend([(richness,lat,lon,taxonomy,labeled_taxonomy,projID)])
				else:
					obs[currGrid].update({id: [(richness,lat,lon,taxonomy,labeled_taxonomy,projID)] })
			else:
				obs[currGrid] = {id: [(richness,lat,lon,taxonomy,labeled_taxonomy,projID)] }
		m_Progress.WriteText("Recieved %d records.\n"%len(nodeList))
		return (obs, metaVals,taxonomyLength)
	
	# Get all possible metadata from the JSON file
	def GRABALLMETADATA(self,input,oldKey,metaKeys,metaVals):
		if isinstance(input,dict):
			for key,value in input.iteritems():
				# only dictionaries store the keys, so creates a key trail so you know where each trait came from
				if( oldKey != "temp" ):
					tempKey = "%s.%s" %(oldKey,key)
				else:
					tempKey = key
				tempKeys,tempVals = self.GRABALLMETADATA(value,tempKey,metaKeys,metaVals)
			return(metaKeys,metaVals)
		elif isinstance(input,list):
			for elem in input:
				index = input.index(elem)
				tempKey = ("%s.%s") %(oldKey,index)
				tempKeys,tempVals = self.GRABALLMETADATA(elem,tempKey,metaKeys,metaVals)
			return(tempKeys,tempVals)
		elif isinstance(input,unicode):
			input = unicodedata.normalize('NFKD',input).encode('ascii','ignore')
			input = re.sub(',',' ',input)
			tempKeys,tempVals = self.GRABALLMETADATA(input,oldKey,metaKeys,metaVals)
			return(tempKeys,tempVals)
		else:
			oldKey = oldKey.encode('utf-8')
			if isinstance(input,str):
				input = re.sub(',',' ',input)
				input = re.sub('[\r\n\t]','  ',input)
			metaKeys.append(oldKey)
			metaVals.append(input)
			return (oldKey,input)
		
	def SPARSEHANDLER(self,matrix):
		ignore = ['data','rows','matrix_type']
		miningMatrix = copy.deepcopy(matrix)
		for i in ignore:
			miningMatrix.pop(i, None)
		metaKeys = [];
		metaVals = [];	
		#convert into dictionary of key: values
		metaKeys, metaVals = self.GRABALLMETADATA(miningMatrix,"temp",metaKeys,metaVals)
		dictionary = dict(zip(metaKeys,metaVals))
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
		
		return (nodeList,dictionary)
	
	def DENSEHANDLER(self,matrix):
		wx.MessageBox("Unexpected data format.")
		return False
		metaData = matrix['columns']
		data = matrix['data']
		id = matrix['rows']
		if matrix['type'] == 'Taxon table':
			print 'yippee kiyay'
		return 1
		
	def GETTEXT (self,obs_list,metaVals):
		OUTL=""
		OUTS=""
		for obs in obs_list:
			locs, seqs = self.MAKEOUTS(obs,metaVals)
			OUTL+=locs
			OUTS+=seqs
		return(OUTL,OUTS)
		
	#	Transforms the mined data into text to be output
	def MAKEOUTS (self,obs,metaVals):
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
						# parse any commas in taxon
						#this works if GenGIS can handle commas
					#	taxOut = re.sub(',','\",\"',taxOut)
						taxOut = re.sub(',','_', taxOut)
						#siteID is now the study
						siteID = ent[5];
						if siteID not in uniqueSiteID:
							uniqueSiteID.add(siteID)
							index = metaVals['columns.0.id'].index(siteID)
							metadata = "" 
							first = True
							for key in sorted(metaVals.keys()):
								if first:
									metadata = "%s" %(metaVals[key][index])
									first = False
								else:
									metadata = "%s,%s" %(metadata, metaVals[key][index])
							OUTLTEXT += ("%s,%f,%f,%s,%s\n" % (siteID, fullLat, fullLon, cellOut+(int(fullLon) +180), metadata))
						toKey = "%s,%s,%f,%f,%s,%s,%s" %(taxOut,ent[5],fullLat,fullLon,ent[0],ent[4],ent[3])
						seqFile.append(toKey)
		for IDlist in seqFile:
			OUTSTEXT += ("%s\n" %(IDlist))
		return(OUTLTEXT,OUTSTEXT)
		