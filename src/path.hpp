#ifndef GRAPHS_PATH_HPP
#define GRAPHS_PATH_HPP

#include <vector>
#include <string>
#include <cstdint>
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
	};

	template<typename TYPE>
	struct Pathtable : public unordered_map<uint32_t, TYPE>
	{
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
	};
}

#endif