# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class PhylogeneticDistanceLayout
###########################################################################

class PhylogeneticDistanceLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Geographically Coupled Phylogenetic Distance Calculator", pos = wx.DefaultPosition, size = wx.Size( 650,600 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
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
		
		m_cboMeasureChoices = []
		self.m_cboMeasure = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_cboMeasureChoices, 0 )
		self.m_cboMeasure.SetSelection( 0 )
		fgSizer2.Add( self.m_cboMeasure, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText8 = wx.StaticText( self, wx.ID_ANY, u"Location Layer:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText8.Wrap( -1 )
		fgSizer2.Add( self.m_staticText8, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		m_cboLocationLayerFieldChoices = []
		self.m_cboLocationLayerField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_cboLocationLayerFieldChoices, 0 )
		self.m_cboLocationLayerField.SetSelection( 0 )
		fgSizer2.Add( self.m_cboLocationLayerField, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText3 = wx.StaticText( self, wx.ID_ANY, u"Tree Layer:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText3.Wrap( -1 )
		fgSizer2.Add( self.m_staticText3, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		m_cboTreeLayerFieldChoices = []
		self.m_cboTreeLayerField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_cboTreeLayerFieldChoices, 0 )
		self.m_cboTreeLayerField.SetSelection( 0 )
		fgSizer2.Add( self.m_cboTreeLayerField, 0, wx.ALL|wx.EXPAND, 5 )
		
		bSizer4.Add( fgSizer2, 0, wx.EXPAND, 5 )
		
		m_rboxPDStateChoices = [ u"PD", u"1-PD" ]
		self.m_rboxPDState = wx.RadioBox( self, wx.ID_ANY, u"PD State", wx.DefaultPosition, wx.DefaultSize, m_rboxPDStateChoices, 2, wx.RA_SPECIFY_COLS )
		self.m_rboxPDState.SetSelection( 1 )
		bSizer4.Add( self.m_rboxPDState, 0, wx.ALL, 5 )
		
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
		
		self.btnSaveDissMatrix = wx.Button( self, wx.ID_ANY, u"Add To GenGIS", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer2.Add( self.btnSaveDissMatrix, 0, wx.ALL, 5 )
		
		
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
		self.m_cboMeasure.Bind( wx.EVT_CHOICE, self.OnMeasureChanged )
		self.btnCalculate.Bind( wx.EVT_BUTTON, self.OnCalculate )
		self.m_btnHelp.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.btnSaveDissMatrix.Bind( wx.EVT_BUTTON, self.OnAddToGenGIS )
		self.btnOK.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnClose( self, event ):
		event.Skip()
	
	def OnMeasureChanged( self, event ):
		event.Skip()
	
	def OnCalculate( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnAddToGenGIS( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

