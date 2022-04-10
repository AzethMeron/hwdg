#pragma once

#include <cstdint>

namespace Graphs
{
	class Node 
	{
		private:
			uint32_t _id;
		public:
			const uint32_t& id(void) const;
			Node() = delete;
			Node(const uint32_t& id);
			Node(const Node& to_copy);
			Node(const Node&& to_move);
			Node& operator = (const Node& to_copy);
			bool operator == (const Node& rval) const;
			std::string str(void) const;
	};
}
