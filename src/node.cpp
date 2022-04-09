
#include <cstdint>
#include <string>
#include "string_format.hpp"
#include "node.hpp"

const uint32_t& Node::id(void) const
{
	return this->_id;
}

Node::Node(const uint32_t& id)
{
	 this->_id = id;
}
 
Node::Node(const Node& to_copy)
{
	 *this = to_copy;
}
 
Node::Node(const Node&& to_move)
{
	 *this = to_move;
} 

Node& Node::operator = (const Node& to_copy)
{
	this->_id = to_copy.id();
	return *this;
}

bool Node::operator == (const Node& rval) const
{
	return this->id() == rval.id();
}

std::string Node::str(void) const
{
	return string_format("%lu", this->id());
}
