#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>

#include "node.hpp"
#include "edge.hpp"
#include "node_in_graph.hpp"

class Graph
{
	private:
		std::unordered_map<uint32_t, NodeInGraph> _nodes;
		std::unordered_map<uint64_t, Edge> _edges;
		double _weight_sum;
	public:
		double weight_sum(void) const;
		unsigned int size_edges(void) const;
		unsigned int size_nodes(void) const;
		typename std::unordered_map<uint32_t, NodeInGraph>::const_iterator begin() const;
		typename std::unordered_map<uint32_t, NodeInGraph>::const_iterator end() const;
		bool has_edge(const Edge& edge) const;
		bool has_node(const Node& node) const;
		typename std::unordered_map<uint32_t, NodeInGraph>::const_iterator get_node(const Node& node) const;
		typename std::unordered_map<uint64_t, Edge>::const_iterator get_edge(const Edge& edge) const;
		const NodeInGraph& fetch_node(const Node& node) const;
		const Edge& fetch_edge(const Edge& edge) const;
		std::string str(void) const;
		std::string str_edges(void) const;
		
		bool add_node(const Node& node);
		bool add_edge(const Edge& edge);
		bool remove_edge(const Edge& edge);
		
		const std::unordered_map<uint64_t, Edge>& edges(void) const;
		const std::unordered_map<uint32_t, NodeInGraph>& nodes(void) const;
		
		float density(void) const;
		Graph();
};
