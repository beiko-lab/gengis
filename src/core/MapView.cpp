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
#include "../core/Camera.hpp"
#include "../core/MapController.hpp"
#include "../core/MapModel.hpp"
#include "../core/MapView.hpp"
#include "../core/MapTexture.hpp"
#include "../core/StudyLayer.hpp"
#include "../core/Viewport.hpp"

#include "../gui/ProgressDlg.hpp"

#include "../utils/StringTools.hpp"
#include "../utils/Log.hpp"

using namespace GenGIS;

MapView::MapView(MapModelPtr mapModel) :
	m_mapModel(mapModel),
	m_root(NULL),
	m_vertices(NULL),
	m_bWireFrame(false),
	m_dimension(0),
	m_curError(0),
	m_cbounderyCount(0),
	m_collapseRatio(0),
	m_collapseRatioCur(0),
	m_collapseRatioInc(0),
	m_uLast(0),
	m_vLast(0),
	m_xAxis(0),
	m_zAxis(0),
	m_k(0), 
	m_distTarget(0),
	m_xCenter(0),
	m_zCenter(0),
	m_collapseAngle(0),
	m_displayList(0),
	polyc(0)
{

}

template<class Archive>
void MapView::serialize(Archive & ar, const unsigned int file_version)
{
	ar & boost::serialization::base_object<View>(*this);
	ar & theNormal;				 // Point3D
	//ar & m_root;                 // QuadtreeNode*
	ar & m_vertices;             // Vertex*
	ar & m_epsilon;              // GLdouble
	ar & m_bWireFrame;           // bool
	ar & m_dimension;            // int
	ar & m_curError;             // float
	ar & m_collapsePoints;       // std::pair<Point3D, Point3D>
	ar & m_cbounderyCount;       // int
	ar & m_collapseRatio;        // float
	ar & m_collapseRatioCur;     // float
	ar & m_collapseRatioInc;     // float
	ar & m_uLast;                // float
	ar & m_vLast;                // float
	ar & m_xAxis;                // float
	ar & m_zAxis;                // float
	ar & m_k;                    // float
	ar & m_distTarget;           // float
	//	ar & m_select1;              // bool
	//	ar & m_select2;              // bool
	ar & m_xCenter;              // float
	ar & m_zCenter;              // float
	ar & m_collapseAngle;        // float
	ar & m_displayList;          // GLuint
	ar & polyc;                  // int
}

template void MapView::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void MapView::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

MapView::~MapView() 
{
	FreeTerrain();
	glDeleteLists(m_displayList, 1);
}

void MapView::FreeTerrain()
{
	if( m_vertices != NULL ) 
	{
		free(m_vertices);
		m_vertices = NULL;
	}

	if( m_root != NULL)
	{
		delete_quadtree( m_root );
		m_root = NULL;
	}
}

bool MapView::BuildTerrain(MapControllerPtr mapController, ProgressDlgPtr progressDlg) 
{
	// free memory to previously allocated terrain
	FreeTerrain();

	if(progressDlg)
	{
		if(!progressDlg->Update(0, _T("Building terrain: creating elevational terrain.")))
			return false;
	}

	MapModelPtr mapModel = mapController->GetMapModel();
//	float maxTemp = mapModel->GetMaxElevationGridSpace();
//	float minTemp = mapModel->GetMinElevationGridSpace();

	// set initial error in level-of-detail to 5% of the elevation range
	SetEpsilon((mapModel->GetMaxElevationGridSpace()-mapModel->GetMinElevationGridSpace())*0.05);

	int dimension_tmp=1;
	if(mapModel->GetElevationMap() == true)
	{
		// trim the terrain data grid by finding the largest "good" dimension for a square grid
		// a "good" dimension for a square grid should be  2^n + 1

		while (dimension_tmp < mapModel->GetHeader()->nCols || dimension_tmp < mapModel->GetHeader()->nRows) 
		{
			dimension_tmp *= 2;
		}

		int maxTerrainResolution = App::Inst().GetViewport()->GetMaxTerrainResolution();
		if(dimension_tmp > maxTerrainResolution)
			m_dimension = maxTerrainResolution + 1;
		else
			m_dimension = dimension_tmp + 1;
	}
	else
	{
		// no need to maintain an elevational grid for strictly 2D maps
		m_dimension = 257; // A value of 3 should be sufficent (i.e., smallest 2^n + 1 value),
		// but things don't seem to work for values smaller than 2^8+1
	}

	if(progressDlg)
	{
		if (!App::Inst().GetLayerTreeController()->GetIsBlankRaster())
		{
			if(!progressDlg->Update(30, _T("Building terrain: creating elevational terrain.")))
				return false;
		}
		else
		{
			if(!progressDlg->Update(30, _T("Reading SHP file.")))
				return false;
		}
	}

	m_vertices = ( Vertex* )malloc( sizeof( Vertex ) * m_dimension * m_dimension );
	if (m_vertices == NULL) 
	{ 
		Log::Inst().Warning("(Warning) Insufficent memory to build terrain.");
		return false;
	}

	init_vertices( mapController );   

	if(progressDlg)
	{
		if (!App::Inst().GetLayerTreeController()->GetIsBlankRaster())
		{
			if(!progressDlg->Update(60, _T("Building terrain: creating elevational terrain.")))
				return false;
		}
		else			
		{
			if(!progressDlg->Update(60, _T("Visualizing the Vector Map.")))
				return false;
		}

	}

	// construct the quadtree from the regular grid.
	m_root = createQuadtree( m_dimension, 0 );

	if(progressDlg)
	{
		if(!progressDlg->Update(99))
			return false;
	}

	return true;
}

