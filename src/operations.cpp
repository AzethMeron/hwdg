
#include <algorithm>
#include "operations.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "node_in_graph.hpp"
#include "graph.hpp"

namespace Graphs
{
	Graph Union(const Graph& a, const Graph& b)
	{
		Graph output = a;
		// adding nodes from b
		for(auto pair : b)
		{
			output.add((const Node&)pair.second);
		}
		// adding edges
		for(auto pair : b.edges())
		{
			if(a.has(pair.second))
			{
				float weight_from_a = a.fetch(pair.second).weight();
				float weight_from_b = pair.second.weight();
				output.remove(pair.second);
				output.add(Edge(pair.second.source(), pair.second.target(), weight_from_a + weight_from_b));
			}
			else
			{
				output.add(pair.second);
			}
		}
		return output;
	}
	
	// balancer = 0..1
	Graph Union(const Graph& a, const Graph& b, float balancer)
	{
		Graph output = a;
		// adding nodes from b
		for(auto pair : b)
		{
			output.add((const Node&)pair.second);
		}
		// adding edges
		for(auto pair : b.edges())
		{
			if(a.has(pair.second))
			{
				float weight_from_a = balancer * a.fetch(pair.second).weight();
				float weight_from_b = (1.0 - balancer) * pair.second.weight();
				output.remove(pair.second);
				output.add(Edge(pair.second.source(), pair.second.target(), weight_from_a + weight_from_b));
			}
			else
			{
				output.add(pair.second);
			}
		}
		return output;
	}

	Graph Intersection(const Graph& a, const Graph& b, float balancer)
	{
		Graph output;
		for(auto pair : a.nodes())
		{
			if(b.has(pair.second))
			{
				output.add((const Node&)pair.second);
			}
		}
		for(auto pair : a.edges())
		{
			if(b.has(pair.second))
			{
				float weight_from_a = balancer * pair.second.weight();
				float weight_from_b = (1.0 - balancer) * b.fetch(pair.second).weight();
				output.add(Edge(pair.second.source(), pair.second.target(), weight_from_a + weight_from_b));
			}
		}
		return output;
	}

	Graph Difference(const Graph& a, const Graph& b)
	{
		Graph output;
		for(auto pair : a.nodes())
		{
			if(!b.has(pair.second))
			{
				output.add((const Node&)pair.second);
			}
		}
		for(auto pair : a.edges())
		{
			if(!b.has(pair.second))
			{
				output.add(pair.second);
			}
		}
		return output;
	}

	float SizeSimilarity(const Graph& a, const Graph& b)
	{
		float size_a = a.size_edges();
		float size_b = b.size_edges();
		return std::min(size_a, size_b) / std::max(size_a, size_b);
	}

	float ContainmentSimilarity(const Graph& a, const Graph& b)
	{
		float common_edges = Intersection(a,b,0.5).size_edges();
		return common_edges / std::min(a.size_edges(), b.size_edges());
	}
	
	float ValueSimilarity(const Graph& a, const Graph& b)
	{
		float output = 0;
		for(auto pair : a.edges())
		{
			if(b.has(pair.second))
			{
				float tmp_nom = std::min(pair.second.weight(), b.fetch(pair.second).weight());
				float tmp_denom = std::max(pair.second.weight(), b.fetch(pair.second).weight());
				output = output + tmp_nom / tmp_denom;
			}
		}
		return output / std::max(a.size_edges(), b.size_edges());
	}
	
	float NormalizedValueSimilarity(const Graph& a, const Graph& b)
	{
		return ValueSimilarity(a,b) / SizeSimilarity(a,b);
	}
}
