//=======================================================================
// Author: Mike Porter
//
// Copyright 2009 Mike Porter
//
// This file is part of GenGIS.
//
// GenGIS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GenGIS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GenGIS.  If not, see <http://www.gnu.org/licenses/>.
//=======================================================================

#include "../core/Precompiled.hpp"

// Don't use this code in Windows debug builds!
#ifndef _DEBUG

#include "PythonInterface.hpp"

#include "../core/App.hpp"
#include "../core/Camera.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/LocationSetLayer.hpp"
#include "../core/MapLayer.hpp"
#include "../core/VectorMapLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/StudyLayer.hpp"
#include "../core/VisualLabel.hpp"
#include "../core/VisualMarker.hpp"
#include "../core/TreeLayer.hpp"
#include "../core/GeoTreeView.hpp"
#include "../core/NodeGeoTree.hpp"
#include "../core/UserElementsWrapper.hpp"
#include "../core/LayoutLine.hpp"
#include "../core/Lighting.hpp"
#include "../core/Viewport.hpp"
#include "../core/ViewportWrapper.hpp"
#include "../core/MapController.hpp"
#include "../core/VectorMapController.hpp"
#include "../core/VectorMapView.hpp"
#include "../core/VectorMapModel.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/SequenceController.hpp"
#include "../core/LocationController.hpp"
#include "../core/LocationSetController.hpp"
#include "../core/GeoTreeOptLeafOrder.hpp"

#include "../gui/GenGisFrame.hpp"
#include "../gui/MainWindowWrapper.hpp"

#include "../utils/Colour.hpp"
#include "../utils/ColourMap.hpp"
#include "../utils/ColourMapDiscrete.hpp"
#include "../utils/ColourMapContinuous.hpp"
#include "../utils/ColourMapManager.hpp"
#include "../utils/Point3D.hpp"
#include "../utils/Tree.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/Log.hpp"

using namespace boost::python;

//==============================================================================================
//   Custom-rolled C++ std::map to Python dictionary wrapper.
//==============================================================================================
template<class Key, class Val> class map_item
{
public:

	static Val& get(std::map<Key,Val> &self, const Key idx)
	{
		if(self.find(idx) != self.end())
		{
			return self[idx];
		}
		PyErr_SetString(PyExc_KeyError,"Map key not found");
		throw_error_already_set();
		return self[idx];
	}

	static void set(std::map<Key,Val> &self, const Key idx, const Val val)
	{
		self[idx]=val;
	}

	static void del(std::map<Key,Val> &self, const Key n)
	{
		self.erase(n);
	}

	static bool in(std::map<Key,Val> const &self, const Key n)
	{
		return self.find(n) != self.end();
	}  

	static boost::python::list keys(std::map<Key,Val> const &self)
	{
		list t;
		typename std::map<Key,Val>::const_iterator it;
		for(it=self.begin(); it!=self.end(); ++it)
		{
			t.append(it->first);
		}
		return t;
	}

	static list values(std::map<Key,Val> const &self)
	{
		list t;
		typename std::map<Key,Val>::const_iterator it;
		for(it=self.begin(); it!=self.end(); ++it)
		{
			t.append(it->second);
		}
		return t;
	}

	static list items(std::map<Key,Val> const &self)
	{
		list t;
		typename std::map<Key,Val>::const_iterator it;
		for(it=self.begin(); it!=self.end(); ++it)
		{
			t.append(make_tuple(it->first, it->second));
		}
		return t;
	}

	static std::wstring str(std::map<Key,Val> const &self)
	{
		std::wstring trailingComma = L"";
		std::wstring dictionaryString = L"{";
		typename std::map<Key,Val>::const_iterator it;
		for(it=self.begin(); it!=self.end(); ++it)
		{
			dictionaryString += trailingComma;
			dictionaryString += it->first;
			dictionaryString += L": ";
			dictionaryString += it->second;
			trailingComma = L", ";
		}
		dictionaryString += L"}";
		return dictionaryString;
	}
};

template<class T>
struct StdVecToPythonList
{
	static PyObject* convert(const std::vector<T>& vec)
	{
		boost::python::list* l = new boost::python::list();
		for(size_t i = 0; i < vec.size(); i++)
			(*l).append(vec[i]);

		return l->ptr();
	}
};

template<typename containedType>
struct CustomVectorFomSeq{
	CustomVectorFomSeq(){ converter::registry::push_back(&convertible,&construct,type_id<std::vector<containedType> >()); }
	static void* convertible(PyObject* obj_ptr){
		// the second condition is important, for some reason otherwise there were attempted conversions of Body to list which failed afterwards.
		if(!PySequence_Check(obj_ptr) || !PyObject_HasAttrString(obj_ptr,"__len__")) return 0;
		return obj_ptr;
	}
	static void construct(PyObject* obj_ptr, converter::rvalue_from_python_stage1_data* data){
		void* storage=((converter::rvalue_from_python_storage<std::vector<containedType> >*)(data))->storage.bytes;
		new (storage) std::vector<containedType>();
		std::vector<containedType>* v=(std::vector<containedType>*)(storage);
		int l=PySequence_Size(obj_ptr); if(l<0) abort(); /*std::cerr<<"l="<<l<<"; "<<typeid(containedType).name()<<std::endl;*/ v->reserve(l); for(int i=0; i<l; i++) { v->push_back(extract<containedType>(PySequence_GetItem(obj_ptr,i))); }
		data->convertible=storage;
	}
};

using namespace GenGIS;

//====================================================================================================
//   Thin wrappers (mainly for methods I want to add to one class but are methods of another class)
//====================================================================================================

// Global functions
void SafeYield() { ((GenGisFrame*)GenGIS::App::Inst().GetMainWindow())->SafeYield(); }

