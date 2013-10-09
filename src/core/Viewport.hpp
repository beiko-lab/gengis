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

#ifndef _GENGIS_VIEWPORT_
#define _GENGIS_VIEWPORT_

#include "../core/Precompiled.hpp"
#include "../utils/Colour.hpp"
#include "../gui/GenGisFrame.hpp"

namespace GenGIS
{
	/**
	* @class Viewport
	* @brief Provides access to an OpenGL canvas.
	*/
	class Viewport : public wxGLCanvas, private Uncopyable 
	{
	public:
		/** Function signature for render signal. */
		typedef boost::signal<void ()> SigRender;

		/** Function signature for mouse signal. */
		typedef boost::signal<void (wxMouseEvent& event)> SigMouse;

		/** Function signature for mouse signal. */
		typedef boost::signal<void (wxMouseEvent& event)> SigMouseWheel;

		/** Function signature for keyboard signal. */
		typedef boost::signal<void (wxKeyEvent& event)> SigKeyboard;

		/** Constructor. */
		explicit Viewport(wxWindow *parent,
			const wxWindowID id  = -1,
			const wxPoint& pos   = wxDefaultPosition,
			const wxSize& size   = wxDefaultSize,
			long  style          = wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS,
			const wxString& name = wxT("Viewport"));

		/** Destructor. */
		~Viewport();

		/** 
		* @brief Render the OpenGL scene. 
		*/
		void Render();

		/** Indicate that the viewport needs to be redrawn immediately. */
		void RenderNow()
		{
			Refresh(false);
			Update();
		}

		/** Disable viewport. */
		bool& Disable()
		{
			return m_bDisabled;
		}

		/** Disable viewport (read-only). */
		const bool& Disable() const
		{
			return m_bDisabled;
		}
		void SetDisable(bool disable)
		{
			m_bDisabled = disable;
		}

		/**
		* @brief Set a slot (callback) to be signalled when rendering occurs.
		* @param slot Slot (function) to call.
		*/
		void SignalRender(const SigRender::slot_type& slot)
		{
			m_sigRender.connect(slot);
		}

		/**
		* @brief Set a slot (callback) to be signalled when a mouse events occurs.
		* @param slot Slot (function) to call.
		*/
		void SignalMouse(const SigMouse::slot_type& slot)
		{
			m_mouseConnection = m_sigMouse.connect(slot);
		}

		/**
		*	@brief Disconnect method for slot(calllback) from mouse events.
		*/
		void UnRegisterMouse()
		{
			m_mouseConnection.disconnect();
		}

		/**
		* @brief Set a slot (callback) to be signalled when mouse wheel event occurs.
		* @param slot Slot (function) to call.
		*/
		void SignalMouseWheel(const SigMouseWheel::slot_type& slot)
		{
			m_sigMouseWheel.connect(slot);
		}

		/**
		* @brief Set a slot (callback) to be signalled when a keyboard event occurs.
		* @param slot Slot (function) to call.
		*/
		void SignalKeyboard(const SigKeyboard::slot_type& slot)
		{
			m_sigKeyboard.connect(slot);
		}

		/**
		* @brief Save current scene to an image file.
		*
		* @param filename File to save image to.
		*/
		void SaveImage(const std::string& filename);

		/** Get camera associated with viewport. */
		CameraPtr GetCamera() const
		{
			return m_camera;
		}

		/** Get lighting associated with viewport. */
		LightingPtr GetLighting() const
		{
			return m_lighting;
		}

		/** Set background colour. */
		void SetBackgroundColour(const Colour& colour);

		/** Get background colour. */
		Colour GetBackgroundColour() const
		{
			return m_bgColour;
		}

		/** Get width of viewport.*/
		int GetWidth() const
		{
			return m_width;
		}

		/** Get height of viewport. */
		int GetHeight() const
		{
			return m_height;
		}

		/** Get the OpenGL model view matrix of the viewport. */
		GLdouble* GetModelViewMatrix()
		{
			return m_modelView;
		}

		/** Get the OpenGL projection matrix of the viewport. */
		GLdouble* GetProjectionMatrix()
		{
			return m_projection;
		}

		/** Get the OpenGL viewport array of the viewport. */
		GLint* GetViewportDimensions()
		{
			return m_viewport;
		}

		void OnCharFromKeyboard(wxKeyEvent& event);

		/** Custom serialization function for the viewport */
		template<class Archive>
		void ViewportSerialize(Archive & ar, const unsigned int file_version);

		/** Set max terrain resolution. */
		void SetMaxTerrainResolution( int resolution )
		{
			m_maxTerrainResolution = resolution;
		}

		/** Get max terrain resolution. */
		int GetMaxTerrainResolution() const
		{
			return m_maxTerrainResolution;
		}

	protected:
		/** 
		* @brief Render the OpenGL scene. Called by Render() and OnPaint() which 
		*				set up the appropriate device context. 
		*/
		void RenderScene();

		/** Setup OpenGL canvas */
		void InitCanvas();

		/** Connect all events handled by the viewport. */
		void ConnectEvents();

		/** Disconnect all events handled by the viewport. */
		void DisconnectEvents();

		/** Handle the size event which is sent when a window is resized. */
		void OnSize(wxSizeEvent& event);

		/** Handle the window enter event which is sent what the mouse first
		moves over a window. */
		void OnEnterWindow(wxMouseEvent& WXUNUSED(event));

		/** Handle mouse move and button down events (translation + zooming). */
		void OnMouse(wxMouseEvent& event);

		/** Handle left click (suggestion: viewport dependent). */
		void OnMouseLeftClick(wxMouseEvent& event) {}

		/** Handle mouse wheel events (suggestion: translation). */
		void OnMouseWheel(wxMouseEvent& event);

		/** Handle the erase event for the viewport panel which is generated
		whenever a window's background needs to be repainted. */
		void OnEraseBackground(wxEraseEvent &WXUNUSED(event));

		/** Handle the paint event which is sent when a window's contents 
		needs to be repainted. */
		void OnPaint(wxPaintEvent& event);

		/**
		* @brief Save current scene as raster PPM file.
		*/
		void SavePPM(const std::string& filename, ProgressDlgPtr progressDlg);

		/**
		* @brief Save current scene as raster PNG file.
		*/
		void SavePNG(const std::string& filename, ProgressDlgPtr progressDlg);

	protected:

		/** Camera associated with the OpenGL viewport. */
		CameraPtr m_camera;

		/** Lighting associated with the OpenGL viewport. */
		LightingPtr m_lighting;

		/** Viewport width and height. */
		int m_width, m_height;

		/** Flag indicating if viewport is disabled. */
		bool m_bDisabled;

		/** Signal that viewport is being rendered. */
		SigRender m_sigRender;

		/** Signal that mouse have moved within viewport. */
		SigMouse m_sigMouse;

		boost::signals::connection m_mouseConnection;

		/** Signal that mouse have moved within viewport. */
		SigMouseWheel m_sigMouseWheel;

		/** Signal that keyboard button has been pressed within viewport. */
		SigKeyboard m_sigKeyboard;

		/** OpenGL model view matrix. */
		GLdouble m_modelView[16];

		/** OpenGL projection matrix. */
		GLdouble m_projection[16];

		/** OpenGL viewport. */
		GLint m_viewport[4];

		/** Background colour of viewport. */
		Colour m_bgColour;

		/** Resolution of the map. */
		int m_maxTerrainResolution;
	};
}

#endif

