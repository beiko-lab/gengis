#=======================================================================
# Author: Donovan Parks
#
# Copyright 2009 Donovan Parks
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


# Provides common measures of alpha diversity.
#
# Author: Donovan Parks
#
# To add a new measure:
#  1. Add the measure to self.measure
#  2. Add the measure to the if/elif clause in Info() and GetMeasureFunction()
#  3. Write a function implementing the measure

import math
import random

from numpy import mean

import wx

class AlphaDiversityMeasures:
	"""Different alpha diversity measures."""
	def __init__(self):
		self.measures = ['Gini-Simpson index', 'Richness', 'Shannon index', 'Shannon diversity', 'Simpson index', 'Simpson diversity', 'Sum of Counts']
		
	def Info(self, measure):
		if measure == 'Gini-Simpson index':
			return ['Gini-Simpson diversity index', '1-sum(pi*pi)', 'A common index used to characterize community diversity which accounts for both abundance and evenness. Also referred to as the probability of interspecific encounter.']
		elif measure == 'Richness':
			return ['Species richness', '', 'The total number of different organisms present in a community.']
		elif measure == 'Shannon index':
			return ["Shannon's diversity index", '-sum(pi log pi)', 'An information theoretic index used to characterize community diversity which accounts for both abundance and evenness.']
		elif measure == 'Shannon diversity':
			return ["Shannon's diversity", 'exp(-sum(pi log pi))', "Effective number of species as determined using Shannon's diversity index.\n\nSee Jost L. et al. 2006. Entropy and diversity. Oikos, 113:363-375."]
		elif measure == 'Simpson index':
			return ["Simpson's diversity index", 'sum(pi*pi)', 'A common index used to characterize community diversity which accounts for both abundance and evenness. Gives the probability of two entities drawn at random being the same type. Index decreases with increasing diversity.']
		elif measure == 'Simpson diversity':
			return ["Simpson's diversity", '1/sum(pi*pi)', "Effective number of species as determined using Simpson's diversity index.\n\nSee Jost L. et al. 2006. Entropy and diversity. Oikos, 113:363-375."]
		elif measure == 'Sum of Counts':
			return ["Sum of Counts", 'x1+x2',"A summation of all Count Field values for all given locations."]
			
	def GetMeasureFunction(self, measure):
		measureFunc = None
		if measure == 'Gini-Simpson index':
			measureFunc = self.GiniSimpsonIndex
		elif measure == 'Richness':
			measureFunc = self.Richness
		elif measure == 'Shannon index':
			measureFunc =  self.ShannonIndex
		elif measure == 'Simpson index':
			measureFunc =  self.SimpsonIndex
		elif measure == 'Shannon diversity':
			measureFunc =  self.ShannonDiversity
		elif measure == 'Simpson diversity':
			measureFunc =  self.SimpsonDiversity
		elif measure == 'Sum of Counts':
			measureFunc = self.SumOfCounts
			
		return measureFunc
			
	def Calculate(self, measure, sampleDict, bIgnoreOtherUnclassified):
		"""
		Calculate specified alpha diversity measure.
		  sampleDict: a dictionary indictaing the number of sequences for each category for each sample
		  bIgnoreOtherUnclassified: ignore fields labelled Other or Unclassified
		"""
		
		# find all categories
		categories = set([])
		for sampleName in sampleDict:
			categoryDict = sampleDict[sampleName]
			for category in categoryDict:
				categories.add(category)
		
		# create data dictionary indicate category counts for each sample
		dataDict = {}
		for sampleName in sampleDict:
			dataDict[sampleName] = []
			
			categoryDict = sampleDict[sampleName]
			for category in categories:
				if bIgnoreOtherUnclassified and (category.lower() == 'other' or category.lower() == 'unclassified'):
					continue
					
				if category in categoryDict:
					dataDict[sampleName].append(categoryDict[category])
				else:
					dataDict[sampleName].append(0)

		# calculate specified alpha diversity measure
		measureFunc = self.GetMeasureFunction(measure)
		if( measure != 'Sum of Counts' ):
			return measureFunc(dataDict)
		else:
			return measureFunc(sampleDict)
		
	def CalculateJackknife(self, measure, sampleDict, bIgnoreOtherUnclassified, replicates, seqsToDraw):
		"""
		Calculate average of specified alpha diversity measure over jackknife replicates.
		  sampleDict: a dictionary indictaing the number of sequences for each category for each sample
		  replicates: number of jackknife replicates to perform
		  seqsToDraw: sequence to draw for each jackknife replicate
		  bIgnoreOtherUnclassified: ignore fields labelled Other or Unclassified
		"""
		
		# find all categories
		categories = set([])
		for sampleName in sampleDict:
			categoryDict = sampleDict[sampleName]
			for category in categoryDict:
				categories.add(category)
				
		# create data dictionary indicate category counts for each sample
		dataDict = {}
		for sampleName in sampleDict:
			dataDict[sampleName] = []
			
			categoryDict = sampleDict[sampleName]
			for category in categories:
				if bIgnoreOtherUnclassified and (category.lower() == 'other' or category.lower() == 'unclassified'):
					continue
					
				if category in categoryDict:
					dataDict[sampleName].append(categoryDict[category])
				else:
					dataDict[sampleName].append(0)
					
		# get specified alpha diversity measure
		measureFunc = self.GetMeasureFunction(measure)
		
		# get mean over replicates for each sample
		dialog = wx.ProgressDialog ( 'Calculating jackknife values', 'Calculating jackknife values for sample:' + ' '*30, maximum = len(dataDict) )
		results = {}
		index = 1
		for sampleName in dataDict:
			dialog.Update(index, 'Calculating jackknife values for sample: ' + sampleName + ' (' + str(index) + ' of ' + str(len(dataDict)) + ')')
			profile = dataDict[sampleName]
			numSeqs = int(sum(profile))
			
			jackknifeValues = []
			tempDataDict = {}
			if numSeqs < seqsToDraw:
				# can't jackknife a profile with fewer sequences than
				# what is being drawn so just use original profile
				tempDataDict[sampleName] = profile
				jackknifeValues.append(measureFunc(tempDataDict)[sampleName])
			else:
				# randomly draw the specified number of sequences without replacement
				for i in xrange(0, replicates):
					drawnSeqs = random.sample(xrange(numSeqs), seqsToDraw)	# sample without replacement
					
					jackknifeProfile = [0]*len(profile)
					for drawnSeq in drawnSeqs:
						seqs = 0
						for j in xrange(0, len(profile)):
							seqs += profile[j]
							if drawnSeq < seqs:
								jackknifeProfile[j] += 1
								break
								
					tempDataDict[sampleName] = jackknifeProfile
					jackknifeValues.append(measureFunc(tempDataDict)[sampleName])
				
			results[sampleName] = mean(jackknifeValues)
			index += 1
			
		return results
		
	def Richness(self, dataDict):
		"""
		Calculate species richness index for each sample.
		  dataDict: dictionary with category counts for each sample
		"""
		richness = {}
		for sampleName in dataDict:
			profile = dataDict[sampleName]
			
			alpha = 0
			for count in profile:
				if count != 0:
					alpha += 1
			
			richness[sampleName] = alpha
			
		return richness
		
	def ShannonIndex(self, dataDict):
		"""
		Calculate Shannon index for each sample.
		  dataDict: dictionary with category counts for each sample
		"""
		shannon = {}
		for sampleName in dataDict:
			profile = dataDict[sampleName]
			
			H = 0
			total = sum(profile)
			for count in profile:
				if count != 0:
					p = float(count)/total
					H -= p * math.log(p)
			
			shannon[sampleName] = H
			
		return shannon
		
	def SimpsonIndex(self, dataDict):
		"""
		Calculate Simpson index for each sample.
		  dataDict: dictionary with category counts for each sample
		"""
		simpson = {}
		for sampleName in dataDict:
			profile = dataDict[sampleName]
			
			D = 0
			total = sum(profile)
			for count in profile:
				p = float(count)/total
				D += p*p
			
			simpson[sampleName] = D
			
		return simpson
		
	def ShannonDiversity(self, dataDict):
		"""
		Calculate Shannon diversity for each sample.
		  dataDict: dictionary with category counts for each sample
		"""
		shannon = {}
		for sampleName in dataDict:
			profile = dataDict[sampleName]
			
			H = 0
			total = sum(profile)
			for count in profile:
				if count != 0:
					p = float(count)/total
					H -= p * math.log(p)
			
			shannon[sampleName] = math.exp(H)
			
		return shannon
		
	def SimpsonDiversity(self, dataDict):
		"""
		Calculate Simpson diversity for each sample.
		  dataDict: dictionary with category counts for each sample
		"""
		simpson = {}
		for sampleName in dataDict:
			profile = dataDict[sampleName]
			
			D = 0
			total = sum(profile)
			for count in profile:
				p = float(count)/total
				D += p*p
			
			simpson[sampleName] = 1.0/D
			
		return simpson
		
	def GiniSimpsonIndex(self, dataDict):
		"""
		Calculate Simpson index for each sample.
		  dataDict: dictionary with category counts for each sample
		"""
		simpson = {}
		for sampleName in dataDict:
			profile = dataDict[sampleName]
			
			D = 0
			total = sum(profile)
			for count in profile:
				p = float(count)/total
				D += p*p
			
			simpson[sampleName] = 1.0 - D
			
		return simpson
		
	def SumOfCounts(self,sampleDict):
		"""
		Calculate the sum of counts for each sample.
		  dataDict: dictionary with category counts for each sample
		"""
		simpson = {}
		for sampleName in sampleDict:
			values = sampleDict[sampleName]
			total = 0
			for key in values:
				total += values[key]
			
			simpson[sampleName] = total
			
		return simpson