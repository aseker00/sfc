#ifndef SFC_FWK_STRING_H
#define SFC_FWK_STRING_H

#include "FwkStringContents.h"
#include "FwkNullPointerException.h"
#include "FwkRangeException.h"
#include "../clc/ClcArray.h"
#include <string.h>

namespace Fwk {

class String {
public:
	typedef Clc::Array<String> StringArray;

	String(const char *cp = "") {
		if (!cp)
			throw NullPointerException("char pointer is null", __FUNC__);
		unsigned int len = strlen(cp);
		definition_ = strContentsMan_->definition(cp, len);
	}
	String(const char *cp, unsigned int len) {
		if (!cp)
			throw NullPointerException("char pointer is null", __FUNC__);
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
	
	// substr - the sub-string to find
	// pos - the position in the searched (this) string to start searching from. if 0 then search the entire string.
	// n - the number of characters from the sub-string to match
	int find(const String &substr, unsigned int pos, unsigned int n) const {
		if (!n)
			return pos;
		auto const size = this->length();
		if (pos + n > size || pos + n < pos)
			throw RangeException("overflow", __FUNC__);
		
		// Use a Boyer-Moore-like trick by comparing the last characters first
		auto const data = this->definition_->data();
		auto const n_1 = substr.length()-1;
		auto const c_1 = substr.charAt(n_1);
		
		// Boyer-Moore skip value for the last char in the substr. 
		// Zero is not a valid value; skip will be computed the first time it's needed.
		unsigned int skip = 0;
		const char *cp = data + pos;
		auto last = data + size - n_1;
		while (cp < last) {
			// Boyer-Moore: match the last element in the needle
			while (cp[n_1] != c_1) {
				if (++cp == last) {
					// not found
					return -1;
				}
			}
			// Here we know that the last char matches - continue in pedestrian mode
			for (unsigned int j = 0; ; ) {
				//assert(j < lastSubIndex);
				if (j >= n)
					throw RangeException("j >= n", __FUNC__);
				if (cp[j] != substr.charAt(j)) {
					// Not found, we can skip
					// Compute the skip value lazily
					if (skip == 0) {
						skip = 1;
						while (skip <= n_1 && substr.charAt(n_1-skip) != c_1) {
							++skip;
						}
					}
					cp += skip;
					break;
				}
				// Check if done searching
				if (++j == n) {
					return cp - data;
				}
			}
		}
		return -1;
	}
	
	String substr(int start, int n) const {
		const char *cp = definition_->data();
		return String(cp+start, n);
	}
	
	StringArray split(const char *delim) const;
	String join(const char *delim, StringArray tokens) const;

private:
	StringContents<char, 1, strcmp>::PtrConst definition_;
	StringContents<char, 1, strcmp>::Manager::Ptr strContentsMan_;
};
}

#endif
