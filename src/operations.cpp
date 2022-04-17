
#include <algorithm>
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
		// adding nodes from b
		for(auto& node : b)
		{
			output.add((const Node&)node);
		}
		// adding edges
		for(auto& edge : b.edges())
		{
			if(a.has(edge))
			{
				float weight_from_a = a.fetch(edge).weight();
				float weight_from_b = edge.weight();
				output.remove(edge);
				output.add(Edge(edge.source(), edge.target(), weight_from_a + weight_from_b));
			}
			else
			{
				output.add(edge);
			}
		}
		return output;
	}
	
	// balancer = 0..1
	Graph Union(const Graph& a, const Graph& b, float balancer)
	{
		Graph output = a;
		// adding nodes from b
		for(auto& node : b)
		{
			output.add((const Node&)node);
		}
		// adding edges
		for(auto& edge : b.edges())
		{
			if(a.has(edge))
			{
				float weight_from_a = balancer * a.fetch(edge).weight();
				float weight_from_b = ((float)1.0 - balancer) * edge.weight();
				output.remove(edge);
				output.add(Edge(edge.source(), edge.target(), weight_from_a + weight_from_b));
			}
			else
			{
				output.add(edge);
			}
		}
		return output;
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
}
