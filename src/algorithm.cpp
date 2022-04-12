
#include <iostream>
#include <limits>
#include <queue>
#include <vector>
#include "algorithm.hpp"
#include "custom_map.hpp"

namespace Graphs
{
	struct ResultCell 
	{
		public:
			Node node;
			double pathweight;
			int64_t prev_id; //this is ID of previous node, except it can be -1 (non-existent)
			ResultCell() = delete;
			ResultCell(const Node& n);
			ResultCell(const Node& n, double weight, int64_t prev);
	};

	ResultCell::ResultCell(const Node& n) : node(n)
	{
		this->pathweight = std::numeric_limits<double>::max();
		this->prev_id = -1;
	}

	ResultCell::ResultCell(const Node& n, double weight, int64_t prev) : node(n)
	{
		this->pathweight = weight;
		this->prev_id = prev;
	}

	struct ResultCellCompare
	{
		bool operator() (const ResultCell& l, const ResultCell& r)
		{
			return l.pathweight > r.pathweight;
		}
	};

	class Results
	{
		private:
			unordered_map<uint32_t, ResultCell> _results;
		public:

	};

	Results Dijkstra(const Graph& graph, const Node& start)
	{
		if (graph.has_negative_weights()) { throw std::invalid_argument("Dijkstra algorithm doesn't work with negative weights in graphs"); }
		unordered_map<uint32_t, ResultCell> results;
		std::priority_queue<ResultCell&, std::vector<ResultCell&>, ResultCellCompare> Q;
		for (const Node& node : graph)
		{
			if (node == start) results.insert({ node.id(), ResultCell(node, 0, node.id()) });
			else results.insert({node.id(), ResultCell(node)});
			Q.push(*results.find(node.id()));
		}
		while (!Q.empty())
		{
			std::cout << Q.top().pathweight << std::endl;
			Q.pop();
		}
	}
}