def name():
	return "Mantel Test"
	
def version():
	return "Version 1.1"

def description():
	return "Perform Mantel analysis."
	
def authors():
	return "Kathryn Dunphy, Rob Beiko, Donovan Parks"

def publicationDate():
	return "May 8, 2012"
	
def minimumVersionOfGenGIS(): 
	return "2.0"

def requireR():
	return True

# R dependency list: separate using commas and no white-spaces
# (e.g., "ade4,somethingElse")
def rDependencies():
	return "ade4"