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
## Class DissimilarityMatrixViewerLayout
###########################################################################

class DissimilarityMatrixViewerLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Dissimilarity matrix viewer", pos = wx.DefaultPosition, size = wx.Size( 850,687 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer4 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer6 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer41 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer3 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.btnLoadMatrix = wx.Button( self, wx.ID_ANY, u"Load dissimilarity matrix", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer3.Add( self.btnLoadMatrix, 0, wx.ALL, 5 )
		
		self.lblMatrixFilename = wx.StaticText( self, wx.ID_ANY, u"<No file loaded>", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblMatrixFilename.Wrap( -1 )
		bSizer3.Add( self.lblMatrixFilename, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		bSizer41.Add( bSizer3, 0, wx.EXPAND, 5 )
		
		sbSizer2 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Select range" ), wx.VERTICAL )
		
		bSizer12 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_staticText7 = wx.StaticText( self, wx.ID_ANY, u"Entries >=", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText7.Wrap( -1 )
		bSizer12.Add( self.m_staticText7, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.txtGreaterThanValue = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer12.Add( self.txtGreaterThanValue, 0, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer2.Add( bSizer12, 0, wx.EXPAND, 5 )
		
		self.m_staticText8 = wx.StaticText( self, wx.ID_ANY, u"and", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText8.Wrap( -1 )
		sbSizer2.Add( self.m_staticText8, 0, wx.ALIGN_CENTER, 5 )
		
		bSizer11 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_staticText6 = wx.StaticText( self, wx.ID_ANY, u"Entries <=", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText6.Wrap( -1 )
		bSizer11.Add( self.m_staticText6, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.txtLessThanValue = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer11.Add( self.txtLessThanValue, 0, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer2.Add( bSizer11, 0, wx.EXPAND, 5 )
		
		self.chkSelectFromUpperTriangleByRange = wx.CheckBox( self, wx.ID_ANY, u"Select entries in upper triangle", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.chkSelectFromUpperTriangleByRange.SetValue(True) 
		sbSizer2.Add( self.chkSelectFromUpperTriangleByRange, 0, wx.ALL, 5 )
		
		self.chkSelectFromLowerTriangleByRange = wx.CheckBox( self, wx.ID_ANY, u"Select entries in lower triangle", wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer2.Add( self.chkSelectFromLowerTriangleByRange, 0, wx.ALL, 5 )
		
		self.btnSelectRange = wx.Button( self, wx.ID_ANY, u"Add selection", wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer2.Add( self.btnSelectRange, 0, wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		bSizer41.Add( sbSizer2, 0, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer3 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Select by location" ), wx.VERTICAL )
		
		fgSizer2 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer2.SetFlexibleDirection( wx.BOTH )
		fgSizer2.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText9 = wx.StaticText( self, wx.ID_ANY, u"Location:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText9.Wrap( -1 )
		fgSizer2.Add( self.m_staticText9, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboLocationChoices = []
		self.cboLocation = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboLocationChoices, 0 )
		self.cboLocation.SetSelection( 0 )
		fgSizer2.Add( self.cboLocation, 0, wx.ALL, 5 )
		
		
		fgSizer2.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		bSizer9 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.radioMost = wx.RadioButton( self, wx.ID_ANY, u"Most similar", wx.DefaultPosition, wx.DefaultSize, wx.RB_GROUP )
		self.radioMost.SetValue( True ) 
		bSizer9.Add( self.radioMost, 0, wx.ALL, 5 )
		
		self.radioLeast = wx.RadioButton( self, wx.ID_ANY, u"Least similar", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer9.Add( self.radioLeast, 0, wx.ALL, 5 )
		
		fgSizer2.Add( bSizer9, 1, wx.EXPAND, 5 )
		
		self.m_staticText10 = wx.StaticText( self, wx.ID_ANY, u"Number:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText10.Wrap( -1 )
		fgSizer2.Add( self.m_staticText10, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.spinNumberToSelect = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 2, 100, 3 )
		fgSizer2.Add( self.spinNumberToSelect, 0, wx.ALL, 5 )
		
		sbSizer3.Add( fgSizer2, 1, wx.EXPAND, 5 )
		
		bSizer10 = wx.BoxSizer( wx.VERTICAL )
		
		self.radioSelectFromUpperTriangleByLoc = wx.RadioButton( self, wx.ID_ANY, u"Select entries in upper triangle", wx.DefaultPosition, wx.DefaultSize, wx.RB_GROUP )
		self.radioSelectFromUpperTriangleByLoc.SetValue( True ) 
		bSizer10.Add( self.radioSelectFromUpperTriangleByLoc, 0, wx.ALL, 5 )
		
		self.radioSelectFromLowerTriangleByLoc = wx.RadioButton( self, wx.ID_ANY, u"Select entries in lower triangle", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer10.Add( self.radioSelectFromLowerTriangleByLoc, 0, wx.ALL, 5 )
		
		sbSizer3.Add( bSizer10, 0, wx.EXPAND, 5 )
		
		self.btnSelectByLocation = wx.Button( self, wx.ID_ANY, u"Add selection", wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer3.Add( self.btnSelectByLocation, 0, wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		bSizer41.Add( sbSizer3, 0, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer1 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Visual properties" ), wx.VERTICAL )
		
		fgSizer1 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText2 = wx.StaticText( self, wx.ID_ANY, u"Minimum line width:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText2.Wrap( -1 )
		fgSizer1.Add( self.m_staticText2, 0, wx.ALL, 5 )
		
		self.spinMinWidth = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 1, 10, 1 )
		fgSizer1.Add( self.spinMinWidth, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText3 = wx.StaticText( self, wx.ID_ANY, u"Maximum line width:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText3.Wrap( -1 )
		fgSizer1.Add( self.m_staticText3, 0, wx.ALL, 5 )
		
		self.spinMaxWidth = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 1, 10, 6 )
		fgSizer1.Add( self.spinMaxWidth, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText4 = wx.StaticText( self, wx.ID_ANY, u"Start colour:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText4.Wrap( -1 )
		fgSizer1.Add( self.m_staticText4, 0, wx.ALL, 5 )
		
		self.colourPickerStart = wx.ColourPickerCtrl( self, wx.ID_ANY, wx.Colour( 255, 255, 255 ), wx.DefaultPosition, wx.DefaultSize, wx.CLRP_DEFAULT_STYLE )
		fgSizer1.Add( self.colourPickerStart, 0, wx.ALL, 5 )
		
		self.m_staticText5 = wx.StaticText( self, wx.ID_ANY, u"End colour:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText5.Wrap( -1 )
		fgSizer1.Add( self.m_staticText5, 0, wx.ALL, 5 )
		
		self.colourPickerEnd = wx.ColourPickerCtrl( self, wx.ID_ANY, wx.Colour( 0, 0, 0 ), wx.DefaultPosition, wx.DefaultSize, wx.CLRP_DEFAULT_STYLE )
		fgSizer1.Add( self.colourPickerEnd, 0, wx.ALL, 5 )
		
		sbSizer1.Add( fgSizer1, 1, wx.EXPAND, 5 )
		
		self.chkRestrictColourRange = wx.CheckBox( self, wx.ID_ANY, u"Restrict colour extents to selected cells", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.chkRestrictColourRange.SetValue(True) 
		sbSizer1.Add( self.chkRestrictColourRange, 0, wx.ALL, 5 )
		
		bSizer41.Add( sbSizer1, 0, wx.ALL|wx.EXPAND, 5 )
		
		bSizer8 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.btnClearAll = wx.Button( self, wx.ID_ANY, u"Clear selections", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer8.Add( self.btnClearAll, 0, wx.ALL, 5 )
		
		
		bSizer8.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnPlot = wx.Button( self, wx.ID_ANY, u"Plot selections", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer8.Add( self.btnPlot, 0, wx.ALIGN_RIGHT|wx.ALL, 5 )
		
		bSizer41.Add( bSizer8, 0, wx.EXPAND, 5 )
		
		bSizer6.Add( bSizer41, 0, wx.EXPAND, 5 )
		
		self.dissMatrix = wx.grid.Grid( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, 0 )
		
		# Grid
		self.dissMatrix.CreateGrid( 3, 3 )
		self.dissMatrix.EnableEditing( False )
		self.dissMatrix.EnableGridLines( True )
		self.dissMatrix.EnableDragGridSize( False )
		self.dissMatrix.SetMargins( 0, 0 )
		
		# Columns
		self.dissMatrix.AutoSizeColumns()
		self.dissMatrix.EnableDragColMove( False )
		self.dissMatrix.EnableDragColSize( True )
		self.dissMatrix.SetColLabelSize( 30 )
		self.dissMatrix.SetColLabelAlignment( wx.ALIGN_CENTRE, wx.ALIGN_CENTRE )
		
		# Rows
		self.dissMatrix.SetRowSize( 0, 17 )
		self.dissMatrix.SetRowSize( 1, 17 )
		self.dissMatrix.SetRowSize( 2, 17 )
		self.dissMatrix.EnableDragRowSize( True )
		self.dissMatrix.SetRowLabelSize( 0 )
		self.dissMatrix.SetRowLabelAlignment( wx.ALIGN_CENTRE, wx.ALIGN_CENTRE )
		
		# Label Appearance
		
		# Cell Defaults
		self.dissMatrix.SetDefaultCellAlignment( wx.ALIGN_LEFT, wx.ALIGN_TOP )
		self.dissMatrix.SetBackgroundColour( wx.Colour( 255, 255, 255 ) )
		self.dissMatrix.SetMinSize( wx.Size( 400,400 ) )
		
		bSizer6.Add( self.dissMatrix, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer4.Add( bSizer6, 1, wx.EXPAND, 5 )
		
		bSizer5 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnHelp = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		self.m_btnHelp.SetToolTipString( u"Help" )
		
		bSizer5.Add( self.m_btnHelp, 0, wx.ALL, 5 )
		
		
		bSizer5.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnOK = wx.Button( self, wx.ID_OK, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer5.Add( self.btnOK, 0, wx.ALL, 5 )
		
		bSizer4.Add( bSizer5, 0, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer4 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.btnLoadMatrix.Bind( wx.EVT_BUTTON, self.OnLoadMatrix )
		self.btnSelectRange.Bind( wx.EVT_BUTTON, self.OnAddRangeSelection )
		self.btnSelectByLocation.Bind( wx.EVT_BUTTON, self.OnAddLocationSelection )
		self.btnClearAll.Bind( wx.EVT_BUTTON, self.OnClearSelections )
		self.btnPlot.Bind( wx.EVT_BUTTON, self.OnPlot )
		self.m_btnHelp.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.btnOK.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnLoadMatrix( self, event ):
		event.Skip()
	
	def OnAddRangeSelection( self, event ):
		event.Skip()
	
	def OnAddLocationSelection( self, event ):
		event.Skip()
	
	def OnClearSelections( self, event ):
		event.Skip()
	
	def OnPlot( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

