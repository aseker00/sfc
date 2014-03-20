#ifndef SFC_CLC_ARRAY_H
#define SFC_CLC_ARRAY_H

#include "../fwk/FwkRangeException.h"

namespace Clc
{

template<class T>
class Array
{
public:
	Array(unsigned int size) : capacity_(size) { t_ = new T[size]; }
	~Array() { deleteAll(); delete t_; }
	unsigned int capacity() const { return capacity_; }
	/*void operator[](unsigned int i, const T &newMember) { 
		if (i >= capacity_) 
			throw Fwk::RangeException("array index out of bounds", "Dsa::Array::operator[](int, const T &)"); 
		t_[i] = newMember; 
	}*/
	const T &operator[](unsigned int i) {
		if (i >= capacity_) 
			throw Fwk::RangeException("array index out of bounds", "Dsa::Array::operator[](int)"); 
		return t_[i];
	}
	T &operator[](unsigned int i) {
		if (i >= capacity_) 
			throw Fwk::RangeException("array index out of bounds", "Dsa::Array::operator[](int)"); 
		return t_[i];
	}
	void deleteAll() 
	{
		for (int i = 0; i < capacity_; ++i)
			t_[i] = 0;
		members_ = 0;
		++version_;
	}

	T* t_;
	unsigned int capacity_;
};
}

#endif