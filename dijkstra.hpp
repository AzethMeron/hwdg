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
#include "heap.hpp"

namespace HWDG
{
#include <vector>
#include <algorithm>

	/**
	* Static class implementing Dijkstra algorithm for pathfinding.
	* Because it's static class, you can't create objects of it. You are supposed only to call Dijkstra::Compute() static function.
	* 
	* Uses binary heap, implemented in HWDG::Heap, to speed up algorithm execution.
	*/
	class Dijkstra : public Heap<Node> 
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
		protected:
			inline void HeapSwap(const size_t& l, const size_t& r) override;
			bool HeapCompare(const size_t& l, const size_t& r) const override;
			Node HeapPop();
		private: 
			Cell& getCell(const size_t& pos_in_heap);
			const Cell& getCell(const size_t& pos_in_heap) const;
		private:
			bool has(const Node& node) const;
			const Cell& getCell(const Node& node) const;
			void UpdateWeight(const Node& node, const Node& prev_node, const double& pathweight);
			void Algorithm(const Graph& graph);
		private:
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
			* \return Pathtable with paths from source Node to every other Node within Graph.
			* \throws std::invalid_argument if Graph has negative weights.
			*
			* \par Time complexity:
			* \f$O(nodes * \log_{2}nodes)\f$
			*/
			static Pathtable<Dijkstra::Cell> Compute(const Graph& graph, const Node& src);
			Dijkstra() = delete;
	};
}

#endif
