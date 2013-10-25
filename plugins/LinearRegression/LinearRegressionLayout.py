# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class LinearRegressionLayout
###########################################################################

class LinearRegressionLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Linear Regression", pos = wx.DefaultPosition, size = wx.Size( 1200,750 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer1 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer5 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer6 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer1 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Linear regression" ), wx.VERTICAL )
		
		gSizer1 = wx.GridSizer( 1, 2, 0, 0 )
		
		cboIndependentVariableDataTypeChoices = [ u"Use Location Data", u"Use Sequence Data" ]
		self.cboIndependentVariableDataType = wx.RadioBox( self, wx.ID_ANY, u"Independent Data", wx.DefaultPosition, wx.DefaultSize, cboIndependentVariableDataTypeChoices, 1, wx.RA_SPECIFY_COLS )
		self.cboIndependentVariableDataType.SetSelection( 0 )
		gSizer1.Add( self.cboIndependentVariableDataType, 0, wx.ALL, 5 )
		
		cboDependentVariableDataTypeChoices = [ u"Use Location Data", u"Use Sequence Data" ]
		self.cboDependentVariableDataType = wx.RadioBox( self, wx.ID_ANY, u"Dependent Data", wx.DefaultPosition, wx.DefaultSize, cboDependentVariableDataTypeChoices, 1, wx.RA_SPECIFY_COLS )
		self.cboDependentVariableDataType.SetSelection( 0 )
		gSizer1.Add( self.cboDependentVariableDataType, 0, wx.ALL, 5 )
		
		sbSizer1.Add( gSizer1, 0, 0, 5 )
		
		szrLocVar = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Independent variable" ), wx.VERTICAL )
		
		fgSizer4 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer4.SetFlexibleDirection( wx.BOTH )
		fgSizer4.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.lblIndependent = wx.StaticText( self, wx.ID_ANY, u"Independent variable (x):", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblIndependent.Wrap( -1 )
		fgSizer4.Add( self.lblIndependent, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboIndependentChoices = []
		self.cboIndependent = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboIndependentChoices, 0 )
		self.cboIndependent.SetSelection( 0 )
		fgSizer4.Add( self.cboIndependent, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.lblIndependentIDField = wx.StaticText( self, wx.ID_ANY, u"Independent ID Field:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblIndependentIDField.Wrap( -1 )
		fgSizer4.Add( self.lblIndependentIDField, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboIndependentIDFieldChoices = []
		self.cboIndependentIDField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboIndependentIDFieldChoices, 0 )
		self.cboIndependentIDField.SetSelection( 0 )
		self.cboIndependentIDField.Enable( False )
		
		fgSizer4.Add( self.cboIndependentIDField, 0, wx.ALL, 5 )
		
		self.lblIndependentSubtype = wx.StaticText( self, wx.ID_ANY, u"Independent Subtype:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblIndependentSubtype.Wrap( -1 )
		fgSizer4.Add( self.lblIndependentSubtype, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboIndependentSubtypeFieldChoices = []
		self.cboIndependentSubtypeField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboIndependentSubtypeFieldChoices, 0 )
		self.cboIndependentSubtypeField.SetSelection( 0 )
		self.cboIndependentSubtypeField.Enable( False )
		
		fgSizer4.Add( self.cboIndependentSubtypeField, 0, wx.ALL, 5 )
		
		self.m_staticText35 = wx.StaticText( self, wx.ID_ANY, u"Independent count field:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText35.Wrap( -1 )
		fgSizer4.Add( self.m_staticText35, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboIndependentCountFieldChoices = []
		self.cboIndependentCountField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboIndependentCountFieldChoices, 0 )
		self.cboIndependentCountField.SetSelection( 0 )
		self.cboIndependentCountField.Enable( False )
		
		fgSizer4.Add( self.cboIndependentCountField, 0, wx.ALL, 5 )
		
		szrLocVar.Add( fgSizer4, 1, wx.EXPAND, 5 )
		
		sbSizer1.Add( szrLocVar, 0, wx.ALL|wx.EXPAND, 5 )
		
		szrSeqVar = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Dependent variable" ), wx.VERTICAL )
		
		fgSizer41 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer41.SetFlexibleDirection( wx.BOTH )
		fgSizer41.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.lblDependent = wx.StaticText( self, wx.ID_ANY, u"Dependent variable (y):", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblDependent.Wrap( -1 )
		fgSizer41.Add( self.lblDependent, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboDependentChoices = []
		self.cboDependent = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboDependentChoices, 0 )
		self.cboDependent.SetSelection( 0 )
		fgSizer41.Add( self.cboDependent, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.lblDependentIDField = wx.StaticText( self, wx.ID_ANY, u"Dependent ID Field:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblDependentIDField.Wrap( -1 )
		fgSizer41.Add( self.lblDependentIDField, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboDependentIDFieldChoices = []
		self.cboDependentIDField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboDependentIDFieldChoices, 0 )
		self.cboDependentIDField.SetSelection( 0 )
		self.cboDependentIDField.Enable( False )
		
		fgSizer41.Add( self.cboDependentIDField, 0, wx.ALL, 5 )
		
		self.lblDependentSubtype = wx.StaticText( self, wx.ID_ANY, u"Dependent Subtype:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblDependentSubtype.Wrap( -1 )
		fgSizer41.Add( self.lblDependentSubtype, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboDependentSubtypeFieldChoices = []
		self.cboDependentSubtypeField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboDependentSubtypeFieldChoices, 0 )
		self.cboDependentSubtypeField.SetSelection( 0 )
		self.cboDependentSubtypeField.Enable( False )
		
		fgSizer41.Add( self.cboDependentSubtypeField, 0, wx.ALL, 5 )
		
		self.m_staticText36 = wx.StaticText( self, wx.ID_ANY, u"Dependent count field", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText36.Wrap( -1 )
		fgSizer41.Add( self.m_staticText36, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboDependentCountFieldChoices = []
		self.cboDependentCountField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboDependentCountFieldChoices, 0 )
		self.cboDependentCountField.SetSelection( 0 )
		self.cboDependentCountField.Enable( False )
		
		fgSizer41.Add( self.cboDependentCountField, 0, wx.ALL, 5 )
		
		szrSeqVar.Add( fgSizer41, 1, wx.EXPAND, 5 )
		
		sbSizer1.Add( szrSeqVar, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer6.Add( sbSizer1, 0, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer2 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Viewport display" ), wx.VERTICAL )
		
		fgSizer1 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText3 = wx.StaticText( self, wx.ID_ANY, u"Plot type:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText3.Wrap( -1 )
		fgSizer1.Add( self.m_staticText3, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboViewportPlotChoices = [ u"Residuals", u"x data", u"y data" ]
		self.cboViewportPlot = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboViewportPlotChoices, 0 )
		self.cboViewportPlot.SetSelection( 0 )
		fgSizer1.Add( self.cboViewportPlot, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText4 = wx.StaticText( self, wx.ID_ANY, u"Colour map:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText4.Wrap( -1 )
		fgSizer1.Add( self.m_staticText4, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboColourMapChoices = []
		self.cboColourMap = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboColourMapChoices, 0 )
		self.cboColourMap.SetSelection( 0 )
		fgSizer1.Add( self.cboColourMap, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText5 = wx.StaticText( self, wx.ID_ANY, u"Line width:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText5.Wrap( -1 )
		fgSizer1.Add( self.m_staticText5, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.spinLineWidth = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 0, 10, 5 )
		fgSizer1.Add( self.spinLineWidth, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText6 = wx.StaticText( self, wx.ID_ANY, u"Scale factor:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText6.Wrap( -1 )
		fgSizer1.Add( self.m_staticText6, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		bSizer8 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.txtScaleFactor = wx.TextCtrl( self, wx.ID_ANY, u"1.0", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer8.Add( self.txtScaleFactor, 0, wx.ALL|wx.EXPAND, 5 )
		
		fgSizer1.Add( bSizer8, 1, wx.EXPAND, 5 )
		
		sbSizer2.Add( fgSizer1, 1, wx.EXPAND, 5 )
		
		bSizer6.Add( sbSizer2, 0, wx.ALL|wx.EXPAND, 5 )
		
		bSizer4 = wx.BoxSizer( wx.HORIZONTAL )
		
		
		bSizer4.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_btnCalculate = wx.Button( self, wx.ID_ANY, u"Calculate", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer4.Add( self.m_btnCalculate, 0, wx.ALL, 5 )
		
		bSizer6.Add( bSizer4, 0, wx.EXPAND, 5 )
		
		self.txtOutput = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_MULTILINE )
		bSizer6.Add( self.txtOutput, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer61 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnHelp = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		self.m_btnHelp.SetToolTipString( u"Help" )
		
		bSizer61.Add( self.m_btnHelp, 0, wx.ALL, 5 )
		
		self.m_btnClearLog = wx.Button( self, wx.ID_ANY, u"Clear log", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer61.Add( self.m_btnClearLog, 0, wx.ALL, 5 )
		
		self.m_btnSaveLog = wx.Button( self, wx.ID_ANY, u"Save log", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer61.Add( self.m_btnSaveLog, 0, wx.ALL, 5 )
		
		
		bSizer61.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		bSizer6.Add( bSizer61, 0, wx.EXPAND, 5 )
		
		bSizer5.Add( bSizer6, 0, wx.EXPAND, 5 )
		
		bSizer51 = wx.BoxSizer( wx.VERTICAL )
		
		self.pnlPlot = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.SIMPLE_BORDER|wx.TAB_TRAVERSAL )
		bSizer51.Add( self.pnlPlot, 1, wx.EXPAND, 5 )
		
		bSizer7 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnSavePlot = wx.Button( self, wx.ID_ANY, u"Save plot", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer7.Add( self.m_btnSavePlot, 0, wx.ALL, 5 )
		
		
		bSizer7.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnOK = wx.Button( self, wx.ID_ANY, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer7.Add( self.btnOK, 0, wx.ALL, 5 )
		
		bSizer51.Add( bSizer7, 0, wx.EXPAND, 5 )
		
		bSizer5.Add( bSizer51, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer1.Add( bSizer5, 1, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer1 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.cboIndependentVariableDataType.Bind( wx.EVT_RADIOBOX, self.OnIndependentClicked )
		self.cboDependentVariableDataType.Bind( wx.EVT_RADIOBOX, self.OnDependentClicked )
		self.cboIndependentIDField.Bind( wx.EVT_CHOICE, self.OnIndependentIDFieldChoice )
		self.cboDependentIDField.Bind( wx.EVT_CHOICE, self.OnDependentIDFieldChoice )
		self.m_btnCalculate.Bind( wx.EVT_BUTTON, self.OnCalculate )
		self.m_btnHelp.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.m_btnClearLog.Bind( wx.EVT_BUTTON, self.OnClearLog )
		self.m_btnSaveLog.Bind( wx.EVT_BUTTON, self.OnSaveLog )
		self.m_btnSavePlot.Bind( wx.EVT_BUTTON, self.OnSavePlot )
		self.btnOK.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnIndependentClicked( self, event ):
		event.Skip()
	
	def OnDependentClicked( self, event ):
		event.Skip()
	
	def OnIndependentIDFieldChoice( self, event ):
		event.Skip()
	
	def OnDependentIDFieldChoice( self, event ):
		event.Skip()
	
	def OnCalculate( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnClearLog( self, event ):
		event.Skip()
	
	def OnSaveLog( self, event ):
		event.Skip()
	
	def OnSavePlot( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
