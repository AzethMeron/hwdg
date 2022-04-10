#pragma once
#include "graph.hpp"

namespace Graphs
{
	Graph Union(const Graph& a, const Graph& b); // linear
	Graph Union(const Graph& a, const Graph& b, float balancer); // linear
	Graph Intersection(const Graph& a, const Graph& b, float balancer); // linear
	Graph Difference(const Graph& a, const Graph& b); // linear
	float ContainmentSimilarity(const Graph& a, const Graph& b); // linear
	float SizeSimilarity(const Graph& a, const Graph& b); // constant
	float ValueSimilarity(const Graph& a, const Graph& b); // linear
	float NormalizedValueSimilarity(const Graph& a, const Graph& b); // linear
}
