#include "bfspathfinding.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "graph.hpp"
#include "path.hpp"
#include "operations.hpp"

namespace HWDG
{
	Pathtable<PathtableCell> BFSPathfinding::Compute(const Graph& graph, const Node& src)
	{
		Pathtable<PathtableCell> table(src);
		table.Initialise(graph, src);

		Operations::BreadthFirstSearch(graph, src,
			[&table](const Edge& edge, const auto& visited)
			{
				auto prev_cell = table.getCell(edge.source());
				table.UpdateWeight(edge.target(), edge.source(), prev_cell.pathweight + BFSPathfinding::EdgeWeight);
			});

		return table;
	}
}