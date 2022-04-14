#ifndef GRAPHS_ALGORITHM_HPP
#define GRAPHS_ALGORITHM_HPP

#include <cstdint>
#include <vector>
#include <unordered_set>
#include "node.hpp"
#include "graph.hpp"
#include "custom_map.hpp"
#include "path.hpp"

namespace Graphs
{
	class Dijkstra
	{
		public:
			struct Cell
			{
				const Node node;
				double pathweight;
				int64_t prev_id;
				size_t heap_position;
				Cell(const Node& n, const Node& src);
			};
		private:
			const Node _source;
			unordered_map<uint32_t, Cell> _results;
			std::vector<Node> _heap; // effectively, Q set
			//std::unordered_set<Node, Node::HashFunction> Q;
			//std::unordered_set<Node, Node::HashFunction> S;
		private: // Heap functions. It's a mess.
			void PushHeap(const Node& node);
			Node PopHeap();
			void RestoreHeap(const size_t& position); // similar to heapify, but it checks parent too
			void Heapify(const size_t& position); 
			void MakeHeap();
			bool HeapCompare(const size_t& index1, const size_t& index2) const;
			inline size_t leftChild(const size_t& index) const;
			inline size_t rightChild(const size_t& index) const;
			inline bool exist(const size_t& index) const;
			inline size_t parent(const size_t& index) const;
			inline void swap(const size_t& l, const size_t& r);
			Cell& getCell(const size_t& pos_in_heap);
			const Cell& getCell(const size_t& pos_in_heap) const;
		private:
			bool has(const Node& node) const;
			const Cell& getCell(const Node& node) const;
			void UpdateWeight(const Node& node, const Node& prev_node, const double& pathweight);
			void Algorithm(const Graph& graph);
		public:
			Dijkstra() = delete;
			Dijkstra(const Graph& graph, const Node& src);
			Path Path(const Node& target) const;
			const unordered_map<uint32_t, Cell>& RawResults(void) const;
	};
}

#endif