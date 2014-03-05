#ifndef SFC_FWK_VALID_PTR_H
#define SFC_FWK_VALID_PTR_H

#include "fwkRangeException.h"

namespace fwk
{
/*
It is attractive to have a pointer type that ensures it is a valid instance (as opposed
to null), so you can check once that a pointer is valid and use it safely in various
places, relying on the type system to ensure that it is the case and to document
this assumption.

First, define a private nested derived class of the interface class called
DefaultInstance that overrides the pure virtual functions of the interface class
to perform whatever action you select, typically throw an exception when called.
This class should also override onZeroReferences to do nothing, so the instance is
never deleted. The constructor should initialize all the data members to appropri-
ate default values, if not provided by the data member type. Moreover, if it has
any data members that are pointers to T, the constructor should explicitly initialize
them to �this�, so the construction of the static default instance happens correctly.
This class can be defined in the implementation file for the interface. (This approach
thus requires an implementation file for each interface class (which can be smart-
pointed at) but this is required in other cases as well, such as capturing interface
invariants in the audit function.)
Second, this implementation class should define a static data member
defaultInstance that is an instance of this derived class.
static DefaultInstance defaultInstance_;
All of the above should be declared and defined in the implementation file!
The public static function defaultPtrValue return a smart pointer to this instance:
ValidPtr<Foo> Foo::defaultPtrValue() { return &defaultInstance_; }
As an optimization for simple classes without pure virtual functions, the default
pointer value can be a static instance of the interface class itself, eliminating the
separate DefaultInstance derived class.
Compile-time cannot check that default pointer values �escape� outside
of object construction. However, audit code can check for this case.
*/
template <class T>
class ValidPtr : public Ptr<T> {
public:
	/*
	Each type T used with PtrInterface defines a static member function defaultPtrValue that
	returns a unique ValidPtr for that type, independent of the interface implementation, that is actually
	an instance of T in the sense of having a valid virtual function table pointer and
	providing data members for the read accessors.
	Any class audit should check that pointers are not the default pointer value after construction.
	*/
	ValidPtr() : ptr_(T::defaultPtrValue().ptr()) { }
	ValidPtr(T* ptr) : Ptr<T>(ptr) { if(!ptr) throw RangeException("null ValidPtr"); }
	ValidPtr(const Ptr<T>& ptr) : Ptr<T>(ptr) { if(!ptr) throw RangeException("null ValidPtr"); }

	/*
	The assignment operator is optimized over the general smart pointer assignment
	because it does not have to check for null.
	*/
	ValidPtr<T>& operator=(const ValidPtr<T> &p);

	template<typename OtherType>
	operator ValidPtr<OtherType>() const { return ValidPtr<OtherType>(Ptr<T>::ptr()); }
};

template<typename T>
ValidPtr<T> &ValidPtr<T>::operator=(const ValidPtr<T> &mp)
{
	T *save = Ptr<T>::rawPtr_;
	Ptr<T>::rawPtr_ = mp.rawPtr_;
	Ptr<T>::rawPtr_->newRef();
	save->deleteRef();
	return *this;
}
}

#endif
