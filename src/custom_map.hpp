#pragma once

#include <unordered_map>

namespace Graphs
{	
	template<typename keytype, typename valuetype>
	class iterator : public std::unordered_map<keytype, valuetype>::iterator
	{
		private:
			typename std::unordered_map<keytype, valuetype>::iterator* cast_to_base()
			{
				return ((typename std::unordered_map<keytype, valuetype>::iterator*) this);
			}
		public:
			valuetype* operator -> () = delete;
			valuetype& operator * ()
			{
				return (*cast_to_base())->second;
			}
			iterator<keytype, valuetype>(const typename std::unordered_map<keytype, valuetype>::iterator& i) : std::unordered_map<keytype, valuetype>::iterator(i) {}
	};
	
	template<typename keytype, typename valuetype>
	class const_iterator : public std::unordered_map<keytype, valuetype>::const_iterator
	{
		private:
			typename std::unordered_map<keytype, valuetype>::const_iterator* cast_to_base() const
			{
				return ((typename std::unordered_map<keytype, valuetype>::const_iterator*) this);
			}
		public:
			const valuetype* operator -> () const = delete;
			const valuetype& operator * () const
			{
				return (*cast_to_base())->second;
			}
			const_iterator<keytype, valuetype>(const typename std::unordered_map<keytype, valuetype>::const_iterator& i) : std::unordered_map<keytype, valuetype>::const_iterator(i) {}
	};
	
	template<typename keytype, typename valuetype>
	class unordered_map : public std::unordered_map<keytype, valuetype>
	{
		public:
			iterator<keytype, valuetype> begin() { return (iterator<keytype, valuetype>) std::unordered_map<keytype, valuetype>::begin(); }
			const_iterator<keytype, valuetype> cbegin() const { return (const_iterator<keytype, valuetype>) std::unordered_map<keytype, valuetype>::cbegin(); }
			const_iterator<keytype, valuetype> begin() const { return (const_iterator<keytype, valuetype>) std::unordered_map<keytype, valuetype>::begin(); }
			iterator<keytype, valuetype> end() { return (iterator<keytype, valuetype>) std::unordered_map<keytype, valuetype>::end(); }
			const_iterator<keytype, valuetype> cend() const { return (const_iterator<keytype, valuetype>) std::unordered_map<keytype, valuetype>::cend(); }
			const_iterator<keytype, valuetype> end() const { return (const_iterator<keytype, valuetype>) std::unordered_map<keytype, valuetype>::end(); }
			iterator<keytype, valuetype> find(const keytype& id) { return (iterator<keytype, valuetype>) std::unordered_map<keytype, valuetype>::find(id); }
			const_iterator<keytype, valuetype> find(const keytype& id) const { return (const_iterator<keytype, valuetype>) std::unordered_map<keytype, valuetype>::find(id); }
	};
}