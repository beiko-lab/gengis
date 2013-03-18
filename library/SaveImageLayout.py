# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class SaveImageLayout
###########################################################################

class SaveImageLayout ( wx.Dialog ):
	
	def __init__( self, parent ):
		wx.Dialog.__init__ ( self, parent, id = wx.ID_ANY, title = u"Save image...", pos = wx.DefaultPosition, size = wx.Size( 380,136 ), style = wx.CAPTION|wx.DEFAULT_DIALOG_STYLE|wx.RESIZE_BORDER|wx.STAY_ON_TOP )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		
		bSizer4 = wx.BoxSizer( wx.VERTICAL )
		
		fgSizer1 = wx.FlexGridSizer( 2, 3, 0, 0 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText1 = wx.StaticText( self, wx.ID_ANY, u"Filename:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText1.Wrap( -1 )
		fgSizer1.Add( self.m_staticText1, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.txtFilename = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.txtFilename.SetMinSize( wx.Size( 200,-1 ) )
		
		fgSizer1.Add( self.txtFilename, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_btnSelectFilename = wx.Button( self, wx.ID_ANY, u"Select", wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer1.Add( self.m_btnSelectFilename, 0, wx.ALL, 5 )
		
		self.m_staticText2 = wx.StaticText( self, wx.ID_ANY, u"Resolution:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText2.Wrap( -1 )
		fgSizer1.Add( self.m_staticText2, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboResolutionChoices = [ u"Native screen resolution", u"100 DPI", u"200 DPI", u"300 DPI", u"600 DPI" ]
		self.cboResolution = wx.ComboBox( self, wx.ID_ANY, u"300 DPI", wx.DefaultPosition, wx.DefaultSize, cboResolutionChoices, wx.CB_READONLY )
		fgSizer1.Add( self.cboResolution, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer4.Add( fgSizer1, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer5 = wx.BoxSizer( wx.HORIZONTAL )
		
		
		bSizer5.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_btnOK = wx.Button( self, wx.ID_OK, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_btnOK.SetDefault() 
		bSizer5.Add( self.m_btnOK, 0, wx.ALL, 5 )
		
		self.m_btnCancel = wx.Button( self, wx.ID_CANCEL, u"Cancel", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer5.Add( self.m_btnCancel, 0, wx.ALL, 5 )
		
		bSizer4.Add( bSizer5, 0, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer4 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.m_btnSelectFilename.Bind( wx.EVT_BUTTON, self.OnSelectFilename )
		self.cboResolution.Bind( wx.EVT_COMBOBOX, self.OnResolutionChanged )
		self.m_btnOK.Bind( wx.EVT_BUTTON, self.OnOK )
		self.m_btnCancel.Bind( wx.EVT_BUTTON, self.OnCancel )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnSelectFilename( self, event ):
		event.Skip()
	
	def OnResolutionChanged( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	
	def OnCancel( self, event ):
		event.Skip()
	

