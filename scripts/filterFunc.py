# Useful functions for filtering data.
# Used within dataHelper.py

def greater(val1, val2):
	return str(val1) > str(val2)
	
def greaterEqual(val1, val2):
	return str(val1) >= str(val2)

def less(val1, val2):
	return str(val1) < str(val2)
	
def lessEqual(val1, val2):
	return str(val1) <= str(val2)
	
def equal(val1, val2):
	return str(val1) == str(val2)
	
def notEqual(val1, val2):
	return str(val1) != str(val2)

def lessDate(date1, date2):
	return date1 < date2
	
def lessEqualDate(date1, date2):
	return date1 <= date2
	
def equalDate(date1, date2):
	return date1 == date2
	
def notEqualDate(date1, date2):
	return date1 != date2
	
def greaterEqualDate(date1, date2):
	return date1 >= date2
	
def greaterDate(date1, date2):
	return date1 > date2