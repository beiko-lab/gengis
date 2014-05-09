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
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Show Spread", pos = wx.DefaultPosition, size = wx.Size( 250,310 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 240,300 ), wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer1 = wx.BoxSizer( wx.VERTICAL )
		
		fgSizer1 = wx.FlexGridSizer( 8, 2, 0, 0 )
		fgSizer1.AddGrowableCol( 0 )
		fgSizer1.AddGrowableCol( 1 )
		fgSizer1.AddGrowableRow( 0 )
		fgSizer1.AddGrowableRow( 1 )
		fgSizer1.AddGrowableRow( 2 )
		fgSizer1.AddGrowableRow( 3 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_DataText = wx.StaticText( self, wx.ID_ANY, u"Data", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_DataText.Wrap( -1 )
		fgSizer1.Add( self.m_DataText, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		m_DataChoiceChoices = []
		self.m_DataChoice = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_DataChoiceChoices, 0 )
		self.m_DataChoice.SetSelection( 0 )
		fgSizer1.Add( self.m_DataChoice, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.EXPAND|wx.TOP|wx.RIGHT|wx.LEFT, 5 )
		
		self.m_SortText = wx.StaticText( self, wx.ID_ANY, u"Sort", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_SortText.Wrap( -1 )
		fgSizer1.Add( self.m_SortText, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		m_SortChoiceChoices = [ u"Descending", u"Ascending" ]
		self.m_SortChoice = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_SortChoiceChoices, 0 )
		self.m_SortChoice.SetSelection( 0 )
		fgSizer1.Add( self.m_SortChoice, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.EXPAND|wx.ALL, 5 )
		
		self.m_StartText = wx.StaticText( self, wx.ID_ANY, u"Start", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_StartText.Wrap( -1 )
		fgSizer1.Add( self.m_StartText, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		m_StartChoiceChoices = []
		self.m_StartChoice = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_StartChoiceChoices, 0 )
		self.m_StartChoice.SetSelection( 0 )
		fgSizer1.Add( self.m_StartChoice, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.EXPAND, 5 )
		
		self.m_StopText = wx.StaticText( self, wx.ID_ANY, u"Stop", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_StopText.Wrap( -1 )
		fgSizer1.Add( self.m_StopText, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		m_StopChoiceChoices = []
		self.m_StopChoice = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_StopChoiceChoices, 0 )
		self.m_StopChoice.SetSelection( 0 )
		fgSizer1.Add( self.m_StopChoice, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.EXPAND, 5 )
		
		self.m_StepsText = wx.StaticText( self, wx.ID_ANY, u"Number of Steps", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_StepsText.Wrap( -1 )
		fgSizer1.Add( self.m_StepsText, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		self.m_StepsCtrl = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 0, 10000, 10 )
		fgSizer1.Add( self.m_StepsCtrl, 0, wx.ALL, 5 )
		
		self.m_colouringText = wx.StaticText( self, wx.ID_ANY, u"Colour by Intensity", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_colouringText.Wrap( -1 )
		fgSizer1.Add( self.m_colouringText, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		self.m_checkIntensity = wx.CheckBox( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_checkIntensity.SetValue(True) 
		fgSizer1.Add( self.m_checkIntensity, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_radioDayFirst = wx.RadioButton( self, wx.ID_ANY, u"D/M/Y", wx.DefaultPosition, wx.DefaultSize, wx.RB_GROUP )
		fgSizer1.Add( self.m_radioDayFirst, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_radioMonthFirst = wx.RadioButton( self, wx.ID_ANY, u"M/D/Y", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_radioMonthFirst.SetValue( True ) 
		fgSizer1.Add( self.m_radioMonthFirst, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_staticTime = wx.StaticText( self, wx.ID_ANY, u"Time per Step (deci)", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticTime.Wrap( -1 )
		fgSizer1.Add( self.m_staticTime, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_spinTime = wx.SpinCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS, 0, 1000, 10 )
		fgSizer1.Add( self.m_spinTime, 0, wx.ALL, 5 )
		
		bSizer1.Add( fgSizer1, 1, wx.EXPAND, 5 )
		
		bSizer2 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_Help = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.Size( 30,-1 ), 0 )
		bSizer2.Add( self.m_Help, 0, wx.ALL|wx.ALIGN_BOTTOM, 5 )
		
		
		bSizer2.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_OK = wx.Button( self, wx.ID_ANY, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer2.Add( self.m_OK, 0, wx.ALL|wx.ALIGN_BOTTOM, 5 )
		
		self.m_Close = wx.Button( self, wx.ID_ANY, u"Close", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer2.Add( self.m_Close, 0, wx.ALL|wx.ALIGN_BOTTOM, 5 )
		
		bSizer1.Add( bSizer2, 0, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer1 )
		self.Layout()
		self.m_statusBar1 = self.CreateStatusBar( 1, wx.ST_SIZEGRIP, wx.ID_ANY )
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.m_DataChoice.Bind( wx.EVT_CHOICE, self.OnDataChange )
		self.m_SortChoice.Bind( wx.EVT_CHOICE, self.OnSort )
		self.m_StepsCtrl.Bind( wx.EVT_SPINCTRL, self.OnSteps )
		self.m_StepsCtrl.Bind( wx.EVT_TEXT, self.OnSteps )
		self.m_checkIntensity.Bind( wx.EVT_CHECKBOX, self.OnColourIntensity )
		self.m_radioDayFirst.Bind( wx.EVT_RADIOBUTTON, self.OnDateRadio )
		self.m_radioMonthFirst.Bind( wx.EVT_RADIOBUTTON, self.OnDateRadio )
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
	
	def OnDateRadio( self, event ):
		event.Skip()
	
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	
	def OnClose( self, event ):
		event.Skip()
	

