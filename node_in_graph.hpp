#ifndef HWDG_NODE_IN_GRAPH_HPP
#define HWDG_NODE_IN_GRAPH_HPP

#include <cstdint>
#include <string>
#include "custom_map.hpp"
#include "node.hpp"
#include "edge.hpp"

/**
* @file node_in_graph.hpp
* @author Jakub Grzana
* @date March 2022
* @brief NodeInGraph - Node within Graph
*
* This file contains NodeInGraph class, which is representation of node/vertex inside Graph's context. 
*/

namespace HWDG
{
	/**
	* Representation of node/vertex inside Graph's context. 
	* Aside from current Node, it includes list of Edge s coming out from this node.
	*/
	class NodeInGraph : public Node
	{
		private:
			Map::unordered_map<uint64_t, Edge> _edges;
		public:
			/**
			* Check whether there's Edge edge coming out from this Node.
			* \param edge Edge that we check for.
			* \return true if this node has given edge coming out.
			* \par Time complexity: 
			* \f$O(1)\f$
			*/
			bool has(const Edge& edge) const; 
			/**
			* Add neighbour to Node. 
			* \param edge Edge leading to neighbour.
			* \return true if neighour was added, otherwise false.
			* \par Time complexity: 
			* \f$O(1)\f$
			*/
			bool add(const Edge& edge); 
			/**
			* Add neighbour to Node.
			* \param tgt_node Node to which Edge will lead to.
			* \param weight Weight of Edge leading from this node to tgt_node.
			* \return true if neighour was added, otherwise false.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			bool add(const Node& tgt_node, float weight); // constant
			/**
			* Remove Edge coming out from this node.
			* \param edge Edge to be removed.
			* \return true if successfully removed, otherwise false.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			bool remove(const Edge& edge); // constant
			
			Map::const_iterator<uint64_t, Edge> begin() const;
			Map::const_iterator<uint64_t, Edge> end() const;

			/**
			* Get number of neighbours (edges)
			* \return Number of edges (neighbours)
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			size_t size_edges(void) const; // constant
			
			/**
			* Get string representation.
			* Useful during development and debugging.
			* \par Time complexity:
			* \f$O(edges)\f$
			*/
			std::string str(void) const; // linear

			/**
			* Nodes cannot be initialised without identifier provided.
			* Default constructor is removed to prohibit that.
			*/
			NodeInGraph() = delete;

			/**
			* Conversion from Node into NodeInGraph. 
			* \param n Node to be converted.
			*/
			NodeInGraph(const Node& n);

			/**
			* Reserve memory for edges (neighbours) Used correctly, it can improve performance (for example, if you want to add neighbours in bulk), but used without understanding causes only problems. 
			* 
			* Use ONLY if you know EXACT amount of edges you'll add.
			* \param count Total number of edges that can be stored in internal list (map) 
			*/
			void reserve_edges(const size_t& count);
	};

}

#endif