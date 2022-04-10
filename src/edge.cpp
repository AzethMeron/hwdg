
#include <cstdint>
#include <limits>
#include <string>
#include "string_format.hpp"
#include "node.hpp"
#include "edge.hpp"

namespace Graphs
{
	uint64_t Edge::calculate_id(const Node& src, const Node& tgt, float weight)
	{
		uint64_t limit32 = std::numeric_limits<uint32_t>::max();
		return src.id() + tgt.id()*limit32;
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

	const uint64_t& Edge::id(void) const
	{
		return this->_id;
	}

	Edge& Edge::operator = (const Edge& to_copy)
	{
		this->_src_node = to_copy.source();
		this->_tgt_node = to_copy.target();
		this->_weight = to_copy.weight();
		this->_id = to_copy.id();
		return *this;
	}

	Edge::Edge(const Node& src, const Node& tgt, float weight) : _src_node(src), _tgt_node(tgt)
	{
		this->_weight = weight;
		this->_id = Edge::calculate_id(src, tgt, weight);
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

}
