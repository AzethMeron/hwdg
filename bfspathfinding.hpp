#ifndef HWDG_BFSPATHFINDING_HPP
#define HWDG_BFSPATHFINDING_HPP

/**
* @file bfspathfinding.hpp
* @author Jakub Grzana
* @date June 2022
* @brief Implementation of BFS pathfinding algorithm (ignores weights!)
*/

#include "node.hpp"
#include "edge.hpp"
#include "graph.hpp"
#include "path.hpp"

namespace HWDG
{
	/**
	* Static class implementing BFS algorithm for pathfinding.
	* Read more in BFSPathfinding::Compute()
	*/
	class BFSPathfinding
	{
		public:
			/**
			* Weight used by algorithm instead of actual weight.
			* Most useful when left as 1.
			*/
			static constexpr float EdgeWeight = 1;
			/**
			* BFS-based algorithm for pathfinding. Ignores weights.
			* Uses Operations::BreadthFirstSearch() to traverse minimal number of edges you need to traverse, to reach any node within a graph.
			* 
			* Completely ignores weight of edges. Calculated pathweights represent number of edges you need to traverse, not the real pathweight. 
			*
			* Results are only accurate for graphs with all edges having the same (positive) weight.
			* Thus, it can be used in unweighted graphs as faster alternative to Dijkstra or BellmanFord. 
			* 
			* \par Time complexity:
			* \f$O(edges)\f$
			*/
			static Pathtable<PathtableCell> Compute(const Graph& graph, const Node& src);
			BFSPathfinding() = delete;
	};
}

#endif