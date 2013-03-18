# GenGIS Makefile
PROGRAM=GenGIS.app

# Set debug/release variables:
GENGISBUILDTYPE=RELEASE
GENGISDEBUGFLAG=

ifeq ($(MAKECMDGOALS),debug)
	GENGISBUILDTYPE=DEBUG
	GENGISDEBUGFLAG=-ggdb -D _DEBUG
endif

OUTPUT_PATH = ./BUILD/$(GENGISBUILDTYPE)/GenGIS

# Export the build type and g++ debug flags, for use by recursive calls to make.
export GENGISBUILDTYPE
export GENGISDEBUGFLAG

# Set the path to third-party libraries
LIBRARY3RD_PATH = ../../../i386/library3rd

# Library versions
BOOST_VERSION = boost_1_47
WX_PYTHON_VERSION = 2.8

# Paths to include directories
BOOST_INCLUDE   =-I$(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/include/
WX_INCLUDE      =-I$(LIBRARY3RD_PATH)/wxPython/lib/wx/include/mac-unicode-debug-$(WX_PYTHON_VERSION) \
                 -I$(LIBRARY3RD_PATH)/wxPython/include/wx-$(WX_PYTHON_VERSION) -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D_IODBC_ -D__WXDEBUG__ -D__WXMAC__
GDAL_INCLUDE    =-I$(LIBRARY3RD_PATH)/Frameworks/Headers/GDAL/Headers/
GL_INCLUDE      =-I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers
X11_INCLUDE     =-I/usr/X11/include/ -I/usr/X11/include/freetype2/
FTGL_INCLUDE    =-I$(LIBRARY3RD_PATH)/ftgl/include/
PYTHON_INCLUDE  =-I$(LIBRARY3RD_PATH)/Frameworks/Headers/Python/Headers/

ALL_INCLUDE_DIRECTORIES = $(BOOST_INCLUDE) $(WX_INCLUDE) $(GDAL_INCLUDE) $(GL_INCLUDE) $(X11_INCLUDE) $(FTGL_INCLUDE) $(PYTHON_INCLUDE)

# Paths to third-party libraries
PNG             = $(LIBRARY3RD_PATH)/lib/libpng12.0.dylib
GL              = /System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib \
                  /System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGLU.dylib
FTGL            = -L$(LIBRARY3RD_PATH)/ftgl/lib/ -lftgl.2
BOOST           = $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_date_time.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_filesystem.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_graph.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_iostreams.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_prg_exec_monitor.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_program_options.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_regex.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_serialization.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_signals.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_system.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_thread.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_unit_test_framework.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_wave.dylib \
                  $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_wserialization.dylib
BOOST_PYTHON    = $(LIBRARY3RD_PATH)/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/libboost_python.dylib
GDAL            = $(LIBRARY3RD_PATH)/Frameworks/Frameworks/GDAL/unix/lib/libgdal.dylib
WXPYTHON        = -framework QuickTime -framework IOKit -framework Carbon -framework Cocoa -framework System \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_base_carbonud-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_base_carbonud_net-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_base_carbonud_odbc-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_base_carbonud_xml-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_macud_adv-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_macud_aui-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_macud_core-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_macud_dbgrid-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_macud_gizmos-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_macud_gizmos_xrc-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_macud_gl-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_macud_html-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_macud_qa-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_macud_richtext-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_macud_stc-$(WX_PYTHON_VERSION).dylib \
                  $(LIBRARY3RD_PATH)/wxPython/lib/libwx_macud_xrc-$(WX_PYTHON_VERSION).dylib
PYTHON          = $(LIBRARY3RD_PATH)/Frameworks/Frameworks/Python/Python

ALL_LIBRARIES   = $(GL) $(FTGL) $(BOOST) $(GDAL) $(WXPYTHON) $(PYTHON) $(BOOST_PYTHON) $(PNG)

# All C++ source files
SOURCE_FILES := \
	$(wildcard ../../../../src/core/*.cpp)   \
	$(wildcard ../../../../src/gui/*.cpp)    \
	$(wildcard ../../../../src/python/*.cpp) \
	$(wildcard ../../../../src/utils/*.cpp)  \
	$(wildcard ../../../../src/widgets/*.cpp)

# All C++ object files
OBJECT_FILES = \
	$(addprefix BUILD/$(GENGISBUILDTYPE)/OBJECTS/, \
		$(notdir $(patsubst %.cpp, %.o, $(SOURCE_FILES))) \
	)

# Set the compiler
CC = g++

# Set C++ flags
CXXFLAGS = -finput-charset=iso-8859-1 -arch i386 -Wno-deprecated-declarations

# Set Linker options
LDFLAGS = -lstdc++ -ldl -Xlinker -lm

# Declare the following as targets (in case there is also a file of the same name, this avoids confusion).
.PHONY: force debug release clean default

# Make a release build by default.
default: release

# Debug mode
debug:
	@if [ ! -d $(LIBRARY3RD_PATH)/$(BOOST_VERSION) ]; then tar -xzf $(LIBRARY3RD_PATH)/$(BOOST_VERSION).tar.gz -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/Frameworks ];       then tar -xzf $(LIBRARY3RD_PATH)/Frameworks.tar.gz       -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/ftgl ];             then tar -xzf $(LIBRARY3RD_PATH)/ftgl.tar.gz             -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/GL ];               then tar -xzf $(LIBRARY3RD_PATH)/GL.tar.gz               -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/lib ];              then tar -xzf $(LIBRARY3RD_PATH)/lib.tar.gz              -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/site-packages ];    then tar -xzf $(LIBRARY3RD_PATH)/site-packages.tar.gz    -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/wxPython ];         then tar -xzf $(LIBRARY3RD_PATH)/wxPython.tar.gz         -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/X11 ];              then tar -xzf $(LIBRARY3RD_PATH)/X11.tar.gz              -C $(LIBRARY3RD_PATH); fi;
	@mkdir -p BUILD/$(GENGISBUILDTYPE)/OBJECTS
	@make -e GenGIS.app

# Release mode
release:
	@if [ ! -d $(LIBRARY3RD_PATH)/$(BOOST_VERSION) ]; then tar -xzf $(LIBRARY3RD_PATH)/$(BOOST_VERSION).tar.gz -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/Frameworks ];       then tar -xzf $(LIBRARY3RD_PATH)/Frameworks.tar.gz       -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/ftgl ];             then tar -xzf $(LIBRARY3RD_PATH)/ftgl.tar.gz             -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/GL ];               then tar -xzf $(LIBRARY3RD_PATH)/GL.tar.gz               -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/lib ];              then tar -xzf $(LIBRARY3RD_PATH)/lib.tar.gz              -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/site-packages ];    then tar -xzf $(LIBRARY3RD_PATH)/site-packages.tar.gz    -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/wxPython ];         then tar -xzf $(LIBRARY3RD_PATH)/wxPython.tar.gz         -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/X11 ];              then tar -xzf $(LIBRARY3RD_PATH)/X11.tar.gz              -C $(LIBRARY3RD_PATH); fi;
	@mkdir -p BUILD/$(GENGISBUILDTYPE)/OBJECTS
	@make -e GenGIS.app

all: $(PROGRAM)

$(PROGRAM): .depend $(OBJECT_FILES)
	# Creating the '.app'
	@if [ ! -d BUILD/$(GENGISBUILDTYPE)/GenGIS.app ]; then \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/GenGIS.app; \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents; \
	fi;

	@echo 'APPL????' > BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/PkgInfo
	
	# Copying the 'Info.plist' metadata file into GenGIS.app/Contents/
	@cp -f ../../../Info.plist BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/

	# Creating and populating the 'Frameworks' directory
	@if [ ! -d BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Frameworks ]; then \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Frameworks; \
	cp -R $(LIBRARY3RD_PATH)/Frameworks/Frameworks/ BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Frameworks; \
	fi;
	
	# Creating the 'Library' directory
	@if [ ! -d BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Library ]; then \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Library; \
	cp ../../library3rd/$(BOOST_VERSION)/$(GENGISBUILDTYPE)/lib/lib* BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Library/; \
	cp ../../library3rd/ftgl/lib/libftgl.2.dylib BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Library/; \
	cp $(LIBRARY3RD_PATH)/lib/* BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Library/; \
	fi;
	
	# Creating and populating the 'Resources' directory
	@if [ ! -d BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources ]; then \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources; \
	fi;
	cp -f ../../../../bin/images/CrazyEye/mac/{CrazyEye.icns,GenGISDocumentIcon.icns} BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/; \
	cp -f ../../../../bin/{release_notes.html,license.html,authors.html} BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/; \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/wxPython/lib/; \
	cp -R $(LIBRARY3RD_PATH)/wxPython/lib/*.dylib BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/wxPython/lib/; \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/images; \
	cp -R ../../../../bin/images/[^\.]* BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/images/; \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/fonts; \
	cp ../../../../bin/fonts/[^\.]* BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/fonts/; \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/site-packages/; \
	cp ../../../../src/python/PythonCode.py BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/site-packages/; \
	cp -R ../../../../scripts/*.py BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/site-packages/; \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/colourMaps; \
	cp ../../../../bin/colourMaps/[^\.]* BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/colourMaps/; \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/Resources/English.lproj;
	
	# Transferring the build to the 'MacOS' directory
	@if [ ! -d BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/MacOS ]; then \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/MacOS; \
	fi;
	
	# Creating and populating the 'Plugins' directory
	@if [ ! -d BUILD/$(GENGISBUILDTYPE)/Plugins ]; then \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/Plugins; \
	fi;
	cp -Rf ../../../../plugins/ BUILD/$(GENGISBUILDTYPE)/Plugins;
	
	# Creating and populating the 'Library' directory for python plugins
	@if [ ! -d BUILD/$(GENGISBUILDTYPE)/Library ]; then \
	mkdir -p BUILD/$(GENGISBUILDTYPE)/Library; \
	cp -f ../../../../library/*.py BUILD/$(GENGISBUILDTYPE)/Library; \
	fi;

#	# Creating and populating the 'Scripts' directory for python plugins
#	@if [ ! -d BUILD/$(GENGISBUILDTYPE)/Scripts ]; then \
#	mkdir -p BUILD/$(GENGISBUILDTYPE)/Scripts/; \
#	cp -R ../../../../scripts/*.py BUILD/$(GENGISBUILDTYPE)/Scripts/; \
#	fi;

	# Linking GenGIS
	@$(CC) $(CXXFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o BUILD/$(GENGISBUILDTYPE)/GenGIS.app/Contents/MacOS/GenGIS $(GENGISDEBUGFLAG) $(ALL_LIBRARIES)
	
	@echo "\n\nAll done!\n\n"

# Clean the solution
clean:
	@echo "\nCleaning...\n"
	rm -rf .depend BUILD
	@echo "\n...done.\n"

# Generate dependencies
depend: .depend

.depend: cmd = $(CC) -MM -MF depend $(var) $(ALL_INCLUDE_DIRECTORIES); \
	`which echo` -n "BUILD/$(GENGISBUILDTYPE)/OBJECTS/" >> .depend; \
	cat depend >> .depend; \
	echo $(var);
.depend: 
	@if [ ! -d $(LIBRARY3RD_PATH)/$(BOOST_VERSION) ]; then tar -xzf $(LIBRARY3RD_PATH)/$(BOOST_VERSION).tar.gz -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/Frameworks ];       then tar -xzf $(LIBRARY3RD_PATH)/Frameworks.tar.gz       -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/ftgl ];             then tar -xzf $(LIBRARY3RD_PATH)/ftgl.tar.gz             -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/GL ];               then tar -xzf $(LIBRARY3RD_PATH)/GL.tar.gz               -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/lib ];              then tar -xzf $(LIBRARY3RD_PATH)/lib.tar.gz              -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/site-packages ];    then tar -xzf $(LIBRARY3RD_PATH)/site-packages.tar.gz    -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/wxPython ];         then tar -xzf $(LIBRARY3RD_PATH)/wxPython.tar.gz         -C $(LIBRARY3RD_PATH); fi;
	@if [ ! -d $(LIBRARY3RD_PATH)/X11 ];              then tar -xzf $(LIBRARY3RD_PATH)/X11.tar.gz              -C $(LIBRARY3RD_PATH); fi;
	@echo "Generating dependencies..."
	@# Delete old '.depend' in case it is corrupt
	@rm -rf .depend
	@$(foreach var, $(SOURCE_FILES), $(cmd))
	@rm -f depend

-include .depend

# Compile each source file
%.o:
	@$(CC) $(CXXFLAGS) $(GENGISDEBUGFLAG) $(ALL_INCLUDE_DIRECTORIES) -c $< -o $@; \
	echo $(notdir $@)" ... done";

# Build precompiled headers
../../../../src/core/Precompiled.hpp.gch: ../../../../src/core/Precompiled.hpp
	$(CC) $(CXXFLAGS) $(ALL_INCLUDE_DIRECTORIES) -x c++-header -c $< -o $@
