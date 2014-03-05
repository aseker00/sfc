#ifndef SFC_FWK_STRING_H
#define SFC_FWK_STRING_H

#include "fwkStringContents.h"
#include "fwkNullPointerException.h"

namespace fwk {
class String {
public:
	String(const char *cp = "") {
		if (!cp)
			throw NullPointerException("", "");
		unsigned int len = strlen(cp);
		definition_ = strContentsMan_->definition(cp, len);
	}
	String(const char *cp, unsigned int len) {
		if (!cp)
			throw NullPointerException("", "");
		definition_ = strContentsMan_->definition(cp, len);
	}
	String(const String &other) {
		definition_ = other.definition_;
	}
	String &operator=(const String &other) {
		definition_ = other.definition_;
		return *this;
	}
	bool operator==(const String &other) const {
		if (definition_ == other.definition_)
			return true;
		return definition_.ptr()->equality(*other.definition_.ptr());
	}
	bool isEmpty() const {
		return definition_->length() > 0;
	}
	const char charAt(unsigned int index) const {
		return definition_->charAt(index);
	}
	unsigned int length() const {
		return definition_->length();
	}

private:
	StringContents::PtrConst definition_;
	StringContents::Manager::Ptr strContentsMan_;
};
}

#endif
