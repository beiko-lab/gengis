# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class WorldClimQueryLayout
###########################################################################

class WorldClimQueryLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"WorldClim Query", pos = wx.DefaultPosition, size = wx.Size( 600,600 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer9 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer5 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer1 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Data" ), wx.VERTICAL )
		
		fgSizer21 = wx.FlexGridSizer( 4, 2, 0, 0 )
		fgSizer21.AddGrowableCol( 1 )
		fgSizer21.AddGrowableRow( 2 )
		fgSizer21.SetFlexibleDirection( wx.BOTH )
		fgSizer21.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_txtMeasure = wx.StaticText( self, wx.ID_ANY, u"Measure:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_txtMeasure.Wrap( -1 )
		fgSizer21.Add( self.m_txtMeasure, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		m_MeasuresChoices = []
		self.m_Measures = wx.ComboBox( self, wx.ID_ANY, u"Combo!", wx.DefaultPosition, wx.DefaultSize, m_MeasuresChoices, wx.CB_READONLY )
		fgSizer21.Add( self.m_Measures, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_txtFullName = wx.StaticText( self, wx.ID_ANY, u"Full name:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_txtFullName.Wrap( -1 )
		fgSizer21.Add( self.m_txtFullName, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.txtFullName = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_READONLY )
		fgSizer21.Add( self.txtFullName, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_txtDescription = wx.StaticText( self, wx.ID_ANY, u"Description:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_txtDescription.Wrap( -1 )
		fgSizer21.Add( self.m_txtDescription, 0, wx.ALL, 5 )
		
		self.txtDescription = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_WORDWRAP )
		self.txtDescription.SetMinSize( wx.Size( -1,75 ) )
		
		fgSizer21.Add( self.txtDescription, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer1.Add( fgSizer21, 1, wx.EXPAND, 5 )
		
		bSizer5.Add( sbSizer1, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer9.Add( bSizer5, 0, wx.EXPAND, 5 )
		
		bSizer6 = wx.BoxSizer( wx.VERTICAL )
		
		self.txtLog = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_MULTILINE )
		bSizer6.Add( self.txtLog, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer2 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnHelp = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		self.m_btnHelp.SetToolTipString( u"Help" )
		
		bSizer2.Add( self.m_btnHelp, 0, wx.ALL, 5 )
		
		self.m_about = wx.Button( self, wx.ID_ANY, u"About", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer2.Add( self.m_about, 0, wx.ALL, 5 )
		
		
		bSizer2.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnCalculate = wx.Button( self, wx.ID_ANY, u"Calculate", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer2.Add( self.btnCalculate, 0, wx.ALL, 5 )
		
		self.btnOK = wx.Button( self, wx.ID_ANY, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer2.Add( self.btnOK, 0, wx.ALL, 5 )
		
		bSizer6.Add( bSizer2, 0, wx.EXPAND, 5 )
		
		bSizer9.Add( bSizer6, 1, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer9 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.m_Measures.Bind( wx.EVT_COMBOBOX, self.OnMeasureChanged )
		self.m_btnHelp.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.m_about.Bind( wx.EVT_BUTTON, self.OnAbout )
		self.btnCalculate.Bind( wx.EVT_BUTTON, self.OnCalculate )
		self.btnOK.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnMeasureChanged( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnAbout( self, event ):
		event.Skip()
	
	def OnCalculate( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

###########################################################################
## Class DescriptionLayout
###########################################################################

class DescriptionLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"About", pos = wx.DefaultPosition, size = wx.Size( 292,205 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 292,205 ), wx.Size( 292,205 ) )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer5 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer2 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Description" ), wx.VERTICAL )
		
		self.m_description = wx.StaticText( self, wx.ID_ANY, u"MyLabel", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_description.Wrap( 10 )
		sbSizer2.Add( self.m_description, 0, wx.ALL, 5 )
		
		self.m_close = wx.Button( self, wx.ID_ANY, u"Close", wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer2.Add( self.m_close, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		bSizer5.Add( sbSizer2, 1, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer5 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.m_close.Bind( wx.EVT_BUTTON, self.OnClose )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	