//===============================
//   GenGIS API Python module
//===============================
BOOST_PYTHON_MODULE(GenGIS)
{
	// This function is called whenever a new session is loaded in order to properly set 
	// pointers to commonly used global objects (see end of this function).
	static bool bInitialized = false;
	if(!bInitialized)
	{
		bInitialized = true;

		//docstring_options doc_options;
		//doc_options.disable_cpp_signatures();	// the C++ signature indicates the required data types so is often useful

		scope().attr("__doc__") = "GenGIS v2.0.2 API";

		//=================================================================
		// Enumerated types
		//=================================================================

		enum_<VisualMarker::MARKER_SHAPE>("MARKER_SHAPE")
			.value("CIRCLE", VisualMarker::CIRCLE)
			.value("CIRCLE_FAST", VisualMarker::CIRCLE_FAST)
			.value("SQUARE", VisualMarker::SQUARE)
			.value("TRIANGLE", VisualMarker::TRIANGLE)
			.value("STAR", VisualMarker::STAR)
			.value("PLUS_SIGN", VisualMarker::PLUS_SIGN)
			.value("OCTAGON", VisualMarker::OCTAGON)
			.value("DIAMOND", VisualMarker::DIAMOND)
			.value("SQUARE", VisualMarker::SQUARE)
			.value("INVERTED_TRIANGLE", VisualMarker::INVERTED_TRIANGLE)
			.export_values()
			;

		enum_<VisualMarker::MARKER_RENDERING_STYLE>("MARKER_RENDERING_STYLE")
			.value("DECAL", VisualMarker::DECAL)
			.value("ALWAYS_VISIBLE", VisualMarker::ALWAYS_VISIBLE)
			.value("MARKER_3D", VisualMarker::MARKER_3D)
			.value("SCREEN_POS", VisualMarker::SCREEN_POS)
			.export_values()
			;

		enum_<VisualLabel::LABEL_RENDERING_STYLE>("LABEL_RENDERING_STYLE")
			.value("PERSPECTIVE", VisualLabel::PERSPECTIVE)
			.value("ORTHO", VisualLabel::ORTHO)
			.export_values()
			;

		enum_<VisualLine::LINE_STYLE>("LINE_STYLE")
			.value("SOLID", VisualLine::SOLID)
			.value("SHORT_DASH", VisualLine::SHORT_DASH)
			.value("LONG_DASH", VisualLine::LONG_DASH)
			.export_values()
			;

		enum_<ChartView::CHART_TYPE>("CHART_TYPE")
			.value("PIE_CHART_2D", ChartView::PIE_CHART_2D)
			.value("PIE_CHART_3D", ChartView::PIE_CHART_3D)
			.export_values()
			;

		enum_<Layer::LAYER_TYPE>("LAYER_TYPE")
			.value("STUDY", Layer::STUDY)
			.value("MAP", Layer::MAP)
			.value("VECTOR_MAP", Layer::VECTOR_MAP)
			.value("LOCATION_SET", Layer::LOCATION_SET)
			.value("TREE", Layer::TREE)
			.value("LOCATION", Layer::LOCATION)
			.value("SEQUENCE", Layer::SEQUENCE)
			.export_values()
			;

		enum_<GeoTreeView::LAYOUT>("LAYOUT")
			.value("NONE", GeoTreeView::NONE)
			.value("CLADOGRAM_3D", GeoTreeView::CLADOGRAM_3D)
			.value("SLANTED_CLADOGRAM_3D", GeoTreeView::SLANTED_CLADOGRAM_3D)
			.value("SLANTED_PHYLOGRAM_3D", GeoTreeView::SLANTED_PHYLOGRAM_3D)
			.value("CLADOGRAM_2D", GeoTreeView::CLADOGRAM_2D)
			.value("SLANTED_CLADOGRAM_2D", GeoTreeView::SLANTED_CLADOGRAM_2D)
			.value("PHYLOGRAM_2D", GeoTreeView::PHYLOGRAM_2D)
			.export_values()
			;

		enum_<ChartSetView::LAYOUT_CHART>("LAYOUT_CHART")
			.value("NONE", ChartSetView::NONE)
			.value("LAYOUT_LINE_EVEN_SPACING", ChartSetView::LAYOUT_LINE_EVEN_SPACING)
			.value("LAYOUT_LINE_PROJECTED", ChartSetView::LAYOUT_LINE_PROJECTED)
			.export_values()
			;

		enum_<GeoTreeView::ORIENTATION>("ORIENTATION")
			.value("HORIZONTAL", GeoTreeView::HORIZONTAL)
			.value("VERTICAL", GeoTreeView::VERTICAL)
			.export_values()
			;

		enum_<GeoTreeView::TREE_COLOUR_STYLE>("TREE_COLOUR_STYLE")
			.value("COLOUR_SINGLE", GeoTreeView::COLOUR_SINGLE)
			.value("COLOUR_DISCRETE", GeoTreeView::COLOUR_DISCRETE)
			.value("COLOUR_CONTINUOUS", GeoTreeView::COLOUR_CONTINUOUS)
			.export_values()
			;


		enum_<ColourMap::COLOUR_MAP_TYPE>("COLOUR_MAP_TYPE")
			.value("UNKNOWN", ColourMap::UNKNOWN)
			.value("CONTINUOUS", ColourMap::CONTINUOUS)
			.value("DISCRETE", ColourMap::DISCRETE)
			.value("DISCRETIZED_CONTINUOUS", ColourMap::DISCRETIZED_CONTINUOUS)
			.export_values()
			;

		//=================================================================
		// Global functions
		//=================================================================

		// <wiki-header>Global functions</wiki-header>
		def("SafeYield", SafeYield, "Yields control to pending messages in the windowing system.");


		//=================================================================
		// STL classes
		//=================================================================

		// converters from std::vector<X> to Python list
		to_python_converter<std::vector<MapLayerPtr, std::allocator<MapLayerPtr> >, StdVecToPythonList<MapLayerPtr> >();
		to_python_converter<std::vector<VectorMapLayerPtr, std::allocator<VectorMapLayerPtr> >, StdVecToPythonList<VectorMapLayerPtr> >();
		to_python_converter<std::vector<LocationSetLayerPtr, std::allocator<LocationSetLayerPtr> >, StdVecToPythonList<LocationSetLayerPtr> >();
		to_python_converter<std::vector<LocationLayerPtr, std::allocator<LocationLayerPtr> >, StdVecToPythonList<LocationLayerPtr> >();
		to_python_converter<std::vector<SequenceLayerPtr, std::allocator<SequenceLayerPtr> >, StdVecToPythonList<SequenceLayerPtr> >();
		to_python_converter<std::vector<TreeLayerPtr, std::allocator<TreeLayerPtr> >, StdVecToPythonList<TreeLayerPtr> >();
		to_python_converter<std::vector<std::wstring, std::allocator<std::wstring> >, StdVecToPythonList<std::wstring> >();
		to_python_converter<std::vector<float, std::allocator<float> >, StdVecToPythonList<float> >();
		to_python_converter<std::vector<LinearResults, std::allocator<LinearResults> >, StdVecToPythonList<LinearResults> >();

		// converters from Python list to std::vector<X>
		CustomVectorFomSeq<std::wstring>();  

		// converters for std::map<X, Y> 
		class_<std::map<std::wstring,std::wstring> >("StringMap")
			.def("__len__", &std::map<std::wstring,std::wstring>::size)
			.def("__getitem__", &map_item<std::wstring,std::wstring>().get, return_value_policy<copy_non_const_reference>() )
			.def("__setitem__", &map_item<std::wstring,std::wstring>().set)
			.def("__delitem__", &map_item<std::wstring,std::wstring>().del)
			.def("__str__", &map_item<std::wstring,std::wstring>().str)
			.def("__repr__", &map_item<std::wstring,std::wstring>().str)
			.def("clear", &std::map<std::wstring,std::wstring>::clear)
			.def("__contains__", &map_item<std::wstring,std::wstring>().in)
			.def("has_key", &map_item<std::wstring,std::wstring>().in)
			.def("keys", &map_item<std::wstring,std::wstring>().keys)
			.def("values", &map_item<std::wstring,std::wstring>().values)
			.def("items", &map_item<std::wstring,std::wstring>().items)
			;

		class_<std::map<uint,uint> >("UnsignedIntMap")
			.def("__len__", &std::map<uint,uint>::size)
			.def("__getitem__", &map_item<uint,uint>().get, return_value_policy<copy_non_const_reference>() )
			.def("__setitem__", &map_item<uint,uint>().set)
			.def("__delitem__", &map_item<uint,uint>().del)
			.def("__str__", &map_item<uint,uint>().str)
			.def("__repr__", &map_item<uint,uint>().str)
			.def("clear", &std::map<uint,uint>::clear)
			.def("__contains__", &map_item<uint,uint>().in)
			.def("has_key", &map_item<uint,uint>().in)
			.def("keys", &map_item<uint,uint>().keys)
			.def("values", &map_item<uint,uint>().values)
			.def("items", &map_item<uint,uint>().items)
			;

		//=================================================================
		// GenGIS classes
		//=================================================================

		// <wiki-header>Main Window class</wiki-header>
		class_<MainWindowWrapper, boost::noncopyable, MainWindowWrapperPtr >("MainWindow", "Main window of GenGIS interface.", no_init)
			.def("GetPointer", &MainWindowWrapper::GetPointer, return_value_policy<reference_existing_object>(), "Get pointer to GenGIS window.")
			.def("SetSidePanelVisibility", &MainWindowWrapper::SetSidePanelVisibility, "Set visibility of side panel.")
			.def("SetConsoleVisibility", &MainWindowWrapper::SetConsoleVisibility, "Set visibility of console.")
			.def("OpenMapFile", &MainWindowWrapper::OpenMapFile, "Open map file.")
			.def("OpenVectorMapFile", &MainWindowWrapper::OpenVectorMapFile, "Open vector map file.")
			.def("OpenLocationsFile", &MainWindowWrapper::OpenLocationsFile, "Open location file.")
			.def("OpenLocationsCSVFile", &MainWindowWrapper::OpenLocationsCSVFile, "Open location CSV file.")
			.def("OpenSequenceFile", &MainWindowWrapper::OpenSequenceFile, "Open sequence file.")
			.def("OpenSequenceCSVFile", &MainWindowWrapper::OpenSequenceCSVFile, "Open sequence CSV file.")
			.def("OpenTreeFile", &MainWindowWrapper::OpenTreeFile, "Open tree file.")
			.def("GetExeDir", &MainWindowWrapper::GetExeDir, "Get directory of GenGIS executable.")
			.def("Quit", &MainWindowWrapper::Quit, "Exit GenGIS.")
			.def("Write", &MainWindowWrapper::Write, "Write text to the GenGIS console and the log file.")
			.def("Warning", &MainWindowWrapper::Warning, "Write warning message to the GenGIS console and the log file.")
			.def("Error", &MainWindowWrapper::Error, "Write error message to the GenGIS console and the log file.")
			.def("Yield", &MainWindowWrapper::Yield, "Yield control to main window for event processing.")
			;

		// <wiki-header>Graphics class</wiki-header>
		class_<UserElementsWrapper, boost::noncopyable, UserElementsWrapperPtr >("UserElements", "Provides access to user generated graphical elements.", no_init)
			.def("AddLine", &UserElementsWrapper::AddLine, "Add line to viewport.")
			.def("RemoveLine", &UserElementsWrapper::RemoveLine, "Remove line with a given id.")
			.def("ClearLines", &UserElementsWrapper::ClearLines, "Clear all custom lines in viewport.")
			.def("AddMarker", &UserElementsWrapper::AddMarker, "Add marker to Viewport.")
			.def("RemoveMarker", &UserElementsWrapper::RemoveMarker, "Remove marker with a given id.")
			.def("ClearMarkers", &UserElementsWrapper::ClearMarkers, "Clear all custom markers in viewport.")
			.def("AddCylinder", &UserElementsWrapper::AddCylinder, "Add cylinder to viewport.")
			.def("RemoveCylinder", &UserElementsWrapper::RemoveCylinder, "Remove cylinder with a given id.")
			.def("ClearCylinders", &UserElementsWrapper::ClearCylinders, "Clear all custom cylinders in viewport.")
			.def("AddLabel", &UserElementsWrapper::AddLabel, "Add label to viewport.")
			.def("RemoveLabel", &UserElementsWrapper::RemoveLabel, "Remove label with a given id.")
			.def("ClearLabels", &UserElementsWrapper::ClearLabels, "Clear all custom labels in Viewport.")
			; 

		// <wiki-header>Camera class</wiki-header>
		class_<Camera, boost::noncopyable, CameraPtr >("Camera", "Control properties for visualizing the viewport contents.", no_init)
			.def("SetPosition", &Camera::SetPosition, "Set position of camera.")
			.def("GetPosition", &Camera::GetPosition, "Get current position of camera.")
			.def("SetHeight", &Camera::SetHeight, "Set height of camera above terrain.")
			.def("GetHeight", &Camera::GetHeight, "Get height of camera above terrain.")
			.def("SetPitch", &Camera::SetPitch, "Set pitch of camera.")
			.def("GetPitch", &Camera::GetPitch, "Get pitch of camera.")
			.def("SetYaw", &Camera::SetYaw, "Set yaw of camera.")
			.def("GetYaw", &Camera::GetYaw, "Get yaw of camera.")
			.def("ZoomDelta", &Camera::ZoomDelta, "Change zoom (height) of camera by specified percentage.")
			.def("TranslateDelta", &Camera::TranslateDelta, "Translate position of camera by specified amount.")
			.def("PitchDelta", &Camera::PitchDelta, "Change pitch of camera by specified amount.")
			.def("YawDelta", &Camera::YawDelta, "Change yaw of camera by specified amount.")
			.def("ResetView", &Camera::ResetView, "Set camera to default position.")
			.def("TopView", &Camera::TopView, "Set camera to provide a top 'orthographic' view of the terrain")
			.def("GetMapExtentsZoom", &Camera::GetMapExtentsZoom, "Get zoom (height) of camera required to view entire map.")
			.def("GetWindowHeight", &Camera::GetWindowHeight, "Get height of the viewport.")
			.def("GetWindowWidth", &Camera::GetWindowWidth, "Get width of the viewport.")    
			;

		// <wiki-header>Map Controller class</wiki-header>
		class_<MapController, boost::noncopyable, MapControllerPtr >("MapController", "Control properties of the map model.", no_init)
			.def("GetProjectionExtents", &MapController::GetProjectionExtents, "Get lower, left and upper, right corners of map after projection.")
			.def("GetWidth", &MapController::GetWidth, "Get width of terrain.")
			.def("GetHeight", &MapController::GetHeight, "Get height of terrain.")
			.def("IsWireFrame", &MapController::IsWireFrame, "Determine if terrain is being rendered as a wireframe.")
			.def("SetWireFrame", &MapController::SetWireFrame, "Set whether terrain should be rendered as a wireframe (True) or with texturing (False).")
			.def("GetElevation", &MapController::GetElevation, "Get elevation at specified grid point.")
			.def("GetExaggeratedElevation", &MapController::GetExaggeratedElevation, "Get exaggerated elevation at specified grid point.")
			.def("GetElevationMap", &MapController::GetElevationMap, "Flag indicating if terrain contains elevation information.")
			.def("GetMinElevation", &MapController::GetMinElevationGridSpace, "Get minimum elevation.")
			.def("GetMaxElevation", &MapController::GetMaxElevationGridSpace, "Get maximum elevation.")
			.def("ProjectToTerrain", &MapController::ProjectToTerrain, "Project screen coodinates onto terrain. ")
			.def("ProjectToScreen", &MapController::ProjectToScreen, "Project 3D world coordinates to screen coordinates.")
			.def("GeoToGrid", &MapController::GeoToGrid, "Convert geographic coordinates to grid coordinates.")
			.def("LatLongToGrid", &MapController::LatLongToGrid, "Convert lat/long coordinates to grid coordinates.")
			.def("GridToGeo", &MapController::GridToGeo, "Convert grid coordinates to geographic coordinates.")
			.def("GetMapBorders", &MapController::GetMapBorders, "Get borders of map.")
			;

		// <wiki-header>Layer Tree Controller class</wiki-header>
		class_<LayerTreeController, boost::noncopyable, LayerTreeControllerPtr >("LayerTreeController", "Provides access to all layers within GenGIS.", no_init)
			.def("GetName", &LayerTreeController::GetName, "Get name of specified layer.")
			.def("GetDescription", &LayerTreeController::GetDescription, "Get description of specified layer.")
			.def("SetDescription", &LayerTreeController::SetDescription, "Set description of specified layer.")
			.def("GetAuthours", &LayerTreeController::GetAuthours, "Get authours of specified layer.")
			.def("SetAuthours", &LayerTreeController::SetAuthours, "Set authours of specified layer.")
			.def("GetPath", &LayerTreeController::GetPath, "Get path to file of specified layer.")
			.def("GetFilename", &LayerTreeController::GetFilename, "Get filename of specified layer.")
			.def("SetVisible", &LayerTreeController::SetVisible, "Set visibility of specified layer.")
			.def("IsVisible", &LayerTreeController::IsVisible, "Get visibility of specified layer.")
			.def("SetVisible", &LayerTreeController::SetVisible, "Set visibility of specified layer.")
			.def("GetNumStudyLayers", &LayerTreeController::GetNumStudyLayers, "Get number of study layers.")
			.def("GetStudyLayer", &LayerTreeController::GetStudyLayer, "Get specified study layer.")
			.def("GetNumMapLayers", &LayerTreeController::GetNumMapLayers, "Get number of map layers.")
			.def("GetMapLayer", &LayerTreeController::GetMapLayer, "Get specified map layer.")
			.def("GetNumVectorMapLayers", &LayerTreeController::GetNumVectorMapLayers, "Get number of vector map layers.")
			.def("GetVectorMapLayer", &LayerTreeController::GetVectorMapLayer, "Get specified vector map layer.")
			.def("GetNumLocationSetLayers", &LayerTreeController::GetNumLocationSetLayers, "Get number of location set layers.")
			.def("GetLocationSetLayer", &LayerTreeController::GetLocationSetLayer, "Get specified location set layer.")
			.def("GetNumLocationLayers", &LayerTreeController::GetNumLocationLayers, "Get number of location layers.")
			.def("GetLocationLayer", &LayerTreeController::GetLocationLayer, "Get specified location layer.")
			.def("GetLocationLayers", &LayerTreeController::GetLocationLayers, "Get all location layers.")
			.def("GetNumSequenceLayers", &LayerTreeController::GetNumSequenceLayers, "Get number of sequence layers.")
			.def("GetSequenceLayer", &LayerTreeController::GetSequenceLayer, "Get specified sequence layer.")
			.def("GetSequenceLayers", &LayerTreeController::GetSequenceLayers, "Get all sequence layers.")
			.def("GetNumTreeLayers", &LayerTreeController::GetNumTreeLayers, "Get number of tree layers.")
			.def("GetTreeLayer", &LayerTreeController::GetTreeLayer, "Get specified tree layer.")
			;

		// <wiki-header>Viewport class</wiki-header>
		class_<ViewportWrapper, boost::noncopyable, ViewportWrapperPtr >("Viewport", "Control properties of the viewport.", no_init)
			.def("Refresh", &ViewportWrapper::Refresh, "Regenerate all graphical elements within the viewport.")
			.def("SaveImage", &ViewportWrapper::SaveImage, "Save image of viewport.")
			.def("GetWidth", &ViewportWrapper::GetWidth, "Width of viewport.")
			.def("GetHeight", &ViewportWrapper::GetHeight, "Height of viewport.")		
			.def("SetBackgroundColour", &ViewportWrapper::SetBackgroundColour, "Set background colour of viewport.")
			.def("GetBackgroundColour", &ViewportWrapper::GetBackgroundColour, "Get background colour of viewport.")
			.def("GetLighting", &ViewportWrapper::GetLighting, "Get object controlling lighting in viewport.")
			;

		// <wiki-header>Lighting class</wiki-header>
		class_<Lighting, boost::noncopyable, LightingPtr >("Lighting", "Control lighting properties of viewport.", no_init)
			.def("GetAltitude", &Lighting::GetAltitude, "Get altitude (y-position) of light.")
			.def("SetAltitude", &Lighting::SetAltitude, "Set altitude (y-position) of light.")
			.def("SetGlobalAmbient", &Lighting::SetGlobalAmbient, "Set strength of global ambient light.")
			.def("GetGlobalAmbient", &Lighting::GetGlobalAmbient, "Get strength of global ambient light.")		
			.def("GetPositionX", &Lighting::GetPositionX, "Get x-position of light.")
			.def("SetPositionX", &Lighting::SetPositionX, "Set x-position of light.")
			.def("GetPositionY", &Lighting::GetPositionY, "Get y-position of light.")
			.def("SetPositionY", &Lighting::SetPositionY, "Set y-position of light.")
			.def("GetPositionZ", &Lighting::GetPositionZ, "Get z-position of light.")
			.def("SetPositionZ", &Lighting::SetPositionZ, "Set z-position of light.")
			;

		// <wiki-header>GeoTreeView class</wiki-header>
		class_<GeoTreeView, boost::noncopyable, GeoTreeViewPtr >("GeoTreeView", "Provides access to geophylogeny properties and functionality.", no_init)
			.def("ProjectTree", &GeoTreeView::ProjectTree, "Project tree to specified leaf nodes.")
			.def("RestoreTree", &GeoTreeView::RestoreTree, "Restore original tree.")
			.def("SetLayout", &GeoTreeView::SetLayout, "Set layout of geophylogeny.")
			.def("GetLayout", &GeoTreeView::GetLayout, "Get layout of geophylogeny.")	
			.def("GetLayoutLineVisibility", &GeoTreeView::GetLayoutLineVisibility, "Get visibility of geographic layout line.")
			.def("SetLayoutLineVisibility", &GeoTreeView::SetLayoutLineVisibility, "Set visibility of geographic layout line.")
			.def("GetLayoutLine", &GeoTreeView::GetLayoutLine, "Get geographic layout line.")
			.def("SetLayoutLine", &GeoTreeView::SetLayoutLine, "Set geographic layout line.")
			.def("IsLayoutLine", &GeoTreeView::IsLayoutLine, "Check if a layout line has been assigned to the tree.")
			.def("IsGeographicAxis", &GeoTreeView::IsGeographicAxis, "Check if a non-linear geographic axis has been assigned to the tree.")
			.def("GetGeographicAxisVisibility", &GeoTreeView::GetGeographicAxisVisibility, "Get visibility of non-linear geographic axis.")
			.def("SetGeographicAxisVisibility", &GeoTreeView::SetGeographicAxisVisibility, "Set visibility of non-linear geographic axis.")
			.def("GetLeafNodeSize", &GeoTreeView::GetLeafNodeSize, "Get size of leaf nodes.")
			.def("SetLeafNodeSize", &GeoTreeView::SetLeafNodeSize, "Set size of leaf nodes.")
			.def("GetInternalNodeSize", &GeoTreeView::GetInternalNodeSize, "Get size of internal nodes.")
			.def("SetInternalNodeSize", &GeoTreeView::SetInternalNodeSize, "Set size of internal nodes.")
			.def("GetLineThickness", &GeoTreeView::GetLineThickness, "Get thickness of tree branches.")
			.def("SetLineThickness", &GeoTreeView::SetLineThickness, "Set thickness of tree branches.")
			.def("GetOrientation", &GeoTreeView::GetOrientation, "Get orientation of tree.")
			.def("SetOrientation", &GeoTreeView::SetOrientation, "Set orientation of tree.")
			.def("GetHeight", &GeoTreeView::GetHeight, "Get height of tree.")
			.def("SetHeight", &GeoTreeView::SetHeight, "Set height of tree.")
			.def("GetColour", &GeoTreeView::GetColour, "Get default colour for tree.")
			.def("SetColour", &GeoTreeView::SetColour, "Set default colour for tree.")
			.def("GetOptimizeTopology", &GeoTreeView::GetOptimizeTopology, "Check if optimal tree layout is being calcualte.")
			.def("SetOptimizeTopology", &GeoTreeView::SetOptimizeTopology, "Set flag indicating if optimal tree layout should be calculate.")
			.def("GetLabelVisibility", &GeoTreeView::GetLabelVisibility, "Get visibility of leaf labels.")
			.def("SetLabelVisibility", &GeoTreeView::SetLabelVisibility, "Set visibility of leaf labels.")
			.def("GetFontSize", &GeoTreeView::GetFontSize, "Get font size for leaf labels.")
			.def("SetFontSize", &GeoTreeView::SetFontSize, "Set font size for leaf labels.")
			.def("GetFontColour", &GeoTreeView::GetFontColour, "Get font colour for leaf labels.")
			.def("SetFontColour", &GeoTreeView::SetFontColour, "Set font colour for leaf labels.")
			.def("Set3dInternalDropLineStyle", &GeoTreeView::Set3dInternalDropLineStyle, "Set internal drop line style for 3D tree.")
			.def("Get3dInternalDropLineStyle", &GeoTreeView::Get3dInternalDropLineStyle, "Get internal drop line style for 3D tree.")
			.def("Set3dLeafDropLineStyle", &GeoTreeView::Set3dLeafDropLineStyle, "Set leaf drop line style for 3D trees.")
			.def("Get3dLeafDropLineStyle", &GeoTreeView::Get3dLeafDropLineStyle, "Get leaf drop line style for 3D tree.")
			.def("SetGeographyLineOffsetPercentage", &GeoTreeView::SetGeographyLineOffsetPercentage, "Set offset of geography line (in percentage of layout line length).")
			.def("GetGeographyLineOffsetPercentage", &GeoTreeView::GetGeographyLineOffsetPercentage, "Get offset of geography line (in percentage of layout line length).")
			.def("GetGeographyLineOffset", &GeoTreeView::GetGeographyLineOffset, "Get offset of geography line (in world coordinates)." )
			.def("SetSpreadGeographyPts", &GeoTreeView::SetSpreadGeographyPts, "Set flag indicating if locations should be spread out evenly along the geography line.")
			.def("SetLeafNodeBorderSize", &GeoTreeView::SetLeafNodeBorderSize, "Set border size for leaf nodes.")
			.def("GetLeafNodeBorderSize", &GeoTreeView::GetLeafNodeBorderSize, "Get border size for leaf nodes.")
			.def("SetInternalNodeBorderSize", &GeoTreeView::SetInternalNodeBorderSize, "Set border size for internal nodes.")
			.def("GetInternalNodeBorderSize", &GeoTreeView::GetInternalNodeBorderSize, "Get border size for internal nodes.")
			.def("SetGeoPtsBorderSize", &GeoTreeView::SetGeoPtsBorderSize, "Set border size for geographic points along geography line.")
			.def("GetGeoPtsBorderSize", &GeoTreeView::GetGeoPtsBorderSize, "Get border size for geographic points along geography line.")
			.def("SetBranchBorderSize", &GeoTreeView::SetBranchBorderSize, "Set border size for tree branches.")
			.def("GetBranchBorderSize", &GeoTreeView::GetBranchBorderSize, "Get border size for tree branches.")
			.def("SetLocationLineBorderSize", &GeoTreeView::SetLocationLineBorderSize, "Set border size for location lines.")
			.def("GetLocationLineBorderSize", &GeoTreeView::GetLocationLineBorderSize, "Get border size for location lines.")
			.def("SetCorrelationLineBorderSize", &GeoTreeView::SetCorrelationLineBorderSize, "Set border size for correlation lines.")
			.def("GetCorrelationLineBorderSize", &GeoTreeView::GetCorrelationLineBorderSize, "Get border size for correlation lines.")
			.def("SetLeafNodeBorderColour", &GeoTreeView::SetLeafNodeBorderColour, "Set colour of leaf node borders.")
			.def("GetLeafNodeBorderColour", &GeoTreeView::GetLeafNodeBorderColour, "Get colour of leaf node borders.")
			.def("SetInternalNodeBorderColour", &GeoTreeView::SetInternalNodeBorderColour, "Set colour of internal node borders.")
			.def("GetInternalNodeBorderColour", &GeoTreeView::GetInternalNodeBorderColour, "Get colour of internal node borders.")
			.def("SetGeoPtsBorderColour", &GeoTreeView::SetGeoPtsBorderColour, "Set colour of geographic point borders.")
			.def("GetGeoPtsBorderColour", &GeoTreeView::GetGeoPtsBorderColour, "Get colour og geographic point borders.")
			.def("SetLocationLineBorderColour", &GeoTreeView::SetLocationLineBorderColour, "Set colour of location line borders.")
			.def("GetLocationLineBorderColour", &GeoTreeView::GetLocationLineBorderColour, "Get colour of location line borders.")
			.def("SetCorrelationLineBorderColour", &GeoTreeView::SetCorrelationLineBorderColour, "Set colour of correlation line borders.")
			.def("GetCorrelationLineBorderColour", &GeoTreeView::GetCorrelationLineBorderColour, "Get colour of correlation line borders.")
			.def("SetBranchBorderColour", &GeoTreeView::SetBranchBorderColour, "Set colour of branch borders.")
			.def("GetBranchBorderColour", &GeoTreeView::GetBranchBorderColour, "Get colour of branch borders.")
			.def("SetTreeColourStyle", &GeoTreeView::SetTreeColourStyle, "Set colouring method for tree.")
			.def("GetTreeColourStyle", &GeoTreeView::GetTreeColourStyle, "Get colouring method for tree.")
			.def("SetReverseOrderGeographicPts", &GeoTreeView::SetReverseOrderGeographicPts, "Set whether geographic points should be reversed.")
			.def("GetReverseOrderGeographicPts", &GeoTreeView::GetReverseOrderGeographicPts, "Check if geographic points are being reversed.")
			.def("GetNumberOfNodes", &GeoTreeView::GetNumberOfNodes, "Get total number of nodes.")
			.def("GetNumberOfLeaves", &GeoTreeView::GetNumberOfLeaves, "Get number of leaf nodes.")
			.def("GetDepthOfTree", &GeoTreeView::GetDepthOfTree, "Get maximum depth of tree.")
			.def("IsMultifurcating", &GeoTreeView::IsMultifurcating, "Check if tree is multifurcating.")		
			.def("GetLeafNames", &GeoTreeView::GetLeafNames, "Get names of leaves.")		
			.def("SignificanceTest", &GeoTreeView::SignificanceTest, "Perform Monte Carlo significance test.")
			.def("GetNewickString", &GeoTreeView::GetNewickString, "Get Newick representation of tree.")
			.def("PerformLinearAxesAnalysisOnRootNode", &GeoTreeView::PerformLinearAxesAnalysisOnRootNode, "Calculate goodness-of-fit for all linear axes.")
			.def("SetGeographicAxisAngleOnRoot", &GeoTreeView::SetGeographicAxisAngleOnRoot, "Set geographic layout line to the specified angle.")
			;

		// <wiki-header>Layer class</wiki-header>
		class_<Layer, boost::noncopyable, LayerPtr >("Layer", "Abstract base class for a layer.", no_init)
			.def("GetId", &Layer::GetId, "Get id of layer")
			.def("GetName", &Layer::GetName, "Get name of layer.")
			.def("SetName", &Layer::SetName, "Set name of layer.")
			.def("GetDescription", &Layer::GetDescription, "Get description of layer.")
			.def("SetDescription", &Layer::SetDescription, "Set description of layer.")
			.def("GetAuthours", &Layer::GetAuthours, "Get authours of layer.")
			.def("SetAuthours", &Layer::GetName, "Set authours of layer.")
			.def("GetPath", &Layer::GetPath, "Get path to layer file.")
			.def("GetFilename", &Layer::GetFilename, "Get filename of layer.")
			.def("GetType", &Layer::GetType, "Get type of layer.")		
			;

		MapLayerPtr(StudyLayer::*GetMapLayerByIndex)(unsigned int index) const = &StudyLayer::GetMapLayer;
		VectorMapLayerPtr(StudyLayer::*GetVectorMapLayerByIndex)(unsigned int index) const = &StudyLayer::GetVectorMapLayer;

		// <wiki-header>Study Layer class</wiki-header>
		class_<StudyLayer, bases<Layer>, boost::noncopyable, StudyLayerPtr >("Study", "Study layer.", no_init)
			.def("GetNumMapLayers", &StudyLayer::GetNumMapLayers, "Get number of map layers in this study.")
			.def("GetNumVectorMapLayers", &StudyLayer::GetNumVectorMapLayers, "Get number of vector map layers in this study.")
			.def("GetMapLayer", GetMapLayerByIndex, "Get specified map layer.") 
			.def("GetVectorMapLayer", GetVectorMapLayerByIndex, "Get specified vector map layer.")
			//.def("GetAllMapLayers", &StudyLayer::GetAllMapLayers, "Get all maps in study.")
			.def("GetController", &StudyLayer::GetStudyController, "Get study controller.")
			;

		// <wiki-header>Study Controller class</wiki-header>
		class_<StudyController, boost::noncopyable, StudyControllerPtr >("StudyController", "Control properties of study.", no_init)
			.def("GetDatum", &StudyController::GetDatum, "Get datum used by study.")
			.def("SetDatum", &StudyController::SetDatum, "Set datum used by study (must be set before map is loaded).")
			.def("GetProjection", &StudyController::GetProjection, "Get projection used by study.")
			.def("SetProjection", &StudyController::SetProjection, "Set projection used by study (must be set before map is loaded).")
			.def("IsGeographic", &StudyController::IsGeographic, "Flag indicating if data is in geographic (i.e. lat/long) coordinates.")
			.def("IsProjectData", &StudyController::IsProjectData, "Get flag indicating if data should be projected.")
			.def("IsUsingProjection", &StudyController::IsUsingProjection, "Flag indicating if data is in geographic (i.e. lat/long) coordinates. For use with Python")
			.def("IsUsingGeographic", &StudyController::IsUsingGeographic, "Get flag indicating if data should be projected. For use with Python")
			;

		LocationSetLayerPtr (MapLayer::*GetLocationSetLayerByIndex)(unsigned int index) const = &MapLayer::GetLocationSetLayer;
		TreeLayerPtr (MapLayer::*GetTreeLayerByIndex)(unsigned int index) const = &MapLayer::GetTreeLayer;

		// <wiki-header>Map Layer class</wiki-header>
		class_<MapLayer, bases<Layer>, boost::noncopyable, MapLayerPtr >("Map", "Map layer.", no_init)
			.def("GetNumLocationSetLayers", &MapLayer::GetNumLocationSetLayers, "Get number of location set layers for this map.")
			.def("GetLocationSetLayer", GetLocationSetLayerByIndex, "Get specified location set layer.")    
			.def("GetAllLocationSetLayers", &MapLayer::GetAllLocationSetLayers, "Get all location set layers.")   
			.def("GetNumTreeLayers", &MapLayer::GetNumTreeLayers, "Get number of tree layers for this map.")
			.def("GetTreeLayer", GetTreeLayerByIndex, "Get specified tree layer.")
			.def("GetAllTreeLayers", &MapLayer::GetAllTreeLayers, "Get all tree layers.")  
			.def("GetController", &MapLayer::GetMapController, "Get map controller.")
			;

		// <wiki-header>Vector Map Layer class</wiki-header>
		class_<VectorMapLayer, bases<Layer>, boost::noncopyable, VectorMapLayerPtr >("VectorMap", "Vector map layer.", no_init)			
			.def("GetController", &VectorMapLayer::GetVectorMapController, "Get vector map controller.")
			;

		// <wiki-header>Vector Map Controller class</wiki-header>
		class_<VectorMapController, boost::noncopyable, VectorMapControllerPtr >("VectorMapController", "properties of the vector map controller.", no_init)
			.def("GetOriginalVectorExtents", &VectorMapController::GetOriginalVectorExtents, "Get lower, left and upper, right corners of Vector map.")
			.def("GetProjectedVectorExtents", &VectorMapController::GetProjectedVectorExtents, "Get lower, left and upper, right corners of Vector map after projection.")
			.def("GetGeometryType", &VectorMapController::GetGeometryType, "Get geometry type of the vector map featurs.")
			.def("GetNumFeatures", &VectorMapController::GetNumFeatures, "Get number of vector map featurs.")
			.def("GetController", &VectorMapController::GetVectorMapView, "Get vector map view.")

			;

		// <wiki-header>Vector Map View class</wiki-header>
		class_<VectorMapView, boost::noncopyable, VectorMapViewPtr >("VectorMapView", "properties of the vector map view.", no_init)
			.def("SetPointColour", &VectorMapView::SetPointColour, "Set the colour of point features in the Vector map.")
			.def("SetPointBorderColour", &VectorMapView::SetPointBorderColour, "Set the border colour of point features in the Vector map.")
			.def("SetPointSize", &VectorMapView::SetPointSize, "Set the size of point features in the Vector map.")
			.def("SetPointBorderSize", &VectorMapView::SetPointBorderSize, "Set the border size of point features in the Vector map.")
			.def("SetPointShape", &VectorMapView::SetPointShape, "Set the shape of point features in the Vector map.")
			.def("SetLineColour", &VectorMapView::SetLineColour, "Set the colour of line features in the Vector map.")
			.def("SetLineBorderColour", &VectorMapView::SetLineBorderColour, "Set the border colour of line features in the Vector map.")
			.def("SetLineSize", &VectorMapView::SetLineSize, "Set the thickness of line features in the Vector map.")
			.def("SetLineBorderSize", &VectorMapView::SetLineBorderSize, "Set the border thickness of line features in the Vector map.")
			.def("SetLineStyle", &VectorMapView::SetLineStyle, "Set the style of line features in the Vector map.")
			.def("SetPolygonBorderColour", &VectorMapView::SetPolygonBorderColour, "Set the border colour of polygon features in the Vector map.")
			.def("SetPolygonBorderSize", &VectorMapView::SetPolygonBorderSize, "Set the border thickness of polygon features in the Vector map.")
			.def("SetPolygonBorderStyle", &VectorMapView::SetPolygonBorderStyle, "Set the border style of polygon features in the Vector map.")
			;


		// <wiki-header>Tree Layer class</wiki-header>
		class_<TreeLayer, bases<Layer>, boost::noncopyable, TreeLayerPtr >("Tree", "Tree layer.", no_init)
			.def("GetGeoTreeView", &TreeLayer::GetGeoTreeView, "Get geophylogeny associated with this layer.")
			.def("IsActive", &TreeLayer::IsActive, "Get active state of layer.")
			;

		// <wiki-header>Location Set Layer class</wiki-header>
		LocationLayerPtr (LocationSetLayer::*GetLocationLayerByIndex)(unsigned int index) const = &LocationSetLayer::GetLocationLayer;

		class_<LocationSetLayer, bases<Layer>, boost::noncopyable, LocationSetLayerPtr >("LocationSet", "Location set layer.", no_init)
			.def("GetNumLocationLayers", &LocationSetLayer::GetNumLocationLayers, "Get number of location layers for this location set.")
			.def("GetLocationLayer", GetLocationLayerByIndex, "Get specified location layer.")   
			.def("GetAllLocationLayers", &LocationSetLayer::GetAllLocationLayers, "Get all location layers.")
			.def("GetAllActiveLocationLayers", &LocationSetLayer::GetAllActiveLocationLayers, "Get all visible location layers.")
			.def("IsSequencesData", &LocationSetLayer::IsSequencesData, "Check if there is sequence data associated with at least one location.")
			.def("GetController", &LocationSetLayer::GetLocationSetController, "Get location set controller.")
			.def("GetChartSet", &LocationSetLayer::GetChartSetView, "Get set of charts associated with this location set.")
			;

		// <wiki-header>Location Set Controller class</wiki-header>
		class_<LocationSetController, boost::noncopyable, LocationSetControllerPtr >("LocationSetController", "Control properties of location set.", no_init)
			.def("SetLocationVisibilities", &LocationSetController::SetLocationVisibilities, "Propogate set visibility state to all locations.")
			.def("SetLocationColours", &LocationSetController::SetLocationColours, "Propogate set colour state to all locations.")
			.def("SetLocationBorders", &LocationSetController::SetLocationBorders, "Propogate set border state to all locations.")
			.def("SetLocationSizes", &LocationSetController::SetLocationSizes, "Propogate set size state to all locations.")
			.def("SetLocationShapes", &LocationSetController::SetLocationShapes, "Propogate set shape state to all locations.")
			.def("SetLocationLabels", &LocationSetController::SetLocationLabels, "Propogate specified label states to all locations.")
			.def("SetColourField", &LocationSetController::SetColourField, "Set field used to determine location colour.")
			.def("GetColourField", &LocationSetController::GetColourField, "Get field used to determine location colour.")
			.def("SetColourMap", &LocationSetController::SetColourMap, "Set colour map associated with location set.")
			.def("GetColourMap", &LocationSetController::GetColourMap, "Get colour map associated with location set.")
			.def("SetUniformColourFlag", &LocationSetController::SetUniformColourFlag, "Set flag indicating if all locations should have the same colour.")
			.def("GetUniformColourFlag", &LocationSetController::GetUniformColourFlag, "Get flag indicating if all locations should have the same colour.")
			.def("SetUniformColour", &LocationSetController::SetUniformColour, "Set uniform colour.")
			.def("GetUniformColour", &LocationSetController::GetUniformColour, "Get uniform colour.")
			.def("SetBorderSize", &LocationSetController::SetBorderSize, "Set border size.")
			.def("GetBorderSize", &LocationSetController::GetBorderSize, "Get border size..")
			.def("SetBorderColour", &LocationSetController::SetBorderColour, "Set border colour.")
			.def("GetBorderColour", &LocationSetController::GetBorderColour, "Get border colour.")
			.def("SetLocationSetVisibility", &LocationSetController::SetLocationSetVisibility, "Set visibility of location set.")
			.def("GetLocationSetVisibility", &LocationSetController::GetLocationSetVisibility, "Get visibility of location set.")
			.def("SetShapeField", &LocationSetController::SetShapeField, "Set field used to determine location shape.")
			.def("GetShapeField", &LocationSetController::GetShapeField, "Get field used to determine location shape.")
			.def("SetShapeMap", &LocationSetController::GetUniformColour, "Set shape map associated with location set.")
			.def("GetShapeMap", &LocationSetController::GetUniformColour, "Get shape map associated with location set.")
			.def("SetUniformShapeFlag", &LocationSetController::SetUniformShapeFlag, "Set flag indicating if all locations should have the same shape.")
			.def("GetUniformShapeFlag", &LocationSetController::GetUniformShapeFlag, "Get flag indicating if all locations should have the same shape.")
			.def("SetUniformShape", &LocationSetController::SetUniformShape, "Set uniform shape.")
			.def("GetUniformShape", &LocationSetController::GetUniformShape, "Get uniform shape.")
			.def("SetSizeField", &LocationSetController::SetSizeField, "Set field used to determine location size.")
			.def("GetSizeField", &LocationSetController::GetSizeField, "Get field used to determine location size.")
			.def("SetMinLocationSize", &LocationSetController::SetMinLocationSize, "Set min location size.")
			.def("GetMinLocationSize", &LocationSetController::GetMinLocationSize, "Get min location size.")
			.def("SetMaxLocationSize", &LocationSetController::SetMaxLocationSize, "Set max location size.")
			.def("GetMaxLocationSize", &LocationSetController::GetMaxLocationSize, "Get max location size.")
			.def("SetLabelField", &LocationSetController::SetLabelField, "Set field used to determine location label.")
			.def("GetLabelField", &LocationSetController::GetLabelField, "Get field used to determine location label.")
			.def("SetLabelColour", &LocationSetController::SetLabelColour, "Set label colour.")
			.def("GetLabelColour", &LocationSetController::GetLabelColour, "Get label colour.")
			.def("SetLabelSize", &LocationSetController::SetLabelSize, "Set label size.")
			.def("GetLabelSize", &LocationSetController::GetLabelSize, "Get label size.")
			.def("SetLabelPosition", &LocationSetController::SetLabelPosition, "Set label position.")
			.def("GetLabelPosition", &LocationSetController::GetLabelPosition, "Get label position.")
			.def("SetLabelStyle", &LocationSetController::SetLabelStyle, "Set label style.")
			.def("GetLabelStyle", &LocationSetController::GetLabelStyle, "Get label style.")
			.def("SetLabelVisibility", &LocationSetController::SetLabelVisibility, "Set label visibility.")
			.def("GetLabelVisibility", &LocationSetController::GetLabelVisibility, "Get label visibility.")
			.def("SetLabelBindToChart", &LocationSetController::SetLabelBindToChart, "Set flag indicating if label is bound to chart.")
			.def("GetLabelBindToChart", &LocationSetController::GetLabelBindToChart, "Get flag indicating if label is bound to chart.")
			.def("GetMetadata", &LocationSetController::GetMetadata, "Get location metadata for the specified field (list of strings).")
			.def("GetNumericMetadata", &LocationSetController::GetNumericMetadata, "Get location metadata for the specified field (list of floats).")
			.def("GetMetadataFields", &LocationSetController::GetMetadataFields, "Get fields for location metadata.")
			.def("GetNumericMetadataFields", &LocationSetController::GetNumericMetadataFields, "Get numeric fields for location metadata.")
			.def("AddMetadata", &LocationSetController::AddMetadata, "Add field to location metadata.")
			;

		// <wiki-header>Location Layer class</wiki-header>
		class_<LocationLayer, bases<Layer>, boost::noncopyable, LocationLayerPtr >("Location", "Location layers.", no_init)
			.def("GetNumSequenceLayers", &LocationLayer::GetNumSequenceLayers, "Get number of sequence layers for this location.")
			.def("GetSequenceLayer", &LocationLayer::GetSequenceLayer, "Get specified sequence layer.")   
			.def("GetAllSequenceLayers", &LocationLayer::GetAllSequenceLayers, "Get all sequence layers.")
			.def("GetAllActiveSequenceLayers", &LocationLayer::GetAllActiveSequenceLayers, "Get all active sequence layers.")
			.def("GetController", &LocationLayer::GetLocationController, "Get location controller.")
			;

		// <wiki-header>Location Controller class</wiki-header>
		class_<LocationController, boost::noncopyable, LocationControllerPtr >("LocationController", "Control properties of location.", no_init)
			.def("GetId", &LocationController::GetId, "Get id of location.")
			.def("GetNorthing", &LocationController::GetNorthing, "Get north or latitudinal position of location.")
			.def("GetEasting", &LocationController::GetEasting, "Get east or longitudinal position of location.")
			.def("GetLatitude", &LocationController::GetNorthing, "Get north or latitudinal position of location.")
			.def("GetLongitude", &LocationController::GetEasting, "Get east or longitudinal position of location.")
			.def("GetData", &LocationController::GetData, "Get metadata associated with location.")
			.def("GetProjection", &LocationController::GetProjection, "Get projected coordinates of location.")
			.def("GetPosition", &LocationController::GetRenderingOrder, "Get grid coordinates of location.")
			.def("SetColour", &LocationController::SetColour, "Set colour of location marker.")
			.def("GetColour", &LocationController::GetColour, "Get colour of location marker.")
			.def("SetSize", &LocationController::SetSize, "Set size of location marker.")
			.def("GetSize", &LocationController::GetSize, "Get size of location marker.")
			.def("SetShape", &LocationController::SetShape, "Set shape of location marker.")
			.def("GetShape", &LocationController::GetShape, "Get shape of location marker.")
			.def("SetBorderSize", &LocationController::SetBorderSize, "Set border size of location marker.")
			.def("GetBorderSize", &LocationController::GetBorderSize, "Get border size of location marker.")
			.def("SetActive", &LocationController::SetActive, "Set active state of location marker.")
			.def("IsActive", &LocationController::IsActive, "Check if location is active.")
			.def("SetRenderingOrder", &LocationController::SetRenderingOrder, "Set rendering order of location marker.")
			.def("GetRenderingOrder", &LocationController::GetRenderingOrder, "Get rendering order of location marker.")
			.def("AddData", &LocationController::AddData, "Add metadata to location.")
			;

		// <wiki-header>Sequence Layer class</wiki-header>
		class_<SequenceLayer, bases<Layer>, boost::noncopyable, SequenceLayerPtr >("Sequence", "Sequence layer.", no_init)
			.def("GetController", &SequenceLayer::GetSequenceController, "Get sequence controller.")
			;

		// <wiki-header>Sequence Controller class</wiki-header>
		class_<SequenceController, boost::noncopyable, SequenceControllerPtr >("SequenceController", "Control properties of sequence.", no_init)
			.def("GetSequenceId", &SequenceController::GetSequenceId, "Get id of sequence.")
			.def("GetSiteId", &SequenceController::GetSiteId, "Get id of site where sequence was sampled.")    
			.def("IsActive", &SequenceController::IsActive, "Check if sequence is active.")
			.def("SetActive", &SequenceController::SetActive, "Set active state of sequence.")
			.def("GetData", &SequenceController::GetData, "Get metadata associated with sequence.")
			.def("GetMetadataFields", &SequenceController::GetMetadataFields, "Get metadata fields.")
			.def("GetNumericMetadataFields", &SequenceController::GetNumericMetadataFields, "Get numeric fields for sequences.")
			;

		// <wiki-header>Chart Set View class</wiki-header>
		class_<ChartSetView, boost::noncopyable, ChartSetViewPtr >("ChartSetView", no_init)
			.def("GetChart", &ChartSetView::GetChart, "Get specified chart.")
			.def("SetLayoutLine", &ChartSetView::SetLayoutLine, "Set layout line for chart set.")
			.def("IsLayoutLine", &ChartSetView::IsLayoutLine, "Check if a layout line has been specified for the chart set.")
			.def("GetLayoutLineVisibility", &ChartSetView::GetLayoutLineVisibility, "Get visibility of layout line.")
			.def("SetLayoutLineVisibility", &ChartSetView::SetLayoutLineVisibility, "Set visibility of layout line.")
			.def("SetLayout", &ChartSetView::SetLayout, "Set layout style.")
			.def("GetLayout", &ChartSetView::GetLayout, "Get layout style.")
			.def("UpdateCharts", &ChartSetView::UpdateCharts, "Update chart to reflect any changes in the underlying data (e.g., changed active sequences).")
			;

		// <wiki-header>Chart View class</wiki-header>
		class_<ChartView, boost::noncopyable, ChartViewPtr >("ChartView", "Provides access to geophylogeny properties and functionality.", no_init)
			.def("GetVisibility", &ChartView::IsVisible, "Get visibility of chart.")
			.def("SetVisibility", &ChartView::SetVisibility, "Set visibility of chart.")
			.def("GetDepth", &ChartView::GetDepth, "Get depth of chart (controls rendering order).")
			.def("SetDepth", &ChartView::SetDepth, "Set depth of chart (controls rendering order).")
			.def("GetWidth", &ChartView::GetWidth, "Get width or diameter of chart.")
			.def("SetWidth", &ChartView::SetWidth, "Set width or diameter of chart.")
			.def("GetHeight", &ChartView::GetHeight, "Get height of chart.")
			.def("SetHeight", &ChartView::SetHeight, "Set height of chart.")
			.def("GetBorderSize", &ChartView::GetBorderSize, "Get border size of chart.")
			.def("SetBorderSize", &ChartView::SetBorderSize, "Set border size of chart.")
			.def("GetWedgeBorders", &ChartView::GetWedgeBorders, "Get wedge border size.")
			.def("SetWedgeBorders", &ChartView::SetWedgeBorders, "Set wedge border size.")
			.def("GetSizeProportionalToSeq", &ChartView::GetSizeProportionalToSeq, "Get flag indicating if chart size should be proportional to sequence count.")
			.def("SetSizeProportionalToSeq", &ChartView::SetSizeProportionalToSeq, "Set flag indicating if chart size should be proportional to sequence count.")
			.def("GetMinSize", &ChartView::GetMinSize, "Get minimum allowed size of chart.")
			.def("SetMinSize", &ChartView::SetMinSize, "Set minimum allowed size of chart.")
			.def("GetMaxSize", &ChartView::GetMaxSize, "Get maximum allowed size of chart.")
			.def("SetMaxSize", &ChartView::SetMaxSize, "Set maximum allowed size of chart.")
			.def("GetAdjustedSize", &ChartView::GetAdjustedSize, "Get adjusted chart size based on number of sequences." )
			.def("SetNormalizedNumSeq", &ChartView::SetNormalizedNumSeq, "Set normalized number of sequences for this chart." )
			.def("SetField", &ChartView::SetField, "Set field used to create chart.")
			.def("GetField", &ChartView::GetField, "Get field used to create chart.")
			.def("GetNames", &ChartView::GetNames, "Get name of all category.")
			.def("GetPercentage", &ChartView::GetPercentage, "Get percentage of sequences assigned to the specified category.")
			.def("SetChartType", &ChartView::SetChartType, "Set type of chart to generate.")
			.def("GetChartType", &ChartView::GetChartType, "Get type of chart to generate.")
			.def("SetAssignToOther", &ChartView::SetAssignToOther, "Set proportion for which a category will be assigned to the 'Other' category.")
			.def("GetAssignToOther", &ChartView::GetAssignToOther, "Get proportion for which a category will be assigned to the 'Other' category.")
			.def("SetCentre", &ChartView::SetCentre, "Set centre of chart.")
			.def("GetCentre", &ChartView::GetCentre, "Get centre of chart.")
			.def("GetLocationPos", &ChartView::GetLocationPos, "Get position of associated location.")
			.def("SetScreenPos", &ChartView::SetScreenPos, "Set position of chart in screen coordinates.")
			.def("GetScreenPos", &ChartView::GetScreenPos, "Get position of chart in screen coordinates.")
			.def("SetLabelBindToChart", &ChartView::SetLabelBindToChart, "Set flag indicating if labels should be bound to charts.")
			.def("GetLabelBindToChart", &ChartView::GetLabelBindToChart, "Get flag indicating if labels should be bound to charts.")
			.def("SetLabel", &ChartView::SetLabel, "Set chart label.")
			.def("GetLabel", &ChartView::GetLabel, "Get chart label.")
			.def("UpdateChart", &ChartView::UpdateChart, "Update chart to reflect any changes in the underlying data (e.g., changed active sequences)." )
			;

		// <wiki-header>Visual Label class</wiki-header>
		class_<VisualLabel>("VisualLabel", "Specifies a label for display in the viewport.")
			.def(init<const std::wstring&, const Colour&, float, VisualLabel::LABEL_RENDERING_STYLE>())
			.def("GetText", &VisualLabel::GetText, "Get text of label.")
			.def("GetColour", &VisualLabel::GetColour, "Get colour of label font.")
			.def("GetSize", &VisualLabel::GetSize, "Get size of label font.")
			.def("GetVisibility", &VisualLabel::IsVisible, "Get visibility of label.")
			.def("GetGridPosition", &VisualLabel::GetGridPosition, "Get grid coordinates of label.")
			.def("GetScreenPosition", &VisualLabel::GetScreenPosition, "Get screen coordinates of label.")
			.def("GetRotateLabel", &VisualLabel::GetRotateLabel, "Get flag indicating if text should be rotated onto the plane.")
			.def("GetRotateZ", &VisualLabel::GetRotateZ, "Get flag indicating if text should rotate to remain horizontal.")
			.def("GetRenderingStyle", &VisualLabel::GetRenderingStyle, "Get rendering style of label.")
			.def("SetText", &VisualLabel::SetText, "Set text of label.")
			.def("SetColour", &VisualLabel::SetColour, "Set colour of label font.")
			.def("SetSize", &VisualLabel::SetSize, "Set size of label font.")
			.def("SetVisibility", &VisualLabel::SetVisibility, "Set visibility of label.")
			.def("SetGridPosition", &VisualLabel::SetGridPosition, "Set grid position of label.")
			.def("SetScreenPosition", &VisualLabel::SetScreenPosition, "Set screen position of label.")
			.def("SetRenderingStyle", &VisualLabel::SetRenderingStyle, "Set rendering style of label.")
			;

		// <wiki-header>Visual Marker class</wiki-header>
		class_<VisualMarker>("VisualMarker", "Specified a marker for display in the viewport.")
			.def(init<const Colour&, float, VisualMarker::MARKER_SHAPE, const Point3D&>())
			.def("GetPosition", &VisualMarker::GetPosition, "Get position of marker.")
			.def("GetShape", &VisualMarker::GetShape, "Get shape of marker.")
			.def("GetRotation", &VisualMarker::GetRotation, "Get rotation of marker.")
			.def("GetDepth", &VisualMarker::GetDepth, "Get depth of marker.")
			.def("GetRenderingStyle", &VisualMarker::GetRenderingStyle, "Get rendering style of marker.")
			.def("GetColour", &VisualMarker::GetColour, "Get colour of marker.")
			.def("GetSize", &VisualMarker::GetSize, "Get size of marker.")
			.def("GetVisibility", &VisualMarker::IsVisible, "Get visibility of marker.")
			.def("GetRenderingOrder", &VisualMarker::GetRenderingOrder, "Get rendering order of marker.")
			.def("GetBorderSize", &VisualMarker::GetBorderSize, "Get border size of marker.")
			.def("SetPosition", &VisualMarker::SetPosition, "Set position of marker.")
			.def("SetShape", &VisualMarker::SetShape, "Set shape of marker.")
			.def("SetRotation", &VisualMarker::SetRotation, "Set rotation of marker.")
			.def("SetDepth", &VisualMarker::SetDepth, "Set depth of marker.")
			.def("SetRenderingStyle", &VisualMarker::SetRenderingStyle, "Set rendering style of marker.")
			.def("SetColour", &VisualMarker::SetColour, "Set colour of marker.")
			.def("SetSize", &VisualMarker::SetSize, "Set size of marker.")
			.def("SetVisibility", &VisualMarker::SetVisibility, "Set visibility of marker.")
			.def("SetRenderingOrder", &VisualMarker::SetRenderingOrder, "Set rendering order of marker.")
			.def("SetBorderSize", &VisualMarker::SetBorderSize, "Set border size of marker.")
			;

		// <wiki-header>Visual Line class</wiki-header>
		class_<VisualLine>("VisualLine", "Specified a line for display in the viewport.")
			.def(init<const Colour&, float, VisualLine::LINE_STYLE, const Line3D&>())
			.def("GetColour", &VisualLine::GetColour, "Get colour of line.")
			.def("GetThickness", &VisualLine::GetThickness, "Get thickness of line.")
			.def("GetVisibility", &VisualLine::IsVisible, "Get visibility of line.")
			.def("GetLineStyle", &VisualLine::GetLineStyle, "Get style of line.")
			.def("GetStartPt", &VisualLine::GetStartPt, "Get start point of line.")
			.def("GetEndPt", &VisualLine::GetEndPt, "Get end point of line.")
			.def("SetColour", &VisualLine::SetColour, "Set colour of line.")
			.def("SetThickness", &VisualLine::SetThickness, "Set thickness of line.")
			.def("SetVisibility", &VisualLine::SetVisibility, "Set visibility of line.")
			.def("SetLineStyle", &VisualLine::SetLineStyle, "Set style of line.")
			.def("SetStartPt", &VisualLine::SetStartPt, "Set start point of line.")
			.def("SetEndPt", &VisualLine::SetEndPt, "Set end point of line.")
			.def("GetLine", &VisualLine::GetLine, "Get line.")
			.def("SetLine", &VisualLine::SetLine, "Set line.")
			.def("GetDepth", &VisualLine::GetDepth, "Get depth of line (determines rendering order).")
			.def("SetDepth", &VisualLine::SetDepth, "Set depth of line (determines rendering order).")
			;

		// <wiki-header>Colour class</wiki-header>
		class_<Colour>("Colour", "Specified RGBA colour. Channels values are between [0, 1].")
			.def(init<float, float, float, optional<float> >())
			.def("GetRed", &Colour::GetRed, "Get red channel.")
			.def("SetRed", &Colour::SetRed, "Set red channel.")
			.def("GetGreen", &Colour::GetGreen, "Get green channel.")
			.def("SetGreen", &Colour::SetGreen, "Set green channel.")
			.def("GetBlue", &Colour::GetBlue, "Get blue channel.")
			.def("SetBlue", &Colour::SetBlue, "Set blue channel.")
			.def("GetAlpha", &Colour::GetAlpha, "Get alpha channel.")
			.def("SetAlpha", &Colour::SetAlpha, "Set alpha channel.")
			.def("SetRGBA", &Colour::SetRGBA, "Set new colour.")
			;

		// <wiki-header>Colour Map Base class</wiki-header>
		class_<ColourMap, boost::noncopyable, ColourMapPtr>("ColourMap", "Base class for colour maps.")
			.def(init<std::wstring, ColourMap::COLOUR_MAP_TYPE>())
			.def("GetColour", &ColourMap::GetColour, "Get colour at specified index.")
			.def("GetInterpolatedColour", &ColourMap::GetInterpolatedColour, "Interpolate colour from colour map.")
			.def("GetName", &ColourMap::GetName, "Get name of colour map.")
			.def("GetExtendedName", &ColourMap::GetExtendedName, "Get name of colour map with either 'Continuous:' or 'Discrete:' prefixed to it.")
			.def("GetSize", &ColourMap::GetSize, "Get size of colour map.")
			.def("GetType", &ColourMap::GetType, "Get type of colour map.")
			;

		// Discrete colour map
		class_<ColourMapDiscrete, bases<ColourMap>, boost::noncopyable, ColourMapDiscretePtr >("ColourMapDiscrete", "Specifies a discrete colour map.")
			;

		// Discrete colour map
		class_<ColourMapContinuous, bases<ColourMap>, boost::noncopyable, ColourMapContinuousPtr >("ColourMapContinuous", "Specifies a continuous colour map.")
			;

		// <wiki-header>Discrete Colour Map class</wiki-header>
		class_<ColourMapManager, boost::noncopyable, ColourMapManagerPtr >("ColourMapManager", "Provides access to all colour maps supported by GenGIS.")
			.def("GetColourMap", &ColourMapManager::GetColourMap, "Get colour map with the provided name.")
			.def("GetColourMapNames", &ColourMapManager::GetColourMapNames, "Get name of all colour maps.")
			.def("GetColourMapDiscreteNames", &ColourMapManager::GetColourMapDiscreteNames, "Get name of all discrete colour maps.")
			.def("GetColourMapContinuousNames", &ColourMapManager::GetColourMapContinuousNames, "Get name of all continuous colour maps.")
			;

		// <wiki-header>Point3D class</wiki-header>
		class_<Point3D>("Point3D", "Specifies a point in 3D space.")
			.def(init<float, float, float>())
			.def("Length", &Point3D::Length, "Distance from origin to point.")
			.def("SquaredLength", &Point3D::SquaredLength, "Squared distance from origin to point.")
			.def("Offset", &Point3D::Offset, "Create new point offset by the specified amount.")
			.def("Normalize", &Point3D::Normalize, return_value_policy<copy_const_reference>(), "Normalize point (vector) to unit length.")
			.def("Cross", &Point3D::Cross, "Calculate cross product between two points (vectors).")
			.def("Dot", &Point3D::Dot, "Calculate dot product between two points (vectors).")
			.def("Angle", &Point3D::Angle, "Calculate angle between two points (vectors).")
			.def_readwrite("x", &Point3D::x, "Get x position of point.")
			.def_readwrite("y", &Point3D::y, "Get y position of point.")
			.def_readwrite("z", &Point3D::z, "Get z position of point.")
			;

		// <wiki-header>GeoCoord class</wiki-header>
		class_<GeoCoord>("GeoCoord", "Specifies geographic point in 2D space.")
			.def(init<float, float>())
			.def_readwrite("easting", &GeoCoord::easting, "Get easting or longitudinal position of location.")
			.def_readwrite("northing", &GeoCoord::northing, "Get northing or latitudinal position of location.")
			.def_readwrite("longitude", &GeoCoord::easting, "Get easting or longitudinal position of location.")
			.def_readwrite("latitude", &GeoCoord::northing, "Get northing or latitudinal position of location.")
			;

		// <wiki-header>Line3D class</wiki-header>
		class_<Line3D>("Line3D", "Specifies a line in 3D space.")
			.def(init<const Point3D&, const Point3D&>())
			.def("Offset", &Line3D::Offset, "New line offset by the specified amount.")
			.def_readwrite("start", &Line3D::start, "Start point of line.")
			.def_readwrite("end", &Line3D::end, "End point of line.")
			.def_readwrite("startId", &Line3D::startId, "General purpose label associated with start point.")
			.def_readwrite("endId", &Line3D::endId, "General purpose label associated with end point.")
			;

		// <wiki-header>Box2D class</wiki-header>
		class_<Box2D>("Box2D", "Specifies a 2D bounding box.")
			.def(init<float, float, float, float>())
			.def_readwrite("x1", &Box2D::x, "Lower, left x-coordinate of box.")
			.def_readwrite("y1", &Box2D::y, "Lower, left y-coordinate of box.")
			.def_readwrite("dx", &Box2D::dx, "Width of bounding box.")
			.def_readwrite("dy", &Box2D::dy, "Height of bounding box.")
			;

		// <wiki-header>Linear Results class</wiki-header>
		class_<LinearResults>("LinearResults", "Results of performing a linear axes analysis.")
			.def(init<float, unsigned int>())
			.def_readonly("angle", &LinearResults::angle, "Angle of geographic layout line in degrees.")
			.def_readonly("numCrossings", &LinearResults::numCrossings, "Number of crossings which occurs for this layout lines.")
			;
	}

	//==============================================================================================
	//  Provide access to common global objects.
	//==============================================================================================
	
	// <wiki-header>Global objects</wiki-header>
	
	scope().attr("study") = App::Inst().GetLayerTreeController()->GetStudyLayer(0); // "provides access to the Study Layer class"
	scope().attr("layerTree") = App::Inst().GetLayerTreeController();               // "provides access to the Layer Tree Controller class"
	scope().attr("viewport") = App::Inst().GetViewportWrapper();                    // "provides access to the Viewport class"
	scope().attr("camera") = App::Inst().GetViewport()->GetCamera();                // "provides access to the Camera class"
	scope().attr("mainWindow") = App::Inst().GetMainWindowWrapper();                // "provides access to the Main Window class"
	scope().attr("graphics") = App::Inst().GetUserElementsWrapper();                // "provides access to the Graphics class"
	scope().attr("colourMapManager") = App::Inst().GetColourMapManager();           // "provides access to the Colour Map Manager class"
}

