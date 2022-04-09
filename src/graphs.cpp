
#include <string>
#include "string_format.hpp"
#include "graphs.hpp"

unsigned int Graph::size_edges(void) const
{
	return this->_edges.size();
}

unsigned int Graph::size_nodes(void) const
{
	return this->_nodes.size();
}

typename std::unordered_map<uint32_t, NodeInGraph>::const_iterator Graph::begin() const
{
	return this->_nodes.cbegin();
}

typename std::unordered_map<uint32_t, NodeInGraph>::const_iterator Graph::end() const
{
	return this->_nodes.cend();
}

bool Graph::has_edge(const Edge& edge) const
{
	std::unordered_map<uint64_t, Edge>::const_iterator iter = this->_edges.find(edge.id());
	if(iter == this->_edges.cend())
	{
		return false;
	}
	return true;
}

bool Graph::has_node(const Node& node) const
{
	std::unordered_map<uint32_t, NodeInGraph>::const_iterator iter = this->_nodes.find(node.id());
	if(iter == this->_nodes.cend())
	{
		return false;
	}
	return true;
}

typename std::unordered_map<uint32_t, NodeInGraph>::const_iterator Graph::get_node(const Node& node) const
{
	return this->_nodes.find(node.id());
}

typename std::unordered_map<uint64_t, Edge>::const_iterator Graph::get_edge(const Edge& edge) const
{
	return this->_edges.find(edge.id());
}

const NodeInGraph& Graph::fetch_node(const Node& node) const
{
	auto iter = this->get_node(node);
	if(iter == this->end()) throw std::out_of_range(string_format("No such node: %s", node.str()).c_str());
	return iter->second;
}

const Edge& Graph::fetch_edge(const Edge& edge) const
{
	auto iter = this->get_edge(edge);
	if(iter == this->_edges.cend()) throw std::out_of_range(string_format("No such edge: %s", edge.str()).c_str());
	return iter->second;
}

bool Graph::add_node(const Node& node)
{
	if(this->has_node(node)) return false;
	NodeInGraph n = node;
	this->_nodes.insert({node.id(), n});
	return true;
}

bool Graph::add_edge(const Edge& edge)
{
	if(this->has_edge(edge)) return false;
	this->_edges.insert({edge.id(), edge});
	const Node& src = edge.source();
	if(!this->has_node(src)) this->add_node(src);
	std::unordered_map<uint32_t, NodeInGraph>::iterator iter = this->_nodes.find(src.id());
	iter->second.add_edge(edge);
	this->add_node(edge.target()); 
	return true;
}

bool Graph::remove_edge(const Edge& edge)
{
	if(!this->has_edge(edge)) return false;
	this->_edges.erase(edge.id());
	const Node& src = edge.source();
	std::unordered_map<uint32_t, NodeInGraph>::iterator iter = this->_nodes.find(src.id());
	iter->second.remove_edge(edge);
	return true;
}

std::string Graph::str(void) const
{
	std::string output = "[\n";
	for(auto pair : *this)
	{
		std::string tmp = string_format("	%s\n", pair.second.str().c_str());
		output.append(tmp);
	}
	output.append("\n]");
	return output;
}

std::string Graph::str_edges(void) const
{
	std::string output = "[\n";
	for(auto pair : this->_edges)
	{
		output.append(string_format("	%s\n", pair.second.str().c_str()));
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
