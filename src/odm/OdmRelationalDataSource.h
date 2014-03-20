#ifndef SFC_ODM_RELATIONAL_DATA_SOURCE_H
#define SFC_ODM_RELATIONAL_DATA_SOURCE_H

#include "OdmDataSource.h"

namespace odm {
template<class T>
class RelationalDataSource : public DataSource<T> {
};
}
#endif