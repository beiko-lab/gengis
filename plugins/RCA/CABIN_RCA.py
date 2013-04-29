import GenGIS

import rpy2
import rpy2.robjects as robjects
r = robjects.r

import os

#Uncomment this to allow run-time debugging
#import pdb; pdb.set_trace()

# define path to this plugin with the R environment
pluginPath = os.path.join(os.path.dirname(os.path.realpath(__file__)),'')

print pluginPath
robjects.globalEnv["hDir"] = pluginPath

'''
Example usage:
sys.path.append("C:\\Projects\\B2.0\\Armanini-CABIN\\Fredericton-RCA\\")
from CABIN_RCA import *
RCA_Example = CABIN_RCA("Group","Label","Count")
RCA_Example.Run_RCA("OoverE")
'''

##################################################;
# R code to build a Rivpacs-type model for Atlantic Canada as per Armanini et al. (Submitted);
# The code here presented is a simplified version with respect to the one used to develop and test the RCA model; It is design to quickly input and test new test dataset
# set up 12 September 2011, for Atlantic Canada data;
# For R versions >= 2.11.1;
# Please cite: Armanini D.G., Monk W.A., Carter L., Cote D. & Baird D.J. Submitted. A biomonitoring Reference Condition Model for rivers in Atlantic Canada. 
##################################################;
# ACKNOWLEDGMENTS
#
# We would like to acknowledge partners who have made this paper possible by sharing their data: 
# 1) the New Brunswick Department of Environment (New Brunswick DENV); 
# 2) Defense Canada, Base Gagetown; 
# 3) Environment Canada's National Agri-Environmental Standards Initiative (NAESI); 
# 4) Canadian Rivers Institute (CRI); and 
# 5) the Canadian Aquatic Biomonitoring Network (CABIN) network; 6) Parks Canada. 
#
# We would also like to thank Dr. John Van Sickle (USEPA) for generously sharing R scripts and for reviewing an earlier version of the manuscript 
# and Pierre Martel (Parks Canada) for support on the Upper Mersey dataset. 
# This research was supported through program funding from Environment Canada.
##################################################;

