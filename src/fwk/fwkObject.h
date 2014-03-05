#ifndef SFC_FWK_OBJECT_H
#define SFC_FWK_OBJECT_H

#include "fwkPtrInterface.h"

namespace fwk
{
/*
Provide a derived type of Object for each object type T, with the derived
type incorporating a data member of type T::Ptr. The �attribute� operations are
overriding to take the appropriate action on the designated attribute of the associ-
ated type, assuming a valid attributeId is specified, and the wrapped index value
and new value (specified as Object::PtrConst) are valid.
With this support, a scripting interface can specify an object by name, use the
name to map to a pointer to the instance and instantiate a generic wrapper for
the instance. It can then perform operations on the instance through this generic
wrapper, allowing the attribute and its parameters to be specified by character
string.
This generic interface with its lack of static type checking means that additional
exceptions can arise. In particular, a client can specify a non-existent attribute. I
have used exceptions in this circumstances because they arise primarily as a result
of programming error, and because the generic interface is not used normally in
destructors or exception processing.
*/
class Object : public PtrInterface<Object> {
public:
	typedef fwk::Ptr<const Object> PtrConst;
	typedef fwk::Ptr<Object> Ptr;

	virtual AttributeId attributeId(String attrName) const;
	virtual Object::Ptr attribute(AttributeId id, const Object::PtrConst &o) const;
	virtual Object::Ptr attributeIs(AttributeId id, const Object::PtrConst &a, const Object::Ptr &o) const;
	virtual Object::Ptr attributeDel(AttributeId id, const Object::PtrConst &a);
};
}
}
