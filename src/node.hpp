#ifndef HWDG_NODE_HPP
#define HWDG_NODE_HPP

/**
* @file node.hpp
* @author Jakub Grzana
* @date March 2022
* @brief Representation of Node (Vertex) detached from graph
* 
* This file contains Node class, which is representation of node/vertex outside of Graph context.
*/

#include <cstdint>
#include <string>
#include <limits>
#include <iostream>

namespace HWDG
{
	/**
	* @brief Representation of node (vertex) outside of Graph context.
	* 
	*/
	class Node 
	{
		private:
			uint32_t _id;
		public:
			/**
			* @brief Get unique identifier.
			* \return Identifier of node
			* \par Time complexity: O(1)
			*/
			const uint32_t& id(void) const;

			/**
			* @brief Nodes cannot be initialised without identifier provided.
			* Default constructor is removed to prohibit that.
			*/
			Node() = delete;

			/**
			* @brief Create new node with given identifier.
			* ID must be unique: two objects of Node class with the same ID are considered the same node.
			*/
			Node(const uint32_t& id);

			/** 
			* @brief Compare nodes.
			* \return true if ID of lvalue is equal to ID of rvalue, false otherwise
			* \par Time complexity: O(1)
			*/
			bool operator == (const Node& rval) const;

			/** 
			* @brief Get string representation.
			* Useful during development and debugging.
			*/
			std::string str(void) const;

			/**
			* @brief Minimal value that can be used as identifier.
			*/
			static constexpr uint32_t MIN_ID = std::numeric_limits<uint32_t>::min();

			/**
			* @brief Maximal value that can be used as identifier.
			*/
			static constexpr uint32_t MAX_ID = std::numeric_limits<uint32_t>::max();

			/**
			* @brief Support for C++ STL hashing.
			* Allows nodes to be stored in STL sets. Hash of Node is equal its' identifier.
			*/
			struct HashFunction
			{
				uint32_t operator()(const Node& node) const;
			};

			/**
			* @brief Save to stream - binary form.
			* \param file Reference to ostream-like object, opened in binary mode.
			* \param node Node to be saved.
			*/
			static void SaveBin(std::ostream& file, const Node& node);
			
			/**
			* @brief Load from stream - binary form
			* \param file Reference to istream-like object, opened in binary mode.
			* \return Node loaded from stream.
			*/
			static Node LoadBin(std::istream& file);
			
			static void SaveTxt(std::ostream& file, const Node& node);
			
			static Node LoadTxt(std::istream& file);
	};
}

#endif
