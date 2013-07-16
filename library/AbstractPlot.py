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

import wx

from SaveImageDlg import SaveImageDlg

class AbstractPlot(wx.Panel):
	def __init__(self, parent, color=None, dpi=None, **kwargs):
		from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg
		from matplotlib.figure import Figure
		
		# set default matplotlib parameters
		import matplotlib as mpl
		fontSize = 10
		mpl.rcParams['font.size'] = fontSize
		mpl.rcParams['axes.titlesize'] = fontSize
		mpl.rcParams['axes.labelsize'] = fontSize
		mpl.rcParams['xtick.labelsize'] = fontSize
		mpl.rcParams['ytick.labelsize'] = fontSize
		mpl.rcParams['legend.fontsize'] = fontSize

		# initialize Panel
		if 'id' not in kwargs.keys():
				kwargs['id'] = wx.ID_ANY
		if 'style' not in kwargs.keys():
				kwargs['style'] = wx.NO_FULL_REPAINT_ON_RESIZE
		wx.Panel.__init__(self, parent, **kwargs)

		# initialize matplotlib stuff
		self.figure = Figure(None, dpi)
		self.canvas = FigureCanvasWxAgg(self, -1, self.figure)
		self.SetColor( color )

		self.SetPlotSize()
		
		self._resizeFlag = False
		
		parent.Bind(wx.EVT_SIZE, self._OnSize)
		self.Bind(wx.EVT_IDLE, self._OnIdle)
		
	def _OnSize(self, event):
		self._resizeFlag = True
		
	def _OnIdle(self, event):
		if self._resizeFlag:
			self._resizeFlag = False
			self.SetPlotSize()

	def SetColor( self, rgbtuple=None ):
		"""Set figure and canvas colours to be the same."""
		if rgbtuple is None:
				rgbtuple = wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ).Get()
		clr = [c/255. for c in rgbtuple]
		self.figure.set_facecolor( clr )
		self.figure.set_edgecolor( clr )
		self.canvas.SetBackgroundColour( wx.Colour( *rgbtuple ) )

	def SetPlotSize( self ):
		""" Set plot to fill entire frame. """
		pixels = tuple( self.GetParent().GetClientSize() )
		self.SetSize( pixels )
		self.canvas.SetSize( pixels )
		self.figure.set_size_inches( float( pixels[0] )/self.figure.get_dpi(),
																 float( pixels[1] )/self.figure.get_dpi() )

	def Draw(self):
		"""Function to be overridden in derived class."""
		pass
		
	def DrawEmptyPlot(self):
		"""Draw empty plot."""
		self.figure.clf()
		
		emptyAxis = self.figure.add_axes([0.1,0.1,0.85,0.85]) 
		
		emptyAxis.set_ylabel('y axis')
		emptyAxis.set_xlabel('x axis')
																		 
		self.canvas.draw()
		
	def Clear(self):
		""" Clear current figure. """
		self.figure.clf()
		
	def SavePlot(self, dpi=300):
		"""Save plot to file."""
		saveImageDlg = SaveImageDlg(self)

		if saveImageDlg.ShowModal() == wx.ID_OK:
			if saveImageDlg.filename != None:
				self.figure.savefig(saveImageDlg.filename, format=saveImageDlg.format, dpi=saveImageDlg.DPI, facecolor='white', edgecolor='white')
	
		saveImageDlg.Destroy()
		
		
