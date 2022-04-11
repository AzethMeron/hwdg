#pragma once

#include <cstdint>
#include <string>
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
	};
}
