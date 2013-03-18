# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Sep  8 2010)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx

###########################################################################
## Class MultiTreeOptimalCrossingTestLayout
###########################################################################

class MultiTreeOptimalCrossingTestLayout ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"Multi-Tree Optimal-Crossing Test", pos = wx.DefaultPosition, size = wx.Size( 1072,732 ), style = wx.CAPTION|wx.CLOSE_BOX|wx.SYSTEM_MENU|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( -1,165 ), wx.DefaultSize )
		self.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_BTNFACE ) )
		
		bSizer1 = wx.BoxSizer( wx.HORIZONTAL )
		
		bSizer7 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer3 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Angle" ), wx.VERTICAL )
		
		bSizer11 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.lblInfo = wx.StaticText( self, wx.ID_ANY, u"Global-Optimal Angle: ", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblInfo.Wrap( -1 )
		bSizer11.Add( self.lblInfo, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.lblGlobalOptimalAngle = wx.StaticText( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblGlobalOptimalAngle.Wrap( -1 )
		bSizer11.Add( self.lblGlobalOptimalAngle, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer3.Add( bSizer11, 0, wx.EXPAND, 5 )
		
		bSizer10 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.lblAngleSel = wx.StaticText( self, wx.ID_ANY, u"Angle: ", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblAngleSel.Wrap( -1 )
		bSizer10.Add( self.lblAngleSel, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.txtAngle = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer10.Add( self.txtAngle, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer3.Add( bSizer10, 0, wx.EXPAND, 5 )
		
		bSizer13 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_textCtrl21 = wx.TextCtrl( self, wx.ID_ANY, u"The global-optimal angle is determined by comparing all trees and determining at what angle the fewest crossings occur. A weighted global-optimal angle can be calculated by selecting an option from the dropdown menu below.\n\nInserting a value into the \"Angle\" field will calculate the fit of each tree to the given angle (represented in the bottom graph). Leaving the \"Angle\" field blank or inserting a value that is not a number  will default all calculations to the calculated global-optimal angle. \n\nThe Calulate button can be used to determine the Global-Optimal Angle and refresh both graphs", wx.DefaultPosition, wx.Size( -1,-1 ), wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_WORDWRAP )
		bSizer13.Add( self.m_textCtrl21, 1, wx.ALL|wx.EXPAND, 5 )
		
		sbSizer3.Add( bSizer13, 1, wx.EXPAND, 5 )
		
		bSizer15 = wx.BoxSizer( wx.VERTICAL )
		
		sbSizer3.Add( bSizer15, 0, wx.EXPAND, 5 )
		
		bSizer7.Add( sbSizer3, 1, wx.EXPAND, 5 )
		
		szrLocation1 = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Weight" ), wx.VERTICAL )
		
		bSizer82 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.lblCrossingWeighted = wx.StaticText( self, wx.ID_ANY, u"Weighted/Unweighted :", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblCrossingWeighted.Wrap( -1 )
		bSizer82.Add( self.lblCrossingWeighted, 1, wx.ALL|wx.EXPAND, 5 )
		
		chcCrossingWeightedChoices = [ u"Normalised", u"Unweighted" ]
		self.chcCrossingWeighted = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, chcCrossingWeightedChoices, 0 )
		self.chcCrossingWeighted.SetSelection( 1 )
		bSizer82.Add( self.chcCrossingWeighted, 1, wx.ALL|wx.ALIGN_BOTTOM|wx.EXPAND, 5 )
		
		szrLocation1.Add( bSizer82, 0, wx.EXPAND, 5 )
		
		bSizer9 = wx.BoxSizer( wx.VERTICAL )
		
		self.txtWeightInfo = wx.TextCtrl( self, wx.ID_ANY, u"The total number of crossings from accross all trees is determined by summing the unweighted number of crossings for each tree.", wx.DefaultPosition, wx.Size( -1,-1 ), wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_WORDWRAP )
		bSizer9.Add( self.txtWeightInfo, 1, wx.ALL|wx.EXPAND, 5 )
		
		szrLocation1.Add( bSizer9, 1, wx.EXPAND, 5 )
		
		self.btnCalculate = wx.Button( self, wx.ID_ANY, u"Calculate", wx.DefaultPosition, wx.DefaultSize, 0 )
		szrLocation1.Add( self.btnCalculate, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		bSizer7.Add( szrLocation1, 1, wx.EXPAND, 5 )
		
		szrLocation = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Tree Rotation" ), wx.VERTICAL )
		
		bSizer12 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.lblTreeSel = wx.StaticText( self, wx.ID_ANY, u"Tree:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblTreeSel.Wrap( -1 )
		bSizer12.Add( self.lblTreeSel, 1, wx.ALL|wx.EXPAND, 5 )
		
		chcTreeSelChoices = []
		self.chcTreeSel = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, chcTreeSelChoices, 0 )
		self.chcTreeSel.SetSelection( 0 )
		bSizer12.Add( self.chcTreeSel, 1, wx.ALL|wx.EXPAND, 5 )
		
		szrLocation.Add( bSizer12, 0, wx.EXPAND, 5 )
		
		bSizer101 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.lblTreeAngleSel = wx.StaticText( self, wx.ID_ANY, u"Angle: ", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.lblTreeAngleSel.Wrap( -1 )
		bSizer101.Add( self.lblTreeAngleSel, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.txtTreeAngle = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer101.Add( self.txtTreeAngle, 1, wx.ALL|wx.EXPAND, 5 )
		
		szrLocation.Add( bSizer101, 0, wx.EXPAND, 5 )
		
		bSizer14 = wx.BoxSizer( wx.VERTICAL )
		
		self.m_textCtrl211 = wx.TextCtrl( self, wx.ID_ANY, u"Rotate any tree in the dropdown menu to any angle: Select the tree to be rotated from the dropdown menu. As a default, if the \"Angle\" field is left empty or contains a value that is not a number, the tree will rotate to the global-optimal angle. Otherwise, insert an angle to have the tree rotated to. \n\nClick the \"Set Angle\" button to complete the rotation. ", wx.DefaultPosition, wx.Size( -1,-1 ), wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_WORDWRAP )
		bSizer14.Add( self.m_textCtrl211, 1, wx.ALL|wx.EXPAND, 5 )
		
		szrLocation.Add( bSizer14, 1, wx.EXPAND, 5 )
		
		bSizer8 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_btnHelp = wx.Button( self, wx.ID_ANY, u"?", wx.DefaultPosition, wx.DefaultSize, wx.BU_EXACTFIT )
		self.m_btnHelp.SetToolTipString( u"Help" )
		
		bSizer8.Add( self.m_btnHelp, 0, wx.ALL, 5 )
		
		
		bSizer8.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		
		self.btnSetAngle = wx.Button( self, wx.ID_ANY, u"Set Angle", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.btnSetAngle.Enable( False )
		
		bSizer8.Add( self.btnSetAngle, 0, wx.ALL|wx.ALIGN_RIGHT, 5 )
		
		szrLocation.Add( bSizer8, 0, wx.EXPAND, 5 )
		
		bSizer7.Add( szrLocation, 1, wx.EXPAND, 5 )
		
		bSizer1.Add( bSizer7, 0, wx.EXPAND, 5 )
		
		bSizer5 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer81 = wx.BoxSizer( wx.VERTICAL )
		
		self.pnlStepGraph = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.SIMPLE_BORDER|wx.TAB_TRAVERSAL )
		self.pnlStepGraph.SetForegroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_CAPTIONTEXT ) )
		self.pnlStepGraph.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_CAPTIONTEXT ) )
		
		bSizer81.Add( self.pnlStepGraph, 1, wx.EXPAND |wx.ALL, 5 )
		
		bSizer5.Add( bSizer81, 1, wx.EXPAND, 5 )
		
		bSizer91 = wx.BoxSizer( wx.VERTICAL )
		
		self.pnlBarGraph = wx.Panel( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.SIMPLE_BORDER|wx.TAB_TRAVERSAL )
		self.pnlBarGraph.SetForegroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_CAPTIONTEXT ) )
		self.pnlBarGraph.SetBackgroundColour( wx.SystemSettings.GetColour( wx.SYS_COLOUR_CAPTIONTEXT ) )
		
		bSizer91.Add( self.pnlBarGraph, 1, wx.EXPAND |wx.ALL, 5 )
		
		bSizer5.Add( bSizer91, 1, wx.EXPAND, 5 )
		
		bSizer3 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.btnClose = wx.Button( self, wx.ID_ANY, u"Close", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer3.Add( self.btnClose, 0, wx.ALL, 5 )
		
		bSizer5.Add( bSizer3, 0, wx.ALIGN_RIGHT, 5 )
		
		bSizer1.Add( bSizer5, 1, wx.EXPAND, 5 )
		
		self.SetSizer( bSizer1 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_SIZE, self.OnSize )
		self.chcCrossingWeighted.Bind( wx.EVT_CHOICE, self.OnWeightSelection )
		self.btnCalculate.Bind( wx.EVT_BUTTON, self.OnCalculate )
		self.m_btnHelp.Bind( wx.EVT_BUTTON, self.OnHelp )
		self.btnSetAngle.Bind( wx.EVT_BUTTON, self.OnSetAngle )
		self.btnClose.Bind( wx.EVT_BUTTON, self.OnClose )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def OnSize( self, event ):
		event.Skip()
	
	def OnWeightSelection( self, event ):
		event.Skip()
	
	def OnCalculate( self, event ):
		event.Skip()
	
	def OnHelp( self, event ):
		event.Skip()
	
	def OnSetAngle( self, event ):
		event.Skip()
	
	def OnClose( self, event ):
		event.Skip()
	

