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
		if slope != None and intercept != None:
			linearX = [min(x), max(x)]
			linearY = polyval([slope, intercept], linearX)
			axesScatter.plot(linearX, linearY)
		
		if r2_value:
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