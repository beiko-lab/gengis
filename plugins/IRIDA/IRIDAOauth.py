#
from rauth import OAuth2Service
import sys, GenGIS, wx, os, certifi
from IRIDAOauthLayout import IRIDAOauthLayout
from urllib3.contrib import pyopenssl
pyopenssl.inject_into_urllib3()

class IRIDAOauth(IRIDAOauthLayout):
	def __init__(self, IRIDA=None):
		IRIDAOauthLayout.__init__(self,None)
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico",wx.BITMAP_TYPE_ICO))
		self.interface = IRIDA
		self.success = False
	
	#Enters User Credentials
	def OnOK(self,event):
		wx.BeginBusyCursor()
		username = self.m_UsernameCtrl.GetValue()
		password = self.m_PasswordCtrl.GetValue()
		URL = "https://cs.dal.ca/~keddy/testing/authme.php"
		values = {'username': username,
					'password': password}
		
		# CALL SERVER AND PASS UNAME, PASSWORD
		#self.success=self.interface.AuthorizeToken()
		if self.success:
	#		self.interface.InitializeResults()
			wx.EndBusyCursor()
			self.Close()
		else:
			wx.EndBusyCursor()
		
	def OnClose(self, event):
		event.Skip()
		if not self.success:
			self.interface.Close()
	#	event.Skip()
	