#ifndef SFC_ODM_TYPE_H
#define SFC_ODM_TYPE_H

#include "../Fwk/FwkName.h"

namespace Odm
{
template <typename V>
class Type
{
public:
	Type(Fwk::Name n, unsigned int s = 0) : name_(n), size_(s) {}
	Fwk::Name name() const { return name_; }
	unsigned int size() const { return size_; }
	boolean valid(V v) { return true; }

private:
	Fwk::Name name_;
	unsigned int size_;
};
}

#endif