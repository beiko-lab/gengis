# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class EnvironmentDataVisualizerLayout
###########################################################################

class EnvironmentDataVisualizerLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Environmental Data Visualizer", pos = wx.DefaultPosition, size = wx.Size( 300,349 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 300,349 ), wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer5 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer6 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer2 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Viewport Display" ), wx.VERTICAL )
		
		fgSizer1 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText3 = wx.StaticText( self, wx.ID_ANY, u"Field:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText3.Wrap( -1 )
		fgSizer1.Add( self.m_staticText3, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboFieldChoices = []
		self.cboField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboFieldChoices, 0 )
		self.cboField.SetSelection( 0 )
		fgSizer1.Add( self.cboField, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText4 = wx.StaticText( self, wx.ID_ANY, u"Colour map:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText4.Wrap( -1 )
		fgSizer1.Add( self.m_staticText4, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboColourMapChoices = []
		self.cboColourMap = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboColourMapChoices, 0 )
		self.cboColourMap.SetSelection( 0 )
		fgSizer1.Add( self.cboColourMap, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText5 = wx.StaticText( self, wx.ID_ANY, u"Line width:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText5.Wrap( -1 )
		fgSizer1.Add( self.m_staticText5, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.spinLineWidth = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 0, 10, 5 )
		fgSizer1.Add( self.spinLineWidth, 0, wx.ALL, 5 )
		
		self.m_staticText6 = wx.StaticText( self, wx.ID_ANY, u"Scale factor:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText6.Wrap( -1 )
		fgSizer1.Add( self.m_staticText6, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		bSizer8 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.txtScaleFactor = wx.TextCtrl( self, wx.ID_ANY, u"1.0", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer8.Add( self.txtScaleFactor, 0, wx.ALL|wx.EXPAND, 5 )
		
		fgSizer1.Add( bSizer8, 1, wx.EXPAND, 5 )
		
		sbSizer2.Add( fgSizer1, 0, wx.EXPAND, 5 )
		
		self.m_staticline1 = wx.StaticLine( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.LI_HORIZONTAL )
		sbSizer2.Add( self.m_staticline1, 0, wx.EXPAND |wx.ALL, 5 )
		
		fgSizer3 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer3.SetFlexibleDirection( wx.BOTH )
		fgSizer3.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText8 = wx.StaticText( self, wx.ID_ANY, u"Dot Type:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText8.Wrap( -1 )
		fgSizer3.Add( self.m_staticText8, 0, wx.ALL, 5 )
		
		cboViewportDotChoices = [ u"Environmental Data", u"<none>" ]
		self.cboViewportDot = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboViewportDotChoices, 0 )
		self.cboViewportDot.SetSelection( 0 )
		fgSizer3.Add( self.cboViewportDot, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText13 = wx.StaticText( self, wx.ID_ANY, u"Colour Map:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText13.Wrap( -1 )
		fgSizer3.Add( self.m_staticText13, 0, wx.ALL, 5 )
		
		cboColourMapDotChoices = []
		self.cboColourMapDot = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboColourMapDotChoices, 0 )
		self.cboColourMapDot.SetSelection( 0 )
		fgSizer3.Add( self.cboColourMapDot, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText14 = wx.StaticText( self, wx.ID_ANY, u"Dot Size:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText14.Wrap( -1 )
		fgSizer3.Add( self.m_staticText14, 0, wx.ALL, 5 )
		
		self.spinDotSize = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 0, 10, 5 )
		fgSizer3.Add( self.spinDotSize, 0, wx.ALL, 5 )
		
		sbSizer2.Add( fgSizer3, 0, wx.EXPAND, 5 )
		
		bSizer6.Add( sbSizer2, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer81 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnHelp = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		self.m_btnHelp.SetToolTipString( u"Help" )
		
		bSizer81.Add( self.m_btnHelp, 0, wx.ALL, 5 )
		
		self.btnCalculate = wx.Button( self, wx.ID_ANY, u"Calculate", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer81.Add( self.btnCalculate, 0, wx.ALL|wx.EXPAND, 5 )
		
		
		bSizer81.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_Close = wx.Button( self, wx.ID_ANY, u"Close", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer81.Add( self.m_Close, 0, wx.ALL, 5 )
		
		bSizer6.Add( bSizer81, 0, wx.EXPAND, 5 )
		
		bSizer5.Add( bSizer6, 1, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer5 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.cboField.Bind( wx.EVT_CHOICE, self.OnPlotTypeChanged )
		self.cboColourMap.Bind( wx.EVT_CHOICE, self.OnColorMapChanged )
		self.spinLineWidth.Bind( wx.EVT_SPINCTRL, self.OnLineWidthChanged )
		self.cboViewportDot.Bind( wx.EVT_CHOICE, self.OnDotTypeChanged )
		self.cboColourMapDot.Bind( wx.EVT_CHOICE, self.OnDotColorMapChanged )
		self.spinDotSize.Bind( wx.EVT_SPINCTRL, self.OnDotSizeChanged )
		self.m_btnHelp.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.btnCalculate.Bind( wx.EVT_BUTTON, self.OnCalculate )
		self.m_Close.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnPlotTypeChanged( self, event ):
		event.Skip()
	
	def OnColorMapChanged( self, event ):
		event.Skip()
	
	def OnLineWidthChanged( self, event ):
		event.Skip()
	
	def OnDotTypeChanged( self, event ):
		event.Skip()
	
	def OnDotColorMapChanged( self, event ):
		event.Skip()
	
	def OnDotSizeChanged( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnCalculate( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

