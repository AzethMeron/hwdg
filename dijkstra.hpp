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
#include <vector>
#include <algorithm>

	template<typename TYPE>
	class Heap {
		private:
			std::vector<TYPE> _heap;
		private:
			inline size_t HeapLeftChild(const size_t& index) const
			{
				return index * 2 + 1;
			}
			inline size_t HeapRightChild(const size_t& index) const
			{
				return index * 2 + 2;
			}
			inline bool HeapExist(const size_t& index) const
			{
				return index >= 0 && index < this->HeapSize();
			}
			inline size_t HeapParent(const size_t& index) const
			{
				if (index == 0) return 0;
				return (index - 1) / 2;
			}
			void Heapify(const size_t& position) // Check & swap with children of given position. Doesn't check parent
			{
				size_t r_child_ind = this->HeapRightChild(position);
				size_t l_child_ind = this->HeapLeftChild(position);
				bool r_child_smaller_than_parent = false;
				bool l_child_smaller_than_parent = false;
				if (this->HeapExist(r_child_ind)) r_child_smaller_than_parent = this->HeapCompare(position, r_child_ind);
				if (this->HeapExist(l_child_ind)) l_child_smaller_than_parent = this->HeapCompare(position, l_child_ind);
				if (r_child_smaller_than_parent && l_child_smaller_than_parent)
				{
					if (this->HeapCompare(r_child_ind, l_child_ind))
					{
						this->HeapSwap(position, l_child_ind);
						this->Heapify(l_child_ind);
					}
					else
					{
						this->HeapSwap(position, r_child_ind);
						this->Heapify(r_child_ind);
					}
				}
				else if (r_child_smaller_than_parent)
				{
					this->HeapSwap(position, r_child_ind);
					this->Heapify(r_child_ind);
				}
				else if (l_child_smaller_than_parent)
				{
					this->HeapSwap(position, l_child_ind);
					this->Heapify(l_child_ind);
				}
			}
		protected:
			virtual inline void HeapSwap(const size_t& l, const size_t& r) = 0;
			virtual bool HeapCompare(const size_t& l, const size_t& r) const = 0;
		public:
			size_t HeapSize() const { return this->_heap.size(); }
			TYPE& HeapAt(const size_t& ind) { return this->_heap.at(ind); } // you're supposed to use RestoreHeap() after modifying 
			const TYPE& HeapAt(const size_t& ind) const { return this->_heap.at(ind); }
			void HeapPush(const TYPE& obj)
			{
				this->_heap.push_back(obj);
				this->RestoreHeap(this->HeapSize() - 1);
			}
			TYPE HeapPop()
			{
				TYPE output = this->HeapAt(0);
				this->HeapSwap(0, this->HeapSize() - 1);
				this->_heap.pop_back();
				this->Heapify(0);
				return output;
			}
			Heap() {}
			Heap(const std::vector<TYPE>& vec)
			{
				this->_heap = vec;
				for (int64_t i = this->HeapParent(this->HeapSize() - 1); i >= 0; --i)
				{
					this->Heapify(i);
				}
			}
			void RestoreHeap(const size_t& position) // Heapify node, then call RestoreHeap for parent of it
			{
				this->Heapify(position);
				size_t parent = this->HeapParent(position);
				if (parent != position)
				{
					this->RestoreHeap(parent);
				}
			}
			auto begin() const { return this->_heap.begin(); }
			auto end() const { return this->_heap.end(); }
			void reserve(const size_t& size)
			{
				this->_heap.reserve(size);
			}
	};


	/**
	* Static class implementing Dijkstra algorithm for pathfinding.
	* Because it's static class, you can't create objects of it. You are supposed only to call Dijkstra::Compute() static function.
	* 
	* There's implementation of binary heap inside Dijkstra class, code might be improved with creation of Heap class that Dijkstra would inherit and override several methods inside.
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
