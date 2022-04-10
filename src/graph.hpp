#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>
#include <initializer_list>
		

#include "node.hpp"
#include "edge.hpp"
#include "node_in_graph.hpp"

namespace Graphs
{
	class Graph
	{
		private:
			std::unordered_map<uint32_t, NodeInGraph> _nodes;
			std::unordered_map<uint64_t, Edge> _edges;
			double _weight_sum;
		public:
			double weight_sum(void) const; // constant
			unsigned int size_edges(void) const; // constant 
			unsigned int size_nodes(void) const; // constant
			typename std::unordered_map<uint32_t, NodeInGraph>::const_iterator begin() const;
			typename std::unordered_map<uint32_t, NodeInGraph>::const_iterator end() const;
			bool has(const Edge& edge) const; // constant
			bool has(const Node& node) const; // constant
			typename std::unordered_map<uint32_t, NodeInGraph>::const_iterator get_node(const Node& node) const; 
			typename std::unordered_map<uint64_t, Edge>::const_iterator get_edge(const Edge& edge) const;
			const NodeInGraph& fetch(const Node& node) const; // constant
			const Edge& fetch(const Edge& edge) const; // constant
			std::string str(void) const;
			std::string str_edges(void) const;
			
			bool add(const Node& node); // constant
			bool add(const Edge& edge); // constant
			bool remove(const Edge& edge); // constant
			void update(const Edge& edge); // constant
			
			const std::unordered_map<uint64_t, Edge>& edges(void) const;
			const std::unordered_map<uint32_t, NodeInGraph>& nodes(void) const;
			
			float density(void) const;
			Graph();
			Graph(const std::initializer_list<Edge>& l);
			Graph(const std::initializer_list<Node>& l);
			Graph(const std::initializer_list<Node>& nodes, const std::initializer_list<Edge>& edges);
			void add(const std::initializer_list<Edge>& l);
			void add(const std::initializer_list<Node>& l);
	};
}
