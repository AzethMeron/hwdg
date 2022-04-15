
#include <cstdint>
#include <limits>
#include <string>
#include "tools.hpp"
#include "node.hpp"
#include "edge.hpp"

namespace Graphs
{
	uint64_t Edge::calculate_id(const Node& src, const Node& tgt, float weight)
	{
		return (uint64_t)src.id() + (uint64_t)tgt.id()*(uint64_t)Node::MAX_ID;
	}

	float Edge::weight(void) const
	{
		return this->_weight;
	}

	const Node& Edge::source(void) const
	{
		return this->_src_node;
	}

	const Node& Edge::target(void) const
	{
		return this->_tgt_node;
	}

	uint64_t Edge::id(void) const
	{
		return Edge::calculate_id(this->source(), this->target(), this->weight());
	}

	Edge& Edge::operator = (const Edge& to_copy)
	{
		this->_src_node = to_copy.source();
		this->_tgt_node = to_copy.target();
		this->_weight = to_copy.weight();
		return *this;
	}

	Edge::Edge(const Node& src, const Node& tgt, float weight) : _src_node(src), _tgt_node(tgt)
	{
		this->_weight = weight;
	}

	Edge::Edge(const Node& src, const Node& tgt) : _src_node(src), _tgt_node(tgt)
	{
		*this = Edge(src, tgt, Edge::DUMMY_WEIGHT);
	}

	bool Edge::operator == (const Edge& edge) const
	{
		return this->id() == edge.id();
	}

	std::string Edge::str(void) const
	{
		return string_format("(%s, %s, %f)", this->source().str().c_str(), this->target().str().c_str(), this->weight());
	}
	
	Edge Edge::Scale(const float& factor) const
	{
		return Edge(this->source(), this->target(), this->weight() * factor);
	}

	Edge Edge::Reverse(void) const
	{
		return Edge(this->target(), this->source(), this->weight());
	}

	uint64_t Edge::HashFunction::operator()(const Edge& node) const
	{
		return node.id();
	}

	void Edge::SaveBin(std::ostream& file, const Edge& edge)
	{
		Node::SaveBin(file, edge.source());
		Node::SaveBin(file, edge.target());
		float weight = edge.weight();
		file.write((const char*)&weight, sizeof(weight));
	}

	Edge Edge::LoadBin(std::istream& file)
	{
		Node src = Node::LoadBin(file);
		Node tgt = Node::LoadBin(file);
		float weight = 0;
		file.read((char*)&weight, sizeof(weight));
		return Edge(src, tgt, weight);
	}

	void Edge::SaveTxt(std::ostream& file, const Edge& edge)
	{
		Node::SaveTxt(file, edge.source());
		Node::SaveTxt(file, edge.target());
		file << edge.weight();
	}

	Edge Edge::LoadTxt(std::istream& file)
	{
		Node src = Node::LoadTxt(file);
		Node tgt = Node::LoadTxt(file);
		float weight = 0;
		file >> weight;
		return Edge(src, tgt, weight);
	}
}
