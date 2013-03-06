# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Oct  8 2012)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx
import wx.xrc

###########################################################################
## Class RCALayout
###########################################################################

class RCALayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Reference Condition Approach", pos = wx.DefaultPosition, size = wx.Size( 303,260 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer1 = wx.BoxSizer( wx.VERTICAL )
		
		fgSizer1 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText2 = wx.StaticText( self, wx.ID_ANY, u"Taxa Names:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText2.Wrap( -1 )
		fgSizer1.Add( self.m_staticText2, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboDiversityChoices = []
		self.cboDiversity = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboDiversityChoices, 0 )
		self.cboDiversity.SetSelection( 0 )
		fgSizer1.Add( self.cboDiversity, 0, wx.ALL, 5 )
		
		self.m_staticText3 = wx.StaticText( self, wx.ID_ANY, u"Taxa Counts:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText3.Wrap( -1 )
		fgSizer1.Add( self.m_staticText3, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboCountChoices = []
		self.cboCount = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboCountChoices, 0 )
		self.cboCount.SetSelection( 0 )
		fgSizer1.Add( self.cboCount, 0, wx.ALL, 5 )
		
		self.m_staticText1 = wx.StaticText( self, wx.ID_ANY, u"Diversity Measure:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText1.Wrap( -1 )
		fgSizer1.Add( self.m_staticText1, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboMetricChoices = []
		self.cboMetric = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboMetricChoices, 0 )
		self.cboMetric.SetSelection( 0 )
		fgSizer1.Add( self.cboMetric, 0, wx.ALL, 5 )
		
		self.btnRun = wx.Button( self, wx.ID_ANY, u"Run", wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer1.Add( self.btnRun, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.btnUpdate = wx.Button( self, wx.ID_ANY, u"Update Plot", wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer1.Add( self.btnUpdate, 0, wx.ALL, 5 )
		
		self.m_staticText4 = wx.StaticText( self, wx.ID_ANY, u"Save Results:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText4.Wrap( -1 )
		fgSizer1.Add( self.m_staticText4, 0, wx.ALL, 5 )
		
		self.filePicker = wx.FilePickerCtrl( self, wx.ID_ANY, u"rca_results.csv", u"Save file as", u"*.*", wx.DefaultPosition, wx.DefaultSize, wx.FLP_SAVE )
		fgSizer1.Add( self.filePicker, 0, wx.ALL, 5 )
		
		
		bSizer1.Add( fgSizer1, 1, wx.EXPAND, 5 )
		
		bSizer3 = wx.BoxSizer( wx.HORIZONTAL )
		
		
		bSizer3.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btOK = wx.Button( self, wx.ID_ANY, u"Close", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer3.Add( self.btOK, 0, wx.ALL, 5 )
		
		
		bSizer1.Add( bSizer3, 0, wx.EXPAND, 5 )
		
		
		self.SetSizer( bSizer1 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.btnRun.Bind( wx.EVT_BUTTON, self.OnRun )
		self.btnUpdate.Bind( wx.EVT_BUTTON, self.OnUpdate )
		self.filePicker.Bind( wx.EVT_FILEPICKER_CHANGED, self.onSaveFile )
		self.btOK.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnRun( self, event ):
		event.Skip()
	
	def OnUpdate( self, event ):
		event.Skip()
	
	def onSaveFile( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

