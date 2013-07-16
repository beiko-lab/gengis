#=======================================================================
# Author: Donovan Parks
#
# Copyright 2013 Alexander Keddy
#
# This file is part of GenGIS.
#
# GenGIS is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# GenGIS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GenGIS.  If not, see <http://www.gnu.org/licenses/>.
#=======================================================================


from RCALayout import RCALayout

import GenGIS
from dataHelper import isNumber
from rpy2 import robjects

from CABIN_RCA import *

import wx
import math
import os

class RCA( RCALayout ):

	def __init__(self, parent=None):
		RCALayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumMapLayers() == 0 or GenGIS.layerTree.GetNumLocationSetLayers() == 0 or GenGIS.layerTree.GetNumSequenceLayers() == 0:
			wx.MessageBox("This plugin requires map, location, and sequence data to be loaded.", "Additional data required.")
			self.Close()
			return
		
		#Diversity measure choices
		metric_choices=("O/E\nRichness","O/E\nShannon","O/E\nSimpson","O/E\nPielou","O/E\nBerger_Parker")
		self.metrics=metric_choices

		#Populate RCA Model Choices
		self.cboModel.Append("atlantic_rca_model")
		self.cboModel.SetSelection(0)

		# populate Diversity and Count combo boxes with sequence column names
		sequenceLayer = GenGIS.layerTree.GetSequenceLayer(0)
		seqMetadataFields = sequenceLayer.GetController().GetMetadataFields()
		for field in seqMetadataFields:
			self.cboDiversity.Append(field)
			self.cboCount.Append(field)
		
		#try and set intelligent default choices for combo boxes
		good_diversity_choice = "Label"
		if good_diversity_choice in seqMetadataFields:
			self.cboDiversity.SetSelection(seqMetadataFields.index(good_diversity_choice))
		else:
			self.cboDiversity.SetSelection(0)

		good_count_choice = "Count"
		if good_count_choice in seqMetadataFields:
			self.cboCount.SetSelection(seqMetadataFields.index(good_count_choice))
		else:
			self.cboCount.SetSelection(0)

		# initialize table
		self.table.Freeze()
		#allow selecting of columns only
		self.table.SetSelectionMode(wx.grid.Grid.wxGridSelectColumns)
		
                #allow scrolling and resizing
		self.table.EnableScrolling(True, True)
		self.table.EnableDragRowSize(True)
		
		#Create column labels
		self.table.DeleteCols(0, self.table.GetNumberCols())
		self.table.AppendCols(len(metric_choices))
		for i,field in enumerate(metric_choices):
			self.table.SetColLabelValue(i, field)


		#get site ids
		locs = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
		site_ids=[loc.GetName() for loc in locs if loc.GetNumSequenceLayers() > 0]	
			
	        #Create row labels

		self.table.DeleteRows(0, self.table.GetNumberRows())
		self.table.AppendRows(len(site_ids))
		for i,value in enumerate(site_ids):
			self.table.SetRowLabelValue(i, value)
	
		#Resize row and col labels
		self.table.SetRowLabelSize(wx.grid.GRID_AUTOSIZE)
		self.table.SetCellHighlightPenWidth(0)
		self.table.SetColLabelSize(wx.grid.GRID_AUTOSIZE)
		self.table.AutoSizeColumns()
		self.table.Update()
		self.table.Thaw()
	

			
	def OnRun( self, event ):
		wx.BeginBusyCursor()
		if hasattr(self,'rca'):
                        self.rca.clearLines()
                        del self.rca
		rca = CABIN_RCA(self.cboDiversity.GetStringSelection(), self.cboCount.GetStringSelection())
		rca.Run_RCA(self.cboModel.GetStringSelection())
		
		self.rca=rca
		
		# place results in table
		self.table.Freeze()
		for col_id in xrange(0,self.table.GetNumberCols()):
			col_name = self.table.GetColLabelValue(col_id)
			for row_id in xrange(0, self.table.GetNumberRows()):
				row_name = self.table.GetRowLabelValue(row_id)
				self.table.SetCellValue(row_id,col_id,'%.3f' % self.rca.results[col_name,row_name])

		self.table.AutoSizeColumns()
		self.table.Thaw()
		wx.EndBusyCursor()

		
	def OnPlot(self, event):
		
		if len(self.selectedCols) != 1:
			wx.MessageBox("Select one (and only one) column to plot!")
			return
		if hasattr(self,'rca'):
			metric=self.table.GetColLabelValue(self.selectedCols[0])
			if isNumber(self.userScaleFactor.GetValue()):
				userScaleFactor = float(self.userScaleFactor.GetValue())
			else:
				wx.MessageBox("Scale factor must be numeric.", "Invalid scale factor")
				return

		        #Update the viewport using the metric selected by user
			self.rca.ViewportPlot(metric,userScaleFactor)
		else:
			wx.MessageBox("Please 'Run' this plugin before attempting to update the plot!")
			return

	def OnOK( self, event ):
		self.Close()

	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/Description_of_GenGIS_plugins#Reference_Condition_Analysis' )

	def OnClose( self, event ):
		if hasattr(self,'rca'):
			self.rca.clearLines()
			del self.rca		
		#self.Destroy()
                event.Skip()

	def OnAdd(self, event):

		if not hasattr(self,'rca'):
			wx.MessageBox("Please 'Run' this plugin before attempting to add results!")
			return
	
		#get list of all locations
		locs=GenGIS.layerTree.GetLocationSetLayer(0).GetAllLocationLayers()
		
		#get name of selected metric (assume only 1 column selected)
	       	metric=self.table.GetColLabelValue(self.selectedCols[0])
			

		#import pdb; pdb.set_trace()
	
	       	for loc in locs:
			data=''
			if (metric,loc.GetName()) in self.rca.results:
				data = str(self.rca.results[metric,loc.GetName()])

			loc.GetController().AddData(metric,data)
			
	       	message='Selected results added to location metadata table as "' + metric + '".'
	       	print '\n'+message+'\n\n'
	       	wx.MessageBox(message) 


	def onSaveFile( self, event ):
		try:
			self.rca.SaveResults(self.filePicker.GetPath())
		except AttributeError:
			wx.MessageBox("Please 'Run' this plugin before attempting to save the results!")
			return

        def OnLabelClick(self, event):
                #disable the ability to select the column using the label
                pass
        
	def OnSelectedRange( self, event ):
                colL = event.GetLeftCol()
                colR = event.GetRightCol()
                if colL != colR:
                        self.table.ClearSelection()
                        
              # for i in range(self.table.GetNumberCols()):
                 #  self.table.DeselectCol(i) 
		
        def OnSelectedCell( self, event ):
		"""Internal update to the selection tracking list"""
		self.selectedCols = [ event.GetCol() ]
		event.Skip()
