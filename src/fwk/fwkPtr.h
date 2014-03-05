#ifndef SFC_FWK_PTR_H
#define SFC_FWK_PTR_H

namespace fwk {
/*
 The Ptr class template given below extends basic pointer semantics to call newRef
 on creation of a pointer and deleteRef on destruction of a pointer, relying on the
 automatic calls to this destructor when the pointer goes out of scope.
 This class template also prohibits pointer arithmetic on its values, an error-prone
 aspect of raw pointers.
 Works with any class T that defines newRef and deleteRef member functions.
 The class T should define a nested type that is to be used as a smart pointer to it:
 class Foo : public NamedInterface<Foo>
 {
 public:
 typedef ::Ptr<Foo> Ptr;
 };
 Then, the client source code simply contains:
 Foo::Ptr foo;
 */
template<class T>
class Ptr {
public:
	/*
	 The constructor provides a default argument of 0 to allow arrays of
	 (smart) pointers to be initialized, the zero value indicating a null pointer.
	 When initialized or assigned a non-zero pointer, it calls newRef on the specified object.
	 */
	Ptr(T *ptr = 0) :
			ptr_(ptr) {
		if (ptr_)
			ptr_->newRef();
	}
	Ptr(const Ptr<T> &other) :
			ptr_(other.ptr_) {
		if (ptr_)
			ptr_->newRef();
	}

	/*
	 The destructor calls deleteRef on the object pointed to by the raw pointer field
	 ptr , if it is non-null.
	 */
	~Ptr() {
		if (ptr_)
			ptr_->deleteRef();
	}

	const Ptr<T> &operator=(const Ptr<T> &other) {
		T *save = ptr_;
		ptr_ = other.ptr_;
		if (ptr_)
			ptr_->newRef();
		if (save)
			save->deleteRef();
		return *this;
	}
	const Ptr<T> &operator=(T *p) {
		T *save = ptr_;
		ptr_ = p;
		if (ptr_)
			ptr_->newRef();
		if (save)
			save->deleteRef();
		return *this;
	}
	bool operator==(const Ptr<T> &other) const {
		return other.ptr_ == ptr_;
	}
	int operator!=(const Ptr<T> &other) const {
		return other.ptr_ != ptr_;
	}

	/*
	 The operator-> is overloaded to allow the member functions of T to be
	 called without explicit conversion of the Ptr type to the raw pointer type
	 */
	const T *operator->() const {
		return ptr_;
	}
	T *operator->() {
		return ptr_;
	}
	T *ptr() const {
		return ptr_;
	}

	/*
	 The conversion operator supports automatic conversion from a const to non-const
	 Ptr and from a Ptr to the derived class to that of the base class.
	 */
	template<class OtherType>
	operator Ptr<OtherType>() const {
		return Ptr<OtherType>(ptr_);
	}

	/*
	 Conversion to a pointer to a nested class to allow the conventional coding
	 if( a ) { // Smart pointer is non-null
	 where a is a smart pointer. This convoluted conversion operator avoids the dangers
	 of a conversion directly to bool, which causes the compiler to accept undesirable
	 expressions, such as the addition of two smart pointers.
	 */
	struct PointerConversion {
		int valid;
	};
	operator int PointerConversion::*() const {
		return ptr_ ? &PointerConversion::valid : 0;
	}

private:
	T *ptr_;
};
}

#endif
