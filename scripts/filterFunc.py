# Useful functions for filtering data.
# Used within dataHelper.py
import dateutil.parser

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
	date1 = dateutil.parser.parse(date1)
	date2 = dateutil.parser.parse(date2)
	return date1 < date2
	
def lessEqualDate(date1, date2):
	date1 = dateutil.parser.parse(date1)
	date2 = dateutil.parser.parse(date2)
	return date1 <= date2
	
def equalDate(date1, date2):
	date1 = dateutil.parser.parse(date1)
	date2 = dateutil.parser.parse(date2)
	return date1 == date2
	
def notEqualDate(date1, date2):
	date1 = dateutil.parser.parse(date1)
	date2 = dateutil.parser.parse(date2)
	return date1 != date2
	
def greaterEqualDate(date1, date2):
	date1 = dateutil.parser.parse(date1)
	date2 = dateutil.parser.parse(date2)
	return date1 >= date2
	
def greaterDate(date1, date2):
	date1 = dateutil.parser.parse(date1)
	date2 = dateutil.parser.parse(date2)
	return date1 > date2
	
def greaterFloat(val1, val2):
	return float(val1) > float(val2)
	
def greaterEqualFloat(val1, val2):
	if float(val1) > float(val2):
		return True
	else:
		return abs( float(val1) - float(val2) ) <= 0.00000001

def lessFloat(val1, val2):
	return float(val1) < float(val2)
	
def lessEqualFloat(val1, val2):
	if float(val1) < float(val2):
		return True
	else:
		return abs( float(val1) - float(val2) )  <= 0.00000001
	
def equalFloat(val1, val2):
	return abs( float(val1) - float(val2) )  <= 0.00000001
	
def notEqualFloat(val1, val2):
	return abs( float(val1) - float(val2) )  > 0.00000001