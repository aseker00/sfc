#pragma once
#include "FwkRangeException.h"

namespace Fwk
{
class Factor
{
public:
	inline Factor(double value) { if (value < 0.0 || value > 1.0) throw RangeException("value has to be between 0 and 1", __FUNC__); }

private:
	double value_;
};
}