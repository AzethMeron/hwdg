#ifndef GRAPHS_BELLMANFORD_HPP
#define GRAPHS_BELLMANFORD_HPP

#include <cstdint>
#include <vector>
#include "node.hpp"
#include "graph.hpp"
#include "custom_map.hpp"
#include "path.hpp"

namespace Graphs
{
	class BellmanFord
	{
		private:
			const Node _source;
			unordered_map<uint32_t, PathtableCell> _results;
		private:
			void Algorithm(const Graph& graph);
		public:
			BellmanFord() = delete;
			BellmanFord(const Graph& graph, const Node& src);
			Path GetPath(const Node& target) const;
			const unordered_map<uint32_t, PathtableCell>& RawResults(void) const;
	};
}

#endif
