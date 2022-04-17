#ifndef HWDG_NODE_HPP
#define HWDG_NODE_HPP

#include <cstdint>
#include <string>
#include <limits>
#include <iostream>

namespace HWDG
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

			static void SaveBin(std::ostream& file, const Node& node);
			static Node LoadBin(std::istream& file);
			static void SaveTxt(std::ostream& file, const Node& node);
			static Node LoadTxt(std::istream& file);
	};
}

#endif
