#ifndef GRAPHS_PATH_HPP
#define GRAPHS_PATH_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
#include "custom_map.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "tools.hpp"

namespace Graphs
{
	struct PathtableCell
	{
		const Node node;
		double pathweight;
		int64_t prev_id;
		PathtableCell() = delete;
		PathtableCell(const Node& n, const Node& src);
		PathtableCell(const Node& n, const double& weight, int64_t prev_id);

		static void SaveTxt(std::ostream& file, const PathtableCell& cell);
		static PathtableCell LoadTxt(std::istream& file);
		static void SaveBin(std::ostream& file, const PathtableCell& cell);
		static PathtableCell LoadBin(std::istream& file);
	};

	class Path
	{
	private:
		std::vector<Node> _nodes;
		double _pathweight;
		bool _exists;
	public:
		size_t size(void) const;
		auto begin(void) const;
		auto end(void) const;
		const Node& operator [] (const unsigned int& i) const;
		double Weight(void) const;
		bool Exists(void) const;
		std::string str(void) const;
		Path() = delete;
		Path(const std::vector<Node>& nodes, const double& weight, bool exists);

		static void SaveTxt(std::ostream& file, const Path& path);
		static Path LoadTxt(std::istream& file);
		static void SaveBin(std::ostream& file, const Path& path);
		static Path LoadBin(std::istream& file);
	};

	template<typename TYPE>
	struct Pathtable : public unordered_map<uint32_t, TYPE>
	{
		const Node source;
		Pathtable() = delete;
		Pathtable(const Node& src) : source(src) {}

		bool has(const Node& node) const
		{
			auto iter = this->find(node.id());
			if (iter == this->end()) return false;
			return true;
		}

		void UpdateWeight(const Node& node, const Node& prev_node, const double& pathweight)
		{
			TYPE& c = (*this->find(node.id()));
			c.pathweight = pathweight;
			c.prev_id = prev_node.id();
		}

		const TYPE& getCell(const Node& node) const
		{
			return (*this->find(node.id()));
		}

		TYPE& getCell(const Node& node)
		{
			return (*this->find(node.id()));
		}

		Path GetPath(const Node& tgt) const
		{
			if (!this->has(tgt)) throw std::invalid_argument(string_format("No node %s in graph this algorithm was used on", tgt.str()));
			std::vector<Node> nodes;
			const double weight = this->getCell(tgt).pathweight;
			const bool exists = this->getCell(tgt).prev_id != -1;
			Node analysing = tgt;
			nodes.push_back(analysing);
			while (true)
			{
				const TYPE& c = this->getCell(analysing);
				if (c.prev_id < 0) break;
				analysing = Node(c.prev_id);
				nodes.push_back(analysing);
			}
			std::reverse(nodes.begin(), nodes.end());
			return Path(nodes, weight, exists);
		}
		
		void Initialise(const Graph& graph, const Node& src)
		{
			this->reserve(graph.size_nodes());
			for (const Node& node : graph)
			{
				this->insert({ node.id(), TYPE(node, src) });
			}
		}
		static void SaveTxt(std::ostream& file, const Pathtable<TYPE>& table)
		{
			file << table.source.id() << ' ' << table.size() << ' ';
			for (const auto& cell : table)
			{
				TYPE::SaveTxt(file, cell);
			}
		}

		static Pathtable<TYPE> LoadTxt(std::istream& file)
		{
			Node node = Node::LoadTxt(file);
			Pathtable<TYPE> output(node);
			size_t size;
			file >> size;
			output.reserve(size);
			for (size_t i = 0; i < size; ++i)
			{
				TYPE cell = TYPE::LoadTxt(file);
				output.insert({ cell.node.id(), cell });
			}
			return output;
		}

		static void SaveBin(std::ostream& file, const Pathtable<TYPE>& table)
		{

		}

		static Pathtable<TYPE> LoadBin(std::istream& file)
		{

		}
	};
}

#endif