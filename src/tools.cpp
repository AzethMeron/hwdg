
#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "tools.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "graph.hpp"

namespace Graphs
{
	constexpr char HEADER_BIN[] = "HDWGBIN";
	constexpr char HEADER_BIN_REV[] = "NIBGWDH";
	constexpr char HEADER_SAVE[] = "HDWGSAV";
	constexpr char HEADER_SAVE_REV[] = "VASGWDH";

	int RandInt(const int& min, const int& max) {
		static thread_local std::random_device rd;
		static thread_local std::mt19937 generator(rd());
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(generator);
	}

	double RandDouble(const double& min, const double& max) {
		static thread_local std::random_device rd;
		static thread_local std::mt19937 generator(rd());
		std::uniform_real_distribution<double> distribution(min, max);
		return distribution(generator);
	}

	Graph RandGraph(size_t size, float density, float weight_min, float weight_max, bool loops)
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
					double chance = RandDouble(0, 1);
					if (chance <= density)
					{
						float weight = (float)RandDouble(weight_min, weight_max);
						output.add(Edge(a, b, weight));
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
		// header
		std::string header;
		file >> header;
		if(header != std::string(HEADER_SAVE)) throw std::ios_base::failure(string_format("This isn't file with graph (plain form): %s", filename.c_str()));
		// data
		Graph graph;
		size_t size_nodes;
		file >> size_nodes;
		graph.reserve_nodes(size_nodes);
		size_t size_edges;
		file >> size_edges;
		graph.reserve_edges(size_edges);
		for (size_t i = 0; i < size_nodes; ++i)
		{
			uint32_t node_id;
			file >> node_id;
			graph.add(Node(node_id));
			size_t size_edges;
			file >> size_edges;
			graph.reserve_edges_in_node(Node(node_id), size_edges);
			for (size_t j = 0; j < size_edges; ++j)
			{
				uint32_t tgt;
				float weight;
				file >> tgt >> weight;
				graph.add(Edge(Node(node_id), tgt, weight));
			}
			if (file.good() == false) throw std::ios_base::failure(string_format("Malformed data in file: %s", filename.c_str()));
		}
		// endmark
		file >> header;
		if (header != std::string(HEADER_SAVE_REV)) throw std::ios_base::failure(string_format("Malformed data (no endmark) in file: %s", filename.c_str()));
		return graph;
	}

	void SaveGraph(const Graph& graph, const std::string& filename)
	{
		std::ofstream file(filename);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		file << HEADER_SAVE << "\n";
		file << graph.size_nodes() << "\n";
		file << graph.size_edges() << "\n";
		for (const NodeInGraph& node : graph)
		{
			file << node.id() << " " << node.size_edges() << " ";
			for (const Edge& edge : node)
			{
				file << edge.target().id() << " " << edge.weight() << " ";
			}
			file << "\n";
		}
		
		file << HEADER_SAVE_REV << "\n";
		if (file.good() == false) throw std::ios_base::failure(string_format("Error happened during saving of a graph to file: %s", filename.c_str()));
	}

	Graph LoadGraphBin(const std::string& filename)
	{
		std::ifstream file;
		file.open(filename, std::ios::in | std::ios::binary);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		// header
		char header[sizeof(HEADER_BIN)] = "";
		file.read(header, sizeof(HEADER_BIN));
		if(std::string(header) != std::string(HEADER_BIN)) throw std::ios_base::failure(string_format("This isn't file with graph (binary form): %s", filename.c_str()));
		// data
		Graph graph;
		size_t size_nodes = 0;
		file.read((char*)&size_nodes, sizeof(size_nodes));
		graph.reserve_nodes(size_nodes);
		size_t size_edges = 0;
		file.read((char*)&size_edges, sizeof(size_edges));
		graph.reserve_edges(size_edges);
		for (size_t i = 0; i < size_nodes; ++i)
		{
			uint32_t node_id = 0;
			file.read((char*)&node_id, sizeof(node_id));
			graph.add(Node(node_id));
			size_t size_edges = 0;
			file.read((char*)&size_edges, sizeof(size_edges));
			graph.reserve_edges_in_node(Node(node_id), size_edges);
			for (size_t  j = 0; j < size_edges; ++j)
			{
				uint32_t tgt = 0;
				float weight = 0;
				file.read((char*)&tgt, sizeof(tgt));
				file.read((char*)&weight, sizeof(weight));
				graph.add(Edge(Node(node_id), Node(tgt), weight));
			}
			if (file.good() == false) throw std::ios_base::failure(string_format("Malformed data in file: %s", filename.c_str()));
		}
		// endmark
		char endmark[sizeof(HEADER_BIN_REV)] = "";
		file.read(endmark, sizeof(HEADER_BIN_REV));
		if(std::string(endmark) != std::string(HEADER_BIN_REV)) throw std::ios_base::failure(string_format("Malformed data (no endmark) in file: %s", filename.c_str()));
		return graph;
	}

	void SaveGraphBin(const Graph& graph, const std::string& filename)
	{
		std::ofstream file;
		file.open( filename, std::ios::out | std::ios::binary);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		// header
		file.write(HEADER_BIN, sizeof(HEADER_BIN));
		// data
		size_t size = graph.size_nodes();
		file.write((const char*)&size, sizeof(size));
		size_t size_edges = graph.size_edges();
		file.write((const char*)&size_edges, sizeof(size_edges));
		for (const NodeInGraph& node : graph)
		{
			uint32_t id = node.id();
			file.write((const char*)&id, sizeof(id));
			size_t edges = node.size_edges();
			file.write((const char*)&edges, sizeof(edges));
			for (const Edge& edge : node)
			{
				uint32_t tgt = edge.target().id();
				file.write((const char*)&tgt, sizeof(tgt));
				float weight = edge.weight();
				file.write((const char*)&weight, sizeof(weight));
			}
		}
		// endmark
		file.write(HEADER_BIN_REV, sizeof(HEADER_BIN_REV));
		if (file.good() == false) throw std::ios_base::failure(string_format("Error happened during saving of a graph to file: %s", filename.c_str()));
	}
}
