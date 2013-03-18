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

#include "../utils/Node.hpp"

#include "../utils/StringTools.hpp"
#include "../utils/Log.hpp"

using namespace GenGIS;
using namespace std;

GenGIS::Node::Node(const Node& rhs)
{
	Copy(rhs);
}

template<class Archive>
void GenGIS::Node::serialize(Archive & ar, const unsigned int version)
{
	ar & m_name;             // std::wstring
	ar & m_children;         // std::vector<Node*>
	ar & m_parent;           // Node*
	ar & m_distanceToParent; // float
	ar & m_leafNodeIndex;    // uint
}
template void GenGIS::Node::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version);
template void GenGIS::Node::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

GenGIS::Node& GenGIS::Node::operator=(const GenGIS::Node& rhs)
{
	if(this == &rhs)
		return *this;

	Copy(rhs);

	return *this;
}

void GenGIS::Node::Copy(const GenGIS::Node& node)
{
	m_id               = node.GetId();
	m_name             = node.GetName();
	m_parent           = node.GetParent();
	m_distanceToParent = node.GetDistanceToParent();
	m_children         = node.GetChildren();
}

std::vector<unsigned int> GenGIS::Node::GetChildrenIds() const
{
	std::vector<unsigned int> childrenId(m_children.size());
	for(unsigned int i = 0; i < m_children.size(); i++) 
	{
		childrenId.at(i) = m_children.at(i)->GetId();
	}

	return childrenId;
}

GenGIS::Node* GenGIS::Node::GetChild(unsigned int pos) const
{
	assert(pos < m_children.size());
	return m_children.at(pos);
} 

void GenGIS::Node::AddChild(unsigned int pos, Node* node)
{
	m_children.insert(m_children.begin() + pos, node);
	node->SetParent(this);
}

void GenGIS::Node::AddChild(Node* node)
{
	m_children.push_back(node);
	node->SetParent(this);
}

void GenGIS::Node::RemoveChild(unsigned int pos)
{
	assert(pos < m_children.size());
	m_children.erase(m_children.begin() + pos);
}

void GenGIS::Node::RemoveChildren() 
{  
	while(m_children.size() != 0) 
		m_children.erase(m_children.begin()); 
}

void GenGIS::Node::RemoveChild(Node* node)
{
	for(unsigned int i = 0; i < m_children.size(); i++)
	{
		if(m_children.at(i) == node)
		{
			m_children.erase(m_children.begin() + i);
			return;
		}
	}
}
void GenGIS::Node::Swap(unsigned int pos1, unsigned int pos2)
{
	Node* node1 = GetChild(pos1);
	Node* node2 = GetChild(pos2);

	// remove higher position item first!
	if(pos1 < pos2)
	{
		RemoveChild(pos2);
		RemoveChild(pos1);
	}
	else
	{
		RemoveChild(pos1);
		RemoveChild(pos2);
	}

	AddChild(pos1, node2);
	AddChild(pos2, node1);
}


vector<GenGIS::Node*> GenGIS::Node::GetNeighbors() const
{
	vector<Node*> neighbors;

	Node* parent = GetParent();

	for(unsigned int i = 0; i < parent->GetNumberOfChildren(); i++) 
	{
		if(parent->GetChild(i) != this)
			neighbors.push_back(parent->GetChild(i));
	}

	return neighbors;
}

unsigned int GenGIS::Node::GetChildPosition(GenGIS::Node* child) const
{
	for(unsigned int i = 0; i < m_children.size(); i++) 
	{
		if(m_children.at(i) == child) 
			return i;
	}

	Log::Inst().Error("GetChildPosition(): invalid child.");
	return m_children.size();	// invalid position
}
