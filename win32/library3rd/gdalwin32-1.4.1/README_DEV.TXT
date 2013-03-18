
GDAL 1.4.1 Development Binaries
===============================

Install
-------

Unzip on top of the gdalwin32exe141 package. 

Add gdalwin32-1.4.1\include to your include path.

For a /MD optimized build link against gdalwin32-1.4.1\lib\gdal_i.lib
  - Uses gdal14.dll

At this time no debug builds are offered. 

Note that package is built using Microsoft VC 7.1.  Other compiler
versions will generally be limited to using the C API, instead of 
the C++ API. 
