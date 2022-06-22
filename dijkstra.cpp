
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include "dijkstra.hpp"
#include "custom_map.hpp"
#include "tools.hpp"
#include "path.hpp"

namespace HWDG
{
	Dijkstra::Cell::Cell(const Node& n, const Node& src) : PathtableCell(n,src)
	{
		this->heap_position = -1;
	}
	Dijkstra::Dijkstra(const Graph& graph, const Node& src) : _results(graph, src)
	{
		if (!graph.has(src)) throw std::invalid_argument(Tools::string_format("Node %s doesn't belong to given graph", src.str().c_str()));
		Algorithm(graph);
	}
	Dijkstra::Cell::Cell(const PathtableCell& cell) : PathtableCell(cell)
	{
		this->heap_position = -1;
	}

	void Dijkstra::Algorithm(const Graph& graph)
	{
		// Condition check
		if (graph.has_negative_weights()) throw std::invalid_argument("Dijkstra algorithm cannot be used for graphs with negative weights of edges.");
		// Initialisation
		this->reserve(graph.size_nodes());
		for (const Node& node : graph) { this->HeapPush(node); }
		for (size_t i = 0; i < this->HeapSize(); ++i)
		{
			Cell& c = this->getCell(i);
			c.heap_position = i;
		}
		// Convert vector to heap. From this point, both results and heap are synchronized
		// Dijkstra Algorithm
		while (this->HeapSize())
		{
			const NodeInGraph& current = graph.fetch(this->HeapPop());
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

	Node Dijkstra::HeapPop()
	{
		Node output = Heap::HeapPop();
		(*this->_results.find(output.id())).heap_position = -1;
		return output;
	}

	// true if n1 wieksze niz n2 
	bool Dijkstra::HeapCompare(const size_t& index1, const size_t& index2) const
	{
		const Cell& c1 = this->getCell(index1);
		const Cell& c2 = this->getCell(index2);
		return c1.pathweight > c2.pathweight;
	}

	void Dijkstra::HeapSwap(const size_t& l, const size_t& r)
	{
		std::swap(this->HeapAt(l), this->HeapAt(r));
		Cell& c1 = this->getCell(l);
		Cell& c2 = this->getCell(r);
		c1.heap_position = l;
		c2.heap_position = r;
	}

	Dijkstra::Cell& Dijkstra::getCell(const size_t& pos_in_heap)
	{
		const Node& n = this->HeapAt(pos_in_heap);
		return this->_results.getCell(n);
	}

	const Dijkstra::Cell& Dijkstra::getCell(const size_t& pos_in_heap) const
	{
		const Node& n = this->HeapAt(pos_in_heap);
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

	const Pathtable<Dijkstra::Cell>& Dijkstra::Results(void) const
	{
		return this->_results;
	}

	Pathtable<Dijkstra::Cell> Dijkstra::Compute(const Graph& graph, const Node& src)
	{
		Dijkstra vessel(graph, src);
		return vessel.Results();
	}
}
