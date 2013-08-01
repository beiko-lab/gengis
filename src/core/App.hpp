//=======================================================================
// Author: Donovan Parks
//
// Copyright 2009 Donovan Parks
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

#ifndef _GENGIS_APP_
#define _GENGIS_APP_

#include "../core/Precompiled.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/MapLayer.hpp"
#include "../core/VectorMapLayer.hpp"

namespace GenGIS
{

	/**
	* @class App
	* @brief Singleton class for holding project specific settings and widely access system level objects.
	*/
	class App
	{
	public:

		/** @brief Program operating modes. */
		enum MODE {
			STANDARD,
			LAYOUT_LINE,
			LAYOUT_ELLIPSE,
			GEO_AXIS_POLYLINE,
			USER_LABEL
		};

		/** Before each new release of GenGIS:
		*   (1) Update the GENGIS_VERSION variable and add a new release entry (e.g., V2_07 below).
		*   (2) If changes to serialization have been made since the last release, update SAVE_VERSION
		*   to the current release version.
		*   The information is used by serialization to determine whether a session
		*   file is compatible with the current release.
		*/
		enum APP_VERSION {
			V2_00 = 0,
			V2_01 = 1,
			V2_02 = 2,
			V2_10 = 3,
			V2_11Pre1 = 4,
			V2_11Pre2 = 5,
			V2_11 = 6,
			//V2_12 = 7,
			//
			GENGIS_VERSION = V2_11,
			SAVE_VERSION   = V2_10 /* For serialization: The last version of GenGIS
			                          whose session file is compatible with this release. */
		};

		/** @brief Destructor */
		~App() { }

		/** @brief Get the one and only instance of this class. */
		static App& Inst()
		{
			static App app;
			return app;
		}

		/** 
		* @brief Set main application window. 
		* @param window Pointer to main application window.
		*/
		void SetMainWindow(wxFrame* window);

		/**
		* @brief Get main application window.
		* @returns A wxFrame pointer to the main window.
		*/
		wxFrame* GetMainWindow() const
		{
			return m_mainWindow;
		}

		/**
		* @brief Get wrapper exposing functionaity of the main application window.
		* @returns A MainWindowWrapper pointer to MainWindowWrapper.
		*/
		MainWindowWrapperPtr GetMainWindowWrapper() const 
		{ 
			return m_mainWindowWrapper; 
		}

		/**
		* @brief Get wrapper exposing functionality of the viewport.
		* @returns A ViewportWrapper pointer to ViewportWrapper.
		*/
		ViewportWrapperPtr GetViewportWrapper() const
		{
			return m_viewportWrapper;
		}

		/**
		* @brief Get wrapper providing access to user generated graphical elements.
		* @returns A UserElementsWrapper pointer to UserElementsWrapper.
		*/
		UserElementsWrapperPtr GetUserElementsWrapper() const
		{
			return m_userElementsWrapper;
		}

		/**
		* @brief Get map controller. Right now we are assuming there is only one map loaded. */
		MapControllerPtr GetMapController() const
		{
			if ( m_layerTreeController->GetNumMapLayers() == 0 )
				return boost::shared_ptr<MapController>();

			return m_layerTreeController->GetMapLayer(0)->GetMapController();
		}

		VectorMapControllerPtr GetVectorMapController(uint index) const
		{
			if ( m_layerTreeController->GetNumVectorMapLayers() == 0 )
				return boost::shared_ptr<VectorMapController>();
			else
			
				return m_layerTreeController->GetVectorMapLayer(index)->GetVectorMapController();
			
		}


		/** 
		* @brief Set OpenGL viewport.
		* @param viewport Smart pointer to OpenGL viewport
		*/
		void SetViewport(ViewportPtr viewport)
		{
			m_viewport = viewport;
		}

		/** @brief Get OpenGL viewport. */
		ViewportPtr GetViewport() const
		{
			return m_viewport;
		}

		/** Set controller to layer tree which gives access to all loaded layers. */

		/** 
		* @brief Set controller to layer tree which gives access to all loaded layers.
		* @param controller Smart pointer to layer tree controller.
		*/
		void SetLayerTreeController(LayerTreeControllerPtr controller)
		{
			m_layerTreeController = controller;
		}

		/** @brief Get controller to layer tree which gives access to all loaded layers. */
		LayerTreeControllerPtr GetLayerTreeController() const
		{
			return m_layerTreeController;
		}

