#pragma once
#include "../fwk/fwkBasePtrCollection.h"
#include "../fwk/fwkBasePtrIterator.h"

namespace dsa
{
template<class T>
class DoublyLinkedList : public fwk::BasePtrCollection
{
	template<class TT>
	class DoublyLinkedListIteratorConst : public fwk::BasePtrIterator<TT>
	{
	public:
		DoublyLinkedList<T> *list() const { return static_cast<DoubleLinkedList<T> *>(collection_); }
		DoublyLinkedListIteratorConst &operator++()
		{ 
			TT *current = ptr();
			ptrIs(current->doublyLinkedListNext());
			return *this;
		}
		LinkedListIteratorConst operator++(int)
		{
			DoublyLinkedListIteratorConst tmp(*this);
			++*this;
			return tmp;
		}
		DoublyLinkedListIteratorConst &operator--()
		{ 
			TT *current = ptr();
			ptrIs(current->doublyLinkedListPrevious());
			return *this;
		}
		DoublyLinkedListIteratorConst operator--(int)
		{
			DoublyLinkedListIteratorConst tmp(*this);
			--*this;
			return tmp;
		}

	protected:
		friend class LinkedList<T>;
		DoublyLinkedListIteratorConst(const DoublyLinkedList<T> *dll, T *t) : BasePtrIterator<TT>(const_cast<DoublyLinkedList<T> *>(dll), t) {}
		DoublyLinkedListIteratorConst(DoublyLinkedList<T> *dll, T *t) : BasePtrIterator<TT>(dll, t) {}
	};

	template<class TT> // TT is either T or const T
	class DoublyLinkedListIterator : public DoublyLinkedListIteratorConst<TT> 
	{
	public:
		DoublyLinkedListIterator &operator++()
		{
			TT *current = ptr();
			ptrIs(current->doublyLinkedListNext());
			return *this;
		}
		DoublyLinkedListIterator &operator--()
		{
			TT *current = ptr();
			ptrIs(current->doublyLinkedListPrevious());
			return *this;
		}
		void operator delete(void *thisPtr)
		{
			DoublyLinkedListIterator *me = (DoublyLinkedListIterator *)thisPtr;
			if(me->savedVersion_ == me->list()->version_)
			{
				deleteMember(thisPtr);
				ptrIs(0);
			}
			TT *current = me->ptr();
			TT *previous = current->doublyLinkedListPrevious();
			TT *newCurrent = current->doublyLinkedListNext();
			if(!newCurrent)
				me->tail_ = previous;
			if(previous)
			{
				previous->doublyLinkedListNextIs(newCurrent);
				newCurrent->doublyLinkedListPreviousIs(previous);
			}
			else 
				me->list()->head_ = newCurrent;
			me->ptrIs(newCurrent);
			--me->list()->members_;
			me->savedVersion_ = ++me->list()->version_;
		}
		void newPtr(T *newMember)
		{
			if(savedVersion_ != list()->version_)
				throw VersionException("list");
			TT *current = me->ptr();
			TT *previous = current->doublyLinkedListPrevious();
			if(previous)
			{
				newMember->doublyLinkedListNextIs(current);
				current->doublyLinkedListPreviousIs(newMember);
				previous->doublyLinkedListNextIs(newMember);
				newMember->doublyLinkedListPreviousIs(previous);
			}
			else 
			{
				newMember->doublyinkedListNextIs(list()->head_);
				if (list()->head_)
					list()->head_->doublyLinkedListPreviousIs(newMember);
				list()->head_ = newMember;
			}
			if(previous == tail_) 
				tail_ = newMember;
			ptrIs(newMember);
			++list()->members_;
			savedVersion_ = ++list()->version_;
		}
		void newMember(T *newMember)
		{
			if(savedVersion_ != list()->version_)
				throw VersionException("list");
			TT *current = ptr();
			TT *next = current->linkedListNext();
			newMember->doublyLinkedListNextIs(next);
			if (next)
				next->doublyLinkedListPreviousIs(newMember);
			current->doublyLinkedListNextIs(newMember);
			newMember->doubleLinkedListPreviousIs(current);
			if(!newMember->doublyLinkedListNext()) 
				tail_ = newMember_;
			++list()->members_;
			savedVersion_ = ++list()->version_;
		}
	protected:
		unsigned int savedVersion_;
		friend class DoublyLinkedList<T>;
		DoublyLinkedListIterator(const DoublyLinkedList<T> *dll, T *t) : DoublyLinkedListIteratorConst<TT>(dll, t), savedVersion_(list()->version_) {}
		DoublyLinkedListIterator(DoublyLinkedList<T> *dll, T *t) : DoublyLinkedListIteratorConst<TT>(dll, t), savedVersion_(list()->version_) {}
	};

public:
	typedef DoublyLinkedListIterator<T> Iterator;
	typedef DoublyLinkedListIteratorConst<const T> IteratorConst;

