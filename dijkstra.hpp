#ifndef HWDG_DIJKSTRA_HPP
#define HWDG_DIJKSTRA_HPP

/**
* @file dijkstra.hpp
* @author Jakub Grzana
* @date March 2022
* @brief Implementation of Dijkstra algorithm for pathfinding.
*
* This file contains class Dijkstra, which is written in horrible yet consistent way with BellmanFord.
*/

#include <cstdint>
#include <vector>
#include "node.hpp"
#include "graph.hpp"
#include "custom_map.hpp"
#include "path.hpp"
#include "bellmanford.hpp"

namespace HWDG
{
	/**
	* Static class implementing Dijkstra algorithm for pathfinding.
	* Because it's static class, you can't create objects of it. You are supposed only to call Dijkstra::Compute() static function.
	* 
	* There's implementation of binary heap inside Dijkstra class, code might be improved with creation of Heap class that Dijkstra would inherit and override several methods inside.
	*/
	class Dijkstra // could be improved by making Heap class, then making Dijkstra inherit on it and overloading certain functions.
		// it would improve readability, but I'm not going to do it right now since it wouldn't change efficiency and this implementation DOES work
	{
		public:
			/**
			* Extension of PathtableCell.
			* Dijkstra is written with binary heap, and cells require to keep track of heap position.
			*/
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
		private:
			Dijkstra() = delete;
			Dijkstra(const Graph& graph, const Node& src);
			Path GetPath(const Node& target) const;
			const Pathtable<Dijkstra::Cell>& Results(void) const;
		public:
			/**
			* Execute Dijkstra algorithm.
			* It calculated shortest path from source Node to every other Node in a Graph.
			*
			* Dijkstra algorithm is fast, but it doesn't work if there're any edges with negative weight within Graph.
			*
			* \param graph Graph that you want to find Pathtable for.
			* \param src Source Node, for which the Pathtable will be created.
			* \throws std::invalid_argument if Graph has negative weights.
			*
			* \par Time complexity:
			* \f$O(nodes * \log_{2}nodes)\f$
			*/
			static const Pathtable<Dijkstra::Cell> Compute(const Graph& graph, const Node& src);
	};
}

#endif
