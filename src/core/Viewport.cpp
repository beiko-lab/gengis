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
#include "../core/Viewport.hpp"
#include "../core/Camera.hpp"
#include "../core/Lighting.hpp"

#include "../utils/ErrorGL.hpp"
#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/ImageIO.hpp"

#include "../gui/ProgressDlg.hpp"

#include "../utils/tr.hpp"

using namespace GenGIS;

Viewport::Viewport( wxWindow *parent, wxWindowID id,
                    const wxPoint& pos, const wxSize& size, long style, const wxString& name ):
	wxGLCanvas( parent, id, pos, size, style, name ),
	m_bDisabled( false ),
	m_bgColour( 1.0f, 1.0f, 1.0f, 1.0f ),
	m_maxTerrainResolution( 2048 )
{
	m_camera = CameraPtr( new Camera( m_width*1.0f / m_height ) );
	m_lighting = LightingPtr( new Lighting() );
	Viewport::ConnectEvents();

	InitCanvas();
}

Viewport::~Viewport()
{
	DisconnectEvents();
}

template<class Archive>
void Viewport::ViewportSerialize(Archive & ar, const unsigned int file_version)
{
	ar & m_camera;               // CameraPtr
	ar & m_lighting;             // LightingPtr
	ar & m_bDisabled;            // bool
	ar & m_modelView;            // GLdouble
	ar & m_projection;           // GLdouble
	ar & m_viewport;             // GLint
	ar & m_bgColour;             // Colour
	ar & m_maxTerrainResolution; // int

	if ( Archive::is_loading::value )
	{
		InitCanvas();

		m_camera->SetAspectRatio(m_width, m_height);	
		glViewport(0, 0, m_width, m_height);
	}
}
template void Viewport::ViewportSerialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int file_version); 
template void Viewport::ViewportSerialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int file_version);

void Viewport::ConnectEvents()
{
	this->Connect( wxEVT_CHAR, wxKeyEventHandler( Viewport::OnCharFromKeyboard ) );

	this->Connect( wxEVT_SIZE, wxSizeEventHandler( Viewport::OnSize ) );

	this->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( Viewport::OnEnterWindow ) );

	this->Connect( wxEVT_LEFT_DOWN,   wxMouseEventHandler( Viewport::OnMouse ) );
	this->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( Viewport::OnMouse ) );
	this->Connect( wxEVT_RIGHT_DOWN,  wxMouseEventHandler( Viewport::OnMouse ) );
	this->Connect( wxEVT_LEFT_UP,     wxMouseEventHandler( Viewport::OnMouse ) );
	this->Connect( wxEVT_RIGHT_UP,    wxMouseEventHandler( Viewport::OnMouse ) );
	this->Connect( wxEVT_MOTION,      wxMouseEventHandler( Viewport::OnMouse ) );
	this->Connect( wxEVT_MOUSEWHEEL,  wxMouseEventHandler( Viewport::OnMouseWheel ) );

	this->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( Viewport::OnEraseBackground ) );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( Viewport::OnPaint ) );
}

void Viewport::DisconnectEvents()
{
	this->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Viewport::OnCharFromKeyboard ) );

	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( Viewport::OnSize ) );

	this->Disconnect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( Viewport::OnEnterWindow ) );

	this->Disconnect( wxEVT_LEFT_DOWN,  wxMouseEventHandler( Viewport::OnMouse ) );
	this->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( Viewport::OnMouse ) );
	this->Disconnect( wxEVT_LEFT_UP,    wxMouseEventHandler( Viewport::OnMouse ) );
	this->Disconnect( wxEVT_RIGHT_UP,   wxMouseEventHandler( Viewport::OnMouse ) );
	this->Disconnect( wxEVT_MOTION,     wxMouseEventHandler( Viewport::OnMouse ) );
	this->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( Viewport::OnMouseWheel ) );

	this->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( Viewport::OnEraseBackground ) );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( Viewport::OnPaint ) );
}

void Viewport::OnSize(wxSizeEvent& event)
{
	SetCurrent();
	error::ErrorGL::Check();

	if(!m_bDisabled)
	{
		// this is also necessary to update the context on some platforms
		wxGLCanvas::OnSize(event);

		GetClientSize(&m_width, &m_height);

		m_camera->SetAspectRatio(m_width, m_height);  

		glViewport(0, 0, m_width, m_height);

		Refresh(false);
	}

	error::ErrorGL::Check();
}

void Viewport::OnPaint(wxPaintEvent& event)
{
	if(!m_bDisabled)
	{
		// Must be called within an OnPaint() event.
		wxPaintDC(this);	

		RenderScene();
	}
}

