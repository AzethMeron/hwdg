#ifndef HWDG_TESTING_TOOLS_HPP
#define HWDG_TESTING_TOOLS_HPP

#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <fstream>

#include "graph.hpp"
#include "tools.hpp"

namespace HWDG
{
	//////////////////////////////////////////////////////////////////////////

	class Timer
	{
	public:
		double nanoseconds(void) const { return std::chrono::duration_cast<std::chrono::nanoseconds>(Stop - Start).count(); }
		double miliseconds(void) const { const double constant = 1e-6; return nanoseconds() * constant; }
	protected:
		std::chrono::high_resolution_clock::time_point Start;
		std::chrono::high_resolution_clock::time_point Stop;
	public:
		void start(void) { Start = std::chrono::high_resolution_clock::now(); }
		void stop(void) { Stop = std::chrono::high_resolution_clock::now(); }
	};

	//////////////////////////////////////////////////////////////////////////

	// requirements for TYPE, there're some
	// 		~TYPE();
	// 		TYPE();
	// 		TYPE(const TYPE&);
	// 		TYPE(const TYPE&&);
	// 		TYPE& operator = (const TYPE&);
	// 		TYPE& operator = (const int&); 
	// 		TYPE operator + (const TYPE&) const;
	// 		TYPE operator / (const TYPE&) const;
	// 		TYPE std::pow(TYPE, int);
	// 		TYPE std::sqrt(TYPE);
	// 		bool operator < (const TYPE&) const;
	// 		std::ostream& operator << (std::ostream&, const TYPE&);
	template<typename TYPE>
	class DataSeries
	{
	protected:
		std::vector<TYPE> dataseries;
		TYPE sum;
	public:
		// tools
		void Add(const TYPE& time);			// O(1)
		TYPE Average(void) const; 			// O(1)
		TYPE Variance(void) const;			// O(n)
		TYPE StandardDeviation(void) const;	// O(n)
		TYPE Median(void) const; 			// O(nlogn)
		DataSeries<TYPE> Sorted(void) const;// O(nlogn)
		DataSeries<TYPE> Reversed(void) const; // O(n)
		TYPE Min(void) const; 				// O(n)
		TYPE Max(void) const;				// O(n)
		double RelativeDeviation(void) const; 	// O(n), return value 0..1
		// getters 
		int Size(void) const;
		TYPE operator[] (const int& ind) const;
		std::vector<TYPE> Data(void) const;
		// Constructors
		DataSeries();
		// Conversion
		DataSeries(const std::vector<TYPE>& in);
		DataSeries<TYPE>& operator = (const std::vector<TYPE>& in);
		// Others
		DataSeries<TYPE> operator + (const DataSeries<TYPE>& r);
	};

	typedef DataSeries<double> TimeSeries;

	//////////////////////////////////////////////////////////////////////////

	template<typename TYPE>
	void DataSeries<TYPE>::Add(const TYPE& data)
	{
		this->dataseries.push_back(data);
		this->sum = this->sum + data;
	}

	template<typename TYPE>
	TYPE DataSeries<TYPE>::Average(void) const
	{
		TYPE size = this->Size();
		return (this->sum / size);
	}

	template<typename TYPE>
	TYPE DataSeries<TYPE>::Variance(void) const
	{
		TYPE average = this->Average();
		TYPE variance = 0;
		for (int i = 0; i < this->Size(); ++i)
		{
			variance = variance + pow((this->dataseries.at(i) - average), 2);
		}
		TYPE size = this->Size();
		variance = variance / size;
		return variance;
	}

	template<typename TYPE>
	TYPE DataSeries<TYPE>::StandardDeviation(void) const
	{
		return sqrt(this->Variance());
	}

	template<typename TYPE>
	TYPE DataSeries<TYPE>::Median(void) const
	{
		DataSeries<TYPE> sorted = this->Sorted();
		if (sorted.Size() == 0) throw - 1;
		if (sorted.Size() % 2)
		{
			int index = floor(sorted.Size() / 2);
			return sorted[index];
		}
		else
		{
			int index = ceil(sorted.Size() / 2);
			TYPE size = 2;
			return (sorted[index - 1] + sorted[index]) / size;
		}
	}

	template<typename TYPE>
	DataSeries<TYPE> DataSeries<TYPE>::Sorted(void) const
	{
		DataSeries<TYPE> sorted = *this;
		sort(sorted.dataseries.begin(), sorted.dataseries.end());
		return sorted;
	}

