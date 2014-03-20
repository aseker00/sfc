#ifndef SFC_CLC_LINKED_LIST_H
#define SFC_CLC_LINKED_LIST_H

#include "../fwk/FwkBasePtrCollection.h"
#include "../fwk/FwkBasePtrIterator.h"

namespace Cls
{
/*
As an invasive collection, LinkedList depends on, and thus requires, any type
T used as a type parameter to it to support the smart pointer interface and to
implement the linkedListNext attribute.
*/
template<class T>
class LinkedList : public Fwk::BasePtrCollection
{
	/*
	An iterator is created with optionally a key or selector indicating the initial member
	to point to. If there is no member designated by the key, the iterator is created as
	pointing to null. Using this iterator mechanism, deletion of a member of list, for
	instance, can be implemented by creating an iterator pointing to that member, spec-
	ified using as key a pointer to this instance, and then invoking the delete operator
	on the iterator.
	*/
	template<class TT>
	class LinkedListIteratorConst : public Fwk::BasePtrIterator<TT>
	{
	public:
		LinkedList<T> *list() const { return static_cast<LinkedList<T> *>(collection_); }
		/*
		Following the pointer model, an iterator indicates being at the end of a collection
		by returning null. An iterator does not throw exceptions. It is the responsibility of
		the programmer to check for an iterator being null. Using a null iterator, including
		incrementing an null iterator is undefined and typically has bad consequences, the
		same as when using a null pointer.
		*/
		LinkedListIteratorConst &operator++()
		{ 
			TT *current = ptr();
			ptrIs(current->linkedListNext());
			return *this;
		}
		LinkedListIteratorConst operator++(int)
		{
			LinkedListIteratorConst tmp(*this);
			++*this;
			return tmp;
		}

	protected:
		friend class LinkedList<T>;
		LinkedListIteratorConst(const LinkedList<T> *ll, T *t) : BasePtrIterator<TT>(const_cast<LinkedList<T> *>(ll), t) {}
		LinkedListIteratorConst(LinkedList<T> *ll, T *t) : BasePtrIterator<TT>(ll, t) {}
	};

	template<class TT> // TT is either T or const T.
	class LinkedListIterator : public LinkedListIteratorConst<TT> 
	{
	public:
		LinkedListIterator &operator++() 
		{
			TT *current = ptr();
			prev_ = current;
			ptrIs(current->linkedListNext());
			return *this;
		}
		/*
		The non-const iterator is used for gaining non-const access to members as well as
		for deleting members using operator delete defined on the iterator. This deletes
		(or reduces to default value) from the collection the object currently pointed to by
		the iterator, if any, leaving the iterator pointing to the previous element, if any. Use
		of operator delete makes sense because it is defined to delete the object pointed to
		by its parameter, not the parameter itself, of course. Thus, this is further consistent
		with treating an iterator as an extended pointer.
		*/
		void operator delete(void *thisPtr)
		{
			/**
			what is thisPtr? is it this? if so why do you need to work on me?
			*/
			LinkedListIterator *me = (LinkedListIterator *)thisPtr;
			/**
			TODO: is this right?
			this whole implementation is questionable.
			why do you need to call deleteMemeber, and ptrIs(0)?
			*/
			if(me->savedVersion_ == me->list()->version_)
			{
				deleteMember(thisPtr);
				ptrIs(0);
			}
			TT *previous = me->prev_.ptr();
			TT *current = me->ptr();
			TT *newCurrent = current->linkedListNext();
			if(!newCurrent) 
				me->tail_ = previous;
			if(previous)
				previous->linkedListNextIs(newCurrent);
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
			TT *previous = prev_.ptr();
			if(previous)
			{
				newMember->linkedListNextIs(ptr());
				previous->linkedListNextIs(newMember);
			}
			else 
			{
				newMember->linkedListNextIs(list()->head());
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
			newMember->linkedListNextIs(current->linkedListNext());
			current->linkedListNextIs(newMember);
			if(!newMember->linkedListNext()) 
				tail_ = newMember_;
			++list()->members_;
			savedVersion_ = ++list()->version_;
		}
	protected:
		typename TT::Ptr prev_;
		unsigned int savedVersion_;
		friend class LinkedList<T>;
		LinkedListIterator(const LinkedList<T> *ll, T *t) : LinkedListIteratorConst<TT>(ll, t), prev_(0), savedVersion_(list()->version_) {}
		LinkedListIterator(LinkedList<T> *ll, T *t) : LinkedListIteratorConst<TT>(ll, t), prev_(0), savedVersion_(list()->version_) {}
	};

public:
	typedef LinkedListIterator<T> Iterator;

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

	typedef LinkedListIteratorConst<const T> IteratorConst;

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
	LinkedList() : head_(0), tail_(0) {}
	T *head() const { return head_.ptr(); }
	
	void newHead(const typename T::Ptr &newMember)
	{
		if(!head_) 
			tail_ = newMember.ptr();
		newMember->linkedListNextIs(head_.ptr()); 
		head_ = newMember;
		++version_;
		++members_;
	}
	void newTail(const typename T::Ptr &newMember) 
	{
		if(tail_) 
			tail_->linkedListNextIs(newMember); 
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
			head_ = ptr->linkedListNext();
			ptr->linkedListNextIs(0);
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
			for(typename T::Ptr nextC = c->linkedListNext(); nextC; c = nextC) 
			{
				nextC = c->linkedListNext();
				c->linkedListNextIs(0); // Deference next member
			}
		}
		members_ = 0;
		++version_;
	}
	/* 
	Besides the normal list operations, it is useful to treat the pointer as the key for a collection 
	such as a list that does not otherwise have a key. 
	Thus, the list implementation further provides a lookup and a delete function.
	*/
	typename T::Ptr member(T *t)
	{
		for(T *c = head_.ptr(); c; c = c->linkedListNext()) 
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
		for(T *c = head_.ptr(); c; c = c->linkedListNext())
		{
			if(c == t)
			{
				r = t;
				if(!c->linkedListNext()) 
					tail_ = prev;
				if(prev) 
					prev->linkedListNextIs(c->linkedListNext());
				else 
					head_ = c->linkedListNext();
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
/*
A collection attribute can be implemented with in-line accessors and in-line or non-
inline mutators.
*/
//class Port : public NamedInterface 
//{
//public:
//	typedef LinkedList<PacketBuf> PacketBufQueue;
//	unsigned int xmitQueueLength() const { return xmitQueue_.members(); }
//	void nextPacketIs(const PacketBuf::Ptr &pb) 
//	{
//		xmitQueue_.newTail(pb);
//		pb->port_ = this;
//	}
//
//private:
//	PacketBufQueue xmitQueue_;
//};
//class PacketBuf : PtrInterface<PacketBuf> 
//{
//public:
//	typedef ::Ptr<PacketBuf> Ptr;
//	PacketBuf *linkedListNext() const { return next_.ptr(); }
//	void linkedListNextIs(const PacketBuf::Ptr &pb) { next_ = pb; }
//	
//private:
//	PacketBuf::Ptr next_;
//	Port *port_;
//};

#endif