
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

	uint32_t Node::HashFunction::operator()(const Node& node) const
	{
		return node.id();
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

	void Node::SaveBin(std::ostream& file, const Node& node)
	{
		uint32_t id = node.id();
		file.write((const char*)&id, sizeof(id));
	}

	Node Node::LoadBin(std::istream& file)
	{
		uint32_t id = 0;
		file.read((char*)&id, sizeof(id));
		return Node(id);
	}

	void Node::SaveTxt(std::ostream& file, const Node& node)
	{
		file << node.id();
	}

	Node Node::LoadTxt(std::istream& file)
	{
		uint32_t id = 0;
		file >> id;
		return Node(id);
	}
}
