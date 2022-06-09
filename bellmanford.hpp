#ifndef HWDG_BELLMANFORD_HPP
#define HWDG_BELLMANFORD_HPP

/**
* @file bellmanford.hpp
* @author Jakub Grzana
* @date March 2022
* @brief Implementation of Bellman-Ford algorithm for pathfinding.
*
* This file contains class BellmanFord, which is written in horrible yet consistent way with Dijkstra.
*/

#include <cstdint>
#include <vector>
#include "node.hpp"
#include "graph.hpp"
#include "custom_map.hpp"
#include "path.hpp"

namespace HWDG
{
	/**
	* Static class implementing Bellman-Ford algorithm for pathfinding.
	* Because it's static class, you can't create objects of it. You are supposed only to call BellmanFord::Compute() static function.
	*/
	class BellmanFord
	{
		private:
			Pathtable<PathtableCell> _results;
		private:
			bool has(const Node& node) const;
			const PathtableCell& getCell(const Node& node) const;
			void UpdateWeight(const Node& node, const Node& prev_node, const double& pathweight);
			void Algorithm(const Graph& graph);
		private:
			BellmanFord() = delete;
			BellmanFord(const Graph& graph, const Node& src);
			Path GetPath(const Node& target) const;
			const Pathtable<PathtableCell>& Results(void) const;
		public:
			/**
			* Execute Bellman-Ford algorithm. 
			* It calculated shortest path from source Node to every other Node in a Graph.
			* 
			* Bellman-Ford algorithm is slower than Dijkstra, but it can work with negative weights. However, graph cannot contain negative cycles.
			*
			* Negative cycle is path within graph that has total negative weight. If you can get from A to A through B with total weight -2, then you can go another cycle to get -4, then -6 and so on. This is negative cycle.
			* So it's impossible to establish shortest path in Graph with negative cycles, because shortest path depends on number of iterations over negative cycles.
			*
			* Bellman-Ford algorithm can be used to detect negative cycles. If it throws std::invalid_argument, then there's negative cycle.
			* 
			* \param graph Graph that you want to find Pathtable for.
			* \param src Source Node, for which the Pathtable will be created.
			* \return Pathtable with paths from source Node to every other Node within Graph.
			* \throws std::invalid_argument if Graph has negative cycles.
			* 
			* \par Time complexity:
			* \f$O(nodes^2)\f$
			*/
			static const Pathtable<PathtableCell> Compute(const Graph& graph, const Node& src);
	}; 
}

#endif
