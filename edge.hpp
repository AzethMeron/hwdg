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
			* 
			* Identifier is based only on Edge::source() and Edge::target(). Direction of edge matters.
			* \return Identifier of Edge
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			uint64_t id(void) const;

			/**
			* Compare Edges. Ignores weight, only Edge::source() and Edge::target() matters.
			* 
			* \param edge Edge to compare against
			* \return true if Edge::id() of edges are equal.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			bool operator == (const Edge& edge) const;

			/**
			* Get string representation.
			* Useful during development and debugging.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			std::string str(void) const;

			/**
			* Scale weight of Edge. 
			* 
			* Creates new Edge with the same source and target Node, and weight multiplied by factor.
			* \param factor Mulitipaction factor for weight.
			* \return New, scaled Edge.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			Edge Scale(const float& factor) const;

			/**
			* Edge pointing in reversed direction.
			* 
			* Creates new Edge pointing from target to source Node, and the same weight. Equivalent to Edge(this->target(), this->source(), this->weight())
			* \return New Edge poinitng in reversed direction
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			Edge Reverse(void) const;

			/**
			* Save to stream - binary form.
			* \param file Reference to ostream-like object, opened in binary mode.
			* \param edge Edge to be saved.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			static void SaveBin(std::ostream& file, const Edge& edge);

			/**
			* Load from stream - binary form.
			* \param file Reference to istream-like object, opened in binary mode.
			* \return Edge loaded from stream.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			static Edge LoadBin(std::istream& file);

			/**
			* Save to stream - text form.
			* \param file Reference to ostream-like object.
			* \param edge Edge to be saved.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			static void SaveTxt(std::ostream& file, const Edge& edge);

			/**
			* Load from stream - text form.
			* \param file Reference to istream-like object.
			* \return Edge loaded from stream.
			* \par Time complexity:
			* \f$O(1)\f$
			*/
			static Edge LoadTxt(std::istream& file);

			/**
			* Support for C++ STL hashing.
			* Allows edges to be stored in STL sets. Hash of Edge is equal its' identifier.
			*/
			struct HashFunction
			{
				uint64_t operator()(const Edge& node) const;
			};
	};
}

#endif
