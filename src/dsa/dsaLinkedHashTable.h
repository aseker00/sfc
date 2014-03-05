#pragma once
#include "../fwk/fwkBasePtrCollection.h"
#include "../fwk/fwkBasePtrIterator.h"
#include "../fwk/fwkFactor.h"

namespace dsa
{
template <class T>
class LinkedHashTable : public fwk::BasePtrCollection
{
public:
	template<class TT>
	class LinkedHashTableIteratorConst : public fwk::BasePtrIterator<TT>
	{
	public:
		LinkedHashTable<T> *table() const { return static_cast<LinkedHashTable<T> *>(collection_); }
		LinkedHashTableIteratorConst &operator++()
		{ 
			TT *current = ptr();
			if (current)
				current = current->linkedHashTableNext();
			while (!current && pos_ < table()->capacity_-1)
				current = table()->buckets_[++pos_].ptr();
			ptrIs(current);
			return *this;
		}
		LinkedHashTableIteratorConst operator++(int)
		{
			LinkedHashTableIteratorConst tmp(*this);
			++*this;
			return tmp;
		}

	protected:
		friend class LinkedHashTable<T>;
		unsigned int pos_;
		LinkedHashTableIteratorConst(const LinkedHashTable<T> *hm, T *t) : BasePtrIterator<TT>(const_cast<LinkedHashTable<T> *>(hm), t), pos_(0) 
		{
			TT *current = ptr();
			while (!current && pos_ < table()->capacity_-1)
				current = table()->buckets_[++pos_].ptr();
			ptrIs(current);
		}
		LinkedHashTableIteratorConst(LinkedHashTable<T> *hm, T *t) : BasePtrIterator<TT>(hm, t) 
		{
			TT *current = ptr();
			while (!current && pos_ < table()->capacity_-1)
				current = table()->buckets_[++pos_].ptr();
			ptrIs(current);
		}
	};
	template<class TT> // TT is either T or const T.
	class LinkedHashTableIterator : public LinkedHashTableIteratorConst<TT> 
	{
	public:
		LinkedHashTableIterator &operator++() 
		{
			prev_ = 0;
			TT *current = ptr();
			if (current)
			{
				prev_ = current;
				current = current->linkedHashTableNext();
			}
			while (!current && pos_ < table()->capacity_-1)
				current = table()->buckets_[++pos_].ptr();
			ptrIs(current);
			return *this;
		}
		void operator delete(void *thisPtr)
		{
			LinkedHashTableIterator *me = (LinkedHashTableIterator *)thisPtr;
			/*if(me->savedVersion_ == me->table()->version_)
			{
				deleteMember(thisPtr);
				ptrIs(0);
			}*/
			TT *previous = me->prev_.ptr();
			TT *current = me->ptr();
			TT *newCurrent = current->linkedHashTableNext();
			if(previous)
				previous->linkedHashTableNextIs(newCurrent);
			else
				me->table()->buckets_[me->pos_] = newCurrent;
			while (!newCurrent && me->pos_ < me->table()->capacity_-1)
				newCurrent = me->table()->buckets_[++me->pos_].ptr();
			me->ptrIs(newCurrent);
			--me->table()->members_;
			me->savedVersion_ = ++me->table()->version_;
		}
	protected:
		typename TT::Ptr prev_;
		unsigned int savedVersion_;
		friend class LinkedHashTable<T>;
		LinkedHashTableIterator(const LinkedHashTable<T> *hm, T *t) : LinkedHashTableIteratorConst<TT>(hm, t), prev_(0), savedVersion_(table()->version_) {}
		LinkedHashTableIterator(LinkedHashTable<T> *hm, T *t) : LinkedHashTableIteratorConst<TT>(hm, t), prev_(0), savedVersion_(table()->version_) {}
	};

	typedef LinkedHashTableIteratorConst<const T> IteratorConst;
	IteratorConst iteratorConst() const { return IteratorConst(this, buckets_[0].ptr()); }
	typedef LinkedHashTableIterator<T> Iterator;
	Iterator iterator() const { return Iterator(this, buckets_[0].ptr()); }

	LinkedHashTable() : capacity_(7), load_(0.75) { buckets_ = new typename T::Ptr[capacity_]; }
	LinkedHashTable(unsigned int capacity) : capacity_(capacity), load_(0.75) { buckets_ = new typename T::Ptr[capacity_]; }
	LinkedHashTable(unsigned int capacity, Factor load) : capacity_(capacity), load_(load) { buckets_ = new typename T::Ptr[capacity_]; }
	~LinkedHashTable() { deleteAll(); }

	typename T::Ptr newMember(const typename T::Ptr &newMember) { memberIs(newMember); return 0; }

	typename T::Ptr member(T *t) { return member(t->hashCode()); }
	typename T::Ptr deleteMember(T *t)
	{
		T *prev = 0;
		typename T::Ptr r;
		unsigned int pos = t->hashCode()%capacity_;
		for(T *c = buckets_[pos].ptr(); c; c = c->linkedHashTableNext())
		{
			if(c->hashCode() == t->hashCode())
			{
				r = t;
				if(prev) 
					prev->linkedHashTableNextIs(c->linkedHashTableNext());
				else 
					buckets_[pos] = c->linkedHashTableNext();
				++version_;
				--members_;
				return r;
			}
			prev = c;
		}
		return 0;
	}
	void deleteAll()	// clear
	{
		for (unsigned int i = 0; i < capacity_; ++i)
		{
			typename T::Ptr c = buckets_[i];
			buckets_[i] = 0;
			if (c)
			{
				for(typename T::Ptr nextC = c->linkedHashTableNext(); nextC; c = nextC) 
				{
					nextC = c->linkedHashTableNext();
					c->linkedHashTableNextIs(0); // Deference next member
				}
			}
		}
		members_ = 0;
		++version_;
	}

	bool isEmpty() const { return members_ == 0; }
	Factor load() const { return load_; }
	inline void loadIs(Factor f);	// rehash?
	unsigned int capacity() const { return capacity_; }
	inline void capacityIs(unsigned int c);	// rehash

private:
	void memberIs(const typename T::Ptr &t) { memberIs(t.ptr()); }
	void memberIs(T *t)
	{
		unsigned int pos = t->hashCode()%capacity_;
		T *node(buckets_[pos].ptr());
		t->linkedHashTableNextIs(node);
		buckets_[pos] = t;
		++members_;
		++version_;
	}
	T *member(unsigned int key) { return const_cast<T *>(const_cast<const LinkedHashTable *>(this)->member(key)); }
	const T *member(unsigned int key) const
	{
		unsigned int pos = key%capacity_;
		T *t(buckets_[pos].ptr());
		for (; t && t->hashCode() != key; t = t->linkedHashTableNext());
		return t;
	}

	typename T::Ptr *buckets_;
	Factor load_;
	unsigned int capacity_;
};
}
