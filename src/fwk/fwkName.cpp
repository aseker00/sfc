#include "fwkName.h"
#include "fwkRangeException.h"

namespace fwk {

Name::Name(String value) {
	if (value_.isEmpty())
		throw fwk::RangeException("", "");
	if (value_.length() > maxLength())
		throw fwk::RangeException("", "");
	if (value_.charAt(0) < 'A' || value_.charAt(0) > 'Z')
		throw fwk::RangeException("", "");
	value_ = value;
}

}
