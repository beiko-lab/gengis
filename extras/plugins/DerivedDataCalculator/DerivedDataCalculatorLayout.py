# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class DerivedDataCalculatorLayout
###########################################################################

class DerivedDataCalculatorLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Derived data calculator", pos = wx.DefaultPosition, size = wx.Size( 448,335 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer4 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer1 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Sequences per location" ), wx.VERTICAL )
		
		bSizer41 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_txtSeqPerLocDesc = wx.TextCtrl( self, wx.ID_ANY, u"Calculates the number of sequences collected at each location. It assumes identical sequences have been assigned a common id. This id field must be set as the 'Unique sequence id field\". The number of sequences of a given type found at a location must also be specified as the 'Sequence count field'.\n\nUsing this data, the number of unique and total sequences at each site can be calculated. The normalized number of unique sequences (= unique sequences / total sequences) can also be determined. Derived data is added to the metadata for each location.", wx.DefaultPosition, wx.DefaultSize, wx.TE_MULTILINE|wx.TE_READONLY )
		bSizer41.Add( self.m_txtSeqPerLocDesc, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer1.Add( bSizer41, 1, wx.EXPAND, 5 )
		
		fgSizer1 = wx.FlexGridSizer( 2, 2, 0, 0 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		self.m_staticText2 = wx.StaticText( self, wx.ID_ANY, u"Unique sequence id field:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText2.Wrap( -1 )
		fgSizer1.Add( self.m_staticText2, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboUniqueSeqIdFieldChoices = []
		self.cboUniqueSeqIdField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboUniqueSeqIdFieldChoices, 0 )
		self.cboUniqueSeqIdField.SetSelection( 0 )
		fgSizer1.Add( self.cboUniqueSeqIdField, 0, wx.ALL, 5 )
		
		self.m_staticText1 = wx.StaticText( self, wx.ID_ANY, u"Sequence count field:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText1.Wrap( -1 )
		fgSizer1.Add( self.m_staticText1, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5 )
		
		cboSeqCountFieldChoices = []
		self.cboSeqCountField = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, cboSeqCountFieldChoices, 0 )
		self.cboSeqCountField.SetSelection( 0 )
		fgSizer1.Add( self.cboSeqCountField, 0, wx.ALL, 5 )
		
		sbSizer1.Add( fgSizer1, 0, wx.EXPAND, 5 )
		
		bSizer3 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.btnUniqueSeqs = wx.Button( self, wx.ID_ANY, u"Unique sequences", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer3.Add( self.btnUniqueSeqs, 0, wx.ALL, 5 )
		
		self.btnTotalSeqs = wx.Button( self, wx.ID_ANY, u"Total sequences", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer3.Add( self.btnTotalSeqs, 0, wx.ALL, 5 )
		
		self.btnNormalizeUniqueSeqs = wx.Button( self, wx.ID_ANY, u"Normalized unique sequence count", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer3.Add( self.btnNormalizeUniqueSeqs, 0, wx.ALL, 5 )
		
		sbSizer1.Add( bSizer3, 0, wx.EXPAND, 5 )
		
		bSizer4.Add( sbSizer1, 1, wx.ALL|wx.EXPAND, 5 )
		
		bSizer5 = wx.BoxSizer( wx.HORIZONTAL )
		
		
		bSizer5.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnOK = wx.Button( self, wx.ID_OK, u"OK", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer5.Add( self.btnOK, 0, wx.ALL, 5 )
		
		bSizer4.Add( bSizer5, 0, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer4 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.btnUniqueSeqs.Bind( wx.EVT_BUTTON, self.OnUniqueSeqs )
		self.btnTotalSeqs.Bind( wx.EVT_BUTTON, self.OnTotalSeqs )
		self.btnNormalizeUniqueSeqs.Bind( wx.EVT_BUTTON, self.OnNormUniqueSeqs )
		self.btnOK.Bind( wx.EVT_BUTTON, self.OnOK )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnUniqueSeqs( self, event ):
		event.Skip()
	
	def OnTotalSeqs( self, event ):
		event.Skip()
	
	def OnNormUniqueSeqs( self, event ):
		event.Skip()
	
	def OnOK( self, event ):
		event.Skip()
	

