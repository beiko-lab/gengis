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

#include "../widgets/WidgetController.hpp"

#include "../core/App.hpp"
#include "../core/Camera.hpp"
#include "../widgets/Texture.hpp"

#include "../widgets/ViewWidget.hpp"
#include "../widgets/MiniMapView2D.hpp"
#include "../widgets/NavView2D.hpp"
#include "../widgets/CompassView2D.hpp"

#include "../core/Viewport.hpp"

using namespace GenGIS;

WidgetController::WidgetController(ViewportPtr viewport) :
	m_viewport(viewport), m_compass(new CompassView2D()),
	m_compassDecoration(new ViewWidget()), m_nav(new NavView2D()),
	m_miniMap(new MiniMapView2D()), m_miniMapDecoration(new ViewWidget())
{
	// render widgets in specified viewport
	m_viewport->SignalRender(boost::bind(&WidgetController::RenderWidgets, this));

	// initialize widgets
	ViewCompass();
	ViewNavControl();
	ViewMiniMap();
}

void WidgetController::SetCompassAngle(float angle) 
{ 	
	m_compass->SetAngle(angle); 		
}

void WidgetController::ToggleCompassVisibility()
{
	m_compass->SetVisibility(!m_compass->IsVisible());
	m_compassDecoration->SetVisibility(!m_compassDecoration->IsVisible());

	m_viewport->Refresh(false);
}

void WidgetController::SetCompassVisibility(bool bVisible)
{
	m_compass->SetVisibility(bVisible);
	m_compassDecoration->SetVisibility(bVisible);

	m_viewport->Refresh(false);
}

void WidgetController::ToggleMiniMapVisibility()
{
	m_miniMap->SetVisibility(!m_miniMap->IsVisible());
	m_miniMapDecoration->SetVisibility(!m_miniMapDecoration->IsVisible());

	m_viewport->Refresh(false);
}

void WidgetController::SetMiniMapVisibility(bool bVisible)
{
	m_miniMap->SetVisibility(bVisible);
	m_miniMapDecoration->SetVisibility(bVisible);

	m_viewport->Refresh(false);
}

void WidgetController::ToggleNavVisibility()
{
	m_nav->SetVisibility(!m_nav->IsVisible());

	m_viewport->Refresh(false);
}

void WidgetController::SetNavVisibility(bool bVisible)
{
	m_nav->SetVisibility(bVisible);

	m_viewport->Refresh(false);
}


void WidgetController::ViewNavControl()
{
	m_nav->SetScale( 50, 50 );

	wxImage image(wxString(App::Inst().GetExeDir() + wxT("images/navigate_40.png")), wxBITMAP_TYPE_PNG);
	SetNavImages(image.GetData(), image.GetAlpha(), image.GetWidth(), image.GetHeight());
}

void WidgetController::SetNavImages(unsigned char* data, unsigned char* alphaData, int width, int height) 
{    
	GLubyte* imageData = (GLubyte*)malloc( sizeof( GLubyte ) * width * height * 4 );

	int dataIndex = 0;
	int j = 0;

	for (int i = 0; i < width * height; i++) 
	{	    
		imageData[ j ] = ( GLubyte )data[dataIndex];
		imageData[ j + 1 ] = ( GLubyte )data[dataIndex + 1];
		imageData[ j + 2 ] = ( GLubyte )data[dataIndex + 2];
		imageData[ j + 3 ] = ( GLubyte )alphaData[i];

		j += 4;
		dataIndex += 3;
	}

	TexturePtr texture( new Texture( imageData, width, height, 256, 256 ) );
	m_nav->SetTexture( texture );
	Point3D scale = m_nav->GetScale();
	m_nav->SetScale( scale.x, 0.50776053215f * scale.y );

	m_nav->SetZoomInOrigin( -0.37250554324f, -0.67248908296f );
	m_nav->SetZoomOutOrigin( 0.31707317074f, -0.67248908296f );
	m_nav->SetZoomInRadii( 0.12195121952f, 0.24017467248f );
	m_nav->SetZoomOutRadii( 0.11973392462f, 0.24017467248f );

	m_nav->SetLeftArrowOrigin( -0.25250554324f, 0.1f );
	m_nav->SetRightArrowOrigin( 0.25207317074f, 0.1f );
	m_nav->SetLeftArrowRadii( 0.12195121952f, 0.24017467248f );
	m_nav->SetRightArrowRadii( 0.11973392462f, 0.24017467248f );

	m_nav->SetUpArrowOrigin( 0.0f, 0.67248908296f );
	m_nav->SetDownArrowOrigin( 0.0f, -0.33248908296f );
	m_nav->SetUpArrowRadii( 0.12195121952f, 0.24017467248f );
	m_nav->SetDownArrowRadii( 0.11973392462f, 0.24017467248f );
}

