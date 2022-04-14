
#include "node_in_graph.hpp"
#include "tools.hpp"

namespace Graphs
{
	bool NodeInGraph::has(const Edge& edge) const
	{
		auto iter = this->_edges.find(edge.id());
		if(iter == this->_edges.cend()) return false;
		return true;
	}

	bool NodeInGraph::add(const Edge& edge)
	{
		if(this->has(edge)) return false;
		this->_edges.insert({edge.id(), edge});
		return true;
	}

	bool NodeInGraph::add(const Node& tgt_node, float weight)
	{
		Edge edge((const Node&)*this, tgt_node, weight);
		return this->add(edge);
	}

	bool NodeInGraph::remove(const Edge& edge)
	{
		if(this->has(edge))
		{
			this->_edges.erase(edge.id());
			return true;
		}
		return false;
	}

	const_iterator<uint64_t, Edge> NodeInGraph::begin() const
	{
		return this->_edges.cbegin();
	}

	const_iterator<uint64_t, Edge> NodeInGraph::end() const
	{
		return this->_edges.cend();
	}

	size_t NodeInGraph::size_edges(void) const
	{
		return this->_edges.size();
	}

	NodeInGraph::NodeInGraph(const Node& n) : Node(n) {}

	std::string NodeInGraph::str(void) const
	{
		std::string list_of_next = "[";
		for(auto& edge : *this)
		{
			list_of_next.append(string_format(" (%s, %.1f) ", edge.target().str().c_str(), edge.weight()));
		}
		list_of_next.append("]");
		std::string output = string_format("(%s, %s)", Node::str().c_str(), list_of_next.c_str());
		return output;
	}
}
