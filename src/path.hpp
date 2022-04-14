#ifndef GRAPHS_PATH_HPP
#define GRAPHS_PATH_HPP

#include <vector>
#include <string>
#include "node.hpp"
#include "edge.hpp"

namespace Graphs
{
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
}

#endif