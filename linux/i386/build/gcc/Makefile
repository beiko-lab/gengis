#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#
#   GenGIS Makefile   #
#_____________________#

# Set debug/release variables:

GENGISBUILDTYPE=RELEASE
GENGISDEBUGFLAG=

ifeq ($(MAKECMDGOALS),debug)
  GENGISBUILDTYPE=DEBUG
  GENGISDEBUGFLAG=-ggdb
endif

# Export the build type and g++ -finput-charset=iso-8859-1 debug flags, for use by recursive calls to make.

export GENGISBUILDTYPE
export GENGISDEBUGFLAG

# Make a release build by default.
default: release

# Declare the following as targets (in case there is also a file of the same name, this avoids confusion).
.PHONY: force debug release clean

# Any target that uses "force" as a dependancy is forced to execute; i.e. make will never say the target is "up-to-date".
force: ;

#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#
#   Include directories   #
#_________________________#

#BOOST_INCLUDE=-I../../../universal/library3rd/boost-1_35/include/

#WX_INCLUDE=-I/usr/include/wx-2.8/ -I/usr/include/wx-2.8/wxPython/ -I/usr/lib/wx/include/gtk2-unicode-release-2.8/ -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXDEBUG__

WX_INCLUDE=`wx-config --cxxflags`

###`wx-config --libs`

GDAL_INCLUDE=-I/usr/include/gdal/

GL_INCLUDE=-I/usr/include/GL/

#X11_INCLUDE=-I/usr/X11/include/ -I/usr/X11/include/freetype2/

FTGL_INCLUDE=-I/usr/include/freetype2/ -I/usr/include/FTGL/

PYTHON_INCLUDE=-I/usr/include/python2.6/

ALL_INCLUDE_DIRECTORIES=$(PYTHON_INCLUDE) $(WX_INCLUDE) $(GDAL_INCLUDE) $(FTGL_INCLUDE) $(GL_INCLUDE)

###ALL_INCLUDE_DIRECTORIES=$(BOOST_INCLUDE) $(WX_INCLUDE) $(GDAL_INCLUDE) $(GL_INCLUDE) $(X11_INCLUDE) $(FTGL_INCLUDE) $(PYTHON_INCLUDE)

#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#
#   Libraries   #
#_______________#

GL=/usr/lib/libGL.so /usr/lib/libGLU.so
FTGL=/usr/lib/libftgl.so
BOOST=/usr/lib/libboost_date_time-mt.so /usr/lib/libboost_filesystem-mt.so /usr/lib/libboost_graph-mt.so /usr/lib/libboost_iostreams-mt.so /usr/lib/libboost_prg_exec_monitor-mt.so /usr/lib/libboost_program_options-mt.so /usr/lib/libboost_regex-mt.so /usr/lib/libboost_wserialization-mt.so /usr/lib/libboost_signals-mt.so /usr/lib/libboost_system-mt.so /usr/lib/libboost_thread-mt.so /usr/lib/libboost_unit_test_framework-mt.so /usr/lib/libboost_wave-mt.so
GDAL=/usr/lib/libgdal1.6.0.so

#WXPYTHON=-framework QuickTime -framework IOKit -framework Carbon -framework Cocoa -framework System ../../../universal/library3rd/wxPython-unicode-2.8.9.2/lib/libwx_macud-2.8.dylib ../../../universal/library3rd/wxPython-unicode-2.8.9.2/lib/libwx_macud_gl-2.8.dylib ../../../universal/library3rd/wxPython-unicode-2.8.9.2/lib/libwx_macud_gizmos-2.8.dylib ../../../universal/library3rd/wxPython-unicode-2.8.9.2/lib/libwx_macud_gizmos_xrc-2.8.dylib ../../../universal/library3rd/wxPython-unicode-2.8.9.2/lib/libwx_macud_stc-2.8.dylib

#WXPYTHON=/usr/lib/libwx_baseud_net-2.8.so /usr/lib/libwx_gtk2ud_richtext-2.8.so /usr/lib/libwx_baseud-2.8.so /usr/lib/libwx_gtk2ud_gl-2.8.so /usr/lib/libwx_gtk2ud_gizmos-2.8.so /usr/lib/libwx_gtk2ud_gizmos_xrc-2.8.so /usr/lib/libwx_gtk2ud_stc-2.8.so

