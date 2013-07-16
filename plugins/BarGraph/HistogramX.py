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

import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas

class HistogramX(AbstractPlot):
	def __init__(self, parent):
		AbstractPlot.__init__(self, parent, (255,255,255))
		
	def Draw(self, x, yOne, yTwo, location, locationTwo, stdOne = None, stdTwo = None,
				xLabel = None, xTicks = None):
		"""Draw Bar Graph"""

		self.figure = matplotlib.figure.Figure()
		N = len(x)

		ind = np.arange(N)  # the x locations for the groups
		width = 0.35       # the width of the bars

		ax = self.figure.add_subplot(111)
		rects1 = ax.bar(ind, yOne, width, color = 'g', yerr = stdOne)
		rects2 = ax.bar(ind + width , yTwo, width, color = 'r', yerr = stdTwo)
		
		# add some
		ax.set_ylabel("Frequency")
		ax.set_xlabel(xLabel)
		if (locationTwo == 'N/A'):
			ax.set_title("Frequency of " + xLabel + " at site " + location)
		else:
			ax.set_title("Frequency of " + xLabel + " at sites " + location + " and " + locationTwo)
		ax.set_xticks(ind + width)
		ax.set_xticklabels( xTicks ) #, rotation = 'vertical' )
		ax.legend( (rects1[0], rects2[0]), (location, locationTwo) )
		
		ax.set_ylim(ymin=0)
		
		self.figure.autofmt_xdate()
		self.canvas = FigureCanvas(self, -1, self.figure)

		self.SetPlotSize()
	