// Function to delete quadtree
void MapView::delete_quadtree( QuadtreeNode *cur_node ) 
{
	if(m_root != NULL && cur_node != NULL)
	{
		if ( cur_node->q1 != NULL )
		{
			delete_quadtree( cur_node->q1 );
		}
		if ( cur_node->q2 != NULL )
		{
			delete_quadtree( cur_node->q2 );
		}
		if ( cur_node->q3 != NULL )
		{
			delete_quadtree( cur_node->q3 );
		}
		if ( cur_node->q4 != NULL )
		{
			delete_quadtree( cur_node->q4 );
		}

		delete cur_node;
		cur_node = NULL;
	}
}

void MapView::SetVerticalExaggeration(MapControllerPtr mapController, float exaggeration) 
{ 
	// DHP: This is an inefficent way to rescale
	// the map, so please feel free to find a more elegant way!
	if(exaggeration != mapController->GetMapModel()->GetVerticalExaggeration())
	{
		wxBusyCursor wait;

		mapController->GetMapModel()->SetVerticalExaggeration(exaggeration);

		delete_quadtree( m_root );
		init_vertices( mapController );
		m_root = createQuadtree( m_dimension, 0 );
		RefreshQuadtree();
	}
}

void MapView::SetVertices(MapControllerPtr mapController)
{
	FileHeader* header = mapController->GetMapModel()->GetHeader();
	Point3D* grid = mapController->GetMapModel()->GetGrid();

	// retrieve the elevation readings from the elevation parser;
	//	and calculate the normals

	for ( int i = 0; i < m_dimension; i++ ) 
	{
		for ( int j = 0; j < m_dimension; j++ ) 
		{
			int indexX = int(j*(1.0f/m_dimension)*header->nCols);
			int indexY = int(i*(1.0f/m_dimension)*header->nRows);
			int index = indexX + indexY*header->nCols;

			m_vertices[i*m_dimension+j].coordinate.x = grid[index].x;
			m_vertices[i*m_dimension+j].coordinate.y = mapController->GetMapModel()->GetVerticalExaggeration()*grid[index].y;
			m_vertices[i*m_dimension+j].coordinate.z = grid[index].z;

			// assign dependencies
			for ( int k = 0; k < 4; k++ ) 
			{
				m_vertices[i*m_dimension+j].dependents[k] = -1;
			}

			m_vertices[i*m_dimension+j].enabled = false;
			m_vertices[i*m_dimension+j].prob = false;
		}
	}
}

void MapView::init_vertices(MapControllerPtr mapController) 
{
	SetVertices(mapController);
}

