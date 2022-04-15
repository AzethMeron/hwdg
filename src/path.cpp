
#include <vector>
#include <string>
#include <limits>
#include "node.hpp"
#include "edge.hpp"
#include "path.hpp"
#include "tools.hpp"

namespace Graphs
{
	PathtableCell::PathtableCell(const Node& n, const Node& src) : node(n)
	{
		this->prev_id = -1;
		if (n == src) this->prev_id = -2;
		this->pathweight = std::numeric_limits<double>::max();
		if (n == src) this->pathweight = 0;
	}

	size_t Path::size(void) const
	{
		return this->_nodes.size();
	}

	auto Path::begin(void) const
	{
		return this->_nodes.begin();
	}
	auto Path::end(void) const
	{
		return this->_nodes.end();
	}

	const Node& Path::operator [] (const unsigned int& i) const
	{
		return this->_nodes.at(i);
	}

	double Path::Weight(void) const
	{
		return this->_pathweight;
	}

	bool Path::Exists(void) const
	{
		return this->_exists;
	}

	std::string Path::str(void) const
	{
		if (!this->Exists())
		{
			return std::string("Path doesn't exist");
		}
		std::string output = string_format("Pathweight: %f [ ", this->Weight());
		for (const Node& node : *this)
		{
			output.append(node.str() + std::string(" "));
		}
		output.append("]");
		return output;
	}

	Path::Path(const std::vector<Node>& nodes, const double& weight, bool exists)
	{
		this->_nodes = nodes;
		this->_pathweight = weight;
		this->_exists = exists;
	}
}

