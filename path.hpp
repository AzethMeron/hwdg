#ifndef HWDG_PATH_HPP
#define HWDG_PATH_HPP

/**
* @file path.hpp
* @author Jakub Grzana
* @date March 2022
* @brief Structures and classes for Pathfinding
*
* This file contains classes Pathtable, PathTableCell and Path, used by all pathfinding algorithms.
*/

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
#include "custom_map.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "tools.hpp"

namespace HWDG
{
	/**
	* Low level, cell within Pathtable, representing previous Node in path, and weight of path to this point. 
	* 
	* You're not supposed to actually use that class, it's used internally by Dijkstra and BellmanFord algorithms. That being said, if you want to salvage some information out of results, feel free to do so.
	*/
	struct PathtableCell
	{
		//! Node corresponding to this cell
		const Node node;  
		//! Sum of weights leading to this point
		double pathweight; 

		/** 
		 * Node::id() of previous Node in path
		 *
		 * PathtableCell::NO_PREVIOUS if unaccessible
		 *
		 * PathtableCell::STARTING if it's starting Node*/
		int64_t prev_id; 

		/**
		 * Get string representation.
		 * Useful during development and debugging.
		 * \par Time complexity:
		 * \f$O(1)\f$
		 */
		std::string str(void) const;

		//! PathtableCell must be initialised, so default constructor is removed.
		PathtableCell() = delete; 

		PathtableCell(const Node& n, const Node& src);
		PathtableCell(const Node& n, const double& weight, int64_t prev_id);

		/**
		* Save to stream - text form.
		* \param file Reference to ostream-like object.
		* \param cell PathtableCell to be saved.
		* \par Time complexity:
		* \f$O(1)\f$
		*/
		static void SaveTxt(std::ostream& file, const PathtableCell& cell);

		/**
		* Load from stream - text form.
		* \param file Reference to istream-like object.
		* \return PathtableCell loaded from stream.
		* \par Time complexity:
		* \f$O(1)\f$
		*/
		static PathtableCell LoadTxt(std::istream& file);
		
		/**
		* Save to stream - binary form.
		* \param file Reference to ostream-like object, opened in binary mode.
		* \param cell PathtableCell to be saved.
		* \par Time complexity:
		* \f$O(1)\f$
		*/
		static void SaveBin(std::ostream& file, const PathtableCell& cell);

		/**
		* Load from stream - binary form.
		* \param file Reference to istream-like object, opened in binary mode.
		* \return PathtableCell loaded from stream.
		* \par Time complexity:
		* \f$O(1)\f$
		*/
		static PathtableCell LoadBin(std::istream& file);

		//! Dummy ID number, used when there's no previous node (unaccessible, or not found yet)
		static constexpr int64_t NO_PREVIOUS = -1;
		//! Dummy ID number, used to distinguish starting node
		static constexpr int64_t STARTING = -2;
	};

	/**
	* High-level class to extract Path from Pathtable.
	* 
	* Path from Node A to B extracted from Pathtable or returned by pathfinding algorithm. Supports indexed and range loops.
	*/
	class Path
	{
		private:
			std::vector<Node> _nodes;
			double _pathweight;
			bool _exists;
		public:
			/**
			* Number of nodes within Path. Includes starting and target Node.
			* \return Number of nodes on path from source to target Node.
			* \par Time complexity:
			* \f$O(1)\$
			*/
			size_t size(void) const;

			/**
			* Support for C++ range loops.
			*/
			typename std::vector<Node>::const_iterator begin(void) const;

			/**
			* Support for C++ range loops.
			*/
			typename std::vector<Node>::const_iterator end(void) const;

			/**
			* Get Node under given index within path.
			* \param i Index within Path, must belong to [0,size()-1]
			* \return Node under given index within path.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			const Node& operator [] (const unsigned int& i) const;

			/**
			* Get total weight of Path.
			* \return Sum of all edges that are taken by this Path.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			double Weight(void) const;

			/**
			* Check whether Path actually exists.
			* \return true, if target Node is accessible from source (path exists)
			* \return false, if target Node is NOT accessible from source (path doesn't exists)
			*/
			bool Exists(void) const;

			/**
			* Get string representation.
			* Useful during development and debugging.
			* \par Time complexity:
			* \f$O(n)\f$
			*/
			std::string str(void) const;

			/**
			* Removed default constructor.
			*/
			Path() = delete;
			Path(const std::vector<Node>& nodes, const double& weight, bool exists);

