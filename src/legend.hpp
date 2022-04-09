#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>
#include "string_format.hpp"
#include "node.hpp"

template<typename TYPE>
class Legend
{
	private:
		std::unordered_map<uint32_t, TYPE> _map;
	public:
		bool has(const Node& node) const;
		const TYPE& fetch(const Node& node) const;
		void update(const Node& node, const TYPE& object);
};

template<typename TYPE>
bool Legend<TYPE>::has(const Node& node) const
{
	auto iter = this->_map.find(node.id());
	if(iter == this->_map.cend()) return false;
	return true;
}

template<typename TYPE>
const TYPE& Legend<TYPE>::fetch(const Node& node) const
{
	auto iter = this->_map.find(node.id());
	if(iter == this->_map.cend()) throw std::out_of_range(string_format("No such node: %s", node.str().c_str()));;
	return iter->second;
}

template<typename TYPE>
void Legend<TYPE>::update(const Node& node, const TYPE& object)
{
	this->_map.insert_or_assign(node.id(), object);
}
