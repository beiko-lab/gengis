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
#include "../core/GeoTreeIO.hpp"
#include "../core/StudyLayer.hpp"
#include "../core/LocationLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/LayerTreeController.hpp"
#include "../core/LocationController.hpp"
#include "../core/StudyController.hpp"

#include "../utils/Log.hpp"
#include "../utils/StringTools.hpp"
#include "../utils/NewickTokenizer.hpp"
#include "../utils/StringTools.hpp"

using namespace GenGIS;
using namespace std;

bool GeoTreeIO::Read(Tree<NodeGeoTree>::Ptr tree, istream & in)
{	
	m_numElement = 0;
	m_processedElement = 0;

	// Checking the existence of specified input stream
	if(!in)
	{
		Log::Inst().Warning("(Warning) Failed to open GTM file.");
		return false;
	}	

	// Read entire tree description from input stream (tree description ends with a semicolon)
	string temp, treeDescription;
	while (!in.eof())
	{
		getline(in, temp); 
		string::size_type index = temp.find(";");
		if(index != string::npos)
		{
			treeDescription += temp.substr(0, index + 1);
			break;
		}
		else treeDescription += temp;
	}
	treeDescription = StringTools::RemoveSubstrings(treeDescription, '[', ']');

	// Read in geographical data (all data between curly brackets)
	string geoDescription;
	bool bReadData = false;
	bool bGeoBlock = false;
	while (!in.eof())
	{
		getline(in, temp); 

		if(temp.find("GEO_LOCATION_BLOCK") != string::npos)
		{
			bGeoBlock = true;
		}

		if(temp.find("}") != string::npos)
		{
			break;
		}

		if(bReadData)
		{
			geoDescription += temp;
			geoDescription += '\n';
		}

		if(temp.find("{") != string::npos)
		{
			bReadData = true;
		}
	}

	bool bSuccess = ParseTree(tree, treeDescription);
	if(bSuccess)
	{
		bSuccess = ParseGeo(tree, geoDescription, bGeoBlock);
	}

	return bSuccess;
}

bool GeoTreeIO::Read( Tree<NodeGeoTree>::Ptr tree, const string& filename )
{
	// Set name of tree to the filename
	wxFileName fullPath( wxString( filename.c_str(), wxConvUTF8 ) );
	tree->SetName( fullPath.GetName().wc_str( *wxConvCurrent ) );

	// Parse Newick file
	ifstream input( fullPath.GetFullPath().mb_str(), std::ios::in );
	bool bLoaded = Read(tree, input);
	input.close();

	return bLoaded;
}

void GeoTreeIO::Write(Tree<NodeGeoTree>::Ptr tree, wostream& out) const
{
	// Checking the existence of specified file, and possibility to open it in write mode
	assert(out != NULL);

	// Write out tree data in Newick format
	out << "(";

	NodeGeoTree* root = tree->GetRootNode();

	if(tree->GetNumberOfLeaves() == 0)
	{
		out << "'" << root->GetName().c_str() << "'";

		float dist = root->GetDistanceToParent();
		if(dist != NodeGeoTree::NO_DISTANCE)
			out << " " << dist;
	}
	else
	{
		WriteNodes(tree, out, root);
	}
	out << ")";

	// Output the name of the root if it has one
	if(!StringTools::IsEmpty(root->GetName()))
		out << "'" << root->GetName().c_str() << "'";

	out << ";" << endl;

	// Write out geographic data in GTM format
	out << endl;
	out << "GEO_LOCATION_BLOCK:" << endl;
	out << "{" << endl;

	std::vector<NodeGeoTree*> nodes = tree->GetNodes();
	foreach(NodeGeoTree* node, nodes)
	{
		if(node->GetLocationView() != LocationViewPtr())
		{
			out << node->GetName().c_str() << " ";
			out << node->GetLocationView()->GetLocationId().c_str() << " ";
		}
	}

	out << "}" << endl;
}

