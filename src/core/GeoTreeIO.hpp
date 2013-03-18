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

#ifndef _GENGIS_GEO_TREE_IO_
#define _GENGIS_GEO_TREE_IO_

#include "../core/Precompiled.hpp"

#include "../utils/Tree.hpp"
#include "../core/NodeGeoTree.hpp"

namespace GenGIS
{

	/**
	* @brief Read/write data in geogrpahic tree model (GTM) format.
	*/
	class GeoTreeIO
	{
	public:
		/** Function signature for LoadingProgress event. */
		typedef boost::signal<bool (int progress, const std::string& msg, bool* skip)> OnLoadingProgress;

	public:		
		/** 
		* @brief Constructor. 
		* @param layerTreeController Layer tree controller which will contain geographic tree.
		*/
		explicit GeoTreeIO(): m_numElement(0), m_processedElement(NULL) {}

		/**
		* @brief Read a phylogenetic tree from a stream.
		*
		* @param tree Tree to populate from file.
		* @param in The input stream.
		* @return True if tree loaded successfully, false loading was cancelled.
		*/
		bool Read(Tree<NodeGeoTree>::Ptr tree, std::istream& in);

		/**
		* @brief Read a phylogenetic tree from a file.
		*
		* @param tree Tree to populate from file.
		* @param filename The file path.
		* @return True if tree loaded successfully, false loading was cancelled.
		*/
		bool Read( Tree<NodeGeoTree>::Ptr tree, const std::string& filename );

		/**
		* @brief Parse a string in Newick format and convert it to a tree.
		*
		* @param tree Tree to populate from file.
		* @param description String to parse.
		* @return True if tree loaded successfully, false loading was cancelled.
		*/
		bool ParseTree(Tree<NodeGeoTree>::Ptr tree, const std::string& description);

		/**
		* @brief Parse a string in geographic tree model (GTM) format.
		*
		* @param tree Tree to populate with geographic information.
		* @param description String to parse.
		* @param bGeoBlock Indicate if GTM file contains an explicit geographic information block.
		* @return True if successfully, false otherwise.
		*/
		bool ParseGeo(Tree<NodeGeoTree>::Ptr tree, const std::string& description, bool bGeoBlock);

		/**
		* @brief Write a phylogenetic tree to a stream.
		*
		* @param tree Tree to write out to file.
		* @param out The output stream.
		* @throw IOException If output stream is invalid
		*/
		void Write(Tree<NodeGeoTree>::Ptr tree, std::wostream & out) const; 

		/**
		* @brief Write a phylogenetic tree to a file.
		*
		* @param tree Tree to write out to file.
		* @param filename The file path.
		* @param overwrite Tell if existing file should be overwritten or appended to.
		* @throw IOException If file can not be opened.
		*/
		void Write(Tree<NodeGeoTree>::Ptr tree, const std::string & filename, bool overwrite = true) const
		{
			std::wofstream output(filename.c_str(), overwrite ? (std::ios::out) : (std::ios::out|std::ios::app));
			Write(tree, output); 
			output.close();
		}

		/**
		* @brief Get Newick string representation of tree.
		*
		* @param tree Tree to get Newick representation of.
		*/
		std::wstring GetNewickString(Tree<NodeGeoTree>::Ptr tree) const
		{
			std::wostringstream stream;
			Write(tree, stream);
			return stream.str();
		}


		/**
		* @brief Set a slot (callback) to indicate loading progress.
		* @param slot Slot (function) to call.
		*/
		void LoadingProgress(const OnLoadingProgress::slot_type& slot) { m_conLoadingProgress = m_sigLoadingProgress.connect(slot); }

	protected:
		/**
		* @brief Different elements that comprise a Newick node.
		*/
		struct Element
		{
			std::string node;
			std::string name;
			std::string length;
			std::string bootstrap;
		};

		/**
		* @brief Parse a string in Newick format and return a single paranthesized element.
		*
		* @param description String to parse.
		* @return Newick element
		*/
		Element Elements(const std::string& description);

		/**
		* @brief Parse a string in Newick format and convert it to a subtree.
		*
		* @param tree Tree to populate from file.
		* @param description String to parse.
		* @param parentID Id of parent node.
		* @return True if tree loaded successfully, false if loading was cancelled.
		*/
		bool ParseNode(Tree<NodeGeoTree>::Ptr tree, const std::string& description, NodeGeoTree* parent);

		/**
		* @brief Write elements of a node to file in Newick format.
		*
		* @param tree Tree to write out to file.
		* @param out The output stream.
		* @param parentID Index of parent node.
		* @param childID Node structure of the child node.
		*/
		void WriteElements(Tree<NodeGeoTree>::Ptr tree, std::wostream& out, NodeGeoTree* parent, NodeGeoTree* child) const;

		/**
		* @brief Write each node to file in Newick format.
		*
		* @param tree Tree to write out to file.
		* @param out The output stream.
		* @param parentID Index of parent node.
		*/
		void WriteNodes(Tree<NodeGeoTree>::Ptr tree, std::wostream& out, NodeGeoTree* parent) const;

		/**
		* @brief Parse geographic tree model (GTM) block.
		*
		* @param tree Tree to populate with geographic information.
		* @param description String to parse.
		* @return True if successfully, false otherwise.
		*/
		bool ParseGeoBlock(Tree<NodeGeoTree>::Ptr tree, const std::string& description);

	protected:
		/** Signal used to indicate loading progress. */
		OnLoadingProgress m_sigLoadingProgress;

		/** Flag used to see if there is a slot connected to this loading progress signal. */
		boost::signals::connection m_conLoadingProgress;

		/** Number of elements to be read from the file (useful for indicating loading progress). */
		uint m_numElement;

		/** Track number of elements processed so far (useful for indicating loading progress and assigning unique node ids). */
		uint m_processedElement;
	};

} 

#endif

