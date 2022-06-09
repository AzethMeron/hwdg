# HWDG - Hashtable–based Weighted Directed Graphs

# What is it?
HWDG is a graph implementation written in C++ with heavy emphasis on time complexity. Internally, it uses unordered maps from STL to store nodes and edges. Because of this, it is very efficient when it comes to adding new nodes/edges, or checking if an edge exists within a graph (constant time complexity for each). The downside of this approach is space-complexity.

# What is Graph? Brief, rudimentary introduction
Graph is mathematical representation of network and can be used to model numerous systems. Typical application of graph is pathfinding (in other words, finding shortest path between two points on map, for example from town to town) although it can be used in many other fields, personally I've used graphs in my NLP-based thesis.

Once you create graph model of your system, you can perform on it many operations and algorithms defined for graphs, like Union, Intersection and more, in similar fashion to any other mathematical model. Think about it this way: if you create quadratic equation (y=ax+^2+bx+c) that models your system, then you can extract some info about this system by using any mathematical tool, you can find minimal/maximal y or calculate derivative. Similarly, for graphs you can easily find shortest path between two states in network using f.e. Dijkstra algorithm. 

This implementation features Weighted Directed Graphs, so Graphs consists of Nodes (also called Vertices in literature) and Edges pointing from Node A to Node B with weight W. Example of such simple graph:

