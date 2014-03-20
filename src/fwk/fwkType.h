#pragma once
#include "FwkNamedInterface.h"
#include "FwkAttribute.h"
#include "FwkBasePtrIterator.h"

namespace Fwk
{
/*
Introspection support to allow run-time naming of types.
Each NamedInterface class also includes an attribute �type� that is realized as a
virtual function, returning a pointer to the Fwk::Type associated with this type.
class Foo : public NamedInterface {
public:
	virtual Fwk::Type::PtrConst type() const { return type_; }
private:
	static Fwk::Type::Ptr type_;
};
This code assumes that the Fwk::Type instance is previously instantiated. 
Alternatively, the type function per NamedInterface type can instantiate the 
Fwk::Type on its first call.
*/
class Type : public NamedInterface
{
public:
	/*typedef Fwk::Ptr<Type> Ptr;
	typedef Fwk::Ptr<const Type> PtrConst;*/
	Type::PtrConst baseType() const;
	Attribute::PtrConst attribute(String attrName) const;
	//AttributeIterator attributeIter() const;
	BasePtrIterator<Attribute> attributeIter() const;
	virtual Fwk::Type::PtrConst type() const { return this; }
};
}
