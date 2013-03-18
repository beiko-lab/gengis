# -*- coding: utf-8 -*- 

def name():
	return "Canonical Correlation Analysis"
	
def version():
	return "Version 0.9"

def description():
	return "Detects correlated environmental and sequence data using Canonical Correlation Analysis"
	
def authors():
	return "Brett O'Donnell, Michael Porter"

def publicationDate():
	return "July 20, 2012"
	
def minimumVersionOfGenGIS(): 
	return "2.02"

def requireR():
	return True

def rDependencies():
	return "CCA,igraph"