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
import decimal

class GBIFGeneric:	
	def GETTEXT (self,obs_list,conversions_list):
		OUTL=""
		OUTS=""
		for obs,convs in zip(obs_list,conversions_list):
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
					#	siteID = "%s_%f_%f" %(taxOut,fullLat,fullLon)
						siteID = "%s_%f_%f" %(ent[3],fullLat,fullLon)
						siteID = re.sub(' ','_',siteID)
						if siteID not in uniqueSiteID:
							uniqueSiteID.add(siteID)
							OUTLTEXT += ("%s,%f,%f,%d,%d,%s,%s\n" % (siteID, fullLat, fullLon, len(obs[cellOut].keys()), cellOut,ent[3],taxOut ))
						toKey = "%s,%f,%f,%s,%s,%s,%s" %(siteID, conversions[cellOut][0],conversions[cellOut][1],ent[3],taxOut,ent[1],ent[2])
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
		
	# stolen from	http://stackoverflow.com/questions/477486/python-decimal-range-step-value written by gimel January 25 '09 11:57
	def drange(self,start, stop, step):
		r = start
		while r < stop:
			yield r
			r += step
	
	#subdivide a given range by longitude
	def SUBDIVIDECOL(self,minlatitude,maxlatitude,minlongitude,maxlongitude,numSubs,step):
		longitudeRange = maxlongitude - minlongitude
		new_coords = []
		for i in self.drange(0,numSubs,step):
			minl = round(decimal.Decimal(str(minlongitude+i)),1)
			maxl = round(decimal.Decimal(str(minlongitude+i+step)),1)
			new_coords.append((minlatitude,maxlatitude,minl,maxl))
		return(new_coords)
	
	#subdivide a given range by latitude
	def SUBDIVIDEROW(self,minlatitude,maxlatitude,minlongitude,maxlongitude,numSubs,step):
		latitudeRange = maxlatitude - minlatitude
		new_coords = []
		for i in self.drange(0,numSubs,step):
			minl = round(decimal.Decimal(str(minlatitude+i)),1)
			maxl = round(decimal.Decimal(str(minlatitude+i+step)),1)
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