void WidgetController::ViewCompass()
{
	m_compass->SetScale( 50, 50 );
	m_compass->SetAngle( m_viewport->GetCamera()->GetYaw() );

	m_compassDecoration->SetScale( 50, 50 );

	wxImage image(wxString(App::Inst().GetExeDir() + wxT("images/compass_40.png")), wxBITMAP_TYPE_PNG);
	wxImage decorImage(wxString(App::Inst().GetExeDir() + wxT("images/compass_grey_40.png")), wxBITMAP_TYPE_PNG);

	SetCompassImages(
		image.GetData(), image.GetAlpha(), image.GetWidth(),
		image.GetHeight(),
		decorImage.GetData(), decorImage.GetAlpha(), decorImage.GetWidth(),
		decorImage.GetHeight()
		);
}

void WidgetController::SetCompassImages( unsigned char* data, unsigned char* alphaData, int width, int height,
										unsigned char* data2, unsigned char* alphaData2, int width2, int height2 ) 
{      
	// actual compass view
	GLubyte* imageData = (GLubyte*)malloc( sizeof( GLubyte ) * width * height * 4 );

	int dataIndex = 0;
	int j = 0;

	for( int i = 0; i < width * height; i++ ) 
	{	    
		imageData[ j ] = ( GLubyte )data[ dataIndex ];
		imageData[ j + 1 ] = ( GLubyte )data[dataIndex + 1 ];
		imageData[ j + 2 ] = ( GLubyte )data[dataIndex + 2 ];
		imageData[ j + 3 ] = ( GLubyte )alphaData[ i ];

		j += 4;
		dataIndex += 3;
	}

	TexturePtr texture = TexturePtr( new Texture( imageData, width, height, 256, 256 ) );
	m_compass->SetTexture( texture );
	m_compass->SetPivotPoint( -0.0066518847f, -0.0044345898f );
	m_compass->SetRadii( 0.9578713969f,	0.955654102f );

	GLubyte* imageData2 = (GLubyte*)malloc( sizeof( GLubyte ) * width2 * height2 * 4 );

	dataIndex = 0;
	j = 0;
	int i = 0;

	for(int h = 0; h < height2; ++h)
	{
		for(int w = 0; w < width2; ++w)
		{
			imageData2[ j ] = ( GLubyte )data2[dataIndex];
			imageData2[ j + 1 ] = ( GLubyte )data2[dataIndex + 1];
			imageData2[ j + 2 ] = ( GLubyte )data2[dataIndex + 2];

			// Hack: this is a complete hack. There is a problem with the image
			// that causes a line near the bottom of the image to appear. The image
			// itself seems fine, so I put in this hack to fix the problem. Notice that
			// the "bottom" of the image will be contain in the "top" of the array.
			if(h < 1)
				imageData2[ j + 3 ] = ( GLubyte )0;
			else
				imageData2[ j + 3 ] = ( GLubyte )alphaData2[i];

			j += 4;
			dataIndex += 3;
			i++;
		}
	}

	texture = TexturePtr( new Texture( imageData2, width, height, 256, 256 ) );
	m_compassDecoration->SetTexture( texture );
}

void WidgetController::ViewMiniMap()
{
	m_miniMap->SetScale( 50, 50 );  
	m_miniMapDecoration->SetScale( 50, 50 );

	wxImage image(wxString(App::Inst().GetExeDir() + wxT("images/minimap_grey_40.png")), wxBITMAP_TYPE_PNG);
	SetMiniMapImages(image.GetData(), image.GetAlpha(), image.GetWidth(), image.GetHeight());
}

