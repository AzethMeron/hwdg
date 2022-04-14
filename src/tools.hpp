#ifndef GRAPHS_TOOLS_HPP
#define GRAPHS_TOOLS_HPP

#include <memory>
#include <string>
#include <stdexcept>
#include "graph.hpp"

namespace Graphs
{
	// Source: https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
	// licensed under CC0 1.0
	// %s - cannot be std::string!
	template<typename ... Args>
	std::string string_format( const std::string& format, Args ... args )
	{
		int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
		if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
		auto size = static_cast<size_t>( size_s );
		std::unique_ptr<char[]> buf( new char[ size ] );
		std::snprintf( buf.get(), size, format.c_str(), args ... );
		return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
	}
	
	// size - number of nodes
	// density - value from 0 to 1, describes density of graph. Result graph won't be precisely of this value
	// weight_min, weight_max - weights for edges
	// loops - are loops allowed?
	Graph RandGraph(unsigned int size, float density, float weight_min, float weight_max, bool loops); // O(n^2)
	double RandDouble(const double& min, const double& max);
	int RandInt(const int& min, const int& max);

	// Temporary (?) functions for tests
	Graph LoadGraph(const std::string& filename);
	void SaveGraph(const Graph& graph, const std::string& filename);
}

#endif