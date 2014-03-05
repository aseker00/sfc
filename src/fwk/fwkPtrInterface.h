#ifndef SFC_FWK_PTR_INTERFACE_H
#define SFC_FWK_PTR_INTERFACE_H

#include "fwkPtr.h"
#include "fwkRefCount.h"

namespace fwk {
/*
 Defines a common interface for reference management of objects.
 Using PtrInterface:
 If Foo is an interface class, it is defined as:
 class Foo : public PtrInterface<Foo>
 This interface class or its implementation class can override onZeroReferences as necessary to
 provide class-specific behavior.
 */
template<class T>
class PtrInterface {
public:
	/*	
	 The reference count is initialized to 1 rather than 0 to avoid problems with the
	 reference count going to zero during object construction. For instance, if an exception
	 is thrown during construction and a sub-object is deleted causes it to dereference
	 the �this� object, the reference count would otherwise go to zero, causing a call to
	 onZeroReferences, causing a call the destructor in the midst of destructing the
	 constructor. As a consequence, the reference count of the object is 1 on normal exit
	 from the constructor. Consequently, one can wrap the constructor with a so-called
	 factory function that decrements the reference count and then returns the pointer,
	 as suggested for some class Foo below:
	 static Foo::Ptr Foo::FooIs( args ) {
	 Foo * m = new Foo( args );
	 m->referencesDec(1); // refcount is now 0.
	 return m;
	 }
	 */
	PtrInterface() :
			references_(1) {
	}

	/*
	 The simple public function references is provided to check the number of references
	 to this object, to determine whether it is shared and for auditing the object. Ideally,
	 the software is structured so there is only one real smart pointer reference to the
	 instance per module, making an instance shared if the count is greater than 1.
	 */
	RefCount references() const {
		return references_;
	}

	/* 
	 Indicates there is a new reference to the object.
	 */
	void newRef() const {
		PtrInterface<T> *me = (PtrInterface<T> *) this;
		++me->references_;
		//return this;
	}

	/* 
	 Indicates that a reference has been removed.
	 */
	void deleteRef() const {
		PtrInterface<T> *me = (PtrInterface<T> *) this;
		if (--me->references_ == 0)
			me->onZeroReferences();
	}

	/*
	 The smart pointer implementation needs to be concurrency-safe because any number
	 of threads/activities can hold a reference to an object. The non-blocking primitive
	 CAS or compare-and-swap is particularly attractive to use
	 */
	void newRefC() const {
		PtrInterface<T> *me = (PtrInterface<T> *) this;
		while (!CAS(&me->references_, me->references_, me->references_ + 1))
			;
		//return this;
	}

	/*
	 In this implementation, we assume that a reference count is one only when at most
	 one activity can possibly have a pointer to the object. Therefore, testing the count
	 after the decrement ensures that this is the thread that decrements it to zero. Testing
	 the references field for zero after the decrement in deleteRef could lead to two
	 or more threads seeing a zero count.
	 Considering the trick of having the reference count go to zero to notify a directory,
	 when the count is actually one still works fine provided that the directory atomically
	 removes the object from the directory continguent on the reference count still being
	 0, and otherwise skips the deletion. This handles the case of a concurrent lookup in
	 the directory accessing the object between the time its reference count goes to zero
	 and the onZeroReferences call being executed
	 */
	void deleteRefC() const {
		PtrInterface<T> *me = (PtrInterface<T> *) this;
		unsigned int count = references_;
		while (!CAS(&me->references_, me->references_, me->references_ - 1)) {
			count = references_;
		}
		/* 
		 The onZeroReferences call is only executed by a single thread so is never concurrent.
		 However, it needs to be treated like a notification if it accesses any shared data
		 structures which other threads can be accessing.
		 */
		if (count == 1)
			me->onZeroReferences();
	}

	/* 
	 In some cases, an aggregate object such as a manager needs to maintain a pointer
	 to an object that it manages but also be notified when all the client references go away.
	 One approach is to add a second event handler, say onOneReference, that is
	 called when the reference count goes to 1. However, this would add extra overhead
	 for a case that relatively rarely arises. Instead, my approach has been to have the
	 manager override onZeroReferences and make an explicit call to deleteRef on
	 the object so that its own reference is not counted.
	 */
	void referencesDec(unsigned int d) {
		if ((references_ -= d) == 0)
			onZeroReferences();
	}

protected:
	/* 
	 The PtrInteface destructor is declared as protected so that the client cannot
	 directly call delete on an instance and circumvent the reference count mechanism.
	 I.e. the decision to delete should be controlled by the reference count.
	 However, to be effective, any derived class of an instance of PtrInterface must also declare its
	 destructor to be protected so the client cannot call that destructor directly either.
	 */
	virtual ~PtrInterface() {
	}

private:
	//friend class Ptr<T>;

	/*
	 The default action of onZeroReferences is to delete this
	 object. (Note: the instance is assumed to have been allocated using operator
	 new, not on the stack and not with operator new[], the array version of the new
	 operator.) However, onZeroReferences can be overridden to take a class-specific
	 action. For example, an object that is logically part of a cache may simply flag itself
	 as not in use and only delete itself after it has been inactive for some period of time
	 or if there is a demand for memory, i.e. deferred deletion providing undo/roleback capability.
	 It may also log the event of its references going to zero.
	 */
	virtual void onZeroReferences() const {
		delete this;
	}

	bool CAS(volatile unsigned int *ptr, unsigned int newValue, unsigned int oldValue) {
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 1050
		return OSAtomicCompareAndSwapPtr (oldValue, newValue, ptr);
#elif defined(_MSC_VER)
		return InterlockedCompareExchange(ptr, newValue, oldValue);
#elif (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) > 40100
		return __sync_val_compare_and_swap(ptr, oldValue, newValue);
#else
#  error No implementation
#endif
	}

	RefCount references_;
};
}

#endif