void Viewport::Render()
{
	if(!m_bDisabled)
	{
		// Must be called when painting to a window outside of an OnPaint() event.
		wxClientDC(this);

		RenderScene();
	}
}

void Viewport::OnEraseBackground(wxEraseEvent &WXUNUSED(event))
{
	// keep empty to reduce flicker
}

void Viewport::InitCanvas()
{
	SetCurrent();
	error::ErrorGL::Check();	

	// get GL viewport dimensions
	GetClientSize(&m_width, &m_height);

	// setup antialiasing and blending to optimize appearance
	glShadeModel(GL_SMOOTH);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	// glEnable(GL_POLYGON_SMOOTH); // for some reason this causes visual artifacts
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// compute the normals
	glEnable(GL_NORMALIZE);

	// depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);

	glClearColor(m_bgColour.GetRed(), m_bgColour.GetGreen(), m_bgColour.GetBlue(), 1.0);

	// Model settings
	//glEnable(GL_CULL_FACE); // better to keep this off since the user can set the camera in a position where 
	glCullFace(GL_BACK);      // the bottom of the map is visible

	const GLubyte* vendor;
	vendor = glGetString(GL_VENDOR); 

	// report basic graphics card information to user
	std::string vendorInfo = "Graphics card vendor: " + std::string(reinterpret_cast<const char*>(vendor));
	Log::Inst().Write(vendorInfo);
	Log::Inst().Write("");

	error::ErrorGL::Check();
}

void Viewport::RenderScene()
{
	SetCurrent();

	error::ErrorGL::Check();
	
	glDisable(GL_LIGHTING);	// Crazy hack that appears to fix a lighting issue under OS X 10.6.8

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// move camera to desired position
	m_camera->ApplyExtrinsicParams();

	// setup lighting for viewport
	m_lighting->SetupLighting();

	// save commonly used OpenGL matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, m_projection);
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	// signal that a render event has occured
	m_sigRender();

	if(!App::Inst().IsSavingHiResImage())
		SwapBuffers();

	error::ErrorGL::Check();
}

void Viewport::OnEnterWindow( wxMouseEvent& WXUNUSED(event) )
{
	//SetFocus();
}

void Viewport::OnMouse(wxMouseEvent& event)
{
	SetCurrent();
	m_sigMouse(event);
}

void Viewport::OnMouseWheel(wxMouseEvent& event)
{
	SetCurrent();
	m_sigMouseWheel(event);
}

void Viewport::SaveImage(const std::string& filename)
{
	Log::Inst().Write("Saving image...");

	SetCurrent();
	error::ErrorGL::Check();

	std::string extension = filename.substr(filename.find_first_of('.')+1, std::string::npos);

	ProgressDlgPtr progressDlg(new ProgressDlg(_T("Saving image"), _T("Saving high resolution image..."), 101));
	if(extension == "ppm")
		SavePPM(filename, progressDlg);
	else
		SavePNG(filename, progressDlg);

	// reset size of viewport
	GetClientSize(&m_width, &m_height);
	m_camera->SetAspectRatio(m_width, m_height);	
	glViewport(0, 0, m_width, m_height);

	Log::Inst().Write("Image saved.");
	Log::Inst().Write("");

	progressDlg->Update(101);

	error::ErrorGL::Check();
}

