#ifndef HWDG_HEAP_HPP
#define HWDG_HEAP_HPP

/**
* @file heap.hpp
* @author Jakub Grzana
* @date October 2022
* @brief Binary heap implementation
*
* Implementation of binary heap with overridable HeapCompare and HeapSwap methods, so it can be used with Dijkstra algorithm.
*/

#include<vector>

namespace HWDG
{
	/**
	* Binary heap implementation.
	* 
	* It allows to create custom compare (HeapCompare) and swap (HeapSwap) method. Furthemore, it allows to RestoreHeap after external modifications.
	* 
	* \tparam TYPE Type to be stored on heap, it should be copyable and movable.
	*/
	template<typename TYPE>
	class Heap {
	private:
		std::vector<TYPE> _heap;
	private:
		inline size_t HeapLeftChild(const size_t& index) const
		{
			return index * 2 + 1;
		}
		inline size_t HeapRightChild(const size_t& index) const
		{
			return index * 2 + 2;
		}
		inline bool HeapExist(const size_t& index) const
		{
			return index >= 0 && index < this->HeapSize();
		}
		inline size_t HeapParent(const size_t& index) const
		{
			if (index == 0) return 0;
			return (index - 1) / 2;
		}
		void Heapify(const size_t& position) // Check & swap with children of given position. Doesn't check parent
		{
			size_t r_child_ind = this->HeapRightChild(position);
			size_t l_child_ind = this->HeapLeftChild(position);
			bool r_child_smaller_than_parent = false;
			bool l_child_smaller_than_parent = false;
			if (this->HeapExist(r_child_ind)) r_child_smaller_than_parent = this->HeapCompare(position, r_child_ind);
			if (this->HeapExist(l_child_ind)) l_child_smaller_than_parent = this->HeapCompare(position, l_child_ind);
			if (r_child_smaller_than_parent && l_child_smaller_than_parent)
			{
				if (this->HeapCompare(r_child_ind, l_child_ind))
				{
					this->HeapSwap(position, l_child_ind);
					this->Heapify(l_child_ind);
				}
				else
				{
					this->HeapSwap(position, r_child_ind);
					this->Heapify(r_child_ind);
				}
			}
			else if (r_child_smaller_than_parent)
			{
				this->HeapSwap(position, r_child_ind);
				this->Heapify(r_child_ind);
			}
			else if (l_child_smaller_than_parent)
			{
				this->HeapSwap(position, l_child_ind);
				this->Heapify(l_child_ind);
			}
		}
	protected:
		/**
		* Swap elements on heap.
		* 
		* Typically std::swap(this->HeapAt(l), this->HeapAt(r)), but it may be used to synchronise something outside of heap. For example, Dijkstra needs to track position of Nodes on heap, so when they're moved on heap, pathtable is updated too.
		* 
		* \param l Index of lvalue
		* \param r Index of rvalue
		*/
		virtual inline void HeapSwap(const size_t& l, const size_t& r) = 0;
		/**
		* Compare values on Heap. 
		* 
		* Used internally by implementation, must be overriden. It may use HeapAt(index) to get values of objects on heap, but may also fetch those values from somewhere else. In Dijkstra algorithm, it fetches pathweights for nodes from pathtable which is implemented within Dijkstra class.
		* 
		* if you want to sort lower to highest value, lvalue must be > rvalue
		* 
		* HeapAt(l) > HeapAt(r) -> 5, 7, 10, 15
		* 
		* HeapAt(l) < HeapAt(r) -> 15, 10, 7, 5
		* 
		* \param l Index of lvalue
		* \param r Index of rvalue
		* 
		* \return True/False, depending on lvalue, rvalue and descending/ascending order.
		*/
		virtual bool HeapCompare(const size_t& l, const size_t& r) const = 0;
	public:
		/**
		* Number of elements on the heap.
		* 
		* \return Number of elements stored on the Heap.
		* 
		* \par Time complexity: 
		* \f$O(1)\f$
		*/
		size_t HeapSize() const { return this->_heap.size(); }
		/**
		* Raw access (read/write) to object on given index on Heap.
		* 
		* This might be useful but if you change anything under this position, you're supposed to manually call RestoreHeap.
		* This call isn't automatically done here, because there're times you actually can't restore heap instantly after changing it.
		* 
		* \param ind Index on the heap. 
		* \return TYPE object stored on the heap (reference)
		* 
		* \par Time complexity: 
		* \f$O(1)\f$
		*/
		TYPE& HeapAt(const size_t& ind) { return this->_heap.at(ind); } 
		/**
		* Read access to data on Heap.
		*
		* \param ind Index on the heap.
		* \return TYPE object stored on the heap (read-only reference)
		*
		* \par Time complexity:
		* \f$O(1)\f$
		*/
		const TYPE& HeapAt(const size_t& ind) const { return this->_heap.at(ind); }
		/**
		* Add object to the Heap.
		*/
		void HeapPush(const TYPE& obj)
		{
			this->_heap.push_back(obj);
			this->RestoreHeap(this->HeapSize() - 1);
		}
		/**
		* Remove object from Heap.
		*/
		TYPE HeapPop()
		{
			TYPE output = this->HeapAt(0);
			this->HeapSwap(0, this->HeapSize() - 1);
			this->_heap.pop_back();
			this->Heapify(0);
			return output;
		}
		Heap() {}
		Heap(const std::vector<TYPE>& vec)
		{
			this->_heap = vec;
			for (int64_t i = this->HeapParent(this->HeapSize() - 1); i >= 0; --i)
			{
				this->Heapify(i);
			}
		}
		/**
		* Restore Heap after values innit on given position changed.
		*/
		void RestoreHeap(const size_t& position) // Heapify node, then call RestoreHeap for parent of it
		{
			this->Heapify(position);
			size_t parent = this->HeapParent(position);
			if (parent != position)
			{
				this->RestoreHeap(parent);
			}
		}
		auto begin() const { return this->_heap.begin(); }
		auto end() const { return this->_heap.end(); }
		void reserve(const size_t& size)
		{
			this->_heap.reserve(size);
		}
	};
}

#endif