	Iterator iterator() { return Iterator(this, head_.ptr()); }
	Iterator iterator(const typename T::Ptr &start)
	{
		T *p = start.ptr();
		bool search = false;
		if(p && (p != head_) && (p != tail_))
		{
			p = head_.ptr();
			search = true;
		}
		Iterator ii(this, p);
		if(search) 
		{
			p = start.ptr();
			while(ii && ii != p) 
				++ii;
		}
		return ii;
	}

	IteratorConst iteratorConst() const { return IteratorConst(this, head_.ptr()); }
	IteratorConst iteratorConst(const typename T::Ptr &start) const 
	{
		T *p = start.ptr();
		bool search = false;
		if(p && (p != head_) && (p != tail_)) 
		{
			p = head_.ptr();
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
public:
	DoublyLinkedList() : head_(0), tail_(0) {}
	T *head() const { return head_.ptr(); }
	
	void newHead(typename T::Ptr &newMember)
	{
		if(!head_) 
			tail_ = newMember.ptr();
		newMember->doublyLinkedListNextIs(head_);
		if (head_)
			head_->doublyLinkedListPreviousIs(newMember);
		head_ = newMember;
		++version_;
		++members_;
	}
	void newTail(const typename T::Ptr &newMember) 
	{
		if(tail_)
		{
			tail_->doublyLinkedListNextIs(newMember); 
			newMember->doublyLinkedListPreviousIs(tail_);
		}
		else
			head_ = newMember;
		tail_ = newMember.ptr();
		++version_;
		++members_;
	}
	void deleteHead() 
	{
		if(T *ptr = head_.ptr()) 
		{
			if(tail_ == ptr) 
				tail_ = 0;
			head_ = ptr->doublyLinkedListNext();
			ptr->doublyLinkedListNextIs(0);
			ptr->doublyLinkedListPreviousIs(0);
			++version_;
			--members_;
		}
	}
	void deleteAll() 
	{
		typename T::Ptr c = head_.ptr();
		head_ = 0;
		if(c) 
		{
			for(typename T::Ptr nextC = c->doublyLinkedListNext(); nextC; c = nextC) 
			{
				nextC = c->linkedListNext();
				c->doublyLinkedListNextIs(0);
				c->doublyLinkedListPreviousIs(0);
			}
		}
		members_ = 0;
		++version_;
	}
	
	typename T::Ptr member(T *t)
	{
		for(T *c = head_.ptr(); c; c = c->doublyLinkedListNext()) 
		{
			if(c == t)
				return t;
		}
		return 0;
	}
	typename T::Ptr deleteMember(T *t) 
	{
		T *prev = 0;
		typename T::Ptr r;
		for(T *c = head_.ptr(); c; c = c->doublyLinkedListNext())
		{
			if(c == t)
			{
				r = t;
				if(!c->doublyLinkedListNext()) 
					tail_ = prev;
				if(prev)
				{
					prev->doublyLinkedListNextIs(c->doublyLinkedListNext());
					c->doublyLinkedListNext()->doublyLinkedListPreviousIs(prev);
				}
				else 
					head_ = c->doublyLinkedListNext();
				++version_;
				--members_;
				return r;
			}
			prev = c;
		}
		return 0;
	}
	~LinkedList() { deleteAll(); }
private:
	typename T::Ptr head_;
	T *tail_;
};
}
