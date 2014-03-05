#ifndef SFC_FWK_STRING_CONTENTS_H
#define SFC_FWK_STRING_CONTENTS_H

#include "fwkPtrInterface.h"
#include "fwkRangeException.h"
#include <cstring>
#include <stdlib.h>

namespace fwk {
/*
 Shared/named descriptions are allocated using a manager structure.
 Thus each manager can implement and manage a memory pool associated with the descriptions that it implements.
 Generally, focusing on the key goal of containing a module to some limited amount of memory, it seems feasible to pass
 the designated memory pool as a parameter to a manager and then have objects created within this memory pool.
 */
class StringContents: public PtrInterface<StringContents> {
public:
	typedef fwk::Ptr<const StringContents> PtrConst;
	typedef fwk::Ptr<StringContents> Ptr;

	class Manager: public PtrInterface<StringContents::Manager> {
		friend class String;
	public:
		typedef fwk::Ptr<const Manager> PtrConst;
		typedef fwk::Ptr<Manager> Ptr;

	protected:
		Manager() :
				strings_(0), strBytes_(0), strBlocks_(0) {
		}
		//virtual const PtrConst &definition(const char *cp, unsigned int len, bool clientCall = true) = 0;
		StringContents::PtrConst definition(const char *cp, unsigned int len) {
			const StringContents *sc = new StringContents(cp, len);
			return StringContents::PtrConst(sc);
		}

	private:
		unsigned int strings_;
		unsigned int strBytes_;
		unsigned int strBlocks_;
	};

	bool equality(const StringContents &other) const {
		return length_ == other.length_ && strcmp(cp_, other.cp_) == 0;
	}
	unsigned int length() const {
		return length_;
	}
	const char charAt(unsigned int index) const {
		if (index >= length_)
			throw RangeException("", "");
		return cp_[index];
	}

private:
	/*
	 * note: because the use of memcpy this is valid implementation only for single byte char.
	 */
	StringContents(const char *cp, unsigned int len) :
			cp_(0), length_(len) {
		if (length_ > 0) {
			cp_ = new char[length_];
			//strcpy_s(cp_, length_, cp);
			memcpy((void *)cp_, (const void *)cp, (unsigned long)length_);
		}
	}

	char *cp_;
	unsigned int length_;
};
}

#endif