void GeoTreeIO::WriteNodes(Tree<NodeGeoTree>::Ptr tree, wostream& out, NodeGeoTree* parent) const
{
	vector<unsigned int> childrenId = parent->GetChildrenIds();
	assert(childrenId.size() > 0);

	WriteElements(tree, out, parent, parent->GetChild(0));
	for(uint i = 1; i < childrenId.size(); i++)
	{
		out << ",";
		WriteElements(tree, out, parent, parent->GetChild(i));
	}
}

void GeoTreeIO::WriteElements(Tree<NodeGeoTree>::Ptr tree, wostream& out, NodeGeoTree* parent, NodeGeoTree* child) const
{	
	if(child->GetNumberOfChildren() != 0)
	{
		out << "(";
		WriteNodes(tree, out, child); 
		out << ")";
	}

	if(!StringTools::IsEmpty(child->GetName()))
		out << "'" << child->GetName().c_str() << "'";

	if(child->GetBootstrapToParent() != NodeGeoTree::NO_DISTANCE)
		out << " " << ((NodeGeoTree*)child)->GetBootstrapToParent();

	if(child->GetDistanceToParent() != NodeGeoTree::NO_DISTANCE)
		out << ":" << child->GetDistanceToParent();
}

GeoTreeIO::Element GeoTreeIO::Elements(const string& description)
{
	Element element;
	element.node      = "";
	element.name      = "";
	element.length    = "";
	element.bootstrap = "";

	string desc = description;

	// Check if description is a node or leaf
	string::size_type lastP = desc.rfind(')');
	string::size_type firstP = desc.find('(');
	if(firstP != string::npos)
	{
		//This is a node:
		if(lastP < firstP)
		{
			wxMessageBox(_T("Incorrectly formated Newick string"), _T("Failed Newick String."), wxOK | wxICON_ERROR);
			return element;
		}

		element.node = desc.substr(firstP + 1, lastP - firstP - 1);
		desc = StringTools::RemoveSurroundingWhiteSpaces(desc.substr(lastP + 1));
	}

	// Check if this element has length
	string::size_type colon = desc.rfind(':');
	if(colon != string::npos)
	{
		element.length = StringTools::RemoveSurroundingWhiteSpaces(desc.substr(colon + 1));
		desc = StringTools::RemoveSurroundingWhiteSpaces(desc.substr(0, colon));
	}

	// Check for name and bootstrap value
	lastP = desc.rfind('\'');
	firstP = desc.find('\'');
	if(firstP != string::npos)
	{
		// Quoted name:
		element.name = desc.substr(firstP+1, lastP-firstP-1);
		element.name = StringTools::ReplaceChar(element.name, '_', ' ');
		element.bootstrap = StringTools::RemoveSurroundingWhiteSpaces(desc.substr(lastP+1));
	}
	else
	{
		// No quotations:
		firstP = desc.find(' ');
		if(firstP != string::npos)
		{
			// Parse the name and bootstrap value
			element.name = desc.substr(0, firstP-1);		
			element.bootstrap = StringTools::RemoveSurroundingWhiteSpaces(desc.substr(firstP+1));
		}
		else
		{
			// If the remaining description is a name. This makes sense at present since GenGIS 
			// doesn't use bootstrap values anyways.
			element.name = desc;
		}
	}

	return element;
}  

