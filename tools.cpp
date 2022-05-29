
#include <random>
#include <vector>
#include <algorithm>
#include "tools.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "graph.hpp"

namespace HWDG
{
	#ifndef DOXYGEN_SHOULD_SKIP_THIS
		int RandomInt(const int& min, const int& max) {
			static thread_local std::random_device rd;
			static thread_local std::mt19937 generator(rd());
			std::uniform_int_distribution<int> distribution(min, max);
			return distribution(generator);
		}

		double RandomDouble(const double& min, const double& max) {
			static thread_local std::random_device rd;
			static thread_local std::mt19937 generator(rd());
			std::uniform_real_distribution<double> distribution(min, max);
			return distribution(generator);
		}
	#endif

	Graph RandomGraph(size_t size, float density, float weight_min, float weight_max, bool loops)
	{
		Graph output;
		output.reserve_nodes(size);
		output.reserve_edges(size * size * density);
		for (size_t i = 0; i < size; ++i)
		{
			Node n = Node(i);
			output.add(n);
		}
		for (const Node& a : output)
		{
			for (const Node& b : output)
			{
				if (!(a == b) || loops)
				{
					double chance = RandomDouble(0, 1);
					if (chance <= density)
					{
						float weight = (float)RandomDouble(weight_min, weight_max);
						output.add(Edge(a, b, weight));
					}
				}
			}
		}
		return output;
	}

	Graph RandomLowDensityGraph(size_t size, float density, float weight_min, float weight_max, bool loops)
	{
		// Manage density, because contrary to previous algorithm, it matters here
		if (density > 1.0) density = 1.0;
		if (density < 0) density = 0;
		// Algorithm
		Graph output;
		output.reserve_nodes(size);
		output.reserve_edges(size * size * density);
		for (size_t i = 0; i < size; ++i)
		{
			Node n = Node(i);
			output.add(n);
		}
		for (const Node& a : output)
		{
			size_t edges_per_node = size * density;
			output.reserve_edges_in_node(a, edges_per_node);
			for (size_t i = 0; i < edges_per_node; ++i)
			{
				for(size_t j = 0; j < size; ++j) // if the edge isn't generate in "size" tries, it is most likely impossible to create
				{
					Node b = Node(RandomInt(0, size - 1));
					float weight = (float)RandomDouble(weight_min, weight_max);
					Edge potential_edge = Edge(a, b, weight);
					// conditions
					if (output.has(potential_edge)) continue; // if exists, try again
					if (!loops && a == b) continue; // if it is loop and loops aren't enabled, try again
					// Success, go to next edge
					output.add(potential_edge);
					break;
				}
			}
		}
		return output;
	}
}
