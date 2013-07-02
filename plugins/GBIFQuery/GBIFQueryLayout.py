# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class GBIFQueryLayout
###########################################################################

class GBIFQueryLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"GBIF Query", pos = wx.DefaultPosition, size = wx.Size( 790,620 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		
		bSizer1 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer2 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer3 = wx.BoxSizer( wx.VERTICAL )
		
		
		bSizer3.AddSpacer( ( 0, 0), 0, 0, 5 )
		
		sbSizer2 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Query" ), wx.HORIZONTAL )
		
		self.m_TaxonName = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer2.Add( self.m_TaxonName, 1, wx.ALL, 5 )
		
		self.m_Search = wx.Button( self, wx.ID_ANY, u"Search", wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer2.Add( self.m_Search, 0, wx.ALL, 5 )
		
		bSizer3.Add( sbSizer2, 0, wx.EXPAND, 5 )
		
		sbSizer4 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Boundries" ), wx.VERTICAL )
		
		gSizer3 = wx.GridSizer( 2, 2, 0, 0 )
		
		self.m_MinLonLabel = wx.StaticText( self, wx.ID_ANY, u"Min Longitude", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_MinLonLabel.Wrap( -1 )
		gSizer3.Add( self.m_MinLonLabel, 0, wx.ALL, 5 )
		
		self.m_MinLon = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, -180, 180, 180 )
		gSizer3.Add( self.m_MinLon, 0, wx.ALL, 5 )
		
		self.m_MaxLonLabel = wx.StaticText( self, wx.ID_ANY, u"Max Longitude", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_MaxLonLabel.Wrap( -1 )
		gSizer3.Add( self.m_MaxLonLabel, 0, wx.ALL, 5 )
		
		self.m_MaxLon = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, -180, 180, 180 )
		gSizer3.Add( self.m_MaxLon, 0, wx.ALL, 5 )
		
		self.m_staticText7 = wx.StaticText( self, wx.ID_ANY, u"Min Latitude", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText7.Wrap( -1 )
		gSizer3.Add( self.m_staticText7, 0, wx.ALL, 5 )
		
		self.m_MinLat = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, -90, 90, 90 )
		gSizer3.Add( self.m_MinLat, 0, wx.ALL, 5 )
		
		self.m_staticText8 = wx.StaticText( self, wx.ID_ANY, u"Max Latitude", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText8.Wrap( -1 )
		gSizer3.Add( self.m_staticText8, 0, wx.ALL, 5 )
		
		self.m_MaxLat = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, -90, 90, 90 )
		gSizer3.Add( self.m_MaxLat, 0, wx.ALL, 5 )
		
		sbSizer4.Add( gSizer3, 1, wx.EXPAND, 5 )
		
		bSizer3.Add( sbSizer4, 0, wx.EXPAND, 5 )
		
		sbSizer8 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Engage" ), wx.HORIZONTAL )
		
		bSizer13 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_goButton = wx.Button( self, wx.ID_ANY, u"Go", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer13.Add( self.m_goButton, 0, wx.ALL, 5 )
		
		self.m_preGoButton = wx.Button( self, wx.ID_ANY, u"PreGo", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer13.Add( self.m_preGoButton, 0, wx.ALL, 5 )
		
		sbSizer8.Add( bSizer13, 0, wx.EXPAND, 5 )
		
		sbSizer13 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Progress" ), wx.VERTICAL )
		
		self.m_ProgressText = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_MULTILINE )
		sbSizer13.Add( self.m_ProgressText, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer8.Add( sbSizer13, 1, wx.EXPAND, 5 )
		
		bSizer3.Add( sbSizer8, 1, wx.EXPAND, 5 )
		
		
		bSizer3.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		sbSizer11 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Summary" ), wx.VERTICAL )
		
		self.m_Summary = wx.StaticText( self, wx.ID_ANY, u"\n", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_Summary.Wrap( -1 )
		sbSizer11.Add( self.m_Summary, 0, wx.ALL, 5 )
		
		bSizer3.Add( sbSizer11, 0, wx.EXPAND, 5 )
		
		bSizer12 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_Help = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		bSizer12.Add( self.m_Help, 0, wx.ALL, 5 )
		
		self.m_AddData = wx.Button( self, wx.ID_ANY, u"Add Data", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer12.Add( self.m_AddData, 0, wx.ALL, 5 )
		
		self.m_ExportData = wx.Button( self, wx.ID_ANY, u"Export Data", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer12.Add( self.m_ExportData, 0, wx.ALL, 5 )
		
		
		bSizer12.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		bSizer3.Add( bSizer12, 0, wx.EXPAND, 5 )
		
		bSizer2.Add( bSizer3, 1, wx.EXPAND, 5 )
		
		bSizer5 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer8 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer5 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Results Table" ), wx.VERTICAL )
		
		bSizer10 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_ResultsTable = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer10.Add( self.m_ResultsTable, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer5.Add( bSizer10, 1, wx.EXPAND, 5 )
		
		bSizer8.Add( sbSizer5, 1, wx.EXPAND, 5 )
		
		sbSizer6 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, wx.EmptyString ), wx.VERTICAL )
		
		bSizer9 = wx.BoxSizer( wx.HORIZONTAL )
		
		
		bSizer9.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_Add = wx.Button( self, wx.ID_ANY, u"Add", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer9.Add( self.m_Add, 0, wx.ALL, 5 )
		
		
		bSizer9.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_Remove = wx.Button( self, wx.ID_ANY, u"Remove", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer9.Add( self.m_Remove, 0, wx.ALL, 5 )
		
		
		bSizer9.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		sbSizer6.Add( bSizer9, 1, wx.EXPAND, 5 )
		
		bSizer8.Add( sbSizer6, 0, wx.EXPAND, 5 )
		
		sbSizer7 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"ID List" ), wx.VERTICAL )
		
		bSizer11 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_IDList = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer11.Add( self.m_IDList, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer7.Add( bSizer11, 1, wx.EXPAND, 5 )
		
		bSizer8.Add( sbSizer7, 1, wx.EXPAND, 5 )
		
		bSizer5.Add( bSizer8, 1, wx.EXPAND, 5 )
		
		bSizer2.Add( bSizer5, 1, wx.EXPAND, 5 )
		
		bSizer1.Add( bSizer2, 1, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer1 )
		self.Layout()
		self.m_statusBar1 = self.CreateStatusBar( 1, wx.ST_SIZEGRIP, wx.ID_ANY )
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.m_Search.Bind( wx.EVT_BUTTON, self.onSearch )
		self.m_goButton.Bind( wx.EVT_BUTTON, self.onCalculate )
		self.m_preGoButton.Bind( wx.EVT_BUTTON, self.onPreCalculate )
		self.m_Help.Bind( wx.EVT_BUTTON, self.onHelp )
		self.m_AddData.Bind( wx.EVT_BUTTON, self.onAddData )
		self.m_ExportData.Bind( wx.EVT_BUTTON, self.onExportData )
		self.m_Add.Bind( wx.EVT_BUTTON, self.onAdd )
		self.m_Remove.Bind( wx.EVT_BUTTON, self.onRemove )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def onSearch( self, event ):
		event.Skip()
	
	def onCalculate( self, event ):
		event.Skip()
	
	def onPreCalculate( self, event ):
		event.Skip()
	
	def onHelp( self, event ):
		event.Skip()
	
	def onAddData( self, event ):
		event.Skip()
	
	def onExportData( self, event ):
		event.Skip()
	
	def onAdd( self, event ):
		event.Skip()
	
	def onRemove( self, event ):
		event.Skip()
	

