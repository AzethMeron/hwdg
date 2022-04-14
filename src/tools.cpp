
#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include "tools.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "graph.hpp"

namespace Graphs
{
	int RandInt(const int & min, const int & max) {
		static thread_local std::random_device rd;
		static thread_local std::mt19937 generator(rd());
		std::uniform_int_distribution<int> distribution(min,max);
		return distribution(generator);
	}
	
	double RandDouble(const double & min, const double & max) {
		static thread_local std::random_device rd;
		static thread_local std::mt19937 generator(rd());
		std::uniform_real_distribution<double> distribution(min,max);
		return distribution(generator);
	}
	
	Graph RandGraph(unsigned int size, float density, float weight_min, float weight_max, bool loops)
	{
		Graph output;
		for(unsigned int i = 0; i < size; ++i)
		{
			output.add(Node(i));
		}
		for(const Node& a : output)
		{
			for(const Node& b : output)
			{
				if(!(a == b) || loops)
				{
					double chance = RandDouble(0,1);
					if(chance <= density)
					{
						float weight = (float)RandDouble(weight_min, weight_max);
						output.add(Edge(a,b,weight));
					}
				}
			}
		}
		return output;
	}

	Graph LoadGraph(const std::string& filename)
	{
		std::ifstream file(filename);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		Graph graph;
		size_t size_nodes;
		file >> size_nodes;
		for (size_t i = 0; i < size_nodes; ++i)
		{
			uint32_t node_id;
			file >> node_id;
			graph.add(Node(node_id));
			size_t size_edges;
			file >> size_edges;
			for (size_t j = 0; j < size_edges; ++j)
			{
				uint32_t tgt;
				float weight;
				file >> tgt >> weight;
				graph.add(Edge(Node(node_id), tgt, weight));
			}
		}
		return graph;
	}

	void SaveGraph(const Graph& graph, const std::string& filename)
	{
		std::ofstream file(filename);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		file << graph.size_nodes() << " ";
		for (const NodeInGraph& node : graph)
		{
			file << node.id() << " " << node.size_edges() << " ";
			for (const Edge& edge : node)
			{
				file << edge.target().id() << " " << edge.weight() << " ";
			}
		}
	}
}
