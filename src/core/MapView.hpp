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

#ifndef _GENGIS_MAP_VIEW_
#define _GENGIS_MAP_VIEW_

#include "../core/Precompiled.hpp"

#include "../core/App.hpp"
#include "../core/View.hpp"
#include "../core/Viewport.hpp"
#include "../core/Layer.hpp"

class GenGIS::MapView;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::MapView * t, const unsigned int file_version);
	}
}

namespace GenGIS
{
	class MapView : public View
	{
	private:
		

		/** Quadtree structure used to build map model. */
		typedef struct sQUADTREE_NODE
		{
			GLint lodvertex[9];
			sQUADTREE_NODE *q1, *q2, *q3, *q4;
			bool enabled;
			float max_err;
		} QuadtreeNode;

		Point3D theNormal;

		void computNormal(int i, int j); 

	public:
		/** Holds all information required by a vertex which is part of the terrain. */
		typedef struct sVERTEX
		{
			Point3D coordinate;		// 12
			bool	enabled;		// 1
			GLint	dependents[4];	// 4x4=16 (can't reduce size...) 
			bool	prob;			// 1

			template <typename Archive>
			void serialize(Archive& ar, const unsigned int version)
			{
				ar & coordinate; // Point3D
				ar & enabled;    // bool
				ar & dependents; // GLint
				ar & prob;       // bool
			}
		} Vertex;					// 56??

		/** Constructor. */
		explicit MapView(MapModelPtr mapModel);

		/** Destructor. */
		~MapView();

		/** Build 3D terrain. */
		bool BuildTerrain(MapControllerPtr mapController, ProgressDlgPtr progressDlg = ProgressDlgPtr());

		/** Render terrain. */
		void Render();
		void Render(bool bSimplified);

		/** Check if terrain is being draw as a wireframe. */
		bool IsWireFrame() { return m_bWireFrame; }

		/** Set terrain to wireframe (true) or textured (false). */
		void SetWireFrame(bool state) { m_bWireFrame = state; }

		/** Set the amount of error allowed in determining the elevation. */
		void SetEpsilon(float error);

		/** Get the amount of error allowed in determining the elevation. */
		float GetEpsilon() const { return m_epsilon; }

		/** Increment amount of error allowed in elevation. */
		void IncEpsilon(float amt);

		/** Set vertical exaggeration of elevation. */
		void SetVerticalExaggeration(MapControllerPtr mapController, float exaggeration);

		/** Rebuild the LOD quadtree structure. */
		void RefreshQuadtree();

		/** Set the visibility of view. */
		void SetVisibility( bool visible );

		/** Set the visibility of view. */
		bool IsVisible() const;
		
		/** Get and Set vertices for Cartograms. */
		void SetVertices(MapControllerPtr mapController);
		QuadtreeNode* createQuadtree( int lodsize, int startindex );
		int GetDimension(){return m_dimension;}
		void SetRoot( QuadtreeNode* root ){ m_root=root;}

		/** Get and Set for Cartogram state. */
		void SetCartogramState(bool state){m_makingCartogram = state;}
		bool GetCartogramState(){return m_makingCartogram;}
	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const MapView * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int file_version);

		
		void init_vertices(MapControllerPtr mapController);
		void getAverageNormal( int i, int j );
		void assignTextureCoordinate( int i , int j );

		// quadtree related functions
		//QuadtreeNode* createQuadtree( int lodsize, int startindex );
		void delete_quadtree( QuadtreeNode *cur_node );
		bool inter_error( int center, int vertexA, int vertexB);
		bool box_test( QuadtreeNode *cur);
		void update_quadtree( QuadtreeNode *root_cur);
		void notify( int cur_idx );
		void update_anticrack( void );
		void clear_enabled_quadtree( QuadtreeNode *root_cur );
		void render_quadtree( QuadtreeNode *root_cur );
		void drawTerrainLeftover();
		void define_vertex( int index_vertex );

		//collapsing function
		void init_collapse();
		void enable_collapse_bounds();
		void collapse_geometry(Point3D &original, Point3D &transformed, bool& isScaled );
		void resort_geometry();

		// utility functions
		void crossProduct(Vertex &a, Vertex &b, Vertex &c, float* normal);

		// free all memory allocated for terrain
		void FreeTerrain();

	private:
		QuadtreeNode* m_root;
		Vertex* m_vertices;

		// terrain state variables
		GLdouble m_epsilon; // for "Level of Detail"
		bool m_bWireFrame;

		// Variables used for construct/rendering Quadtree
		// used by quadtree
		//----------------------------------
		int   m_dimension;
		float m_curError;

		//
		// Collapsing control variables
		//----------------------------------
		std::pair<Point3D, Point3D> m_collapsePoints;
		int m_cbounderyCount;

		float m_collapseRatio, m_collapseRatioCur, m_collapseRatioInc;
		float m_uLast, m_vLast;

		float m_xAxis, m_zAxis;
		float m_k;
		float m_distTarget;
		bool  m_select1, m_select2;
		float m_xCenter, m_zCenter;
		float m_collapseAngle;

		GLuint m_displayList;

		int polyc;

		MapModelPtr m_mapModel;

		bool m_makingCartogram;
	};
}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const MapView * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			ar << t->m_mapModel;
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, MapView * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			MapModelPtr _mapModel;
			ar >> _mapModel;

			::new(t)MapView( _mapModel );
		}
	}
}

#endif