![ezgif-1-a456a8853e](https://user-images.githubusercontent.com/41695668/170822880-fc0ba747-e863-4aa3-b9a0-b695f782c32e.png)

# Documentation
This project has documentation written in Doxygen. You can view it by opening Dox/html/index.html

# Features
- Graph representation using list of successors (except list is replaced with unordered map)
- Serialization of graphs (and most of other types)
- Addition of edge, removal or check of existence has constant time complexity.
- Heap-based Dijkstra algorithm.
- Bellman-Ford algorithm.
- Graph union, intersection, difference, several algorithms for graph similarity check.
- BFS and DFS search of graph.
- and more.

# Third-party code used
Doxygen dark theme by MaJerle: https://github.com/MaJerle/doxygen-dark-theme

String formatting by iFreilicht: https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf

# Few notes about terms
Graphs are built using Nodes (also called Vertices in literature) and weighted Edges that points from source Node to target Node. Two edges are considered identical if their source and target nodes have the same ID - weight is omitted during comparison. Nodes are identified by ID - unsigned 32bit integer. If you want to store some other datatype inside your graph, like strings, you can use externally implemented dictionary and hashing functions to do so. For any node A and B, within the graph, there can be only one edge with source node A and target node B.

Nodes DO NOT have to have consecutive ID numbers, I've been using such in examples only for simplicity. It's an unordered map, so ID can be any unsigned 32bit integer.

Internally, Graph uses unordered map, which means the order of nodes is undefined.

# Brief introduction
Given there's no documentation yet, I've decided to make quick instruction on how-to-use.

Following code generates a random graph of size 10 with density=50%, minimal weight of edge=10 and maximal=400, with loops allowed. Then, a string representation of this graph is displayed to standard output. Finally, the graph is saved to file "graph" in binary form.
```c++
#include <iostream>
#include "hwdg.hpp"

int main()
{
	HWDG::Graph graph = HWDG::RandomLowDensityGraph(10, 0.5, 10, 400, true);
	std::cout << "Generated random graph: " << graph.str();
	HWDG::SaveBin(graph, "graph");
	return 0;
}
```

Let's load previously generated graph, generate new one, and try some basic operations: union, difference, intersection
```c++
#include <iostream>
#include "hwdg.hpp"

int main()
{
	HWDG::Graph graph_a = HWDG::RandomGraph(10, 0.5, 10, 400, true);
	HWDG::Graph graph_b = HWDG::LoadBin<HWDG::Graph>("graph");
	std::cout << "Union: " << HWDG::Union(graph_a, graph_b).str() << std::endl;
	std::cout << "Difference: " << HWDG::Difference(graph_a, graph_b).str() << std::endl;
	std::cout << "Intersection: " << HWDG::Intersection(graph_a, graph_b, 0.5).str() << std::endl;
	return 0;
}
```

And now, let's create the graph manually:
```c++
#include <iostream>
#include "hwdg.hpp"

int main()
{
	// Creating blank graph
	HWDG::Graph graph;
	// Creating list of nodes (vertices)
	HWDG::Node n[6] = { HWDG::Node(0), HWDG::Node(1), HWDG::Node(2), HWDG::Node(3), HWDG::Node(4), HWDG::Node(5) };
	// Adding nodes to graph
	graph.add(n[0]);
	// It supports initializer list too
	graph.add({ n[1], n[2], n[3], n[4], n[5] });
	// Adding edges
	graph.add(HWDG::Edge(n[0], n[1], 3)); // Creating edge from n[0] to n[1] with weight=3
	graph.add({ // initializer list is supported here too
		HWDG::Edge(n[0], n[4], 3),
		HWDG::Edge(n[1], n[2]), // if weight isn't specified, it defaults to 1
		HWDG::Edge(n[2], n[3], 3),
		HWDG::Edge(n[2], n[5], 1),
		HWDG::Edge(n[3], n[1], 3),
		HWDG::Edge(n[4], n[5], 2),
		HWDG::Edge(n[5], n[0], 6),
		HWDG::Edge(n[5], n[3], 1)
		});
	// Getting some information
	std::cout << "Density: " << graph.density() << std::endl
		<< "Nodes: " << graph.size_nodes() << std::endl
		<< "Edges: " << graph.size_edges() << std::endl
		<< "Sum of weights: " << graph.weight_sum() << std::endl
		<< "Has negative weights: " << graph.has_negative_weights() << std::endl;
	// Check for nodes/edges
	std::cout << "Has node 3? " << graph.has(n[3]) << std::endl
		<< "Has edge 2->3? " << graph.has(HWDG::Edge(n[2], n[3])) << std::endl
		<< "Has edge 3->2? " << graph.has(HWDG::Edge(n[3], n[2])) << std::endl;
	// Saving to file in text form
	HWDG::SaveTxt(graph, "manual.txt");
	return 0;
}
```

Now, Dijkstra and BellmanFord algorithms. I'll use the graph from the previous example (which was saved in manual.txt)
```c++
#include <iostream>
#include "hwdg.hpp"

int main()
{
    HWDG::Graph graph = HWDG::LoadTxt<HWDG::Graph>("manual.txt");
    try {
        auto dijkstra_path = HWDG::Dijkstra::Compute(graph, HWDG::Node(0));
        auto bellman_path = HWDG::BellmanFord::Compute(graph, HWDG::Node(0));
        std::cout << dijkstra_path.str() << std::endl;
        std::cout << bellman_path.str() << std::endl;
    }
    catch (const std::invalid_argument& e)
    {
        // This section will be triggered if graph contains negative edges
        // or if there's negative cycle, in case of Bellman-Ford algorithm
        std::cout << "Exception occured: " << e.what() << std::endl;
    }
    return 0;
}
```

Traversing graph (through nodes and edges originating from each node)
```c++
#include <iostream>
#include "hwdg.hpp"

int main()
{
	HWDG::Graph graph = HWDG::LoadTxt<HWDG::Graph>("manual.txt");
	for (const auto& node : graph)
	{
		for (const auto& edge : node)
		{
			std::cout << "Traversing: " << node.id() << " - " << edge.str() << std::endl;
		}
	}
	return 0;
}
```

Traversing graph (through all edges, in undefined order)
```c++
#include <iostream>
#include "hwdg.hpp"

int main()
{
	HWDG::Graph graph = HWDG::LoadTxt<HWDG::Graph>("manual.txt");
	for (const auto& edge : graph.edges())
	{
		std::cout << edge.str() << std::endl;
	}
	return 0;
}
```

You can also use BreadthFirstSearch() and DepthFirstSearch() to traverse graph, in slightly different manner.
```c++
#include <iostream>
#include "hwdg.hpp"

int main()
{
	HWDG::Graph graph = HWDG::LoadTxt<HWDG::Graph>("manual.txt");
	HWDG::BreadthFirstSearch(graph, HWDG::Node(3), [&graph](const HWDG::Edge& edge, const auto& visited) {
		HWDG::NodeInGraph current = graph.fetch(edge.target());
		for (const auto& e : current)
		{
			// do something with all neighbours of current node
			// if you want to check whether node was visited...
			auto check_if_visited = visited.find(e.target());
			if (check_if_visited != visited.end()) // Check if target node was visited
			{ }
			else // not visited
			{ }
		}
	});
	return 0;
}
```

There's also another implementation of functions above that allows you to specify order (priority) of neighbours to follow, so you still do BFS/DFS algorithms but with more control which path within graph you follow.
```c++
#include <iostream>
#include <algorithm>
#include "hwdg.hpp"

int main()
{
	HWDG::Graph graph = HWDG::RandomLowDensityGraph(100, 0.3, 100, 150, true);
	HWDG::DepthFirstSearch(graph, HWDG::Node(3),
		[&graph](const HWDG::Edge& edge, const auto& set_of_visited) {
			HWDG::NodeInGraph current = graph.fetch(edge.target());
			std::cout << edge.str() << std::endl;
		},
		[](const HWDG::NodeInGraph& node, std::vector<HWDG::Edge>& to_be_visited, const auto& set_of_visited) {
			// add all neighbours in undefined order
			for (const HWDG::Edge& edge : node)
			{
				to_be_visited.push_back(edge); // no need to worry about visited/not visited, underlying algorithm will deal with it on its' own
			}
			// sort ascending by weight
			std::sort(to_be_visited.begin(), to_be_visited.end(), [](const auto& a, const auto& b) { return a.weight() < b.weight(); });
		});
	return 0;
}
```

# Serialization
You could see in examples above LoadTxt(), SaveTxt(), LoadBin(), SaveBin() functions. Those are wrappers for more raw functions that allow you to store most of datatypes in files (or streams). All serialization functions are written as static member functions of classes, and have no checksum control or any sanity check for loaded data, so be careful.

SaveTxt stores datatype with basic string representation that can be viewed in any text editor, and it's actually a viable way to create small graphs. Txt representation also works the same for any computer architecture, regardless of big endian / little endian. Downsides are: it's slower and wasteful when it comes to space.

SaveBin stores as little data as necessary, representing attributes with their binary form. It's fast and space efficient, but you can't even safely open such files, and it depends on computer architecture.
```c++
#include <iostream>
#include <fstream>
#include "hwdg.hpp"

int main()
{
	std::ofstream txt_file; txt_file.open("serial.txt"); // File to save TXT representation
	std::ofstream bin_file;	bin_file.open("serial", std::ios::out | std::ios::binary); // File to save BIN representation

	HWDG::Node node(420);
	HWDG::Node::SaveTxt(txt_file, node); // Readable and platform-independant, but slow and spaceful way of serialization
	HWDG::Node::SaveBin(bin_file, node); // Unreadable and platform-dependant, but fast and compact way of serialization

	HWDG::Edge edge(HWDG::Node(0), HWDG::Node(1));
	HWDG::Edge::SaveTxt(txt_file, edge);
	HWDG::Edge::SaveBin(bin_file, edge);
	return 0;
}
```
```c++
#include <iostream>
#include <fstream>
#include "hwdg.hpp"

int main()
{
	std::ifstream txt_file; txt_file.open("serial.txt");
	std::ifstream bin_file;	bin_file.open("serial", std::ios::in | std::ios::binary);

	HWDG::Node n1 = HWDG::Node::LoadTxt(txt_file);
	HWDG::Node n2 = HWDG::Node::LoadBin(bin_file);

	HWDG::Edge e1 = HWDG::Edge::LoadTxt(txt_file);
	HWDG::Edge e2 = HWDG::Edge::LoadBin(bin_file);

	return 0;
}
```

# Performance
Operating on graphs is fast. All basic operations are implemented properly, with constant/linear complexity. Pathfinding algorithms are also well-implemented, with Dijkstra complexity equal nodes*ln(nodes). Below I've attached results of the performance test:

![bellman](https://user-images.githubusercontent.com/41695668/170328332-fa909703-aaa1-4d0f-a250-e31807a516e8.png)
![dijkstra](https://user-images.githubusercontent.com/41695668/170328336-b6d431b6-7957-46ab-90af-294691d01677.png)

Test were conducted for fixed amount of edges per node (100), minimal weight of edge (50) and maximal weight (500), repeated for 10 different, randomly generated graphs. Standard deviation for the Dijkstra algorithm was negligible and isn't included in the chart.

While time complexity of my implementation is good, space complexity is not. Here are some graph sizes I've tested:

    20000 nodes, density 0.1 ⇾ 9.6 GB of RAM occupied
    10000 nodes, density 0.2 ⇾ 5.1 GB of RAM occupied
    5000 nodes, density 0.5 ⇾ 3.3 GB of RAM occupied
    1000 nodes, density 1.0 ⇾ 250 MB of RAM occupied

This is because I'm using hashtables, not only to store nodes in the graph, but also to store edges for each node. Note that for 20k nodes and density 0.1, it still means there are 2000 edges per node, totaling to 4 M edges.

Within documentation, **all time complexity represents average case** 