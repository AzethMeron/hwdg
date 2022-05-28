
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <stack>
#include <functional>

#include "operations.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "node_in_graph.hpp"
#include "graph.hpp"
#include "custom_map.hpp"

namespace HWDG
{
	Graph Union(const Graph& a, const Graph& b)
	{
		Graph output = a;
		MergeUnion(output, b);
		return output;
	}
	
	// balancer = 0..1
	Graph Union(const Graph& a, const Graph& b, float balancer)
	{
		Graph output = a;
		MergeUnion(output, b, balancer);
		return output;
	}

	void MergeUnion(Graph& a, const Graph& b)
	{
		// adding nodes
		for (auto& node : b)
		{
			a.add((const Node&)node);
		}
		// adding edges
		for (auto& edge : b.edges())
		{
			if (a.has(edge))
			{
				float weight_from_a = a.fetch(edge).weight();
				float weight_from_b = edge.weight();
				a.remove(edge);
				a.add(Edge(edge.source(), edge.target(), weight_from_a + weight_from_b));
			}
			else
			{
				a.add(edge);
			}
		}
	}
	void MergeUnion(Graph& a, const Graph& b, float balancer)
	{
		// adding nodes from b
		for (auto& node : b)
		{
			a.add((const Node&)node);
		}
		// adding edges
		for (auto& edge : b.edges())
		{
			if (a.has(edge))
			{
				float weight_from_a = balancer * a.fetch(edge).weight();
				float weight_from_b = ((float)1.0 - balancer) * edge.weight();
				a.remove(edge);
				a.add(Edge(edge.source(), edge.target(), weight_from_a + weight_from_b));
			}
			else
			{
				a.add(edge);
			}
		}
	}

	Graph Intersection(const Graph& a, const Graph& b, float balancer)
	{
		Graph output;
		for(auto& node : a.nodes())
		{
			if(b.has(node))
			{
				output.add((const Node&)node);
			}
		}
		for(auto& edge : a.edges())
		{
			if(b.has(edge))
			{
				float weight_from_a = balancer * edge.weight();
				float weight_from_b = ((float)1.0 - balancer) * b.fetch(edge).weight();
				output.add(Edge(edge.source(), edge.target(), weight_from_a + weight_from_b));
			}
		}
		return output;
	}

	Graph Difference(const Graph& a, const Graph& b)
	{
		Graph output;
		for(auto& node : a.nodes())
		{
			if(!b.has(node))
			{
				output.add((const Node&)node);
			}
		}
		for(auto& edge : a.edges())
		{
			if(!b.has(edge))
			{
				output.add(edge);
			}
		}
		return output;
	}

	double SizeSimilarity(const Graph& a, const Graph& b)
	{
		double size_a = a.size_edges();
		double size_b = b.size_edges();
		return std::min(size_a, size_b) / std::max(size_a, size_b);
	}

	double ContainmentSimilarity(const Graph& a, const Graph& b)
	{
		double common_edges = Intersection(a,b,0.5).size_edges();
		return common_edges / std::min(a.size_edges(), b.size_edges());
	}
	
	double ValueSimilarity(const Graph& a, const Graph& b)
	{
		double output = 0;
		for(auto& edge : a.edges())
		{
			if(b.has(edge))
			{
				double tmp_nom = std::min(edge.weight(), b.fetch(edge).weight());
				double tmp_denom = std::max(edge.weight(), b.fetch(edge).weight());
				output = output + tmp_nom / tmp_denom;
			}
		}
		return output / std::max(a.size_edges(), b.size_edges());
	}
	
	double NormalizedValueSimilarity(const Graph& a, const Graph& b)
	{
		return ValueSimilarity(a,b) / SizeSimilarity(a,b);
	}

	void BreadthFirstSearch(const Graph& graph, const Node& starting_node, std::function<void(const Edge&)> func)
	{
		std::unordered_set<Node, Node::HashFunction> visited;
		std::queue<NodeInGraph> next;
		if (!graph.has(starting_node)) { throw std::invalid_argument("BreadthFirstSearch: starting node isn't part of given graph"); }
		next.push(graph.fetch(starting_node)); visited.insert(starting_node);
		while (next.size())
		{
			NodeInGraph current = next.front(); next.pop();
			for (const auto& edge : current)
			{
				auto iter = visited.find(edge.target());
				if (iter == visited.end())
				{
					next.push(graph.fetch(edge.target()));
					visited.insert(edge.target());
					func(edge);
				}
			}
		}
	}

	// This implementation is WRONG
	void DepthFirstSearch(const Graph& graph, const Node& starting_node, std::function<void(const Edge&)> func)
	{
		std::unordered_set<Node, Node::HashFunction> visited; visited.insert(starting_node);
		std::stack<Edge> next;
		if (!graph.has(starting_node)) { throw std::invalid_argument("DepthFirstSearch: starting node isn't part of given graph"); }
		for (const Edge& edge : graph.fetch(starting_node)) { next.push(edge); visited.insert(edge.target()); }
		while (next.size())
		{
			Edge edge = next.top(); next.pop();
			func(edge);
			for (const Edge& new_edge : graph.fetch(edge.target())) 
			{ 
				auto iter = visited.find(new_edge.target());
				if(iter == visited.end())
				{ next.push(new_edge); visited.insert(new_edge.target()); }
			}
		}
	}
}
