#ifndef GRAPHS_GRAPH_HPP
#define GRAPHS_GRAPH_HPP

#include <cstdint>
#include <string>
#include <initializer_list>
		
#include "custom_map.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "node_in_graph.hpp"

namespace Graphs
{
	// 20000 nodes, density 0.1, 9,6 GB
	// 10000 nodes, density 0.2, 5,1 GB
	// 5000 nodes, density 0.5, 3,3 GB
	// 1000 nodes, density 1, 250 MB
	class Graph
	{
		private:
			unordered_map<uint32_t, NodeInGraph> _nodes;
			unordered_map<uint64_t, Edge> _edges;
			double _weight_sum;
			unsigned int _negative_edges;
			unsigned int _loops;
		public:
			double weight_sum(void) const; // constant
			unsigned int size_edges(void) const; // constant 
			unsigned int size_nodes(void) const; // constant
			const_iterator<uint32_t, NodeInGraph> begin() const;
			const_iterator<uint32_t, NodeInGraph> end() const;
			bool has(const Edge& edge) const; // constant
			bool has(const Node& node) const; // constant
			const_iterator<uint32_t, NodeInGraph> get_node(const Node& node) const; 
			const_iterator<uint64_t, Edge> get_edge(const Edge& edge) const;
			const NodeInGraph& fetch(const Node& node) const; // constant
			const Edge& fetch(const Edge& edge) const; // constant
			std::string str(void) const;
			std::string str_edges(void) const;
			
			bool add(const Node& node); // constant
			bool add(const Edge& edge); // constant
			bool remove(const Edge& edge); // constant
			void update(const Edge& edge); // constant
			
			const unordered_map<uint64_t, Edge>& edges(void) const;
			const unordered_map<uint32_t, NodeInGraph>& nodes(void) const;
			
			double density(void) const;
			Graph();
			Graph(const std::initializer_list<Edge>& l);
			Graph(const std::initializer_list<Node>& l);
			Graph(const std::initializer_list<Node>& nodes, const std::initializer_list<Edge>& edges);
			void add(const std::initializer_list<Edge>& l);
			void add(const std::initializer_list<Node>& l);

			bool has_negative_weights(void) const; // constant
			bool has_loops(void) const; // constant
			
			Graph Transpose(void) const; // linear
			Graph ScaleWeight(float factor) const; // linear
	};
}

#endif