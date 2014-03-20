#ifndef SFC_ODM_ATTRIBUTE_H
#define SFC_ODM_ATTRIBUTE_H

#include "OdmType.h"

namespace Odm
{
template<Type T>
class Attribute : public Fwk::PtrInterface<Attribute>
{
public:
	typedef Fwk::Ptr<const Attribute> PtrConst;
	typedef Fwk::Ptr<Attribute> Ptr;

	Attribute *linkedBinaryTreeLeft() const { return left_; }
	void linkedBinaryTreeLeftIs(Attribute *a) { left_ = a; }
	Attribute *linkedBinaryTreeRight() const { return right_; }
	void linkedBinaryTreeRightIs(Attribute *a) { right_ = a; }
	Attribute *linkedHashTableNext() const { return next_; }
	void linkedHashTableNextIs(Attribute *a) { next_ = a; }
	Fwk::Name name() const { return name_; }
	Fwk::Name typeName() const { return type_.name(); }
	unsigned int hashCode() { return 0; }

private:
	Attribute *left_, *right_, *next_;
	Fwk::String name_;
	T type_;
};
}

#endif