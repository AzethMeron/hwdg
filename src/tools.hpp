#ifndef HWDG_TOOLS_HPP
#define HWDG_TOOLS_HPP

#include <memory>
#include <string>
#include <stdexcept>
#include "graph.hpp"

namespace HWDG
{
	#ifndef DOXYGEN_SHOULD_SKIP_THIS
		double RandomDouble(const double& min, const double& max);
		int RandomInt(const int& min, const int& max);
		
		// Source: https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
		// licensed under CC0 1.0
		// %s - cannot be std::string!
		template<typename ... Args>
		std::string string_format(const std::string& format, Args ... args)
		{
			int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
			if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
			auto size = static_cast<size_t>(size_s);
			std::unique_ptr<char[]> buf(new char[size]);
			std::snprintf(buf.get(), size, format.c_str(), args ...);
			return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
		}
	#endif
	
	/**
	* Generates random graph. Useful during development, for tests. Value of density in output Graph is roughly equal to the one that was used in function call, but remember it ISN'T guaranteed.
	* \param size Number of nodes.
	* \param density Density of graph. Must belong to [0,1]
	* \param weight_min Minimal weight of edge in graph.
	* \param weight_max Maximal weight of edge in graph.
	* \param loops Are loops allowed?
	* \return Randomly generated Graph.
	* \par Time complexity: O(nodes * nodes)
	*/
	Graph RandomGraph(size_t size, float density, float weight_min, float weight_max, bool loops); // O(n^2)
	/**
	* Generates random graph. Useful during development, for tests. Value of density in output Graph is roughly equal to the one that was used in function call, but remember it ISN'T guaranteed.
	* 
	* This implementation is much faster than RandomGraph() for small density, but much slower for high.
	* \param size Number of nodes.
	* \param density Density of graph. Must belong to [0,1]
	* \param weight_min Minimal weight of edge in graph.
	* \param weight_max Maximal weight of edge in graph.
	* \param loops Are loops allowed?
	* \return Randomly generated Graph.
	* \par Time complexity: O(nodes * nodes * density) for small density.
	*/
	Graph RandomLowDensityGraph(size_t size, float density, float weight_min, float weight_max, bool loops); // O(n^2*density) for very small density, for dense it is O(n^3) 

	// Serialisation. Allows to store graph in files. 
	// Binary form is actual serialisation that can be used to pickle later. Fast and compact, but platform dependant and unreadable for humans
	// Plain text save/load is good for manual creation of graphs. Slow and takes more space, but platform independant and human readable
	Graph LoadGraphTxtFile(const std::string& filename);
	void SaveGraphTxtFile(const Graph& graph, const std::string& filename);
	Graph LoadGraphBinFile(const std::string& filename);
	void SaveGraphBinFile(const Graph& graph, const std::string& filename);
}

#endif