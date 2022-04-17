
#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "tools.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "graph.hpp"

namespace HWDG
{
	constexpr char HEADER_BIN[] = "HDWGBIN";
	constexpr char HEADER_BIN_REV[] = "NIBGWDH";
	constexpr char HEADER_SAVE[] = "HDWGSAV";
	constexpr char HEADER_SAVE_REV[] = "VASGWDH";

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

	Graph LoadGraphTxtFile(const std::string& filename)
	{
		std::ifstream file(filename);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		// header
		std::string header;
		file >> header;
		if(header != std::string(HEADER_SAVE)) throw std::ios_base::failure(string_format("This isn't file with graph (plain form): %s", filename.c_str()));
		// data
		Graph graph = Graph::LoadTxt(file);
		// endmark
		file >> header;
		if (header != std::string(HEADER_SAVE_REV)) throw std::ios_base::failure(string_format("Malformed data (no endmark) in file: %s", filename.c_str()));
		return graph;
	}

	void SaveGraphTxtFile(const Graph& graph, const std::string& filename)
	{
		std::ofstream file(filename);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		file << HEADER_SAVE << "\n";
		Graph::SaveTxt(file, graph);
		file << HEADER_SAVE_REV << "\n";
		if (file.good() == false) throw std::ios_base::failure(string_format("Error happened during saving of a graph to file: %s", filename.c_str()));
	}

	Graph LoadGraphBinFile(const std::string& filename)
	{
		std::ifstream file;
		file.open(filename, std::ios::in | std::ios::binary);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		// header
		char header[sizeof(HEADER_BIN)] = "";
		file.read(header, sizeof(HEADER_BIN));
		if(std::string(header) != std::string(HEADER_BIN)) throw std::ios_base::failure(string_format("This isn't file with graph (binary form): %s", filename.c_str()));
		// data
		Graph graph = Graph::LoadBin(file);
		// endmark
		char endmark[sizeof(HEADER_BIN_REV)] = "";
		file.read(endmark, sizeof(HEADER_BIN_REV));
		if(std::string(endmark) != std::string(HEADER_BIN_REV)) throw std::ios_base::failure(string_format("Malformed data (no endmark) in file: %s", filename.c_str()));
		return graph;
	}

	void SaveGraphBinFile(const Graph& graph, const std::string& filename)
	{
		std::ofstream file;
		file.open( filename, std::ios::out | std::ios::binary);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		// header
		file.write(HEADER_BIN, sizeof(HEADER_BIN));
		// data
		Graph::SaveBin(file, graph);
		// endmark
		file.write(HEADER_BIN_REV, sizeof(HEADER_BIN_REV));
		if (file.good() == false) throw std::ios_base::failure(string_format("Error happened during saving of a graph to file: %s", filename.c_str()));
	}
}
