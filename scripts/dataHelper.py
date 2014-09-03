# Example:
#
# import dataHelper
# locData = dataHelper.getLocationSetData()
# dataHelper.printData(locData, 'Temperature')
# filteredData = dataHelper.filterData(locData, 'Temperature', 20, dataHelper.filterFunc.greater)
# dataHelper.printData(filteredData, 'Temperature')

import GenGIS as ggAPI
import filterFunc
		
def filter(dataList, field, filterValue, filterFunc):
	"""
	Filter a list of data objects.
	
	Example: dataHelper.filterData(myData, 'Temperature', 10, dataHelper.filterFunc.greater)
	will return only data objects whose Temperature field is strictly greater than 10.

	@param dataList: list with location set or sequence data objects
	@param field: data field to filter on
	@param filterValue: value to filter on
	@param filterFunc: function to filter on (see filterFunc.py)
	@return: List containing only data objects that pass the filter
	"""
	filteredList = []
	for item in dataList:
		if filterFunc(item.GetController().GetData()[field], filterValue):
			filteredList.append(item)			
	return filteredList
	
def genericFilter(dataList, filterValue, filterFunc):
	filteredList = []
	for item in dataList:
		if filterFunc(item, filterValue):
			filteredList.append(item)
			
	return filteredList

	
def GetNonNullLocations( field ):
	locData = ggAPI.layerTree.GetLocationLayers()
	nonNullLoc = []
	for loc in locData:
		# get vector of all metadata
		data = loc.GetController().GetData()[field]
		# If fields are any of the special values for NULL
		if ( data != "None" and data != "Null" and data != "NA" ):
			nonNullLoc.append(loc)
	return nonNullLoc
		
		
def GetNonNullSequences( field ):
	locData = ggAPI.layerTree.GetSequenceLayers()
	nonNullLoc = []
	for loc in locData:
		# get vector of all metadata
		data = loc.GetController().GetData()[field]
		# If fields are any of the special values for NULL
		if ( data != "None" and data != "Null" and data != "NA" ):
			nonNullLoc.append(loc)
	return nonNullLoc
	
def LocationSetWeeder(  ):
	locs = ggAPI.layerTree.GetLocationSetLayer(0).GetAllActiveLocationLayers()
	switchedLocs = []
	for loc in locs:
		for field in loc.GetController().GetData().keys():
		#	print field," ",loc.GetController().GetData()[field]
			if loc.GetController().GetData()[field] == "None":
				switchedLocs.append(loc)
				loc.GetController().SetActive(False)
	return switchedLocs
	
def LocationSetRestore( locations ):
	for loc in locations:
		loc.GetController().SetActive(True)
	
def isNumber(str):
	try:
		float(str)
		return True
	except ValueError:
		return False
		

		