# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class ShowSpreadLayout
###########################################################################

class ShowSpreadLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Show Spread", pos = wx.DefaultPosition, size = wx.Size( 276,310 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 276,310 ), wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer1 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_DisplayNotebook = wx.Notebook( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_Page1 = wx.Panel( self.m_DisplayNotebook, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		fgSizer1 = wx.FlexGridSizer( 6, 2, 0, 0 )
		fgSizer1.AddGrowableCol( 0 )
		fgSizer1.AddGrowableCol( 1 )
		fgSizer1.AddGrowableRow( 0 )
		fgSizer1.AddGrowableRow( 1 )
		fgSizer1.AddGrowableRow( 2 )
		fgSizer1.AddGrowableRow( 3 )
		fgSizer1.AddGrowableRow( 4 )
		fgSizer1.AddGrowableRow( 5 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_DataText = wx.StaticText( self.m_Page1, wx.ID_ANY, u"Data", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_DataText.Wrap( -1 )
		fgSizer1.Add( self.m_DataText, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		m_DataChoiceChoices = []
		self.m_DataChoice = wx.Choice( self.m_Page1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_DataChoiceChoices, 0 )
		self.m_DataChoice.SetSelection( 0 )
		fgSizer1.Add( self.m_DataChoice, 0, wx.ALL|wx.EXPAND|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_SortText = wx.StaticText( self.m_Page1, wx.ID_ANY, u"Sort", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_SortText.Wrap( -1 )
		fgSizer1.Add( self.m_SortText, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		m_SortChoiceChoices = [ u"Descending", u"Ascending" ]
		self.m_SortChoice = wx.Choice( self.m_Page1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_SortChoiceChoices, 0 )
		self.m_SortChoice.SetSelection( 0 )
		fgSizer1.Add( self.m_SortChoice, 0, wx.ALL|wx.EXPAND|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_StartText = wx.StaticText( self.m_Page1, wx.ID_ANY, u"Start", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_StartText.Wrap( -1 )
		fgSizer1.Add( self.m_StartText, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		m_StartChoiceChoices = []
		self.m_StartChoice = wx.Choice( self.m_Page1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_StartChoiceChoices, 0 )
		self.m_StartChoice.SetSelection( 0 )
		fgSizer1.Add( self.m_StartChoice, 0, wx.ALL|wx.EXPAND|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_StopText = wx.StaticText( self.m_Page1, wx.ID_ANY, u"Stop", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_StopText.Wrap( -1 )
		fgSizer1.Add( self.m_StopText, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		m_StopChoiceChoices = []
		self.m_StopChoice = wx.Choice( self.m_Page1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_StopChoiceChoices, 0 )
		self.m_StopChoice.SetSelection( 0 )
		fgSizer1.Add( self.m_StopChoice, 0, wx.ALL|wx.EXPAND|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_StepsText = wx.StaticText( self.m_Page1, wx.ID_ANY, u"Number of Steps", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_StepsText.Wrap( -1 )
		fgSizer1.Add( self.m_StepsText, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_StepsCtrl = wx.SpinCtrl( self.m_Page1, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 0, 10000, 10 )
		fgSizer1.Add( self.m_StepsCtrl, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_staticTime = wx.StaticText( self.m_Page1, wx.ID_ANY, u"Time per Step (deci)", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticTime.Wrap( -1 )
		fgSizer1.Add( self.m_staticTime, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_SpinTime = wx.SpinCtrl( self.m_Page1, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 0, 1000, 10 )
		fgSizer1.Add( self.m_SpinTime, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_Page1.SetSizer( fgSizer1 )
		self.m_Page1.Layout()
		fgSizer1.Fit( self.m_Page1 )
		self.m_DisplayNotebook.AddPage( self.m_Page1, u"Default", True )
		self.m_Page2 = wx.Panel( self.m_DisplayNotebook, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
		fgSizer3 = wx.FlexGridSizer( 7, 2, 0, 0 )
		fgSizer3.AddGrowableCol( 0 )
		fgSizer3.AddGrowableCol( 1 )
		fgSizer3.AddGrowableRow( 0 )
		fgSizer3.AddGrowableRow( 1 )
		fgSizer3.AddGrowableRow( 2 )
		fgSizer3.AddGrowableRow( 3 )
		fgSizer3.AddGrowableRow( 4 )
		fgSizer3.AddGrowableRow( 5 )
		fgSizer3.AddGrowableRow( 6 )
		fgSizer3.SetFlexibleDirection( wx.BOTH )
		fgSizer3.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_ColouringText = wx.StaticText( self.m_Page2, wx.ID_ANY, u"Colour by Intensity", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_ColouringText.Wrap( -1 )
		fgSizer3.Add( self.m_ColouringText, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_CheckIntensity = wx.CheckBox( self.m_Page2, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer3.Add( self.m_CheckIntensity, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_BinText = wx.StaticText( self.m_Page2, wx.ID_ANY, u"Binning", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_BinText.Wrap( -1 )
		fgSizer3.Add( self.m_BinText, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_CheckBin = wx.CheckBox( self.m_Page2, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer3.Add( self.m_CheckBin, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_StepSizeText = wx.StaticText( self.m_Page2, wx.ID_ANY, u"Step Size", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_StepSizeText.Wrap( -1 )
		fgSizer3.Add( self.m_StepSizeText, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_StepSizeTextCtrl = wx.TextCtrl( self.m_Page2, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_READONLY )
		fgSizer3.Add( self.m_StepSizeTextCtrl, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_BinStartText = wx.StaticText( self.m_Page2, wx.ID_ANY, u"Bin Start", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_BinStartText.Wrap( -1 )
		fgSizer3.Add( self.m_BinStartText, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_BinStartCtrl = wx.TextCtrl( self.m_Page2, wx.ID_ANY, u"0", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_BinStartCtrl.SetMaxLength( 1000 ) 
		fgSizer3.Add( self.m_BinStartCtrl, 0, wx.ALL, 5 )
		
		self.m_BinEndText = wx.StaticText( self.m_Page2, wx.ID_ANY, u"Bin End", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_BinEndText.Wrap( -1 )
		fgSizer3.Add( self.m_BinEndText, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_BinEndCtrl = wx.TextCtrl( self.m_Page2, wx.ID_ANY, u"0", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_BinEndCtrl.SetMaxLength( 1000 ) 
		fgSizer3.Add( self.m_BinEndCtrl, 0, wx.ALL, 5 )
		
		self.m_RadioDayFirst = wx.RadioButton( self.m_Page2, wx.ID_ANY, u"D/M/Y", wx.DefaultPosition, wx.DefaultSize, wx.RB_GROUP )
		fgSizer3.Add( self.m_RadioDayFirst, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		self.m_RadioMonthFirst = wx.RadioButton( self.m_Page2, wx.ID_ANY, u"M/D/Y", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_RadioMonthFirst.SetValue( True ) 
		fgSizer3.Add( self.m_RadioMonthFirst, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		
		fgSizer3.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_Default = wx.Button( self.m_Page2, wx.ID_ANY, u"Restore", wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer3.Add( self.m_Default, 0, wx.ALL|wx.EXPAND, 5 )
		
		self.m_Page2.SetSizer( fgSizer3 )
		self.m_Page2.Layout()
		fgSizer3.Fit( self.m_Page2 )
		self.m_DisplayNotebook.AddPage( self.m_Page2, u"Advanced", False )
		
		bSizer1.Add( self.m_DisplayNotebook, 1, wx.EXPAND |wx.ALL, 5 )
		
		bSizer2 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_Help = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.Size( 30,-1 ), 0 )
		bSizer2.Add( self.m_Help, 0, wx.ALL, 5 )
		
		
		bSizer2.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_OK = wx.Button( self, wx.ID_ANY, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer2.Add( self.m_OK, 0, wx.ALL, 5 )
		
		self.m_Close = wx.Button( self, wx.ID_ANY, u"Close", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer2.Add( self.m_Close, 0, wx.ALL, 5 )
		
		bSizer1.Add( bSizer2, 0, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer1 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.m_DataChoice.Bind( wx.EVT_CHOICE, self.OnDataChange )
		self.m_SortChoice.Bind( wx.EVT_CHOICE, self.OnSort )
		self.m_StepsCtrl.Bind( wx.EVT_SPINCTRL, self.OnSteps )
		self.m_StepsCtrl.Bind( wx.EVT_TEXT, self.OnSteps )
		self.m_CheckIntensity.Bind( wx.EVT_CHECKBOX, self.OnColourIntensity )
		self.m_CheckBin.Bind( wx.EVT_CHECKBOX, self.OnBinning )
		self.m_RadioDayFirst.Bind( wx.EVT_RADIOBUTTON, self.OnDateRadio )
		self.m_RadioMonthFirst.Bind( wx.EVT_RADIOBUTTON, self.OnDateRadio )
		self.m_Default.Bind( wx.EVT_BUTTON, self.OnRestore )
		self.m_Help.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.m_OK.Bind( wx.EVT_BUTTON, self.OnOK )
		self.m_Close.Bind( wx.EVT_BUTTON, self.OnClose )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnDataChange( self, event ):
		event.Skip()
	
	def OnSort( self, event ):
		event.Skip()
	
	def OnSteps( self, event ):
		event.Skip()
	
	
	def OnColourIntensity( self, event ):
		event.Skip()
	
	def OnBinning( self, event ):
		event.Skip()
	
	def OnDateRadio( self, event ):
		event.Skip()
	
	
	def OnRestore( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	
	def OnClose( self, event ):
		event.Skip()
	

