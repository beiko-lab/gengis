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


# Provides common measures of beta diversity.
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

class BetaDiversityMeasures:
	"""Different alpha diversity measures."""
	def __init__(self):
		self.measures = ['Bray-Curtis','Sorensen','Canberra','Euclidean','Manhattan','Hamming','Soergel','Jaccard','Simple matching']
		
	def Info(self, measure):
		if measure == 'Bray-Curtis':
			return ['Bray-Curtis index', 'sum(|pi-pj|)/sum(pi+pj)', 'Quantitative measure bound between 0 (similar) and 1 (dissimilar).\n\nBray JR and Curtis JT. 1957. An ordination of upland forest communities of southern Wisconsin. Ecological Monographs 27:325-349.']
		elif measure == 'Sorensen':
			return ['Sorensen index', '(b+c)/(2a+b+c)', 'Qualitative measure.']
		elif measure == 'Canberra':
			return ['Canberra index', 'sum(|pi-pj|/(pi+pj))', 'Quantitative measure.']
		elif measure == 'Euclidean':
			return ['Euclidean distance', 'sqrt(sum((pi-pj)^2))', 'Quantitative measure.']
		elif measure == 'Manhattan':
			return ['Manhattan distance', 'sum(|pi-pj|)', 'Quantitative measure.']
		elif measure == 'Hamming':
			return ['Hamming distance', 'b+c', 'Qualitative measure.']
		elif measure == 'Soergel':
			return ['Soergel index', 'sum(|pi-pj|)/sum(max(pi,pj))', 'Quantitative measure.']
		elif measure == 'Jaccard':
			return ['Jaccard index', '(b+c)/(a+b+c)', 'Qualitative measure.']
		elif measure == 'Simple matching':
			return ['Simple matching coefficient', '(b+c)/(a+b+c+d)', 'Qualitative measure.']

	def GetMeasureFunction(self, measure):
		measureFunc = None
		if measure == 'Bray-Curtis':
			measureFunc = self.BrayCurtis
		elif measure == 'Sorensen':
			measureFunc = self.Sorensen
		elif measure == 'Canberra':
			measureFunc = self.Canberra
		elif measure == 'Euclidean':
			measureFunc = self.Euclidean
		elif measure == 'Manhattan':
			measureFunc = self.Manhattan
		elif measure == 'Hamming':
			measureFunc = self.Hamming
		elif measure == 'Soergel':
			measureFunc = self.Soergel
		elif measure == 'Jaccard':
			measureFunc = self.Jaccard
		elif measure == 'Simple matching':
			measureFunc = self.SimpleMatching
			
		return measureFunc
			
	def Calculate(self, measure, sampleDict, bIgnoreOtherUnclassified):
		"""
		Calculate specified beta diversity measure.
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
		
		# calculate specified beta diversity measure
		measureFunc = self.GetMeasureFunction(measure)
		return measureFunc(dataDict)
		
	def Jackknife(self, profile, numSeqs, seqsToDraw):
		# jackknife profile
		if numSeqs < seqsToDraw:
			# can't jackknife a profile with fewer sequences than
			# what is being drawn so just use original profile
			jackknifeProfile = profile
		else:
			# randomly draw the specified number of sequences without replacement
			drawnSeqs = random.sample(xrange(numSeqs), seqsToDraw)	# sample without replacement
			
			jackknifeProfile = [0]*len(profile)
			for drawnSeq in drawnSeqs:
				seqs = 0
				for j in xrange(0, len(profile)):
					seqs += profile[j]
					if drawnSeq < seqs:
						jackknifeProfile[j] += 1
						break
						
		return jackknifeProfile
		
	def CalculateJackknife(self, measure, sampleDict, bIgnoreOtherUnclassified, replicates, seqsToDraw):
		"""
		Calculate average of specified beta diversity measure over jackknife replicates.
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
		numPairs = (len(dataDict)*len(dataDict)-len(dataDict))/2
		dialog = wx.ProgressDialog ( 'Calculating jackknife values', 'Calculating jackknife values for sample pair:' + ' '*30, maximum = numPairs )
		results = {}
		index = 1
		
		sampleNames = dataDict.keys()
		for i in xrange(0, len(sampleNames)):
			sampleNameI = sampleNames[i]
			profileI = dataDict[sampleNameI]
			numSeqsI = int(sum(profileI))
			
			row = {}
			for j in xrange(i+1, len(sampleNames)):
				sampleNameJ = sampleNames[j]
				profileJ = dataDict[sampleNameJ]
				numSeqsJ = int(sum(profileJ))
				
				dialog.Update(index, 'Calculating jackknife values for sample pair: ' + sampleNameI + '-' + sampleNameJ + ' (' + str(index) + ' of ' + str(numPairs) + ')')
				index += 1
				
				jackknifeValues = []
				for r in xrange(0, replicates):
					jackknifeProfileI = self.Jackknife(profileI, numSeqsI, seqsToDraw)
					jackknifeProfileJ = self.Jackknife(profileJ, numSeqsJ, seqsToDraw)

					tempDataDict = {}
					tempDataDict[sampleNameI] = jackknifeProfileI
					tempDataDict[sampleNameJ] = jackknifeProfileJ

					dissDict = measureFunc(tempDataDict)
					keys = dissDict.keys()
					if len(dissDict[keys[0]]) == 1:
						diss = dissDict[keys[0]][dissDict[keys[0]].keys()[0]]
					else:
						diss = dissDict[keys[1]][dissDict[keys[1]].keys()[0]]
					jackknifeValues.append(diss)
					
				row[sampleNameJ] = mean(jackknifeValues)
				
			results[sampleNameI] = row
			
		return results
		
	def BrayCurtis(self, dataDict):
		results = {}
		
		keys = dataDict.keys()
		for i in xrange(0, len(dataDict)):
			profileI = dataDict[keys[i]]
			totalI = sum(profileI)
			
			row = {}
			for j in xrange(i+1, len(dataDict)):
				profileJ = dataDict[keys[j]]
				totalJ = sum(profileJ)
				
				num = 0
				den = 0
				for k in xrange(0, len(profileI)):
					pi = float(profileI[k])/totalI
					pj = float(profileJ[k])/totalJ
					
					num += abs(pi-pj)
					den += pi+pj
					
				row[keys[j]] = num / den
			
			results[keys[i]] = row
			
		return results
		
	def Canberra(self, dataDict):
		results = {}
		
		keys = dataDict.keys()
		for i in xrange(0, len(dataDict)):
			profileI = dataDict[keys[i]]
			totalI = sum(profileI)
			
			row = {}
			for j in xrange(i+1, len(dataDict)):
				profileJ = dataDict[keys[j]]
				totalJ = sum(profileJ)
				
				diss = 0
				for k in xrange(0, len(profileI)):
					pi = float(profileI[k])/totalI
					pj = float(profileJ[k])/totalJ
					
					if pi+pj > 0:
						diss += abs(pi-pj)/(pi+pj)
					
				row[keys[j]] = diss
			
			results[keys[i]] = row
			
		return results
		
	def Euclidean(self, dataDict):
		results = {}
		
		keys = dataDict.keys()
		for i in xrange(0, len(dataDict)):
			profileI = dataDict[keys[i]]
			totalI = sum(profileI)
			
			row = {}
			for j in xrange(i+1, len(dataDict)):
				profileJ = dataDict[keys[j]]
				totalJ = sum(profileJ)
				
				diss = 0
				for k in xrange(0, len(profileI)):
					pi = float(profileI[k])/totalI
					pj = float(profileJ[k])/totalJ
					
					diss += (pi-pj)*(pi-pj)
					
				row[keys[j]] = math.sqrt(diss)
			
			results[keys[i]] = row
			
		return results
		
	def Manhattan(self, dataDict):
		results = {}
		
		keys = dataDict.keys()
		for i in xrange(0, len(dataDict)):
			profileI = dataDict[keys[i]]
			totalI = sum(profileI)
			
			row = {}
			for j in xrange(i+1, len(dataDict)):
				profileJ = dataDict[keys[j]]
				totalJ = sum(profileJ)
				
				diss = 0
				for k in xrange(0, len(profileI)):
					pi = float(profileI[k])/totalI
					pj = float(profileJ[k])/totalJ
					
					diss += abs(pi-pj)
					
				row[keys[j]] = diss
			
			results[keys[i]] = row
			
		return results
		
	def Soergel(self, dataDict):
		results = {}
		
		keys = dataDict.keys()
		for i in xrange(0, len(dataDict)):
			profileI = dataDict[keys[i]]
			totalI = sum(profileI)
			
			row = {}
			for j in xrange(i+1, len(dataDict)):
				profileJ = dataDict[keys[j]]
				totalJ = sum(profileJ)
				
				num = 0
				den = 0
				for k in xrange(0, len(profileI)):
					pi = float(profileI[k])/totalI
					pj = float(profileJ[k])/totalJ
					
					num += abs(pi-pj)
					den += max(pi, pj)
					
				row[keys[j]] = num/den
			
			results[keys[i]] = row
			
		return results
			
	def Sorensen(self, dataDict):
		results = {}
		
		keys = dataDict.keys()
		for i in xrange(0, len(dataDict)):
			profileI = dataDict[keys[i]]
			
			row = {}
			for j in xrange(i+1, len(dataDict)):
				profileJ = dataDict[keys[j]]
				
				a = 0
				b = 0
				c = 0
				d = 0
				for k in xrange(0, len(profileI)):
					pi = float(profileI[k])
					pj = float(profileJ[k])
					
					if pi > 0 and pj > 0:
						a += 1
					elif pi > 0 and pj == 0:
						b += 1
					elif pi == 0 and pj > 0:
						c += 1
					else:
						d += 1

				row[keys[j]] = float(b+c)/(2*a+b+c)
			
			results[keys[i]] = row
			
		return results
		
	def Hamming(self, dataDict):
		results = {}
		
		keys = dataDict.keys()
		for i in xrange(0, len(dataDict)):
			profileI = dataDict[keys[i]]
			
			row = {}
			for j in xrange(i+1, len(dataDict)):
				profileJ = dataDict[keys[j]]
				
				a = 0
				b = 0
				c = 0
				d = 0
				for k in xrange(0, len(profileI)):
					pi = float(profileI[k])
					pj = float(profileJ[k])
					
					if pi > 0 and pj > 0:
						a += 1
					elif pi > 0 and pj == 0:
						b += 1
					elif pi == 0 and pj > 0:
						c += 1
					else:
						d += 1

				row[keys[j]] = (b+c)
			
			results[keys[i]] = row
			
		return results
		
	def Jaccard(self, dataDict):
		results = {}
		
		keys = dataDict.keys()
		for i in xrange(0, len(dataDict)):
			profileI = dataDict[keys[i]]
			
			row = {}
			for j in xrange(i+1, len(dataDict)):
				profileJ = dataDict[keys[j]]
				
				a = 0
				b = 0
				c = 0
				d = 0
				for k in xrange(0, len(profileI)):
					pi = float(profileI[k])
					pj = float(profileJ[k])
					
					if pi > 0 and pj > 0:
						a += 1
					elif pi > 0 and pj == 0:
						b += 1
					elif pi == 0 and pj > 0:
						c += 1
					else:
						d += 1

				row[keys[j]] = float(b+c)/(a+b+c)
			
			results[keys[i]] = row
			
		return results
		
	def SimpleMatching(self, dataDict):
		results = {}
		
		keys = dataDict.keys()
		for i in xrange(0, len(dataDict)):
			profileI = dataDict[keys[i]]
			
			row = {}
			for j in xrange(i+1, len(dataDict)):
				profileJ = dataDict[keys[j]]
				
				a = 0
				b = 0
				c = 0
				d = 0
				for k in xrange(0, len(profileI)):
					pi = float(profileI[k])
					pj = float(profileJ[k])
					
					if pi > 0 and pj > 0:
						a += 1
					elif pi > 0 and pj == 0:
						b += 1
					elif pi == 0 and pj > 0:
						c += 1
					else:
						d += 1

				row[keys[j]] = float(b+c)/(a+b+c+d)
			
			results[keys[i]] = row
			
		return results