bool GeoTreeIO::ParseNode(Tree<NodeGeoTree>::Ptr tree, const string& description, NodeGeoTree* parent)
{
	Element element = Elements(description);
	m_processedElement++;

	// update loading progress and check if user has cancelled the load
	if(m_conLoadingProgress.connected())
	{
		bool bTemp;
		if(!m_sigLoadingProgress((int)(float(m_processedElement*100)/m_numElement + 0.5), "", &bTemp))
			return false;
	}

	// New node:
	NodeGeoTree* node(new NodeGeoTree(m_processedElement));
	parent->AddChild(node);

	node->SetName(_T(""));
	if(!StringTools::IsEmpty(element.name))
	{
		std::string name = StringTools::RemoveSurroundingWhiteSpaces(element.name);
		node->SetName(StringTools::ToStringW(name));
	}

	node->SetDistanceToParent(NodeGeoTree::NO_DISTANCE);
	if(!StringTools::IsEmpty(element.length))
	{
		node->SetDistanceToParent(StringTools::ToDouble(element.length));
	}

	node->SetBootstrapToParent(NodeGeoTree::NO_DISTANCE);
	if(!StringTools::IsEmpty(element.bootstrap))
	{
		node->SetBootstrapToParent(StringTools::ToDouble(element.bootstrap));
	}

	// Parse rest of tree
	NewickTokenizer nt(element.node);

	if(nt.NumTokens() != 1)
	{
		for(uint i = 0; i < nt.NumTokens(); i++)
		{
			if(!ParseNode(tree, nt.Token(i), node))
				return false;
		}
	}

	return true;
}

bool GeoTreeIO::ParseTree(Tree<NodeGeoTree>::Ptr tree, const string& description)
{
	string::size_type lastP  = description.rfind(')');
	if(lastP == string::npos)
	{
		wxMessageBox(_T("Missing closing parenthesis."), _T("Failed Newick String."), wxOK | wxICON_ERROR);
		return false;
	}

	string::size_type firstP = description.find('(');
	if(firstP == string::npos)
	{
		wxMessageBox(_T("Missing opening parenthesis."), _T("Failed Newick String."), wxOK | wxICON_ERROR);
		return false;
	}

	string::size_type semi = description.rfind(';');
	if(semi == string::npos)
	{
		wxMessageBox(_T("Missing closing semicolon."), _T("Failed Newick String."), wxOK | wxICON_ERROR);
		return false;
	}

	assert(lastP > firstP);

	string content = description.substr(firstP + 1, lastP - firstP - 1);
	string root_elements = description.substr(lastP + 1, semi - lastP - 1);

	// Quickly determine number of elements in the file.
	foreach(char ch, content)
	{
		if(ch == ',' || ch == ')')
			m_numElement++;
	}
	m_numElement++;

	// Track number of elements that have been processed
	m_processedElement = 0;

	// Create root node:
	NodeGeoTree* root(new NodeGeoTree(m_processedElement));
	tree->SetRootNode(root);
	root->SetDistanceToParent(Node::NO_DISTANCE);

	// Parse rest of tree
	NewickTokenizer nt(content);

	// The check for a parenthese is necessary as it is allowable to have
	// a tree rooted on a leaf in which case it should not be treated as a leaf.
	if(nt.NumTokens() == 1 && content.rfind("(") == string::npos)
	{    
		std::string name = StringTools::RemoveSurroundingWhiteSpaces(nt.Token(0));
		root->SetName(StringTools::ToStringW(name));
	}
	else
	{
		// This is a node.
		Element elements = Elements(root_elements);
		root->SetName(_T(""));
		if(!StringTools::IsEmpty(elements.name))
		{
			std::string name = StringTools::RemoveSurroundingWhiteSpaces(elements.name);
			root->SetName(StringTools::ToStringW(name));
		}

		for(uint i = 0; i < nt.NumTokens(); i++)
		{
			if(!ParseNode(tree, nt.Token(i), root))
				return false;
		}
	}

	return true;
}

