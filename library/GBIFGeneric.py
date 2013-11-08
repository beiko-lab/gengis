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
from decimal import Decimal
from operator import itemgetter

class GBIFGeneric:
	
	def roundCoord(self,num):
		return round(Decimal(str(num)),1)
	
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
		seqFileAgg = {}
		for cellOut in sorted(obs.keys()):
			if len(obs[cellOut].keys()) > 0:
				for taxOut in sorted(obs[cellOut].keys()):
					thisList=obs[cellOut][taxOut]
					for ent in sorted(thisList,key=itemgetter(1,2)):
						fullLat = ent[1]
						fullLon = ent[2]
						siteID = "%s_%f_%f" %(ent[3],fullLat,fullLon)
						siteID = re.sub(' ','_',siteID)
						if siteID not in uniqueSiteID:
							uniqueSiteID.add(siteID)
						#	OUTLTEXT += ("%s,%f,%f,%d,%d,%s,%s\n" % (siteID, fullLat, fullLon, len(obs[cellOut].keys()), cellOut+(int(fullLon) +180),ent[3],taxOut ))
							OUTLTEXT += ("%s,%f,%f,%d,%s,%s\n" % (siteID, fullLat, fullLon, cellOut+(int(fullLon) +180),ent[3],taxOut ))
					#	toKey = "%s,%f,%f,%s,%s,%s,%s" %(siteID, fullLat,fullLon,ent[3],taxOut,ent[1],ent[2])
						toKey = "%s,%s,%s,%s,%s" %(siteID,ent[3],taxOut,ent[1],ent[2])
						toKey = re.sub(r'\<.*?\>','',toKey)
						try:
							seqFileAgg[toKey].extend([ent[0]])
						except KeyError:
							seqFileAgg[toKey]=[ent[0]]
		seqFileAgg_items = seqFileAgg.items()
		seqFileAgg_items.sort(key=lambda x: x)
		for outME,IDlist in seqFileAgg_items:
			save = IDlist[0]
			IDlist = set(IDlist)
			OUTSTEXT += ("%d,%s,%d,%s\n" %(save,outME,len(IDlist),'|'.join(str(i) for i in IDlist)))
		return(OUTLTEXT,OUTSTEXT)
				
	# Populate the Results Table using Taxa and Geographic boundaries
	def CPPOUT (self,input):
		array = input.split("\n")
		return (array)
		
	def drange(self,start,stop,step):
		r = self.roundCoord(start)
		list = []
		while (r + step) <= (stop):
			list.append(self.roundCoord(r))
			r+= self.roundCoord(step)
		if r < stop:
			r -= self.roundCoord(step)
			r += (stop-step-r)
			tempR = self.roundCoord(r)	#quick rounding to try and account for the innacuracy of floats
			list.append(tempR)
		return(list)
	
	
	#subdivide a given range by longitude
	def SUBDIVIDECOL(self,minlatitude,maxlatitude,minlongitude,maxlongitude,numSubs,step):
		new_coords = []
		tem = self.drange(minlongitude, maxlongitude,step)
		#protecting for rounding errors
		minlongitude = self.roundCoord(minlongitude)
		maxlongitude = self.roundCoord(maxlongitude)
		step = self.roundCoord(step)
		for i in tem:
			minl = i
			maxl = i+step
			new_coords.append((minlatitude,maxlatitude,minl,maxl))
		return(new_coords)
	
	#subdivide a given range by latitude
	def SUBDIVIDEROW(self,minlatitude,maxlatitude,minlongitude,maxlongitude,numSubs,step):
		new_coords = []
		tem = self.drange(minlatitude,maxlatitude,step)
		#protecting for rounding errors
		minlatitude = self.roundCoord(minlatitude)
		maxlatitude = self.roundCoord(maxlatitude)
		step = self.roundCoord(step)
		for i in tem:
			minl = i
			maxl = i+step
			new_coords.append((minl,maxl,minlongitude,maxlongitude))
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