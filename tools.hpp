#ifndef HWDG_TOOLS_HPP
#define HWDG_TOOLS_HPP

/**
* @file tools.hpp
* @author Jakub Grzana
* @date March 2022
* @brief Utilities and serialisation
*
* 
*/

#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <fstream>
#include "graph.hpp"

namespace HWDG
{
	#ifndef DOXYGEN_SHOULD_SKIP_THIS
		constexpr char HEADER_BIN[] = "HDWGBIN";
		constexpr char HEADER_BIN_REV[] = "NIBGWDH";
		constexpr char HEADER_SAVE[] = "HDWGSAV";
		constexpr char HEADER_SAVE_REV[] = "VASGWDH";

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
	* \par Time complexity: 
	* \f$O(nodes^2)\f$
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
	* \par Time complexity: 
	* \f$O(nodes^2 * density)\f$ for small density, otherwise \f$O(nodes^3)\f$
	*/
	Graph RandomLowDensityGraph(size_t size, float density, float weight_min, float weight_max, bool loops); // O(n^2*density) for very small density, for dense it is O(n^3) 

	/** Save object to file in binary mode. 
	* 
	* \tparam TYPE Type of data you want to save. Must implement static member function TYPE::SaveBin()
	* \param obj Object to be saved.
	* \param filename Name of file to which you want to save object to.
	* \throws std::ios_base::failure Writing to file error.
	* \par Time complexity: 
	* \f$O(n)\f$
	*/
	template<typename TYPE>
	void SaveBin(const TYPE& obj, const std::string& filename)
	{
		std::ofstream file;
		file.open(filename, std::ios::out | std::ios::binary);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		file.write(HEADER_BIN, sizeof(HEADER_BIN));
		TYPE::SaveBin(file, obj);
		file.write(HEADER_BIN_REV, sizeof(HEADER_BIN_REV));
		if (file.good() == false) throw std::ios_base::failure(string_format("Error happened during saving of a graph to file: %s", filename.c_str()));
	}

	/** Load object from file in binary mode.
	*
	* \tparam TYPE Type of data you want to save. Must implement static member function TYPE::LoadBin()
	* \param filename Name of file from which data will be loaded
	* \throws std::ios_base::failure Reading error
	* \par Time complexity: 
	* \f$O(n)\f$
	*/
	template<typename TYPE>
	TYPE LoadBin(const std::string& filename)
	{
		std::ifstream file;
		file.open(filename, std::ios::in | std::ios::binary);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		// header
		char header[sizeof(HEADER_BIN)] = "";
		file.read(header, sizeof(HEADER_BIN));
		if (std::string(header) != std::string(HEADER_BIN)) throw std::ios_base::failure(string_format("This isn't file with graph (binary form): %s", filename.c_str()));
		// data
		TYPE output = TYPE::LoadBin(file);
		// endmark
		char endmark[sizeof(HEADER_BIN_REV)] = "";
		file.read(endmark, sizeof(HEADER_BIN_REV));
		if (std::string(endmark) != std::string(HEADER_BIN_REV)) throw std::ios_base::failure(string_format("Malformed data (no endmark) in file: %s", filename.c_str()));
		return output;
	}

	/** Save object to file in text mode.
	*
	* \tparam TYPE Type of data you want to save. Must implement static member function TYPE::SaveTxt()
	* \param obj Object to be saved.
	* \param filename Name of file to which you want to save object to.
	* \throws std::ios_base::failure Writing to file error.
	* \par Time complexity: 
	* \f$O(n)\f$
	*/
	template<typename TYPE>
	void SaveTxt(const TYPE& obj, const std::string& filename)
	{
		std::ofstream file(filename);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		file << HEADER_SAVE << "\n";
		TYPE::SaveTxt(file, obj);
		file << HEADER_SAVE_REV << "\n";
		if (file.good() == false) throw std::ios_base::failure(string_format("Error happened during saving of a graph to file: %s", filename.c_str()));
	}

	/** Load object from file in text mode.
	*
	* \tparam TYPE Type of data you want to save. Must implement static member function TYPE::LoadTxt()
	* \param filename Name of file from which data will be loaded
	* \throws std::ios_base::failure Reading error
	* \par Time complexity: 
	* \f$O(n)\f$
	*/
	template<typename TYPE>
	TYPE LoadTxt(const std::string& filename)
	{
		std::ifstream file(filename);
		if (file.good() == false) throw std::ios_base::failure(string_format("Failed to open file: %s", filename.c_str()));
		// header
		std::string header;
		file >> header;
		if (header != std::string(HEADER_SAVE)) throw std::ios_base::failure(string_format("This isn't file with graph (plain form): %s", filename.c_str()));
		// data
		TYPE output = TYPE::LoadTxt(file);
		// endmark
		file >> header;
		if (header != std::string(HEADER_SAVE_REV)) throw std::ios_base::failure(string_format("Malformed data (no endmark) in file: %s", filename.c_str()));
		return output;
	}
}

#endif