		/** @brief Get study controller. */
		StudyControllerPtr GetStudyController() const;

		/** 
		* @brief Set the current program mode.
		* @param mode Desired program mode.
		*/
		void SetMode(MODE mode)
		{
			m_mode = mode;
		}

		/** @brief Get the current program mode. */
		MODE GetMode() const
		{
			return m_mode;
		}

		/** 
		* @brief Set the 3D position of the mouse in world (OpenGL) coordinates.
		* @param pt Position of mouse in world coordinates.
		*/
		void SetMouseWorldPos(const Point3D& pt)
		{
			m_mouseWorldPos = pt;
		}

		/** @brief Get the 3D position of the mouse in world (OpenGL) coordinates. */
		Point3D GetMouseWorldPos() const
		{
			return m_mouseWorldPos;
		}

		/** @brief Get the 2D position of the mouse in the plane of the terrain (y=0). */
		Point3D GetMouseTerrainPlanePos() const
		{
			return Point3D(m_mouseWorldPos.x, 0, m_mouseWorldPos.z);
		}

		/** 
		* @brief Set the 2D position of the mouse in screen coordinates.
		* @param pt Position of mouse in screen coordinates.
		*/
		void SetMouseScreenPos(const Point2D& pt)
		{
			m_mouseScreenPos = pt;
		}

		/** @brief Get the 2D position of the mouse in screen coordinates. */
		Point2D GetMouseScreenPos() const
		{
			return m_mouseScreenPos;
		}

		/** @brief Set flag indicating if the mouse is being dragged. */
		void SetMouseDragging(bool state)
		{
			m_mouseDragging = state;
		}

		/** @brief Get flag indicating if the mouse is being dragged. */
		bool GetMouseDragging() const
		{
			return m_mouseDragging;
		}

		/** @brief Get layout object properties. */
		LayoutObjectPropertiesPtr GetLayoutObjProp() const
		{
			return m_layoutObjProp;
		}

		/** Set currently selected view. */

		/** 
		* @brief Set currently selected view.
		* @param view Smart pointer to currently selected view.
		*/
		void SetSelectedView( ViewPtr view );

		/** @brief Get currently selected view. */
		ViewPtr GetSelectedView() const
		{
			return m_selectedView;
		}

		/** 
		* @brief Set currently active right-click view.
		* @param view Smart pointer to currently active right-click view.
		*/
		void SetRightClickView( ViewPtr view )
		{
			m_rightClickView = view;
		}

		/** @brief Get currently active right-click view. */
		ViewPtr GetRightClickView() const
		{
			return m_rightClickView;
		}

		/** @brief Get selection colour. */
		Colour GetSelectionColour() const;

		/** @brief Get selection thickness. */
		float GetSelectionThickness() const
		{
			return 2;
		}

		/** @brief Get directory containing program executable. */
		wxString GetExeDir() const;

		/** @brief Get directory containing user data for program. */
		wxString GetUserDataDir() const;

		/** @brief Save user preferences. */
		void SaveUserPreferences();

		/** @brief Load user preferences. */
		void LoadUserPreferences();

		/** @brief Get colour map manager. */
		ColourMapManagerPtr GetColourMapManager() const
		{
			return m_colourMapManager;
		}

		/** @brief Get shape map manager. */
		ShapeMapManagerPtr GetShapeMapManager() const
		{
			return m_shapeMapManager;
		}

		/** @brief Initialize OpenGL font. */
		void InitFont(const std::wstring& font);

		/** @brief Get OpenGL font. */
		FontPtr GetFont() const
		{
			return m_font;
		}

		/** @brief Get database connection string. */
		std::wstring GetDatabaseConnectionString() const
		{
			return m_dbConnectionString;
		}

		/** @brief Set database connection string. */
		void SetDatabaseConnectionString(const std::wstring& str)
		{
			m_dbConnectionString = str;
		}

		/** @brief Set resolution factor for saving high resolution images. */
		void SetResolutionFactor(uint factor)
		{
			m_resolutionFactor = factor;
		}

		/** @brief Get resolution factor for saving high resolution images. */
		uint GetResolutionFactor() const
		{
			return m_resolutionFactor;
		}

		/* @brief Set flag indicating if high resolution image is being saved. */
		void SetSavingHiResImage(bool state)
		{
			m_bRenderingHiResImage = state;
		}