PythonInterface::PythonInterface()
{
	// Initialize Python
	Py_Initialize();
	PyEval_InitThreads();

	PyRun_SimpleString("import sys");

	// WINDOWS: Add GenGIS\bin\Lib\site-packages to Python's path, so it can find the Python modules (ie wxPython) contained within.
	wxString gengisDir = App::Inst().GetExeDir();
	gengisDir = gengisDir.BeforeLast('\\').BeforeLast('\\');
	wxString sitePackagesPath = gengisDir + wxT("\\bin\\Lib\\site-packages");
	std::string sitePackagesStr = "sys.path.insert(0,r\"" + std::string(sitePackagesPath.mb_str())+ "\")";
	PyRun_SimpleString(sitePackagesStr.c_str());

	// MAC: Add site-packages directory to sys.path
	PyRun_SimpleString("sys.path.insert(0,r\"./GenGIS.app/Contents/Resources/site-packages\")");

	// Load the wxPython core API.
	if(!wxPyCoreAPI_IMPORT())
	{
		wxLogError(wxT("Error importing the wxPython API!"));
		PyErr_Print();
		Py_Finalize();
		return;
	}

	// Save the current Python thread state and release the Global Interpreter Lock.
	// m_mainTState = wxPyBeginAllowThreads();         // ~mikep. This make everything crash. When I get the time, find out why and fix it......
	m_mainTState = NULL;

	// Add Python to GenGIS:
	((GenGisFrame*)GenGIS::App::Inst().GetMainWindow())->AddConsolePanel(GetPythonPanel(), _T("Python"));

	// Initialize the Boost module:
	try
	{
		initGenGIS();                            // Tell Boost to initialize the module (all that boost code above).
		PyRun_SimpleString("import PythonCode"); // Initialize python interpreter
		PyRun_SimpleString("import GenGIS");     // Import the Boost module's contents into Python
		PyRun_SimpleString("import numpy");      // Start with some modules imported
		PyRun_SimpleString("from scipy.stats import linregress");
		PyRun_SimpleString("import wx");
		PyRun_SimpleString("import math");

		// add scripts directory to Python path
		wxString gengisDir = App::Inst().GetExeDir();
		gengisDir = gengisDir.BeforeLast('\\').BeforeLast('\\');
		wxString scriptsPath = gengisDir + wxT("\\scripts");
		std::string scriptsPathStr = "sys.path.insert(0,r\"" + std::string(scriptsPath.mb_str())+ "\")";
		PyRun_SimpleString(scriptsPathStr.c_str());
		
		// add /usr/X11/bin and /usr/local/bin to $PATH, if on a Mac/*nix
		#ifndef WIN32
			PyRun_SimpleString("import os");
			PyRun_SimpleString("os.environ['PATH']=os.environ['PATH']+':/usr/local/bin:/usr/X11/bin'");
		#endif
	}
	catch(error_already_set)
	{
		PyErr_Print();
	}
}