class CABIN_RCA:

	### Initialize the data structure. setCol is the name of the group column (e.g., 'cal','val','test'), spCol is the name of the column in the sequence file that has diversity data, and countCol is the column that has count information.
	def __init__(self, spCol, countCol):
		#self.setLabel = setCol
		self.speciesLabel = spCol
		self.countField = countCol
		
		self.graphicalElementIds = []

	def create_rca_model(self):
		'''Creates a RCA model'''
		#1)Get "cal" data
		#2)process and create RCA model
		#3)Save model to disk
		pass

	### Run the reference condition analysis. Since this was initially written as an R script, most of the R code is simply preserved intact, with RPy used to pass data into and out of the original script. ###		
	def Run_RCA(self,rca_model = 'atlantic_rca_model'):
		'''Runs test data against the given RCA model'''

		
		### Get the layer data ###
		locs = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
		#only use locs with sequence data
		locs=[loc for loc in locs if loc.GetNumSequenceLayers() > 0]
		
		
		#################################### ENVIRONMENTAL DATA #####################################
		envFields = ["geo_sed", "geo_intr", "geo_sedvol", "clim_Trange"]
		
		### Hash of hashes to store the env field data
		envValues = {}
			
		### Iterate through the environmental data and build dictionary of values
		for loc in locs:			

			siteID = loc.GetName()
			envValues[siteID] = {}
			
			### Add values for each environmental value
			for envField in envFields:
				data = loc.GetController().GetData()[envField]
				envValues[siteID][envField] = data
				
		### Build habitat data frame for each subset of samples
			
		### The list of names ###
		PyNameList = envValues.keys()
	       	numExamples = len(PyNameList)
			
		RnameList = r["c"](robjects.StrVector(PyNameList))
		RenvList = r["c"](robjects.StrVector(envFields))
		Rdims = r["list"](RnameList,RenvList)
			
		RvarName = str("test_HAB_NAME")
		robjects.globalEnv[RvarName] = RnameList
			
		### The habitat value matrix ###
		PyHabList = []
		for addName in PyNameList:
			for addEnv in envFields:
				if addEnv in envValues[addName].keys():
					PyHabList.append(float(envValues[addName][addEnv]))
				else:
					PyHabList.append(0.0)
			
		### Create the matrix and send it to R ###
		RhabList = r["c"](robjects.FloatVector(PyHabList))
		RhabMatrix = r.matrix(RhabList, nrow = numExamples, byrow = True, dimnames = Rdims)
		RhabName = str("test_HAB_MATR")
		robjects.globalEnv[RhabName] = RhabMatrix
			
		### Create the data frame in R ###
		RhabFrName = str("test_HAB_FRAME")
		r(RhabFrName + " = data.frame(" + RhabName + ")")
		
		###################################### SEQUENCE DATA ########################################
		
		### Get the species names for indexing
		speciesList = set()
		for seqLayer in GenGIS.layerTree.GetSequenceLayers():
			seqController = seqLayer.GetController()
			if seqController.IsActive():
				speciesList.add(seqController.GetData()[self.speciesLabel])
		
		### Hash of hashes to store the species count data
		speciesCounts = {}
		#for lab in labelsToCreate:
		#	speciesCounts[lab] = {}
			
		### Iterate through the sequence data and build the dictionary of counts
		for loc in locs:
			
			### Create the entry for this location
			siteID = loc.GetName()
			speciesCounts[siteID] = {}
			
			### Add values for each observed species
			seqLayers = loc.GetAllActiveSequenceLayers()
			for seqLayer in seqLayers:
				data = seqLayer.GetController().GetData()
				speciesCounts[siteID][data[self.speciesLabel]] = data[self.countField]
		

		### Build species data frame for each subset of samples
			
		### The list of names ###
	       	PyNameList = speciesCounts.keys()
	       	numExamples = len(PyNameList)
			
		RnameList = r["c"](robjects.StrVector(PyNameList))
		RspList = r["c"](robjects.StrVector(list(speciesList)))
		Rdims = r["list"](RnameList,RspList)
			
		RvarName = str("test_SP_NAME")
		robjects.globalEnv[RvarName] = RnameList
			
		### The biodiversity matrix ###
		PyDivList = []
		for addName in PyNameList:
			for addSpecies in speciesList:
				if addSpecies in speciesCounts[addName].keys():
					PyDivList.append(float(speciesCounts[addName][addSpecies]))
				else:
					PyDivList.append(0.0)
			
		### Create the matrix and send it to R ###
		RdivList = r["c"](robjects.FloatVector(PyDivList))
		RdivMatrix = r.matrix(RdivList, nrow = numExamples, byrow = True, dimnames = Rdims)
		RmatName = str("test_SP_MATR")
		robjects.globalEnv[RmatName] = RdivMatrix
			
		### Create the data frame in R ###
		RfrName = str("test_SP_FRAME")
		r(RfrName + " = data.frame(" + RmatName + ")")

		#pass the name of the rca model
		#r("rca_model <- " + rca_model)
		############################################ RCA R CODE ################################################
			
		r('''
                #location of R library
		infile <- paste(hDir,"rca_functions.r",sep="")
		source(infile)

                #location of RCA model
                model<-paste(hDir,"atlantic_rca_model.RData",sep="")

                #Run the analysis
                rca_results<-run_test_rca(test_HAB_FRAME,test_SP_FRAME,model)
                
                #output results to file
		#outfile <- paste(hDir,"OE_test_GG.csv",sep="")
                #write.csv(rca_results,file=outfile, row.names=FALSE)

                ''')

		#the following two lists must have the same number of elements and be in the same order
		diversity_measures_in_r_land=['Richness','Shannon','Simpson','Pielou','Berger_Parker']
		diversity_measures_in_plugin=['O/E\nRichness','O/E\nShannon','O/E\nSimpson','O/E\nPielou','O/E\nBerger_Parker']


		#import pdb; pdb.set_trace()
	
		#Save the results in a python dict
		self.results ={}
		site_ids=list(r("rca_results$" + 'site_ids'))
		for x,metric in enumerate(diversity_measures_in_r_land):
			raw_data=list(r("rca_results$" + metric))
			for i,value in enumerate(raw_data):
				self.results[(diversity_measures_in_plugin[x], site_ids[i])]=value
			
		self.locs=locs

	#	if colToPlot:
	#			self.ViewportPlot(colToPlot)
			
	def clearLines(self):
		for id in self.graphicalElementIds:
			GenGIS.graphics.RemoveLine(id)
		GenGIS.viewport.Refresh()
		
	def SaveResults(self,filepath):
                f = open(filepath,'w')
                tuples=self.results.keys()
                measures,site_ids=zip(*tuples)
                measures=list(set(measures))
                site_ids=list(set(site_ids))
                header=['site_ids']+measures
       

                #write header                                                                                                                                           
                f.write(",".join(header)+"\n")

                #write data                                                                                                                                             
                for site in site_ids:
                        row = [site]+[self.results[metric,site] for metric in measures]
                        f.write(",".join(map(str,row))+"\n")

                f.close()
		

	def ViewportPlot(self,metric,userScaleFactor=1.0):

		#pull out data values for only the metric specified
		data=[self.results[t] for t in self.results.keys() if t[0]==metric]

		#Remove any previously plotted lines
		self.clearLines()

		print "\nGenerating plot using diversity measure: " + metric
		terrainController = GenGIS.layerTree.GetMapLayer(0).GetController()
		
		# desired plot attributes
		lineWidth = 5
							
		maxValue = max(max(data), abs(min(data)))
		if maxValue != 0:
			scaleFactor = (0.2 * terrainController.GetWidth()) / maxValue
		else:
			scaleFactor = 0
		scaleFactor *= userScaleFactor

		# get min and max values for mapping colours
		if min(data) < 0 and max(data) > 0:
			# have both negative and positive values so map negative values to 
			# first half of map and positive values to the second half of the map
			maxValue = max(max(data), -min(data))
			minValue = -maxValue
		else:
			# all data is either negative or positive so just map over the full colour map
			#minValue = min(data)
			#maxValue = max(data)
			minValue = 0.0
			maxValue = 1.0

		# plot data
		self.graphicalElementIds = []
		for i,locLayer in enumerate(self.locs):			
			site_id = locLayer.GetName()
			value=self.results[metric,site_id]
			geoCoord = GenGIS.GeoCoord(locLayer.GetController().GetLongitude(), locLayer.GetController().GetLatitude())
			pos = GenGIS.Point3D()
			terrainController.GeoToGrid(geoCoord, pos)

			colourMap = GenGIS.colourMapManager.GetColourMap('Diverging (Red-White)')
			print site_id + "   " + str(value)
			colour = colourMap.GetInterpolatedColour(min(value,1.0), minValue, maxValue)
			
			endPos = GenGIS.Point3D(pos.x, scaleFactor * abs(value), pos.z)
			line = GenGIS.VisualLine(colour, lineWidth, GenGIS.LINE_STYLE.SOLID, GenGIS.Line3D(pos, endPos))
			lineId = GenGIS.graphics.AddLine(line)
			self.graphicalElementIds.append(lineId)
		
		GenGIS.viewport.Refresh()
