#ifndef HWDG_OPERATIONS_HPP
#define HWDG_OPERATIONS_HPP

#include "graph.hpp"

namespace HWDG
{
	Graph Union(const Graph& a, const Graph& b); // linear
	Graph Union(const Graph& a, const Graph& b, float balancer); // linear
	Graph Intersection(const Graph& a, const Graph& b, float balancer); // linear
	Graph Difference(const Graph& a, const Graph& b); // linear
	double ContainmentSimilarity(const Graph& a, const Graph& b); // linear
	double SizeSimilarity(const Graph& a, const Graph& b); // constant
	double ValueSimilarity(const Graph& a, const Graph& b); // linear
	double NormalizedValueSimilarity(const Graph& a, const Graph& b); // linear
}

#endif