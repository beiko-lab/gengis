# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Oct  8 2012)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx
import wx.xrc
import wx.grid

###########################################################################
## Class RCALayout
###########################################################################

class RCALayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Reference Condition Analysis", pos = wx.DefaultPosition, size = wx.Size( 850,450 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 850,450 ), wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer1 = wx.BoxSizer( wx.VERTICAL )
		
		fgSizer4 = wx.FlexGridSizer( 0, 2, 0, 0 )
		fgSizer4.AddGrowableCol( 0 )
		fgSizer4.AddGrowableCol( 1 )
		fgSizer4.AddGrowableRow( 0 )
		fgSizer4.SetFlexibleDirection( wx.BOTH )
		fgSizer4.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		fgSizer1 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText6 = wx.StaticText( self, wx.ID_ANY, u"RCA Model:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText6.Wrap( -1 )
		fgSizer1.Add( self.m_staticText6, 0, wx.ALL, 5 )
		
		cboModelChoices = []
		self.cboModel = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboModelChoices, 0 )
		self.cboModel.SetSelection( 0 )
		fgSizer1.Add( self.cboModel, 0, wx.ALL, 5 )
		
		self.m_staticText2 = wx.StaticText( self, wx.ID_ANY, u"Taxon Names:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText2.Wrap( -1 )
		fgSizer1.Add( self.m_staticText2, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboDiversityChoices = []
		self.cboDiversity = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboDiversityChoices, 0 )
		self.cboDiversity.SetSelection( 0 )
		fgSizer1.Add( self.cboDiversity, 0, wx.ALL, 5 )
		
		self.m_staticText3 = wx.StaticText( self, wx.ID_ANY, u"Taxon Counts:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText3.Wrap( -1 )
		fgSizer1.Add( self.m_staticText3, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboCountChoices = []
		self.cboCount = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboCountChoices, 0 )
		self.cboCount.SetSelection( 0 )
		fgSizer1.Add( self.cboCount, 0, wx.ALL, 5 )
		
		
		fgSizer1.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnRun = wx.Button( self, wx.ID_ANY, u"Run", wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer1.Add( self.btnRun, 1, wx.ALL|wx.EXPAND, 5 )
		
		
		fgSizer4.Add( fgSizer1, 1, wx.EXPAND, 5 )
		
		bSizer31 = wx.BoxSizer( wx.VERTICAL )
		
		self.table = wx.grid.Grid( self, wx.ID_ANY, wx.DefaultPosition, wx.Size( -1,-1 ), wx.HSCROLL|wx.VSCROLL )
		
		# Grid
		self.table.CreateGrid( 5, 5 )
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
		self.table.SetRowLabelSize( 50 )
		self.table.SetRowLabelAlignment( wx.ALIGN_CENTRE, wx.ALIGN_CENTRE )
		
		# Label Appearance
		
		# Cell Defaults
		self.table.SetDefaultCellAlignment( wx.ALIGN_LEFT, wx.ALIGN_TOP )
		self.table.SetMinSize( wx.Size( 550,300 ) )
		
		bSizer31.Add( self.table, 1, wx.ALL|wx.EXPAND, 5 )
		
		gbSizer1 = wx.GridBagSizer( 0, 0 )
		gbSizer1.SetFlexibleDirection( wx.BOTH )
		gbSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText51 = wx.StaticText( self, wx.ID_ANY, u"Bar plot scale factor:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText51.Wrap( -1 )
		gbSizer1.Add( self.m_staticText51, wx.GBPosition( 0, 0 ), wx.GBSpan( 1, 1 ), wx.ALL, 5 )
		
		self.userScaleFactor = wx.TextCtrl( self, wx.ID_ANY, u"1.0", wx.DefaultPosition, wx.Size( 45,-1 ), 0 )
		gbSizer1.Add( self.userScaleFactor, wx.GBPosition( 0, 1 ), wx.GBSpan( 1, 1 ), wx.ALL, 5 )
		
		self.btnPlot = wx.Button( self, wx.ID_ANY, u"Plot Selected Data", wx.DefaultPosition, wx.DefaultSize, 0 )
		gbSizer1.Add( self.btnPlot, wx.GBPosition( 0, 2 ), wx.GBSpan( 1, 1 ), wx.ALL, 5 )
		
		self.m_btnAdd = wx.Button( self, wx.ID_ANY, u"Add Selected To GenGIS", wx.DefaultPosition, wx.DefaultSize, 0 )
		gbSizer1.Add( self.m_btnAdd, wx.GBPosition( 0, 3 ), wx.GBSpan( 1, 1 ), wx.ALL, 5 )
		
		
		bSizer31.Add( gbSizer1, 0, wx.EXPAND, 5 )
		
		fgSizer3 = wx.FlexGridSizer( 0, 2, 0, 0 )
		fgSizer3.SetFlexibleDirection( wx.BOTH )
		fgSizer3.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText5 = wx.StaticText( self, wx.ID_ANY, u"Save table of results to file:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText5.Wrap( -1 )
		fgSizer3.Add( self.m_staticText5, 0, wx.ALL, 5 )
		
		self.filePicker = wx.FilePickerCtrl( self, wx.ID_ANY, u"rca_results.csv", u"Save file as", u"*.csv", wx.DefaultPosition, wx.DefaultSize, wx.FLP_OVERWRITE_PROMPT|wx.FLP_SAVE )
		fgSizer3.Add( self.filePicker, 0, wx.ALL, 5 )
		
		
		bSizer31.Add( fgSizer3, 0, wx.EXPAND, 5 )
		
		
		fgSizer4.Add( bSizer31, 1, wx.EXPAND, 5 )
		
		
		bSizer1.Add( fgSizer4, 1, wx.EXPAND, 5 )
		
		bSizer3 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnHelp = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		bSizer3.Add( self.m_btnHelp, 0, wx.ALL, 5 )
		
		
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
		self.table.Bind( wx.grid.EVT_GRID_LABEL_LEFT_CLICK, self.OnLabelClick )
		self.table.Bind( wx.grid.EVT_GRID_RANGE_SELECT, self.OnSelectedRange )
		self.table.Bind( wx.grid.EVT_GRID_SELECT_CELL, self.OnSelectedCell )
		self.btnPlot.Bind( wx.EVT_BUTTON, self.OnPlot )
		self.m_btnAdd.Bind( wx.EVT_BUTTON, self.OnAdd )
		self.filePicker.Bind( wx.EVT_FILEPICKER_CHANGED, self.onSaveFile )
		self.m_btnHelp.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.btOK.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnRun( self, event ):
		event.Skip()
	
	def OnLabelClick( self, event ):
		event.Skip()
	
	def OnSelectedRange( self, event ):
		event.Skip()
	
	def OnSelectedCell( self, event ):
		event.Skip()
	
	def OnPlot( self, event ):
		event.Skip()
	
	def OnAdd( self, event ):
		event.Skip()
	
	def onSaveFile( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

