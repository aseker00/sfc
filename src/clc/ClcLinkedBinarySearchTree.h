#ifndef SFC_CLC_LINKED_BINARY_SEARCH_TREE_H
#define SFC_CLC_LINKED_BINARY_SEARCH_TREE_H

#include "../fwk/FwkBasePtrCollection.h"
#include "../fwk/FwkBasePtrIterator.h"

namespace Clc
{
template <class T>
class LinkedBinarySearchTree : public Fwk::BasePtrCollection
{
public:
	template<class TT>
	class LinkedBinarySearchTreeIteratorConst : public Fwk::BasePtrIterator<TT>
	{
	public:
		LinkedBinarySearchTree<T> *tree() const { return static_cast<LinkedBinarySearchTree<T> *>(collection_); }
		LinkedBinarySearchTreeIteratorConst &operator++()
		{
			TT *current = ptr();
			if (current->linkedBinaryTreeRight())
				min(current->linkedBinaryTreeRight());
			ptrIs(stack_.head());
			stack_.deleteHead();
			return *this;
		}
		LinkedBinarySearchTreeIteratorConst operator++(int)
		{
			LinkedBinaryTreeIteratorConst tmp(*this);
			++*this;
			return tmp;
		}

	protected:
		friend class LinkedBinarySearchTree<T>;
		typedef LinkedList<T> Stack;
		LinkedBinarySearchTreeIteratorConst(const LinkedBinarySearchTree<T> *lbst, T *t) : BasePtrIterator<TT>(const_cast<LinkedBinarySearchTree<T> *>(lbst), t)
		{
			min(t);
			ptrIs(stack_.head());
			stack_.deleteHead();
		}
		LinkedBinarySearchTreeIteratorConst(LinkedBinarySearchTree<T> *lbst, T *t) : BasePtrIterator<TT>(lbst, t) {}
		void min(T *t)
		{
			T::Ptr next = t;
			while (next)
			{
				stack_.newHead(next);
				next = next->linkedBinaryTreeLeft();
			}
		}

		Stack stack_;
	};

	typedef LinkedBinarySearchTreeIteratorConst<const T> IteratorConst;

	IteratorConst iteratorConst() const { return IteratorConst(this, root_.ptr()); }
	IteratorConst iteratorConst(const typename T::Ptr &start) const 
	{
		T *p = start.ptr();
		bool search = false;
		if(p && (p != root_)) 
		{
			p = root_.ptr();
			search = true;
		}
		IteratorConst ii(this, p);
		if(search) 
		{
			p = start.ptr();
			while(ii && ii != p) 
				++ii;
		}
		return ii;
	}

	LinkedBinarySearchTree() : root_(0) {}
	
	typename T::Ptr newMember(const typename T::Ptr &newMember)
	{
		if (!root_)
		{
			root_ = newMember;
			++version_;
			++members_;
			return root_;
		}
		typename T::Ptr next = root_;
		T *parent = 0;
		while (next)
		{
			int diff = newMember->difference(next);
			/*if (diff == 0)
			{
				if (parent->linkedBinaryTreeLeft() == next)
					parent->linkedBinaryTreeLeftIs(newMember);
				else
					parent->linkedBinaryTreeRightIs(newMember);
				if (next->linkedBinaryTreeLeft())
				{
					newMember->linkedBinaryTreeLeftIs(next->linkedBinaryTreeLeft());
					next->linkedBinaryTreeLeftIs(0);
				}
				if (next->linkedBinaryTreeRight())
				{
					newMember->linkedBinaryTreeRightIs(next->linkedBinaryTreeRight());
					next->linkedBinaryTreeRightIs(0);
				}
				++version_;
				return next;
			}*/
			if (diff <= 0)
			{
				if (!next->linkedBinaryTreeLeft())
				{
					next->linkedBinaryTreeLeftIs(newMember.ptr());
					++version_;
					++members_;
					return 0;
				}
				next = next->linkedBinaryTreeLeft();
			}
			else
			{
				if (!next->linkedBinaryTreeRight())
				{
					next->linkedBinaryTreeRightIs(newMember.ptr());
					++version_;
					++members_;
					return 0;
				}
				next = next->linkedBinaryTreeRight();
			}
			parent = next.ptr();
		}
		return 0;
	}

	typename T::Ptr member(T *t)
	{
		T *next = root_;
		while (next)
		{
			int diff = t->difference(next);
			if (diff == 0)
				return next;
			//next = order < 0 ? next->linkedBinaryTreeLeft() : next->linkedBinaryTreeRight();
			next = diff < 0 ? next->linkedBinaryTreeLeft() : next->linkedBinaryTreeRight();
        }
		return 0;
    }
	typename T::Ptr deleteMember(T *t)
	{
		if (!root_)
			return 0;
		T *next = root_;
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
	T *min(T *t)
	{
		T *node = 0;
		T *next = t;
		while (next)
		{
			node = next;
			next = next->linkedBinaryTreeLeft();
		}
		return node;
	}
	typename T::Ptr root_;
};
}

#endif