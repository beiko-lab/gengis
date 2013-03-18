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

#ifndef _GENGIS_VIEWPORT_WRAPPER_
#define _GENGIS_VIEWPORT_WRAPPER_

#include "../core/Precompiled.hpp"

#include "../core/Viewport.hpp"

#include "../utils/Colour.hpp"

namespace GenGIS
{

	/**
	* @brief Wrapper viewport functionality that should be exposed to Python interface.
	*/
	class ViewportWrapper : private Uncopyable 
	{
	public:
		explicit ViewportWrapper() {}
		~ViewportWrapper() {}

		/** Refresh the OpenGL scene. */
		void Refresh()
		{
			App::Inst().GetViewport()->Refresh(false);
		}

		/**
		* @brief Save current scene to an image file.
		* @param filename File to save image to.
		*/
		void SaveImage(const std::string& filename)
		{
			App::Inst().GetViewport()->SaveImage(filename);
		}

		/** Get lighting associated with viewport. */
		LightingPtr GetLighting() const
		{
			return App::Inst().GetViewport()->GetLighting();
		}

		/** Set background colour. */
		void SetBackgroundColour(const Colour& colour)
		{
			App::Inst().GetViewport()->SetBackgroundColour(colour);
		}

		/** Get background colour. */
		Colour GetBackgroundColour() const
		{
			return App::Inst().GetViewport()->GetBackgroundColour();
		}

		/** Get width of viewport.*/
		int GetWidth() const
		{
			return App::Inst().GetViewport()->GetWidth();
		}

		/** Get height of viewport. */
		int GetHeight() const
		{
			return App::Inst().GetViewport()->GetHeight();
		}
	};
}

#endif
