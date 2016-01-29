# execfile("C:\Users\Admin\Documents\PEscript.py")
from PhylogeneticDistanceLayout import PhylogeneticDistanceLayout
import GenGIS
import wx
from scipy.spatial import Delaunay
import dendropy


class PhylogeneticDistance( PhylogeneticDistanceLayout ):
	
	combinationMethods = {"Average(Local)": "AVGLOCAL", "Average(Global)": "AVGGLOBAL", "Sum": "SUM"}
	
	def __init__(self, parent=None):
		PhylogeneticDistanceLayout.__init__( self, parent )
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0 or GenGIS.layerTree.GetNumTreeLayers() == 0:
			wx.MessageBox("This plugin requires map, location, and tree data to be loaded.", "Additional data required.")
			self.Close()
			return
		# load up measure options
		for keys in self.combinationMethods.keys():
			self.m_cboMeasure.Append(keys)
		self.m_cboMeasure.SetSelection(0)
		# load up location layers
		for locLayer in GenGIS.layerTree.GetLocationSetLayers():
			self.m_cboLocationLayerField.Append(locLayer.GetName())
		self.m_cboLocationLayerField.SetSelection(0)
		# load up tree layers
		for treeIndex in range(0,GenGIS.layerTree.GetNumTreeLayers()):
			tree = GenGIS.layerTree.GetTreeLayer(treeIndex)
			self.m_cboTreeLayerField.Append(tree.GetName())
		self.m_cboTreeLayerField.SetSelection(0)
		self.OnMeasureChanged("fake")
	
	def OnOK(self, event):
		self.Close()
	
	def OnMeasureChanged(self, event):
		if self.m_cboMeasure.GetStringSelection() == "Average(Local)":
			self.txtFullName.SetValue("Average(Local)")
			self.txtFormula.SetValue("(a + b)/count(a,b)")
			self.txtDescription.SetValue("This option combines edge weights by averaging every edge associated with a location. The sum of all associated weights is divided by the number of branches associated with that location. This accounts more strongly for differing edge counts between nodes.")
	
		elif self.m_cboMeasure.GetStringSelection() == "Average(Global)":
			self.txtFullName.SetValue("Average(Global)")
			self.txtFormula.SetValue("(a + b)/ (Number of Locations)")
			self.txtDescription.SetValue("This option combines edge weights by averaging every edge associated with a location. The sum of all associated weights is divided by the number of locations. This more strongly preserves network like signal")
		
		elif self.m_cboMeasure.GetStringSelection() == "Sum":
			self.txtFullName.SetValue("Sum")
			self.txtFormula.SetValue(" a + b ")
			self.txtDescription.SetValue("This option combines edge weights by summing every edge associated with a location.")
	
	def OnCalculate(self, event):
		# get selected location layer
		self.locSetIndex = self.m_cboLocationLayerField.GetSelection()
		# get selected tree layer
		self.treeIndex = self.m_cboTreeLayerField.GetSelection()
		# get selected combination method
		selectedMethod = self.m_cboMeasure.GetStringSelection()
		
		self.locWeights = self.Main( self.locSetIndex, self.treeIndex, self.combinationMethods[selectedMethod] )
		self.PrintWeights(self.locSetIndex, self.locWeights)
	
	def PrintWeights(self, locSetIndex, locWeights):
		locSet = GenGIS.layerTree.GetLocationSetLayer(locSetIndex)
		for i in range(0, locSet.GetNumLocationLayers() ):
			loc = locSet.GetLocationLayer(i)
			if i in locWeights.keys():
				value = locWeights[i]
			else:
				continue
			
			printStr = "%s: %s\n" %(loc.GetName(), value)
			self.txtLog.AppendText(printStr)
		self.txtLog.AppendText("\n\n")
		
	
	def OnAddToGenGIS(self, event):
		self.AddDataToLocationLayer()
	
	def CombineWeights(self, phyloWeights, comb):
		final = {}
		if comb == "SUM":
			for key in phyloWeights.keys():
				val = sum(phyloWeights[key])
				final[key] = val
				
		elif comb == "AVGLOCAL":
			for key in phyloWeights.keys():
				sm = sum(phyloWeights[key])
			#	leng = len(phyloWeights[key])
				leng = len(phyloWeights[key]) if len(phyloWeights[key])!=0 else 1
				val = sm/leng
				final[key]= val
				
		elif comb == "AVGGLOBAL":
			leng = 0
			for key in phyloWeights.keys():
				sm = sum(phyloWeights[key])
				leng = leng + len(phyloWeights[key])
				final[key] = sm
			for key,val in final.items():
				final[key] = val/leng
		return final

	# distribute all the edge weights to locations
	def WeightsToLocations(self, phyloWeights ):
		locations={}	
		for key,value in phyloWeights.items():
			loc1 = key[0]
			loc2 = key[1]
			if loc1 in locations.keys():
				locations[loc1] += value * .5
			else:
				locations[loc1] = value *.5
			
			if loc2 in locations.keys():
				locations[loc2] += value * .5
			else:
				locations[loc2] = value *.5
		return locations
			
	def AddDataToLocationLayer(self):
		# ADD DATA BACK TO LOCATIONS METADATA
		
		val = []
		name = "PhylogeneticDistance"
		for i in range(0,GenGIS.layerTree.GetNumLocationLayers()):
			if i in self.locWeights.keys():
				if isinstance(self.locWeights[i],float):
					val.append("%0.15f" %self.locWeights[i])
				else:
					val.append(str(self.locWeights[i]))
			#	val.append(""self.locWeights[i])
			else:
				val.append("0")
		GenGIS.layerTree.GetLocationSetLayer(self.locSetIndex).GetController().AddMetadata( name , val )

	def Main(self,locationLayer, treeLayer, combinationMethod):	
		#need at least location, and tree layers.
		# check required data has been loaded		
	#	treeLayer = 0
	#	locationLayer = 0
	#	combinationMethod = "AVG"
		
		# weights of every leaf node against every other
		newick = GenGIS.layerTree.GetTreeLayer(treeLayer).GetGeoTreeView().GetNewickString()
		leafNames = GenGIS.layerTree.GetTreeLayer(0).GetGeoTreeView().GetLeafNames()
		tree = dendropy.Tree.get_from_string(newick,schema="newick")
		pdm = dendropy.treecalc.PatristicDistanceMatrix(tree)
			
		# make array of points
		points = []
		treeLocationMap = {}
		PhyloDistance = {}
		activeLocations = GenGIS.layerTree.GetLocationSetLayer(locationLayer).GetAllActiveLocationLayers()
		i=0
		
		# map leaf names to nodes for easy use
		leafMap = {}
		for taxon in tree.taxon_set:
			for leaf in leafNames:
				if leaf == taxon.label:
					leafMap[leaf] = taxon
					continue
		
		for loc in activeLocations:
			taxonObjects = []
			
			east = loc.GetController().GetEasting()
			north = loc.GetController().GetNorthing()
			points.append([east,north])
			
			if loc.GetName() in leafNames:
				taxonObjects.append( leafMap[ loc.GetName() ] )
			# translate sequence ID to taxon objects
			if loc.GetNumSequenceLayers() > 0:
				for seq in loc.GetAllActiveSequenceLayers():
					if seq.GetName() in leafNames:
						taxonObjects.append( leafMap[ seq.GetName() ] )
						continue
			
			treeLocationMap[i] = taxonObjects	
			i += 1

		tri = Delaunay(points)
		# indices are in the same order as stored by GenGIS in the Active Location Layer Set
		
		indices = tri.simplices
		pairs = []
		for line in indices:
			pairs.append((line[0],line[1]))
			pairs.append((line[0],line[2]))
			pairs.append((line[1],line[2]))

		# total weight to divide pairwise weights by (0-1 metric)
		totalWeight = tree.length()

		max = 0
		for pair in pairs:
			a = pair[0]
			b = pair[1]
			dist = 0
			'''
			if (a,b) in PhyloDistance.keys():
				weights = []
				for t1 in treeLocationMap[a]:
					for t2 in treeLocationMap[b]:
						weights.append(1-pdm(t1, t2))
				PhyloDistance[a,b].extend(weights)
			else:
				weights = []
				for t1 in treeLocationMap[a]:
					for t2 in treeLocationMap[b]:
						weights.append(1-pdm(t1, t2))
				if len(weights) > 0:
					PhyloDistance[a,b] = weights
			'''
			weights = []
			for t1 in treeLocationMap[a]:
				for t2 in treeLocationMap[b]:
					dist = pdm(t1,t2)
					weights.append(dist)
					if dist > max:
						max = dist
			if (a,b) in PhyloDistance.keys():
				PhyloDistance[a,b].extend(weights)
			else:
				PhyloDistance[a,b] = weights
		flipWeights = True
		if flipWeights:
			# flip PD so that closely related things have the largest values
			for key, val in PhyloDistance.iteritems():
				weights = []
				for wght in val:
					wght = max - wght
					weights.append(wght)
			
				PhyloDistance[key] = weights
		phyloDistComb = self.CombineWeights(PhyloDistance, combinationMethod)
	#	print phyloDistComb
		locationWeights = self.WeightsToLocations(phyloDistComb)
		return locationWeights