WXPYTHON=`wx-config --libs` -lwx_gtk2u_gl-2.8

PYTHON=/usr/lib/libpython2.6.so
BOOST_PYTHON=/usr/lib/libboost_python-mt-py26.so

ALL_LIBRARIES=$(GL) $(FTGL) $(BOOST) $(GDAL) $(WXPYTHON) $(PYTHON) $(BOOST_PYTHON)

#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#
#   Object files   #
#__________________#

ALL_OBJECT_PATHS=\
BUILD/$(GENGISBUILDTYPE)/OBJECTS/Camera.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ChartSetView.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ChartView.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/Colour.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ColourMap.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/CompassView2D.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ConfigureDatabaseDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSelectSampleDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ConfigureDatabaseLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSaveResultsDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSaveResultsLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSelectAttributesDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSelectAttributesLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSelectSampleLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSequenceSummaryDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSequenceSummaryLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/DemReader.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/DropLines.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/Font.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/GenGisApp.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/GenGisFrame.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/GenGisLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoTreeIO.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoTreeEllipticalLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoTreeController.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoTreeLinearLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/Geometry.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/GraphVizIO.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayerTree.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayerTreeController.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayerTreeView.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayoutEllipse.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayoutLine.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayoutObjectPropertiesDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayoutObjectPropertiesLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/Lighting.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LightingPropertiesDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LightingPropertiesLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationLayer.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationPropertiesDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationPropertiesLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationSetIO.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationSetLayer.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationSetPropertiesDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationSetPropertiesLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationView.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/MapLayer.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/MapPropertiesDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/MapPropertiesLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/MiniMapView2D.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/NavView2D.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/NewickIO.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/Node.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/Point3D.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/Precompiled.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/PythonInterface.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/SequenceIO.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/SequenceLayer.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/SequencePropertiesDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/SequencePropertiesLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/StringTools.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/StudyLayer.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/StudyModel.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/StudyPropertiesDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/StudyPropertiesLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/TerrainController.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/TerrainModel.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/TerrainTexture.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/TerrainView.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/Texture.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/TreeLayer.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/TreePropertiesDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/TreePropertiesLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ViewWidget.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/Viewport.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/WidgetController.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/NodeGeoTree.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoAxisPolyline.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoTreeOptLeafOrder.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoTreeView.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationSetController.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ShapeMapWidget.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualPolyLine.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ShapeMap.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ShapeMapManager.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ColourMapManager.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualLabel.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/TerrainMapWidget.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualMarker.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ColourMapWidget.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ColourMapDiscrete.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualLine.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/glUtils.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualObject.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LabelToolDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LabelToolLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LabelController.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/ColourPickerDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/UserElements.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/App.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/API.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/SequenceController.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/SeqSelectionDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/SeqSelectionLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/Log.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/tr.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/FontPropertiesDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/FontPropertiesLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/BarChartView.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/PieChartView.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/SaveImageDlg.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/SaveImageLayout.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualPoints.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualLines.o \
BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayoutObjectProperties.o 


# Plugin stuff, ignore for now
#BUILD/$(GENGISBUILDTYPE)/OBJECTS/PluginManager.o \
#BUILD/$(GENGISBUILDTYPE)/OBJECTS/SharedLibraryUnix.o

#‾‾‾‾‾‾‾‾‾‾‾#
#   DEBUG   #
#___________#

debug: force
	mkdir -p BUILD/$(GENGISBUILDTYPE)/OBJECTS
	make -e GenGIS

#‾‾‾‾‾‾‾‾‾‾‾‾‾#
#   RELEASE   #
#_____________#

release: force
	mkdir -p BUILD/$(GENGISBUILDTYPE)/OBJECTS
	make -e GenGIS

#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#
#   GenGIS program   #
#____________________#

