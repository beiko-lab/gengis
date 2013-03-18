#!/bin/perl
#use File::Basename;

##################################################################################
# Filename    : generateWikiPage.pl                                              #
# Author      : Timothy Mankowski                                                #
# Date        : July 11, 2012                                                    #
# Description : This script scans PythonInterface.cpp and generates wiki syntax  #
#               for the http://kiwi.cs.dal.ca/GenGIS/GenGIS_API page.            #
##################################################################################

# Open the Python interface file
open( PYTHON_INTERFACE_FILE, "<../../src/python/PythonInterface.cpp" );

open( WIKI_OUPUT_FILE, ">wiki.txt" );

# Extract function names from PythonInterface.cpp
while( <PYTHON_INTERFACE_FILE> )
{
	chomp( $_ );

	# Match variables
	if ( ( $_ =~ /def_readwrite(.*)"(.*)"(.*)"(.*)"/ ) )
	{
		print WIKI_OUPUT_FILE "* $2 - $4\n";
	}
	# Match functions
	elsif ( ( $_ =~ /def(.*)"(.*)"(.*)"(.*)"/ ) )
	{
		print WIKI_OUPUT_FILE "* $2() - $4\n";
	}
	
	# Match global objects
	if ( ( $_ =~ /scope\(\).attr(.*)"(.*)"(.*)"(.*)"/ ) )
	{
		unless ( $2 eq "__doc__" )
		{
			print WIKI_OUPUT_FILE "* $2 - $4\n";
		}
	}

	# Look for the <wiki-header> tag which specifies a header name
	if ( $_ =~ /<wiki-header>(.*)<\/wiki-header>/ ) #if ( $_ =~ /<wiki-header>(.+?)<\/wiki-header>/ )
	{
		print WIKI_OUPUT_FILE "\n= $1 =\n\n";
	}
}

close( PYTHON_INTERFACE_FILE );
close( WIKI_OUPUT_FILE );

print "SUCCESS: Results saved in \'wiki.txt\'\n";
print "Press any key to continue...";
<>;