#ifndef SFC_ODM_VALUE_H
#define SFC_ODM_VALUE_H

#include "OdmType.h"

namespace Odm
{
template<typename V, Type T>
class Value
{
public:
	Value(V v) : value_(v) { type_.valid(v); }
private:
	T<V> type_;
	V value_;
};
}

#endif