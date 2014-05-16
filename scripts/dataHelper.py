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
	
def genericFilter(dataList, field, filterValue, filterFunc):
	filteredList = []
	for item in dataList:
		if filterFunc(item, filterValue):
			filteredList.append(item)
			
	return filteredList

def isNumber(str):
	try:
		float(str)
		return True
	except ValueError:
		return False
		