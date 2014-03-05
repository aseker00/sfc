#ifndef SFC_FWK_ATTRIBUTE_ID_H
#define SFC_FWK_ATTRIBUTE_ID_H

namespace fwk {
class AttributeId {
public:
	AttributeId(unsigned value);
	AttributeId(const AttributeId &other) :
			value_(other.value_) {
	}
	AttributeId &operator=(const AttributeId &other) {
		value_ = other.value_;
		return *this;
	}
	bool operator==(const AttributeId &other) const {
		return value_ == other.value_;
	}
	unsigned int maxLength() const {
		return 128;
	}
	int hash() const;

private:
	unsigned int value_;
};
}

#endif
