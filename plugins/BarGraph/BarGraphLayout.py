# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class BarGraphLayout
###########################################################################

class BarGraphLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Bar Graph", pos = wx.DefaultPosition, size = wx.Size( 1383,785 ), style = wx.CAPTION|wx.CLOSE_BOX|wx.SYSTEM_MENU|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( -1,165 ), wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer1 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer7 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer3 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Select Field Type" ), wx.VERTICAL )
		
		bSizer82 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.lblFieldSel = wx.StaticText( self, wx.ID_ANY, u"Field: ", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblFieldSel.Wrap( -1 )
		bSizer82.Add( self.lblFieldSel, 1, wx.ALL, 5 )
		
		chcFieldSelChoices = []
		self.chcFieldSel = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, chcFieldSelChoices, 0 )
		self.chcFieldSel.SetSelection( 0 )
		bSizer82.Add( self.chcFieldSel, 0, wx.ALL, 5 )
		
		sbSizer3.Add( bSizer82, 1, wx.EXPAND, 5 )
		
		bSizer7.Add( sbSizer3, 0, wx.EXPAND, 5 )
		
		szrLocation = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Location" ), wx.HORIZONTAL )
		
		fgSizer1 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.lblLoc1Sel = wx.StaticText( self, wx.ID_ANY, u"Location 1:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblLoc1Sel.Wrap( -1 )
		fgSizer1.Add( self.lblLoc1Sel, 0, wx.ALL, 5 )
		
		chcLocSelChoices = []
		self.chcLocSel = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, chcLocSelChoices, 0 )
		self.chcLocSel.SetSelection( 0 )
		fgSizer1.Add( self.chcLocSel, 0, wx.ALL|wx.ALIGN_BOTTOM|wx.EXPAND, 5 )
		
		self.lblLoc2Sel = wx.StaticText( self, wx.ID_ANY, u"Location 2:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblLoc2Sel.Wrap( -1 )
		fgSizer1.Add( self.lblLoc2Sel, 0, wx.ALL, 5 )
		
		chcLocSel2Choices = []
		self.chcLocSel2 = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, chcLocSel2Choices, 0 )
		self.chcLocSel2.SetSelection( 0 )
		fgSizer1.Add( self.chcLocSel2, 0, wx.ALL, 5 )
		
		szrLocation.Add( fgSizer1, 1, wx.EXPAND, 5 )
		
		bSizer7.Add( szrLocation, 0, wx.EXPAND, 5 )
		
		szrVariable = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Variable" ), wx.HORIZONTAL )
		
		self.lblVarSel = wx.StaticText( self, wx.ID_ANY, u"Variable:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblVarSel.Wrap( -1 )
		szrVariable.Add( self.lblVarSel, 0, wx.ALL, 5 )
		
		
		szrVariable.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		chcVarSelChoices = []
		self.chcVarSel = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, chcVarSelChoices, 0 )
		self.chcVarSel.SetSelection( 0 )
		szrVariable.Add( self.chcVarSel, 0, wx.ALL|wx.EXPAND, 5 )
		
		bSizer7.Add( szrVariable, 0, wx.EXPAND, 5 )
		
		bSizer9 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer7.Add( bSizer9, 1, wx.EXPAND, 5 )
		
		bSizer8 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnHelp = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		self.m_btnHelp.SetToolTipString( u"Help" )
		
		bSizer8.Add( self.m_btnHelp, 0, wx.ALL, 5 )
		
		
		bSizer8.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnCalculate = wx.Button( self, wx.ID_ANY, u"Calculate", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer8.Add( self.btnCalculate, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		bSizer7.Add( bSizer8, 0, wx.EXPAND, 5 )
		
		bSizer1.Add( bSizer7, 0, wx.EXPAND, 5 )
		
		bSizer5 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer81 = wx.BoxSizer( wx.VERTICAL )
		
		self.pnlHistogram = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.SIMPLE_BORDER|wx.TAB_TRAVERSAL )
		self.pnlHistogram.SetForegroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_CAPTIONTEXT ) )
		self.pnlHistogram.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_CAPTIONTEXT ) )
		
		bSizer81.Add( self.pnlHistogram, 1, wx.EXPAND |wx.ALL, 5 )
		
		bSizer5.Add( bSizer81, 1, wx.EXPAND, 5 )
		
		bSizer3 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.btnClose = wx.Button( self, wx.ID_ANY, u"Close", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer3.Add( self.btnClose, 0, wx.ALL, 5 )
		
		bSizer5.Add( bSizer3, 0, wx.ALIGN_RIGHT, 5 )
		
		bSizer1.Add( bSizer5, 1, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer1 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_SIZE, self.OnSize )
		self.chcFieldSel.Bind( wx.EVT_CHOICE, self.OnChoice )
		self.m_btnHelp.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.btnCalculate.Bind( wx.EVT_BUTTON, self.OnCalculate )
		self.btnClose.Bind( wx.EVT_BUTTON, self.OnClose )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnSize( self, event ):
		event.Skip()
	
	def OnChoice( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnCalculate( self, event ):
		event.Skip()
	
	def OnClose( self, event ):
		event.Skip()
	