			/**
			* Save to stream - text form.
			* \param file Reference to ostream-like object.
			* \param path Path to be saved.
			* \par Time complexity:
			* \f$O(n)\f$
			*/
			static void SaveTxt(std::ostream& file, const Path& path);

			/**
			* Load from stream - text form.
			* \param file Reference to istream-like object.
			* \return Path loaded from stream.
			* \par Time complexity:
			* \f$O(n)\f$
			*/
			static Path LoadTxt(std::istream& file);

			/**
			* Save to stream - binary form.
			* \param file Reference to ostream-like object, opened in binary mode.
			* \param path Path to be saved.
			* \par Time complexity:
			* \f$O(n)\f$
			*/
			static void SaveBin(std::ostream& file, const Path& path);

			/**
			* Load from stream - binary form.
			* \param file Reference to istream-like object, opened in binary mode.
			* \return Patj loaded from stream.
			* \par Time complexity:
			* \f$O(n)\f$
			*/
			static Path LoadBin(std::istream& file);
	};

	template<typename TYPE>
	struct Pathtable : public Map::unordered_map<uint32_t, TYPE>
	{
		const Node source;
		Pathtable() = delete;
		Pathtable(const Node& src) : source(src) {}

		bool has(const Node& node) const
		{
			auto iter = this->find(node.id());
			if (iter == this->end()) return false;
			return true;
		}

		void UpdateWeight(const Node& node, const Node& prev_node, const double& pathweight)
		{
			TYPE& c = (*this->find(node.id()));
			c.pathweight = pathweight;
			c.prev_id = prev_node.id();
		}

		const TYPE& getCell(const Node& node) const
		{
			return (*this->find(node.id()));
		}

		TYPE& getCell(const Node& node)
		{
			return (*this->find(node.id()));
		}

		Path GetPath(const Node& tgt) const
		{
			if (!this->has(tgt)) throw std::invalid_argument(string_format("No node %s in graph this algorithm was used on", tgt.str()));
			std::vector<Node> nodes;
			const double weight = this->getCell(tgt).pathweight;
			const bool exists = this->getCell(tgt).prev_id != TYPE::NO_PREVIOUS;
			Node analysing = tgt;
			nodes.push_back(analysing);
			while (true)
			{
				const TYPE& c = this->getCell(analysing);
				if ((c.prev_id == TYPE::NO_PREVIOUS) || (c.prev_id == TYPE::STARTING)) break;
				analysing = Node(c.prev_id);
				nodes.push_back(analysing);
			}
			std::reverse(nodes.begin(), nodes.end());
			return Path(nodes, weight, exists);
		}
		
		void Initialise(const Graph& graph, const Node& src)
		{
			this->reserve(graph.size_nodes());
			for (const Node& node : graph)
			{
				this->insert({ node.id(), TYPE(node, src) });
			}
		}

		std::string str(void) const
		{
			std::string output = "";
			for (const auto& cell : *this)
			{
				output = output + cell.str() + "\n";
			}
			return output;
		}

		static void SaveTxt(std::ostream& file, const Pathtable<TYPE>& table)
		{
			Node::SaveTxt(file, table.source);
			file << table.size() << ' ';
			for (const auto& cell : table)
			{
				TYPE::SaveTxt(file, cell);
			}
		}

		static Pathtable<TYPE> LoadTxt(std::istream& file)
		{
			Node node = Node::LoadTxt(file);
			Pathtable<TYPE> output(node);
			size_t size;
			file >> size;
			output.reserve(size);
			for (size_t i = 0; i < size; ++i)
			{
				TYPE cell = TYPE::LoadTxt(file);
				output.insert({ cell.node.id(), cell });
			}
			return output;
		}

		static void SaveBin(std::ostream& file, const Pathtable<TYPE>& table)
		{
			Node::SaveBin(file, table.source);
			size_t size = table.size();
			file.write((const char*)&size, sizeof(size));
			for (const auto& cell : table)
			{
				TYPE::SaveBin(file, cell);
			}
		}

		static Pathtable<TYPE> LoadBin(std::istream& file)
		{
			Node source = Node::LoadBin(file);
			Pathtable<TYPE> output(source);
			size_t size = 0;
			file.read((char*)&size, sizeof(size));
			output.reserve(size);
			for (size_t i = 0; i < size; ++i)
			{
				TYPE cell = TYPE::LoadBin(file);
				output.insert({ cell.node.id(), cell });
			}
			return output;
		}
	};
}

#endif