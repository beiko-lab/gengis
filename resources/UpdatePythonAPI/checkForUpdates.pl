#!/bin/perl
use File::Basename;

##################################################################################
# Filename    : checkForUpdates.pl                                               #
# Author      : Timothy Mankowski                                                #
# Date        : July 11, 2012                                                    #
# Description : This script scans the GenGIS source code and reports any         #
#               functions that are not included in PythonInterface.              #
# Other:      : This script uses Exuberant CTags (http://ctags.sourceforge.net/) #
##################################################################################

# Globals used by this script
use constant EXISTS_IN_HEADER_ONLY => 0;
use constant EXISTS_IN_BOTH        => 1;

###################################################
# Extract function names from from header files   #
###################################################

# Collect the names of all header files and store them in a string
my $filelist;

print "Do you have Cygwin installed? [y/n, default: n]: ";
my $answer = <>;
chomp( $answer );

if ( lc( $answer ) eq "y" )
{
	# Use this code if Cygwin is installed
	# Cygwin installs its own version of the 'dir' command, which uses a different syntax
	# from the native Windows version.
	$filelist = `dir ../../src/core/*.h?? ../../src/gui/*.h* ../../src/utils/*.h* ../../src/widgets/*.h*`;
}
else # Use the native Windows version of the 'dir' command
{
	$filelist = `dir /b /s ..\\..\\src\\core\\*.h?? ..\\..\\src\\gui\\*.h?? ..\\..\\src\\utils\\*.h?? ..\\..\\src\\widgets\\*.h??`;
}

$filelist =~ s/\n/ /g;

# Parse each header file using 'ctags' and store the raw output into an array
my @string = `ctags -x --c-kinds=f+p $filelist`;

# Define a hash table for storing names of functions
my %functions;

# Copy the function names from the ctags output and store
# them as key values in a hash table
foreach ( @string )
{
	my @tokens = split( " ", $_ );
	if ( $tokens[0] ne "operator" )
	{
		$tokens[3] = basename( $tokens[3], ".hpp" ); # Strip the folder path and filename extension
		$tokens[3] = basename( $tokens[3], ".h" );
		$functions{"$tokens[3]::$tokens[0]"} = EXISTS_IN_HEADER_ONLY;
	}
}

###################################################
# Extract function names from PythonInterface.cpp #
###################################################

# Open the Python interface file
open( PYTHON_INTERFACE_FILE, "<../../src/python/PythonInterface.cpp" );

while( <PYTHON_INTERFACE_FILE> )
{
	# Split string at spaces and remove any trailing spaces
	my @tokens = split( " ", $_ );

	if ( $tokens[0] =~ /^.def/ )
	{
		# Remove the ampersand and comma
		$tokens[1] =~ s/&|,//g;

		if ( exists $functions{$tokens[1]} )
		{
			$functions{$tokens[1]} = EXISTS_IN_BOTH;
		}
	}
}

close( PYTHON_INTERFACE_FILE );

####################################################
# Output results to 'results.txt'                  #
####################################################

# Open a file to which results can be outputted
open( RESULTS_OUPUT_FILE, ">results.txt" );

print RESULTS_OUPUT_FILE "ALL C++ FUNCTIONS IN GENGIS\n" .
                         "? = Not present in PythonInterface.cpp\n" .
                         "--------------------------------------\n";

# Output date when last comparison was made
my ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime;
$year += 1900;
$mon  += 1;
my $datetime = sprintf "%04d-%02d-%02d %02d:%02d:%02d", $year, $mon, $mday, $hour, $min, $sec;
print RESULTS_OUPUT_FILE "Generated: $datetime\n\n";

my $lastClassName = "";
foreach $key (sort keys %functions)
{
	# Print a new line if the current class is differs from the previous
	unless ( $key =~ m/^$lastClassName(::)/ ) { print RESULTS_OUPUT_FILE "\n"; }

	if ( $functions{$key} == EXISTS_IN_HEADER_ONLY )
	{
		print RESULTS_OUPUT_FILE "? $key\n";
	}
	else
	{
		print RESULTS_OUPUT_FILE "  $key\n";
	}
	$lastClassName = $key;
	$lastClassName =~ s/::.*//;
}

close( RESULTS_OUPUT_FILE );

print "SUCCESS: Results saved in \'results.txt\'\n";
print "Press any key to continue...";
<>;
