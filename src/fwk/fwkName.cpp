#include "FwkName.h"
#include "FwkRangeException.h"

namespace Fwk {

Name::Name(String value) {
	if (value_.isEmpty())
		throw RangeException("value is empty", __FUNC__);
	if (value_.length() > maxLength())
		throw RangeException("value length over max length > ", __FUNC__);
	if (value_.charAt(0) < 'A' || value_.charAt(0) > 'Z')
		throw RangeException("first letter not capitalized", __FUNC__);
	value_ = value;
}

}