// steve - added to save memory
void MapView::computNormal(int i, int j) 
{ 
	float adjNormals[8][3];
	unsigned short count = 0;

	if( i > 0 ) 
	{ 	    
		if( j > 0 ) 
		{
			crossProduct( m_vertices[i*( m_dimension )+j], m_vertices[( i-1 )*( m_dimension )+j], m_vertices[( i-1 )*( m_dimension )+j-1], adjNormals[0] );
			crossProduct( m_vertices[i*( m_dimension )+j], m_vertices[( i-1 )*( m_dimension )+j-1], m_vertices[i*( m_dimension )+j-1], adjNormals[1] );
			count += 2;
		}
		else 
		{
			adjNormals[0][0] = 0;
			adjNormals[0][1] = 1;
			adjNormals[0][2] = 0;
			adjNormals[1][0] = 0;
			adjNormals[1][1] = 1;
			adjNormals[1][2] = 0; 
		}

		if ( j < m_dimension-1 ) 
		{
			crossProduct( m_vertices[i*( m_dimension )+j], m_vertices[( i-1 )*( m_dimension )+j+1], m_vertices[( i-1 )*( m_dimension )+j], adjNormals[2] );
			crossProduct( m_vertices[i*( m_dimension )+j], m_vertices[i*( m_dimension )+j+1], m_vertices[( i-1 )*( m_dimension )+j+1], adjNormals[3] );
			count += 2;
		}
		else 
		{
			adjNormals[2][0] = 0;
			adjNormals[2][1] = 1;
			adjNormals[2][2] = 0;
			adjNormals[3][0] = 0;
			adjNormals[3][1] = 1;
			adjNormals[3][2] = 0; 
		}
	}
	else 
	{
		adjNormals[0][0] = 0;
		adjNormals[0][1] = 1;
		adjNormals[0][2] = 0;
		adjNormals[1][0] = 0;
		adjNormals[1][1] = 1;
		adjNormals[1][2] = 0; 
		adjNormals[2][0] = 0;
		adjNormals[2][1] = 1;
		adjNormals[2][2] = 0;
		adjNormals[3][0] = 0;
		adjNormals[3][1] = 1;
		adjNormals[3][2] = 0; 
	}

	if ( i < m_dimension-1 )  
	{
		if ( j > 0 ) 
		{
			crossProduct( m_vertices[i*( m_dimension )+j], m_vertices[i*( m_dimension )+j-1], m_vertices[( i+1 )*( m_dimension )+j-1], adjNormals[4] );
			crossProduct( m_vertices[i*( m_dimension )+j], m_vertices[( i+1 )*( m_dimension )+j-1], m_vertices[( i+1 )*( m_dimension )+j], adjNormals[5] );
			count += 2;      
		}
		else 
		{
			adjNormals[4][0] = 0;
			adjNormals[4][1] = 1;
			adjNormals[4][2] = 0;
			adjNormals[5][0] = 0;
			adjNormals[5][1] = 1;
			adjNormals[5][2] = 0; 
		}

		if ( j < m_dimension-1 ) 
		{
			crossProduct( m_vertices[i*( m_dimension )+j], m_vertices[( i+1 )*( m_dimension )+j], m_vertices[( i+1 )*( m_dimension )+j+1], adjNormals[6] );
			crossProduct( m_vertices[i*( m_dimension )+j], m_vertices[( i+1 )*( m_dimension )+j+1], m_vertices[i*( m_dimension )+j+1], adjNormals[7] );
			count += 2;      
		}
		else 
		{
			adjNormals[6][0] = 0;
			adjNormals[6][1] = 1;
			adjNormals[6][2] = 0;
			adjNormals[7][0] = 0;
			adjNormals[7][1] = 1;
			adjNormals[7][2] = 0; 
		}
	}
	else 
	{
		adjNormals[4][0] = 0;
		adjNormals[4][1] = 1;
		adjNormals[4][2] = 0;
		adjNormals[5][0] = 0;
		adjNormals[5][1] = 1;
		adjNormals[5][2] = 0; 
		adjNormals[6][0] = 0;
		adjNormals[6][1] = 1;
		adjNormals[6][2] = 0;
		adjNormals[7][0] = 0;
		adjNormals[7][1] = 1;
		adjNormals[7][2] = 0; 
	}

	// compute the average
	float x = 0, y = 0, z = 0;

	for ( int m = 0; m < 8 ; m++ ) 
	{
		x += adjNormals[m][0];
		y += adjNormals[m][1];
		z += adjNormals[m][2];
	}

	theNormal.x = x / count;
	theNormal.y = y / count;
	theNormal.z = z / count;

	// The y component of the normal should always be greater than 0 in order to 
	// ensure it correctly reflects light. Under some undetermined circumstances the
	// normal can become less than (or at least equal to) 0 so we fix it up here.
	if(theNormal.y <= 0)
		theNormal.y = 1e-10f;
}

void MapView::crossProduct(Vertex &a, Vertex &b, Vertex &c, float* normal ) 
{
	// calculate the two vectors in the plane
	float AB[3], AC[3];

	AB[0] = b.coordinate.x - a.coordinate.x;
	AC[0] = c.coordinate.x - a.coordinate.x;
	AB[1] = b.coordinate.y - a.coordinate.y;
	AC[1] = c.coordinate.y - a.coordinate.y;
	AB[2] = b.coordinate.z - a.coordinate.z;
	AC[2] = c.coordinate.z - a.coordinate.z;

	// cross product
	normal[0] = AB[1] * AC[2] - AB[2] * AC[1];
	normal[1] = -( AB[0] * AC[2] - AB[2] * AC[0] );
	normal[2] = AB[0] * AC[1] - AB[1] * AC[0];
}

// -- quadtree functions
void MapView::update_anticrack()
{
	for ( int i = 0; i < m_dimension; i++ )
	{
		for ( int j = 0; j < m_dimension; j++ )
		{
			if ( m_vertices[i*m_dimension+j].prob )
			{
				notify( i*m_dimension + j );
			}
		}

	}
}

void MapView::notify( int cur_idx ) 
{
	int idx;
	for ( int j = 0 ; j < 4; j++ )
	{
		idx = m_vertices[cur_idx].dependents[j];
		if ( idx != -1 )
		{ // points to a Vertex

			if ( !m_vertices[idx].enabled )
			{
				//cout<< "--> turn on ";
				m_vertices[idx].enabled = true;
				notify( idx );
			}
		}
	}

	m_vertices[cur_idx].prob = false;
}

