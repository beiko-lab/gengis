NAME:

	checkForUpdates.pl

DESCRIPTION:

	This script scans the GenGIS source code and reports any functions that are not
	included in PythonInterface.cpp.  In other words, the results indicate which C++
	functions are not exposed to the Python GenGIS interpreter and GenGIS plugins.
	Results are stored in new file called 'results.txt'.

HOW TO USE (Windows only):

	1 Make sure a Perl interpreter is installed
	  (e.g., ActivePerl - http://www.activestate.com/activeperl)
	2 Double click 'checkForUpdates.pl' to generate 'results.txt'
	3 In results.txt, functions whose names are prepended with a '?'
	  are missing from PythonInterface.cpp.

OTHER STUFF:

	This script makes use of "Exuberant CTags" (http://ctags.sourceforge.net/)
