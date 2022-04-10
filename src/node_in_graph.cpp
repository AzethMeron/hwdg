
#include "node_in_graph.hpp"
#include "string_format.hpp"

namespace Graphs
{
	bool NodeInGraph::has_edge(const Edge& edge) const
	{
		std::unordered_map<uint64_t, Edge>::const_iterator iter = this->_edges.find(edge.id());
		if(iter == this->_edges.cend()) return false;
		return true;
	}

	bool NodeInGraph::add_edge(const Edge& edge)
	{
		if(this->has_edge(edge)) return false;
		this->_edges.insert({edge.id(), edge});
		return true;
	}

	bool NodeInGraph::add_edge(const Node& tgt_node, float weight)
	{
		Edge edge((const Node&)*this, tgt_node, weight);
		return this->add_edge(edge);
	}

	bool NodeInGraph::remove_edge(const Edge& edge)
	{
		if(this->has_edge(edge))
		{
			this->_edges.erase(edge.id());
			return true;
		}
		return false;
	}

	typename std::unordered_map<uint64_t, Edge>::const_iterator NodeInGraph::begin() const
	{
		return this->_edges.cbegin();
	}

	typename std::unordered_map<uint64_t, Edge>::const_iterator NodeInGraph::end() const
	{
		return this->_edges.cend();
	}

	unsigned int NodeInGraph::size_edges(void) const
	{
		return this->_edges.size();
	}

	NodeInGraph::NodeInGraph(const Node& n) : Node(n) {}

	std::string NodeInGraph::str(void) const
	{
		std::string list_of_next = "[";
		for(auto pair : *this)
		{
			list_of_next.append(string_format(" (%s, %.1f) ", pair.second.target().str().c_str(), pair.second.weight()));
		}
		list_of_next.append("]");
		std::string output = string_format("(%s, %s)", Node::str().c_str(), list_of_next.c_str());
		return output;
	}
}
