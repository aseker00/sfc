#ifndef SFC_CLC_ARRAY_BINARY_HEAP_H
#define SFC_CLC_ARRAY_BINARY_HEAP_H

#include "../fwk/FwkBasePtrCollection.h"

namespace Clc
{

// Non invasive collection
template <class T>
class ArrayBinaryHeap : public Fwk::BasePtrCollection
{
public:
	ArrayBinaryHeap(unsigned int length) : length_(length) data_(0) { data_ = new typename T::Ptr[length_]; }
	T *head() const { return (*data_).ptr(); }
	void deleteHead()
	{
		typename T::Ptr node = data_[members-1];
		data_[0] = node;
		heapifyDown(0);
		++version_;
		--members_;
	}

	typename T::Ptr member(T *t)
	{
		unsigned int index = 0;
		while (index < members_)
		{
			typename T::Ptr node = data_[index];
			int diff = t->difference(node);
			if (diff == 0)
				return node;
			if (diff < 0)
				index = getLeftChildIndex(index);
			else
				index = getRightChildIndex(index);
			node = data_[index];
		}
		return 0;
    }
	typename T::Ptr newMember(typename T::Ptr &newMember)
	{
		data_[members_-1] = newMember;
		heapifyUp(members_-1);
		++version_;
		++members_;
		return 0;
	}
	typename T::Ptr deleteMember(T *t)
	{
		unsigned int index = 0;
		while (index < members_)
		{
			typename T::Ptr node = data_[index];
			int diff = t->difference(node);
			if (diff == 0)
			{
				typename T::Ptr lastNode = data_[members-1];
				data_[index] = lastNode;
				heapifyDown(index);
				++version_;
				--members_;
				return node;
			}
			if (diff < 0)
				index = getLeftChildIndex(index);
			else
				index = getRightChildIndex(index);
			node = data_[index];
		}
		return 0;
	}

private:
	void heapifyUp(unsigned int index)
	{
		while (index > 0)
		{
			typename T::Ptr node = data_[index];
			unsigned int parentIndex = getParentIndex(index);
			typename T::Ptr parent = data_[parentIndex];
			//int diff = newMember.difference(parent);
			int diff = node->difference(parent);
			if (diff <= 0)
				break;
			index = parentIndex;
		}
	}
	void heapifyDown(unsigned int index)
	{
		while (index < members_)
		{
			typename T::Ptr node = data_[index];
			unsigned int leftChildIndex = getLeftChildIndex(index);
			unsigned int rightChildIndex = getRightChildIndex(index);	
			unsigned int minIndex;
			if (leftChildIndex >= members_)
			{
				if (rightChildIndex >= members_)
					break;
				minIndex = rightChildIndex;
			}
			else
			{
				minIndex = leftChildIndex;
			}
			typename T::Ptr min = data_[minIndex];
			int diff = node->difference(min);
			if (diff > 0)
			{
				data_[minIndex] = node;
				data_[index] = min;
				index = minIndex;
			}
		}
	}
	int getLeftChildIndex(int nodeIndex) { return 2*nodeIndex+1; }
	int getRightChildIndex(int nodeIndex) { return 2*nodeIndex+2; }
	int getParentIndex(int nodeIndex) { return (nodeIndex-1)/2; }

	typename T::Ptr *data_;
	unsigned int length_;
};
}

#endif