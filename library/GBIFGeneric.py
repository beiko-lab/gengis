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


import re
import wx

class GBIFGeneric:	
	def GETTEXT (self,obs_list,conversions_list):
		OUTL=""
		OUTS=""
		for obs,convs in zip(obs_list,conversions_list):
	#		import pdb; pdb.set_trace()
			locs, seqs = self.MAKEOUTS(obs,convs)
			OUTL+=locs
			OUTS+=seqs
		return(OUTL,OUTS)
		
	#	Transforms the mined data into text to be output
	def MAKEOUTS (self,obs,conversions):
		uniqueSiteID = set()
		OUTLTEXT=""
		OUTSTEXT=""
		seqFileAgg = {}
		for cellOut in sorted(obs.keys()):
			if len(obs[cellOut].keys()) > 0:
				for taxOut in sorted(obs[cellOut].keys()):
					thisList=obs[cellOut][taxOut]
					for ent in thisList:
						fullLat = float(re.sub(r'\<.*?\>','',ent[1]))
						fullLon = float(re.sub(r'\<.*?\>','',ent[2]))
						siteID = "%s_%f_%f" %(taxOut,fullLat,fullLon)
						if siteID not in uniqueSiteID:
							uniqueSiteID.add(siteID)
							OUTLTEXT += ("%s,%f,%f,%d,%d\n" % (siteID, fullLat, fullLon, len(obs[cellOut].keys()), cellOut ))
						toKey = "%s,%f,%f,%s,%s,%s,%s" %(siteID, conversions[cellOut][0],conversions[cellOut][1],ent[3],taxOut,ent[1],ent[2])
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
		return (array)
	
	#subdivide a given range by longitude
	def SUBDIVIDECOL(self,minlatitude,maxlatitude,minlongitude,maxlongitude):
		longitudeRange = maxlongitude - minlongitude
		new_coords = []
		longitudeBase= float(longitudeRange)/float(10)
		for i in range(0,10):
			new_coords.append(( minlatitude,maxlatitude,minlongitude+longitudeBase * i,minlongitude+longitudeBase* (i+1)))
		return(new_coords)
	
	#subdivide a given range by latitude
	def SUBDIVIDEROW(self,minlatitude,maxlatitude,minlongitude,maxlongitude):
		latitudeRange = maxlatitude - minlatitude
		new_coords = []
		latitudeBase= float(latitudeRange)/float(10)
		for i in range(0,10):
			new_coords.append((minlatitude + latitudeBase*i,minlatitude + latitudeBase *(i+1),minlongitude,maxlongitude))
		return(new_coords)	
		
	def WRITEEXPORT(self,outfile,outtext,header):
		try:
			OUTL=open(outfile,'w')
			if(len(header)>0):
				OUTL.write(header)
			OUTL.write(outtext)
			OUTL.close()
		except IOError:
			wx.MessageBox("File could not be written. Perhaps another program is using it.")