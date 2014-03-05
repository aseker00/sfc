#ifndef SFC_FWK_RANGE_EXCEPTION_H
#define SFC_FWK_RANGE_EXCEPTION_H

#include "fwkException.h"

namespace fwk
{
class RangeException : public Exception
{
public:
	RangeException(const char *what, const char *where) : Exception(what, where) {}
};
}

#endif
