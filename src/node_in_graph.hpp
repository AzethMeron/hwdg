#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>
#include "node.hpp"
#include "edge.hpp"

class NodeInGraph : public Node
{
	private:
		std::unordered_map<uint64_t, Edge> _edges;
	public:
		bool has_edge(const Edge& edge) const; // true - zawiera
		bool add_edge(const Edge& edge); // false - nie udalo sie dodac (juz istnieje taki edge)
		bool add_edge(const Node& tgt_node, float weight);
		bool remove_edge(const Edge& edge);
		
		typename std::unordered_map<uint64_t, Edge>::const_iterator begin() const;
		typename std::unordered_map<uint64_t, Edge>::const_iterator end() const;
		unsigned int size_edges(void) const;
		std::string str(void) const;
		
		NodeInGraph() = delete;
		NodeInGraph(const Node& n);
};

