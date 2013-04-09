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
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Bar Graph", pos = wx.DefaultPosition, size = wx.Size( 1383,785 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( -1,165 ), wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer1 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer7 = wx.BoxSizer( wx.VERTICAL )
		
		szrLocation = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Groups to Plot" ), wx.HORIZONTAL )
		
		fgSizer3 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer3.SetFlexibleDirection( wx.BOTH )
		fgSizer3.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.lblFieldSel = wx.StaticText( self, wx.ID_ANY, u"Field: ", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblFieldSel.Wrap( -1 )
		fgSizer3.Add( self.lblFieldSel, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboGroupFieldChoices = []
		self.cboGroupField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboGroupFieldChoices, 0 )
		self.cboGroupField.SetSelection( 0 )
		fgSizer3.Add( self.cboGroupField, 1, wx.BOTTOM|wx.EXPAND|wx.LEFT, 5 )
		
		self.lblLoc1Sel = wx.StaticText( self, wx.ID_ANY, u"Group 1:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblLoc1Sel.Wrap( -1 )
		fgSizer3.Add( self.lblLoc1Sel, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboGroup1Choices = []
		self.cboGroup1 = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboGroup1Choices, 0 )
		self.cboGroup1.SetSelection( 0 )
		fgSizer3.Add( self.cboGroup1, 1, wx.BOTTOM|wx.EXPAND|wx.LEFT|wx.TOP, 5 )
		
		self.lblLoc2Sel = wx.StaticText( self, wx.ID_ANY, u"Group 2:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblLoc2Sel.Wrap( -1 )
		fgSizer3.Add( self.lblLoc2Sel, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboGroup2Choices = []
		self.cboGroup2 = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboGroup2Choices, 0 )
		self.cboGroup2.SetSelection( 0 )
		fgSizer3.Add( self.cboGroup2, 1, wx.EXPAND|wx.LEFT|wx.TOP, 5 )
		
		szrLocation.Add( fgSizer3, 1, wx.EXPAND, 5 )
		
		bSizer7.Add( szrLocation, 0, wx.EXPAND, 5 )
		
		szrVariable = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Data to Plot" ), wx.HORIZONTAL )
		
		fgSizer2 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer2.SetFlexibleDirection( wx.BOTH )
		fgSizer2.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.lblVarSel = wx.StaticText( self, wx.ID_ANY, u"Field:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblVarSel.Wrap( -1 )
		fgSizer2.Add( self.lblVarSel, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboDataFieldChoices = []
		self.cboDataField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboDataFieldChoices, 0 )
		self.cboDataField.SetSelection( 0 )
		fgSizer2.Add( self.cboDataField, 1, wx.BOTTOM|wx.EXPAND|wx.LEFT, 5 )
		
		self.m_staticText5 = wx.StaticText( self, wx.ID_ANY, u"Count field:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText5.Wrap( -1 )
		fgSizer2.Add( self.m_staticText5, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboCountFieldChoices = []
		self.cboCountField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboCountFieldChoices, 0 )
		self.cboCountField.SetSelection( 0 )
		fgSizer2.Add( self.cboCountField, 1, wx.EXPAND|wx.LEFT|wx.TOP, 5 )
		
		szrVariable.Add( fgSizer2, 1, wx.EXPAND, 0 )
		
		bSizer7.Add( szrVariable, 0, wx.EXPAND, 5 )
		
		self.btnPlot = wx.Button( self, wx.ID_ANY, u"Plot", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer7.Add( self.btnPlot, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		
		bSizer7.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		bSizer8 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnHelp = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		self.m_btnHelp.SetToolTipString( u"Help" )
		
		bSizer8.Add( self.m_btnHelp, 0, wx.ALL, 5 )
		
		
		bSizer8.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		bSizer7.Add( bSizer8, 0, wx.EXPAND, 5 )
		
		bSizer1.Add( bSizer7, 0, wx.EXPAND, 5 )
		
		bSizer5 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer81 = wx.BoxSizer( wx.VERTICAL )
		
		self.pnlHistogram = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.SIMPLE_BORDER|wx.TAB_TRAVERSAL )
		bSizer81.Add( self.pnlHistogram, 1, wx.EXPAND |wx.ALL, 5 )
		
		bSizer5.Add( bSizer81, 1, wx.EXPAND, 5 )
		
		bSizer3 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.btnSavePlot = wx.Button( self, wx.ID_ANY, u"Save plot", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer3.Add( self.btnSavePlot, 0, wx.ALL, 5 )
		
		
		bSizer3.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnOK = wx.Button( self, wx.ID_ANY, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer3.Add( self.btnOK, 0, wx.ALL, 5 )
		
		bSizer5.Add( bSizer3, 0, wx.EXPAND, 5 )
		
		bSizer1.Add( bSizer5, 1, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer1 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_SIZE, self.OnSize )
		self.cboGroupField.Bind( wx.EVT_CHOICE, self.OnChangeGroupField )
		self.cboCountField.Bind( wx.EVT_CHOICE, self.OnCountFieldChanged )
		self.btnPlot.Bind( wx.EVT_BUTTON, self.OnPlot )
		self.m_btnHelp.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.btnSavePlot.Bind( wx.EVT_BUTTON, self.OnSavePlot )
		self.btnOK.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnSize( self, event ):
		event.Skip()
	
	def OnChangeGroupField( self, event ):
		event.Skip()
	
	def OnCountFieldChanged( self, event ):
		event.Skip()
	
	def OnPlot( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnSavePlot( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

