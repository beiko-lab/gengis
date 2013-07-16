#=======================================================================
# Author: Donovan Parks
#
# Copyright 2012 Brett O'Donnell
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
from numpy import ma
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas

class MultiTreeGraph(AbstractPlot):
	def __init__(self, parent):
		AbstractPlot.__init__(self, parent, (255,255,255))
		
	def Draw(self, x, y, yMin):
		"""Draw Step Graph"""

		self.figure = matplotlib.figure.Figure()

		ax = self.figure.add_subplot(111)
		ax.step(x, y, label='Global Total')

		ax.set_ylabel("Number of Crossings")

		ax.legend()

		ax.set_xlim(90, 270)
		ax.set_ylim(ymin = yMin - 1)

		self.canvas = FigureCanvas(self, -1, self.figure)

		self.SetPlotSize()