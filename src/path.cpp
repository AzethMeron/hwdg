
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

	PathtableCell::PathtableCell(const Node& n, const double& weight, int64_t prev_id) : node(n)
	{
		this->pathweight = weight;
		this->prev_id = prev_id;
	}

	void PathtableCell::SaveTxt(std::ostream& file, const PathtableCell& cell)
	{
		file << cell.node.id() << ' ' << cell.pathweight << ' ' << cell.prev_id << ' ';
	}

	PathtableCell PathtableCell::LoadTxt(std::istream& file)
	{
		uint32_t node_id;
		double weight;
		int64_t prev_id;
		file >> node_id >> weight >> prev_id;
		return PathtableCell(Node(node_id), weight, prev_id);
	}

	void PathtableCell::SaveBin(std::ostream& file, const PathtableCell& cell)
	{
		file.write((const char*)&cell.node.id(), sizeof(cell.node.id()));
		file.write((const char*)&cell.pathweight, sizeof(cell.pathweight));
		file.write((const char*)&cell.prev_id, sizeof(cell.prev_id));
	}

	PathtableCell PathtableCell::LoadBin(std::istream& file)
	{
		uint32_t node_id;
		double weight;
		int64_t prev_id;
		file.read((char*)&node_id, sizeof(node_id));
		file.read((char*)&weight, sizeof(weight));
		file.read((char*)&prev_id, sizeof(prev_id));
		return PathtableCell(Node(node_id), weight, prev_id);
	}

	void Path::SaveTxt(std::ostream& file, const Path& path)
	{
		file << path._exists << ' ' << path._pathweight << ' ' << path._nodes.size() << ' ';
		for (const auto& node : path._nodes)
		{
			Node::SaveTxt(file, node);
		}
	}

	Path Path::LoadTxt(std::istream& file)
	{
		bool exists;
		double pathweight;
		size_t size;
		std::vector<Node> nodes;
		file >> exists >> pathweight >> size;
		nodes.reserve(size);
		for (size_t i = 0; i < size; ++i)
		{
			Node node = Node::LoadTxt(file);
			nodes.push_back(node);
		}
		return Path(nodes, pathweight, exists);
	}

	void Path::SaveBin(std::ostream& file, const Path& path)
	{
		file.write((const char*)&path._exists, sizeof(path._exists));
		file.write((const char*)&path._pathweight, sizeof(path._pathweight));
		auto size = path._nodes.size();
		file.write((const char*)&size, sizeof(size));
		for (const Node& node : path)
		{
			Node::SaveBin(file, node);
		}
	}

	Path Path::LoadBin(std::istream& file)
	{
		bool exists;
		double pathweight;
		size_t size;
		file.read((char*)&exists, sizeof(exists));
		file.read((char*)&pathweight, sizeof(pathweight));
		file.read((char*)&size, sizeof(size));
		std::vector<Node> nodes;
		nodes.reserve(size);
		for (size_t i = 0; i < size; ++i)
		{
			Node n = Node::LoadBin(file);
			nodes.push_back(n);
		}
		return Path(nodes, pathweight, exists);
	}
}

