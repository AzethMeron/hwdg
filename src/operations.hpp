#pragma once
#include "graph.hpp"

Graph Union(const Graph& a, const Graph& b, float balancer);
Graph Intersection(const Graph& a, const Graph& b, float balancer);
Graph Difference(const Graph& a, const Graph& b);
float ContainmentSimilarity(const Graph& a, const Graph& b);
float SizeSimilarity(const Graph& a, const Graph& b);
