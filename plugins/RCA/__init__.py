def name():
	return "Reference Condition Analysis"
	
def version():
	return "Version 1.0"

def description():
	return "Perform a Reference Condition Analysis (RCA)."
	
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

# Specifies the destination plugin menu (plugins or data) within GenGIS
def pluginMenu():
	return "plugins"
