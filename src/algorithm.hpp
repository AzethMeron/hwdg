#ifndef GRAPHS_ALGORITHM_HPP
#define GRAPHS_ALGORITHM_HPP

#include <cstdint>
#include "node.hpp"
#include "graph.hpp"

namespace Graphs
{
	struct ResultCell;
	class Results;

	Results Dijkstra(const Graph& graph, const Node& start);
}

#endif