Reference Condition Analysis Plugin For GenGIS
==============================================

This is a plugin for GenGIS (http://kiwi.cs.dal.ca/GenGIS/Main_Page) that does reference condition analysis. 

Installation
------------
1) Install newest release of GenGIS
2) Unzip the RCA.zip file into the "Plugins" directory within the GenGIS program installation.
3) Start up Gengis and load the test files (/test):  map file (CABIN_region.tif), location file (RCA_locs_test.csv) and seqs file (RCA_test_seqs.csv)
4) From Plugins choose the RCA plugin. 
5) The default columns chosen should be fine (e.g. Label for taxonomy names and Count for taxonomy counts. )
6) Click Run and in a few seconds there should be bar graphs on the map. 
7) You can then change the diversity metric selection and click "update plot" to plot the new diversity measure (this will be faster then clicking on "Run" again since it is just getting the precomputed diversity measure). 
