#ifndef SFC_FWK_ATTRIBUTE_H
#define SFC_FWK_ATTRIBUTE_H

#include "fwkNamedInterface.h"

namespace fwk
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
	//Flags flags() const;
	Attribute::PtrConst indexAttr() const;
};
}

#endif
