# Class for superimosing a lat/long grid over the map.
#
# Usage:
#
# import LatLongGrid
# latLongGrid = LatLongGrid.LatLongGrid()
# latLongGrid.ShowGrid()
# ...
# latLongGrid.HideGrid()

import GenGIS

class LatLongGrid:
	def __init__(self):
		"""
		Class for superimosing a lat/long grid over the map.
		"""
		self.mapController = GenGIS.layerTree.GetMapLayer(0).GetController()
		self.extents =  self.mapController.GetProjectionExtents()
		print self.extents.x1
		print self.extents.y1
		print self.extents.dx
		print self.extents.dy

	def ShowGrid(self):
		colour = GenGIS.Colour(0.0, 0.0, 0.0, 1.0)
		size = 1
				
		# draw longitudinal grid
		longSpacing = 5
		startLong = int(self.extents.x1)
		
		curLong = startLong
		while curLong > self.extents.x1:
			startPt = GenGIS.Point3D()
			self.mapController.GeoToGrid(GenGIS.GeoCoord(curLong, self.extents.y1), startPt)
			
			endPt = GenGIS.Point3D()
			self.mapController.GeoToGrid(GenGIS.GeoCoord(curLong, self.extents.dy), endPt)

			line = GenGIS.VisualLine(colour, size, GenGIS.LINE_STYLE.SOLID, GenGIS.Line3D(startPt, endPt))
			GenGIS.graphics.AddLine(line)
			
			label = GenGIS.VisualLabel(str(curLong), colour, 10, GenGIS.LABEL_RENDERING_STYLE.ORTHO)
			label.SetGridPosition(startPt)
			GenGIS.graphics.AddLabel(label)
			
			curLong -= longSpacing
		
		curLong = startLong + longSpacing
		while curLong < self.extents.dx:
			startPt = GenGIS.Point3D()
			self.mapController.GeoToGrid(GenGIS.GeoCoord(curLong, self.extents.y1), startPt)
			
			endPt = GenGIS.Point3D()
			self.mapController.GeoToGrid(GenGIS.GeoCoord(curLong, self.extents.dy), endPt)

			line = GenGIS.VisualLine(colour, size, GenGIS.LINE_STYLE.SOLID, GenGIS.Line3D(startPt, endPt))
			GenGIS.graphics.AddLine(line)
			
			label = GenGIS.VisualLabel(str(curLong), colour, 10, GenGIS.LABEL_RENDERING_STYLE.ORTHO)
			label.SetGridPosition(startPt)
			GenGIS.graphics.AddLabel(label)
			
			curLong += longSpacing
			
		# draw latitudinal grid
		latSpacing = 5
		startLat = int(self.extents.y1)
		
		curLat = startLat
		while curLat > self.extents.y1:
			startPt = GenGIS.Point3D()
			self.mapController.GeoToGrid(GenGIS.GeoCoord(self.extents.x1, curLat), startPt)
			
			endPt = GenGIS.Point3D()
			self.mapController.GeoToGrid(GenGIS.GeoCoord(self.extents.dx, curLat), endPt)

			line = GenGIS.VisualLine(colour, size, GenGIS.LINE_STYLE.SOLID, GenGIS.Line3D(startPt, endPt))
			GenGIS.graphics.AddLine(line)
			
			label = GenGIS.VisualLabel(str(curLat), colour, 10, GenGIS.LABEL_RENDERING_STYLE.ORTHO)
			label.SetGridPosition(startPt)
			GenGIS.graphics.AddLabel(label)
			
			curLat -= latSpacing
		
		curLat = startLat + latSpacing
		while curLat < self.extents.dy:
			startPt = GenGIS.Point3D()
			self.mapController.GeoToGrid(GenGIS.GeoCoord(self.extents.x1, curLat), startPt)
			
			endPt = GenGIS.Point3D()
			self.mapController.GeoToGrid(GenGIS.GeoCoord(self.extents.dx, curLat), endPt)

			line = GenGIS.VisualLine(colour, size, GenGIS.LINE_STYLE.SOLID, GenGIS.Line3D(startPt, endPt))
			GenGIS.graphics.AddLine(line)
			
			label = GenGIS.VisualLabel(str(curLat), colour, 10, GenGIS.LABEL_RENDERING_STYLE.ORTHO)
			label.SetGridPosition(startPt)
			GenGIS.graphics.AddLabel(label)
			
			curLat += latSpacing
	
		GenGIS.viewport.Refresh()
