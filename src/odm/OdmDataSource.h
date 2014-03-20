#ifndef SFC_ODM_DATA_SOURCE_H
#define SFC_ODM_DATA_SOURCE_H

namespace odm {
template<class T>
class DataSource {
public:
	virtual void objectAdd(T::PtrConst t) = 0;
	T::Ptr object
};
}
#endif