MapView::QuadtreeNode* MapView::createQuadtree( int lodsize, int startindex ) 
{
	QuadtreeNode *current = new QuadtreeNode;

	if ( lodsize == 3 ) { // recurrsion terminates

		// 3 by 3 values...
		for ( int i = 0; i < lodsize ; i++ )
		{
			for ( int j = 0; j < lodsize; j++ )
			{
				current->lodvertex[i*3+j] = startindex + i * m_dimension + j;        
			}
		}

		current->q1 = NULL;
		current->q2 = NULL;
		current->q3 = NULL;
		current->q4 = NULL;

		// calculate the max Vertex error
		current->max_err = -1;

		// Go through the four sides and calculate the Vertex errors

		// top --------------

		m_curError =	fabs( 0.5 * ( m_vertices[current->lodvertex[0]].coordinate.y + 
			m_vertices[current->lodvertex[2]].coordinate.y )
			- m_vertices[current->lodvertex[1]].coordinate.y );

		if ( m_curError > current->max_err )
		{
			current->max_err = m_curError;
		}

		// left --------------
		m_curError = fabs( 0.5 * ( m_vertices[current->lodvertex[0]].coordinate.y + 
			m_vertices[current->lodvertex[6]].coordinate.y )
			- m_vertices[current->lodvertex[3]].coordinate.y );

		if ( m_curError > current->max_err )
		{
			current->max_err = m_curError;
		}

		// bottom --------------
		m_curError = fabs( 0.5 * ( m_vertices[current->lodvertex[6]].coordinate.y + 
			m_vertices[current->lodvertex[8]].coordinate.y )
			- m_vertices[current->lodvertex[7]].coordinate.y );

		if ( m_curError > current->max_err )
		{
			current->max_err = m_curError;
		}

		// right --------------
		m_curError = fabs( 0.5 * ( m_vertices[current->lodvertex[2]].coordinate.y + 
			m_vertices[current->lodvertex[8]].coordinate.y )
			- m_vertices[current->lodvertex[5]].coordinate.y );

		if ( m_curError > current->max_err )
		{
			current->max_err = m_curError;
		}

		// by here, the max_err has the true max error in the current LOD block

		// Assign dependents for each Vertex
		//		artificially assign left/up 0, right/down 1;
		//		4 corners don't matter

		m_vertices[current->lodvertex[4]].dependents[0] = current->lodvertex[2];
		m_vertices[current->lodvertex[4]].dependents[1] = current->lodvertex[0];
		m_vertices[current->lodvertex[4]].dependents[2] = current->lodvertex[6];
		m_vertices[current->lodvertex[4]].dependents[3] = current->lodvertex[8];

		m_vertices[current->lodvertex[1]].dependents[1] = current->lodvertex[4];
		m_vertices[current->lodvertex[3]].dependents[1] = current->lodvertex[4];
		m_vertices[current->lodvertex[5]].dependents[0] = current->lodvertex[4];
		m_vertices[current->lodvertex[7]].dependents[0] = current->lodvertex[4];

	}
	else
	{ // generate full-size LOD from m_vertices
		current->lodvertex[0] = startindex;
		current->lodvertex[1] = startindex + lodsize / 2;
		current->lodvertex[2] = startindex + lodsize - 1;
		current->lodvertex[3] = startindex + ( lodsize / 2 ) * m_dimension;
		current->lodvertex[4] = startindex + ( lodsize / 2 ) * m_dimension + lodsize / 2;
		current->lodvertex[5] = startindex + ( lodsize / 2 ) * m_dimension + lodsize - 1;
		current->lodvertex[6] = startindex + ( lodsize - 1 ) * m_dimension;
		current->lodvertex[7] = startindex + ( lodsize - 1 ) * m_dimension + lodsize / 2;
		current->lodvertex[8] = startindex + ( lodsize - 1 ) * m_dimension + lodsize - 1 ;

		// Assign dependents for each Vertex
		//		artificially assign left/up 0, right/down 1;
		//		4 corners don't matter

		m_vertices[current->lodvertex[4]].dependents[0] = current->lodvertex[2];
		m_vertices[current->lodvertex[4]].dependents[1] = current->lodvertex[0];
		m_vertices[current->lodvertex[4]].dependents[2] = current->lodvertex[6];
		m_vertices[current->lodvertex[4]].dependents[3] = current->lodvertex[8];

		m_vertices[current->lodvertex[1]].dependents[1] = current->lodvertex[4];
		m_vertices[current->lodvertex[3]].dependents[1] = current->lodvertex[4];
		m_vertices[current->lodvertex[5]].dependents[0] = current->lodvertex[4];
		m_vertices[current->lodvertex[7]].dependents[0] = current->lodvertex[4];

		current->q1 = createQuadtree( lodsize / 2 + 1, current->lodvertex[1] );
		current->q2 = createQuadtree( lodsize / 2 + 1, startindex );
		current->q3 = createQuadtree( lodsize / 2 + 1, current->lodvertex[3] );
		current->q4 = createQuadtree( lodsize / 2 + 1, current->lodvertex[4] );

		// calculate the max Vertex error
		current->max_err = current->q1->max_err;

		if ( current->q2->max_err > current->max_err )
		{
			current->max_err = current->q2->max_err;
		}

		if ( current->q3->max_err > current->max_err )
		{
			current->max_err = current->q3->max_err;

		}

		if ( current->q4->max_err > current->max_err )
		{
			current->max_err = current->q4->max_err;
		}

		// Go through the four sides and calculate the Vertex errors

		// top
		m_curError = fabs( 0.5 * ( m_vertices[current->lodvertex[0]].coordinate.y + m_vertices[current->lodvertex[2]].coordinate.y )
			- m_vertices[current->lodvertex[1]].coordinate.y );

		if ( m_curError > current->max_err )
		{
			current->max_err = m_curError;
		}

		// left
		m_curError = fabs( 0.5 * ( m_vertices[current->lodvertex[0]].coordinate.y + m_vertices[current->lodvertex[6]].coordinate.y )
			- m_vertices[current->lodvertex[3]].coordinate.y );

		if ( m_curError > current->max_err )
		{
			current->max_err = m_curError;
		}

		// bottom
		m_curError = fabs( 0.5 * ( m_vertices[current->lodvertex[6]].coordinate.y + m_vertices[current->lodvertex[8]].coordinate.y )
			- m_vertices[current->lodvertex[7]].coordinate.y );

		if ( m_curError > current->max_err )
		{
			current->max_err = m_curError;
		}

		// right
		m_curError = fabs( 0.5 * ( m_vertices[current->lodvertex[2]].coordinate.y + m_vertices[current->lodvertex[8]].coordinate.y )
			- m_vertices[current->lodvertex[5]].coordinate.y );

		if ( m_curError > current->max_err )
		{
			current->max_err = m_curError;
		}

		// by here, the max_err has the true max error in the current LOD block
	}

	return current;
}