	template<typename TYPE>
	DataSeries<TYPE> DataSeries<TYPE>::Reversed(void) const
	{
		DataSeries<TYPE> reversed = *this;
		std::reverse(reversed.dataseries.begin(), reversed.dataseries.end());
		return reversed;
	}

	template<typename TYPE>
	TYPE DataSeries<TYPE>::Min(void) const
	{
		if (this->Size() == 0) throw - 1;
		int ind = 0;
		for (int i = 1; i < this->Size(); ++i)
		{
			if ((*this)[i] < (*this)[ind])
			{
				ind = i;
			}
		}
		return (*this)[ind];
	}

	template<typename TYPE>
	TYPE DataSeries<TYPE>::Max(void) const
	{
		if (this->Size() == 0) throw - 1;
		int ind = 0;
		for (int i = 1; i < this->Size(); ++i)
		{
			if ((*this)[ind] < (*this)[i])
			{
				ind = i;
			}
		}
		return (*this)[ind];
	}

	template<typename TYPE>
	double DataSeries<TYPE>::RelativeDeviation(void) const
	{
		TYPE dev = this->StandardDeviation();
		return dev / this->Average();
	}

	template<typename TYPE>
	std::ostream& operator << (std::ostream& ostream, const DataSeries<TYPE>& series)
	{
		for (int i = 0; i < series.Size(); ++i)
		{
			ostream << series[i] << ' ';
		}
		return ostream;
	}

	template<typename TYPE>
	int DataSeries<TYPE>::Size(void) const
	{
		return this->dataseries.size();
	}

	template<typename TYPE>
	TYPE DataSeries<TYPE>::operator[] (const int& ind) const
	{
		return this->dataseries.at(ind);
	}

	template<typename TYPE>
	DataSeries<TYPE>::DataSeries() : dataseries()
	{
		this->sum = 0;
	}

	template<typename TYPE>
	std::vector<TYPE> DataSeries<TYPE>::Data(void) const
	{
		return this->dataseries;
	}

	template<typename TYPE>
	DataSeries<TYPE>::DataSeries(const std::vector<TYPE>& in)
	{
		*this = in;
	}

	template<typename TYPE>
	DataSeries<TYPE>& DataSeries<TYPE>::operator = (const std::vector<TYPE>& in)
	{
		this->sum = 0;
		this->dataseries = in;
		for (unsigned int i = 0; i < in.size(); ++i)
		{
			this->sum = this->sum + in[i];
		}
		return *this;
	}

	template<typename TYPE>
	DataSeries<TYPE> DataSeries<TYPE>::operator + (const DataSeries<TYPE>& r)
	{
		DataSeries<TYPE> out = *this;
		for (int i = 0; i < r.Size(); ++i)
		{
			out.Add(r[i]);
		}
		return out;
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename ALGORITHM>
	void FixedEdgesTest(std::string filename, std::ostream& out, std::string timename,
		int fixed_edges, float weight_min, float weight_max,
		int start, int base, int power,
		int limit,  // if you want to stop at some number of nodes, type it here
		int times) // how many graphs are generated and tested (resulting time is average of those
	{
		std::ofstream file;
		file.open(filename, std::ios::out | std::ios::binary);
		std::ofstream time(timename); // CSV file for time output
		time << "nodes, average, standard deviation" << std::endl;
		Timer Time;
		for (int j = 0; j < power; ++j)
		{
			for (int i = 1; i < base; ++i)
			{
				int size = i * start * pow(base, j);
				if (size > limit) break;
				float density = float(fixed_edges) / float(size);

				TimeSeries timeseries;
				for (int t = 0; t < times; ++t)
				{
					Graph graph = RandomLowDensityGraph(size, density, weight_min, weight_max, true);
					Time.start();
					ALGORITHM d(graph, Node(0));
					Time.stop();
					timeseries.Add(Time.miliseconds());
				}

				auto standarddeviation = timeseries.StandardDeviation();
				out << "Size: " << size << ", Average: " << timeseries.Average() << " ms, Standard Deviation: " << standarddeviation << std::endl;
				time << size << ',' << timeseries.Average() << ',' << standarddeviation << std::endl;
			}
		}
		file.close();
		time.close();
	}
}

#endif
