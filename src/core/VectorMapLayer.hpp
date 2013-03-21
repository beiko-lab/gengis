//=======================================================================
// Author: Timothy Mankowski & Somayyeh Zangooei
//
// Copyright 2012 Timothy Mankowski & Somayyeh Zangooei
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

#ifndef _GENGIS_VECTOR_MAP_LAYER_
#define _GENGIS_VECTOR_MAP_LAYER_

#include "../core/Precompiled.hpp"

#include "../core/Layer.hpp"

class GenGIS::VectorMapLayer;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::VectorMapLayer * t, const unsigned int file_version);
	}
}

namespace GenGIS
{
	class VectorMapLayer : public Layer
	{
	public:
		/** 
		* @brief Constuctor.
		* @param id Unique identifier used to specify map.
		* @param parent Parent of this layer.
		*/
		explicit VectorMapLayer( uint id, LayerPtr parent, VectorMapControllerPtr vectorMapController ) :
			Layer( id, Layer::VECTOR_MAP, parent ), m_vectorMapController( vectorMapController ) {}

		/** Destructor. */
		~VectorMapLayer() {}

		/** Get map controller. */
		VectorMapControllerPtr GetVectorMapController() const;

		/** Render contents of layer. */
		void Render();

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		void SetActive(bool state);

		/** Get active state of layer. */
		bool IsActive() const;

		/** Toggle active state of layer. */
		void ToggleActive();

	private:

		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const VectorMapLayer * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

		template<class Archive>
		void load(Archive & ar, const unsigned int version);

		template<class Archive>
		void save(Archive & ar, const unsigned int version) const;

	protected:
		/** Pointer to the VectorMapController. */
		VectorMapControllerPtr m_vectorMapController;
	};
}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data( Archive & ar, const VectorMapLayer * t, const unsigned int file_version )
		{
			
			ar << t->m_id;                // uint (Layer class)
			ar << t->m_parent;            // LayerPtr (Layer class)
			ar << t->m_vectorMapController; // VectorMapControllerPtr
		}

		template<class Archive>
		inline void load_construct_data( Archive & ar, VectorMapLayer * t, const unsigned int file_version )
		{
			// Retrieve data from archive required to construct new instance
			uint _id;
			ar >> _id;

			LayerPtr _parent;
			ar >> _parent;

			VectorMapControllerPtr _vectorMapController;
			ar >> _vectorMapController;

			::new(t)VectorMapLayer( _id, _parent, _vectorMapController );
		}
	}
}

#endif