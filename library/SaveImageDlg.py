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


from SaveImageLayout import SaveImageLayout

import GenGIS
import wx

class SaveImageDlg( SaveImageLayout ):

	def __init__(self, parent=None):
		SaveImageLayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		self.filename = None
		self.DPI = 300
		
	def OnResolutionChanged( self, event ):
		if self.cboResolution.GetValue() == "Native screen resolution":
			self.DPI = 96
		elif self.cboResolution.GetValue() == "100 DPI":
			self.DPI = 100
		elif self.cboResolution.GetValue() == "200 DPI":
			self.DPI = 200
		elif self.cboResolution.GetValue() == "300 DPI":
			self.DPI = 300
		elif self.cboResolution.GetValue() == "600 DPI":
			self.DPI = 600
		
	def OnSelectFilename(self, event):
		fileTypes = 'Portable Network Graphics (*.png)|*.png|Portable Document Format (*.pdf)|*.pdf|PostScript (*.ps)|*.ps|Encapsulated PostScript (*.eps)|*.eps|Scalable Vector Graphics (*.svg)|*.svg'
		
		dlg = wx.FileDialog(self, "Save plot", "", "", fileTypes, wx.SAVE)
		if dlg.ShowModal() == wx.ID_OK:
			self.format = dlg.GetFilename()[dlg.GetFilename().rfind('.')+1:len(dlg.GetFilename())]
			if self.format in ['png', 'pdf', 'ps', 'eps','svg']:
				self.filename = dlg.GetDirectory() + '\\' + dlg.GetFilename()
				self.txtFilename.SetValue(self.filename)
			else:
				wx.MessageBox('The specified image extension is unknown.', 'Uknown file type')
	
		dlg.Destroy()
