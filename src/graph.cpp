
#include <string>
#include "string_format.hpp"
#include "graph.hpp"

namespace Graphs
{
	
	unsigned int Graph::size_edges(void) const
	{
		return this->_edges.size();
	}

	unsigned int Graph::size_nodes(void) const
	{
		return this->_nodes.size();
	}

	const_iterator<uint32_t, NodeInGraph> Graph::begin() const
	{
		return this->_nodes.cbegin();
	}

	const_iterator<uint32_t, NodeInGraph> Graph::end() const
	{
		return this->_nodes.cend();
	}

	bool Graph::has(const Edge& edge) const
	{
		const_iterator<uint64_t, Edge> iter = this->_edges.find(edge.id());
		if(iter == this->_edges.cend())
		{
			return false;
		}
		return true;
	}

	bool Graph::has(const Node& node) const
	{
		const_iterator<uint32_t, NodeInGraph> iter = this->_nodes.find(node.id());
		if(iter == this->_nodes.cend())
		{
			return false;
		}
		return true;
	}

	const_iterator<uint32_t, NodeInGraph> Graph::get_node(const Node& node) const
	{
		return this->_nodes.find(node.id());
	}

	const_iterator<uint64_t, Edge> Graph::get_edge(const Edge& edge) const
	{
		return this->_edges.find(edge.id());
	}

	const NodeInGraph& Graph::fetch(const Node& node) const
	{
		auto iter = this->get_node(node);
		if(iter == this->end()) throw std::out_of_range(string_format("No such node: %s", node.str().c_str()));
		return *iter;
	}

	const Edge& Graph::fetch(const Edge& edge) const
	{
		auto iter = this->get_edge(edge);
		if(iter == this->_edges.cend()) throw std::out_of_range(string_format("No such edge: %s", edge.str().c_str()));
		return *iter;
	}

	bool Graph::add(const Node& node)
	{
		if(this->has(node)) return false;
		NodeInGraph n = node;
		this->_nodes.insert({node.id(), n});
		return true;
	}

	bool Graph::add(const Edge& edge)
	{
		if(this->has(edge)) return false;
		this->_edges.insert({edge.id(), edge});
		this->_weight_sum = this->_weight_sum + edge.weight();
		const Node& src = edge.source();
		if(!this->has(src)) this->add(src);
		auto iter = this->_nodes.find(src.id());
		(*iter).add(edge);
		this->add(edge.target()); 
		return true;
	}
	
	void Graph::update(const Edge& edge)
	{
		this->remove(edge);
		this->add(edge);
	}
	

	bool Graph::remove(const Edge& edge)
	{
		if(!this->has(edge)) return false;
		this->_weight_sum = this->_weight_sum - this->fetch(edge).weight();
		this->_edges.erase(edge.id());
		const Node& src = edge.source();
		auto iter = this->_nodes.find(src.id());
		(*iter).remove(edge);
		return true;
	}

	std::string Graph::str(void) const
	{
		std::string output = "[\n";
		for(const auto& node : *this)
		{
			std::string tmp = string_format("	%s\n", node.str().c_str());
			output.append(tmp);
		}
		output.append("]");
		return output;
	}

	std::string Graph::str_edges(void) const
	{
		std::string output = "[\n";
		for(const auto& edge : this->_edges)
		{
			output.append(string_format("	%s\n", edge.str().c_str()));
		}
		output.append("]");
		return output;
	}

	float Graph::density(void) const
	{
		float max_edges = this->size_nodes()*this->size_nodes();
		float edges = this->size_edges();
		return edges / max_edges;
	}

	Graph::Graph()
	{
		this->_weight_sum = 0;
	}

	double Graph::weight_sum(void) const
	{
		return this->_weight_sum;
	}

	const unordered_map<uint64_t, Edge>& Graph::edges(void) const
	{
		return this->_edges;
	}

	const unordered_map<uint32_t, NodeInGraph>& Graph::nodes(void) const
	{
		return this->_nodes;
	}

	Graph::Graph(const std::initializer_list<Edge>& l)
	{
		this->_weight_sum = 0;
		this->add(l);
	}
	
	Graph::Graph(const std::initializer_list<Node>& l)
	{
		this->_weight_sum = 0;
		this->add(l);
	}
	
	Graph::Graph(const std::initializer_list<Node>& nodes, const std::initializer_list<Edge>& edges)
	{
		this->_weight_sum = 0;
		this->add(nodes);
		this->add(edges);
	}
	
	void Graph::add(const std::initializer_list<Edge>& l)
	{
		for(const Edge& edge : l)
		{
			this->add(edge);
		}
	}
	
	void Graph::add(const std::initializer_list<Node>& l)
	{
		for(const Node& node : l)
		{
			this->add(node);
		}
	}
}
