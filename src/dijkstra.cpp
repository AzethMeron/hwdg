
#include <cstdint>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>
#include "dijkstra.hpp"
#include "custom_map.hpp"
#include "tools.hpp"

namespace Graphs
{
	Dijkstra::Cell::Cell(const Node& n, const Node& src) : node(n)
	{
		this->prev_id = -1;
		this->pathweight = std::numeric_limits<double>::max();
		if (n == src) this->pathweight = 0;
		this->heap_position = -1;
	}
	Dijkstra::Dijkstra(const Graph& graph, const Node& src) : _source(src)
	{
		Algorithm(graph);
	}

	void Dijkstra::Algorithm(const Graph& graph)
	{
		// Condition check
		if (graph.has_negative_weights()) throw std::invalid_argument("Dijkstra algorithm cannot be used for graphs with negative weights of edges.");
		// Initialisation
		for (const Node& node : graph)
		{
			this->results.insert({node.id(), Dijkstra::Cell(node, this->_source)});
			this->heap.push_back(node);
		}
		// Convert vector to heap. From this point, both results and heap are synchronized
		this->MakeHeap();
		// Dijkstra Algorithm
		while (this->heap.size())
		{
			NodeInGraph current = graph.fetch(this->PopHeap());
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
		/* this was used to test heap
		for (auto& node : this->heap)
		{
			const Cell& c1 = (*this->results.find(node.id()));
			std::cout << c1.pathweight << " ";
		}
		std::cout << std::endl;
		while (this->heap.size())
		{
			const Cell& c1 = (*this->results.find(this->PopHeap().id()));
			std::cout << c1.pathweight << " ";
		}*/
	}

	void Dijkstra::MakeHeap()
	{
		for (int64_t i = parent(this->heap.size()-1); i >= 0; --i)
		{
			this->Heapify(i);
		}
		for (size_t i = 0; i < this->heap.size(); ++i)
		{
			Cell& c = this->getCell(i);
			c.heap_position = i;
		}
	}

	void Dijkstra::PushHeap(const Node& node)
	{
		this->heap.push_back(node);
		this->RestoreHeap(this->heap.size()-1);
	}

	Node Dijkstra::PopHeap()
	{
		Node output = this->heap.at(0);
		this->swap(0, this->heap.size() - 1);
		this->heap.pop_back();
		this->Heapify(0);
		return output;
	}

	inline size_t Dijkstra::parent(const size_t& index) const
	{
		if (index == 0) return 0;
		return (index - 1) / 2;
	}

	inline size_t Dijkstra::leftChild(const size_t& index) const
	{
		return index * 2 + 1;
	}
	inline size_t Dijkstra::rightChild(const size_t& index) const
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

	inline bool Dijkstra::exist(const size_t& index) const
	{
		return index >= 0 && index < this->heap.size();
	}

	void Dijkstra::RestoreHeap(const size_t& position)
	{
		this->Heapify(position);
		size_t parent = this->parent(position);
		if (parent != position)
		{
			RestoreHeap(parent);
		}
	}

	void Dijkstra::Heapify(const size_t& position)
	{
		size_t r_child_ind = this->rightChild(position);
		size_t l_child_ind = this->leftChild(position);
		bool r_child_smaller_than_parent = false;
		bool l_child_smaller_than_parent = false;
		if (this->exist(r_child_ind)) r_child_smaller_than_parent = this->HeapCompare(position, r_child_ind);
		if (this->exist(l_child_ind)) l_child_smaller_than_parent = this->HeapCompare(position, l_child_ind);
		if (r_child_smaller_than_parent && l_child_smaller_than_parent)
		{
			if (this->HeapCompare(r_child_ind, l_child_ind))
			{
				this->swap(position, l_child_ind);
				this->Heapify(l_child_ind);
			}
			else
			{
				this->swap(position, r_child_ind);
				this->Heapify(r_child_ind);
			}
		}
		else if(r_child_smaller_than_parent)
		{
			this->swap(position, r_child_ind);
			this->Heapify(r_child_ind);
		}
		else if (l_child_smaller_than_parent)
		{
			this->swap(position, l_child_ind);
			this->Heapify(l_child_ind);
		}
	}

	void Dijkstra::swap(const size_t& l, const size_t& r)
	{
		std::swap(this->heap.at(l), this->heap.at(r));
		const Node& n1 = this->heap.at(l);
		const Node& n2 = this->heap.at(r);
		Cell& c1 = this->getCell(l);
		Cell& c2 = this->getCell(r);
		c1.heap_position = l;
		c2.heap_position = r;
	}

	Dijkstra::Cell& Dijkstra::getCell(const size_t& pos_in_heap)
	{
		const Node& n = this->heap.at(pos_in_heap);
		Cell& out = (*this->results.find(n.id()));
		return out;
	}

	const Dijkstra::Cell& Dijkstra::getCell(const size_t& pos_in_heap) const
	{
		const Node& n = this->heap.at(pos_in_heap);
		const Cell& out = (*this->results.find(n.id()));
		return out;
	}

	const Dijkstra::Cell& Dijkstra::getCell(const Node& node) const
	{
		return (*this->results.find(node.id()));
	}

	void Dijkstra::UpdateWeight(const Node& node, const Node& prev_node, const double& pathweight)
	{
		Cell& c = (*this->results.find(node.id()));
		c.pathweight = pathweight;
		c.prev_id = prev_node.id();
		if(c.heap_position >= 0) this->RestoreHeap(c.heap_position);
	}
}