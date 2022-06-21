
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <stack>
#include <functional>
#include <vector>
#include <algorithm>

#include "operations.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "node_in_graph.hpp"
#include "graph.hpp"
#include "custom_map.hpp"

namespace HWDG
{
	Graph Operations::Union(const Graph& a, const Graph& b)
	{
		Graph output = a;
		Operations::MergeUnion(output, b);
		return output;
	}
	
	// balancer = 0..1
	Graph Operations::Union(const Graph& a, const Graph& b, float balancer)
	{
		Graph output = a;
		Operations::MergeUnion(output, b, balancer);
		return output;
	}

	void Operations::MergeUnion(Graph& a, const Graph& b)
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
	void Operations::MergeUnion(Graph& a, const Graph& b, float balancer)
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

	Graph Operations::Intersection(const Graph& a, const Graph& b, float balancer)
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

	Graph Operations::InverseIntersection(const Graph& a, const Graph& b)
	{
		Graph output;
		for (auto& node : a.nodes()) { if (!b.has(node)) { output.add((const Node&)node); } }
		for (auto& node : b.nodes()) { if (!a.has(node)) { output.add((const Node&)node); } }
		for (auto& edge : a.edges()) { if (!b.has(edge)) { output.add(edge); } }
		for (auto& edge : b.edges()) { if (!a.has(edge)) { output.add(edge); } }
		return output;
	}

	Graph Operations::Difference(const Graph& a, const Graph& b)
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

	double Operations::SizeSimilarity(const Graph& a, const Graph& b)
	{
		double size_a = a.size_edges();
		double size_b = b.size_edges();
		return std::min(size_a, size_b) / std::max(size_a, size_b);
	}

	double Operations::ContainmentSimilarity(const Graph& a, const Graph& b)
	{
		double common_edges = Operations::Intersection(a,b,0.5).size_edges();
		return common_edges / std::min(a.size_edges(), b.size_edges());
	}
	
	double Operations::ValueSimilarity(const Graph& a, const Graph& b)
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
	
	double Operations::NormalizedValueSimilarity(const Graph& a, const Graph& b)
	{
		return ValueSimilarity(a,b) / SizeSimilarity(a,b);
	}

	void Operations::BreadthFirstSearch(const Graph& graph, const Node& starting_node, std::function<void(const Edge& edge, const std::unordered_set<Node, Node::HashFunction>& visited)> func)
	{
		Operations::BreadthFirstSearch(graph, starting_node, func, [](const NodeInGraph& node, std::vector<Edge>& to_be_visited_first, const std::unordered_set<Node, Node::HashFunction>& visited) {
			// Default behaviour: undefined order for edges (neighbours) on given depth
			for (const Edge& edge : node)
			{
				to_be_visited_first.push_back(edge);
			}
			});
	}

	void Operations::BreadthFirstSearch(const Graph& graph, const Node& starting_node,
		std::function<void(const Edge& edge, const std::unordered_set<Node, Node::HashFunction>& visited)> func, 
		std::function<void(const NodeInGraph& node, std::vector<Edge>& to_be_traversed_first, const std::unordered_set<Node, Node::HashFunction>& visited)> priority)
	{
		std::unordered_set<Node, Node::HashFunction> visited; visited.reserve(graph.size_nodes());
		std::queue<NodeInGraph> next;
		if (!graph.has(starting_node)) { throw std::invalid_argument("BreadthFirstSearch: starting node isn't part of given graph"); }
		next.push(graph.fetch(starting_node)); visited.insert(starting_node);
		while (next.size())
		{
			NodeInGraph current = next.front(); next.pop();

			std::vector<Edge> res;
			priority(current, res, visited);

			for (const auto& edge : res)
			{
				auto iter = visited.find(edge.target());
				if (iter == visited.end())
				{
					next.push(graph.fetch(edge.target()));
					visited.insert(edge.target());
					func(edge, visited);
				}
			}
		}
	}

	void Operations::DepthFirstSearch(const Graph& graph, const Node& starting_node, std::function<void(const Edge& edge, const std::unordered_set<Node, Node::HashFunction>& visited)> func)
	{
		Operations::DepthFirstSearch(graph, starting_node, func, [](const NodeInGraph& node, std::vector<Edge>& to_be_visited_first, const std::unordered_set<Node, Node::HashFunction>& visited){
			// Default behaviour: undefined order for edges (neighbours) on given depth
			for (const Edge& edge : node)
			{
				to_be_visited_first.push_back(edge);
			}
			});
	}

	void Operations::DepthFirstSearch(const Graph& graph, const Node& starting_node,
		std::function<void(const Edge& edge, const std::unordered_set<Node, Node::HashFunction>& visited)> func, 
		std::function<void(const NodeInGraph& node, std::vector<Edge>& to_be_traversed_first, const std::unordered_set<Node, Node::HashFunction>& visited)> priority)
	{
		std::unordered_set<Node, Node::HashFunction> visited; visited.reserve(graph.size_nodes());
		std::stack<Edge> next;
		if (!graph.has(starting_node)) { throw std::invalid_argument("DepthFirstSearch: starting node isn't part of given graph"); }
		visited.insert(starting_node);
		for (const Edge& edge : graph.fetch(starting_node)) { next.push(edge); }
		while (next.size())
		{
			Edge current = next.top(); next.pop();
			Node target = current.target();
			// Check if target node was visited, since stack allows duplicates. Skip if that's the case
			auto iter = visited.find(target);
			if (iter != visited.end()) continue;
			// Mark as visited
			visited.insert(target);
			// Function call
			func(current, visited);

			std::vector<Edge> res;
			priority(graph.fetch(current.target()), res, visited);
			std::reverse(res.begin(), res.end());
			for (const Edge& edge : res)
			{
				next.push(edge);
			}
		}
	}
}
