#ifndef SFC_FWK_REF_COUNT_H
#define SFC_FWK_REF_COUNT_H

namespace fwk {
/*
 Can provide atomic increment and decrement and protection against simultaneous update
 to support concurrency execution environment
 */
class RefCount {
public:
	RefCount(unsigned long count) {
		count_ = count;
	}
	RefCount &operator++() {
		++count_;
		return *this;
	}
	RefCount &operator--() {
		--count_;
		return *this;
	}
	bool operator==(const RefCount &other) const {
		return count_ == other.count_;
	}
private:
	unsigned long count_;
};
}

#endif
