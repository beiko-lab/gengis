# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class AlphaDiversityLayout
###########################################################################

class AlphaDiversityLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Alpha-diversity calculator", pos = wx.DefaultPosition, size = wx.Size( 600,600 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer9 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer5 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer4 = wx.BoxSizer( wx.VERTICAL )
		
		fgSizer2 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer2.AddGrowableCol( 1 )
		fgSizer2.SetFlexibleDirection( wx.BOTH )
		fgSizer2.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText7 = wx.StaticText( self, wx.ID_ANY, u"Measure:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText7.Wrap( -1 )
		fgSizer2.Add( self.m_staticText7, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboMeasureChoices = []
		self.cboMeasure = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboMeasureChoices, 0 )
		self.cboMeasure.SetSelection( 0 )
		fgSizer2.Add( self.cboMeasure, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText8 = wx.StaticText( self, wx.ID_ANY, u"Category field:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText8.Wrap( -1 )
		fgSizer2.Add( self.m_staticText8, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboCategoryFieldChoices = []
		self.cboCategoryField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboCategoryFieldChoices, 0 )
		self.cboCategoryField.SetSelection( 0 )
		fgSizer2.Add( self.cboCategoryField, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText3 = wx.StaticText( self, wx.ID_ANY, u"Count field:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText3.Wrap( -1 )
		fgSizer2.Add( self.m_staticText3, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboCountFieldChoices = []
		self.cboCountField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboCountFieldChoices, 0 )
		self.cboCountField.SetSelection( 0 )
		fgSizer2.Add( self.cboCountField, 0, wx.ALL|wx.EXPAND, 5 )
		
		bSizer4.Add( fgSizer2, 0, wx.EXPAND, 5 )
		
		self.chkIgnoreOther = wx.CheckBox( self, wx.ID_ANY, u"Ignore 'Other' and 'Unclassified' categories", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer4.Add( self.chkIgnoreOther, 0, wx.ALL, 5 )
		
		sbSizer2 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Subsampling without Replacement" ), wx.VERTICAL )
		
		self.chkSubsample = wx.CheckBox( self, wx.ID_ANY, u"Perform subsampling", wx.DefaultPosition, wx.DefaultSize, 0 )
		sbSizer2.Add( self.chkSubsample, 0, wx.ALL, 5 )
		
		fgSizer3 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer3.AddGrowableCol( 1 )
		fgSizer3.SetFlexibleDirection( wx.BOTH )
		fgSizer3.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText71 = wx.StaticText( self, wx.ID_ANY, u"Replicates:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText71.Wrap( -1 )
		fgSizer3.Add( self.m_staticText71, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboReplicatesChoices = [ u"10", u"100", u"1000" ]
		self.cboReplicates = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboReplicatesChoices, 0 )
		self.cboReplicates.SetSelection( 1 )
		self.cboReplicates.Enable( False )
		
		fgSizer3.Add( self.cboReplicates, 0, wx.ALL, 5 )
		
		self.m_staticText81 = wx.StaticText( self, wx.ID_ANY, u"Sequences to draw:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText81.Wrap( -1 )
		fgSizer3.Add( self.m_staticText81, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.txtSeqsToDraw = wx.TextCtrl( self, wx.ID_ANY, u"100", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.txtSeqsToDraw.Enable( False )
		
		fgSizer3.Add( self.txtSeqsToDraw, 0, wx.ALL|wx.EXPAND, 5 )
		
		
		fgSizer3.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnMinimumSeqs = wx.Button( self, wx.ID_ANY, u"Min. Seqs in Sample", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.btnMinimumSeqs.Enable( False )
		
		fgSizer3.Add( self.btnMinimumSeqs, 0, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer2.Add( fgSizer3, 1, wx.EXPAND, 5 )
		
		bSizer4.Add( sbSizer2, 0, wx.ALL|wx.EXPAND, 5 )
		
		bSizer5.Add( bSizer4, 0, wx.EXPAND, 5 )
		
		sbSizer1 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Details" ), wx.VERTICAL )
		
		fgSizer21 = wx.FlexGridSizer( 3, 2, 0, 0 )
		fgSizer21.AddGrowableCol( 1 )
		fgSizer21.AddGrowableRow( 2 )
		fgSizer21.SetFlexibleDirection( wx.BOTH )
		fgSizer21.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText4 = wx.StaticText( self, wx.ID_ANY, u"Full name:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText4.Wrap( -1 )
		fgSizer21.Add( self.m_staticText4, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.txtFullName = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_READONLY )
		fgSizer21.Add( self.txtFullName, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText5 = wx.StaticText( self, wx.ID_ANY, u"Formula:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText5.Wrap( -1 )
		fgSizer21.Add( self.m_staticText5, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		self.txtFormula = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_READONLY )
		fgSizer21.Add( self.txtFormula, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText6 = wx.StaticText( self, wx.ID_ANY, u"Description:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText6.Wrap( -1 )
		fgSizer21.Add( self.m_staticText6, 0, wx.ALL, 5 )
		
		self.txtDescription = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_WORDWRAP )
		self.txtDescription.SetMinSize( wx.Size( -1,75 ) )
		
		fgSizer21.Add( self.txtDescription, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer1.Add( fgSizer21, 1, wx.EXPAND, 5 )
		
		bSizer5.Add( sbSizer1, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer9.Add( bSizer5, 0, wx.EXPAND, 5 )
		
		bSizer6 = wx.BoxSizer( wx.VERTICAL )
		
		self.btnCalculate = wx.Button( self, wx.ID_ANY, u"Calculate", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer6.Add( self.btnCalculate, 0, wx.ALL, 5 )
		
		self.txtLog = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_MULTILINE )
		bSizer6.Add( self.txtLog, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer2 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnHelp = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		self.m_btnHelp.SetToolTipString( u"Help" )
		
		bSizer2.Add( self.m_btnHelp, 0, wx.ALL, 5 )
		
		
		bSizer2.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnOK = wx.Button( self, wx.ID_ANY, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer2.Add( self.btnOK, 0, wx.ALL, 5 )
		
		bSizer6.Add( bSizer2, 0, wx.EXPAND, 5 )
		
		bSizer9.Add( bSizer6, 1, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer9 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.OnClose )
		self.cboMeasure.Bind( wx.EVT_CHOICE, self.OnMeasureChanged )
		self.chkSubsample.Bind( wx.EVT_CHECKBOX, self.OnPerformSubsampling )
		self.btnMinimumSeqs.Bind( wx.EVT_BUTTON, self.OnSetMinimumSeqs )
		self.btnCalculate.Bind( wx.EVT_BUTTON, self.OnCalculate )
		self.m_btnHelp.Bind( wx.EVT_BUTTON, self.OnHelp )
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
	
	def OnCalculate( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

