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