bool GeoTreeIO::ParseGeo(Tree<NodeGeoTree>::Ptr tree, const string& description, bool bGeoBlock)
{
	if(bGeoBlock)
	{
		ParseGeoBlock(tree, description);
	}
	else
	{
		// find either sequence or location site with the same name as the leaf node
		std::vector<NodeGeoTree*> leafNodes = tree->GetLeaves();
		std::set<wstring> leafNames;
		foreach(NodeGeoTree* leafNode, leafNodes)
		{
			wstring leafName = leafNode->GetName();

			// make sure leaf name is unique
			if(leafNames.find(leafName) != leafNames.end())
			{
				std::wstring error = _T("(Error) Multiple leaf nodes with the name '") + leafName + _T("'.");
				Log::Inst().Error(error);
				return false;
			}
			else
			{
				leafNames.insert(leafName);
			}

			LocationViewPtr locationView;

			for(uint i = 0; i < App::Inst().GetLayerTreeController()->GetNumLocationLayers(); ++i)
			{
				LocationLayerPtr locationLayer = App::Inst().GetLayerTreeController()->GetLocationLayer(i);
				if(locationLayer->GetLocationController()->GetId() == leafName)
				{
					locationView = locationLayer->GetLocationController()->GetLocationView();
					break;
				}

				for(uint j = 0; j < locationLayer->GetNumSequenceLayers(); ++j)
				{
					if(wxString(locationLayer->GetSequenceLayer(j)->GetName().c_str()) == wxString(leafName.c_str()))
					{
						locationView = locationLayer->GetLocationController()->GetLocationView();
						break;
					}
				}

				if(locationView != LocationViewPtr())
					break;
			}

			if(locationView != LocationViewPtr())
			{
				// create bidirectional link between this leaf node and its associated location
				leafNode->SetLocationView(locationView);
				locationView->AddLeafNode(leafNode);
			}
			else
			{
				std::wstring error = _T("(Error) Unable to find a location with a site or sequence id of '") + leafName + _T("'.");
				Log::Inst().Error(error);
				return false;
			}
		}
	}

	// check that all leaf nodes have been assigned to a geographic location
	std::vector<NodeGeoTree*> leafNodes = tree->GetLeaves();
	foreach(NodeGeoTree* leaf, leafNodes)
	{
		if(leaf->GetLocationView() == LocationViewPtr())
		{
			std::wstring error = _T("(Error) No geographic location assigned to the leaf node '") + leaf->GetName() + _T("'.");
			Log::Inst().Error(error);
			return false;
		}
	}

	return true;
}

bool GeoTreeIO::ParseGeoBlock(Tree<NodeGeoTree>::Ptr tree, const string& description)
{
	std::vector< std::string > missingLeafNodes;

	int startIndex = 0;
	int stopIndex = description.find('\n');

	while(stopIndex != string::npos)
	{
		// get single GTM line
		string gtmData = description.substr(startIndex, stopIndex-startIndex);

		// parse line to obtain individual fields
		boost::tokenizer< boost::char_separator<char> > gtmDataItems(gtmData, boost::char_separator<char>(" \t"));
		boost::tokenizer< boost::char_separator<char> >::iterator dataItr = gtmDataItems.begin();     

		// add data to appropriate node
		std::string leafName = *dataItr;
		std::vector< std::wstring > names = tree->GetLeafNames();
		NodeGeoTree* node = tree->GetNode(StringTools::ToStringW(leafName));
		if(node)
		{
			// find location of site with the specified site id
			string siteId = *(++dataItr);
			LocationControllerPtr locationController = App::Inst().GetLayerTreeController()->GetLocationController(wstring(siteId.begin(), siteId.end()));
			if(locationController)
			{
				// create bidirectional link between this leaf node and its associated location
				node->SetLocationView(locationController->GetLocationView());
				locationController->GetLocationView()->AddLeafNode(node);
			}
			else
			{
				std::string warning = "(Warning) Unable to find a location with the site id = " + siteId + ".";
				Log::Inst().Warning(warning);
				return false;
			}
		}
		else
		{
			missingLeafNodes.push_back(leafName);
		}

		// move to next line
		startIndex = stopIndex+1;
		stopIndex = description.find('\n', startIndex);
	}

	// report any geographic block entries not in the tree
	if(!missingLeafNodes.empty())
	{
		std::string warning = "(Warning) The following geographic references are not in the tree:\n";
		foreach(std::string& leafName, missingLeafNodes)
		{
			warning += leafName + "\n";
		}

		Log::Inst().Warning(warning);
	}

	return true;
}
