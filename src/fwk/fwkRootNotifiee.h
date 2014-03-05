#ifndef SFC_FWK_ROOT_NOTIFIEE_H
#define SFC_FWK_ROOT_NOTIFIEE_H

#include "fwkPtrInterface.h"

namespace fwk
{
class Activity;
/*
Used to provide a common base class for manipulation by framework functions, 
including processing resources.
*/
class RootNotifiee : public PtrInterface<RootNotifiee> 
{
public:
	/*
	There are a variety of attributes you can provide in this class, depending on the
	infrastructure support. For instance, the handleNotification function is provided
	to support scheduled notifications using activities.
	*/
	virtual void handleNotification(Activity *) {}
};
}

#endif
