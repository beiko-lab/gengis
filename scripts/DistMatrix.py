# Class for reading and visualizing the contents of a distance matrix in PHYLIP format.
#
# Usage:
# locLayers = GenGIS.layerTree.GetLocationSetLayer(0).GetAllLocationLayers()
#
# import distMatrix
# dm = distMatrix.distMatrix('myDistMatrix.phylip', locLayers)
# dm.SetActiveTriangle('upper')
# dm.DrawLines(['GBR'],['*'])

import GenGIS
import dataHelper as dh

class DistMatrix:
	def __init__(self, filename, locLayers):
		"""
		Class for reading and visualizing the contents of a distance matrix in PHYLIP format. 
				
		@param filename: name of file containing distance matrix
		@param locLayers: location layers for sites specified in distance matrix
		"""
		
		# read in distance matrix
		fin = open(filename)
		matrix = fin.readlines()
		fin.close()
		
		# parse distance matrix
		self.index = {}
		self.matrix = []
		self.minDist = 1e10
		self.maxDist = -1e10
		for i in xrange(1, len(matrix)):
			data = matrix[i].split()
			self.index[data[0]] = i-1
			
			row = []
			for j in xrange(1,len(data)):
				dist = float(data[j])
				if dist < self.minDist:
					self.minDist = dist
					
				if dist > self.maxDist:
					self.maxDist = dist
				row.append(dist)
				
			self.matrix.append(row)

		# get dictionary indicating the geographic coordinates of each location site
		self.locDict = {}
		for loc in locLayers:
			self.locDict[loc.GetController().GetId()] = [loc.GetController().GetLongitude(), loc.GetController().GetLatitude()]
			
		self.minSize = 2
		self.maxSize = 8
		
		self.startColour = GenGIS.Colour(1.0, 1.0, 1.0, 1.0)
		self.endColour = GenGIS.Colour(1.0, 0.0, 0.0, 1.0)
			
	def SetLineSizes(self, minSize, maxSize):
		"""
		Set min and max size of lines.
		"""
		self.minSize = minSize
		self.maxSize = maxSize
			
	def SetLineColours(self, startColour, endColour):
		"""
		Set start and end colour of lines.
		"""
		self.startColour = startColour
		self.endColour = endColour
			
	def __InterpolateSize(self, dist):
		"""
		Interpolate size of line.
		
		@param dist: distance value
		@return Size of line with a distance value of dist.
		"""
		offset = (dist - self.minDist) / (self.maxDist - self.minDist)
		return self.minSize + offset*(self.maxSize-self.minSize)
	
	def __InterpolateColour(self, dist):
		"""
		Interpolate size of line.
		
		@param dist: distance value
		@return Colour of line with a distance of dist.
		"""
		offset = (dist - self.minDist) /  (self.maxDist - self.minDist)
		return GenGIS.Colour(self.startColour.GetRed() + offset*(self.endColour.GetRed()-self.startColour.GetRed()), 
													self.startColour.GetGreen() + offset*(self.endColour.GetGreen() - self.startColour.GetGreen()), 
													self.startColour.GetBlue() + offset*(self.endColour.GetBlue() - self.startColour.GetBlue()), 
													self.startColour.GetAlpha() + offset*(self.endColour.GetAlpha() - self.startColour.GetAlpha()))

	def DrawLines(self, lines):
		"""
		Draw lines from between locations specified in distance matrix. For example,
		lines=[['A', 'B']] will draw a line from A to B. The distance for this line 
		will be taken from row A and column B. Multiple lines	can be draw using 
		lines = [['A','B'],['C','D']]. 
		
		@param lines: specifies lines to draw as a list of lists
		"""
					
		# add graph lines to GenGIS Viewport
		terrainController = GenGIS.layerTree.GetMapLayer(0).GetController()
		for line in lines:
			r = self.index[line[0]]
			c = self.index[line[1]]
			
			dist = self.matrix[r][c]
	
			startPt = GenGIS.Point3D()
			terrainController.GeoToGrid(GenGIS.GeoCoord(self.locDict[line[0]][0], self.locDict[line[0]][1]), startPt)
		
			endPt = GenGIS.Point3D()
			terrainController.GeoToGrid(GenGIS.GeoCoord(self.locDict[line[1]][0], self.locDict[line[1]][1]), endPt)
		
			size = self.__InterpolateSize(dist)
			colour = self.__InterpolateColour(dist)
		
			line = GenGIS.VisualLine(colour, size, GenGIS.LINE_STYLE.SOLID, GenGIS.Line3D(startPt, endPt))
			GenGIS.graphics.AddLine(line)
		
			GenGIS.viewport.Refresh()
				
	def DrawTopEntries(self, locationId, num, bMax, bRow):
		'''
		Draw lines indicating top \'num\' minimum or maximum entries for a given location.
		
		@param locationId: location of interest
		@param num: number of lines to draw
		@param bMax: indicates if lines should be drawn for the maximum (True) or minimum (False) entries
		@param bRow: indicates if distance are read along a row (True) or column (False)
		'''
		
		GenGIS.graphics.ClearLines()
		
		# get import or export distance from given location to all other locations
		locIndex = self.index[locationId]
		dist = {}
		if bRow:
			for key in self.index:
				dist[key] = self.matrix[locIndex][self.index[key]]
		else:
			for key in self.index:
				dist[key] = self.matrix[self.index[key]][locIndex]

		# sort dictionary by value
		items = [(v, k) for k, v in dist.items()]
		items.sort()
		
		if bMax:
			items.reverse()
		items = [(k, v) for v, k in items]
		
		# set min and max distances
		self.maxDist = -1e-10
		self.minDist = 1e10
		for i in xrange(0,num):
			dist = items[i][1]
			if dist < self.minDist:
				self.minDist = dist
			if dist > self.maxDist:
				self.maxDist = dist
    
		# draw top 'num' lines
		terrainController = GenGIS.layerTree.GetMapLayer(0).GetController()
		for i in xrange(0,num):
			curLocId = items[i][0]
			dist = items[i][1]
						
			startPt = GenGIS.Point3D()
			terrainController.GeoToGrid(GenGIS.GeoCoord(self.locDict[locationId][0], self.locDict[locationId][1]), startPt)
			
			endPt = GenGIS.Point3D()
			terrainController.GeoToGrid(GenGIS.GeoCoord(self.locDict[curLocId][0], self.locDict[curLocId][1]), endPt)
			
			size = self.__InterpolateSize(dist)
			colour = self.__InterpolateColour(dist)
			
			line = GenGIS.VisualLine(colour, size, GenGIS.LINE_STYLE.SOLID, GenGIS.Line3D(startPt, endPt))
			GenGIS.graphics.AddLine(line)
			
			GenGIS.viewport.Refresh()
