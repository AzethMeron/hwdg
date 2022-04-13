
#include <iostream>
#include <limits>
#include <queue>
#include <vector>
#include "dijkstra.hpp"
#include "custom_map.hpp"

namespace Graphs
{
	Dijkstra::Cell::Cell(const Node& n) : node(n)
	{
		this->prev_id = -1;
		this->pathweight = std::numeric_limits<double>::max();
		this->heap_position = -1;
	}
	Dijkstra::Dijkstra(const Graph& graph, const Node& src) : _source(src)
	{
		Algorithm(graph);
	}

	void Dijkstra::Algorithm(const Graph& graph)
	{
		// Initialisation
		for (const Node& node : graph)
		{
			this->results.insert({node.id(), Dijkstra::Cell(node)});
			this->heap.push_back(node);
		}
		(*this->results.find(this->_source.id())).pathweight = 0;
		this->MakeHeap();
	}

	void Dijkstra::MakeHeap()
	{
		// zaczynamy od do³u, od indeksu this->heap.size() / 2 -1
		// here add heapify
		for (size_t i = 0; i < this->heap.size(); ++i)
		{
			const Node& node = this->heap.at(i);
			(*this->results.find(node.id())).prev_id = i;
		}
	}

	void Dijkstra::PushHeap(Node node)
	{
		this->heap.push_back(node);
	}

	Node Dijkstra::PopHeap()
	{
		return Node(1);
	}

	size_t Dijkstra::parent(size_t index) const
	{
		return (index - 1) / 2;
	}

	size_t Dijkstra::leftChild(size_t index) const
	{
		return index * 2 + 1;
	}
	size_t Dijkstra::rightChild(size_t index) const
	{
		return index * 2 + 2;
	}

	// true if n1 mniejsze niz n2 
	bool Dijkstra::HeapCompare(size_t index1, size_t index2) const
	{
		const Node& n1 = this->heap.at(index1);
		const Node& n2 = this->heap.at(index2);
		const Cell& c1 = (*this->results.find(n1.id()));
		const Cell& c2 = (*this->results.find(n2.id()));
		return c1.pathweight < c2.pathweight;
	}

	bool Dijkstra::has(size_t index) const
	{
		return index >= 0 && index < this->heap.size();
	}

	void Dijkstra::RestoreHeap(size_t position)
	{

	}
}