PythonInterface::~PythonInterface()
{
	// Tell wxPython to close and clean up.
	// wxPyEndAllowThreads(m_mainTState);     // ~mikep. had to comment this line out along with wxPyBeginAllowThreads() (which occurs later in this file)
	// Close the Python interpreter.
	Py_Finalize();
}

void PythonInterface::UpdatePythonInterface()
{
	initGenGIS();
}

// Adapted from the samples in wxPython
wxPanel* PythonInterface::GetPythonPanel() const
{
	wxWindow* parent = ((GenGisFrame*)GenGIS::App::Inst().GetMainWindow())->GetBottomNotebook();

	PyObject* result;
	wxPyBlock_t blocked = wxPyBeginBlockThreads(); // Get the Global Interpreter Lock.

	PyObject* globals = PyDict_New();
	PyObject* builtins = PyImport_ImportModule("__builtin__");
	PyDict_SetItemString(globals, "__builtins__", builtins);
	Py_DECREF(builtins);

	// Make a method that makes the wxPython panel and return it.
	char* pythonPanelCode = "import sys\nsys.path.insert(0,'./GenGIS.app/Contents/Resources') # Mac location \nsys.path.insert(0,'.') # Windows location \nfrom PythonCode import PythonInterpreter\n\ndef makePanel(parent):\n  pyPanel = PythonInterpreter(parent)\n  return pyPanel\n";
	result = PyRun_String(pythonPanelCode, Py_file_input, globals, globals);

	// Check for an exception.
	if(!result)
	{
		PyErr_Print();
		wxPyEndBlockThreads(blocked);
		return NULL;
	}
	Py_DECREF(result);

	// Get the 'makePanel' method
	PyObject* func = PyDict_GetItemString(globals, "makePanel");
	wxASSERT(PyCallable_Check(func));

	// Call makePanel
	PyObject* arg = wxPyMake_wxObject(parent, false);
	wxASSERT(arg != NULL);
	PyObject* tuple = PyTuple_New(1);
	PyTuple_SET_ITEM(tuple, 0, arg);
	result = PyEval_CallObject(func, tuple);

	// Check for an exception.
	if(!result)
	{
		PyErr_Print();
	}
	else
	{
		// Bring the wxPython wxPanel into GenGIS
		bool success = wxPyConvertSwigPtr(result, (void**)&m_pythonPanel, _T("wxPanel"));

		wxASSERT_MSG(success, _T("Returned object was not a wxPanel!"));
		Py_DECREF(result);
	}

	// Release the python objects we still have
	Py_DECREF(globals);
	Py_DECREF(tuple);

	// Release the Global Interpreter Lock.
	wxPyEndBlockThreads(blocked);

	// Redirect sys.stderr, sys.stdin, sys.stdout to the shell.
	PyRun_SimpleString("shell.redirectStderr(redirect=True)");
	PyRun_SimpleString("shell.redirectStdin(redirect=True)");
	PyRun_SimpleString("shell.redirectStdout(redirect=True)");

	return m_pythonPanel;
}
#endif
