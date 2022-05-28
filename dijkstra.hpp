#ifndef HWDG_DIJKSTRA_HPP
#define HWDG_DIJKSTRA_HPP

#include <cstdint>
#include <vector>
#include "node.hpp"
#include "graph.hpp"
#include "custom_map.hpp"
#include "path.hpp"
#include "bellmanford.hpp"

namespace HWDG
{
	class Dijkstra // could be improved by making Heap class, then making Dijkstra inherit on it and overloading certain functions.
		// it would improve readability, but I'm not going to do it right now since it wouldn't change efficiency and this implementation DOES work
	{
		public:
			struct Cell : public PathtableCell
			{
				size_t heap_position;
				Cell(const Node& n, const Node& src);
				Cell(const PathtableCell& cell);
			};
		private:
			Pathtable<Cell> _results;
			std::vector<Node> _heap; // effectively, Q set
		private: // Heap functions. It's a mess.
			void PushHeap(const Node& node);
			Node PopHeap();
			void RestoreHeap(const size_t& position); // Heapify node, then call RestoreHeap for parent of it
			void Heapify(const size_t& position); // Check & swap with children of given position. Doesn't check parent
			void MakeHeap();
			bool HeapCompare(const size_t& index1, const size_t& index2) const;
			inline size_t HeapLeftChild(const size_t& index) const;
			inline size_t HeapRightChild(const size_t& index) const;
			inline bool HeapExist(const size_t& index) const;
			inline size_t HeapParent(const size_t& index) const;
			inline void HeapSwap(const size_t& l, const size_t& r);
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
			Path GetPath(const Node& target) const;
			const Pathtable<Dijkstra::Cell>& Results(void) const;
	};

	typedef Pathtable<Dijkstra::Cell> DijkstraTable;
	DijkstraTable DijkstraResults(const Graph& graph, const Node& source);
}

#endif
