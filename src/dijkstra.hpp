#ifndef GRAPHS_ALGORITHM_HPP
#define GRAPHS_ALGORITHM_HPP

#include <cstdint>
#include <vector>
#include <unordered_set>
#include "node.hpp"
#include "graph.hpp"
#include "custom_map.hpp"

namespace Graphs
{
	class Dijkstra
	{
		private:
			const Node _source;
			struct Cell
			{
				Node node;
				double pathweight;
				int64_t prev_id;
				size_t heap_position;
				Cell(const Node& n);
			};
			unordered_map<uint32_t, Cell> results;
			std::vector<Node> heap; // effectively, Q set
			//std::unordered_set<Node, Node::HashFunction> Q;
			//std::unordered_set<Node, Node::HashFunction> S;
		private:
			void PushHeap(Node);
			Node PopHeap();
			void RestoreHeap(size_t position);
			void MakeHeap();
			bool HeapCompare(size_t index1, size_t index2) const;
			size_t leftChild(size_t index) const;
			size_t rightChild(size_t index) const;
			bool has(size_t index) const;
			size_t parent(size_t index) const;
		private:
			void Algorithm(const Graph& graph);
		public:
			Dijkstra() = delete;
			Dijkstra(const Graph& graph, const Node& src);
	};
}

#endif