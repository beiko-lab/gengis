# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class IRIDAQueryLayout
###########################################################################

class IRIDAQueryLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"IRIDA Query", pos = wx.DefaultPosition, size = wx.Size( 790,628 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 790,628 ), wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer1 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer2 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer3 = wx.BoxSizer( wx.VERTICAL )
		
		
		bSizer3.AddSpacer( ( 0, 0), 0, 0, 5 )
		
		sbSizer2 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Query" ), wx.HORIZONTAL )
		
		self.m_ProjectName = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer2.Add( self.m_ProjectName, 1, wx.ALL, 5 )
		
		self.m_Search = wx.Button( self, wx.ID_ANY, u"Search", wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer2.Add( self.m_Search, 0, wx.ALL, 5 )
		
		bSizer3.Add( sbSizer2, 0, wx.EXPAND|wx.ALL, 5 )
		
		sbSizer8 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Progress" ), wx.HORIZONTAL )
		
		sbSizer8.SetMinSize( wx.Size( 300,250 ) ) 
		self.m_Progress = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.Size( -1,-1 ), wx.TE_DONTWRAP|wx.TE_MULTILINE|wx.TE_READONLY|wx.CLIP_CHILDREN )
		self.m_Progress.SetMinSize( wx.Size( 300,100 ) )
		
		sbSizer8.Add( self.m_Progress, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer3.Add( sbSizer8, 1, wx.EXPAND|wx.RIGHT|wx.LEFT, 5 )
		
		bSizer12 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_Help = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		bSizer12.Add( self.m_Help, 0, wx.ALL, 5 )
		
		self.m_AddData = wx.Button( self, wx.ID_ANY, u"Add Data to Map", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer12.Add( self.m_AddData, 0, wx.ALL, 5 )
		
		self.m_ExportData = wx.Button( self, wx.ID_ANY, u"Export Data to File...", wx.DefaultPosition, wx.DefaultSize, 0|wx.TAB_TRAVERSAL )
		bSizer12.Add( self.m_ExportData, 0, wx.ALL, 5 )
		
		
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
		
		gSizer2 = wx.GridSizer( 1, 1, 0, 0 )
		
		self.m_Calc = wx.Button( self, wx.ID_ANY, u"Retrieve Data", wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer2.Add( self.m_Calc, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_RIGHT, 5 )
		
		bSizer11.Add( gSizer2, 0, wx.EXPAND, 5 )
		
		sbSizer7.Add( bSizer11, 1, wx.EXPAND, 5 )
		
		bSizer8.Add( sbSizer7, 1, wx.EXPAND|wx.ALL, 5 )
		
		bSizer5.Add( bSizer8, 1, wx.EXPAND|wx.ALL, 5 )
		
		bSizer14 = wx.BoxSizer( wx.HORIZONTAL )
		
		
		bSizer14.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_Login = wx.Button( self, wx.ID_ANY, u"Login", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer14.Add( self.m_Login, 0, wx.ALL, 5 )
		
		self.m_Credentials = wx.Button( self, wx.ID_ANY, u"Manage Credentials", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer14.Add( self.m_Credentials, 0, wx.ALL, 5 )
		
		self.m_Close = wx.Button( self, wx.ID_ANY, u"Close", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer14.Add( self.m_Close, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		bSizer5.Add( bSizer14, 0, wx.EXPAND|wx.ALL, 5 )
		
		bSizer2.Add( bSizer5, 1, wx.EXPAND, 5 )
		
		bSizer1.Add( bSizer2, 1, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer1 )
		self.Layout()
		self.m_statusBar = self.CreateStatusBar( 1, wx.ST_SIZEGRIP, wx.ID_ANY )
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.m_Search.Bind( wx.EVT_BUTTON, self.OnSearch )
		self.m_Help.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.m_AddData.Bind( wx.EVT_BUTTON, self.OnAddData )
		self.m_ExportData.Bind( wx.EVT_BUTTON, self.OnExportData )
		self.m_Result.Bind( wx.EVT_LEFT_DCLICK, self.OnAdd )
		self.m_Add.Bind( wx.EVT_BUTTON, self.OnAdd )
		self.m_Remove.Bind( wx.EVT_BUTTON, self.OnRemove )
		self.m_IDList.Bind( wx.EVT_LEFT_DCLICK, self.OnRemove )
		self.m_Calc.Bind( wx.EVT_BUTTON, self.OnCalculate )
		self.m_Login.Bind( wx.EVT_BUTTON, self.ReLogin )
		self.m_Credentials.Bind( wx.EVT_BUTTON, self.OnCredentials )
		self.m_Close.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnSearch( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnAddData( self, event ):
		event.Skip()
	
	def OnExportData( self, event ):
		event.Skip()
	
	def OnAdd( self, event ):
		event.Skip()
	
	
	def OnRemove( self, event ):
		event.Skip()
	
	
	def OnCalculate( self, event ):
		event.Skip()
	
	def ReLogin( self, event ):
		event.Skip()
	
	def OnCredentials( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

###########################################################################
## Class IRIDALoginLayout2
###########################################################################

class IRIDALoginLayout2 ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = wx.EmptyString, pos = wx.DefaultPosition, size = wx.Size( 352,149 ), style = wx.DEFAULT_FRAME_STYLE|wx.STAY_ON_TOP|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 352,149 ), wx.Size( -1,150 ) )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		sbSizer6 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Login" ), wx.VERTICAL )
		
		fgSizer3 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer3.AddGrowableCol( 0 )
		fgSizer3.AddGrowableCol( 1 )
		fgSizer3.SetFlexibleDirection( wx.BOTH )
		fgSizer3.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_UsernameText = wx.StaticText( self, wx.ID_ANY, u"Username:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_UsernameText.Wrap( -1 )
		fgSizer3.Add( self.m_UsernameText, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_UsernameCtrl = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer3.Add( self.m_UsernameCtrl, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_PasswordText = wx.StaticText( self, wx.ID_ANY, u"Password:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_PasswordText.Wrap( -1 )
		fgSizer3.Add( self.m_PasswordText, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_PasswordCtrl = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_PASSWORD )
		fgSizer3.Add( self.m_PasswordCtrl, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer6.Add( fgSizer3, 0, wx.EXPAND, 5 )
		
		bSizer11 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_save = wx.CheckBox( self, wx.ID_ANY, u"Save Credentials?", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_save.Hide()
		
		bSizer11.Add( self.m_save, 0, wx.ALL, 5 )
		
		
		bSizer11.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_OptionsClose = wx.Button( self, wx.ID_ANY, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_OptionsClose.SetDefault() 
		bSizer11.Add( self.m_OptionsClose, 0, wx.ALL|wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		sbSizer6.Add( bSizer11, 0, wx.EXPAND, 5 )
		
		self.SetSizer( sbSizer6 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.m_OptionsClose.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

###########################################################################
## Class IRIDACredentialsLayout
###########################################################################

class IRIDACredentialsLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Credentials Manager", pos = wx.DefaultPosition, size = wx.Size( 392,302 ), style = wx.DEFAULT_FRAME_STYLE|wx.STAY_ON_TOP|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer12 = wx.BoxSizer( wx.VERTICAL )
		
		fgSizer2 = wx.FlexGridSizer( 1, 2, 0, 0 )
		fgSizer2.AddGrowableCol( 0 )
		fgSizer2.AddGrowableCol( 1 )
		fgSizer2.AddGrowableRow( 0 )
		fgSizer2.SetFlexibleDirection( wx.BOTH )
		fgSizer2.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		m_AliasListChoices = []
		self.m_AliasList = wx.ListBox( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_AliasListChoices, 0 )
		fgSizer2.Add( self.m_AliasList, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer13 = wx.BoxSizer( wx.VERTICAL )
		
		
		bSizer13.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_AddCredential = wx.Button( self, wx.ID_ANY, u"Add", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer13.Add( self.m_AddCredential, 0, wx.ALL, 5 )
		
		self.m_EditCredential = wx.Button( self, wx.ID_ANY, u"Edit", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer13.Add( self.m_EditCredential, 0, wx.ALL, 5 )
		
		self.m_DeleteCredential = wx.Button( self, wx.ID_ANY, u"Delete", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer13.Add( self.m_DeleteCredential, 0, wx.ALL, 5 )
		
		self.m_Active = wx.Button( self, wx.ID_ANY, u"Make Active", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer13.Add( self.m_Active, 0, wx.ALL, 5 )
		
		fgSizer2.Add( bSizer13, 0, wx.ALIGN_CENTER_VERTICAL, 5 )
		
		bSizer12.Add( fgSizer2, 1, wx.EXPAND, 5 )
		
		self.m_CredentialsClose = wx.Button( self, wx.ID_ANY, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer12.Add( self.m_CredentialsClose, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		self.SetSizer( bSizer12 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.m_AddCredential.Bind( wx.EVT_BUTTON, self.OnAdd )
		self.m_EditCredential.Bind( wx.EVT_BUTTON, self.OnEdit )
		self.m_DeleteCredential.Bind( wx.EVT_BUTTON, self.OnDelete )
		self.m_Active.Bind( wx.EVT_BUTTON, self.OnActive )
		self.m_CredentialsClose.Bind( wx.EVT_BUTTON, self.OnCredentialClose )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnAdd( self, event ):
		event.Skip()
	
	def OnEdit( self, event ):
		event.Skip()
	
	def OnDelete( self, event ):
		event.Skip()
	
	def OnActive( self, event ):
		event.Skip()
	
	def OnCredentialClose( self, event ):
		event.Skip()
	

###########################################################################
## Class IRIDAClientLayout
###########################################################################

class IRIDAClientLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Client Information", pos = wx.DefaultPosition, size = wx.Size( 324,175 ), style = wx.DEFAULT_FRAME_STYLE|wx.STAY_ON_TOP|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer14 = wx.BoxSizer( wx.VERTICAL )
		
		fgSizer3 = wx.FlexGridSizer( 3, 2, 0, 0 )
		fgSizer3.AddGrowableCol( 1 )
		fgSizer3.SetFlexibleDirection( wx.BOTH )
		fgSizer3.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_AliasText = wx.StaticText( self, wx.ID_ANY, u"Alias", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_AliasText.Wrap( -1 )
		fgSizer3.Add( self.m_AliasText, 0, wx.ALL, 5 )
		
		self.m_AliasCtrl = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer3.Add( self.m_AliasCtrl, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_IDText = wx.StaticText( self, wx.ID_ANY, u"Client ID", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_IDText.Wrap( -1 )
		fgSizer3.Add( self.m_IDText, 0, wx.ALL, 5 )
		
		self.m_IDCtrl = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer3.Add( self.m_IDCtrl, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_SecretText = wx.StaticText( self, wx.ID_ANY, u"Client Secret", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_SecretText.Wrap( -1 )
		fgSizer3.Add( self.m_SecretText, 0, wx.ALL, 5 )
		
		self.m_SecretCtrl = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer3.Add( self.m_SecretCtrl, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer14.Add( fgSizer3, 1, wx.EXPAND, 5 )
		
		self.m_ClientClose = wx.Button( self, wx.ID_ANY, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer14.Add( self.m_ClientClose, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		self.SetSizer( bSizer14 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.m_ClientClose.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnOK( self, event ):
		event.Skip()
	

###########################################################################
## Class IRIDALoginLayout
###########################################################################

class IRIDALoginLayout ( wx.Dialog ):
	
	def __init__( self, parent ):
		wx.Dialog.__init__ ( self, parent, id = wx.ID_ANY, title = wx.EmptyString, pos = wx.DefaultPosition, size = wx.Size( 352,149 ), style = wx.DEFAULT_DIALOG_STYLE|wx.STAY_ON_TOP )
		
		self.SetSizeHintsSz( wx.Size( 352,149 ), wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		sbSizer6 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Login" ), wx.VERTICAL )
		
		fgSizer3 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer3.AddGrowableCol( 0 )
		fgSizer3.AddGrowableCol( 1 )
		fgSizer3.SetFlexibleDirection( wx.BOTH )
		fgSizer3.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_UsernameText = wx.StaticText( self, wx.ID_ANY, u"Username:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_UsernameText.Wrap( -1 )
		fgSizer3.Add( self.m_UsernameText, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_UsernameCtrl = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer3.Add( self.m_UsernameCtrl, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_PasswordText = wx.StaticText( self, wx.ID_ANY, u"Password:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_PasswordText.Wrap( -1 )
		fgSizer3.Add( self.m_PasswordText, 0, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL, 5 )
		
		self.m_PasswordCtrl = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_PASSWORD )
		fgSizer3.Add( self.m_PasswordCtrl, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer6.Add( fgSizer3, 0, wx.EXPAND, 5 )
		
		bSizer11 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_save = wx.CheckBox( self, wx.ID_ANY, u"Save Credentials?", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_save.Hide()
		
		bSizer11.Add( self.m_save, 0, wx.ALL, 5 )
		
		
		bSizer11.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.m_OptionsClose = wx.Button( self, wx.ID_ANY, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_OptionsClose.SetDefault() 
		bSizer11.Add( self.m_OptionsClose, 0, wx.ALL|wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		sbSizer6.Add( bSizer11, 0, wx.EXPAND, 5 )
		
		self.SetSizer( sbSizer6 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.m_OptionsClose.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

