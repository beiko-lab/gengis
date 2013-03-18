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

#ifndef _GENGIS_LAYER_
#define _GENGIS_LAYER_

#include "../core/Precompiled.hpp"

class GenGIS::Layer;
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::Layer * t, const unsigned int file_version);
	}
}

namespace GenGIS
{
	/**
	* @brief Abstract base class representing a generic GIS layer.
	*/
	class Layer 
	{
	public:
		/** All supported layer types. */
		enum LAYER_TYPE { STUDY, MAP, LOCATION_SET, TREE, LOCATION, SEQUENCE, VECTOR_MAP };

		/**
		* @brief Get string which indicates the layer type.
		* @param type Flag indicating layer type.
		* @return String indicating layer type.
		*/
		static std::wstring GetLayerTypeString(LAYER_TYPE type)
		{
			if      (type == STUDY)        return _T("Study");
			else if (type == MAP)          return _T("Map");
			else if (type == VECTOR_MAP)    return _T("Vector Map");

			else if (type == LOCATION_SET) return _T("Location Set");
			else if (type == TREE)         return _T("Tree");
			else if (type == LOCATION)     return _T("Location");
			else if (type == SEQUENCE)     return _T("Sequence");

			return _T("Unknown layer type! Problem in Layer::GetLayerTypeString()!");
		}

		/** 
		* @brief Constuctor.
		* @param id Unique string to identify layer.
		* @param type Type of layer.
		* @param parent Parent of this layer.
		*/
		explicit Layer(uint id, LAYER_TYPE type, LayerPtr parent) :
			m_id( id ),
			m_wxTreeItemId( NULL ),
			m_type( type ),
			m_parent( parent ),
			m_name( _T( "" ) ),
			m_description( _T( "" ) ),
			m_authours( _T( "" ) ),
			m_path( wxT( "" ) ),
			m_expanded( false ),
			m_bPropertiesDialogOpen( false ),
			m_currentPropertiesDialog( NULL ) {};

		/** Destructor. */
		virtual ~Layer();

		/** Get id of layer. */
		virtual uint GetId() const { return m_id; }

		/** Set id of layer. */
		virtual void SetId(uint id) { m_id = id; }

		/** Get the layer tree (wxTreeCtrl) id of this layer. */
		wxTreeItemId GetWXTreeItemId() { return m_wxTreeItemId; }

		/** Set the layer tree (wxTreeCtrl) id of this layer. */
		void SetWXTreeItemId( wxTreeItemId _wxTreeItemId ) { m_wxTreeItemId = _wxTreeItemId; }

		/** Get name of layer. */
		virtual std::wstring GetName() const { return m_name; }

		/** Get name of layer. */
		virtual wxString GetWXName() const { return wxString( m_name.c_str() ); }

		/** Set name of layer. */
		virtual void SetName(const wxString& name) { m_name = name.wc_str(*wxConvCurrent); }

		/** Get description of layer. */
		virtual std::wstring GetDescription() const { return m_description; }

		/** Set description of layer. */
		virtual void SetDescription(const std::wstring& description) { m_description = description; }

		/** Get authours of layer. */
		virtual std::wstring GetAuthours() const { return m_authours; }

		/** Set authours of layer. */
		virtual void SetAuthours(const std::wstring& authours) { m_authours = authours; }

		/** Get filename (excluding directory) of layer. */
		virtual std::wstring GetFilename() const;

		/** Set filename (excluding directory) of layer. */
		virtual void SetFilename(const std::wstring& filename);

		/** Get path (excluding filename) of layer. */
		virtual std::wstring GetPath() const;

		/** Set path (excluding filename) of layer. */
		virtual void SetPath(const std::wstring& path);

		/** Get full path (directory + filename) of layer. */
		virtual wxString GetFullPath() const;

		/** Set full path (directory + filename) of layer. */
		virtual void SetFullPath(const wxString& path);

