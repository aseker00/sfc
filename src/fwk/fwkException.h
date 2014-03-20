#ifndef SFC_FWK_EXCEPTION_H
#define SFC_FWK_EXCEPTION_H

namespace Fwk
{
class Exception
{
public:
	Exception(const char *what, const char *where) : what_(what), where_(where) {}
	const char *what() const { return what_; }
	const char *where() const { return where_; }

protected:
	const char *what_;
	const char *where_;
};
}

#endif