void Viewport::SavePNG(const std::string& filename, ProgressDlgPtr progressDlg)
{
	int width, height;
	GetClientSize(&width, &height);

	// make size a multiple of 4
	int widthAdj = width % 4;
	int heightAdj = height % 4;

	if(widthAdj != 0)
		width -= widthAdj;

	if(heightAdj != 0)
		height -= heightAdj;
	m_camera->SetAspectRatio(width, height);	

	// setup tiling
	const int TILE_WIDTH = width;
	const int TILE_HEIGHT = height;
	const int TILE_BORDER = 0;
	const int TILE_FACTOR = App::Inst().GetResolutionFactor();

	const int IMAGE_WIDTH = width * TILE_FACTOR;
	const int IMAGE_HEIGHT = height * TILE_FACTOR;

	TRcontext *tr;
	GLubyte *tile;

	// allocate buffer large enough to store entire image
	unsigned char* imageBuffer = new unsigned char[IMAGE_WIDTH*IMAGE_HEIGHT*4];
	if(imageBuffer == NULL)
	{
		Log::Inst().Error("(Error) Viewport::SaveImage(): failed to allocate image buffer."); 
		return;
	}

	// allocate buffer large enough to store one tile 
	tile = (GLubyte*) malloc(TILE_WIDTH * TILE_HEIGHT * 3 * sizeof(GLubyte));
	if (!tile) 
	{
		printf("Malloc of tile buffer failed!\n");
		return;
	}

	// set each tile to TILE_WIDTH x TILE_HEIGHT pixels 
	tr = trNew();
	trTileSize(tr, TILE_WIDTH, TILE_HEIGHT, TILE_BORDER);
	trTileBuffer(tr, GL_RGB, GL_UNSIGNED_BYTE, tile);
	trImageSize(tr, IMAGE_WIDTH, IMAGE_HEIGHT);
	trRowOrder(tr, TR_TOP_TO_BOTTOM);

	trPerspective(tr, m_camera->GetFOV(), m_camera->GetAspectRatio(), m_camera->GetNearViewingPlane(), m_camera->GetFarViewingPlane());

	// Set GL_PACK_ALIGNMENT to 1 if the image width is not a multiple of 4.
	// However, some NVIDIA card seem to fail on this setting which is why we 
	// froce the image width to be a multiple of 4 above.
	glPixelStorei(GL_PACK_ALIGNMENT, 4);

	// Render each tiles
	int moreTiles = 1;
	int rowNum = 0;
	int tileNum = 0;

	while(moreTiles) 
	{	
		trBeginTile(tr);
		int curColumn = trGet(tr, TR_CURRENT_COLUMN);
		RenderScene();     
		moreTiles = trEndTile(tr);

		// save tile into image buffer
		for(int y=0; y < TILE_HEIGHT; ++y)
		{
			for(int x=0; x < TILE_WIDTH; ++x)
			{
				uint red = tile[x*3 + y*TILE_WIDTH*3];
				uint green = tile[x*3+1 + y*TILE_WIDTH*3];
				uint blue = tile[x*3+2 + y*TILE_WIDTH*3];

				imageBuffer[curColumn*TILE_WIDTH*4 + (rowNum)*TILE_HEIGHT*TILE_WIDTH*TILE_FACTOR*4 + x*4 + (TILE_HEIGHT-y-1)*IMAGE_WIDTH*4] = red;
				imageBuffer[curColumn*TILE_WIDTH*4 + (rowNum)*TILE_HEIGHT*TILE_WIDTH*TILE_FACTOR*4 + x*4+1 + (TILE_HEIGHT-y-1)*IMAGE_WIDTH*4] = green;
				imageBuffer[curColumn*TILE_WIDTH*4 + (rowNum)*TILE_HEIGHT*TILE_WIDTH*TILE_FACTOR*4 + x*4+2 + (TILE_HEIGHT-y-1)*IMAGE_WIDTH*4] = blue;
				imageBuffer[curColumn*TILE_WIDTH*4 + (rowNum)*TILE_HEIGHT*TILE_WIDTH*TILE_FACTOR*4 + x*4+3 + (TILE_HEIGHT-y-1)*IMAGE_WIDTH*4] = 255;
			}
		}

		if(curColumn == trGet(tr, TR_COLUMNS)-1) 
			rowNum++;

		tileNum++;
		progressDlg->Update(float(tileNum) / (TILE_FACTOR*TILE_FACTOR) * 100.0f);
	}

	trDelete(tr);
	free(tile);

	ImageIO::WritePNG(filename, imageBuffer, IMAGE_WIDTH, IMAGE_HEIGHT);

	delete[] imageBuffer;
}

