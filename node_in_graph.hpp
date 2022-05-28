#ifndef HWDG_NODE_IN_GRAPH_HPP
#define HWDG_NODE_IN_GRAPH_HPP

#include <cstdint>
#include <string>
#include "custom_map.hpp"
#include "node.hpp"
#include "edge.hpp"

namespace HWDG
{
	class NodeInGraph : public Node
	{
		private:
			Map::unordered_map<uint64_t, Edge> _edges;
		public:
			bool has(const Edge& edge) const; // true - zawiera, constant
			bool add(const Edge& edge); // false - nie udalo sie dodac (juz istnieje taki edge), constant
			bool add(const Node& tgt_node, float weight); // constant
			bool remove(const Edge& edge); // constant
			
			Map::const_iterator<uint64_t, Edge> begin() const;
			Map::const_iterator<uint64_t, Edge> end() const;
			size_t size_edges(void) const; // constant
			std::string str(void) const; // linear
			
			NodeInGraph() = delete;
			NodeInGraph(const Node& n);

			void reserve_edges(const size_t& count);
	};

}

#endif