void MapView::update_quadtree(QuadtreeNode *root_cur) 
{
	// enable the current block
	root_cur->enabled = true;

	// center Vertex
	m_vertices[root_cur->lodvertex[4]].enabled = true;

	// corner m_vertices
	m_vertices[root_cur->lodvertex[0]].enabled = true;
	m_vertices[root_cur->lodvertex[2]].enabled = true;
	m_vertices[root_cur->lodvertex[6]].enabled = true;
	m_vertices[root_cur->lodvertex[8]].enabled = true;

	// testing four edge m_vertices :
	//		if the edge Vertex is already set

	//top
	if ( !inter_error( root_cur->lodvertex[1], root_cur->lodvertex[0], root_cur->lodvertex[2]) )
	{
		m_vertices[root_cur->lodvertex[1]].enabled = true;
		// crack preventing
		m_vertices[root_cur->lodvertex[1]].prob = true;

	}
	//left
	if ( !inter_error( root_cur->lodvertex[3], root_cur->lodvertex[0], root_cur->lodvertex[6]) )
	{
		m_vertices[root_cur->lodvertex[3]].enabled = true;
		// crack preventing
		m_vertices[root_cur->lodvertex[3]].prob = true;
	}

	//bottom
	if ( !inter_error( root_cur->lodvertex[7], root_cur->lodvertex[6], root_cur->lodvertex[8]) )
	{
		m_vertices[root_cur->lodvertex[7]].enabled = true;

		// crack preventing
		m_vertices[root_cur->lodvertex[7]].prob = true;

	}

	//right
	if ( !inter_error( root_cur->lodvertex[5], root_cur->lodvertex[2], root_cur->lodvertex[8]) )
	{
		m_vertices[root_cur->lodvertex[5]].enabled = true;

		// crack preventing
		m_vertices[root_cur->lodvertex[5]].prob = true;

	}

	// box-testing the four quadrants
	//		may save some of the calculation needed for edge m_vertices in current LOD

	if ( root_cur->q1 && box_test( root_cur->q1) 

		/*&& !camera->block_cliptest( m_vertices[root_cur->q1->lodvertex[0]],
		m_vertices[root_cur->q1->lodvertex[2]],
		m_vertices[root_cur->q1->lodvertex[6]],
		m_vertices[root_cur->q1->lodvertex[8]] ) */) {
		update_quadtree( root_cur->q1);

		m_vertices[root_cur->lodvertex[1]].prob = true;
		m_vertices[root_cur->lodvertex[5]].prob = true;
	}


	if ( root_cur->q2 && box_test( root_cur->q2) /*&& !camera->block_cliptest( m_vertices[root_cur->q2->lodvertex[0]],
												 m_vertices[root_cur->q2->lodvertex[2]],
												 m_vertices[root_cur->q2->lodvertex[6]],
												 m_vertices[root_cur->q2->lodvertex[8]] ) */) {
		update_quadtree( root_cur->q2);

		m_vertices[root_cur->lodvertex[1]].prob = true;
		m_vertices[root_cur->lodvertex[3]].prob = true;
	}

	if ( root_cur->q3 && box_test( root_cur->q3) /*&& !camera->block_cliptest( m_vertices[root_cur->q3->lodvertex[0]],
												 m_vertices[root_cur->q3->lodvertex[2]],
												 m_vertices[root_cur->q3->lodvertex[6]],
												 m_vertices[root_cur->q3->lodvertex[8]]  )*/ ) {
		update_quadtree( root_cur->q3);
		m_vertices[root_cur->lodvertex[3]].prob = true;
		m_vertices[root_cur->lodvertex[7]].prob = true;
	}

	if ( root_cur->q4 && box_test( root_cur->q4) /*&& !camera->block_cliptest( m_vertices[root_cur->q4->lodvertex[0]],
												 m_vertices[root_cur->q4->lodvertex[2]],
												 m_vertices[root_cur->q4->lodvertex[6]],
												 m_vertices[root_cur->q4->lodvertex[8]] ) */) {
		update_quadtree( root_cur->q4);
		m_vertices[root_cur->lodvertex[7]].prob = true;
		m_vertices[root_cur->lodvertex[5]].prob = true;
	}
}

