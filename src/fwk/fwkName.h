#ifndef SFC_FWK_NAME_H
#define SFC_FWK_NAME_H

#include "fwkString.h"

namespace fwk {
class Name {
public:
	Name(String value);
	Name(const Name &other) :
			value_(other.value_) {
	}
	Name &operator=(const Name &other) {
		value_ = other.value_;
		return *this;
	}
	bool operator==(const Name &other) const {
		return value_ == other.value_;
	}
	unsigned int maxLength() const {
		return 128;
	}
	int hash() const;

private:
	String value_;
};
}

#endif
