#ifndef GRAPHS_BELLMANFORD_HPP
#define GRAPHS_BELLMANFORD_HPP

#include <cstdint>
#include <vector>
#include "node.hpp"
#include "graph.hpp"
#include "custom_map.hpp"
#include "path.hpp"

namespace HWDG
{
	class BellmanFord
	{
		private:
			Pathtable<PathtableCell> _results;
		private:
			bool has(const Node& node) const;
			const PathtableCell& getCell(const Node& node) const;
			void UpdateWeight(const Node& node, const Node& prev_node, const double& pathweight);
			void Algorithm(const Graph& graph);
		public:
			BellmanFord() = delete;
			BellmanFord(const Graph& graph, const Node& src);
			Path GetPath(const Node& target) const;
			const Pathtable<PathtableCell>& RawResults(void) const;
	}; 

	typedef Pathtable<PathtableCell> BellmanTable;
}

#endif
