#ifndef SFC_FWK_BASE_PTR_COLLECTION_H
#define SFC_FWK_BASE_PTR_COLLECTION_H

namespace fwk
{
/*
	Provide a base type for iterator support with invasive collections.
*/
class BasePtrCollection
{
public:
	BasePtrCollection() : members_(0), version_(0) {}
	unsigned int members() { return members_; }
	unsigned int version() { return version_; }
	template<class T, class Tool>
	void apply(Tool *tool) 
	{
		T *me = reinterpret_cast<T *>(this);
		T::Iterator ii = me->iterator();
		for(; ii; ++ii) tool->applicant(ii.ptr());
	}
protected:
	unsigned int members_;
	unsigned int version_;
};
}

#endif
