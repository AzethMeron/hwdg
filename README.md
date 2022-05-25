# HWDG - Hashtable-based Weighted Directed Graphs
This is WIP. For now, it's C++ library without documentation, but when I finish it should have Doxygen comments and Python bindings (using pybind)  

# What is it?
HWDG is graph implementation written in C++ with heavy emphasis on time complexity. Internally, it uses unordered maps from STL to store nodes and edges. Because of this, it is very efficent when it comes to adding new nodes/edges, or checking if edge exists within a graph (constant time complexity for each). Downside of this approach is space-complexity.

# Features
- Graph representation using list of successors (except list is replaced with map)
- Serialization of graphs (and most of other types)
- Addition of edge, removal or check of existance has constant time complexity.
- Heap-based Dijkstra algorithm.
- Bellman-Ford algorithm.
- Graph union, intersection, difference, several algorithms for graph similarity check.
- and more.

# Brief introduction
Given there's no documentation yet, I've decided to make quick instruction on how-to-use.

Following code generates random graph of size 10 with density=50%, minimal weight of edge=10 and maximal=400, with loops allowed. Then, string representation of this graph is displayed to standard output. Finally, graph is saved to file "graph" in binary form.
```c++
#include <iostream>
#include "hwdg.hpp"

int main()
{
	HWDG::Graph graph = HWDG::RandomLowDensityGraph(10, 0.5, 10, 400, true);
	std::cout << "Generated random graph: " << graph.str();
	HWDG::SaveGraphBinFile(graph, "graph");
	return 0;
}
```

Let's load previously generated graph, generate new one, and try some basic operations: union, difference, instersection
```c++
#include <iostream>
#include "hwdg.hpp"

int main()
{
	HWDG::Graph graph_a = HWDG::RandomGraph(10, 0.5, 10, 400, true);
	HWDG::Graph graph_b = HWDG::LoadGraphBinFile("graph.txt");
	std::cout << "Union: " << HWDG::Union(graph_a, graph_b).str() << std::endl;
	std::cout << "Difference: " << HWDG::Difference(graph_a, graph_b).str() << std::endl;
	std::cout << "Intersection: " << HWDG::Intersection(graph_a, graph_b, 0.5).str() << std::endl;
	return 0;
}
```

And now, let's create graph manually:
