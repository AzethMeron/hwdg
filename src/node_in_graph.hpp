#ifndef GRAPHS_NODE_IN_GRAPH_HPP
#define GRAPHS_NODE_IN_GRAPH_HPP

#include <cstdint>
#include <string>
#include "custom_map.hpp"
#include "node.hpp"
#include "edge.hpp"

namespace Graphs
{
	class NodeInGraph : public Node
	{
		private:
			unordered_map<uint64_t, Edge> _edges;
		public:
			bool has(const Edge& edge) const; // true - zawiera, constant
			bool add(const Edge& edge); // false - nie udalo sie dodac (juz istnieje taki edge), constant
			bool add(const Node& tgt_node, float weight); // constant
			bool remove(const Edge& edge); // constant
			
			const_iterator<uint64_t, Edge> begin() const;
			const_iterator<uint64_t, Edge> end() const;
			unsigned int size_edges(void) const; // constant
			std::string str(void) const; // linear
			
			NodeInGraph() = delete;
			NodeInGraph(const Node& n);
	};

}

#endif