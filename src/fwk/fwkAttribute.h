#ifndef SFC_FWK_ATTRIBUTE_H
#define SFC_FWK_ATTRIBUTE_H

#include "FwkNamedInterface.h"

namespace Fwk
{
/*
In the general (unstructured) case for C++, complete introspection support means
providing run-time information about every program element, including classes,
functions, data members, etc. However, with our attribute-structure software and
strict definition of interface modules, we just need to provide type information for
these NamedInterface types and their attributes.
*/
class Attribute : public NamedInterface 
{
public:
	//typedef Fwk::Ptr<Attribute> Ptr;
	//
	typedef Fwk::Ptr<const Attribute> PtrConst;
	//Flags flags() const;
	Attribute::PtrConst indexAttr() const;
};
}

#endif
