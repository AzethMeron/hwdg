
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include "bellmanford.hpp"
#include "custom_map.hpp"
#include "tools.hpp"
#include "path.hpp"

namespace Graphs
{
	BellmanFord::BellmanFord(const Graph& graph, const Node& src) : _source(src)
	{
		if (!graph.has(src)) throw std::invalid_argument(string_format("Node %s doesn't belong to given graph", src.str().c_str()));
		Algorithm(graph);
	}

	void BellmanFord::Algorithm(const Graph& graph)
	{
		// Initialisation
		this->_results.reserve(graph.size_nodes());
		for (const Node& node : graph)
		{
			this->_results.insert({ node.id(), PathtableCell(node, this->_source) });
		}
	}

	const unordered_map<uint32_t, PathtableCell>& BellmanFord::RawResults(void) const
	{
		return this->_results;
	}
}
