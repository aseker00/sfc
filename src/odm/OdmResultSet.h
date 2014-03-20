#ifndef SFC_ODM_RESULT_SET_H
#define SFC_ODM_RESULT_SET_H

#include "OdmRelation.h"

namespace Odm
{
class ResultSet
{
public:
	void tupleAdd(Relation::Tuple t) {
		tuples_->addTuple(
	}
	Relation::Tuple tuple(unsigned int i);
	Relation::Body::Tuples::IteratorConst iteratorConst() const { return tuples_.iteratorConst(); }

private:
	Relation::Ptr tuples_;
};
}

#endif