#ifndef HWDG_GRAPH_HPP
#define HWDG_GRAPH_HPP

/**
* @file graph.hpp
* @author Jakub Grzana
* @date March 2022
* @brief Computer representation of Graph.
*/


#include <cstdint>
#include <string>
#include <initializer_list>
#include <iostream>
		
#include "custom_map.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "node_in_graph.hpp"

namespace HWDG
{
	/**
	* Representation of Graph.
	* 
	* Contains tools to build, search, and extract basic information from graph. 
	* More complex operations are implemented separately, for example in operations.hpp
	*/
	class Graph
	{
		private:
			Map::unordered_map<uint32_t, NodeInGraph> _nodes;
			Map::unordered_map<uint64_t, Edge> _edges;
			double _weight_sum;
			unsigned int _negative_edges;
			unsigned int _loops;
		public:
			/**
			* Get sum of weights of all edges within a Graph.
			* \return Sum of weights of all edges within a Graph.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			double weight_sum(void) const; 
			/**
			* Get number of nodes within a Graph,
			* \return Number of nodes within a Graph,
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			size_t size_edges(void) const; 
			/**
			* Get number of edges within a Graph,
			* \return Number of edges within a Graph,
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			size_t size_nodes(void) const; 
			/**
			* Support for C++ range loops.
			*/
			Map::const_iterator<uint32_t, NodeInGraph> begin() const;
			/**
			* Support for C++ range loops.
			*/
			Map::const_iterator<uint32_t, NodeInGraph> end() const;
			/**
			* Check whether Graph contains specific Edge. Uses Edge::operator==() to compare.
			* \param edge Edge that you want to check for.
			* \return true if Graph contains edge, false otherwise.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			bool has(const Edge& edge) const; 
			/**
			* Check whether Graph contains specific Node. Uses Node::operator==() to compare.
			* \param node Node that you want to check for.
			* \return true if Graph contains node, false otherwise.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			bool has(const Node& node) const;
			/**
			* Get std::unordered_map-like iterator to NodeInGraph. 
			* It returns iterator to past-last element in case if there's no such node in graph.
			* 
			* In short: don't use it. Use Graph::has() combined with Graph::fetch() instead.
			* 
			* \param node Node which you want to search for.
			* \return iterator to NodeInGraph, or iterator to past-last element in case if there's no such node in graph.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			Map::const_iterator<uint32_t, NodeInGraph> get_node(const Node& node) const; 
			/**
			* Get std::unordered_map-like iterator to Edge.
			* It returns iterator to past-last element in case if there's no such edge in graph.
			*
			* In short: don't use it. Use Graph::has() combined with Graph::fetch() instead.
			*
			* \param edge Edge which you want to search for.
			* \return iterator to Edge, or iterator to past-last element in case if there's no such edge in graph.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			Map::const_iterator<uint64_t, Edge> get_edge(const Edge& edge) const;
			/**
			* Get reference to NodeInGraph within Graph.
			* \param node Node which you want to search for.
			* \return Reference to NodeInGraph within graph.
			* \throws std::out_of_range if there's no such node in graph.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			const NodeInGraph& fetch(const Node& node) const; 
			/**
			* Get reference to Edge within Graph.
			* \param edge Edge which you want to search for.
			* \return Reference to Edge within graph.
			* \throws std::out_of_range if there's no such edge in graph.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			const Edge& fetch(const Edge& edge) const; 
			/**
			* Get string representation - list of successor.
			* Useful during development and debugging.
			* \par Time complexity:
			* \f$O(edges)\f$
			*/
			std::string str(void) const;
			/**
			* Get string representation - list of edges.
			* Useful during development and debugging.
			* \par Time complexity:
			* \f$O(edges)\f$
			*/
			std::string str_edges(void) const;
			
