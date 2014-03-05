#ifndef SFC_FWK_BASE_PTR_ITERATOR_H
#define SFC_FWK_BASE_PTR_ITERATOR_H

#include "fwkPtr.h"

namespace fwk
{
class BasePtrCollection;

template<class T>
class BasePtrIterator : public Ptr<T> 
{
public:
	//BasePtrIterator(const BasePtrCollection *collection, T *t) : Ptr<T>(t), collection_(collection) {}
	BasePtrIterator(BasePtrCollection *collection, T *t) : Ptr<T>(t), collection_(collection) {}
	BasePtrIterator<T> &operator=(const BasePtrIterator<T> &other) { collection_ = other.collection_; ptrIs(other.ptr()); return *this; }
	bool operator==(const BasePtrIterator<T> &other) const { return (other.ptr() == ptr()) && (other.collection_ == collection_); }
	bool operator!=(const BasePtrIterator<T> &other ) const { return !(this == other); }
	bool operator==(const Ptr<T> &p) const { return (p.ptr() == ptr()); }
	bool operator!=(const Ptr<T> &p) const { return !(this == p); }
	//using Ptr<T>::ptr;

protected:
	void ptrIs(T *t) { Ptr<T>::operator=(t); }

	//const BasePtrCollection *collection_;
	BasePtrCollection *collection_;
};
}

#endif
