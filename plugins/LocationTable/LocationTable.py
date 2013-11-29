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

from LocationTableLayout import LocationTableLayout

import GenGIS
import wx

class LocationTable( LocationTableLayout ):
	def __init__(self, parent=None):
		LocationTableLayout.__init__ ( self, parent )
		
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico", wx.BITMAP_TYPE_ICO))
		
		# check required data has been loaded
		if GenGIS.layerTree.GetNumLocationSetLayers() == 0:
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
		locationSetLayer = GenGIS.layerTree.GetLocationSetLayer(0)
		fields = sorted(locationSetLayer.GetController().GetMetadataFields())
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
			
		fields = [siteId] + fields
			
		self.table.DeleteCols(0, self.table.GetNumberCols());
		self.table.AppendCols(len(fields))
		for i in xrange(0, len(fields)):
			self.table.SetColLabelValue(i, fields[i])
			
		# set cell values
		self.table.DeleteRows(0, self.table.GetNumberRows());
		self.table.AppendRows(len(locs))
		for i in xrange(0, len(locs)):
			data = locs[i].GetController().GetData()
			for j in xrange(0, len(fields)):
				self.table.SetCellValue(i, j, data[fields[j]])
		
		self.table.AutoSizeColumns()
		self.table.Thaw()
		
		wx.EndBusyCursor()

	def OnClose(self, event):
		event.Skip()
	
	def OnOK( self, event ):
		self.Close()

	def OnHelp( self, event ):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/index.php/Description_of_GenGIS_plugins#Location_Table_Viewer' )
		
	def OnShowAll( self, event ):
		if self.chkShowAll.IsChecked():
			locs = GenGIS.layerTree.GetLocationSetLayer(0).GetAllLocationLayers()
		else:
			locs = GenGIS.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
		self.UpdateTable(locs)
	
