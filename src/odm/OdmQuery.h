#ifndef SFC_ODM_QUERY_H
#define SFC_ODM_QUERY_H

#include "OdmResultSet.h"

namespace Odm
{
class Query
{
public:
	ResultSet resultSet() const { return rs_; }
	void resultSetIs(ResultSet);

private:
	ResultSet rs_;
};
}

#endif