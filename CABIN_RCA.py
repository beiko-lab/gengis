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
	def __init__(self, setCol, spCol, countCol):
		self.setLabel = setCol
		self.speciesLabel = spCol
		self.countField = countCol
		
		self.graphicalElementIds = []

	### Run the reference condition analysis. Since this was initially written as an R script, most of the R code is simply preserved intact, with RPy used to pass data into and out of the original script. ###
	def Run_RCA(self, colToPlot = None):

		### Get the layer data ###
		locs = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
		testLocs = []
		
		### Get the label types ###
		labelsToCreate = set()
		for locX in locs:
			labelsToCreate.add(locX.GetController().GetData()[self.setLabel])
		
		#################################### ENVIRONMENTAL DATA #####################################
		envFields = ["geo_sed", "geo_intr", "geo_sedvol", "clim_Trange"]
		
		### Hash of hashes to store the env field data
		envValues = {}
		for lab in labelsToCreate:
			envValues[lab] = {}
			
		### Iterate through the environmental data and build dictionary of values
		for loc in locs:
		
			### Create the entry for this location
			locType = loc.GetController().GetData()[self.setLabel]
			if locType == "test":
				testLocs.append(loc)
			
			siteID = loc.GetController().GetData()["Site ID"]
			envValues[locType][siteID] = {}
			
			### Add values for each environmental value
			for envField in envFields:
				data = loc.GetController().GetData()[envField]
				envValues[locType][siteID][envField] = data
				
		### Build habitat data frame for each subset of samples
		for labDo in labelsToCreate:
			
			### The list of names ###
			PyNameList = envValues[labDo].keys()
			numExamples = len(PyNameList)
			
			RnameList = r["c"](robjects.StrVector(PyNameList))
			RenvList = r["c"](robjects.StrVector(envFields))
			Rdims = r["list"](RnameList,RenvList)
			
			RvarName = str(labDo + "_HAB_NAME")
			robjects.globalEnv[RvarName] = RnameList
			
			### The habitat value matrix ###
			PyHabList = []
			for addName in PyNameList:
				for addEnv in envFields:
					if addEnv in envValues[labDo][addName].keys():
						PyHabList.append(float(envValues[labDo][addName][addEnv]))
					else:
						PyHabList.append(0.0)
			
			### Create the matrix and send it to R ###
			RhabList = r["c"](robjects.FloatVector(PyHabList))
			RhabMatrix = r.matrix(RhabList, nrow = numExamples, byrow = True, dimnames = Rdims)
			RhabName = str(labDo + "_HAB_MATR")
			robjects.globalEnv[RhabName] = RhabMatrix
			
			### Create the data frame in R ###
			RhabFrName = str(labDo + "_HAB_FRAME")
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
		for lab in labelsToCreate:
			speciesCounts[lab] = {}
			
		### Iterate through the sequence data and build the dictionary of counts
		for loc in locs:
			
			### Create the entry for this location
			locType = loc.GetController().GetData()[self.setLabel]
			siteID = loc.GetController().GetData()["Site ID"]
			speciesCounts[locType][siteID] = {}
			
			### Add values for each observed species
			seqLayers = loc.GetAllActiveSequenceLayers()
			for seqLayer in seqLayers:
				data = seqLayer.GetController().GetData()
				speciesCounts[locType][siteID][data[self.speciesLabel]] = data[self.countField]
		
		### Build species data frame for each subset of samples
		for labDo in labelsToCreate:
			
			### The list of names ###
			PyNameList = speciesCounts[labDo].keys()
			numExamples = len(PyNameList)
			
			RnameList = r["c"](robjects.StrVector(PyNameList))
			RspList = r["c"](robjects.StrVector(list(speciesList)))
			Rdims = r["list"](RnameList,RspList)
			
			RvarName = str(labDo + "_SP_NAME")
			robjects.globalEnv[RvarName] = RnameList
			
			### The biodiversity matrix ###
			PyDivList = []
			for addName in PyNameList:
				for addSpecies in speciesList:
					if addSpecies in speciesCounts[labDo][addName].keys():
						PyDivList.append(float(speciesCounts[labDo][addName][addSpecies]))
					else:
						PyDivList.append(0.0)
			
			### Create the matrix and send it to R ###
			RdivList = r["c"](robjects.FloatVector(PyDivList))
			RdivMatrix = r.matrix(RdivList, nrow = numExamples, byrow = True, dimnames = Rdims)
			RmatName = str(labDo + "_SP_MATR")
			robjects.globalEnv[RmatName] = RdivMatrix
			
			### Create the data frame in R ###
			RfrName = str(labDo + "_SP_FRAME")
			r(RfrName + " = data.frame(" + RmatName + ")")

		############################################ RCA R CODE ################################################
			
		r('''
			require(vegan) # vegan 1.17-4
			require(cluster)
		''')

		############;
		# STEP 1 -- INITIAL SETUP -- Input and organize the biological and predictor data;
		# The exact code for step is very particular for each data set, but similar operations will need;
		# to be done on every data set;
		# Data setup below is one example that was used for building the Atlantic Canada model.
		###############################;
		r('''
		#data transformation
		log.trans.cal <- log(cal_HAB_FRAME[4]+1,10) # log transformation of continuous numerical variables
		arcsinesqroot.trans.cal <- asin(sqrt(cal_HAB_FRAME[1:3])) # arcsinesqroot transformation of % variables
		predcal <- cbind(log.trans.cal,arcsinesqroot.trans.cal)
		log.trans.val <- log(val_HAB_FRAME[4]+1,10) # log transformation of continuos numerical variables
		arcsinesqroot.trans.val <- asin(sqrt(val_HAB_FRAME[1:3])) # arcsinesqroot transformation of % variables
		predval <- cbind(log.trans.val,arcsinesqroot.trans.val)

		biolcal <- decostand(cal_SP_FRAME, "total") # Selection and transformation of biological variables for the calibration dataset: relative abundance data
		biolcal.pa <- decostand(cal_SP_FRAME,"pa") # Selection and transformation of biological variables for the calibration dataset: presence-absence data
		biolval <- decostand(val_SP_FRAME, "total") # Selection and transformation of biological variables for the validation dataset: relative abundance data
		biolval.pa <- decostand(val_SP_FRAME, "pa") # Selection and transformation of biological variables for the validation dataset: presence-absence data

		########################################; Import and process Test site data
		#data transformation
		log.trans.test <- log(test_HAB_FRAME[4]+1,10) # log transformation of continuous numerical variables
		arcsinesqroot.trans.test <- asin(sqrt(test_HAB_FRAME[1:3])) # arcsinesqroot transformation of % variables
		predtest <- cbind(log.trans.test,arcsinesqroot.trans.test)
		bioltest <- decostand(test_SP_FRAME, "total") # Selection and transformation of biological variables for the test dataset: relative abundance data
		bioltest.pa <- decostand(test_SP_FRAME,"pa") # Selection and transformation of biological variables for the test dataset: presence-absence data
		########################################;

		# Data alignment control
		# colnames(cal)==colnames(val);
		#row.names(biolcal)==row.names(predcal);
		#row.names(biolval)==row.names(predval);
		#colnames(biolcal)==colnames(biolval);
		#colnames(predcal)==colnames(predval);
		''')
		###########################
		
		#########################;
		# STEP 2 -- Dissimilarity computation and clustering of calibration sites;
		r('''
		braycurtis <- vegdist(biolcal, method="bray") # Bray-Curtis dissimilarity for relative abundance data
		clustering <- agnes(braycurtis, diss=TRUE, method = "average") # clustering using agnes average function
		grps <-cutree(as.hclust(clustering), k=3)  # group definition as per Armanini et al. 2011

		preds.final<-c("clim_Trange", "geo_intr", "geo_sed", "geo_sedvol")  # predictor selected using best-subset DFA approach: see Armanini et al., 2011
		
		''')
		################################
		
		################################
		#STEP 4 - Finalize calculations of final, chosen predictive model;
		r('''
		# To specify the entire final model, you need to store/export 5 things:
		# 4.1) The site-by-taxa matrix of observed presence/absence at calibration sites (bugcal.pa, already available);
		# 4.2) Specify the vector of final group membership assignments at calibration sites(grps.final);
		  grps.final<-grps
		# 4.3) Specify the final selected predictor variables (preds.final) - already done in step 3.5;
		# 4.4) Calculate the matrix(grpmns) of mean values of the preds.final variables for the final calibration site groups ;
		 datmat<-as.matrix(predcal[,preds.final])
		 grpmns<-apply(datmat,2,function(x)tapply(x,grps.final,mean));
		# 4.5) Calculate the inverse pooled covariance matrix(covpinv) of the preds.final variables at the calibration sites;
		 #First, calculate a list of covariance matrices for each group;
		  covlist<-lapply(split.data.frame(datmat,grps.final),cov);
		  #pooled cov matrix is weighted average of group matrices, weighted by group size. Johnson & Wichern, 11-64;
		  grpsiz<-table(grps.final);
		  ngrps<-length(grpsiz);
		  npreds<-length(preds.final);
		 #zero out an initial matrix for pooled covariance;
		 covpool<-matrix(rep(0,npreds*npreds),nrow=npreds,dimnames=dimnames(covlist[[1]]));
		 #weighted sum of covariance matrices;
		 for(i in 1:ngrps){covpool<-covpool+(grpsiz[i]-1)*covlist[[i]]};
		 covpool<-covpool/(sum(grpsiz)-ngrps);#renormalize;
		 covpinv<-solve(covpool); #inverse of pooled cov matrix;
		''')

		#Step 5 - Perform prediction with final model;
		r('''
		infile = paste(hDir,"model.predict.v4.1.r",sep="")
		source(infile);
		OE.assess.test<-model.predict.v4.1(bugcal.pa=biolcal.pa,grps.final,preds.final, grpmns,covpinv,prednew=predtest,bugnew=bioltest.pa,Pc=0.5)  ;
		OE.assess.test$OE.scores;
		outfile = paste(hDir,"OE_test_GG.csv",sep="")
		write.csv(OE.assess.test$OE.scores, file=outfile)
		''')

		if colToPlot:
			toPlot = list(r("OE.assess.test$OE.scores$" + colToPlot))
			self.ViewportPlot(toPlot,testLocs)
			
	def clearLines(self):
		for id in self.graphicalElementIds:
			GenGIS.graphics.RemoveLine(id)
		GenGIS.viewport.Refresh()
		
	def ViewportPlot(self,data,locations):

		print "Running ViewportPlot..."
		terrainController = GenGIS.layerTree.GetMapLayer(0).GetController()
		
		# desired plot attributes
		lineWidth = 5
		userScaleFactor = 0.2
							
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
		for i in xrange(0, len(locations)):
			locLayer = locations[i]
			geoCoord = GenGIS.GeoCoord(locLayer.GetController().GetLongitude(), locLayer.GetController().GetLatitude())
			pos = GenGIS.Point3D()
			terrainController.GeoToGrid(geoCoord, pos)

			colourMap = GenGIS.colourMapManager.GetColourMap('Diverging (Red-White)')
			print str(i) + "   " + str(data[i])
			colour = colourMap.GetInterpolatedColour(min(data[i],1.0), minValue, maxValue)
			
			endPos = GenGIS.Point3D(pos.x, scaleFactor * abs(data[i]), pos.z)
			line = GenGIS.VisualLine(colour, lineWidth, GenGIS.LINE_STYLE.SOLID, GenGIS.Line3D(pos, endPos))
			lineId = GenGIS.graphics.AddLine(line)
			self.graphicalElementIds.append(lineId)
		
		GenGIS.viewport.Refresh()
