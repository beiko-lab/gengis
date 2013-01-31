def name():
	return "Reference Condition Approach"
	
def version():
	return "Version 1.0"

def description():
	return "Perform a Reference Condition Approach (RCA) analysis."
	
def authors():
	return "Morgan Langille and Rob Beiko"

def publicationDate():
	return "November 23, 2012"
	
def minimumVersionOfGenGIS(): 
	return "2.0"

def requireR():
	return True

# R dependency list: separate using commas and no white-spaces
# (e.g., "ade4,somethingElse")
def rDependencies():
	return "vegan"