GenGIS: objects
	
	#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#
	#   Remove the old .app   #
	#_________________________#
	
	rm -rf BUILD/$(GENGISBUILDTYPE)/GenGIS
	
	#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#
	#   Link GenGIS   #
	#_________________#
	
	g++ -finput-charset=iso-8859-1 -lstdc++ -ldl -o BUILD/$(GENGISBUILDTYPE)/GenGIS $(GENGISDEBUGFLAG) -Xlinker -lm $(ALL_OBJECT_PATHS) $(ALL_LIBRARIES)
	
	@echo "\n\nAll done!\n\n"

#‾‾‾‾‾‾‾‾‾‾‾#
#   Clean   #
#___________#

clean:
	@echo "\nCleaning...\n"
	rm -rf BUILD
	@echo "\n...done.\n"

#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#
#   Object files   #
#__________________#

objects: $(ALL_OBJECT_PATHS)

#BUILD/$(GENGISBUILDTYPE)/OBJECTS/EnglishTextPlugin.so
# ~mikep. plugins are far from done; please ignore this for now.

#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#
#   Code files   #
#________________#

BUILD/$(GENGISBUILDTYPE)/OBJECTS/Camera.o: ../../../../src/core/Camera.cpp ../../../../src/core/Camera.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ChartSetView.o: ../../../../src/core/ChartSetView.cpp ../../../../src/core/ChartSetView.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ChartView.o: ../../../../src/core/ChartView.cpp ../../../../src/core/ChartView.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/Colour.o: ../../../../src/utils/Colour.cpp  ../../../../src/utils/Colour.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ColourMap.o: ../../../../src/utils/ColourMap.cpp ../../../../src/utils/ColourMap.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/CompassView2D.o: ../../../../src/widgets/CompassView2D.cpp ../../../../src/widgets/CompassView2D.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ConfigureDatabaseDlg.o: ../../../../src/gui/ConfigureDatabaseDlg.cpp ../../../../src/gui/ConfigureDatabaseDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSelectSampleDlg.o: ../../../../src/gui/DbSelectSampleDlg.cpp ../../../../src/gui/DbSelectSampleDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/DemReader.o: ../../../../src/utils/DemReader.cpp ../../../../src/utils/DemReader.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/DropLines.o: ../../../../src/core/DropLines.cpp ../../../../src/core/DropLines.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/Font.o: ../../../../src/utils/Font.cpp ../../../../src/utils/Font.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/GenGisApp.o: ../../../../src/gui/GenGisApp.cpp ../../../../src/gui/GenGisApp.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/GenGisFrame.o: ../../../../src/gui/GenGisFrame.cpp ../../../../src/gui/GenGisFrame.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/GenGisLayout.o: ../../../../src/gui/GenGisLayout.cpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoTreeIO.o: ../../../../src/core/GeoTreeIO.cpp ../../../../src/core/GeoTreeIO.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoTreeView.o: ../../../../src/core/GeoTreeView.cpp ../../../../src/core/GeoTreeView.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoTreeEllipticalLayout.o: ../../../../src/core/GeoTreeEllipticalLayout.cpp ../../../../src/core/GeoTreeEllipticalLayout.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoTreeController.o: ../../../../src/core/GeoTreeController.cpp ../../../../src/core/GeoTreeController.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoTreeLinearLayout.o: ../../../../src/core/GeoTreeLinearLayout.cpp ../../../../src/core/GeoTreeLinearLayout.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/Geometry.o: ../../../../src/utils/Geometry.cpp ../../../../src/utils/Geometry.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/GraphVizIO.o: ../../../../src/utils/GraphVizIO.cpp ../../../../src/utils/GraphVizIO.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayerTree.o: ../../../../src/core/LayerTree.cpp ../../../../src/core/LayerTree.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayerTreeController.o: ../../../../src/core/LayerTreeController.cpp ../../../../src/core/LayerTreeController.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayerTreeView.o: ../../../../src/core/LayerTreeView.cpp ../../../../src/core/LayerTreeView.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayoutEllipse.o: ../../../../src/core/LayoutEllipse.cpp ../../../../src/core/LayoutEllipse.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayoutLine.o: ../../../../src/core/LayoutLine.cpp ../../../../src/core/LayoutLine.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayoutObjectPropertiesDlg.o: ../../../../src/gui/LayoutObjectPropertiesDlg.cpp ../../../../src/gui/LayoutObjectPropertiesDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayoutObjectPropertiesLayout.o: ../../../../src/gui/LayoutObjectPropertiesLayout.cpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/Lighting.o: ../../../../src/core/Lighting.cpp ../../../../src/core/Lighting.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LightingPropertiesDlg.o: ../../../../src/gui/LightingPropertiesDlg.cpp ../../../../src/gui/LightingPropertiesDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LightingPropertiesLayout.o: ../../../../src/gui/LightingPropertiesLayout.cpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationLayer.o: ../../../../src/core/LocationLayer.cpp ../../../../src/core/LocationLayer.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationPropertiesDlg.o: ../../../../src/gui/LocationPropertiesDlg.cpp ../../../../src/gui/LocationPropertiesDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationPropertiesLayout.o: ../../../../src/gui/LocationPropertiesLayout.cpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationSetIO.o: ../../../../src/core/LocationSetIO.cpp ../../../../src/core/LocationSetIO.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationSetLayer.o: ../../../../src/core/LocationSetLayer.cpp ../../../../src/core/LocationSetLayer.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationSetPropertiesDlg.o: ../../../../src/gui/LocationSetPropertiesDlg.cpp ../../../../src/gui/LocationSetPropertiesDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationSetPropertiesLayout.o: ../../../../src/gui/LocationSetPropertiesLayout.cpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationView.o: ../../../../src/core/LocationView.cpp ../../../../src/core/LocationView.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/MapLayer.o: ../../../../src/core/MapLayer.cpp ../../../../src/core/MapLayer.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/MapPropertiesDlg.o: ../../../../src/gui/MapPropertiesDlg.cpp ../../../../src/gui/MapPropertiesDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/MapPropertiesLayout.o: ../../../../src/gui/MapPropertiesLayout.cpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/MiniMapView2D.o: ../../../../src/widgets/MiniMapView2D.cpp ../../../../src/widgets/MiniMapView2D.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/NavView2D.o: ../../../../src/widgets/NavView2D.cpp ../../../../src/widgets/NavView2D.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/NewickIO.o: ../../../../src/utils/NewickIO.cpp ../../../../src/utils/NewickIO.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/Node.o: ../../../../src/utils/Node.cpp ../../../../src/utils/Node.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/Point3D.o: ../../../../src/utils/Point3D.cpp ../../../../src/utils/Point3D.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/Precompiled.o: ../../../../src/core/Precompiled.cpp ../../../../src/core/Precompiled.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/PythonInterface.o: ../../../../src/python/PythonInterface.cpp ../../../../src/python/PythonInterface.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/SequenceIO.o: ../../../../src/core/SequenceIO.cpp ../../../../src/core/SequenceIO.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/SequenceLayer.o: ../../../../src/core/SequenceLayer.cpp ../../../../src/core/SequenceLayer.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/SequencePropertiesDlg.o: ../../../../src/gui/SequencePropertiesDlg.cpp ../../../../src/gui/SequencePropertiesDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/SequencePropertiesLayout.o: ../../../../src/gui/SequencePropertiesLayout.cpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/StringTools.o: ../../../../src/utils/StringTools.cpp ../../../../src/utils/StringTools.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/StudyLayer.o: ../../../../src/core/StudyLayer.cpp ../../../../src/core/StudyLayer.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/StudyModel.o: ../../../../src/core/StudyModel.cpp ../../../../src/core/StudyModel.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/StudyPropertiesDlg.o: ../../../../src/gui/StudyPropertiesDlg.cpp ../../../../src/gui/StudyPropertiesDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/StudyPropertiesLayout.o: ../../../../src/gui/StudyPropertiesLayout.cpp ../../../../src/gui/StudyPropertiesLayout.h
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/TerrainController.o: ../../../../src/core/TerrainController.cpp ../../../../src/core/TerrainController.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/TerrainModel.o: ../../../../src/core/TerrainModel.cpp ../../../../src/core/TerrainModel.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/TerrainTexture.o: ../../../../src/core/TerrainTexture.cpp ../../../../src/core/TerrainTexture.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/TerrainView.o: ../../../../src/core/TerrainView.cpp ../../../../src/core/TerrainView.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/Texture.o: ../../../../src/widgets/Texture.cpp ../../../../src/widgets/Texture.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/TreeLayer.o: ../../../../src/core/TreeLayer.cpp ../../../../src/core/TreeLayer.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/TreePropertiesDlg.o: ../../../../src/gui/TreePropertiesDlg.cpp ../../../../src/gui/TreePropertiesDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/TreePropertiesLayout.o: ../../../../src/gui/TreePropertiesLayout.cpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ViewWidget.o: ../../../../src/widgets/ViewWidget.cpp ../../../../src/widgets/ViewWidget.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/Viewport.o: ../../../../src/core/Viewport.cpp ../../../../src/core/Viewport.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/WidgetController.o: ../../../../src/widgets/WidgetController.cpp ../../../../src/widgets/WidgetController.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/NodeGeoTree.o: ../../../../src/core/NodeGeoTree.cpp ../../../../src/core/NodeGeoTree.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualPrimatives.o: ../../../../src/core/VisualPrimatives.cpp ../../../../src/core/VisualPrimatives.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoAxisPolyline.o: ../../../../src/core/GeoAxisPolyline.cpp ../../../../src/core/GeoAxisPolyline.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/GeoTreeOptLeafOrder.o: ../../../../src/core/GeoTreeOptLeafOrder.cpp ../../../../src/core/GeoTreeOptLeafOrder.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LocationSetController.o: ../../../../src/core/LocationSetController.cpp ../../../../src/core/LocationSetController.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ShapeMapWidget.o: ../../../../src/gui/ShapeMapWidget.cpp ../../../../src/gui/ShapeMapWidget.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualPolyLine.o: ../../../../src/core/VisualPolyLine.cpp ../../../../src/core/VisualPolyLine.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ShapeMap.o: ../../../../src/utils/ShapeMap.cpp ../../../../src/utils/ShapeMap.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ShapeMapManager.o: ../../../../src/utils/ShapeMapManager.cpp ../../../../src/utils/ShapeMapManager.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ColourMapManager.o: ../../../../src/utils/ColourMapManager.cpp ../../../../src/utils/ColourMapManager.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualLabel.o: ../../../../src/core/VisualLabel.cpp ../../../../src/core/VisualLabel.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/TerrainMapWidget.o: ../../../../src/gui/TerrainMapWidget.cpp ../../../../src/gui/TerrainMapWidget.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ColourMapDiscrete.o: ../../../../src/utils/ColourMapDiscrete.cpp ../../../../src/utils/ColourMapDiscrete.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualMarker.o: ../../../../src/core/VisualMarker.cpp ../../../../src/core/VisualMarker.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ColourMapWidget.o: ../../../../src/gui/ColourMapWidget.cpp ../../../../src/gui/ColourMapWidget.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualLine.o: ../../../../src/core/VisualLine.cpp ../../../../src/core/VisualLine.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/glUtils.o: ../../../../src/utils/glUtils.cpp ../../../../src/utils/glUtils.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualObject.o: ../../../../src/core/VisualObject.cpp ../../../../src/core/VisualObject.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LabelToolDlg.o: ../../../../src/gui/LabelToolDlg.cpp ../../../../src/gui/LabelToolDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LabelToolLayout.o: ../../../../src/gui/LabelToolLayout.cpp ../../../../src/gui/LabelToolLayout.h
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ColourPickerDlg.o: ../../../../src/gui/ColourPickerDlg.cpp ../../../../src/gui/ColourPickerDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LabelController.o: ../../../../src/core/LabelController.cpp ../../../../src/core/LabelController.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/UserElements.o: ../../../../src/core/UserElements.cpp ../../../../src/core/UserElements.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/App.o: ../../../../src/core/App.cpp ../../../../src/core/App.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/API.o: ../../../../src/core/API.cpp ../../../../src/core/API.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/SequenceController.o: ../../../../src/core/SequenceController.cpp ../../../../src/core/SequenceController.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/SeqSelectionDlg.o: ../../../../src/gui/SeqSelectionDlg.cpp ../../../../src/gui/SeqSelectionDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/SeqSelectionLayout.o: ../../../../src/gui/SeqSelectionLayout.cpp ../../../../src/gui/SeqSelectionLayout.h
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/Log.o: ../../../../src/utils/Log.cpp ../../../../src/utils/Log.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/ConfigureDatabaseLayout.o: ../../../../src/gui/ConfigureDatabaseLayout.cpp ../../../../src/gui/ConfigureDatabaseLayout.h
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSaveResultsDlg.o: ../../../../src/gui/DbSaveResultsDlg.cpp ../../../../src/gui/DbSaveResultsDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSaveResultsLayout.o: ../../../../src/gui/DbSaveResultsLayout.cpp ../../../../src/gui/DbSaveResultsLayout.h
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSelectAttributesDlg.o: ../../../../src/gui/DbSelectAttributesDlg.cpp ../../../../src/gui/DbSelectAttributesDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSelectAttributesLayout.o: ../../../../src/gui/DbSelectAttributesLayout.cpp ../../../../src/gui/DbSelectAttributesLayout.h
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSelectSampleLayout.o: ../../../../src/gui/DbSelectSampleLayout.cpp ../../../../src/gui/DbSelectSampleLayout.h
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSequenceSummaryDlg.o: ../../../../src/gui/DbSequenceSummaryDlg.cpp ../../../../src/gui/DbSequenceSummaryDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/DbSequenceSummaryLayout.o: ../../../../src/gui/DbSequenceSummaryLayout.cpp ../../../../src/gui/DbSequenceSummaryLayout.h
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/tr.o: ../../../../src/utils/tr.cpp ../../../../src/utils/tr.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/FontPropertiesLayout.o: ../../../../src/gui/FontPropertiesLayout.cpp ../../../../src/gui/FontPropertiesLayout.h
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/FontPropertiesDlg.o: ../../../../src/gui/FontPropertiesDlg.cpp ../../../../src/gui/FontPropertiesDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/BarChartView.o: ../../../../src/core/BarChartView.cpp ../../../../src/core/BarChartView.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/PieChartView.o: ../../../../src/core/PieChartView.cpp ../../../../src/core/PieChartView.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/SaveImageDlg.o: ../../../../src/gui/SaveImageDlg.cpp ../../../../src/gui/SaveImageDlg.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/SaveImageLayout.o: ../../../../src/gui/SaveImageLayout.cpp ../../../../src/gui/SaveImageLayout.h
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualPoints.o: ../../../../src/core/VisualPoints.cpp ../../../../src/core/VisualPoints.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/VisualLines.o: ../../../../src/core/VisualLines.cpp ../../../../src/core/VisualLines.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."

