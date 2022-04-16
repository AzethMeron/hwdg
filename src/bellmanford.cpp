
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

	const unordered_map<uint32_t, PathtableCell>& BellmanFord::RawResults(void) const
	{
		return this->_results;
	}

	bool BellmanFord::has(const Node& node) const
	{
		auto iter = this->_results.find(node.id());
		if (iter == this->_results.end()) return false;
		return true;
	}
	const PathtableCell& BellmanFord::getCell(const Node& node) const
	{
		return (*this->_results.find(node.id()));
	}

	void BellmanFord::UpdateWeight(const Node& node, const Node& prev_node, const double& pathweight)
	{
		PathtableCell& c = (*this->_results.find(node.id()));
		c.pathweight = pathweight;
		c.prev_id = prev_node.id();
	}

	Path BellmanFord::GetPath(const Node& target) const
	{
		if (!this->has(target)) throw std::invalid_argument(string_format("No node %s in graph this algorithm was used on", target.str()));
		std::vector<Node> nodes;
		const double weight = this->getCell(target).pathweight;
		const bool exists = this->getCell(target).prev_id != -1;
		Node analysing = target;
		nodes.push_back(analysing);
		while (true)
		{
			const PathtableCell& c = this->getCell(analysing);
			if (c.prev_id < 0) break;
			analysing = Node(c.prev_id);
			nodes.push_back(analysing);
		}
		std::reverse(nodes.begin(), nodes.end());
		return Path(nodes, weight, exists);
	}
}
