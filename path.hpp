#ifndef HWDG_PATH_HPP
#define HWDG_PATH_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
#include "custom_map.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "tools.hpp"

namespace HWDG
{
	struct PathtableCell
	{
		const Node node;
		double pathweight;
		int64_t prev_id;
		std::string str(void) const;
		PathtableCell() = delete;
		PathtableCell(const Node& n, const Node& src);
		PathtableCell(const Node& n, const double& weight, int64_t prev_id);

		static void SaveTxt(std::ostream& file, const PathtableCell& cell);
		static PathtableCell LoadTxt(std::istream& file);
		static void SaveBin(std::ostream& file, const PathtableCell& cell);
		static PathtableCell LoadBin(std::istream& file);

		static constexpr int64_t NO_PREVIOUS = -1;
		static constexpr int64_t STARTING = -2;
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
	struct Pathtable : public Map::unordered_map<uint32_t, TYPE>
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
			const bool exists = this->getCell(tgt).prev_id != TYPE::NO_PREVIOUS;
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

		std::string str(void) const
		{
			std::string output = "";
			for (const auto& cell : *this)
			{
				output = output + cell.str() + "\n";
			}
			return output;
		}

		static void SaveTxt(std::ostream& file, const Pathtable<TYPE>& table)
		{
			Node::SaveTxt(file, table.source);
			file << table.size() << ' ';
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
			Node::SaveBin(file, table.source);
			size_t size = table.size();
			file.write((const char*)&size, sizeof(size));
			for (const auto& cell : table)
			{
				TYPE::SaveBin(file, cell);
			}
		}

		static Pathtable<TYPE> LoadBin(std::istream& file)
		{
			Node source = Node::LoadBin(file);
			Pathtable<TYPE> output(source);
			size_t size = 0;
			file.read((char*)&size, sizeof(size));
			output.reserve(size);
			for (size_t i = 0; i < size; ++i)
			{
				TYPE cell = TYPE::LoadBin(file);
				output.insert({ cell.node.id(), cell });
			}
			return output;
		}
	};
}

#endif