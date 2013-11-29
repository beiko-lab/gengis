#=======================================================================
# Author: Donovan Parks
#
# Copyright 2009 Donovan Parks
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

from SequenceTableLayout import SequenceTableLayout

import GenGIS
import wx

class SequenceTable( SequenceTableLayout ):
	def __init__(self, parent=None):
		SequenceTableLayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumSequenceLayers() == 0:
			wx.MessageBox("This plugin requires location data to be loaded.", "Additional data required.")
			self.Close()
			return
			
		locs = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
		self.UpdateTable(locs)

	def UpdateTable(self, locs):
		wx.BeginBusyCursor()
		
		# setup table
		self.table.Freeze()
		self.table.SetSelectionMode(wx.grid.Grid.wxGridSelectRows)
		self.table.EnableScrolling(True, True)
		self.table.SetRowLabelSize(0)

		# set field (column) labels
		seqLayer = GenGIS.layerTree.GetSequenceLayer(0)
		fields = sorted(seqLayer.GetController().GetData().keys())
		if fields.count('sequenceid') != 0:
			fields.remove('sequenceid')
			seqId = 'sequenceid'
		elif fields.count('SequenceId') != 0:
			fields.remove('SequenceId') 
			seqId = 'SequenceId'
		elif fields.count('sequence id') != 0:
			fields.remove('sequence id')
			seqId = 'sequence id'
		elif fields.count('Sequence Id') != 0:
			fields.remove('Sequence Id')
			seqId = 'Sequence Id'
		elif fields.count('Sequence ID') != 0:
			fields.remove('Sequence ID')
			seqId = 'Sequence ID'
			
		if fields.count('siteid') != 0:
			fields.remove('siteid')
			siteId = 'siteid'
		elif fields.count('SiteId') != 0:
			fields.remove('SiteId') 
			siteId = 'SiteId'
		elif fields.count('site id') != 0:
			fields.remove('site id')
			siteId = 'site id'
		elif fields.count('Site Id') != 0:
			fields.remove('Site Id')
			siteId = 'Site Id'
		elif fields.count('Site ID') != 0:
			fields.remove('Site ID')
			siteId = 'Site ID'
			
		fields = [seqId, siteId] + fields
			
		self.table.DeleteCols(0, self.table.GetNumberCols());
		self.table.AppendCols(len(fields))
		for i in xrange(0, len(fields)):
			self.table.SetColLabelValue(i, fields[i])
			
		# set cell values
		self.table.DeleteRows(0, self.table.GetNumberRows())
		rowIndex = 0
		for loc in locs:
			if self.chkShowAllSeq.IsChecked():
				seqLayers = loc.GetAllSequenceLayers()
			else:
				seqLayers = loc.GetAllActiveSequenceLayers()
				
			self.table.AppendRows(len(seqLayers))
			for j in xrange(0, len(seqLayers)):
				data = seqLayers[j].GetController().GetData()
		
				for k in xrange(0, len(fields)):
					self.table.SetCellValue(rowIndex, k, data[fields[k]])
				rowIndex += 1
		
		self.table.AutoSizeColumns()
		self.table.Thaw()
		
		wx.EndBusyCursor()
		
	def OnClose(self, event):
		event.Skip()
		
	def OnOK( self, event ):
		self.Close()
	
	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/index.php/Description_of_GenGIS_plugins#Sequence_Table_Viewer' )
		
	def OnShowAllLoc( self, event ):
		if self.chkShowAllLoc.IsChecked():
			locs = GenGIS.layerTree.GetLocationSetLayer(0).GetAllLocationLayers()
		else:
			locs = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
		self.UpdateTable(locs)
		
	def OnShowAllSeq( self, event ):
		self.OnShowAllLoc(event)
