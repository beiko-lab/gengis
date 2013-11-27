# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class MGRASTQueryLayout
###########################################################################

class MGRASTQueryLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"MG-RAST Query", pos = wx.DefaultPosition, size = wx.Size( -1,-1 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 790,628 ), wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer1 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer2 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer3 = wx.BoxSizer( wx.VERTICAL )
		
		
		bSizer3.AddSpacer( ( 0, 0), 0, 0, 5 )
		
		sbSizer2 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Query" ), wx.HORIZONTAL )
		
		self.m_TaxonName = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer2.Add( self.m_TaxonName, 1, wx.ALL, 5 )
		
		self.m_Search = wx.Button( self, wx.ID_ANY, u"Search", wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer2.Add( self.m_Search, 0, wx.ALL, 5 )
		
		bSizer3.Add( sbSizer2, 0, wx.EXPAND|wx.ALL, 5 )
		
		sbSizer9 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Geographic Range" ), wx.VERTICAL )
		
		fgSizer2 = wx.FlexGridSizer( 3, 3, 0, 0 )
		fgSizer2.AddGrowableCol( 0 )
		fgSizer2.AddGrowableCol( 1 )
		fgSizer2.AddGrowableCol( 2 )
		fgSizer2.AddGrowableRow( 0 )
		fgSizer2.AddGrowableRow( 1 )
		fgSizer2.AddGrowableRow( 2 )
		fgSizer2.SetFlexibleDirection( wx.BOTH )
		fgSizer2.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		
		fgSizer2.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_MaxLat = wx.TextCtrl( self, wx.ID_ANY, u"90", wx.DefaultPosition, wx.Size( 70,-1 ), wx.TE_CENTRE )
		self.m_MaxLat.SetMinSize( wx.Size( 70,-1 ) )
		self.m_MaxLat.SetMaxSize( wx.Size( 70,-1 ) )
		
		fgSizer2.Add( self.m_MaxLat, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		
		fgSizer2.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_MinLon = wx.TextCtrl( self, wx.ID_ANY, u"-180", wx.DefaultPosition, wx.DefaultSize, wx.TE_CENTRE )
		self.m_MinLon.SetMinSize( wx.Size( 70,-1 ) )
		self.m_MinLon.SetMaxSize( wx.Size( 70,-1 ) )
		
		fgSizer2.Add( self.m_MinLon, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_RIGHT, 5 )
		
		self.m_Compass = wx.StaticBitmap( self, wx.ID_ANY, wx.NullBitmap, wx.DefaultPosition, wx.Size( 128,128 ), 0 )
		fgSizer2.Add( self.m_Compass, 1, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_MaxLon = wx.TextCtrl( self, wx.ID_ANY, u"180", wx.DefaultPosition, wx.DefaultSize, wx.TE_CENTRE )
		self.m_MaxLon.SetMinSize( wx.Size( 70,-1 ) )
		self.m_MaxLon.SetMaxSize( wx.Size( 70,-1 ) )
		
		fgSizer2.Add( self.m_MaxLon, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		
		fgSizer2.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_MinLat = wx.TextCtrl( self, wx.ID_ANY, u"-90", wx.DefaultPosition, wx.DefaultSize, wx.TE_CENTRE )
		self.m_MinLat.SetMinSize( wx.Size( 70,-1 ) )
		self.m_MinLat.SetMaxSize( wx.Size( 70,-1 ) )
		
		fgSizer2.Add( self.m_MinLat, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		
		fgSizer2.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		sbSizer9.Add( fgSizer2, 1, wx.EXPAND, 5 )
		
		bSizer3.Add( sbSizer9, 0, wx.EXPAND, 5 )
		
		sbSizer8 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Progress" ), wx.HORIZONTAL )
		
		sbSizer8.SetMinSize( wx.Size( 300,200 ) ) 
		self.m_Progress = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.Size( -1,90 ), wx.TE_DONTWRAP|wx.TE_MULTILINE|wx.TE_READONLY|wx.CLIP_CHILDREN )
		self.m_Progress.SetMinSize( wx.Size( 300,200 ) )
		
		sbSizer8.Add( self.m_Progress, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer3.Add( sbSizer8, 1, wx.EXPAND|wx.ALL, 5 )
		
		sbSizer11 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Summary" ), wx.HORIZONTAL )
		
		self.m_Summary = wx.StaticText( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_Summary.Wrap( -1 )
		sbSizer11.Add( self.m_Summary, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer3.Add( sbSizer11, 0, wx.EXPAND|wx.ALL, 5 )
		
		bSizer12 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_Help = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		bSizer12.Add( self.m_Help, 0, wx.ALL, 5 )
		
		self.m_AddData = wx.Button( self, wx.ID_ANY, u"Add Data to Map", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer12.Add( self.m_AddData, 0, wx.ALL, 5 )
		
		self.m_ExportData = wx.Button( self, wx.ID_ANY, u"Export Data to File...", wx.DefaultPosition, wx.DefaultSize, 0|wx.TAB_TRAVERSAL )
		bSizer12.Add( self.m_ExportData, 0, wx.ALL, 5 )
		
		self.m_Options = wx.Button( self, wx.ID_ANY, u"Options", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer12.Add( self.m_Options, 0, wx.ALL, 5 )
		
		
		bSizer12.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		bSizer3.Add( bSizer12, 0, wx.EXPAND|wx.ALL, 5 )
		
		bSizer2.Add( bSizer3, 1, wx.EXPAND, 5 )
		
		bSizer5 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer8 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer5 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Results Table" ), wx.VERTICAL )
		
		bSizer10 = wx.BoxSizer( wx.HORIZONTAL )
		
		m_ResultChoices = []
		self.m_Result = wx.ListBox( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_ResultChoices, wx.LB_EXTENDED|wx.LB_HSCROLL )
		bSizer10.Add( self.m_Result, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer5.Add( bSizer10, 1, wx.EXPAND, 5 )
		
		bSizer8.Add( sbSizer5, 1, wx.EXPAND|wx.ALL, 5 )
		
		bSizer9 = wx.BoxSizer( wx.HORIZONTAL )
		
		
		bSizer9.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_Add = wx.BitmapButton( self, wx.ID_ANY, wx.NullBitmap, wx.DefaultPosition, wx.Size( -1,-1 ), wx.BU_AUTODRAW )
		bSizer9.Add( self.m_Add, 0, wx.RIGHT|wx.LEFT, 5 )
		
		
		bSizer9.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_Remove = wx.BitmapButton( self, wx.ID_ANY, wx.NullBitmap, wx.DefaultPosition, wx.DefaultSize, wx.BU_AUTODRAW )
		bSizer9.Add( self.m_Remove, 0, wx.RIGHT|wx.LEFT, 5 )
		
		
		bSizer9.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		bSizer8.Add( bSizer9, 0, wx.EXPAND|wx.ALL, 5 )
		
		sbSizer7 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"ID List" ), wx.VERTICAL )
		
		bSizer11 = wx.BoxSizer( wx.VERTICAL )
		
		m_IDListChoices = []
		self.m_IDList = wx.ListBox( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_IDListChoices, wx.LB_EXTENDED|wx.LB_HSCROLL )
		bSizer11.Add( self.m_IDList, 1, wx.ALL|wx.EXPAND, 5 )
		
		gSizer2 = wx.GridSizer( 1, 2, 0, 0 )
		
		self.m_Calc = wx.Button( self, wx.ID_ANY, u"Retrieve Data", wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer2.Add( self.m_Calc, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_PreCalc = wx.Button( self, wx.ID_ANY, u"Query Records", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_PreCalc.Hide()
		
		gSizer2.Add( self.m_PreCalc, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		bSizer11.Add( gSizer2, 0, wx.EXPAND, 5 )
		
		sbSizer7.Add( bSizer11, 1, wx.EXPAND, 5 )
		
		bSizer8.Add( sbSizer7, 1, wx.EXPAND|wx.ALL, 5 )
		
		bSizer5.Add( bSizer8, 1, wx.EXPAND|wx.ALL, 5 )
		
		bSizer14 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_Close = wx.Button( self, wx.ID_ANY, u"Close", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer14.Add( self.m_Close, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		bSizer5.Add( bSizer14, 0, wx.EXPAND|wx.ALL, 5 )
		
		bSizer2.Add( bSizer5, 1, wx.EXPAND, 5 )
		
		bSizer1.Add( bSizer2, 1, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer1 )
		self.Layout()
		bSizer1.Fit( self )
		self.m_statusBar = self.CreateStatusBar( 1, wx.ST_SIZEGRIP, wx.ID_ANY )
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.m_Search.Bind( wx.EVT_BUTTON, self.OnSearch )
		self.m_MaxLat.Bind( wx.EVT_TEXT, self.OnLatEnter )
		self.m_MinLon.Bind( wx.EVT_TEXT, self.OnLonEnter )
		self.m_MaxLon.Bind( wx.EVT_TEXT, self.OnLonEnter )
		self.m_MinLat.Bind( wx.EVT_TEXT, self.OnLatEnter )
		self.m_Help.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.m_AddData.Bind( wx.EVT_BUTTON, self.OnAddData )
		self.m_ExportData.Bind( wx.EVT_BUTTON, self.OnExportData )
		self.m_Options.Bind( wx.EVT_BUTTON, self.OnOptions )
		self.m_Result.Bind( wx.EVT_LEFT_DCLICK, self.OnAdd )
		self.m_Add.Bind( wx.EVT_BUTTON, self.OnAdd )
		self.m_Remove.Bind( wx.EVT_BUTTON, self.OnRemove )
		self.m_IDList.Bind( wx.EVT_LEFT_DCLICK, self.OnRemove )
		self.m_Calc.Bind( wx.EVT_BUTTON, self.OnCalculate )
		self.m_PreCalc.Bind( wx.EVT_BUTTON, self.OnPreCalculate )
		self.m_Close.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnSearch( self, event ):
		event.Skip()
	
	def OnLatEnter( self, event ):
		event.Skip()
	
	def OnLonEnter( self, event ):
		event.Skip()
	
	
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnAddData( self, event ):
		event.Skip()
	
	def OnExportData( self, event ):
		event.Skip()
	
	def OnOptions( self, event ):
		event.Skip()
	
	def OnAdd( self, event ):
		event.Skip()
	
	
	def OnRemove( self, event ):
		event.Skip()
	
	
	def OnCalculate( self, event ):
		event.Skip()
	
	def OnPreCalculate( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

###########################################################################
## Class OptionsFrame
###########################################################################

class OptionsFrame ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = wx.EmptyString, pos = wx.DefaultPosition, size = wx.Size( 502,338 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 502,338 ), wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		sbSizer6 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Options" ), wx.VERTICAL )
		
		fgSizer2 = wx.FlexGridSizer( 9, 3, 0, 0 )
		fgSizer2.AddGrowableCol( 0 )
		fgSizer2.AddGrowableCol( 1 )
		fgSizer2.AddGrowableCol( 2 )
		fgSizer2.AddGrowableCol( 3 )
		fgSizer2.AddGrowableCol( 4 )
		fgSizer2.AddGrowableCol( 5 )
		fgSizer2.AddGrowableCol( 6 )
		fgSizer2.AddGrowableRow( 0 )
		fgSizer2.AddGrowableRow( 1 )
		fgSizer2.AddGrowableRow( 2 )
		fgSizer2.AddGrowableRow( 3 )
		fgSizer2.AddGrowableRow( 4 )
		fgSizer2.AddGrowableRow( 5 )
		fgSizer2.AddGrowableRow( 6 )
		fgSizer2.SetFlexibleDirection( wx.BOTH )
		fgSizer2.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_ALL )
		
		self.m_searchTypeCheck = wx.CheckBox( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer2.Add( self.m_searchTypeCheck, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_searchTypeText = wx.StaticText( self, wx.ID_ANY, u"Search Type", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_searchTypeText.Wrap( -1 )
		fgSizer2.Add( self.m_searchTypeText, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		m_searchTypeChoices = [ u"organism", u"function", u"study" ]
		self.m_searchType = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_searchTypeChoices, 0 )
		self.m_searchType.SetSelection( 0 )
		fgSizer2.Add( self.m_searchType, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_filterLevelCheck = wx.CheckBox( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer2.Add( self.m_filterLevelCheck, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_filterLevelText = wx.StaticText( self, wx.ID_ANY, u"Filter Level", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_filterLevelText.Wrap( -1 )
		fgSizer2.Add( self.m_filterLevelText, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		m_filterLevelChoices = [ u"function", u"level3", u"level2", u"level1" ]
		self.m_filterLevel = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_filterLevelChoices, 0 )
		self.m_filterLevel.SetSelection( 0 )
		fgSizer2.Add( self.m_filterLevel, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.EXPAND, 5 )
		
		self.m_filterSourceCheck = wx.CheckBox( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer2.Add( self.m_filterSourceCheck, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_filterSourceText = wx.StaticText( self, wx.ID_ANY, u"Filter Source", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_filterSourceText.Wrap( -1 )
		fgSizer2.Add( self.m_filterSourceText, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		m_filterSourceChoices = [ u"subsystems", u"NOG", u"COG", u"KO" ]
		self.m_filterSource = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_filterSourceChoices, 0 )
		self.m_filterSource.SetSelection( 0 )
		fgSizer2.Add( self.m_filterSource, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.EXPAND, 5 )
		
		self.m_groupLevelCheck = wx.CheckBox( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer2.Add( self.m_groupLevelCheck, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_groupLevelText = wx.StaticText( self, wx.ID_ANY, u"Group Level", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_groupLevelText.Wrap( -1 )
		fgSizer2.Add( self.m_groupLevelText, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		m_groupLevelChoices = [ u"strain", u"species", u"genus", u"family", u"order", u"class", u"phylum", u"domain" ]
		self.m_groupLevel = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_groupLevelChoices, 0 )
		self.m_groupLevel.SetSelection( 0 )
		fgSizer2.Add( self.m_groupLevel, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.EXPAND, 5 )
		
		self.m_hitTypeCheck = wx.CheckBox( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer2.Add( self.m_hitTypeCheck, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_hitTypeText = wx.StaticText( self, wx.ID_ANY, u"Hit Type", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_hitTypeText.Wrap( -1 )
		fgSizer2.Add( self.m_hitTypeText, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		m_hitTypeChoices = [ u"all", u"single", u"lca" ]
		self.m_hitType = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_hitTypeChoices, 0 )
		self.m_hitType.SetSelection( 0 )
		fgSizer2.Add( self.m_hitType, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.EXPAND, 5 )
		
		self.m_identityCheck = wx.CheckBox( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer2.Add( self.m_identityCheck, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_identityText = wx.StaticText( self, wx.ID_ANY, u"Identity", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_identityText.Wrap( -1 )
		fgSizer2.Add( self.m_identityText, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_identity = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 0, 100, 60 )
		fgSizer2.Add( self.m_identity, 0, wx.ALL, 5 )
		
		self.m_lengthCheck = wx.CheckBox( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer2.Add( self.m_lengthCheck, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_lengthText = wx.StaticText( self, wx.ID_ANY, u"Length", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_lengthText.Wrap( -1 )
		fgSizer2.Add( self.m_lengthText, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_length = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 0, 50, 15 )
		fgSizer2.Add( self.m_length, 0, wx.ALL, 5 )
		
		self.m_sourceCheck = wx.CheckBox( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer2.Add( self.m_sourceCheck, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_sourceText = wx.StaticText( self, wx.ID_ANY, u"Source", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_sourceText.Wrap( -1 )
		fgSizer2.Add( self.m_sourceText, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		m_sourceChoices = [ u"M5NR", u"RefSeq", u"SwissProt", u"GenBank", u"IMG", u"SEED", u"TrEMBL", u"PATRIC", u"KEGG", u"M5RNA", u"Greengenes", u"LSU", u"SSU" ]
		self.m_source = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_sourceChoices, 0 )
		self.m_source.SetSelection( 0 )
		fgSizer2.Add( self.m_source, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.EXPAND, 5 )
		
		sbSizer6.Add( fgSizer2, 0, wx.EXPAND, 5 )
		
		bSizer11 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_OptionsClose = wx.Button( self, wx.ID_ANY, u"Close", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer11.Add( self.m_OptionsClose, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		sbSizer6.Add( bSizer11, 1, wx.EXPAND, 5 )
		
		self.SetSizer( sbSizer6 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.m_OptionsClose.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnOK( self, event ):
		event.Skip()
	