			/**
			* Add Node to Graph.
			* \param node Node you wish to add.
			* \return true if succeeded, false otherwise (most likely reason: this node is already part of Graph)
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			bool add(const Node& node); 
			/**
			* Add Edge to Graph.
			* If either Edge::source() or Edge::target() doesn't belong to Graph, it is automatically added.
			* \param edge Edge you wish to add.
			* \return true if succeeded, false otherwise (most likely reason: this edge is already part of Graph)
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			bool add(const Edge& edge);
			/**
			* Remove edge from Graph.
			* \param edge Edge you wish to remove.
			* \return true if succeeded, false otherwise (most likely reason: there's no such edge within Graph)
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			bool remove(const Edge& edge); 
			/**
			* Update (weight of) edge.
			* Replaces existing edge in graph, or adds it if it doesn't exist yet. Useful for changing weight of edge.
			* \param edge Edge to be updated.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			void update(const Edge& edge); 
			
			/**
			* Get access to "list" of edges.
			* With this, you can get access to raw data about all edges in a graph. Useful for iteration.
			* 
			* \return Internal unordered map of all edges in a graph.
			* \par Time complexity: 
			* \f$O(1)\f$
			*/
			const Map::unordered_map<uint64_t, Edge>& edges(void) const;
			/**
			* Get access to "list" of nodes.
			* With this, you can get access to raw data about all nodes in a graph. Useful for iteration.
			*
			* \return Internal unordered map of all nodes in a graph.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			const Map::unordered_map<uint32_t, NodeInGraph>& nodes(void) const;
			
			/**
			* Get density of Graph.
			* Density is a value from 0 to 1, representing how many percent of total edges possible (which is \f$nodes^2\f$) exists in graph.
			* \return Density (value from 0 to 1)
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			double density(void) const;
			/**
			* Default constructor, creates Graph without any Node or Edge.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			Graph();
			/**
			* Create graph using initializer list of edges.
			* Nodes used by edges are automatically added to graph.
			* \param l Initializer list of edges, might be curly bracket {}
			* \par Time complexity:
			* \f$O(n)\f$
			*/
			Graph(const std::initializer_list<Edge>& l);
			/**
			* Create graph using initializer list of nodes.
			* \param l Initializer list of nodes, might be curly bracket {}
			* \par Time complexity:
			* \f$O(n)\f$
			*/
			Graph(const std::initializer_list<Node>& l);
			/**
			* Create graph using initializer list of nodes and edges.
			* Nodes used by edges are automatically added to graph.
			* \param nodes Initializer list of nodes, might be curly bracket {}
			* \param edges Initializer list of edges, might be curly bracket {}
			* \par Time complexity:
			* \f$O(n)\f$
			*/
			Graph(const std::initializer_list<Node>& nodes, const std::initializer_list<Edge>& edges);
			/**
			* Add edges in bulk.
			* If either Edge::source() or Edge::target() doesn't belong to Graph, it is automatically added.
			* \param l Initializer list of edges, might be curly bracket {}
			* \par Time complexity:
			* \f$O(n)\f$
			*/
			void add(const std::initializer_list<Edge>& l);
			/**
			* Add nodes in bulk.
			* \param l Initializer list of nodes, might be curly bracket {}
			* \par Time complexity:
			* \f$O(n)\f$
			*/
			void add(const std::initializer_list<Node>& l);
			/**
			* Check whether graph contains any Edge with negative weight.
			* It's used to determine whether Graph is eligible for Dijkstra algorithm.
			* \return true if there're edges with negative weight within graph, false otherwise
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			bool has_negative_weights(void) const; 
			/**
			* Check whether graph contains any loops.
			* Loop is an Edge with Edge::source() equal to Edge::target(), edge pointing from Node into the same Node.
			* \return true if there're loops within graph, false otherwise
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			bool has_loops(void) const; 
			
			/**
			* Transposition of Graph.
			* Creates new graph with the same nodes, and all edges reversed (Edge::Reverse())
			* \return Graph with the same nodes, and all edges reversed.
			* \par Time complexity:
			* \f$O(edges)\f$
			*/
			Graph Transpose(void) const; 
			/**
			* Scale weight of all edges within a Graph.
			* Creates new graph with the same nodes, and all edges scaled (Edge::Scale())
			* \param factor Factor by which weights will be scaled.
			* \return New graph with the same nodes, and all edges scaled.
			* \par Time complexity:
			* \f$O(edges)\f$
			*/
			Graph ScaleWeight(float factor) const; 

			/**
			* Reserve memory for given number of nodes.
			* Useful when you know EXACT amount of nodes, or upper limit of them. It guarantees average time complexity of adding new nodes if used properly.
			* 
			* Use only if you know what you're doing. Incorrect usage can have negative impact on performance or memory usage.
			* \param count Number of nodes that you want to store in this Graph.
			* \par Time complexity:
			* \f$O(nodes)\f$
			*/
			void reserve_nodes(const size_t& count);
			/**
			* Reserve memory for given number of edges.
			* Useful when you know EXACT amount of edges, or upper limit of them. It guarantees average time complexity of adding new edges if used properly.
			*
			* Use only if you know what you're doing. Incorrect usage can have negative impact on performance or memory usage.
			* \param count Number of edges that you want to store in this Graph.
			* \par Time complexity:
			* \f$O(edges)\f$
			*/
			void reserve_edges(const size_t& count);
			/**
			* Reserve memory for successors (neighbours) of given Node.
			* Useful when you know EXACT amount of successors, or upper limit of them. It guarantees average time complexity of adding new neighbours if used properly.
			*
			* Use only if you know what you're doing. Incorrect usage can have negative impact on performance or memory usage.
			* \param node Node for which you will reserve the memory.
			* \param count Number of neighbours that you want to store.
			* \par Time complexity:
			* \f$O(edges)\f$
			*/
			void reserve_edges_in_node(const Node& node, const size_t& count);

			/**
			* Save to stream - binary form.
			* \param file Reference to ostream-like object, opened in binary mode.
			* \param graph Graph to be saved.
			* \par Time complexity:
			* \f$O(edges)\f$
			*/
			static void SaveBin(std::ostream& file, const Graph& graph);
			/**
			* Load from stream - binary form.
			* \param file Reference to istream-like object, opened in binary mode.
			* \return Graph loaded from stream.
			* \par Time complexity:
			* \f$O(edges)\f$
			*/
			static Graph LoadBin(std::istream& file);
			/**
			* Save to stream - text form.
			* \param file Reference to ostream-like object.
			* \param graph Graph to be saved.
			* \par Time complexity:
			* \f$O(edges)\f$
			*/
			static void SaveTxt(std::ostream& file, const Graph& graph);
			/**
			* Load from stream - text form.
			* \param file Reference to istream-like object.
			* \return Graph loaded from stream.
			* \par Time complexity:
			* \f$O(edges)\f$
			*/
			static Graph LoadTxt(std::istream& file);
	};
}

#endif