		/* @brief Get flag indicating if high resolution image is being saved. */
		bool IsSavingHiResImage() const
		{
			return m_bRenderingHiResImage;
		}

		/** @brief Serialization: Get the full path (directory + filename) of the stored session file */
		wxString GetSessionFullPath() const
		{
			return m_sessionFullPath.GetPath();
		}

		/** @brief Serialization: Set the full path (directory + filename) of the stored session file */
		void SetSessionFullPath( wxString sessionFilePath );

		/** @brief Serialization: Get flag indicating whether serialization is currently being performed. */
		bool IsCurrentlySerializing()
		{
			return m_bCurrentlySerializing;
		}

		/** @brief Serialization: Set flag to indicate whether or not serialization is currently being performed. */
		void ToggleSerializationStatus()
		{
			m_bCurrentlySerializing = !m_bCurrentlySerializing;
		}

		/** @brief Serialization: Set flag to indicate that changes have been made since last save operation. */
		void SetSaveStatus( bool status );

		/** @brief Serialization: Get flag indicating whether or not changes have been made since last save operation. */
		bool IsSaveUpToDate()
		{
			return m_bSaveIsUpToDate;
		}
        
        /** @brief Determine whether the current session is empty or not. */
        bool IsNewSession()
        {
            return m_bIsEmptySession;
        }
        
        /** @brief Set flag to indicate that the current session is no longer empty. */
        void NoLongerANewSession()
        {
            m_bIsEmptySession = false;
        }

		/** @brief Returns the version of GenGIS. */
		wxString GetAppVersion()
		{
			return m_appVersion;
		}

	protected:
		/** @brief Private singleton constructor (must instantiate this class through Inst()). */
		App();
		
		/** @brief Private copy constructor (must instantiate this class through Inst()). */
		App(const App&);

		/** @brief Private assignment constructor (must instantiate this class through Inst()). */
		App& operator= (const App&);

	private:
		/* Serialization. */
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

	private:
		/** Pointer to main application window. */
		wxFrame* m_mainWindow;

		/** Wrapper exposing main functionality of application window via a smart pointer (needed for Python interface). */
		MainWindowWrapperPtr m_mainWindowWrapper;

		/** Pointer to OpenGL viewport. */
		ViewportPtr m_viewport;

		/** Wrapper exposing viewport functionality via a smart pointer (nedded for Python interface). */
		ViewportWrapperPtr m_viewportWrapper;

		/** Pointer to layer tree controller which holds all objects loaded into program. */
		LayerTreeControllerPtr m_layerTreeController;

		/** Wrapper providing access to user generated graphical elements. */
		UserElementsWrapperPtr m_userElementsWrapper;

		/** Current program mode. */
		MODE m_mode;

		/** The 3D position of the mouse in world (OpenGL) coordinates. */
		Point3D m_mouseWorldPos;

		/** The 2D position of the mouse in screen coordinates. */
		Point2D m_mouseScreenPos;

		/** Flag indicating if the mouse is currently being dragged. */
		bool m_mouseDragging;

		/** Visual properties for layout objects (e.g., line, ellipse). */
		LayoutObjectPropertiesPtr m_layoutObjProp;

		/** Currently selected view in OpenGL viewport. */
		ViewPtr m_selectedView;

		/** Currently active right-click view. */
		ViewPtr m_rightClickView;

		/** Colour map manager. */
		ColourMapManagerPtr m_colourMapManager;

		/** Shape map manager. */
		ShapeMapManagerPtr m_shapeMapManager;

		/** Font class used to render TrueType fonts on an OpenGL canvas. */
		FontPtr m_font;

		/** Database connection string. */
		std::wstring m_dbConnectionString;

		/** Sizing factor for saving high resolution image. */
		uint m_resolutionFactor;

		/** Flag indicating high resolution image is being rendered. */
		bool m_bRenderingHiResImage;
		
		/** Serialization: Full path (directory + filename) of the stored session file */
		wxFileName m_sessionFullPath;

		/** Serialization: Flag indicating whether serialization is currently being performed. */
		bool m_bCurrentlySerializing;

		/** Serialization: Flag indicating whether changes have been made since the last save operation. */
		bool m_bSaveIsUpToDate;
        
        /** Flag indicating whether the current session is empty. */
        bool m_bIsEmptySession;

		/** String containing the version of GenGIS. */
		wxString m_appVersion;
	};
}

BOOST_CLASS_VERSION( App, App::SAVE_VERSION )

#endif