void WidgetController::SetMiniMapImages(unsigned char* data, unsigned char* alphaData, int width, int height) 
{
	GLubyte* imageData = (GLubyte*)malloc( sizeof( GLubyte ) * width * height * 4 );

	int dataIndex = 0;
	int j = 0;

	for ( int i = 0; i < width * height; i++ ) 
	{      
		imageData[ j ] = ( GLubyte )data[dataIndex];
		imageData[ j + 1 ] = ( GLubyte )data[dataIndex + 1];
		imageData[ j + 2 ] = ( GLubyte )data[dataIndex + 2];
		imageData[ j + 3 ] = ( GLubyte )alphaData[i];

		j += 4;
		dataIndex += 3;
	}

	TexturePtr texture( new Texture( imageData, width, height, 256, 256 ) );

	m_miniMapDecoration->SetTexture( texture );

	m_miniMap->SetOrigin( -0.0155210643f, -0.00294117648f );

	Point3D scale =  m_miniMap->GetScale();      
	m_miniMap->SetScale( 0.89578713969f * scale.x, 0.85588235294f * scale.y );
}

void WidgetController::RenderWidgets()
{
	//switch to 2D orthographic projection
	m_viewport->GetCamera()->SetOrthoCamera();

	glPolygonMode(GL_FRONT, GL_FILL);

	if(m_miniMap && m_nav && m_viewport->GetCamera())
	{
		const int WIDGET_OFFSET = 10;

		Point3D coords;
		Point3D scale;
		float yOffset = WIDGET_OFFSET;

		// draw navigation control
		if(m_nav->IsVisible() && !App::Inst().IsSavingHiResImage())
		{
			scale = m_nav->GetScale();	

			coords.x = m_viewport->GetWidth() - scale.x - WIDGET_OFFSET;
			coords.y = m_viewport->GetHeight() - scale.y - yOffset;
			coords.z = 1.0;
			m_nav->SetScreenCoords( coords );

			m_nav->Render();

			yOffset += 2*scale.y;
		}


		// draw minimap
		if(m_miniMap->IsVisible() && !App::Inst().IsSavingHiResImage())
		{
			scale = m_miniMapDecoration->GetScale();

			coords.x = m_viewport->GetWidth() - scale.x - WIDGET_OFFSET;
			coords.y = m_viewport->GetHeight() - scale.y - yOffset;
			coords.z = 1.0;
			m_miniMapDecoration->SetScreenCoords( coords );			

			coords.x = coords.x + scale.x * m_miniMap->GetOrigin().x;
			coords.y = coords.y + scale.y * m_miniMap->GetOrigin().y;
			coords.z = 0.9f;
			m_miniMap->SetScreenCoords( coords );

			m_miniMap->Render();
			m_miniMapDecoration->Render();

			yOffset += 2*scale.y;
		}

		// draw compass control
		if(m_compass->IsVisible() && !App::Inst().IsSavingHiResImage())
		{
			scale = m_compass->GetScale();

			coords.x = m_viewport->GetWidth() - scale.x - WIDGET_OFFSET;
			coords.y = m_viewport->GetHeight() - scale.y - yOffset;
			coords.z = 0.9f;
			m_compass->SetScreenCoords(coords);

			coords.z = 1.0f;
			m_compassDecoration->SetScreenCoords(coords);

			m_compass->Render();
			m_compassDecoration->Render();

			yOffset += 2*scale.y;
		}
	}

	m_viewport->GetCamera()->UnsetOrthoCamera();
}

bool WidgetController::MouseLeftDown(const Point2D& mousePt)
{
	if(m_nav->MouseLeftDown(mousePt))
	{
		App::Inst().SetSelectedView(m_nav);
		return true;
	}

	if(m_miniMap->MouseLeftDown(mousePt))
	{
		App::Inst().SetSelectedView(m_miniMap);
		return true;
	}

	if(m_compass->MouseLeftDown(mousePt))
	{
		App::Inst().SetSelectedView(m_compass);
		return true;
	}

	return false;
}

bool WidgetController::MouseRightDown(const Point2D& mousePt, wxMenu& popupMenu)
{
	return false;
}
