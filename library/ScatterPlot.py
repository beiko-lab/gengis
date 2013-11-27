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


from AbstractPlot import AbstractPlot

from scipy.stats import linregress
from scipy import polyval

class ScatterPlot(AbstractPlot):
	def __init__(self, parent):
		AbstractPlot.__init__(self, parent, (255,255,255))
		
	def Draw(self, x, y, xLabel = None, yLabel = None, slope = None, intercept = None, r2_value = None):
	
		"""Draw scatter plot."""
		self.Clear()
		axesScatter = self.figure.add_axes([0.15,0.1,0.80,0.85]) 
		
		# set labels
		if yLabel:
			axesScatter.set_ylabel(yLabel)
			
		if xLabel:
			axesScatter.set_xlabel(xLabel)
			
		# draw scatter plot
		axesScatter.scatter(x, y)

		# plot linear regression line
		if slope != None and intercept != None and slope != 0:
			print "one"
			linearX = [min(x), max(x)]
			linearY = polyval([slope, intercept], linearX)
			axesScatter.plot(linearX, linearY)
			print linearX
			print linearY
		
		print r2_value
		
		if r2_value:
			print "two"
			axesScatter.text(0.02, 0.98, r'R$^2$ = ' + ('%0.3f' % r2_value), horizontalalignment='left', verticalalignment='top', transform=axesScatter.transAxes)
		
		# make plot pretty
		for a in axesScatter.yaxis.majorTicks:
			a.tick1On=True
			a.tick2On=False
				
		for a in axesScatter.xaxis.majorTicks:
			a.tick1On=True
			a.tick2On=False
					
		for loc, spine in axesScatter.spines.iteritems():
			if loc in ['right','top']:
				spine.set_color('none')
		
		self.canvas.draw()