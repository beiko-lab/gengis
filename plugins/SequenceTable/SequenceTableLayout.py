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
## Class SequenceTableLayout
###########################################################################

class SequenceTableLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Sequence table", pos = wx.DefaultPosition, size = wx.Size( 1000,600 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer9 = wx.BoxSizer( wx.VERTICAL )
		
		self.table = wx.grid.Grid( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, 0 )
		
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
		self.table.SetRowLabelSize( 80 )
		self.table.SetRowLabelAlignment( wx.ALIGN_CENTRE, wx.ALIGN_CENTRE )
		
		# Label Appearance
		
		# Cell Defaults
		self.table.SetDefaultCellAlignment( wx.ALIGN_LEFT, wx.ALIGN_TOP )
		bSizer9.Add( self.table, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer10 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnHelp = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		self.m_btnHelp.SetToolTipString( u"Help" )
		
		bSizer10.Add( self.m_btnHelp, 0, wx.ALL, 5 )
		
		self.chkShowAllLoc = wx.CheckBox( self, wx.ID_ANY, u"Show data for all locations", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer10.Add( self.chkShowAllLoc, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.chkShowAllSeq = wx.CheckBox( self, wx.ID_ANY, u"Show data for all sequences", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer10.Add( self.chkShowAllSeq, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		
		bSizer10.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnOK = wx.Button( self, wx.ID_ANY, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer10.Add( self.btnOK, 0, wx.ALL, 5 )
		
		bSizer9.Add( bSizer10, 0, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer9 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.m_btnHelp.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.chkShowAllLoc.Bind( wx.EVT_CHECKBOX, self.OnShowAllLoc )
		self.chkShowAllSeq.Bind( wx.EVT_CHECKBOX, self.OnShowAllSeq )
		self.btnOK.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnShowAllLoc( self, event ):
		event.Skip()
	
	def OnShowAllSeq( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

