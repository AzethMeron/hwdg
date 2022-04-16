
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include "dijkstra.hpp"
#include "custom_map.hpp"
#include "tools.hpp"
#include "path.hpp"

namespace Graphs
{
	Dijkstra::Cell::Cell(const Node& n, const Node& src) : PathtableCell(n,src)
	{
		this->heap_position = -1;
	}
	Dijkstra::Dijkstra(const Graph& graph, const Node& src) : _results(src)
	{
		if (!graph.has(src)) throw std::invalid_argument(string_format("Node %s doesn't belong to given graph", src.str().c_str()));
		Algorithm(graph);
	}

	void Dijkstra::Algorithm(const Graph& graph)
	{
		// Condition check
		if (graph.has_negative_weights()) throw std::invalid_argument("Dijkstra algorithm cannot be used for graphs with negative weights of edges.");
		// Initialisation
		this->_results.Initialise(graph, this->_results.source);
		this->_heap.reserve(graph.size_nodes());
		for (const Node& node : graph) { this->_heap.push_back(node); }
		// Convert vector to heap. From this point, both results and heap are synchronized
		this->MakeHeap();
		// Dijkstra Algorithm
		while (this->_heap.size())
		{
			const NodeInGraph& current = graph.fetch(this->PopHeap());
			const Cell& current_cell = this->getCell(current);

			for (const Edge& edge : current)
			{
				const Node& neighbour = edge.target();
				const Cell& neighbour_cell = this->getCell(neighbour);
				double pathweight_from_current = current_cell.pathweight + edge.weight();
				if (neighbour_cell.pathweight > pathweight_from_current)
				{
					this->UpdateWeight(neighbour, current, pathweight_from_current);
				}
			}
		}
	}

	void Dijkstra::MakeHeap()
	{
		for (int64_t i = HeapParent(this->_heap.size()-1); i >= 0; --i)
		{
			this->Heapify(i);
		}
		for (size_t i = 0; i < this->_heap.size(); ++i)
		{
			Cell& c = this->getCell(i);
			c.heap_position = i;
		}
	}

	void Dijkstra::PushHeap(const Node& node)
	{
		this->_heap.push_back(node);
		this->RestoreHeap(this->_heap.size()-1);
	}

	Node Dijkstra::PopHeap()
	{
		Node output = this->_heap.at(0);
		this->HeapSwap(0, this->_heap.size() - 1);
		this->_heap.pop_back();
		this->Heapify(0);
		(*this->_results.find(output.id())).heap_position = -1;
		return output;
	}

	inline size_t Dijkstra::HeapParent(const size_t& index) const
	{
		if (index == 0) return 0;
		return (index - 1) / 2;
	}

	inline size_t Dijkstra::HeapLeftChild(const size_t& index) const
	{
		return index * 2 + 1;
	}
	inline size_t Dijkstra::HeapRightChild(const size_t& index) const
	{
		return index * 2 + 2;
	}

	// true if n1 wieksze niz n2 
	bool Dijkstra::HeapCompare(const size_t& index1, const size_t& index2) const
	{
		const Cell& c1 = this->getCell(index1);
		const Cell& c2 = this->getCell(index2);
		return c1.pathweight > c2.pathweight;
	}

	inline bool Dijkstra::HeapExist(const size_t& index) const
	{
		return index >= 0 && index < this->_heap.size();
	}

	void Dijkstra::RestoreHeap(const size_t& position)
	{
		this->Heapify(position);
		size_t parent = this->HeapParent(position);
		if (parent != position)
		{
			RestoreHeap(parent);
		}
	}

	void Dijkstra::Heapify(const size_t& position)
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
		else if(r_child_smaller_than_parent)
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

	void Dijkstra::HeapSwap(const size_t& l, const size_t& r)
	{
		std::swap(this->_heap.at(l), this->_heap.at(r));
		Cell& c1 = this->getCell(l);
		Cell& c2 = this->getCell(r);
		c1.heap_position = l;
		c2.heap_position = r;
	}

	Dijkstra::Cell& Dijkstra::getCell(const size_t& pos_in_heap)
	{
		const Node& n = this->_heap.at(pos_in_heap);
		return this->_results.getCell(n);
	}

	const Dijkstra::Cell& Dijkstra::getCell(const size_t& pos_in_heap) const
	{
		const Node& n = this->_heap.at(pos_in_heap);
		return this->_results.getCell(n);
	}

	const Dijkstra::Cell& Dijkstra::getCell(const Node& node) const
	{
		return this->_results.getCell(node);
	}

	void Dijkstra::UpdateWeight(const Node& node, const Node& prev_node, const double& pathweight)
	{
		this->_results.UpdateWeight(node, prev_node, pathweight);
		Cell& c = this->_results.getCell(node);
		if(c.heap_position >= 0) this->RestoreHeap(c.heap_position);
	}

	Path Dijkstra::GetPath(const Node& target) const
	{
		return this->_results.GetPath(target);
	}

	bool Dijkstra::has(const Node& node) const
	{
		return this->_results.has(node);
	}

	const Pathtable<Dijkstra::Cell>& Dijkstra::RawResults(void) const
	{
		return this->_results;
	}
}
