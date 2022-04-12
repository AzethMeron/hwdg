
#include <random>
#include <vector>
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
}
