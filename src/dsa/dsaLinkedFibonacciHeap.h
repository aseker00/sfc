#pragma once
#include "../fwk/fwkBasePtrCollection.h"
#include "../fwk/fwkBasePtrIterator.h"

namespace dsa
{
template <class T>
class LinkedFibonnaciHeap : public fwk::BasePtrCollection
{
public:
	LinkedFibonacciHeap() : head_(0), n_(0), t_(0), m_(0) {}
	typename T::Ptr head() const { return head_; }
	LinkedFibonnaciHeap::Ptr merge(LinkedFibonacciHeap::PtrConst heap2) const
	{
		LinkedFibonacciHeap::Ptr heap = new LinkedFibonacciHeap();
		heap->head_(head_);

		return heap;
	}
	typename T::Ptr newMember(const typename T::Ptr &newMember)
	{
		const_cast<typename T::Ptr>(newMember)->fibonacciHeapDegreeIs(0);
		const_cast<typename T::Ptr>(newMember)->fibonacciHeapParentIs(0);
		const_cast<typename T::Ptr>(newMember)->fibonacciHeapChildIs(0);
		const_cast<typename T::Ptr>(newMember)->fibonacciHeapLeftIs(this);
		const_cast<typename T::Ptr>(newMember)->fibonacciHeapRightIs(this);
		const_cast<typename T::Ptr>(newMember)->fibonacciHeapMarkIs(false);
		if (!head_) 
		{
			head_ = newMember;
		}
		else 
		{
			// concatenate root list of newMember with this roos list
			newMember->doublyLinkedListNextIs(head_);
			newMember->doublyLinkedListPreviousIs(head_->doublyLinkedListPrevious());
			head_->doublyLinkedListPreviousIs(newMember);
			int diff = newMember->difference(head_);
			if (diff < 0)
				head_ = newMember;
		}
		++version_;
		++members_;
		++n_;
		return 0;
	}

	typename T::Ptr member(T *t) const
	{
		T *current = t;
		typename T::Ptr parent = current->fibonacciHeapParent();
		while (parent)
		{
			current = parent.ptr();
			parent = current->fibonacciHeapParent();
		}
		while (current)
		{
			int diff = t->difference(current);
			if (diff == 0)
				return current;
			current = diff < 0 ? current->linkedBinaryTreeLeft() : current->linkedBinaryTreeRight();
        }
		return 0;
    }
	typename T::Ptr deleteMember(T *t)
	{
		if (!head_)
			return 0;
		T *next = head_;
		T *parent = 0;
		while (next)
		{
			int diff = t->difference(next);
			if (diff == 0)
			{
				if (next->linkedBinaryTreeLeft() && next->linkedBinaryTreeRight())
				{
					T *min = min(next->linkedBinaryTreeRight());
					T::Ptr node = deleteMember(min);
					if (parent->linkedBinaryTreeLeft() == next)
						parent->linkedBinaryTreeLeftIs(node);
					else
						parent->linkedBinaryTreeRightIs(node);
					node->linkedBinaryTreeLeftIs(next->linkedBinaryTreeLeft());
					node->linkedBinaryTreeRightIs(next->linkedBinaryTreeRight());
				}
				else if (parent->linkedBinaryTreeLeft() == next)
					parent->linkedBinaryTreeLeftIs(next->linkedBinaryTreeLeft() ? next->linkedBinaryTreeLeft() : next->linkedBinaryTreeRight());
				else
					parent->linkedBinaryTreeRightIs(next->linkedBinaryTreeLeft() ? next->linkedBinaryTreeLeft() : next->linkedBinaryTreeRight());
				++version_;
				--members_;
				return next;
			}
			parent = next;
			next = diff < 0 ? next->linkedBinaryTreeLeft() : next->linkedBinaryTreeRight();
		}
		return 0;
	}

private:
	unsigned int n_;	// number of nodes in H
	unsigned int t_;	// number of trees in the h
	unsigned int m_;	// number of marked nodes in H
	typename T::Ptr head_;	// min/max pointer
};
}
