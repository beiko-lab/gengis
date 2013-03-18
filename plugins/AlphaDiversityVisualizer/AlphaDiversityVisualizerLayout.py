# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx
import wx.grid

###########################################################################
## Class AlphaDiversityVisualizerLayout
###########################################################################

class AlphaDiversityVisualizerLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Alpha-diversity Visualizer", pos = wx.DefaultPosition, size = wx.Size( 900,761 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer5 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_panelLeft = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer6 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer1 = wx.StaticBoxSizer( wx.StaticBox( self.m_panelLeft, wx.ID_ANY, u"Alpha Diversity" ), wx.VERTICAL )
		
		fgSizer4 = wx.FlexGridSizer( 3, 2, 0, 0 )
		fgSizer4.AddGrowableCol( 1 )
		fgSizer4.SetFlexibleDirection( wx.BOTH )
		fgSizer4.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.lblMeasure = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Measure:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblMeasure.Wrap( -1 )
		fgSizer4.Add( self.lblMeasure, 0, wx.ALL, 5 )
		
		cboMeasureChoices = []
		self.cboMeasure = wx.Choice( self.m_panelLeft, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboMeasureChoices, 0 )
		self.cboMeasure.SetSelection( 0 )
		fgSizer4.Add( self.cboMeasure, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.lblCatField = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Category Field:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblCatField.Wrap( -1 )
		fgSizer4.Add( self.lblCatField, 0, wx.ALL, 5 )
		
		cboCategoryFieldChoices = []
		self.cboCategoryField = wx.Choice( self.m_panelLeft, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboCategoryFieldChoices, 0 )
		self.cboCategoryField.SetSelection( 0 )
		fgSizer4.Add( self.cboCategoryField, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.lblCountField = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Count Field:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblCountField.Wrap( -1 )
		fgSizer4.Add( self.lblCountField, 0, wx.ALL, 5 )
		
		cboCountFieldChoices = []
		self.cboCountField = wx.Choice( self.m_panelLeft, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboCountFieldChoices, 0 )
		self.cboCountField.SetSelection( 0 )
		fgSizer4.Add( self.cboCountField, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer1.Add( fgSizer4, 1, wx.EXPAND, 5 )
		
		self.chkIgnoreOther = wx.CheckBox( self.m_panelLeft, wx.ID_ANY, u"Ignore 'Other' and 'Unclassified' categories", wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer1.Add( self.chkIgnoreOther, 0, wx.ALL, 5 )
		
		bSizer6.Add( sbSizer1, 0, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer6 = wx.StaticBoxSizer( wx.StaticBox( self.m_panelLeft, wx.ID_ANY, u"Details" ), wx.VERTICAL )
		
		fgSizer5 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer5.AddGrowableCol( 1 )
		fgSizer5.AddGrowableRow( 2 )
		fgSizer5.SetFlexibleDirection( wx.BOTH )
		fgSizer5.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText131 = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Full name:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText131.Wrap( -1 )
		fgSizer5.Add( self.m_staticText131, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.txtFullName = wx.TextCtrl( self.m_panelLeft, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_READONLY )
		fgSizer5.Add( self.txtFullName, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.lblFormula = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Formula:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblFormula.Wrap( -1 )
		fgSizer5.Add( self.lblFormula, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.txtFormula = wx.TextCtrl( self.m_panelLeft, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_READONLY )
		fgSizer5.Add( self.txtFormula, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText15 = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Description:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText15.Wrap( -1 )
		fgSizer5.Add( self.m_staticText15, 0, wx.ALL, 5 )
		
		self.txtDescription = wx.TextCtrl( self.m_panelLeft, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_WORDWRAP )
		fgSizer5.Add( self.txtDescription, 0, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer6.Add( fgSizer5, 1, wx.EXPAND, 5 )
		
		bSizer6.Add( sbSizer6, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer5 = wx.StaticBoxSizer( wx.StaticBox( self.m_panelLeft, wx.ID_ANY, u"Subsampling without Replacement" ), wx.VERTICAL )
		
		self.chkSubsample = wx.CheckBox( self.m_panelLeft, wx.ID_ANY, u"Perform subsampling", wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer5.Add( self.chkSubsample, 0, wx.ALL, 5 )
		
		fgSizer41 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer41.AddGrowableCol( 1 )
		fgSizer41.SetFlexibleDirection( wx.BOTH )
		fgSizer41.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText11 = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Replicates:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText11.Wrap( -1 )
		fgSizer41.Add( self.m_staticText11, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboReplicatesChoices = [ u"10", u"100", u"1000" ]
		self.cboReplicates = wx.Choice( self.m_panelLeft, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboReplicatesChoices, 0 )
		self.cboReplicates.SetSelection( 1 )
		self.cboReplicates.Enable( False )
		
		fgSizer41.Add( self.cboReplicates, 0, wx.ALL, 5 )
		
		self.m_staticText12 = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Seqs to draw:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText12.Wrap( -1 )
		fgSizer41.Add( self.m_staticText12, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		bSizer9 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.txtSeqsToDraw = wx.TextCtrl( self.m_panelLeft, wx.ID_ANY, u"100", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.txtSeqsToDraw.Enable( False )
		
		bSizer9.Add( self.txtSeqsToDraw, 0, wx.ALL, 5 )
		
		self.btnMinimumSeqs = wx.Button( self.m_panelLeft, wx.ID_ANY, u"Min. Seqs in Sample", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.btnMinimumSeqs.Enable( False )
		
		bSizer9.Add( self.btnMinimumSeqs, 0, wx.ALL, 5 )
		
		fgSizer41.Add( bSizer9, 1, wx.EXPAND, 5 )
		
		sbSizer5.Add( fgSizer41, 1, wx.EXPAND, 5 )
		
		bSizer6.Add( sbSizer5, 0, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer2 = wx.StaticBoxSizer( wx.StaticBox( self.m_panelLeft, wx.ID_ANY, u"Viewport Display" ), wx.VERTICAL )
		
		fgSizer1 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText3 = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Plot type:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText3.Wrap( -1 )
		fgSizer1.Add( self.m_staticText3, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboViewportPlotChoices = [ u"Residuals", u"Alpha Diversity", u"Environmental Data", u"<none>" ]
		self.cboViewportPlot = wx.Choice( self.m_panelLeft, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboViewportPlotChoices, 0 )
		self.cboViewportPlot.SetSelection( 0 )
		fgSizer1.Add( self.cboViewportPlot, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText4 = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Colour map:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText4.Wrap( -1 )
		fgSizer1.Add( self.m_staticText4, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboColourMapChoices = []
		self.cboColourMap = wx.Choice( self.m_panelLeft, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboColourMapChoices, 0 )
		self.cboColourMap.SetSelection( 0 )
		fgSizer1.Add( self.cboColourMap, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText5 = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Line width:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText5.Wrap( -1 )
		fgSizer1.Add( self.m_staticText5, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.spinLineWidth = wx.SpinCtrl( self.m_panelLeft, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 0, 10, 5 )
		fgSizer1.Add( self.spinLineWidth, 0, wx.ALL, 5 )
		
		self.m_staticText6 = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Scale factor:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText6.Wrap( -1 )
		fgSizer1.Add( self.m_staticText6, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		bSizer8 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.txtScaleFactor = wx.TextCtrl( self.m_panelLeft, wx.ID_ANY, u"1.0", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer8.Add( self.txtScaleFactor, 0, wx.ALL|wx.EXPAND, 5 )
		
		fgSizer1.Add( bSizer8, 1, wx.EXPAND, 5 )
		
		sbSizer2.Add( fgSizer1, 0, wx.EXPAND, 5 )
		
		self.m_staticline1 = wx.StaticLine( self.m_panelLeft, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.LI_HORIZONTAL )
		sbSizer2.Add( self.m_staticline1, 0, wx.EXPAND |wx.ALL, 5 )
		
		fgSizer3 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer3.SetFlexibleDirection( wx.BOTH )
		fgSizer3.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText8 = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Dot Type:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText8.Wrap( -1 )
		fgSizer3.Add( self.m_staticText8, 0, wx.ALL, 5 )
		
		cboViewportDotChoices = [ u"Residuals", u"Alpha Diversity", u"Environmental Data", u"<none>" ]
		self.cboViewportDot = wx.Choice( self.m_panelLeft, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboViewportDotChoices, 0 )
		self.cboViewportDot.SetSelection( 0 )
		fgSizer3.Add( self.cboViewportDot, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText13 = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Colour Map:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText13.Wrap( -1 )
		fgSizer3.Add( self.m_staticText13, 0, wx.ALL, 5 )
		
		cboColourMapDotChoices = []
		self.cboColourMapDot = wx.Choice( self.m_panelLeft, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboColourMapDotChoices, 0 )
		self.cboColourMapDot.SetSelection( 0 )
		fgSizer3.Add( self.cboColourMapDot, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText14 = wx.StaticText( self.m_panelLeft, wx.ID_ANY, u"Dot Size:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText14.Wrap( -1 )
		fgSizer3.Add( self.m_staticText14, 0, wx.ALL, 5 )
		
		self.spinDotSize = wx.SpinCtrl( self.m_panelLeft, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 0, 10, 5 )
		fgSizer3.Add( self.spinDotSize, 0, wx.ALL, 5 )
		
		sbSizer2.Add( fgSizer3, 0, wx.EXPAND, 5 )
		
		bSizer6.Add( sbSizer2, 0, wx.ALL|wx.EXPAND, 5 )
		
		bSizer81 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnHelp = wx.Button( self.m_panelLeft, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		self.m_btnHelp.SetToolTipString( u"Help" )
		
		bSizer81.Add( self.m_btnHelp, 0, wx.ALL, 5 )
		
		
		bSizer81.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnCalculate = wx.Button( self.m_panelLeft, wx.ID_ANY, u"Calculate", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer81.Add( self.btnCalculate, 0, wx.ALL|wx.EXPAND, 5 )
		
		bSizer6.Add( bSizer81, 0, wx.EXPAND, 5 )
		
		self.m_panelLeft.SetSizer( bSizer6 )
		self.m_panelLeft.Layout()
		bSizer6.Fit( self.m_panelLeft )
		bSizer5.Add( self.m_panelLeft, 0, wx.EXPAND |wx.ALL, 5 )
		
		self.m_panelRight = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		bSizer51 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer3 = wx.StaticBoxSizer( wx.StaticBox( self.m_panelRight, wx.ID_ANY, u"Linear Regression Results" ), wx.VERTICAL )
		
		self.table = wx.grid.Grid( self.m_panelRight, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.HSCROLL|wx.VSCROLL )
		
		# Grid
		self.table.CreateGrid( 2, 3 )
		self.table.EnableEditing( False )
		self.table.EnableGridLines( True )
		self.table.EnableDragGridSize( False )
		self.table.SetMargins( 0, 0 )
		
		# Columns
		self.table.EnableDragColMove( False )
		self.table.EnableDragColSize( True )
		self.table.SetColLabelSize( 30 )
		self.table.SetColLabelAlignment( wx.ALIGN_CENTRE, wx.ALIGN_CENTRE )
		
		# Rows
		self.table.EnableDragRowSize( True )
		self.table.SetRowLabelSize( 80 )
		self.table.SetRowLabelAlignment( wx.ALIGN_CENTRE, wx.ALIGN_CENTRE )
		
		# Label Appearance
		
		# Cell Defaults
		self.table.SetDefaultCellAlignment( wx.ALIGN_LEFT, wx.ALIGN_TOP )
		self.table.SetMinSize( wx.Size( 50,125 ) )
		self.table.SetMaxSize( wx.Size( -1,125 ) )
		
		sbSizer3.Add( self.table, 0, wx.ALL|wx.EXPAND, 5 )
		
		bSizer51.Add( sbSizer3, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.pnlPlot = wx.Panel( self.m_panelRight, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.SIMPLE_BORDER|wx.TAB_TRAVERSAL )
		bSizer51.Add( self.pnlPlot, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer7 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnSavePlot = wx.Button( self.m_panelRight, wx.ID_ANY, u"Save plot", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer7.Add( self.m_btnSavePlot, 0, wx.ALL, 5 )
		
		
		bSizer7.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnOK = wx.Button( self.m_panelRight, wx.ID_ANY, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer7.Add( self.btnOK, 0, wx.ALL, 5 )
		
		bSizer51.Add( bSizer7, 0, wx.EXPAND, 5 )
		
		self.m_panelRight.SetSizer( bSizer51 )
		self.m_panelRight.Layout()
		bSizer51.Fit( self.m_panelRight )
		bSizer5.Add( self.m_panelRight, 1, wx.EXPAND |wx.ALL, 5 )
		
		self.SetSizer( bSizer5 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.cboMeasure.Bind( wx.EVT_CHOICE, self.OnMeasureChanged )
		self.chkSubsample.Bind( wx.EVT_CHECKBOX, self.OnPerformSubsampling )
		self.btnMinimumSeqs.Bind( wx.EVT_BUTTON, self.OnSetMinimumSeqs )
		self.cboViewportPlot.Bind( wx.EVT_CHOICE, self.OnPlotTypeChanged )
		self.cboColourMap.Bind( wx.EVT_CHOICE, self.OnColorMapChanged )
		self.spinLineWidth.Bind( wx.EVT_SPINCTRL, self.OnLineWidthChanged )
		self.cboViewportDot.Bind( wx.EVT_CHOICE, self.OnDotTypeChanged )
		self.cboColourMapDot.Bind( wx.EVT_CHOICE, self.OnDotColorMapChanged )
		self.spinDotSize.Bind( wx.EVT_SPINCTRL, self.OnDotSizeChanged )
		self.m_btnHelp.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.btnCalculate.Bind( wx.EVT_BUTTON, self.OnCalculate )
		self.table.Bind( wx.grid.EVT_GRID_RANGE_SELECT, self.OnSelectedRange )
		self.table.Bind( wx.grid.EVT_GRID_SELECT_CELL, self.OnSelectedCell )
		self.m_btnSavePlot.Bind( wx.EVT_BUTTON, self.OnSavePlot )
		self.btnOK.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnMeasureChanged( self, event ):
		event.Skip()
	
	def OnPerformSubsampling( self, event ):
		event.Skip()
	
	def OnSetMinimumSeqs( self, event ):
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
	
	def OnSelectedRange( self, event ):
		event.Skip()
	
	def OnSelectedCell( self, event ):
		event.Skip()
	
	def OnSavePlot( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

