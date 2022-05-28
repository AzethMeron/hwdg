
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include "bellmanford.hpp"
#include "custom_map.hpp"
#include "tools.hpp"
#include "path.hpp"

namespace HWDG
{
	BellmanFord::BellmanFord(const Graph& graph, const Node& src) : _results(src)
	{
		if (!graph.has(src)) throw std::invalid_argument(string_format("Node %s doesn't belong to given graph", src.str().c_str()));
		Algorithm(graph);
	}

	void BellmanFord::Algorithm(const Graph& graph)
	{
		// Initialisation
		this->_results.Initialise(graph, this->_results.source);
		// Algorithm
		for (int i = 0; i < graph.size_nodes() - 1; ++i)
		{
			bool has_changed = false;
			for (const Edge& edge : graph.edges())
			{
				const PathtableCell& src = this->getCell(edge.source());
				const PathtableCell& tgt = this->getCell(edge.target());
				if (tgt.pathweight > src.pathweight + edge.weight())
				{
					this->UpdateWeight(tgt.node, src.node, src.pathweight + edge.weight());
					has_changed = true;
				}
			}
			if (!has_changed) break;
		}
		// Check for negative cycle
		for (const Edge& edge : graph.edges())
		{
			const PathtableCell& src = this->getCell(edge.source());
			const PathtableCell& tgt = this->getCell(edge.target());
			if (tgt.pathweight > src.pathweight + edge.weight())
			{
				throw std::invalid_argument("Negative cycle detected");
			}
		}
	}

	const Pathtable<PathtableCell>& BellmanFord::Results(void) const
	{
		return this->_results;
	}

	bool BellmanFord::has(const Node& node) const
	{
		return this->_results.has(node);
	}
	const PathtableCell& BellmanFord::getCell(const Node& node) const
	{
		return this->_results.getCell(node);
	}

	void BellmanFord::UpdateWeight(const Node& node, const Node& prev_node, const double& pathweight)
	{
		this->_results.UpdateWeight(node, prev_node, pathweight);
	}

	Path BellmanFord::GetPath(const Node& target) const
	{
		return this->_results.GetPath(target);
	}
}
