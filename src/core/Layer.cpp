//=======================================================================
// Author: Timothy Mankowski
//
// Copyright 2011 Timothy Mankowski
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

#include "../core/Layer.hpp"

using namespace GenGIS;

GenGIS::Layer::~Layer()
{
}

template<class Archive>
void GenGIS::Layer::serialize(Archive & ar, const unsigned int version)
{
	boost::serialization::split_member(ar, *this, version);

	ar & m_name;        // std::wstring
	ar & m_description; // std::wstring
	ar & m_authours;    // std::wstring
	ar & m_expanded;    // bool	
}
template void GenGIS::Layer::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void GenGIS::Layer::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

template<class Archive>
void GenGIS::Layer::load(Archive & ar, const unsigned int version)
{
	std::wstring _path;
	ar & _path;
	m_path.Assign( _path.c_str() );
}
template void GenGIS::Layer::load<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

template<class Archive>
void GenGIS::Layer::save(Archive & ar, const unsigned int version) const
{
	std::wstring _path = m_path.GetFullPath().wc_str(*wxConvCurrent);
	ar & _path;
}
template void GenGIS::Layer::save<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version) const; 

/** Get filename (excluding directory) of layer. */
std::wstring GenGIS::Layer::GetFilename() const
{
	return std::wstring( m_path.GetFullName() );
}

/** Set filename (excluding directory) of layer. */
void GenGIS::Layer::SetFilename(const std::wstring& filename)
{
	m_path.SetFullName( wxString(filename.c_str()) );
}

/** Get path (excluding filename) of layer. */
std::wstring GenGIS::Layer::GetPath() const
{
	return std::wstring( m_path.GetPath() );
}

/** Set path (excluding filename) of layer. */
void GenGIS::Layer::SetPath(const std::wstring& path)
{
	m_path.AssignDir( wxString(path.c_str()) );
}

/** Get full path (directory + filename) of layer. */
wxString GenGIS::Layer::GetFullPath() const
{
	return m_path.GetFullPath();
}

/** Set full path (directory + filename) of layer. */
void GenGIS::Layer::SetFullPath(const wxString& path)
{
	m_path.Assign( path );
}