void MapView::clear_enabled_quadtree( QuadtreeNode *root_cur )
{
	for ( int i = 0; i < m_dimension; i++ )
	{
		for ( int j = 0; j < m_dimension; j++ )
		{
			if ( m_vertices[i*m_dimension+j].enabled )
			{
				m_vertices[i*m_dimension+j].enabled = false;
			}
		}
	}
}

bool MapView::inter_error( int center, int vertexA, int vertexB) 
{
	GLdouble err = fabs( m_vertices[center].coordinate.y -
		0.5 * ( m_vertices[vertexB].coordinate.y + m_vertices[vertexA].coordinate.y ) );

	Point3D pos = App::Inst().GetViewport()->GetCamera()->GetPosition();

	GLdouble maxX = fabs( m_vertices[center].coordinate.x - pos.x );
	GLdouble maxY = fabs( m_vertices[center].coordinate.y - pos.y );
	GLdouble maxZ = fabs( m_vertices[center].coordinate.z - pos.z );
	GLdouble max = maxX;
	if(max < maxY)
		max = maxY;

	if(max < maxZ)
		max = maxZ;

	return (err / max) <= m_epsilon;
}


bool MapView::box_test( QuadtreeNode *cur) 
{
	Point3D pos = App::Inst().GetViewport()->GetCamera()->GetPosition();

	GLdouble boxspan =  fabs( m_vertices[cur->lodvertex[2]].coordinate.x - m_vertices[cur->lodvertex[0]].coordinate.x );

	GLdouble maxX = fabs( m_vertices[cur->lodvertex[4]].coordinate.x - pos.x ) - boxspan;
	GLdouble maxY = fabs( m_vertices[cur->lodvertex[4]].coordinate.z - pos.z ) - boxspan;
	GLdouble maxZ = fabs( m_vertices[cur->lodvertex[4]].coordinate.y - pos.y );
	GLdouble max = maxX;
	if(max < maxY)
		max = maxY;

	if(max < maxZ)
		max = maxZ;

	return ( cur->max_err / max ) > m_epsilon;
}

// --- collapse terrain functions
void MapView::init_collapse() 
{
	// Collasping settings
	m_select1 = false;
	m_select2 = false;

	// collapsing variable initialization
	m_cbounderyCount = 0;
}

void MapView::enable_collapse_bounds() 
{
	Point3D point1, point2;

	point1 = m_collapsePoints.first;
	point2 = m_collapsePoints.second;

	// if there is collapsing, the m_vertices along the bounderies should be turned on
	MapModelPtr mapModel = App::Inst().GetMapController()->GetMapModel();
	if ( m_collapseAngle != PI / 2 ) { // slope exists

		float k_ab = tan( m_collapseAngle );

		int bound1, bound2;

		if ( m_collapseAngle >= PI / 4 && m_collapseAngle <= 3*PI / 4 )
		{
			for ( int m = 0; m < mapModel->GetHeader()->nRows; m++ )
			{
				bound1 = ( int )floor( point1.x - k_ab * ( m - point1.z ) );
				bound2 = ( int )floor( point2.x - k_ab * ( m - point2.z ) );

				if ( bound1 >= 0 && bound1 < mapModel->GetHeader()->nCols )
				{
					m_vertices[m*mapModel->GetHeader()->nCols+ bound1].enabled = true;
					m_vertices[m*mapModel->GetHeader()->nCols+ bound1].prob    = true;
				}
				if ( bound2 >= 0 && bound2 < mapModel->GetHeader()->nCols )
				{
					m_vertices[m*mapModel->GetHeader()->nCols+ bound2].enabled = true;
					m_vertices[m*mapModel->GetHeader()->nCols+ bound2].prob    = true;
				}
			}
		}
		else
		{ // each column (x direction) has a point on collapsing axis
			for ( int n = 0; n < mapModel->GetHeader()->nCols; n++ )
			{
				bound1 = ( int )floor( point1.z - 1 / k_ab * ( n - point1.x ) );
				bound2 = ( int )floor( point2.z - 1 / k_ab * ( n - point2.x ) );

				if ( bound1 >= 0 && bound1 < mapModel->GetHeader()->nRows )
				{
					m_vertices[mapModel->GetHeader()->nCols*bound1+n].enabled = true;
					m_vertices[mapModel->GetHeader()->nCols*bound1+n].prob    = true;
				}

				if ( bound2 >= 0 && bound2 < mapModel->GetHeader()->nRows )
				{
					m_vertices[mapModel->GetHeader()->nCols*bound2+n].enabled = true;
					m_vertices[mapModel->GetHeader()->nCols*bound2+n].prob    = true;
				}
			}
		}

	}
	else
	{ // collapsing angle is 90 degree.. no slope
		for ( int n = 0; n < mapModel->GetHeader()->nCols; n++ )
		{
			m_vertices[( int )floor( point1.z )*mapModel->GetHeader()->nCols+ n].enabled = true;
			m_vertices[( int )floor( point1.z )*mapModel->GetHeader()->nCols+ n].prob    = true;
			m_vertices[( int )floor( point2.z )*mapModel->GetHeader()->nCols+ n].enabled = true;
			m_vertices[( int )floor( point2.z )*mapModel->GetHeader()->nCols+ n].prob    = true;
		}
	}
}

