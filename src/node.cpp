
#include <cstdint>
#include <string>
#include "tools.hpp"
#include "node.hpp"

namespace Graphs
{
	const uint32_t& Node::id(void) const
	{
		return this->_id;
	}

	Node::Node(const uint32_t& id)
	{
		 this->_id = id;
	}

	bool Node::operator == (const Node& rval) const
	{
		return this->id() == rval.id();
	}

	std::string Node::str(void) const
	{
		return string_format("%lu", this->id());
	}
}
