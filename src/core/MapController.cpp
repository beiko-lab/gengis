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

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"
#include "../core/MapController.hpp"
#include "../core/Camera.hpp"
#include "../core/MapView.hpp"

#include "../utils/ErrorGL.hpp"
#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/glUtils.hpp"

#include "../core/Viewport.hpp"

using namespace GenGIS;

template<class Archive>
void MapController::serialize(Archive & ar, const unsigned int file_version)
{
	ar & m_mapModel;   // MapModelPtr
	ar & m_mapView;    // MapViewPtr
	ar & m_bMapLoaded; // bool
}
template void MapController::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void MapController::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

/** Project screen point onto terrain. */
Point3D MapController::ProjectToTerrain(const Point2D& screenPt, bool bInvertY)
{
	GLdouble* modelView = App::Inst().GetViewport()->GetModelViewMatrix();
	GLdouble* projection = App::Inst().GetViewport()->GetProjectionMatrix();
	GLint* viewport = App::Inst().GetViewport()->GetViewportDimensions();

	// Need depth buffer to just contain information from terrain, so we quickly 
	// render it
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		App::Inst().GetViewport()->GetCamera()->ApplyExtrinsicParams();

		// render the terrain
		GetMapView()->Render(true);

		float winX = screenPt.x;
		float winY;
		if(bInvertY)
			winY = (float)viewport[3] - (float) screenPt.y;
		else
			winY = screenPt.y;

		// get "depth" of mouse cursor
		GLfloat winZ;
		glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
		
		// (un)project mouse cursor into model space
		GLdouble posX, posY, posZ;
		if(glUtils::UnProject4(winX, winY, winZ,
			                   modelView, projection, viewport, TERRAIN_START_DEPTH, 1.0f,
			                   &posX, &posY, &posZ) == GL_FALSE)
		{
			return Point3D(-1.0f, -1.0f, -1.0f);
		}

	glPopMatrix();

	return Point3D(posX, posY, posZ);
}

/** Project 3D point to screen. */
Point3D MapController::ProjectToScreen(const Point3D& worldPt)
{
	GLdouble* modelView = App::Inst().GetViewport()->GetModelViewMatrix();
	GLdouble* projection = App::Inst().GetViewport()->GetProjectionMatrix();
	GLint* viewport = App::Inst().GetViewport()->GetViewportDimensions();

	GLdouble winX = 0, winY = 0, winZ = 0;
	gluProject(worldPt.x, worldPt.y, worldPt.z, modelView, projection, viewport, &winX, &winY, &winZ );

	return Point3D(winX, winY, winZ);
}

bool MapController::IsWireFrame() 
{ 
	return m_mapView->IsWireFrame(); 
}

void MapController::SetWireFrame(bool state) 
{ 
	m_mapView->SetWireFrame(state); 
}

//void MapController::SetVerticalExaggeration(float exaggeration)
//{
//	//m_mapView->SetVerticalExaggeration(this, exaggeration);
//}

//float MapController::GetVerticalExaggeration() const
//{
//	return m_mapModel->GetVerticalExaggeration();	
//}