void MapView::collapse_geometry( Point3D &original, Point3D &transformed, bool& isScaled ) 
{
	float k_ab = tan( m_collapseAngle );

	m_xAxis = ( k_ab * original.x + m_xCenter / k_ab + m_zCenter - original.z )
		/ ( k_ab + 1 / k_ab ) ;

	m_zAxis = k_ab * ( m_xAxis - original.x ) +  original.z;

	Point3D point1, point2;
	point1 = m_collapsePoints.first;
	point2 = m_collapsePoints.second;

	// calculate the distance, to decide what to do about the collapsing.
	float dist_cur;

	dist_cur = sqrt( pow( m_xAxis - original.x, 2 ) + pow( m_zAxis - original.z, 2 ) );

	transformed.y = original.y;

	if ( dist_cur <= m_distTarget )
	{ // inside the collapsing zone
		//"scaled" according to perpendicular line
		transformed.x = m_xAxis + m_collapseRatioCur * ( original.x - m_xAxis );
		transformed.z = m_zAxis + m_collapseRatioCur * ( original.z - m_zAxis );
	}

	else
	{ // outside the zone
		if (  original.x <= m_xAxis )
		{
			if ( original.z <= m_zAxis )
			{
				transformed.x = original.x + ( 1 - m_collapseRatioCur ) * fabs( point1.x - m_xCenter );
				transformed.z = original.z + ( 1 - m_collapseRatioCur ) * fabs( point1.z - m_zCenter );

			}
			else
			{
				transformed.x = original.x + ( 1 - m_collapseRatioCur ) * fabs( point1.x - m_xCenter );
				transformed.z = original.z - ( 1 - m_collapseRatioCur ) * fabs( point1.z - m_zCenter );
			}
		}
		else
		{
			if ( original.z <= m_zAxis )
			{
				transformed.x = original.x - ( 1 - m_collapseRatioCur ) * fabs( point1.x - m_xCenter );
				transformed.z = original.z + ( 1 - m_collapseRatioCur ) * fabs( point1.z - m_zCenter );

			}
			else
			{
				transformed.x = original.x - ( 1 - m_collapseRatioCur ) * fabs( point1.x - m_xCenter );
				transformed.z = original.z - ( 1 - m_collapseRatioCur ) * fabs( point1.z - m_zCenter );
			}
		}
	}
	return;
}

void MapView::RefreshQuadtree() 
{
	// refresh the terrain quadtree if a map
	// has been loaded
	if(m_root)
	{
		clear_enabled_quadtree( m_root );
		update_quadtree( m_root );
		update_anticrack(); // dealing with cracks

		if(m_displayList == 0)
			m_displayList = glGenLists(1);

		polyc = 0;
		glNewList(m_displayList, GL_COMPILE);
		render_quadtree(m_root);
		glEndList();
	}
}

// drawing functions
void MapView::define_vertex( int index_vertex ) 
{
	polyc++;

	int i = index_vertex / m_dimension;
	int j = index_vertex - i * m_dimension;

	// normals (lighting)
	computNormal(i, j); 

	// steve - glNormal3d( m_vertices[index_vertex].normal.x, m_vertices[index_vertex].normal.y, m_vertices[index_vertex].normal.z );
	glNormal3d( theNormal.x, theNormal.y, theNormal.z );

	//steve - m_vertices[i*m_dimension+j].texturecoor.x = j * 1.0f / m_dimension;
	//steve - m_vertices[i*m_dimension+j].texturecoor.y = i * 1.0f / m_dimension;


	// texture mapping
	//steve - glTexCoord2d( m_vertices[index_vertex].texturecoor.x,  m_vertices[index_vertex].texturecoor.y );
	glTexCoord2d( (float)j / (float) m_dimension,  (float)i / (float) m_dimension );

	// geometry
	if ( m_cbounderyCount < 2 )
	{ // not collapsing
		glVertex3f( m_vertices[index_vertex].coordinate.x ,
			m_vertices[index_vertex].coordinate.y,
			m_vertices[index_vertex].coordinate.z );
	}
	else
	{ // collapsing according to the bounderies specified

		Point3D original, transformed;
		original.x = m_vertices[index_vertex].coordinate.x;
		original.y = m_vertices[index_vertex].coordinate.y;
		original.z = m_vertices[index_vertex].coordinate.z;

		bool isScaled;

		collapse_geometry( original, transformed, isScaled );

		glVertex3f( transformed.x, transformed.y, transformed.z );
	}// collapse ends
}