		/** Get type of layer. */
		virtual LAYER_TYPE GetType() const { return m_type; }

		/** 
		* @brief Set active state of layer.
		* @param state Active state of layer.
		*/
		virtual void SetActive(bool state) { return; }

		/** Get active state of layer. */
		virtual bool IsActive() const { return false; }

		/** Toggle active state of layer. */
		virtual void ToggleActive() { return; }

		/** 
		* @brief Set layer state (expanded/ not expanded).
		* @param state Desired state of layer (expanded/ not expanded).
		*/
		virtual void SetExpanded(bool state) { m_expanded = state; }

		/** Determine whether layer is expanded. */
		virtual bool IsExpanded() const { return m_expanded; }

		/** Toggle expanded state of layer. */
		virtual bool ToggleExpanded() { m_expanded = !m_expanded; return m_expanded; }

		/** Determine whether the corresponding properties dialog is open. */
		virtual bool HasPropertiesDialogOpen() { return m_bPropertiesDialogOpen; }


		virtual void SetPropertiesDialogOpenStatus( bool status ) { m_bPropertiesDialogOpen = status; }


		virtual void SetPropertiesDialog( wxDialog* dialog ) { m_currentPropertiesDialog = dialog; }


		virtual wxDialog* GetPropertiesDialog() { return m_currentPropertiesDialog; }


		/**
		* @brief Get layer below this layer in the tree. 
		* @param id Unique id of desired layer.
		* @return layer at given id.
		*/
		virtual LayerPtr GetLayer(uint id) const { return LayerPtr(); }

		/**
		* @brief Remove layer below this layer from the tree. 
		* @param id Unique id of layer to remove.
		* @return True if layer removed, false if layer not found.
		*/
		virtual bool RemoveLayer(uint id) { return false; }

		/** Get parent layer. */
		virtual LayerPtr GetParent() const { return m_parent; }

		/** Render contents of layer. */
		virtual void Render() {}

	private:
		/** Serialization. */
		friend class boost::serialization::access;

		template<class Archive>
		friend void boost::serialization::save_construct_data(Archive & ar, const Layer * t, const unsigned int file_version);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version);

		template<class Archive>
		void load(Archive & ar, const unsigned int version);

		template<class Archive>
		void save(Archive & ar, const unsigned int version) const;

	protected:
		/** Unique number identifying this layer. */
		uint m_id;

		/** Layer tree (wxTreeCtrl) id of this layer.
		    Stored here for convenience, not to be serialized. */
		wxTreeItemId m_wxTreeItemId;

		/** Type of layer (i.e., study, map, location set, ...). */
		LAYER_TYPE m_type;

		/** Pointer to parent of this layer. */
		LayerPtr m_parent;

		/** Name of layer. */
		std::wstring m_name;

		/** Description of layer. */
		std::wstring m_description;

		/** Authours of layer. */
		std::wstring m_authours;

		/** Path to file associated with this layer. */
		//std::wstring m_path;
		wxFileName m_path;

		/** Indicate whether layer is currently expanded. */
		bool m_expanded;

		/** Indicate wither the corresponding properties dialog is currently open. */
		bool m_bPropertiesDialogOpen;


		wxDialog* m_currentPropertiesDialog;
	};
}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const GenGIS::Layer * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			//uint id = t->GetId(); // m_id (uint)
			//ar << id;     
			//ar << t->GetType();   // m_type (LAYER_TYPE)
			//ar << t->GetParent(); // m_parent (LayerPtr)

			ar << t->m_id;     // uint
			ar << t->m_type;   // LAYER_TYPE
			ar << t->m_parent; // LayerPtr
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, GenGIS::Layer * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			uint _id;
			ar >> _id;

			GenGIS::Layer::LAYER_TYPE _type;
			ar >> _type;

			LayerPtr _parent;
			ar >> _parent;

			::new(t)GenGIS::Layer(_id, _type, _parent);
		}
	}
}

#endif
