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
