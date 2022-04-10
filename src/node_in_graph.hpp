#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>
#include "node.hpp"
#include "edge.hpp"

namespace Graphs
{
	class NodeInGraph : public Node
	{
		private:
			std::unordered_map<uint64_t, Edge> _edges;
		public:
			bool has_edge(const Edge& edge) const; // true - zawiera, constant
			bool add_edge(const Edge& edge); // false - nie udalo sie dodac (juz istnieje taki edge), constant
			bool add_edge(const Node& tgt_node, float weight); // constant
			bool remove_edge(const Edge& edge); // constant
			
			typename std::unordered_map<uint64_t, Edge>::const_iterator begin() const;
			typename std::unordered_map<uint64_t, Edge>::const_iterator end() const;
			unsigned int size_edges(void) const; // constant
			std::string str(void) const; // linear
			
			NodeInGraph() = delete;
			NodeInGraph(const Node& n);
	};

}
