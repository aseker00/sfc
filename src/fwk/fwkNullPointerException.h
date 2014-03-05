#ifndef SFC_FWK_NULL_POINTER_EXCEPTION_H
#define SFC_FWK_NULL_POINTER_EXCEPTION_H

#include "fwkException.h"

namespace fwk
{
class NullPointerException : public Exception
{
public:
	NullPointerException(const char *what, const char *where) : Exception(what, where) {}
};
}

#endif
