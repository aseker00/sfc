#ifndef SFC_FWK_VERSION_EXCEPTION_H
#define SFC_FWK_VERSION_EXCPETION_H

#include "FwkException.h"

namespace Fwk
{
class VersionException : public Exception
{
public:
	VersionException(const char *what, const char *where) : Exception(what, where) {}
};
}

#endif