void MapView::render_quadtree( QuadtreeNode *root_cur ) 
{
	if ( root_cur->q1 != NULL && m_vertices[root_cur->q1->lodvertex[4]].enabled )
	{
		render_quadtree( root_cur->q1 );
	}

	// Second Quadrant
	if ( root_cur->q2 != NULL && m_vertices[root_cur->q2->lodvertex[4]].enabled )
	{
		render_quadtree( root_cur->q2 );
	}

	// The third quadrant
	if ( root_cur->q3 != NULL && m_vertices[root_cur->q3->lodvertex[4]].enabled )
	{
		render_quadtree( root_cur->q3 );
	}

	//cout<<"Fourth quadrant: ";

	if ( root_cur->q4 != NULL && m_vertices[root_cur->q4->lodvertex[4]].enabled )
	{
		render_quadtree( root_cur->q4 );
	}

	// actual rendering the geometry

	glBegin( GL_TRIANGLE_FAN );

	define_vertex( root_cur->lodvertex[4] );

	if ( root_cur->q1 == NULL || !m_vertices[root_cur->q1->lodvertex[4]].enabled )
	{
		if ( m_vertices[root_cur->lodvertex[5]].enabled )
		{
			define_vertex( root_cur->lodvertex[5] );
		}

		define_vertex( root_cur->lodvertex[2] );

		if ( m_vertices[root_cur->lodvertex[1]].enabled )
		{
			define_vertex( root_cur->lodvertex[1] );
		}

	} else
	{
		define_vertex( root_cur->lodvertex[4] );
	}


	if ( root_cur->q2 == NULL || !m_vertices[root_cur->q2->lodvertex[4]].enabled )
	{
		if ( m_vertices[root_cur->lodvertex[1]].enabled )
		{
			define_vertex( root_cur->lodvertex[1] );
		}

		define_vertex( root_cur->lodvertex[0] );

		if ( m_vertices[root_cur->lodvertex[3]].enabled )
		{
			define_vertex( root_cur->lodvertex[3] );
		}

	}
	else
	{
		define_vertex( root_cur->lodvertex[4] );
	}
	if ( root_cur->q3 == NULL || !m_vertices[root_cur->q3->lodvertex[4]].enabled )
	{
		if ( m_vertices[root_cur->lodvertex[3]].enabled )
		{
			define_vertex( root_cur->lodvertex[3] );
		}

		define_vertex( root_cur->lodvertex[6] );

		if ( m_vertices[root_cur->lodvertex[7]].enabled )
		{
			define_vertex( root_cur->lodvertex[7] );
		}
	}
	else
	{
		define_vertex( root_cur->lodvertex[4] );
	}

	if ( root_cur->q4 == NULL || !m_vertices[root_cur->q4->lodvertex[4]].enabled )
	{
		if ( m_vertices[root_cur->lodvertex[7]].enabled )
		{
			define_vertex( root_cur->lodvertex[7] );
		}

		define_vertex( root_cur->lodvertex[8] );

		if ( m_vertices[root_cur->lodvertex[5]].enabled )
		{
			define_vertex( root_cur->lodvertex[5] );
		}
		else
		{
			define_vertex( root_cur->lodvertex[2] );
		}
	}
	else
	{
		define_vertex( root_cur->lodvertex[4] );
	}
	glEnd();

	return;
}

void MapView::Render(bool bSimplified)
{
	glDepthRange(TERRAIN_START_DEPTH, 1.0);

	// draw "base plate" below the terrain so user can click
	// outside of the map and still recieve valid world coordinates
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	Colour bgColour = App::Inst().GetViewport()->GetBackgroundColour();
	glColor4ub( bgColour.GetRedInt(), bgColour.GetGreenInt(), bgColour.GetBlueInt(), 255 ); 
	glBegin(GL_QUADS);
	glVertex3f( -4.0f, -0.01f, 4.0f );
	glVertex3f( 4.0f, -0.01f, 4.0f );
	glVertex3f( 4.0f, -0.01f, -4.0f );
	glVertex3f( -4.0f, -0.01f, -4.0f );		
	glEnd();
	glEnable(GL_BLEND);

	// render terrain
	if(App::Inst().GetMapController()->GetMapModel()->GetTexture())
	{		
		if(!bSimplified)
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			//uint id = App::Inst().GetMapController()->GetMapModel()->GetTexture()->GetTextureId();
			glBindTexture(GL_TEXTURE_2D, App::Inst().GetMapController()->GetMapModel()->GetTexture()->GetTextureId());

			if (m_bWireFrame)  
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else 
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{		
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glCallList(m_displayList);
		glDisable(GL_TEXTURE_2D);
	}
}

void MapView::Render() 
{
	if(!m_bVisible)
		return;

	Render(false);
}

void MapView::IncEpsilon( float amt ) 
{
	const float MIN_EPSILON = 1e-6f;

	m_epsilon += amt;

	if(m_epsilon < MIN_EPSILON)
		m_epsilon = MIN_EPSILON;

	RefreshQuadtree();
}

void MapView::SetEpsilon(float error) 
{ 
	const float MIN_EPSILON = 1e-6f;

	if(m_epsilon != error)
	{
		if(error < MIN_EPSILON)
			error = MIN_EPSILON;

		m_epsilon = error; 
		RefreshQuadtree();
	}
}

void MapView::SetVisibility( bool visible ) 
{ 
	m_mapModel->SetActive(visible); 
}

bool MapView::IsVisible() const 
{ 
	return m_mapModel->IsActive(); 
}