void Viewport::SavePPM(const std::string& filename, ProgressDlgPtr progressDlg)
{
	int width, height;
	GetClientSize(&width, &height);

	// make size a multiple of 4
	int widthAdj = width % 4;
	int heightAdj = height % 4;

	if(widthAdj != 0)
		width -= widthAdj;

	if(heightAdj != 0)
		height -= heightAdj;
	m_camera->SetAspectRatio(width, height);	

	// setup tiling
	const int TILE_WIDTH  = width;
	const int TILE_HEIGHT = height;
	const int TILE_BORDER = 0;
	const int TILE_FACTOR = App::Inst().GetResolutionFactor();

	const int IMAGE_WIDTH  = width*TILE_FACTOR;
	const int IMAGE_HEIGHT = height*TILE_FACTOR;

	TRcontext *tr;
	GLubyte *buffer;
	GLubyte *tile;
	FILE *f;
	int more;

	// allocate buffer large enough to store one tile 
	tile = (GLubyte*) malloc(TILE_WIDTH * TILE_HEIGHT * 3 * sizeof(GLubyte));
	if (!tile) 
	{
		printf("Malloc of tile buffer failed!\n");
		return;
	}

	// allocate buffer to hold a row of tiles 
	buffer = (GLubyte*) malloc(IMAGE_WIDTH * TILE_HEIGHT * 3 * sizeof(GLubyte));
	if (!buffer) 
	{
		free(tile);
		printf("Malloc of tile row buffer failed!\n");
		return;
	}

	// Setup.  Each tile is TILE_WIDTH x TILE_HEIGHT pixels. 
	tr = trNew();
	trTileSize(tr, TILE_WIDTH, TILE_HEIGHT, TILE_BORDER);
	trTileBuffer(tr, GL_RGB, GL_UNSIGNED_BYTE, tile);
	trImageSize(tr, IMAGE_WIDTH, IMAGE_HEIGHT);
	trRowOrder(tr, TR_TOP_TO_BOTTOM);

	trPerspective(tr, m_camera->GetFOV(), m_camera->GetAspectRatio(), m_camera->GetNearViewingPlane(), m_camera->GetFarViewingPlane());
	// Prepare ppm output file 
	f = fopen(filename.c_str(), "w");
	if (!f) 
	{
		printf("Couldn't open image file: %s\n", filename.c_str());
		return;
	}
	fprintf(f,"P6\n");
	fprintf(f,"# File created by GenGIS\n");
	fprintf(f,"%i %i\n", IMAGE_WIDTH, IMAGE_HEIGHT);
	fprintf(f,"255\n");
	fclose(f);

	f = fopen(filename.c_str(), "ab");  
	if (!f) {
		printf("Couldn't append to image file: %s\n", filename.c_str());
		return;
	}

	// Set GL_PACK_ALIGNMENT to 1 if the image width is not a multiple of 4.
	// However, some NVIDIA card seem to fail on this setting which is why we 
	// force the image width to be a multiple of 4 above.
	glPixelStorei(GL_PACK_ALIGNMENT, 4);

	// Draw tiles
	uint tileNum = 0;
	more = 1;
	while (more) 
	{
		int curColumn;
		trBeginTile(tr);
		curColumn = trGet(tr, TR_CURRENT_COLUMN);
		RenderScene();     
		more = trEndTile(tr);

		// save tile into tile row buffer
		{
			int curTileWidth = trGet(tr, TR_CURRENT_TILE_WIDTH);
			int bytesPerImageRow = IMAGE_WIDTH*3*sizeof(GLubyte);
			int bytesPerTileRow = (TILE_WIDTH-2*TILE_BORDER) * 3*sizeof(GLubyte);
			int xOffset = curColumn * bytesPerTileRow;
			int bytesPerCurrentTileRow = (curTileWidth-2*TILE_BORDER)*3*sizeof(GLubyte);
			int i;
			int curTileHeight = trGet(tr, TR_CURRENT_TILE_HEIGHT);

			// swap bytes in tile buffer
			for(int y=0; y < TILE_HEIGHT; ++y)
			{
				for(int x=0; x < TILE_WIDTH; ++x)
				{
					uint red = tile[x*3 + y*TILE_WIDTH*3];
					uint green = tile[x*3+1 + y*TILE_WIDTH*3];
					uint blue = tile[x*3+2 + y*TILE_WIDTH*3];

					tile[x*3 + y*TILE_WIDTH*3] = green;
					tile[x*3+1 + y*TILE_WIDTH*3] = blue;
					tile[x*3+2 + y*TILE_WIDTH*3] = red;
				}
			}

			for (i=0;i<curTileHeight;i++) 
			{
				memcpy(buffer + i*bytesPerImageRow + xOffset, 
					tile + i*bytesPerTileRow,             
					bytesPerCurrentTileRow);            
			}
		}

		if (curColumn == trGet(tr, TR_COLUMNS)-1) 
		{
			// write this buffered row of tiles to the file 
			int curTileHeight = trGet(tr, TR_CURRENT_TILE_HEIGHT);
			int bytesPerImageRow = IMAGE_WIDTH*3*sizeof(GLubyte);
			int i;
			GLubyte *rowPtr;
			// The arithmetic is a bit tricky here because of borders and
			// the up/down flip.  Thanks to Marcel Lancelle for fixing it.

			for (i=2*TILE_BORDER;i<curTileHeight;i++) 
			{
				// Remember, OpenGL images are bottom to top.  Have to reverse. 
				rowPtr = buffer + (curTileHeight-1-i) * bytesPerImageRow;
				fwrite(rowPtr, 1, IMAGE_WIDTH*3, f);
			}
		}

		tileNum++;
		progressDlg->Update(float(tileNum) / ((TILE_FACTOR+1)*(TILE_FACTOR+1)) * 100.0f);
	}

	trDelete(tr);

	fclose(f);

	free(tile);
	free(buffer);
}

void Viewport::OnCharFromKeyboard( wxKeyEvent& event )
{
	event.Skip();

	m_sigKeyboard(event);
}

void Viewport::SetBackgroundColour(const Colour& colour)
{
	m_bgColour = colour;
	glClearColor(m_bgColour.GetRed(), m_bgColour.GetGreen(), m_bgColour.GetBlue(), 1.0);
}
