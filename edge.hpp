#ifndef HWDG_EDGE_HPP
#define HWDG_EDGE_HPP

/**
* @file edge.hpp
* @author Jakub Grzana
* @date March 2022
* @brief Representation of Edge
*
* This file contains Edge class.
*/

#include <iostream>
#include <cstdint>
#include <string>
#include <limits>
#include "node.hpp"

namespace HWDG
{
	/**
	* Edge pointing from one Node to another.
	*/
	class Edge
	{
		private:
			Node _src_node;
			Node _tgt_node;
			float _weight;
			static uint64_t calculate_id(const Node& src, const Node& tgt, float weight);
		public:
			/**
			* Default weight, used when weight of Edge wasn't provided..
			*/
			static constexpr float DUMMY_WEIGHT = 1;

			/**
			* Maximal weight that can be assigned to Edge
			*/
			static constexpr float MAX_WEIGHT = std::numeric_limits<float>::max();

			/**
			* Minimal weight that can be assigned to Edge
			*/
			static constexpr float MIN_WEIGHT = std::numeric_limits<float>::min();
		public:
			/**
			* Get weight of Edge.
			* \return Weight of Edge.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			float weight(void) const;

			/**
			* Get source Node of Edge.
			* \return Node from which this Edge comes out.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			const Node& source(void) const;

			/**
			* Get target Node of Edge.
			* \return Node to which this Edge points to.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			const Node& target(void) const;

			/**
			* Edge must have source and target Node initialised, so default constructor is removed.
			*/
			Edge() = delete;

			/**
			* Create Edge pointing from src to tgt Node with given weight
			* \param src Source Node
			* \param tgt Target Node
			* \param weight Weight for Edge
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			Edge(const Node& src, const Node& tgt, float weight);

			/**
			* Create Edge pointing from src to tgt Node with default weight.
			* 
			* Default weight is defined by Edge::DUMMY_WEIGHT
			* \param src Source Node
			* \param tgt Target Node
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			Edge(const Node& src, const Node& tgt);

			/**
			* Assignment operator overload. While Edge is immutable, you can assign to variable of this type another object.
			* 
			* \param to_copy Object of type Edge to be copied.
			* \return Reference to assigned object.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			Edge& operator = (const Edge& to_copy);

			/**
			* Get unique identifier of Edge.
			*/
			uint64_t id(void) const;

			bool operator == (const Edge& edge) const;

			std::string str(void) const;

			Edge Scale(const float& factor) const;

			Edge Reverse(void) const;

			static void SaveBin(std::ostream& file, const Edge& edge);
			static Edge LoadBin(std::istream& file);
			static void SaveTxt(std::ostream& file, const Edge& edge);
			static Edge LoadTxt(std::istream& file);

			struct HashFunction
			{
				uint64_t operator()(const Edge& node) const;
			};
	};
}

#endif
