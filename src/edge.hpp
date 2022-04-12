#ifndef GRAPHS_EDGE_HPP
#define GRAPHS_EDGE_HPP

#include <cstdint>
#include <string>
#include <limits>
#include "node.hpp"

namespace Graphs
{
	class Edge
	{
		private:
			Node _src_node;
			Node _tgt_node;
			float _weight;
			static uint64_t calculate_id(const Node& src, const Node& tgt, float weight);
		public:
			static constexpr float DUMMY_WEIGHT = 1;
			static constexpr float MAX_WEIGHT = std::numeric_limits<float>::max();
			static constexpr float MIN_WEIGHT = std::numeric_limits<float>::min();
		public:
			float weight(void) const;
			const Node& source(void) const;
			const Node& target(void) const;
			Edge() = delete;
			Edge(const Node& src, const Node& tgt, float weight);
			Edge(const Node& src, const Node& tgt);
			Edge& operator = (const Edge& to_copy);
			uint64_t id(void) const;
			bool operator == (const Edge& edge) const;
			std::string str(void) const;
			Edge Scale(const float& factor) const;
			Edge Reverse(void) const;
	};
}

#endif
