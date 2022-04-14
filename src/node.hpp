#ifndef GRAPHS_NODE_HPP
#define GRAPHS_NODE_HPP

#include <cstdint>
#include <string>

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
			bool operator == (const Node& rval) const;
			std::string str(void) const;

			static constexpr uint32_t MIN_ID = std::numeric_limits<uint32_t>::min();
			static constexpr uint32_t MAX_ID = std::numeric_limits<uint32_t>::max();

			struct HashFunction
			{
				uint32_t operator()(const Node& node) const;
			};
	};
}

#endif