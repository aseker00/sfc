#ifndef SFC_FWK_STRING_CONTENTS_H
#define SFC_FWK_STRING_CONTENTS_H

#include "FwkPtrInterface.h"
#include "FwkRangeException.h"
#include <stdlib.h>

namespace Fwk {
/*
 Shared/named descriptions are allocated using a manager structure.
 Thus each manager can implement and manage a memory pool associated with the descriptions that it implements.
 Generally, focusing on the key goal of containing a module to some limited amount of memory, it seems feasible to pass
 the designated memory pool as a parameter to a manager and then have objects created within this memory pool.
 */

/*
 * Char is the underlying character type - char or wchar_t
 * size is 1 for char (1 byte) or 2 for wchar_t
 */
template <typename Char, int size, int cmpfunc(const Char *, const Char *)>
class StringContents : public PtrInterface<StringContents<Char, size, cmpfunc>> {
public:
	typedef Fwk::Ptr<const StringContents> PtrConst;
	typedef Fwk::Ptr<StringContents> Ptr;

	class Manager: public PtrInterface<Manager> {
	friend class String;
	public:
		typedef Fwk::Ptr<const Manager> PtrConst;
		typedef Fwk::Ptr<Manager> Ptr;

	protected:
		Manager() : strings_(0), strBytes_(0), strBlocks_(0) {}
		//virtual const PtrConst &definition(const char *cp, unsigned int len, bool clientCall = true) = 0;
		typename StringContents::PtrConst definition(const Char *cp, unsigned int len) {
			const StringContents *sc = new StringContents(cp, len);
			return StringContents::PtrConst(sc);
		}

	private:
		unsigned int strings_;
		unsigned int strBytes_;
		unsigned int strBlocks_;
	};

	bool equality(const StringContents &other) const {
		return length_ == other.length_ && cmpfunc(cp_, other.cp_) == 0;
	}
	unsigned int length() const {
		return length_;
	}
	const Char charAt(unsigned int index) const {
		if (index >= length_)
			throw RangeException("index >= length", __FUNC__);
		return cp_[index];
	}
	const Char *data() const {
		return cp_; 
	}

private:
	/*
	 * note1: because the use of memcpy this is valid implementation only for single byte char.
	 * note2: i think i fixed this by adding Char and size template parameters.
	 */
	StringContents(const Char *cp, unsigned int len) : cp_(0), length_(len) {
		if (length_ > 0) {
			cp_ = new Char[length_];
			memcpy_s((void *)cp_, length_*size, (const void *)cp, (unsigned long)length_*size);
		}
	}

	Char *cp_;
	unsigned int length_;
};
}

#endif