BUILD/$(GENGISBUILDTYPE)/OBJECTS/LayoutObjectProperties.o: ../../../../src/core/LayoutObjectProperties.cpp ../../../../src/core/LayoutObjectProperties.hpp
	@echo "Compiling $@..."
	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
	@echo "...done."



## Plugin stuff
#
#BUILD/$(GENGISBUILDTYPE)/OBJECTS/PluginManager.o: ../../../../src/core/PluginManager.cpp ../../../../src/core/PluginManager.hpp
#	@echo "Compiling $@..."
#	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -o $@ $<
#	@echo "...done."
#
#BUILD/$(GENGISBUILDTYPE)/OBJECTS/SharedLibraryUnix.o: ../../../../src/core/SharedLibraryUnix.cpp
#	@echo "Compiling $@..."
#	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) -c $(GENGISDEBUGFLAG) -lstdc++ -ldl -o $@ $<
#	@echo "...done."
#
#BUILD/$(GENGISBUILDTYPE)/OBJECTS/EnglishTextPlugin.so: ../../../../src/core/EnglishTextPlugin.cxx
#	@echo "Compiling $@..."
#	g++ -finput-charset=iso-8859-1 $(ALL_INCLUDE_DIRECTORIES) $(GENGISDEBUGFLAG) -dynamiclib $< -o $@
#	@echo "...done."
